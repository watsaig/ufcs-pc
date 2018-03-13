#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include "src/cpp/communicator.h"
#include "src/cpp/applicationcontroller.h"
#include "src/cpp/guihelper.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<PCHelper>("org.example.ufcs", 1, 0, "PCHelper");
    qmlRegisterType<ValveSwitchHelper>("org.example.ufcs", 1, 0, "ValveSwitchHelper");
    qmlRegisterSingletonType(QUrl("qrc:/src/qml/Style.qml"), "org.example.ufcs", 1, 0, "Style"); // an alternative to this not-very-clean solution is to use a qmldir file. This way the QML-only stuff would stay separate from C++.

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Backend", ApplicationController::appController());
    engine.rootContext()->setContextProperty("RoutineController", ApplicationController::appController()->routineController());

    engine.load(QUrl(QLatin1String("qrc:/src/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
