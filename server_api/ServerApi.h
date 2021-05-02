#ifndef SERVERAPI_H
#define SERVERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include "Models/requesttreenode.h"
#include "db/JsonStorage.h"
#include "Authenticator.h"

class ServerApi : public QObject
{
    Q_OBJECT
public:
    explicit ServerApi(JsonStorage *settings, Authenticator *authenticator,
                       QObject *parent = nullptr);

    QList<RequestTreeNode *> getNodes();
    void createNode(RequestTreeNode *node);
    void updateNode(RequestTreeNode *node);
    void deleteNode(QString uuid);
    QJsonObject checkSync(QList<RequestTreeNode *> nodes);

private:
    QString getFullUrl(QString relativeUrl);
    QNetworkReply *makeRequest(QByteArray method, QString url,
                              QByteArray data = "");

    QNetworkAccessManager *m_network;
    JsonStorage *m_settings;
    Authenticator *m_authenticator;
};

#endif // SERVERAPI_H
