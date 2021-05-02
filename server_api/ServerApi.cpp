#include <QUrl>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include "ServerApi.h"

#include "Error.h"
#include "NodeSerializer.h"

ServerApi::ServerApi(JsonStorage *settings, Authenticator *authenticator,
                     QObject *parent) : QObject(parent)
{
    m_authenticator = authenticator;
    m_network = new QNetworkAccessManager(this);
    m_settings = settings;
}

QList<RequestTreeNode *> ServerApi::getNodes()
{
    QNetworkReply *reply = makeRequest("GET", "/api/nodes/");
    QString json = reply->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError)
        throw Error("ServerApi::getNodes(): got invalid json from server");

    QList<RequestTreeNode *> nodes;
    for (QJsonValue value : doc.array())
        nodes.append(NodeSerializer::fromJson(value.toObject()));

    return nodes;
}

void ServerApi::createNode(RequestTreeNode *node)
{
    QJsonObject json = NodeSerializer::toJson(node);
    QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    QNetworkReply *reply = makeRequest("POST", "/api/nodes/", data);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
            .toInt();
    if (status != 201)
        throw Error(QString("ServerApi::createNode() failed %1").arg(QString(reply->readAll())));
}

void ServerApi::updateNode(RequestTreeNode *node)
{
    QJsonObject json = NodeSerializer::toJson(node);
    QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    QString url = QString("/api/nodes/%1/").arg(node->uuid());
    QNetworkReply *reply = makeRequest("PUT", url, data);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
            .toInt();
    if (status != 200)
        throw Error(QString("ServerApi::updateNode() failed %1").arg(QString(reply->readAll())));
}

void ServerApi::deleteNode(QString uuid)
{
    QString url = QString("/api/nodes/%1/").arg(uuid);
    QNetworkReply *reply = makeRequest("DELETE", url);
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status != 204)
        throw Error("ServerApi::deleteNode() failed");
}

QJsonObject ServerApi::checkSync(QList<RequestTreeNode *> nodes)
{
    QJsonObject data;
    for (RequestTreeNode *node : nodes) {
        QJsonObject nodeData;
        nodeData["pointer"] = node->pointer();
        nodeData["edited_at"] = node->editedAt().toString(Qt::DateFormat::ISODateWithMs);
        data[node->uuid()] = nodeData;
    }

    QNetworkReply *reply = makeRequest("POST", "/api/sync-check/", QJsonDocument(data).toJson(QJsonDocument::Compact));
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status != 200)
        throw Error(QString("ServerApi::checkSync() failed. Got status: %1. Message: %2").arg(status)
                    .arg(QString(reply->readAll()).left(500)));

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    return doc.object();
}

QString ServerApi::getFullUrl(QString relativeUrl)
{
    QString serverUrl = m_settings->get("server").toString();
    QUrl fullUrl = QUrl(serverUrl).resolved(QUrl(relativeUrl));
    return fullUrl.toString();
}

QNetworkReply *ServerApi::makeRequest(QByteArray method, QString url,
                                      QByteArray data)
{
    qDebug() << QString("ServerApi::makeRequest: method %1, url %2").arg(QString(method)).arg(getFullUrl(url));
    QNetworkRequest request(getFullUrl(url));
    if (!data.isEmpty())
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_authenticator->authenticateRequest(request);
    QNetworkReply *reply = m_network->sendCustomRequest(request, method, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return reply;
}
