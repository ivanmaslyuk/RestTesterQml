#ifndef APP_H
#define APP_H

#include <QObject>
#include "Models/request.h"
#include "Models/requesttreenode.h"
#include "Models/treemodel.h"
#include "utils/HttpClient.h"

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Request* activeRequest READ activeRequest WRITE setActiveRequest NOTIFY activeRequestChanged)
    Q_PROPERTY(TreeModel* requestTreeModel READ requestTreeModel NOTIFY requestTreeModelChanged)
    Q_PROPERTY(HttpClient *httpClient READ httpClient CONSTANT)

public:
    explicit App(QObject *parent = nullptr);

    Request *activeRequest() const;
    void setActiveRequest(Request *request);
    HttpClient *httpClient() const;
    TreeModel *requestTreeModel() const;

private:
    TreeModel *m_requestTreeModel;
    Request *m_activeRequest;
    RequestTreeNode *m_rootRequestTreeNode;
    HttpClient *m_httpClient;

signals:
    void activeRequestChanged(Request *request);
    void requestTreeModelChanged(TreeModel *treeModel);

public slots:
    Q_INVOKABLE void requestTreeItemActivated(QModelIndex index);
};

#endif // APP_H
