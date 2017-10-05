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
    mCommunicator.connect();

    connect(&mCommunicator, &Communicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    connect(&mCommunicator, &Communicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    //connect(&mCommunicator, &Communicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    connect(&mCommunicator, &Communicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);
}

QString ApplicationController::connectionStatus()
{
    return mCommunicator.getConnectionStatusString();
}

void ApplicationController::registerPCHelper(int controllerNumber, PCHelper* instance)
{
    mQmlPressureControllers[controllerNumber] = instance;
}

void ApplicationController::registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance)
{
    mQmlValveSwitches[valveNumber] = instance;
    if (mQmlValveSwitches.size() == 23) {
        mCommunicator.refreshAll(); // shitty duct tape solution. TODO: do this properly, e.g. when the QML window is fully loaded and shown
    }
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
    Q_UNUSED(newStatus)
    emit connectionStatusChanged(mCommunicator.getConnectionStatusString());
}
