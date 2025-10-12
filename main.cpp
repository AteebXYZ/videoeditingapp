#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "backend.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion"); // or whatever style you use

    QQmlApplicationEngine engine;

    // 👇 Add this line
    // engine.addImportPath("qrc:/qml/components"); // if you’re using a qrc
    // // OR, if loading from disk:
    // engine.addImportPath("qml/components");

    Backend *backend = new Backend(&app);
    engine.rootContext()->setContextProperty("backend", backend);
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
