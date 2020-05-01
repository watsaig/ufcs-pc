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
    Q_PROPERTY(QString logFilePath READ logFilePath)
    Q_PROPERTY(bool darkMode READ darkMode WRITE setDarkMode NOTIFY darkModeChanged)
    Q_PROPERTY(int windowWidth READ loadWindowWidth WRITE saveWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ loadWindowHeight WRITE saveWindowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(bool showGraphicalControl READ loadShowGraphicalControl WRITE saveShowGraphicalControl)
    Q_PROPERTY(int baudRate READ loadBaudRate WRITE saveBaudRate)
    Q_PROPERTY(bool useBluetooth READ useBluetooth CONSTANT)

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

    bool useBluetooth() { return mUseBluetooth; }

    // Settings
    bool darkMode();
    void setDarkMode(bool enabled = false);

    int loadWindowWidth();
    void saveWindowWidth(int width);

    int loadWindowHeight();
    void saveWindowHeight(int height);

    Q_INVOKABLE QUrl loadRoutineFolder();
    Q_INVOKABLE void saveRoutineFolder(QUrl folder);

    Q_INVOKABLE QString loadValveLabel(int valveNumber);
    Q_INVOKABLE void saveValveLabel(int valveNumber, QString label);

    bool loadShowGraphicalControl();
    void saveShowGraphicalControl(bool show);

    int loadBaudRate();
    void saveBaudRate(int rate);

    QSettings* settings() { return mSettings; }

signals:
    void connectionStatusChanged(QString newStatus);
    void newLogMessage(QVariant newMessage);
    void darkModeChanged(bool enabled);
    void windowWidthChanged(int width);
    void windowHeightChanged(int height);

private slots:
    void onValveStateChanged(int valveNumber, bool open);
    void onPumpStateChanged(int pumpNumber, bool on);
    void onPressureChanged(int controllerNumber, double pressure);
    void onPressureSetpointChanged(int controllerNumber, double pressure);

    void onCommunicatorStatusChanged(BluetoothCommunicator::ConnectionStatus newStatus);

private:
    /// True if the communicator uses bluetooth; false if USB
    bool mUseBluetooth;

    Communicator * mCommunicator;
    RoutineController * mRoutineController;

    QMap<int, QList<PCHelper*> > mQmlPressureControllers;
    QMap<int, QList<ValveSwitchHelper*> > mQmlValveSwitches;
    QMap<int, PumpSwitchHelper*> mQmlPumpSwitches;

    QString mLogFilePath;
    QVariantList mLog;

    QSettings * mSettings;
};

#endif // APPLICATIONCONTROLLER_H
