#ifndef APP_H
#define APP_H

#include <QObject>

#include "Models/request.h"
#include "Models/requesttreenode.h"
#include "Models/treemodel.h"
#include "db/SQLiteStorage.h"
#include "utils/HttpClient.h"
#include "server_api/Authenticator.h"
#include "db/JsonStorage.h"
#include "server_api/ServerSyncService.h"

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Request* activeRequest READ activeRequest WRITE setActiveRequest NOTIFY activeRequestChanged)
    Q_PROPERTY(TreeModel* requestTreeModel READ requestTreeModel NOTIFY requestTreeModelChanged)
    Q_PROPERTY(Authenticator* authenticator READ authenticator CONSTANT)
    Q_PROPERTY(HttpClient *httpClient READ httpClient CONSTANT)
    Q_PROPERTY(JsonStorage *settings READ settings CONSTANT)
    Q_PROPERTY(ServerSyncService *serverSyncService READ serverSyncService CONSTANT)
    Q_PROPERTY(bool requestSelected READ requestSelected NOTIFY requestSelectedChanged)
    Q_PROPERTY(bool showRequestList READ showRequestList WRITE setShowRequestList NOTIFY showRequestListChanged)

public:
    explicit App(QObject *parent = nullptr);

    Request *activeRequest() const;
    void setActiveRequest(Request *request);

    HttpClient *httpClient() const;
    TreeModel *requestTreeModel() const;

    Authenticator *authenticator() const;

    JsonStorage *settings() const;

    ServerSyncService *serverSyncService() const;

    bool requestSelected() const;

    Q_INVOKABLE void saveCurrentRequest();
    Q_INVOKABLE bool hasUnsavedRequests();
    Q_INVOKABLE void createRequest(QString name, QString method, QModelIndex index);
    Q_INVOKABLE void createFolder(QString name, QModelIndex index);
    Q_INVOKABLE void renameNode(QString newName, QModelIndex index);
    Q_INVOKABLE void deleteNode(QModelIndex index);

    bool showRequestList() const;
    void setShowRequestList(bool showRequestList);

private:
    bool m_requestSelected;
    ServerSyncService *m_serverSyncService;
    Authenticator *m_authenticator;
    TreeModel *m_requestTreeModel;
    Request *m_activeRequest;
    RequestTreeNode *m_rootRequestTreeNode;
    HttpClient *m_httpClient;
    SQLiteStorage *m_storage;
    JsonStorage *m_settings;

    bool m_showRequestList;

signals:
    void activeRequestChanged(Request *request);
    void requestTreeModelChanged(TreeModel *treeModel);
    void requestSelectedChanged(bool requestSelected);
    void showRequestListChanged(bool showRequestList);

public slots:
    Q_INVOKABLE void requestTreeItemActivated(QModelIndex index);
    Q_INVOKABLE void startSync();
    void reloadTree();
};

#endif // APP_H
