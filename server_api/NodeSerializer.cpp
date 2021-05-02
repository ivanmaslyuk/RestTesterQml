#include <QJsonDocument>
#include <QJsonParseError>

#include "NodeSerializer.h"

#include "RequestSerializer.h"
#include "Error.h"


RequestTreeNode *NodeSerializer::fromJson(QJsonObject json)
{
    RequestTreeNode *node = new RequestTreeNode(json["uuid"].toString());
    node->setParentUuid(json["parent"].toString());
    node->setIsFolder(json["is_folder"].toBool());
    node->setFolderName(json["folder_name"].toString());
    node->setPointer(json["pointer"].toInt());
    node->setDeleted(json["deleted"].toBool());
    node->setEditedAt(QDateTime::fromString(json["edited_at"].toString().remove('Z'), Qt::ISODateWithMs));
    if (!json["request"].isNull())
        node->setRequest(RequestSerializer::fromJson(json["request"].toObject(), node));
    return node;
}

QJsonObject NodeSerializer::toJson(RequestTreeNode *node)
{
    QJsonObject json;
    json["uuid"] = node->uuid();
    json["parent"] = node->parentUuid();
    json["is_folder"] = node->isFolder();
    json["folder_name"] = node->folderName();
    json["pointer"] = node->pointer();
    json["deleted"] = node->deleted();
    json["edited_at"] = node->editedAt().toString(Qt::ISODateWithMs).remove('Z');
    if (!node->isFolder())
        json["request"] = RequestSerializer::toJson(node->request());
    else
        json["request"] = QJsonValue();
    return json;
}
