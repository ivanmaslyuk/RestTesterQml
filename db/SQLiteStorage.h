#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QObject>
#include <QSqlDatabase>
#include "Models/requesttreenode.h"

class SQLiteStorage : public QObject
{
    Q_OBJECT
public:
    explicit SQLiteStorage(QObject *parent = nullptr);

    RequestTreeNode *getRequestTree();
    void saveNode(RequestTreeNode *node);
    void createNode(RequestTreeNode *node);
    void deleteNode(RequestTreeNode *node);

private:
    QList<ParamModel *> paramsFromJson(QString json);
    QString paramsToJson(QList<ParamModel *> params);
    void handleErrors(QSqlQuery query);
    QString getUuid();

    QSqlDatabase m_db;

};

#endif // SQLITESTORAGE_H
