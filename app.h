#ifndef APP_H
#define APP_H

#include <QObject>
#include "Models/requestmodel.h"

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit App(QObject *parent = nullptr);
    RequestModel activeRequest() const;
    void setActiveRequest(RequestModel request);

    QString name() const;
    void setName(QString value);

signals:
    void activeRequestChanged(RequestModel request);
    void nameChanged(QString name);

private:
    RequestModel m_activeRequest;
    QString m_name;
};

#endif // APP_H
