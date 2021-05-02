#include <QDebug>
#include "Authenticator.h"
#include "Error.h"

Authenticator::Authenticator(JsonStorage *settings, QObject *parent)
    : QObject(parent)
{
    m_settings = settings;
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &Authenticator::requestFinished);
}

bool Authenticator::loggedIn() const
{
    return !m_settings->get("server").isNull()
            && !m_settings->get("username").isNull()
            && !m_settings->get("password").isNull();
}

void Authenticator::authenticateRequest(QNetworkRequest &request)
{
    QVariant username = m_settings->get("username");
    QVariant password = m_settings->get("password");

    if (username.isNull() || password.isNull()) {
        throw Error("Authenticator::authenticateRequest: "
                    "User is not logged in. Cannot authenticate request.");
    }

    applyBasicAuth(request, username.toString(), password.toString());
}

void Authenticator::logIn(QString serverUrl, QString username, QString password)
{
    QUrl url = QUrl(serverUrl).resolved(QUrl("/auth/check-credentials/"));
    QNetworkRequest request(url);
    applyBasicAuth(request, username, password);

    QNetworkReply *reply = m_networkAccessManager->get(request);
    reply->setProperty("requestType", "logIn");
    reply->setProperty("server", serverUrl);
    reply->setProperty("username", username);
    reply->setProperty("password", password);
}

void Authenticator::logOut()
{
    m_settings->set("server", QVariant());
    m_settings->set("username", QVariant());
    m_settings->set("password", QVariant());
    emit loginStatusChanged(false);
}

void Authenticator::applyBasicAuth(QNetworkRequest &request, QString username, QString password)
{
    QByteArray credentials = (username + ":" + password).toUtf8();
    request.setRawHeader("Authorization", "Basic " + credentials.toBase64());
}

void Authenticator::requestFinished(QNetworkReply *reply)
{
    if (reply->property("requestType").toString() == "logIn") {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        bool success = status == 200;

        if (success) {
            m_settings->set("server", reply->property("server"));
            m_settings->set("username", reply->property("username"));
            m_settings->set("password", reply->property("password"));
        }

        emit loginStatusChanged(success);
    }
}
