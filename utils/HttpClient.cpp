#include "HttpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_timer = new QElapsedTimer();

    connect(m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &HttpClient::requestFinished);
}

void HttpClient::makeRequest(Request *request)
{
    if (pendingReply)
        return; // Don't make new request if there is one already running

    emit requestStarted();

    QNetworkRequest networkRequest(QUrl(request->url()));
    networkRequest.setHeader(QNetworkRequest::UserAgentHeader, "RestTester/0.1");

    QString method = request->method();
    bool sendData = QList<QString>({"GET", "OPTIONS", "HEAD"}).contains(method);
    if (sendData)
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, request->contentType());

    for (ParamModel *header : request->headers()) {
        networkRequest.setRawHeader(header->key.toUtf8(), header->value.toUtf8());
    }

    m_timer->start();
    QByteArray data = request->data();
    if (sendData) {
        pendingReply = m_networkAccessManager->sendCustomRequest(networkRequest,
                                                                 method.toUtf8(), data);
    } else {
        pendingReply = m_networkAccessManager->sendCustomRequest(networkRequest, method.toUtf8());
    }
}

void HttpClient::abortRequest()
{
    if (!pendingReply)
        return;

    pendingReply->abort();
}

void HttpClient::requestFinished(QNetworkReply *reply)
{
    pendingReply = nullptr;

    reply->setProperty("time", QVariant::fromValue<qint64>(m_timer->elapsed()));
    Response *response = new Response(reply, this);
    emit responseUpdated(response);
}
