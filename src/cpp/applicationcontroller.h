#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QQmlEngine>

#include "bluetoothcommunicator.h"
#include "serialcommunicator.h"

#include "routinecontroller.h"

/*
 * ApplicationController is the backend of the application. Either the brains of the operation or middle management,
 * depending on who you ask.
 *
 * It relays commands between the user interface and the serial communicator, saves and loads settings, etc.
 *
 * To be able to update GUI elements based on information received from the microcontroller, AC has QMaps of
 * components, with a label (the valve number, for example) referring to a pointer to a GUI Helper object.
 * These are the backend of the controls (valve switches, pump switches and pressure controllers) shown in the GUI.
 *
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
    Q_PROPERTY(bool darkMode READ isDarkModeEnabled WRITE setDarkModeEnabled NOTIFY darkModeChanged)
    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(bool graphicalControlEnabled READ isGraphicalControlEnabled WRITE setGraphicalControlEnabled)
    Q_PROPERTY(int baudRate READ serialBaudRate WRITE setSerialBaudRate)
    Q_PROPERTY(bool bluetoothEnabled READ isBluetoothEnabled CONSTANT)
    Q_PROPERTY(bool denseThemeEnabled READ isDenseThemeEnabled WRITE setDenseThemeEnabled NOTIFY denseThemeChanged)

private:
    ApplicationController(QObject *parent = nullptr);

public:
    static ApplicationController* appController();
    virtual ~ApplicationController();

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    Q_INVOKABLE void connect();
    Q_INVOKABLE void requestRefresh() { mCommunicator->refreshAll(); }

    int nValves();
    int nPumps();
    int nPressureControllers();
    double minPressure(int controllerNumber);
    double maxPressure(int controllerNumber);

    QString appVersion() { return GIT_VERSION; }
    QString connectionStatus();

    void registerPCHelper(int controllerNumber, PCHelper* instance);
    void registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance);
    void registerPumpSwitchHelper(int pumpNumber, PumpSwitchHelper* instance);

    RoutineController* routineController() { return mRoutineController; }

    QString logFilePath() { return mLogFilePath; }

    QVariantList log() { return mLog; }
    void addToLog(QVariant entry);

    bool isBluetoothEnabled() { return mBluetoothEnabled; }

    // Settings
    bool isDarkModeEnabled();
    void setDarkModeEnabled(bool enabled = false);

    bool isDenseThemeEnabled();
    void setDenseThemeEnabled(bool enabled);

    int windowWidth();
    void setWindowWidth(int width);
    int windowHeight();
    void setWindowHeight(int height);

    Q_INVOKABLE QUrl routineFolder();
    Q_INVOKABLE void setRoutineFolder(QUrl folder);

    Q_INVOKABLE QString valveLabel(int valveNumber);
    Q_INVOKABLE void setValveLabel(int valveNumber, QString label);

    bool isGraphicalControlEnabled(); // isGraphicalControlEnabled
    void setGraphicalControlEnabled(bool show); // setGraphicalControlEnabled

    QMap<QString, QUrl> graphicalControlScreenSources();
    Q_INVOKABLE void setGraphicalControlScreenSources(QMap<QString, QUrl> sources);
    Q_INVOKABLE QVariantList graphicalControlScreenLabels();
    Q_INVOKABLE QString currentGraphicalControlScreenLabel();
    Q_INVOKABLE void setCurrentGraphicalControlScreen(QString label);
    Q_INVOKABLE QUrl currentGraphicalControlScreenURL();

    uint serialBaudRate();
    void setSerialBaudRate(int rate);

    QSettings* settings() { return mSettings; }

public slots:
    void setValve(int valveNumber, bool open) { mCommunicator->setValve(valveNumber, open); }
    void setPump(int pumpNumber, bool on) { mCommunicator->setPump(pumpNumber, on); }
    void setPressure(int controllerNumber, double pressure) { mCommunicator->setPressure(controllerNumber, pressure); }

signals:
    void connectionStatusChanged(QString newStatus);
    void newLogMessage(QVariant newMessage);
    void darkModeChanged(bool enabled);
    void denseThemeChanged(bool enabled);
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
    bool mBluetoothEnabled;

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
