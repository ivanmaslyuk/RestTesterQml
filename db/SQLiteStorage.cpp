#include <QDebug>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlError>
#include "SQLiteStorage.h"
#include "Error.h"


SQLiteStorage::SQLiteStorage(QString connectionName, QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    QString folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (folder.isEmpty())
        qDebug() << "No writable location";

    m_db.setDatabaseName(folder + "/data.sqlite");
    m_db.open();

    if (!m_db.isOpen()) {
        qDebug() << "Could not open database";
    } else {
        QSqlQuery query(m_db);
        query.exec("PRAGMA foreign_keys = ON;");
        handleErrors(query);
    }
}

RequestTreeNode *SQLiteStorage::getRequestTree()
{
    RequestTreeNode *rootNode = new RequestTreeNode(this);
    rootNode->setIsFolder(true);

    // Get nodes from db
    QSqlQuery nodeQuery("SELECT * FROM node WHERE deleted = false ORDER BY is_folder DESC;", m_db);
    QMap<int, RequestTreeNode *> nodeMap;
    while (nodeQuery.next()) {
        int localId = nodeQuery.value("id").toInt();
        RequestTreeNode *node = new RequestTreeNode(localId);
        node->setProperty("parent_id", nodeQuery.value("parent_id"));
        nodeMap[localId] = node;
        node->setUuid(nodeQuery.value("uuid").toString());
        node->setIsFolder(nodeQuery.value("is_folder").toBool());
        node->setFolderName(nodeQuery.value("folder_name").toString());
        node->setDeleted(nodeQuery.value("deleted").toBool());
        node->setEditedAt(QDateTime::fromString(nodeQuery.value("edited_at").toString(), Qt::ISODateWithMs));
        node->setPointer(nodeQuery.value("pointer").toInt());
    }

    // Assign parent nodes
    for (int nodeId : nodeMap.keys()) {
        RequestTreeNode *node = nodeMap[nodeId];

        RequestTreeNode *parentNode = nodeMap.value(node->property("parent_id").toInt(), rootNode);
        node->setParent(parentNode);
        node->setParentUuid(parentNode->uuid());
    }

    // Get requests from db
    QSqlQuery requestQuery("SELECT * FROM request r JOIN node n ON n.id = r.node_id WHERE n.deleted = false;", m_db);
    while (requestQuery.next()) {
        QString queryParamsJson = requestQuery.value("query_params_json").toString();
        QList<ParamModel *> queryParams =
                ParamModel::listFromJson(queryParamsJson);

        QString dataParamsJson = requestQuery.value("data_params_json").toString();
        QList<ParamModel *> dataParams =
                ParamModel::listFromJson(dataParamsJson);

        QString headersJson = requestQuery.value("headers_json").toString();
        QList<ParamModel *> headers =
                ParamModel::listFromJson(headersJson);

        int localId = requestQuery.value("id").toInt();
        RequestTreeNode *node = nodeMap[requestQuery.value("node_id").toInt()];

        Request *request = new Request(localId, node);
        request->setUuid(requestQuery.value("uuid").toString());
        request->setUrl(requestQuery.value("url").toString());
        request->setQueryParams(queryParams);
        request->setDataParams(dataParams);
        request->setHeaders(headers);
        request->setRawData(requestQuery.value("raw_data").toString());
        request->setMethod(requestQuery.value("method").toString());
        request->setName(requestQuery.value("name").toString());
        request->setContentType(requestQuery.value("content_type").toString());
        request->setDocumentation(requestQuery.value("documentation").toString());
        request->setTests(requestQuery.value("tests").toString());
        request->setEdited(false);

        node->setRequest(request);
    }

    return rootNode;
}

void SQLiteStorage::saveNode(RequestTreeNode *node, bool updatedByUser)
{
    if (QList<int>{0,-1}.contains(node->localId()))
        throw Error("QSQLiteStorage::saveNode: Cannot save node without an id");

    if (!node->isFolder() && QList<int>{0, -1}.contains(node->request()->localId()))
        throw Error("QSQLiteStorage::saveNode: Cannot save request without an id");

    QSqlQuery nodeQuery(m_db);
    nodeQuery.prepare("UPDATE node SET folder_name = :folder_name, parent_id = :parent_id, "
                      "pointer = :pointer, edited_at = :edited_at, deleted = :deleted WHERE id = :id;");
    nodeQuery.bindValue(":folder_name", node->folderName());
    nodeQuery.bindValue(":id", node->localId());
    nodeQuery.bindValue(":pointer", node->pointer());
    if (updatedByUser)
        node->setEditedAt(QDateTime::currentDateTime());
    nodeQuery.bindValue(":edited_at", node->editedAt().toString(Qt::ISODateWithMs));
    nodeQuery.bindValue(":deleted", node->deleted());

    // Get information about parent node from parent QObject if possible.
    // Fallback to hasParent() and parentUuid().
    if (node->parent()) {
        RequestTreeNode *parentNode = static_cast<RequestTreeNode *>(node->parent());

        if (!parentNode->isFolder()) {
            qDebug() << "Parent node must be a folder or root node";
            return;
        }

        if (parentNode->localId() != -1)
            nodeQuery.bindValue(":parent_id", parentNode->localId());
        else
            nodeQuery.bindValue(":parent_id", QVariant());
    } else {
        if (node->hasParent()) {
            QSqlQuery parentQuery(m_db);
            parentQuery.prepare("SELECT id FROM node WHERE uuid = :uuid;");
            parentQuery.bindValue(":uuid", node->parentUuid());
            parentQuery.exec();
            handleErrors(parentQuery);

            if (!parentQuery.next())
                throw Error(QString("QSLiteStorage::saveNode: trying to find parent: could not find node with uuid %1")
                            .arg(node->parentUuid()));

            nodeQuery.bindValue(":parent_id", parentQuery.value("id"));
        } else {
            nodeQuery.bindValue(":parent_id", QVariant());
        }
    }

    nodeQuery.exec();
    handleErrors(nodeQuery);

    // Save request
    if (!node->isFolder()) {
        QSqlQuery requestQuery(m_db);
        requestQuery.prepare("UPDATE request SET name = :name, url = :url, raw_data = :raw_data, "
                          "method = :method, content_type = :content_type, documentation = :documentation, "
                          "query_params_json = :query_params_json, data_params_json = :data_params_json, "
                          "headers_json = :headers_json, tests = :tests WHERE id = :id;");
        requestQuery.bindValue(":id", node->request()->localId());
        requestQuery.bindValue(":name", node->request()->name());
        requestQuery.bindValue(":url", node->request()->url());
        requestQuery.bindValue(":raw_data", node->request()->rawData());
        requestQuery.bindValue(":method", node->request()->method());
        requestQuery.bindValue(":content_type", node->request()->contentType());
        requestQuery.bindValue(":documentation", node->request()->documentation());
        requestQuery.bindValue(":query_params_json", ParamModel::listToJson(node->request()->queryParams()));
        requestQuery.bindValue(":data_params_json", ParamModel::listToJson(node->request()->dataParams()));
        requestQuery.bindValue(":headers_json", ParamModel::listToJson(node->request()->headers()));
        requestQuery.bindValue(":tests", node->request()->tests());
        requestQuery.exec();
        handleErrors(requestQuery);
    }
}

void SQLiteStorage::createNode(RequestTreeNode *node, bool createdByUser)
{
    m_db.transaction();

    QSqlQuery nodeQuery(m_db);
    nodeQuery.prepare("INSERT INTO node (uuid, is_folder, pointer, edited_at, deleted) "
                      "VALUES (:uuid, :is_folder, :pointer, :edited_at, :deleted);");
    nodeQuery.bindValue(":uuid", createdByUser ? getUuid() : node->uuid());
    nodeQuery.bindValue(":is_folder", node->isFolder());
    nodeQuery.bindValue(":pointer", node->pointer());
    if (createdByUser)
        node->setEditedAt(QDateTime::currentDateTime());
    nodeQuery.bindValue(":edited_at", node->editedAt().toString(Qt::ISODateWithMs));
    nodeQuery.bindValue(":deleted", node->deleted());
    nodeQuery.exec();
    handleErrors(nodeQuery);

    int nodeId = nodeQuery.lastInsertId().toInt();
    node->setLocalId(nodeId);

    if (!node->isFolder()) {
        QSqlQuery requestQuery(m_db);
        requestQuery.prepare("INSERT INTO request (uuid, node_id, name) VALUES (:uuid, :node_id, :name);");
        requestQuery.bindValue(":uuid", createdByUser ? getUuid() : node->request()->uuid());
        requestQuery.bindValue(":node_id", nodeId);
        requestQuery.bindValue(":name", node->request()->name());
        requestQuery.exec();
        handleErrors(requestQuery);

        int requestId = requestQuery.lastInsertId().toInt();
        node->request()->setLocalId(requestId);
    }

    saveNode(node, createdByUser);

    m_db.commit();
}

int SQLiteStorage::getNodeLocalId(QString uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM node WHERE uuid = :uuid;");
    query.bindValue(":uuid", uuid);
    query.exec();
    handleErrors(query);

    if (query.next())
        return query.value("id").toInt();
    else
        throw Error("SQLiteStorage::getNodeLocalId: node not found");
}

int SQLiteStorage::getRequestLocalId(QString uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM request WHERE uuid = :uuid;");
    query.bindValue(":uuid", uuid);
    query.exec();
    handleErrors(query);

    if (query.next())
        return query.value("id").toInt();
    else
        throw Error("SQLiteStorage::getRequestLocalId: request not found");
}

RequestTreeNode *SQLiteStorage::getNode(QString uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT n.id, n.uuid, n.is_folder, n.folder_name, n.pointer, n.edited_at, n.deleted, "
                  "pn.uuid as parent_uuid FROM node n LEFT JOIN node pn ON n.parent_id = pn.id WHERE n.uuid = :uuid;");
    query.bindValue(":uuid", uuid);
    query.exec();
    handleErrors(query);

    if (!query.next())
        throw Error("SQLiteStorage::getNode(uuid): node not found");

    RequestTreeNode *node = new RequestTreeNode(uuid);
    node->setLocalId(query.value("id").toInt());
    node->setUuid(query.value("uuid").toString());
    node->setIsFolder(query.value("is_folder").toBool());
    node->setFolderName(query.value("folder_name").toString());
    node->setPointer(query.value("pointer").toInt());
    node->setEditedAt(QDateTime::fromString(query.value("edited_at").toString(), Qt::ISODateWithMs));
    node->setDeleted(query.value("deleted").toBool());
    node->setParentUuid(query.value("parent_uuid").toString());

    if (!node->isFolder()) {
        QSqlQuery requestQuery(m_db);
        requestQuery.prepare("SELECT * FROM request WHERE node_id = :node_id;");
        requestQuery.bindValue(":node_id", node->localId());
        requestQuery.exec();
        handleErrors(requestQuery);

        if (!requestQuery.next())
            throw Error(QString("SQLiteStorage::getNode(): request not found for node with uuid %1")
                        .arg(node->uuid()));

        Request *request = new Request(node);

        QString queryParamsJson = requestQuery.value("query_params_json").toString();
        QList<ParamModel *> queryParams = ParamModel::listFromJson(queryParamsJson);

        QString dataParamsJson = requestQuery.value("data_params_json").toString();
        QList<ParamModel *> dataParams = ParamModel::listFromJson(dataParamsJson);

        QString headersJson = requestQuery.value("headers_json").toString();
        QList<ParamModel *> headers = ParamModel::listFromJson(headersJson);

        request->setUuid(requestQuery.value("uuid").toString());
        request->setLocalId(requestQuery.value("id").toInt());
        request->setUrl(requestQuery.value("url").toString());
        request->setQueryParams(queryParams);
        request->setDataParams(dataParams);
        request->setHeaders(headers);
        request->setRawData(requestQuery.value("raw_data").toString());
        request->setMethod(requestQuery.value("method").toString());
        request->setName(requestQuery.value("name").toString());
        request->setContentType(requestQuery.value("content_type").toString());
        request->setDocumentation(requestQuery.value("documentation").toString());
        request->setTests(requestQuery.value("tests").toString());
        request->setEdited(false);

        node->setRequest(request);
    }

    return node;
}

QList<RequestTreeNode *> SQLiteStorage::getNodes()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT n.id, n.uuid, n.is_folder, n.folder_name, n.pointer, n.edited_at, n.deleted, "
                  "pn.uuid as parent_uuid FROM node n LEFT JOIN node pn ON n.parent_id = pn.id;");
    query.exec();
    handleErrors(query);

    QList<RequestTreeNode *> result;
    while (query.next()) {
        RequestTreeNode *node = new RequestTreeNode(query.value("uuid").toString());
        node->setLocalId(query.value("id").toInt());
        node->setUuid(query.value("uuid").toString());
        node->setIsFolder(query.value("is_folder").toBool());
        node->setFolderName(query.value("folder_name").toString());
        node->setPointer(query.value("pointer").toInt());
        node->setEditedAt(QDateTime::fromString(query.value("edited_at").toString(), Qt::ISODateWithMs));
        node->setDeleted(query.value("deleted").toBool());
        node->setParentUuid(query.value("parent_uuid").toString());
        result.append(node);
    }

    return result;
}

void SQLiteStorage::updatePointer(QString nodeUuid, int value)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE node SET pointer = :pointer WHERE uuid = :uuid;");
    query.bindValue(":pointer", value);
    query.bindValue(":uuid", nodeUuid);
    query.exec();
    handleErrors(query);
}

void SQLiteStorage::handleErrors(QSqlQuery query)
{
    QSqlError error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        QString errorName;

        switch (error.type()) {
        case QSqlError::NoError:
            errorName = "NoError";
            break;
        case QSqlError::ConnectionError:
            errorName = "ConnectionError";
            break;
        case QSqlError::StatementError:
            errorName = "StatementError";
            break;
        case QSqlError::TransactionError:
            errorName = "TransactionError";
            break;
        case QSqlError::UnknownError:
            errorName = "UnknownError";
            break;
        }

        qDebug() << "Error" << errorName << "while executing query:" << query.lastQuery()
                 << "Error text: " << error.text();
    }
}

QString SQLiteStorage::getUuid()
{
    return QUuid::createUuid().toString().remove('{').remove('}');
}

void SQLiteStorage::runMigrations()
{
    QString createNodeTable = ""
        "create table node"
        "("
        "    id          integer  not null primary key autoincrement,"
        "    uuid        CHAR(36) not null unique,"
        "    parent_id   integer  references node on delete cascade,"
        "    pointer     integer  not null default 1,"
        "    edited_at   text     not null,"
        "    deleted     boolean  not null default false,"
        "    is_folder   bool     not null,"
        "    folder_name text"
        ");";
    QSqlQuery(createNodeTable, m_db).exec();

    QString createRequestTable = ""
        "create table request"
        "("
        "    id                integer  not null primary key autoincrement,"
        "    uuid              CHAR(36) not null unique,"
        "    node_id           integer  not null references node on delete cascade,"
        "    name              text     not null,"
        "    url               text,"
        "    raw_data          text,"
        "    method            text,"
        "    content_type      text,"
        "    documentation     text,"
        "    query_params_json text,"
        "    data_params_json  text,"
        "    headers_json      text,"
        "    tests             text"
        ");";
    QSqlQuery(createRequestTable, m_db).exec();
}
