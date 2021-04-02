#include <QDebug>
#include "app.h"
#include "db/JsonStorage.h"
#include "Models/treeitem.h"

App::App(QObject *parent) : QObject(parent)
{
    qDebug() << "REST Tester app launched!";

    m_settings = new JsonStorage("settings.json", this);

    m_activeRequest = new Request(this);

    m_storage = new SQLiteStorage(this);
    m_rootRequestTreeNode = m_storage->getRequestTree();

    m_requestTreeModel = new TreeModel(m_rootRequestTreeNode, this);

    m_httpClient = new HttpClient(this);
    m_authenticator = new Authenticator(m_settings, this);
}

Request *App::activeRequest() const
{
    return m_activeRequest;
}

void App::setActiveRequest(Request *request)
{
    m_activeRequest = request;
    emit activeRequestChanged(request);
}

HttpClient *App::httpClient() const
{
    return m_httpClient;
}

TreeModel *App::requestTreeModel() const
{
    return m_requestTreeModel;
}

Authenticator *App::authenticator() const
{
    return m_authenticator;
}

JsonStorage *App::settings() const
{
    return m_settings;
}

void App::saveCurrentRequest()
{
    QObject *parentNode = m_activeRequest->parent();
    RequestTreeNode *node = static_cast<RequestTreeNode *>(parentNode);
    if (!node) {
        qDebug() << "Can't get parent of current request";
        return;
    }

    if (node->localId() != -1)
        m_storage->saveNode(node);
    else
        m_storage->saveNode(node);

    node->request()->setEdited(false);
}

void App::createRequest(QString name, QString method, QModelIndex index)
{
    TreeItem *treeItem;
    if (index.isValid())
        treeItem = static_cast<TreeItem *>(index.internalPointer());
    else
        treeItem = m_requestTreeModel->rootItem();

    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();

    RequestTreeNode *parentNode;
    if (node->isFolder())
        parentNode = node;
    else
        parentNode = static_cast<RequestTreeNode *>(node->parent());

    RequestTreeNode *newNode = new RequestTreeNode(parentNode);
    newNode->setIsFolder(false);

    Request *request = new Request(newNode);
    request->setName(name);
    request->setMethod(method);
    newNode->setRequest(request);
    request->setEdited(false);

    m_storage->createNode(newNode);

    QModelIndex parentIndex = node->isFolder() ? index : index.parent();
    m_requestTreeModel->insertRow(0, parentIndex);
    m_requestTreeModel->setData(m_requestTreeModel->index(0, 0, parentIndex), QVariant::fromValue(newNode));
}

void App::createFolder(QString name, QModelIndex index)
{
    TreeItem *treeItem;
    if (index.isValid())
        treeItem = static_cast<TreeItem *>(index.internalPointer());
    else
        treeItem = m_requestTreeModel->rootItem();

    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();

    RequestTreeNode *parentNode;
    if (node->isFolder())
        parentNode = node;
    else
        parentNode = static_cast<RequestTreeNode *>(node->parent());

    RequestTreeNode *newNode = new RequestTreeNode(parentNode);
    newNode->setIsFolder(true);
    newNode->setFolderName(name);

    m_storage->createNode(newNode);

    QModelIndex parentIndex = node->isFolder() ? index : index.parent();
    m_requestTreeModel->insertRow(0, parentIndex);
    m_requestTreeModel->setData(m_requestTreeModel->index(0, 0, parentIndex), QVariant::fromValue(newNode));
}

void App::renameNode(QString newName, QModelIndex index)
{
    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();

    if (node->isFolder())
        node->setFolderName(newName);
    else {
        node->request()->setName(newName);
        node->request()->setEdited(false);
    }

    m_storage->saveNode(node);

    m_requestTreeModel->setData(index, QVariant::fromValue(node));
}

void App::deleteNode(QModelIndex index)
{
    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();

    m_storage->deleteNode(node);

    m_requestTreeModel->removeRow(index.row(), index.parent());
}

void App::requestTreeItemActivated(QModelIndex index)
{
    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();
    if (!node->isFolder())
        setActiveRequest(node->request());
}
