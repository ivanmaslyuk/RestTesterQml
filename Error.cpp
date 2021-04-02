#include <QDebug>
#include "Error.h"

Error::Error()
{

}

Error::Error(QString message)
{
    setMessage(message);
}

QString Error::message() const
{
    return m_message;
}

void Error::setMessage(QString newMessage)
{
    qDebug() << "Error:" << newMessage;
    m_message = newMessage;
}
