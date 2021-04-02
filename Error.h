#ifndef ERROR_H
#define ERROR_H

#include <QString>


class Error
{
public:
    Error();
    Error(QString message);

    QString message() const;
    void setMessage(QString newMessage);

protected:
    QString m_message;
};

#endif // ERROR_H
