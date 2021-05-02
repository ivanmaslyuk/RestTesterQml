#include "ServerSyncService.h"

#include <algorithm>

#include <QJsonArray>
#include <QScopedPointer>

#include "ServerApi.h"
#include "Models/requesttreenode.h"
#include "NodeSerializer.h"
#include "Error.h"

ServerSyncService::ServerSyncService(JsonStorage *settings, Authenticator *authenticator)
{
    m_settings = settings;
    m_authenticator = authenticator;
    m_storage = new SQLiteStorage("sync_database_for_service");
    m_storage->moveToThread(this);
}

ServerSyncService::~ServerSyncService()
{
    delete m_storage;
}

void ServerSyncService::run()
{
    ServerApi api(m_settings, m_authenticator);

    // Send sync_check request to server
    QList<RequestTreeNode *> nodes = m_storage->getNodes();
    QJsonObject syncStatus;
    try {
         syncStatus = api.checkSync(nodes);
    } catch (Error e) {
        emit syncError(e.message());
        return;
    }

    // Putting nodes in QScopedPointers so that they are destroyed when out of scope
//    QList<QScopedPointer<RequestTreeNode>> scopedNodes;
//    for (RequestTreeNode *node : nodes)
//        scopedNodes.append(QScopedPointer<RequestTreeNode>(node));

    // Exit if data is already in sync
    if (!syncStatus["out_of_sync"].toBool()) {
        emit syncFinished();
        return;
    }

    // Save new nodes from the server to local database
    QJsonArray newNodes = syncStatus["new"].toArray();
    QList<QString> localUuids;
    for (RequestTreeNode *node : nodes)
        localUuids.append(node->uuid());

    while (!newNodes.isEmpty()) {
        QJsonValue newNode = newNodes.takeAt(0);

        // Don't save the new node before its parent
        QJsonValue parentUuid = newNode["parent"];
        if (!localUuids.contains(parentUuid.toString()) && !parentUuid.isNull()) {
            newNodes.append(newNode);
            continue;
        }

        QScopedPointer<RequestTreeNode> node(NodeSerializer::fromJson(newNode.toObject()));
        qDebug() << "new" << node->uuid();
        m_storage->createNode(node.data(), false);
        localUuids.append(newNode["uuid"].toString());
    }

    // Updated nodes from the server
    for (QJsonValue value : syncStatus["updated"].toArray()) {
        qDebug() << "updated" << value["uuid"].toString();

        int localId;
        try {
            localId = m_storage->getNodeLocalId(value["uuid"].toString());
        } catch (Error e) {
            emit syncError("ServerSyncService: error while downloading updated node from server: " + e.message());
            return;
        }

        int requestLocalId;
        if (!value["isFolder"].toBool()) {
            try {
                requestLocalId = m_storage->getRequestLocalId(value["request"]["uuid"].toString());
            } catch (Error e) {
                emit syncError("ServerSyncService: error while downloading updated node from server: " + e.message());
                return;
            }
        }

        QScopedPointer<RequestTreeNode> node(NodeSerializer::fromJson(value.toObject()));
        node->setLocalId(localId);
        if (!node->isFolder())
            node->request()->setLocalId(requestLocalId);
        m_storage->saveNode(node.data(), false);
    }

    // Send locally updated nodes to server
    for (QJsonValue value : syncStatus["outdated"].toArray()) {
        QString uuid = value.toString();
        qDebug() << "outdated" << uuid;

        QScopedPointer<RequestTreeNode> node(m_storage->getNode(uuid));

        try {
            api.updateNode(node.data());
            node->setPointer(node->pointer() + 1);
            m_storage->updatePointer(node->uuid(), node->pointer());
        } catch (Error e) {
            emit syncError(QString("ServerSyncService: error while uploading updated nodes to server: ") + e.message());
            return;
        }
    }

    // Send new locally created nodes to server
    // Sort first so that child nodes are created last.
    QList<QVariant> unknownUuids = syncStatus["unknown"].toArray().toVariantList();
    QMap<QString, RequestTreeNode *> nodesByUuid;
    for (RequestTreeNode *node : nodes)
        nodesByUuid[node->uuid()] = node;

    std::sort(unknownUuids.begin(), unknownUuids.end(), [&nodesByUuid](QVariant &uuid1, QVariant &uuid2) {
        RequestTreeNode *node2 = nodesByUuid[uuid2.toString()];
        return uuid1 == node2->parentUuid() && !node2->parentUuid().isEmpty();
    });
    for (QVariant value : unknownUuids) {
        QString uuid = value.toString();

        qDebug() << "unknown" << uuid;

        QScopedPointer<RequestTreeNode> node(m_storage->getNode(uuid));

        try {
            api.createNode(node.data());
        } catch (Error e) {
            emit syncError(QString("ServerSyncService: error while uploading new nodes to server: ") + e.message());
            return;
        }
    }

    // Prompt user to solve conflicts
    for (QJsonValue value : syncStatus["conflicts"].toArray()) {
        emit syncConflict(value["uuid"].toString());
    }

    emit syncFinished();
}
