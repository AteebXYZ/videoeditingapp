#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "MediaPoolManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    MediaPoolManager mediaPool;
    engine.rootContext()->setContextProperty("MediaPoolManager", &mediaPool);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
