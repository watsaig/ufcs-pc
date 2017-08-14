#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/cpp/communicator.h"
#include "src/cpp/applicationcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ApplicationController controller;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Backend", &controller);

    engine.load(QUrl(QLatin1String("qrc:/src/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
