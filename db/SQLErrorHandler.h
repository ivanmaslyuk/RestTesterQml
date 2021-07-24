#ifndef SQLERRORHANDLER_H
#define SQLERRORHANDLER_H


#include <QSqlQuery>


class SQLErrorHandler
{
public:
    SQLErrorHandler() = delete;

    static void handleErrors(QSqlQuery query);
};

#endif // SQLERRORHANDLER_H
