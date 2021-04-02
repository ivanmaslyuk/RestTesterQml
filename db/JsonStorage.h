#ifndef JSONSTORAGE_H
#define JSONSTORAGE_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include "Models/requesttreenode.h"

class JsonStorage : public QObject
{
    Q_OBJECT
public:
    explicit JsonStorage(QString filaname, QObject *parent);

    Q_INVOKABLE QVariant get(QString key);
    Q_INVOKABLE void set(QString key, QVariant value);

private:
    QFile *getFile();
    void loadData();
    void saveData();

    QString m_filename;
    QJsonObject m_object;
    QFile *m_file;

signals:
    void settingChanged(QString key, QVariant value);
};


#endif // JSONSTORAGE_H
