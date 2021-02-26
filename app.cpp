#include <QDebug>
#include "app.h"

App::App(QObject *parent) : QObject(parent)
{
    qDebug() << "REST Tester app launched!";
}

RequestModel App::activeRequest() const
{
    return m_activeRequest;
}

void App::setActiveRequest(RequestModel request)
{
    m_activeRequest = request;
    emit activeRequestChanged(m_activeRequest);
}

QString App::name() const
{
    return m_name;
}

void App::setName(QString value)
{
    m_name = value;
    emit nameChanged(value);
}
