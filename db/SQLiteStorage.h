#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QObject>
#include <QSqlDatabase>
#include "Models/requesttreenode.h"

class SQLiteStorage : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteStorage(QString connectionName, QObject *parent = nullptr);

    RequestTreeNode *getRequestTree();
    void saveNode(RequestTreeNode *node, bool updatedByUser = true);
    void createNode(RequestTreeNode *node, bool createdByUser = true);
    int getNodeLocalId(QString uuid);
    int getRequestLocalId(QString uuid);
    RequestTreeNode *getNode(QString uuid);
    QList<RequestTreeNode *> getNodes();
    void updatePointer(QString nodeUuid, int value);
    void runMigrations();

private:
    void handleErrors(QSqlQuery query);
    QString getUuid();

    QSqlDatabase m_db;

};

#endif // SQLITESTORAGE_H
