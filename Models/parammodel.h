#ifndef PARAMMODEL_H
#define PARAMMODEL_H

#include <QString>

struct ParamModel
{
    ParamModel(QString key, QString value, bool enabled = true);

    static QList<ParamModel *> listFromJson(QString json);
    static QString listToJson(QList<ParamModel *> list);

    bool isEmpty() const;

    bool enabled;
    QString key;
    QString value;
};

#endif // PARAMMODEL_H
