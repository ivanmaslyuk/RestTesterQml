#include "request.h"
#include <QUrl>

Request::Request(QObject *parent) : QObject(parent)
{
    m_queryParamsModel = new ParamsItemModel(&m_queryParams, this);
    m_dataParamsModel = new ParamsItemModel(&m_dataParams, this);
    m_headersModel = new ParamsItemModel(&m_headers, this);

    connect(m_queryParamsModel, &ParamsItemModel::dataChanged, this, &Request::handleQueryParamsChanged);
    connect(m_dataParamsModel, &ParamsItemModel::dataChanged, this, &Request::handleBodyParamsChanged);
    connect(m_headersModel, &ParamsItemModel::dataChanged, this, &Request::handleHeadersChanged);
    m_localId = -1;
}

Request::Request(int localId, QObject *parent) : Request(parent)
{
    m_localId = localId;
}

QByteArray Request::data()
{
    if (m_contentType == "application/x-www-form-urlencoded") {
        return toUrlencoded(m_dataParams).toUtf8();
    }

    return m_rawData.toUtf8();
}

int Request::localId() const
{
    return m_localId;
}

void Request::setLocalId(int id)
{
    m_localId = id;
}

bool Request::edited() const
{
    return m_edited;
}

QString Request::url() const
{
    return m_url;
}

QList<ParamModel *> Request::queryParams() const
{
    return m_queryParams;
}

QList<ParamModel *> Request::dataParams() const
{
    return m_dataParams;
}

QList<ParamModel *> Request::headers() const
{
    return m_headers;
}

QString Request::rawData() const
{
    return m_rawData;
}

QString Request::method() const
{
    return m_method;
}

QString Request::name() const
{
    return m_name;
}

QString Request::contentType() const
{
    return m_contentType;
}

QString Request::documentation() const
{
    return m_documentation;
}

ParamsItemModel *Request::queryParamsModel() const
{
    return m_queryParamsModel;
}

ParamsItemModel *Request::dataParamsModel() const
{
    return m_dataParamsModel;
}

ParamsItemModel *Request::headersModel() const
{
    return m_headersModel;
}

void Request::setUrl(QString url)
{
    if (url == m_url) return;
    m_url = url;

    // Update m_queryParams
    QStringList urlParts = url.split("?");
    if (urlParts.length() == 1)
        urlParts.append("");

    QList<ParamModel *> disabledParams;
    for (ParamModel *param : m_queryParams)
        if (!param->enabled)
            disabledParams.append(param);

    QList<ParamModel *> newQueryParams = fromUrlencoded(urlParts[1]);
    m_queryParamsModel->replaceData(disabledParams + newQueryParams);

    setEdited(true);
    emit urlChanged(url);
}

void Request::setQueryParams(QList<ParamModel *> queryParams)
{
    if (queryParams == m_queryParams) return;
    m_queryParams = queryParams;
    m_queryParamsModel->replaceData(queryParams);
    setEdited(true);
}

void Request::setDataParams(QList<ParamModel *> dataParams)
{
    if (dataParams == m_dataParams) return;
    m_dataParams = dataParams;
    m_dataParamsModel->replaceData(dataParams);
    setEdited(true);
}

void Request::setHeaders(QList<ParamModel *> headers)
{
    if (headers == m_headers) return;
    m_headers = headers;
    m_headersModel->replaceData(headers);
    setEdited(true);
}

void Request::setRawData(QString rawData)
{
    if (rawData == m_rawData) return;
    m_rawData = rawData;
    setEdited(true);
    emit rawDataChanged(rawData);
}

void Request::setMethod(QString method)
{
    if (method == m_method) return;
    m_method = method;
    setEdited(true);
    emit methodChanged(method);
}

void Request::setName(QString name)
{
    if (name == m_name) return;
    m_name = name;
    setEdited(true);
    emit nameChanged(name);
}

void Request::setContentType(QString contentType)
{
    if (contentType == m_contentType) return;
    m_contentType = contentType;
    setEdited(true);
    emit contentTypeChanged(contentType);
}

void Request::setDocumentation(QString documentation)
{
    if (documentation == m_documentation) return;
    m_documentation = documentation;
    setEdited(true);
    emit documentationChanged(documentation);
}

QString Request::toUrlencoded(QList<ParamModel *> params)
{
    QString urlencoded;
    for (ParamModel *param: params)
    {
        if (param->key.isEmpty() && param->value.isEmpty()) continue;
        if (!param->enabled) continue;

        if (!urlencoded.isEmpty()) {
            urlencoded += "&";
        }
        urlencoded += QUrl::toPercentEncoding(param->key) + "=" + QUrl::toPercentEncoding(param->value);
    }

    return urlencoded;
}

QList<ParamModel *> Request::fromUrlencoded(QString urlencoded)
{
    QStringList params = urlencoded.split('&');
    QList<ParamModel *> result;

    for (QString param: params)
    {
        if (param.isEmpty()) continue;

        auto keyAndValue = param.split('=');
        keyAndValue.append("");

        QString key = QUrl::fromPercentEncoding(keyAndValue[0].toUtf8());
        QString value = QUrl::fromPercentEncoding(keyAndValue[1].toUtf8());
        result.append(new ParamModel(key, value));
    }

    return result;
}

void Request::setEdited(bool edited)
{
    if (m_edited == edited) return;

    m_edited = edited;
    emit editedChanged(edited);
}

void Request::handleQueryParamsChanged()
{
    // Update query in url
    QString path = m_url.section('?', 0, 0);
    QString oldUrlencoded = m_url.section('?', 1);
    QString newUrlencoded = toUrlencoded(m_queryParams);
    if (oldUrlencoded == newUrlencoded)
        return;
    else
        setEdited(true);

    if (!newUrlencoded.isEmpty())
        m_url = path + "?" + newUrlencoded;
    else
        m_url = path;
    emit urlChanged(m_url);
}

void Request::handleBodyParamsChanged()
{
    setEdited(true);
}

void Request::handleHeadersChanged()
{
    setEdited(true);
}

