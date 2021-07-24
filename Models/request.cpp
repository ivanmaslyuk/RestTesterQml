#include "request.h"
#include <QUrl>
#include "Error.h"
#include "db/SQLErrorHandler.h"

Request::Request(QObject *parent) : QObject(parent)
{
    m_queryParamsModel = new ParamsItemModel(&m_queryParams, false, this);
    m_dataParamsModel = new ParamsItemModel(&m_dataParams, false, this);
    m_headersModel = new ParamsItemModel(&m_headers, false, this);

    connect(m_queryParamsModel, &ParamsItemModel::dataChanged,
            this, &Request::handleQueryParamsChanged);
    connect(m_dataParamsModel, &ParamsItemModel::dataChanged,
            this, &Request::handleBodyParamsChanged);
    connect(m_headersModel, &ParamsItemModel::dataChanged,
            this, &Request::handleHeadersChanged);
    m_localId = -1;
    m_uuid = "";
}

Request::Request(int localId, QObject *parent) : Request(parent)
{
    m_localId = localId;
}

Request::Request(QString uuid, QObject *parent) : Request(parent)
{
    m_uuid = uuid;
}

Request *Request::fromSqlQuery(QSqlQuery query)
{
    QString queryParamsJson = query.value("query_params_json").toString();
    QList<ParamModel *> queryParams = ParamModel::listFromJson(queryParamsJson);

    QString dataParamsJson = query.value("data_params_json").toString();
    QList<ParamModel *> dataParams = ParamModel::listFromJson(dataParamsJson);

    QString headersJson = query.value("headers_json").toString();
    QList<ParamModel *> headers = ParamModel::listFromJson(headersJson);

    Request *request = new Request;
    request->setLocalId(query.value("id").toInt());
    request->setUuid(query.value("uuid").toString());
    request->setUrl(query.value("url").toString());
    request->setQueryParams(queryParams);
    request->setDataParams(dataParams);
    request->setHeaders(headers);
    request->setRawData(query.value("raw_data").toString());
    request->setMethod(query.value("method").toString());
    request->setName(query.value("name").toString());
    request->setContentType(query.value("content_type").toString());
    request->setDocumentation(query.value("documentation").toString());
    request->setTests(query.value("tests").toString());
    request->setEdited(false);

    return request;
}

void Request::save(QString connectionName)
{
    if (m_localId == -1)
        throw Error("Cannot save Request that hasn't been created first.");

    QSqlDatabase db = QSqlDatabase::database(connectionName);

    QSqlQuery query(db);
    query.prepare("UPDATE request SET url = :url, query_params_json = :query_params_json, "
                  "data_params_json = :data_params_json, headers_json = :headers_json, raw_data = :raw_data,"
                  "method = :method, name = :name, content_type = :content_type, documentation = :documentation,"
                  "tests = :tests WHERE id = :local_id;");
    query.bindValue(":url", m_url);
    query.bindValue(":query_params_json", ParamModel::listToJson(m_queryParams));
    query.bindValue(":data_params_json", ParamModel::listToJson(m_dataParams));
    query.bindValue(":headers_json", ParamModel::listToJson(m_headers));
    query.bindValue(":raw_data", m_rawData);
    query.bindValue(":method", m_method);
    query.bindValue(":name", m_name);
    query.bindValue(":content_type", m_contentType);
    query.bindValue(":documentation", m_documentation);
    query.bindValue(":tests", m_tests);
    query.bindValue("local_id", m_localId);

    query.exec();
    SQLErrorHandler::handleErrors(query);
}

QByteArray Request::data()
{
    if (m_contentType == "application/x-www-form-urlencoded") {
        return toUrlencoded(m_dataParams).toUtf8();
    }

    return m_rawData.toUtf8();
}

QString Request::uuid()
{
    return m_uuid;
}

void Request::setUuid(QString uuid)
{
    m_uuid = uuid;
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

QString Request::tests() const
{
    return m_tests;
}

void Request::setTests(QString value)
{
    if (value == m_tests)
        return;

    m_tests = value;
    emit testsChanged(value);
    setEdited(true);
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

