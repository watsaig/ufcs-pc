#include "applicationcontroller.h"
#include "guihelper.h"

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
    QObject::connect(&mCommunicator, &BluetoothCommunicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    QObject::connect(&mCommunicator, &BluetoothCommunicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    //QObject::connect(&mCommunicator, &BluetoothCommunicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    QObject::connect(&mCommunicator, &BluetoothCommunicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);

    mRoutineController = new RoutineController(&mCommunicator); // TODO: check if this is really the best way to do this (a singleton may be better)
}

ApplicationController::~ApplicationController()
{
    delete mRoutineController;
}

QString ApplicationController::connectionStatus()
{
    return mCommunicator.getConnectionStatusString();
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
    
    mCommunicator.connect();

    //mCommunicator.connect(QBluetoothAddress("24:0A:C4:05:7E:EA"), 2); // devkitC
    //mCommunicator.connect(QBluetoothAddress("24:0A:C4:83:28:E2"), 2); // esp32thing
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
    if (newStatus == mCommunicator.Connected)
        mCommunicator.refreshAll();

    emit connectionStatusChanged(mCommunicator.getConnectionStatusString());
}
