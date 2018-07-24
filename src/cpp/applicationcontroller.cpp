#include "applicationcontroller.h"
#include "guihelper.h"

void ApplicationController::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString messageType;
    QString message;

    switch (type) {
    case QtDebugMsg:
        messageType  = "Debug";
        message = msg;
        break;
    case QtInfoMsg:
        messageType  = "Info";
        message = msg;
        break;
    case QtWarningMsg:
        messageType  = "Warning";
        message = msg;
        break;
    case QtCriticalMsg:
        messageType  = "Critical error";
        message = QString("%1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtFatalMsg:
        messageType  = "Fatal error";
        message = QString("%1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    }

    QString text = timestamp + " " + messageType + ": " + message + "\n";

    QByteArray b = text.toLocal8Bit();
    fprintf(stderr, b.constData());
    fflush(stderr); // Force output to be printed right away

    QFile logFile(appController()->logFilePath());
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&logFile);
        ts << text;
    }

    // Logs are stored in a fragmented way to make rich markup easier in QML
    QStringList toAdd;
    toAdd << timestamp << messageType << message;
    appController()->addToLog(toAdd);
}

ApplicationController* singleton = nullptr;

ApplicationController* ApplicationController::appController()
{
    if (singleton == nullptr) {
        singleton = new ApplicationController();
    }
    return singleton;
}

ApplicationController::ApplicationController(QObject *parent) : QObject(parent)
{
    mCommunicator = new BluetoothCommunicator();
    // or:
    //mCommunicator = new SerialCommunicator();

    QObject::connect(mCommunicator, &Communicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    QObject::connect(mCommunicator, &Communicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    //QObject::connect(mCommunicator, &Communicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    QObject::connect(mCommunicator, &Communicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);

    mRoutineController = new RoutineController(mCommunicator); // TODO: check if this is really the best way to do this (a singleton may be better)

    mLogFilePath = "log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + ".txt";

    // This is used by mSettings
    QCoreApplication::setApplicationName("ufcs-pc");
    QCoreApplication::setOrganizationName("Watsaig");
    QCoreApplication::setOrganizationDomain("watsaig.com");
    mSettings = new QSettings();
}

ApplicationController::~ApplicationController()
{
    delete mRoutineController;
    delete mCommunicator;
}

QString ApplicationController::connectionStatus()
{
    return mCommunicator->getConnectionStatusString();
}

void ApplicationController::connect()
{
    mCommunicator->connect();
}

void ApplicationController::registerPCHelper(int controllerNumber, PCHelper* instance)
{
    mQmlPressureControllers[controllerNumber] = instance;
}

void ApplicationController::registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance)
{
    mQmlValveSwitches[valveNumber] = instance;
}

void ApplicationController::onValveStateChanged(int valveNumber, bool open)
{
    qInfo() << "Valve" << valveNumber << (open ? "opened" : "closed");

    if (mQmlValveSwitches.contains(valveNumber))
        mQmlValveSwitches[valveNumber]->setState(open);
}

void ApplicationController::onPumpStateChanged(int pumpNumber, bool on)
{
    Q_UNUSED(pumpNumber)
    Q_UNUSED(on)

    // Not used for now, probably not necessary
}

void ApplicationController::onPressureChanged(int controllerNumber, double pressure)
{
    //qInfo() << "Measured pressure (normalized) on controller" << controllerNumber << ":" << pressure;
    if (pressure < 0)
        qDebug() << "pressure: " << pressure;
    if (mQmlPressureControllers.contains(controllerNumber))
        mQmlPressureControllers[controllerNumber]->setMeasuredValue(pressure);
}

void ApplicationController::onCommunicatorStatusChanged(Communicator::ConnectionStatus newStatus)
{
    qDebug() << "App controller: communicator status changed to" << mCommunicator->getConnectionStatusString();

    if (newStatus == Communicator::Connected)
        mCommunicator->refreshAll();

    emit connectionStatusChanged(mCommunicator->getConnectionStatusString());
}
