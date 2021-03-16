#include "requesttreenode.h"

RequestTreeNode::RequestTreeNode(QObject *parent) : QObject(parent)
{
    m_isFolder = false;
    m_localId = -1;
}

RequestTreeNode::RequestTreeNode(int localId, QObject *parent)
    : RequestTreeNode(parent)
{
    m_localId = localId;
}

int RequestTreeNode::localId() const
{
    return m_localId;
}

void RequestTreeNode::setLocalId(int id)
{
    m_localId = id;
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
