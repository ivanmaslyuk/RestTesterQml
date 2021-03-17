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
    Q_INVOKABLE void abortRequest();

private:
    QNetworkAccessManager *m_networkAccessManager;
    QElapsedTimer *m_timer;
    QNetworkReply *m_pendingReply;

signals:
    void responseUpdated(Response *response);
    void requestStarted();

private slots:
    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void requestFinished(QNetworkReply *reply);
};

#endif // HTTPCLIENT_H
