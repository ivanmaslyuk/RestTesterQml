#ifndef NODESERIALIZER_H
#define NODESERIALIZER_H

#include <QJsonObject>

#include "Models/requesttreenode.h"

class NodeSerializer
{
public:
    NodeSerializer() = delete;

    static RequestTreeNode *fromJson(QJsonObject json);
    static QJsonObject toJson(RequestTreeNode *node);
};

#endif // NODESERIALIZER_H
