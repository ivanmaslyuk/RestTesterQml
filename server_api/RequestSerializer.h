#ifndef REQUESTSERIALIZER_H
#define REQUESTSERIALIZER_H

#include <QJsonObject>

#include "Models/request.h"

class RequestSerializer
{
public:
    RequestSerializer() = delete;

    static QJsonObject toJson(Request *request);
    static Request *fromJson(QJsonObject json, QObject *node);
};

#endif // REQUESTSERIALIZER_H
