#ifndef REQUESTMODEL_H
#define REQUESTMODEL_H

#include <QObject>
#include <QList>
#include "parammodel.h"
#include "item_models/ParamsItemModel.h"

class Request : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool edited READ edited NOTIFY editedChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
//    Q_PROPERTY(QList<ParamModel *> queryParams READ queryParams WRITE setQueryParams NOTIFY queryParamsChanged)
//    Q_PROPERTY(QList<ParamModel *> dataParams READ dataParams WRITE setDataParams NOTIFY dataParamsChanged)
//    Q_PROPERTY(QList<ParamModel *> headers READ headers WRITE setHeaders NOTIFY headersChanged)
    Q_PROPERTY(QString rawData READ rawData WRITE setRawData NOTIFY rawDataChanged)
    Q_PROPERTY(QString method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QString documentation READ documentation WRITE setDocumentation NOTIFY documentationChanged)

    // Input Fiels - same properties but they don't send signals on set()
//    Q_PROPERTY(QString urlInput READ url WRITE setUrl NOTIFY noSignal)
//    Q_PROPERTY(QString rawDataInput READ rawData WRITE setRawData NOTIFY noSignal)
//    Q_PROPERTY(QString documentationInput READ documentation WRITE setDocumentation NOTIFY noSignal)
//    Q_PROPERTY(QString methodInput READ method WRITE setMethod NOTIFY noSignal)

    // Item Model Fields
    Q_PROPERTY(ParamsItemModel* queryParamsModel READ queryParamsModel CONSTANT)
    Q_PROPERTY(ParamsItemModel* dataParamsModel READ dataParamsModel CONSTANT)
    Q_PROPERTY(ParamsItemModel* headersModel READ headersModel CONSTANT)

public:
    explicit Request(QString url, QList<ParamModel *> queryParams,
                          QList<ParamModel *> dataParams, QList<ParamModel *> headers,
                          QString rawData, QString method, QString name, QString contentType,
                          QString documentation, QObject *parent = nullptr);

    static Request *empty(QObject *parent = nullptr);

    QByteArray data();

    bool edited() const;
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
    void setEdited(bool edited);

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

    ParamsItemModel *m_queryParamsModel;
    ParamsItemModel *m_dataParamsModel;
    ParamsItemModel *m_headersModel;

private slots:
    void handleQueryParamsChanged();

signals:
    void urlChanged(QString url);
//    void queryParamsChanged(QList<ParamModel *> queryParams);
//    void dataParamsChanged(QList<ParamModel *> dataParams);
//    void headersChanged(QList<ParamModel *> headers);
    void rawDataChanged(QString rawData);
    void methodChanged(QString method);
    void nameChanged(QString name);
    void contentTypeChanged(QString contentType);
    void documentationChanged(QString documentation);
    bool editedChanged(bool edited);

    void noSignal(QString);
};

#endif // REQUESTMODEL_H
