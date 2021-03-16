#ifndef APP_H
#define APP_H

#include <QObject>
#include "Models/request.h"
#include "Models/requesttreenode.h"
#include "Models/treemodel.h"
#include "db/SQLiteStorage.h"
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

    Q_INVOKABLE void saveCurrentRequest();
    Q_INVOKABLE void createRequest(QString name, QString method, QModelIndex index);
    Q_INVOKABLE void createFolder(QString name, QModelIndex index);

private:
    TreeModel *m_requestTreeModel;
    Request *m_activeRequest;
    RequestTreeNode *m_rootRequestTreeNode;
    HttpClient *m_httpClient;
    SQLiteStorage *m_storage;

signals:
    void activeRequestChanged(Request *request);
    void requestTreeModelChanged(TreeModel *treeModel);

public slots:
    Q_INVOKABLE void requestTreeItemActivated(QModelIndex index);
};

#endif // APP_H
