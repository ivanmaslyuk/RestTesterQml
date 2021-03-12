#include <QDebug>
#include "app.h"

App::App(QObject *parent) : QObject(parent)
{
    qDebug() << "REST Tester app launched!";

    m_activeRequest = Request::empty(this);

    // <TEST DATA>
    m_rootRequestTreeNode = new RequestTreeNode(this);

    Request *r1 = new Request("http://example.com",
                                        {new ParamModel("query 1", "val1")},
                                        {new ParamModel("data param 1", "val1")},
                                        {new ParamModel("header 1", "val1")},
                                        "raw data 1",
                                        "GET",
                                        "name",
                                        "text/plain",
                                        "documentation 1",
                                        this);
    RequestTreeNode *firstNode = new RequestTreeNode(m_rootRequestTreeNode);
    firstNode->setRequest(r1);

    Request *r2 = new Request("http://postman-echo.com/post?q=cars",
                                        {new ParamModel("q", "cars", false)},
                                        {new ParamModel("class", "S", false)},
                                        {new ParamModel("mobile-version", "1", false)},
                                        "{\"brand\": \"Toyota\"}",
                                        "POST",
                                        "Postman Echo",
                                        "application/json",
                                        "documentation 2",
                                        this);
    RequestTreeNode *sNode = new RequestTreeNode(m_rootRequestTreeNode);
    sNode->setRequest(r2);
    // </TEST DATA>

    m_requestTreeModel = new TreeModel(m_rootRequestTreeNode, this);
    m_httpClient = new HttpClient(this);
}

Request *App::activeRequest() const
{
    return m_activeRequest;
}

void App::setActiveRequest(Request *request)
{
    qDebug() << "Active request is now " + request->name();
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

void App::requestTreeItemActivated(QModelIndex index)
{
    TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
    RequestTreeNode *node = treeItem->data().value<RequestTreeNode *>();
    if (!node->isFolder())
        setActiveRequest(node->request());
}
