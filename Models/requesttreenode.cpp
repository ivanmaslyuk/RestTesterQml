#include "requesttreenode.h"

#include <QSqlDatabase>
#include <QSqlQuery>

#include "Error.h"
#include "db/SQLErrorHandler.h"

RequestTreeNode::RequestTreeNode(QObject *parent)
    : QObject(parent)
{
    m_isFolder = false;
    m_localId = -1;
    m_uuid = "";
    m_deleted = false;
    m_pointer = 1;
    m_editedAt = QDateTime::currentDateTime();
}

RequestTreeNode::RequestTreeNode(int localId, QObject *parent)
    : RequestTreeNode(parent)
{
    m_localId = localId;
}

RequestTreeNode::RequestTreeNode(QString uuid, QObject *parent)
    : RequestTreeNode(parent)
{
    m_uuid = uuid;
}

RequestTreeNode *RequestTreeNode::getById(int id, QString connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    QSqlQuery nodeQuery("SELECT * FROM node n JOIN node p ON n.parent_id = p.id WHERE deleted = false LIMIT 1;", db);
    if (!nodeQuery.next())
        throw Error(QString("Node with id %1 was not found in the DB.").arg(id));

    RequestTreeNode *node = RequestTreeNode::fromSqlQuery(nodeQuery);

    if (!node->isFolder()) {
        QSqlQuery requestQuery("SELECT * FROM request r JOIN node n ON n.id = r.node_id WHERE n.deleted = false;", db);
        if (!requestQuery.next())
            throw Error(QString("Request for node with id %1 was not found.").arg(id));

        Request *request = Request::fromSqlQuery(requestQuery);
        request->setParent(node);

        node->setRequest(request);
    }

    return node;
}

QList<RequestTreeNode *> RequestTreeNode::getAll(QString connectionName)
{
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    QSqlQuery query("SELECT * FROM node n JOIN node p ON n.parent_id = p.id WHERE deleted = false");

    while (query.next()) {

    }
}

RequestTreeNode *RequestTreeNode::fromSqlQuery(QSqlQuery query)
{
    RequestTreeNode *node = new RequestTreeNode();
    int localId = query.value("id").toInt();
    node->setLocalId(localId);
    node->setParentUuid(query.value("p.uuid").toString());
    node->setUuid(query.value("uuid").toString());
    node->setIsFolder(query.value("is_folder").toBool());
    node->setFolderName(query.value("folder_name").toString());
    node->setDeleted(query.value("deleted").toBool());
    node->setEditedAt(QDateTime::fromString(query.value("edited_at").toString(), Qt::ISODateWithMs));
    node->setPointer(query.value("pointer").toInt());

    return node;
}

void RequestTreeNode::save(QString connectionName)
{
    if (m_localId == -1)
        throw Error("Cannot save Node that hasn't been created first.");

    QSqlDatabase db = QSqlDatabase::database(connectionName);

    QSqlQuery query(db);
    query.prepare("UPDATE node SET folder_name = :folder_name, deleted = :deleted, edited_at = :edited_at,"
                  "pointer = :pointer WHERE id = :local_id;");
    query.bindValue(":folder_name", m_folderName);
    query.bindValue(":deleted", m_deleted);
    query.bindValue(":edited_at", m_editedAt.toString(Qt::ISODateWithMs));
    query.bindValue(":pointer", m_pointer);
    query.bindValue(":local_id", m_localId);
    query.exec();

    SQLErrorHandler::handleErrors(query);

    if (!m_isFolder)
        m_request->save(connectionName);
}

void RequestTreeNode::setEditedAtToNow()
{
    m_editedAt = QDateTime::currentDateTime();
}

void RequestTreeNode::incrementPointer()
{
    m_pointer += 1;
}

int RequestTreeNode::localId() const
{
    return m_localId;
}

void RequestTreeNode::setLocalId(int id)
{
    m_localId = id;
}

QString RequestTreeNode::uuid() const
{
    return m_uuid;
}

void RequestTreeNode::setUuid(QString uuid)
{
    m_uuid = uuid;
}

Request *RequestTreeNode::request() const
{
    return m_request;
}

void RequestTreeNode::setRequest(Request *request)
{
    m_request = request;
    emit requestChanged(request);
}

QString RequestTreeNode::folderName() const
{
    return m_folderName;
}

void RequestTreeNode::setFolderName(QString name)
{
    m_folderName = name;
    emit folderNameChanged(name);
}

bool RequestTreeNode::isFolder() const
{
    return m_isFolder;
}

void RequestTreeNode::setIsFolder(bool isFolder)
{
    m_isFolder = isFolder;
}

QDateTime RequestTreeNode::editedAt() const
{
    return m_editedAt;
}

void RequestTreeNode::setEditedAt(QDateTime value)
{
    m_editedAt = value;
}

int RequestTreeNode::pointer() const
{
    return m_pointer;
}

void RequestTreeNode::setPointer(int value)
{
    m_pointer = value;
}

bool RequestTreeNode::deleted() const
{
    return m_deleted;
}

void RequestTreeNode::setDeleted(bool deleted)
{
    m_deleted = deleted;
}

QString RequestTreeNode::parentUuid() const
{
    return m_parentUuid;
}

void RequestTreeNode::setParentUuid(QString parentUuid)
{
    m_parentUuid = parentUuid;
}

bool RequestTreeNode::hasParent() const
{
    return !m_parentUuid.isEmpty();
}
