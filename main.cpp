#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>
#include <QQmlContext>
#include "app.h"
#include "Models/treemodel.h"
#include "ui/PlainText.h"
#include <qqml.h>

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

    qmlRegisterType<PlainText>("RestTesterUI", 1, 0, "PlainText");

    App appObject;
    engine.rootContext()->setContextProperty("app", &appObject);

    engine.load(url);
//    return 0;

    return app.exec();
}
