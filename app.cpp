#include <QDebug>
#include "app.h"

App::App(QObject *parent) : QObject(parent)
{
    qDebug() << "REST Tester app launched!";


    // Test request
    m_activeRequest = new RequestModel(this);
    m_activeRequest->setQueryParams({
        new ParamModel("query param 1", "val", false)
    });
    m_activeRequest->setDataParams({
        new ParamModel("data param 1", "val", false)
    });
    m_activeRequest->setHeaders({
        new ParamModel("header 1", "val", false)
    });


    // Test tree data
    m_rootRequestTreeNode = new RequestTreeNode(this);

    RequestTreeNode *childTreeNode = new RequestTreeNode(m_rootRequestTreeNode);
    childTreeNode->setFolderName("hello folder");
    childTreeNode->setIsFolder(true);

    RequestTreeNode *childTreeNode2 = new RequestTreeNode(childTreeNode);
    childTreeNode2->setFolderName("folder inside folder! :)");
    childTreeNode2->setIsFolder(true);

    RequestTreeNode *requestNode1 = new RequestTreeNode(childTreeNode2);
    RequestModel *request1 = new RequestModel(this);
    requestNode1->setRequest(request1);
    request1->setName("test");
    request1->setMethod("DELETE");
    request1->setUrl("http://test.com/");
    request1->setContentType("application/json");
    request1->setDocumentation("# Test\n[test link](http://example.com)");
    request1->setRawData("{}");
    request1->setContentType("application/json");
    request1->setQueryParams({
        new ParamModel("query param 1", "val", false)
    });
    request1->setDataParams({
        new ParamModel("data param 1", "val", false)
    });
    request1->setHeaders({
        new ParamModel("header 1", "val", false)
    });

    RequestTreeNode *requestNode2 = new RequestTreeNode(childTreeNode2);
    RequestModel *request2 = new RequestModel(this);
    requestNode2->setRequest(request2);
    request2->setName("test2");
    request2->setMethod("POST");
    request2->setUrl("http://test2.com/");
    request2->setContentType("application/json");
    request2->setDocumentation("# Test2\n[test link 2](http://example.com)");
    request2->setRawData("raw data lololol");
    request2->setContentType("text/plain");
    request2->setQueryParams({
        new ParamModel("query param 1", "val", false)
    });
    request2->setDataParams({
        new ParamModel("data param 1", "val", false)
    });
    request2->setHeaders({
        new ParamModel("header 1", "val", false)
    });

    m_requestTreeModel = new TreeModel(m_rootRequestTreeNode, this);
}

RequestModel *App::activeRequest() const
{
    return m_activeRequest;
}

void App::setActiveRequest(RequestModel *request)
{
    qDebug() << "Active request is now " + request->name();
    m_activeRequest = request;
    emit activeRequestChanged(request);
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
