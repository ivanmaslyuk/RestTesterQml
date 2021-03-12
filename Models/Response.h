#ifndef RESPONSE_H
#define RESPONSE_H

#include <QObject>
#include <QNetworkReply>
#include "parammodel.h"
#include "item_models/ParamsItemModel.h"

class Response : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString body READ body CONSTANT)
    Q_PROPERTY(int status READ status CONSTANT)
    Q_PROPERTY(qint64 time READ time CONSTANT)
    Q_PROPERTY(qint64 size READ size CONSTANT)
    Q_PROPERTY(ParamsItemModel* headersModel READ headersModel CONSTANT)

public:
    explicit Response(QNetworkReply *reply, QObject *parent = nullptr);

    QString body() const;
    int status() const;
    qint64 time() const;
    qint64 size() const;
    QList<ParamModel *> headers() const;
    ParamsItemModel *headersModel() const;

private:
    void applyNetworkReply(QNetworkReply *reply);

    QString m_body;
    int m_status;
    qint64 m_time;
    qint64 m_size;
    QList<ParamModel *> m_headers;
    ParamsItemModel *m_headersModel;

};

#endif // RESPONSE_H
