#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QQmlEngine>

#include "bluetoothcommunicator.h"
#include "serialcommunicator.h"

#include "routinecontroller.h"

/*
 *
 * Application controller interfaces between the GUI (QML) and the Communicator, which talks to the microcontroller.
 * To be able to update the right GUI element when new information comes from the microcontroller, it has a list of pointers to the GUI elements
 * (switches and other elements). Actually a map. So that when Communicator says "valve number 3 was switched on", it can find the right GUI element and toggle it.
 * */

class PCHelper;
class ValveSwitchHelper;
class PumpSwitchHelper;

class ApplicationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(QVariantList logMessageList READ log NOTIFY newLogMessage)
    Q_PROPERTY(QString appVersion READ appVersion)

private:
    ApplicationController(QObject *parent = nullptr);

public:
    static ApplicationController* appController();
    virtual ~ApplicationController();

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    Q_INVOKABLE void connect();

    Q_INVOKABLE void setValve(int valveNumber, bool open) { mCommunicator->setValve(valveNumber, open); }
    Q_INVOKABLE void setPump(int pumpNumber, bool on) { mCommunicator->setPump(pumpNumber, on); }
    Q_INVOKABLE void setPressure(int controllerNumber, double pressure) { mCommunicator->setPressure(controllerNumber, pressure); }

    Q_INVOKABLE void requestRefresh() { mCommunicator->refreshAll(); }

    double minPressure(int controllerNumber) { return mCommunicator->minPressure(controllerNumber); }
    double maxPressure(int controllerNumber) { return mCommunicator->maxPressure(controllerNumber); }

    QString appVersion() { return GIT_VERSION; }
    QString connectionStatus();

    void registerPCHelper(int controllerNumber, PCHelper* instance);
    void registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance);
    void registerPumpSwitchHelper(int pumpNumber, PumpSwitchHelper* instance);

    RoutineController* routineController() { return mRoutineController; }

    QString logFilePath() { return mLogFilePath; }

    Q_INVOKABLE QVariantList log() { return mLog; }
    void addToLog(QVariant entry);

    QSettings* settings() { return mSettings; }

signals:
    void connectionStatusChanged(QString newStatus);
    void newLogMessage(QVariant newMessage);

private slots:
    void onValveStateChanged(int valveNumber, bool open);
    void onPumpStateChanged(int pumpNumber, bool on);
    void onPressureChanged(int controllerNumber, double pressure);

    void onCommunicatorStatusChanged(BluetoothCommunicator::ConnectionStatus newStatus);

private:

    Communicator * mCommunicator;
    RoutineController * mRoutineController;

    QMap<int, PCHelper*> mQmlPressureControllers;
    QMap<int, ValveSwitchHelper*> mQmlValveSwitches;
    QMap<int, PumpSwitchHelper*> mQmlPumpSwitches;

    QString mLogFilePath;
    QVariantList mLog;

    QSettings * mSettings;
};

#endif // APPLICATIONCONTROLLER_H
