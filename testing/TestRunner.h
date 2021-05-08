#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include <QJSEngine>
#include "Models/request.h"
#include "Models/Response.h"

class TestRunner : public QObject
{
    Q_OBJECT
public:
    explicit TestRunner(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList runTests(Request *request, Response *response);

private:
    QJSEngine *m_engine;
    QString m_jsTestLibrary;

    void reset();

};

#endif // TESTRUNNER_H
