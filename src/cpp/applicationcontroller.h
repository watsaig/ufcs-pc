#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QQmlEngine>

#include "communicator.h"
/*
 *
 * Application controller interfaces between the GUI (QML) and the Communicator, which talks to the microcontroller.
 * To be able to update the right GUI element when new information comes from the microcontroller, it has a list of pointers to the GUI elements
 * (switches and other elements). Actually a map. So that when Communicator says "valve number 3 was switched on", it can find the right GUI element and toggle it.
 * */

class PCHelper;
class ValveSwitchHelper;

class ApplicationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
private:
    ApplicationController(QObject *parent = nullptr);

public:
    static ApplicationController* appController();
    virtual ~ApplicationController() {}

    Q_INVOKABLE void setValve(int valveNumber, bool open) { mCommunicator.setValve(valveNumber, open); }
    Q_INVOKABLE void setPump(int pumpNumber, bool on) { mCommunicator.setPump(pumpNumber, on); }
    Q_INVOKABLE void setPressure(int controllerNumber, double pressure) { mCommunicator.setPressure(controllerNumber, pressure); }

    double minPressure(int controllerNumber) { return mCommunicator.minPressure(controllerNumber); }
    double maxPressure(int controllerNumber) { return mCommunicator.maxPressure(controllerNumber); }

    QString connectionStatus();

    void registerPCHelper(int controllerNumber, PCHelper* instance);
    void registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance);

signals:
    void connectionStatusChanged(QString newStatus);

private slots:
    void onValveStateChanged(int valveNumber, bool open);
    void onPumpStateChanged(int pumpNumber, bool on);
    void onPressureChanged(int controllerNumber, double pressure);

    void onCommunicatorStatusChanged(Communicator::ConnectionStatus newStatus);

private:

    Communicator mCommunicator;

    QMap<int, ValveSwitchHelper*> mQmlValveSwitches;
    QMap<int, PCHelper*> mQmlPressureControllers; // To do (?): allow several instances of PCHelper* per controller number
};

#endif // APPLICATIONCONTROLLER_H
