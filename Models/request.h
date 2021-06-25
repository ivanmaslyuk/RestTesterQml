#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include <QSqlQuery>

#include "parammodel.h"
#include "item_models/ParamsItemModel.h"

class Request : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool edited READ edited NOTIFY editedChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString rawData READ rawData WRITE setRawData NOTIFY rawDataChanged)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QString documentation READ documentation WRITE setDocumentation NOTIFY documentationChanged)
    Q_PROPERTY(QString tests READ tests WRITE setTests NOTIFY testsChanged)

    // Item Model Fields
    Q_PROPERTY(ParamsItemModel* queryParamsModel READ queryParamsModel CONSTANT)
    Q_PROPERTY(ParamsItemModel* dataParamsModel READ dataParamsModel CONSTANT)
    Q_PROPERTY(ParamsItemModel* headersModel READ headersModel CONSTANT)

public:
    explicit Request(QObject *parent = nullptr);
    explicit Request(int localId, QObject *parent = nullptr);
    explicit Request(QString uuid, QObject *parent = nullptr);

    static Request *empty(QObject *parent = nullptr);
    static Request *fromSqlQuery(QSqlQuery query);

    QByteArray data();

    QString uuid();
    void setUuid(QString uuid);

    int localId() const;
    void setLocalId(int id);

    bool edited() const;
    void setEdited(bool edited);

    QString tests() const;
    void setTests(QString value);

    QString url() const;
    QList<ParamModel *> queryParams() const;
    QList<ParamModel *> dataParams() const;
    QList<ParamModel *> headers() const;
    QString rawData() const;
    QString method() const;
    QString name() const;
    QString contentType() const;
    QString documentation() const;
    ParamsItemModel *queryParamsModel() const;
    ParamsItemModel *dataParamsModel() const;
    ParamsItemModel *headersModel() const;

    void setUrl(QString url);
    void setQueryParams(QList<ParamModel *> queryParams);
    void setDataParams(QList<ParamModel *> dataParams);
    void setHeaders(QList<ParamModel *> headers);
    void setRawData(QString rawData);
    void setMethod(QString method);
    void setName(QString name);
    void setContentType(QString contentType);
    void setDocumentation(QString documentation);

private:
    QString toUrlencoded(QList<ParamModel *> params);
    QList<ParamModel *> fromUrlencoded(QString urlencoded);

    QString m_uuid;
    int m_localId;
    bool m_edited;
    QString m_url;
    QList<ParamModel *> m_queryParams;
    QList<ParamModel *> m_dataParams;
    QList<ParamModel *> m_headers;
    QString m_rawData;
    QString m_method;
    QString m_name;
    QString m_contentType;
    QString m_documentation;
    QString m_tests;

    ParamsItemModel *m_queryParamsModel;
    ParamsItemModel *m_dataParamsModel;
    ParamsItemModel *m_headersModel;

private slots:
    void handleQueryParamsChanged();
    void handleBodyParamsChanged();
    void handleHeadersChanged();

signals:
    void urlChanged(QString url);
    void rawDataChanged(QString rawData);
    void methodChanged(QString method);
    void nameChanged(QString name);
    void contentTypeChanged(QString contentType);
    void documentationChanged(QString documentation);
    void editedChanged(bool edited);
    void testsChanged(QString value);

    void noSignal(QString);
};

#endif // REQUESTMODEL_H
