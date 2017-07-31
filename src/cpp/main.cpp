#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/cpp/communicator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/src/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    Communicator comms;
    comms.connect();

    return app.exec();
}
