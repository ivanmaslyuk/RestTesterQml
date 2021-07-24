#include <QDebug>
#include "app.h"
#include "db/JsonStorage.h"
#include "Models/treeitem.h"

App::App(QObject *parent) : QObject(parent)
{
    m_requestSelected = false;
    m_showRequestList = true;
    m_settings = new JsonStorage("settings.json", this);

    m_activeRequest = new Request(this);

    m_storage = new SQLiteStorage("qt_sql_default_connection", this);
    m_storage->runMigrations();
    m_rootRequestTreeNode = m_storage->getRequestTree();

    m_requestTreeModel = new TreeModel(m_rootRequestTreeNode, this);

    m_httpClient = new HttpClient(this);
    m_authenticator = new Authenticator(m_settings, this);
    m_serverSyncService = new ServerSyncService(m_settings, m_authenticator);

    // TODO: TEMP
    connect(m_serverSyncService, &ServerSyncService::syncFinished, this, &App::reloadTree);
    connect(m_serverSyncService, &ServerSyncService::syncFinished, []() {
        qDebug() << "S Y N C  F I N I S H E D";
    });

    connect(m_serverSyncService, &ServerSyncService::syncError, this, &App::reloadTree);
    connect(m_serverSyncService, &ServerSyncService::syncError, [](QString msg) {
        qDebug() << "S Y N C  E R R O R:" << msg;
    });

    connect(m_serverSyncService, &ServerSyncService::syncConflict, [](QString nodeName) {
        qDebug() << "S Y N C  C O N F L I C T:" << nodeName;
    });
}

Request *App::activeRequest() const
{
    return m_activeRequest;
}

void App::setActiveRequest(Request *request)
{
    m_activeRequest = request;
    emit activeRequestChanged(request);

    m_requestSelected = true;
    emit requestSelectedChanged(m_requestSelected);
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

ServerSyncService *App::serverSyncService() const
{
    return m_serverSyncService;
}

bool App::requestSelected() const
{
    return m_requestSelected;
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

bool App::hasUnsavedRequests()
{
    QList<RequestTreeNode *> nodes = m_rootRequestTreeNode->findChildren<RequestTreeNode *>();
    for (RequestTreeNode *node : nodes)
        if (!node->isFolder() && node->request()->edited())
            return true;

    return false;
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
    m_requestTreeModel->setData(m_requestTreeModel->index(0, 0, parentIndex),
                                QVariant::fromValue(newNode));
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
    m_requestTreeModel->setData(m_requestTreeModel->index(0, 0, parentIndex),
                                QVariant::fromValue(newNode));
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

    node->setDeleted(true);
    m_storage->saveNode(node);

    if (node->isFolder()) {
        QList<RequestTreeNode *> childrenNodes = node->findChildren<RequestTreeNode *>();
        for (RequestTreeNode *childNode : childrenNodes) {
            childNode->setDeleted(true);
            m_storage->saveNode(childNode);
        }
    }

    m_requestTreeModel->removeRow(index.row(), index.parent());
}

bool App::showRequestList() const
{
    return m_showRequestList;
}

void App::setShowRequestList(bool showRequestList)
{
    if (m_showRequestList == showRequestList)
        return;

    m_showRequestList = showRequestList;
    emit showRequestListChanged(m_showRequestList);
}

void App::requestTreeItemActivated(QModelIndex index)
{
    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();
    if (!node->isFolder())
        setActiveRequest(node->request());
}

void App::startSync()
{
    m_requestSelected = false;
    emit requestSelectedChanged(m_requestSelected);

    m_serverSyncService->start();

}

void App::reloadTree()
{
    m_rootRequestTreeNode = m_storage->getRequestTree();
    m_requestTreeModel->setRootNode(m_rootRequestTreeNode);
}
