#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <functional>
#include "JsonStorage.h"
#include "Models/request.h"

JsonStorage::JsonStorage(QString filename, QObject *parent = nullptr)
    : QObject(parent)
{
    m_filename = filename;
    m_file = getFile();
    loadData();
}

QVariant JsonStorage::get(QString key)
{
    return m_object[key].toVariant();
}

void JsonStorage::set(QString key, QVariant value)
{
    m_object[key] = QJsonValue::fromVariant(value);
    emit settingChanged(key, value);
    saveData();
}

void JsonStorage::loadData()
{
    QByteArray text;
    if (m_file->open(QFile::ReadWrite)) {
        text = m_file->readAll();
        m_file->close();
    } else {
        qDebug() << "Could not open data file";
        m_object = QJsonObject();
    }

    if (text.isEmpty())
        text = "{}";

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(text, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Could not parse json: " << parseError.errorString();
        m_object = QJsonObject();
    }

    m_object = doc.object();
}

void JsonStorage::saveData()
{
    QJsonDocument doc(m_object);

    if (m_file->open(QFile::WriteOnly)) {
        m_file->write(doc.toJson());
        m_file->close();
    } else {
        qDebug() << "Could not open data file for writing";
    }
}

QFile *JsonStorage::getFile()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDataPath.isEmpty()) {
        qDebug() << QString("Writable app data location not found");
        return nullptr;
    }
    QDir appDataDir = QDir(appDataPath);
    if (!appDataDir.exists()) {
        bool created = QDir().mkpath(appDataPath);
        if (!created) {
            qDebug() << QString("Could not create app data directory");
            return nullptr;
        }
    }

    QFile *file = new QFile(appDataDir.absoluteFilePath(m_filename));
    return file;
}
