#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSharedMemory>
#include "QmlCompressControl.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

//    QmlCompressControl model;
//    engine.rootContext()->setContextProperty("qmlCompressControl", &model);
    qmlRegisterType<QmlCompressControl>("QmlCompressControl",1,0,"QmlCompressControl");

    QSharedMemory singleton(app.applicationName());
    if(!singleton.create(1))  {    //已经存在的
        exit(0);
    }

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
