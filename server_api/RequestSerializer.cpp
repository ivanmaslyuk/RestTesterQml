#include <QJsonDocument>

#include "RequestSerializer.h"

#include "Models/parammodel.h"
#include "Error.h"

QJsonObject RequestSerializer::toJson(Request *request)
{
    if (request->uuid().isEmpty())
        throw Error("Request::toJson(): Cannot serialize Request with no uuid");

    QJsonObject json;
    json["uuid"] = request->uuid();
    json["name"] = request->name();
    json["url"] = request->url();
    json["method"] = request->method();
    json["query_params"] = ParamModel::listToJson(request->queryParams());
    json["data_params"] = ParamModel::listToJson(request->dataParams());
    json["headers"] = ParamModel::listToJson(request->headers());
    json["raw_data"] = request->rawData();
    json["content_type"] = request->contentType();
    json["documentation"] = request->documentation();
    return json;
}

Request *RequestSerializer::fromJson(QJsonObject json, QObject *node)
{
    Request *request = new Request(json["uuid"].toString(), node);
    request->setName(json["name"].toString());
    request->setUrl(json["url"].toString());
    request->setMethod(json["method"].toString());
    request->setQueryParams(ParamModel::listFromJson(
                                json["query_params"].toString()));
    request->setDataParams(ParamModel::listFromJson(
                               json["data_params"].toString()));
    request->setHeaders(ParamModel::listFromJson(json["headers"].toString()));
    request->setRawData(json["raw_data"].toString());
    request->setContentType(json["content_type"].toString());
    request->setDocumentation(json["documentation"].toString());
    request->setEdited(false);
    return request;
}
