#include "SQLErrorHandler.h"

#include <QSqlError>
#include <QDebug>

void SQLErrorHandler::handleErrors(QSqlQuery query)
{
    QSqlError error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        QString errorName;

        switch (error.type()) {
        case QSqlError::NoError:
            errorName = "NoError";
            break;
        case QSqlError::ConnectionError:
            errorName = "ConnectionError";
            break;
        case QSqlError::StatementError:
            errorName = "StatementError";
            break;
        case QSqlError::TransactionError:
            errorName = "TransactionError";
            break;
        case QSqlError::UnknownError:
            errorName = "UnknownError";
            break;
        }

        qDebug() << "Error" << errorName << "while executing query:" << query.lastQuery()
                 << "Error text: " << error.text();
    }
}
