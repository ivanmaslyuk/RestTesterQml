#include <QDebug>
#include "requestmodel.h"

RequestModel::RequestModel(QObject *parent) : QObject(parent)
{
    m_edited = false;
    m_queryParamsModel = new ParamsItemModel(&m_queryParams, this);
    m_dataParamsModel = new ParamsItemModel(&m_dataParams, this);
    m_headersModel = new ParamsItemModel(&m_headers, this);
}

bool RequestModel::edited() const
{
    return m_edited;
}

QString RequestModel::url() const
{
    return m_url;
}

QList<ParamModel *> RequestModel::queryParams() const
{
    return m_queryParams;
}

QList<ParamModel *> RequestModel::dataParams() const
{
    return m_dataParams;
}

QList<ParamModel *> RequestModel::headers() const
{
    return m_headers;
}

QString RequestModel::rawData() const
{
    return m_rawData;
}

QString RequestModel::method() const
{
    return m_method;
}

QString RequestModel::name() const
{
    return m_name;
}

QString RequestModel::contentType() const
{
    return m_contentType;
}

QString RequestModel::documentation() const
{
    return m_documentation;
}

ParamsItemModel *RequestModel::queryParamsModel() const
{
    return m_queryParamsModel;
}

ParamsItemModel *RequestModel::dataParamsModel() const
{
    return m_dataParamsModel;
}

ParamsItemModel *RequestModel::headersModel() const
{
    return m_headersModel;
}

void RequestModel::setUrl(QString url)
{
    m_url = url;
    setEdited(true);
    emit urlChanged(url);
//    qDebug() << "RequestModel url set: " << url;
}

void RequestModel::setQueryParams(QList<ParamModel *> queryParams)
{
    m_queryParams = queryParams;
    setEdited(true);
    emit queryParamsChanged(queryParams);
}

void RequestModel::setDataParams(QList<ParamModel *> dataParams)
{
    m_dataParams = dataParams;
    setEdited(true);
    emit dataParamsChanged(dataParams);
}

void RequestModel::setHeaders(QList<ParamModel *> headers)
{
    m_headers = headers;
    setEdited(true);
    emit headersChanged(headers);
}

void RequestModel::setRawData(QString rawData)
{
    m_rawData = rawData;
    setEdited(true);
    emit rawDataChanged(rawData);
//    qDebug() << "RequestModel rawData set: " << rawData;
}

void RequestModel::setMethod(QString method)
{
    m_method = method;
    setEdited(true);
    emit methodChanged(method);
//    qDebug() << "RequestModel " + name() + " method set: " << method;
}

void RequestModel::setName(QString name)
{
    m_name = name;
    setEdited(true);
    emit nameChanged(name);
//    qDebug() << "RequestModel name set: " << name;
}

void RequestModel::setContentType(QString contentType)
{
    m_contentType = contentType;
    setEdited(true);
    emit contentTypeChanged(contentType);
//    qDebug() << "RequestModel contentType set: " << contentType;
}

void RequestModel::setDocumentation(QString documentation)
{
    m_documentation = documentation;
    setEdited(true);
    emit documentationChanged(documentation);
//    qDebug() << "RequestModel documentation set: " << documentation;
}

void RequestModel::setEdited(bool edited)
{
    if (m_edited == edited) return;

//    qDebug() << "RequestModel edited set: " << edited;
    m_edited = edited;
    emit editedChanged(edited);
}

