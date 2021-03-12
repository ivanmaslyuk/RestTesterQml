#include "Response.h"
#include <QJsonDocument>

Response::Response(QNetworkReply *reply, QObject *parent) : QObject(parent)
{
    applyNetworkReply(reply);
    m_headersModel = new ParamsItemModel(&m_headers, this);
}

QString Response::body() const
{
    return m_body;
}

int Response::status() const
{
    return m_status;
}

qint64 Response::time() const
{
    return m_time;
}

qint64 Response::size() const
{
    return m_size;
}

QList<ParamModel *> Response::headers() const
{
    return m_headers;
}

ParamsItemModel *Response::headersModel() const
{
    return m_headersModel;
}

void Response::applyNetworkReply(QNetworkReply *reply)
{
    QByteArray body = reply->readAll();

    // Calculate approximate size of headers and body
    qint64 bodySize = body.size();
    qint64 headersSize = 0;
    for (auto headerPair: reply->rawHeaderPairs()) {
        auto header = headerPair.first + ": " + headerPair.second;
        headersSize += header.size();
    }

    m_size = bodySize + headersSize;

    // Prettify response if possible
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString().split(';')[0];
    if (contentType == "application/json") {
        QJsonDocument doc = QJsonDocument::fromJson(body);
        m_body = doc.toJson(QJsonDocument::Indented);
    } else {
        m_body = body;
    }

    // Set headers
    for (QPair<QByteArray, QByteArray> header : reply->rawHeaderPairs()) {
        ParamModel *headerParam = new ParamModel(header.first, header.second);
        m_headers.append(headerParam);
    }

    // Other data
    m_time = reply->property("time").value<qint64>();
    m_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
}
