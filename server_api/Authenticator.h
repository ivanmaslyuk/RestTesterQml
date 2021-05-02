#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "db/JsonStorage.h"

class Authenticator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loginStatusChanged)

public:
    explicit Authenticator(JsonStorage *settings, QObject *parent = nullptr);

    bool loggedIn() const;

    void authenticateRequest(QNetworkRequest &request);

    Q_INVOKABLE void logIn(QString serverUrl, QString username, QString password);
    Q_INVOKABLE void logOut();

signals:
    void loginStatusChanged(bool loggedIn);

private:
    void applyBasicAuth(QNetworkRequest &request, QString username, QString password);

    QNetworkAccessManager *m_networkAccessManager;
    JsonStorage *m_settings;

private slots:
    void requestFinished(QNetworkReply *reply);

};

#endif // AUTHENTICATOR_H
