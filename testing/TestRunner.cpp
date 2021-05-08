#include "TestRunner.h"
#include <QDebug>
#include <QFile>

TestRunner::TestRunner(QObject *parent) : QObject(parent)
{
    m_engine = new QJSEngine(this);
    m_engine->installExtensions(QJSEngine::AllExtensions);

    QFile file(":/js/testing/TestUtils.js");
    file.open(QFile::ReadOnly);
    if (!file.isOpen())
        qDebug() << "Could not open TestUtils.js";
    m_jsTestLibrary = file.readAll();
    file.close();
}

QVariantList TestRunner::runTests(Request *request, Response *response)
{
    reset();

    // Convert request data to JS values
    QVariantMap requestHeaders;
    for (ParamModel *param : request->headers())
        requestHeaders[param->key] = param->value;

    QVariantMap requestData;
    requestData["headers"] = requestHeaders;
    requestData["method"] = request->method();
    requestData["url"] = request->url();
    requestData["body"] = request->rawData();
    m_engine->globalObject().setProperty("__request", m_engine->toScriptValue(requestData));

    // Convert response data to JS values
    QVariantMap responseHeaders;
    for (ParamModel *header : response->headers())
        responseHeaders[header->key] = header->value;

    QVariantMap responseData;
    responseData["headers"] = responseHeaders;
    responseData["status"] = response->status();
    responseData["body"] = response->body();
    responseData["time"] = response->time();
    m_engine->globalObject().setProperty("__response", m_engine->toScriptValue(responseData));

    // Run tests
    m_engine->evaluate(request->tests());
    QJSValue results = m_engine->globalObject().property("__testResults");

    return results.toVariant().toList();
}

void TestRunner::reset()
{
    m_engine->evaluate(m_jsTestLibrary);
}
