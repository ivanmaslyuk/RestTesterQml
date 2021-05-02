#include "requesttreenode.h"

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
