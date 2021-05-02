#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "parammodel.h"

ParamModel::ParamModel(QString key, QString value, bool enabled)
{
    this->key = key;
    this->value = value;
    this->enabled = enabled;
}

QList<ParamModel *> ParamModel::listFromJson(QString json)
{
    QList<ParamModel *> params;
    for (QJsonValue value : QJsonDocument::fromJson(json.toUtf8()).array())
        params.append(new ParamModel(value["key"].toString(),
                      value["value"].toString(), value["enabled"].toBool()));

    return params;
}

QString ParamModel::listToJson(QList<ParamModel *> list)
{
    QJsonArray array;
    for (ParamModel *param : list)
        array.append(QJsonObject({{"key", param->key}, {"value", param->value},
                                  {"enabled", param->enabled}}));

    QJsonDocument doc;
    doc.setArray(array);
    return doc.toJson();
}

bool ParamModel::isEmpty() const
{
    return key == "" && value == "" && enabled;
}
