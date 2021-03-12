#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QElapsedTimer>
#include "Models/Response.h"
#include "Models/request.h"

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);

    Q_INVOKABLE void makeRequest(Request *request);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QElapsedTimer *m_timer;

signals:
    void responseUpdated(Response *response);

private slots:
    void requestFinished(QNetworkReply *reply);
};

#endif // HTTPCLIENT_H
