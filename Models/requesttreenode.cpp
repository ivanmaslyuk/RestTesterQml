#include "requesttreenode.h"

RequestTreeNode::RequestTreeNode(QObject *parent) : QObject(parent)
{
    m_isFolder = false;
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
