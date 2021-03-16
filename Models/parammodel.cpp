#include "parammodel.h"

ParamModel::ParamModel(QString key, QString value, bool enabled)
{
    this->key = key;
    this->value = value;
    this->enabled = enabled;
}

bool ParamModel::isEmpty() const
{
    return key == "" && value == "" && enabled;
}
