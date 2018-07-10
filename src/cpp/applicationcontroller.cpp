#include "applicationcontroller.h"
#include "guihelper.h"

void ApplicationController::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QString text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
    case QtDebugMsg:
        text += QString("Debug: %1 \n").arg(msg);
        break;
    case QtInfoMsg:
        text += QString("Info: %1 \n").arg(msg);
        break;
    case QtWarningMsg:
        text += QString("Warning: %1 \n").arg(msg);
        break;
    case QtCriticalMsg:
        text += QString("Critical error: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtFatalMsg:
        text += QString("Fatal error: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    }

    QByteArray b = text.toLocal8Bit();
    fprintf(stderr, b.constData());
    fflush(stderr); // Force output to be printed right away

    QFile logFile(appController()->logFilePath());
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&logFile);
    ts << text;

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
    //mCommunicator = new BluetoothCommunicator();
    // or:
    mCommunicator = new SerialCommunicator();

    QObject::connect(mCommunicator, &Communicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    QObject::connect(mCommunicator, &Communicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    //QObject::connect(mCommunicator, &Communicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    QObject::connect(mCommunicator, &Communicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);

    mRoutineController = new RoutineController(mCommunicator); // TODO: check if this is really the best way to do this (a singleton may be better)

    mLogFilePath = "log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz") + ".txt";
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
    /*
     * Currently I can't use proper scanning / connection functions, I think due to limitations with the ESP32 (SDP browsing not possible).
     * The workaround for now is to use mSocket->connectToService(QBluetoothAddress, Channel).
     * To find the bluetooth address of the device (on Linux), run:
     *
     *  $ hcitool scan
     *
     * And find the address of the device -- for example 24:0A:C4:05:7E:EA
     * Then, run
     *
     *  $ sdptool records 24:0A:C4:05:7E:EA | grep Channel
     *
     * This will tell you the channel number (e.g. 2).
     */
    
    mCommunicator->connect();

    //mCommunicator->connect(QBluetoothAddress("24:0A:C4:05:7E:EA"), 2); // devkitC
    //mCommunicator->connect(QBluetoothAddress("24:0A:C4:83:28:E2"), 2); // esp32thing
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

void ApplicationController::onCommunicatorStatusChanged(BluetoothCommunicator::ConnectionStatus newStatus)
{
    qDebug() << "App controller: communicator status changed";
    if (newStatus == Communicator::Connected)
        mCommunicator->refreshAll();

    emit connectionStatusChanged(mCommunicator->getConnectionStatusString());
}
