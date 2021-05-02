#ifndef SERVERSYNCSERVICE_H
#define SERVERSYNCSERVICE_H

#include <QThread>

#include "db/JsonStorage.h"
#include "db/SQLiteStorage.h"
#include "Authenticator.h"

class ServerSyncService : public QThread
{
    Q_OBJECT
public:
    ServerSyncService(JsonStorage *settings, Authenticator *authenticator);
    ~ServerSyncService();

signals:
    void syncFinished();
    void syncError(QString message);
    void syncConflict(QString nodeName);

protected:
    void run() override;

private:
    JsonStorage *m_settings;
    Authenticator *m_authenticator;
    SQLiteStorage *m_storage;
};

#endif // SERVERSYNCSERVICE_H
