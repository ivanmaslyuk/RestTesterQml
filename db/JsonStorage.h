#ifndef JSONSTORAGE_H
#define JSONSTORAGE_H

#include <QObject>
#include <QJsonObject>
#include "Models/requesttreenode.h"

class JsonStorage : public QObject
{
    Q_OBJECT

public:
    explicit JsonStorage(QObject *parent = nullptr);

    RequestTreeNode *getRequestTree();

private:
    QJsonObject readData();
    RequestTreeNode *nodeFromJson(QJsonObject node, QObject *parent);
};

#endif // JSONSTORAGE_H
