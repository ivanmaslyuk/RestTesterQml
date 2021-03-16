#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <functional>
#include "JsonStorage.h"
#include "Models/request.h"

JsonStorage::JsonStorage(QObject *parent) : QObject(parent)
{

}

RequestTreeNode *JsonStorage::getRequestTree()
{
    std::function<void(QJsonArray, QObject *)> getNodes;
    getNodes = [this, getNodes](QJsonArray nodes, QObject *parentNode){
        for (QJsonValue nodeValue : nodes) {
            RequestTreeNode *node = nodeFromJson(nodeValue.toObject(), parentNode);
            if (nodeValue["isFolder"].toBool())
                getNodes(nodeValue["children"].toArray(), node);
        }
    };

    QJsonObject data = readData();
    if(!data.contains("nodes")) {
        qDebug() << "Nodes not declared in app data";
        return new RequestTreeNode(this);
    }

    RequestTreeNode *rootNode = new RequestTreeNode(this);
    getNodes(data["nodes"].toArray(), rootNode);

    return rootNode;
}

QJsonObject JsonStorage::readData()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDataPath.isEmpty()) {
        qDebug() << QString("Writable app data location not found");
        return QJsonObject();
    }
    QDir appDataDir = QDir(appDataPath);
    if (!appDataDir.exists()) {
        bool created = QDir().mkpath(appDataPath);
        if (!created) {
            qDebug() << QString("Could not create app data directory");
            return QJsonObject();
        }
    }

    QFile file(appDataDir.absoluteFilePath("data.json"));
    QByteArray text;
    if (file.open(QFile::ReadWrite)) {
        text = file.readAll();
        file.close();
    } else {
        qDebug() << "Could not open data file";
        return QJsonObject();
    }

    if (text.isEmpty())
        text = "{ \"nodes\": [] }";

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(text, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Could not parse json: " << parseError.errorString();
        return QJsonObject();
    }

    return doc.object();
}

RequestTreeNode *JsonStorage::nodeFromJson(QJsonObject node, QObject *parentObject)
{
    RequestTreeNode *result = new RequestTreeNode(parentObject);
    result->setIsFolder(node["isFolder"].toBool());

    if (result->isFolder()) {
        result->setFolderName(node["folderName"].toString());
    } else {
        QJsonValue requestData = node["request"];

        QList<ParamModel *> queryParams;
        for (QJsonValue value : requestData["queryParams"].toArray())
            queryParams.append(new ParamModel(value["key"].toString(),
                               value["value"].toString(), value["enabled"].toBool()));

        QList<ParamModel *> dataParams;
        for (QJsonValue value : requestData["dataParams"].toArray())
            dataParams.append(new ParamModel(value["key"].toString(),
                               value["value"].toString(), value["enabled"].toBool()));

        QList<ParamModel *> headers;
        for (QJsonValue value : requestData["headers"].toArray())
            headers.append(new ParamModel(value["key"].toString(),
                               value["value"].toString(), value["enabled"].toBool()));

        Request *request = new Request(
//                    requestData["url"].toString(),
//                    queryParams,
//                    dataParams,
//                    headers,
//                    requestData["rawData"].toString(),
//                    requestData["method"].toString(),
//                    requestData["name"].toString(),
//                    requestData["contentType"].toString(),
//                    requestData["documentation"].toString(),
                    this);

        result->setRequest(request);
    }

    return result;
}
