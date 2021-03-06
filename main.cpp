#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>
#include <QQmlContext>
#include "app.h"
#include "Models/treemodel.h"
#include "Theme.h"
#include "testing/TestRunner.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QFont font("Roboto");
    app.setFont(font);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    App appObject;
    engine.rootContext()->setContextProperty("app", &appObject);

    Theme theme;
    engine.rootContext()->setContextProperty("theme", &theme);

    TestRunner testRunner;
    engine.rootContext()->setContextProperty("testRunner", &testRunner);

    engine.load(url);

    return app.exec();
}
