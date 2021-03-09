#ifndef APP_H
#define APP_H

#include <QObject>
#include "Models/requestmodel.h"
#include "Models/requesttreenode.h"
#include "Models/treemodel.h"

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(RequestModel* activeRequest READ activeRequest WRITE setActiveRequest NOTIFY activeRequestChanged)
    Q_PROPERTY(TreeModel* requestTreeModel READ requestTreeModel NOTIFY requestTreeModelChanged)

public:
    explicit App(QObject *parent = nullptr);

    RequestModel *activeRequest() const;
    void setActiveRequest(RequestModel *request);

    TreeModel *requestTreeModel() const;

private:
    TreeModel *m_requestTreeModel;
    RequestModel *m_activeRequest;
    RequestTreeNode *m_rootRequestTreeNode;

signals:
    void activeRequestChanged(RequestModel *request);
    void requestTreeModelChanged(TreeModel *treeModel);

public slots:
    Q_INVOKABLE void requestTreeItemActivated(QModelIndex index);
};

#endif // APP_H
