#include "applicationcontroller.h"
#include "guihelper.h"

void ApplicationController::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

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

    QString text = date + " " + time + " " + messageType + ": " + message + "\n";

    // Write to console
#ifdef LOG_TO_TERMINAL
    QByteArray b = text.toLocal8Bit();
    fprintf(stdout, b.constData());
    fflush(stdout); // Force output to be printed right away
#endif

    // Write to file
    QFile logFile(appController()->logFilePath());
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&logFile);
        ts << text;
    }

    else {
        QByteArray path = appController()->logFilePath().toLocal8Bit();
        fprintf(stderr, "Could not open log file for writing at %s", path.constData());
        fflush(stderr);
    }

    // Write to app

    // Logs are stored in a fragmented way to make rich markup easier in QML.
    // Date is omitted since not particularly useful within the app
    QStringList toAdd;
    toAdd << time << messageType << message;
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
    // Initialize mCommunicator. Can be either USB ("Serial") or Bluetooth. Windows
    // doesn't support Bluetooth, and Android doesn't support serial over USB (at least,
    // not without rooting your phone), so we can set defaults for those platforms.
    // For other platforms, use whichever one you prefer.
    // This has to be specified at compile time for now; run-time switching may be supported later.

#if defined(Q_OS_ANDROID)
    mUseBluetooth = true;
#else
    mUseBluetooth = false;
#endif

    if (mUseBluetooth)
        mCommunicator = new BluetoothCommunicator();
    else
        mCommunicator = new SerialCommunicator();

    QObject::connect(mCommunicator, &Communicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    QObject::connect(mCommunicator, &Communicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    QObject::connect(mCommunicator, &Communicator::pressureSetpointChanged, this, &ApplicationController::onPressureSetpointChanged);
    QObject::connect(mCommunicator, &Communicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    QObject::connect(mCommunicator, &Communicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);

    mRoutineController = new RoutineController(mCommunicator); // TODO: check if this is really the best way to do this (a singleton may be better)

    // Initialize log file path
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
    QDir d;
    if (!d.mkpath(dataLocation))
        fprintf(stderr, "Could not create directory for log file storage\n");

    QString fileName = "log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    mLogFilePath = QDir::cleanPath(dataLocation + "/" + fileName);

    QByteArray path = mLogFilePath.toLocal8Bit();
    fprintf(stdout, "Log file location: %s\n", path.constData());

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
    if (!mQmlPressureControllers.contains(controllerNumber))
        mQmlPressureControllers[controllerNumber] = QList<PCHelper*>();

    mQmlPressureControllers[controllerNumber].push_back(instance);
}

void ApplicationController::registerValveSwitchHelper(int valveNumber, ValveSwitchHelper* instance)
{
    if (!mQmlValveSwitches.contains(valveNumber))
        mQmlValveSwitches[valveNumber] = QList<ValveSwitchHelper*>();

    mQmlValveSwitches[valveNumber].push_back(instance);
}

void ApplicationController::registerPumpSwitchHelper(int pumpNumber, PumpSwitchHelper *instance)
{
    mQmlPumpSwitches[pumpNumber] = instance;
}

/**
 * @brief Add a message to the (internal) log, for access within the application.
 * @param entry A QStringList (or compatible type) with 3 elements: time, message type and message text.
 *
 * This log is only for access within the application. It is separate from the messages logged to file.
 * See ApplicationController::messageHandler for the rest of the logging code.
 */
void ApplicationController::addToLog(QVariant entry)
{
    mLog.append(entry);
    emit newLogMessage(entry);
}

bool ApplicationController::darkMode()
{
    return mSettings->value("darkMode", false).toBool();
}

/**
 * @brief Set the theme to dark or light mode.
 * @param enabled if true, theme is set to dark mode
 *
 * This is handled in C++ so that the change can be persisted, and the appropriate QML elements
 * updated at application startup.
 */
void ApplicationController::setDarkMode(bool enabled)
{
    mSettings->setValue("darkMode", enabled);
    qInfo() << "Setting theme to" << (enabled ? "dark" : "light") << "mode";
    emit darkModeChanged(enabled);
}

/**
 * @brief Load the window width from settings
 */
int ApplicationController::loadWindowWidth()
{
    return mSettings->value("windowWidth", 580).toInt();
}

/**
 * @brief Persist the window width to settings
 */
void ApplicationController::saveWindowWidth(int width)
{
    mSettings->setValue("windowWidth", width);
}

/**
 * @brief Load the window height from settings
 */
int ApplicationController::loadWindowHeight()
{
    return mSettings->value("windowHeight", 850).toInt();
}

/**
 * @brief Persist the window height to settings
 */
void ApplicationController::saveWindowHeight(int height)
{
    mSettings->setValue("windowHeight", height);
}

/**
 * @brief Load the last location from which a routine file was opened
 *
 * This is the path that the dialog box for loading routines will first display
 */
QUrl ApplicationController::loadRoutineFolder()
{
    return mSettings->value("routineFolder", "").toUrl();
}

/**
 * @brief Save the location from which routine files are loaded
 *
 * This is the path that the dialog box for loading routines will first display
 */
void ApplicationController::saveRoutineFolder(QUrl folder)
{
    mSettings->setValue("routineFolder", folder);
}


/**
 * @brief Load the label associated with a given valve
 * @param valveNumber The _valveNumber_ attribute of the QML GraphicalLabeledValveSwitch
 * @return The user-defined label, or "Unlabeled input" if none is set
 */
QString ApplicationController::loadValveLabel(int valveNumber)
{
    QString key = QString("valveLabels/") + QString::number(valveNumber);
    return mSettings->value(key, "Unlabeled input").toString();
}

/**
 * @brief Persist a user-defined label for a given valve
 * @param valveNumber The _valveNumber_ attribute of the QML GraphicalLabeledValveSwitch
 * @param label The label to be saved
 */
void ApplicationController::saveValveLabel(int valveNumber, QString label)
{
    QString key = QString("valveLabels/") + QString::number(valveNumber);
    mSettings->setValue(key, label);
}

/**
 * @brief Get setting for showing or hiding the graphical control view
 * @return True if the view should be shown
 */
bool ApplicationController::loadShowGraphicalControl()
{
    return mSettings->value("showGraphicalControl", false).toBool();
}

/**
 * @brief Show or hide the graphical control view
 */
void ApplicationController::saveShowGraphicalControl(bool show)
{
    mSettings->setValue("showGraphicalControl", show);
}

/**
 * @brief Load the baud rate for USB communication from settings
 * @return The baud rate; default value is 115200
 */
int ApplicationController::loadBaudRate()
{
    return mSettings->value("baudRate", 115200).toInt();
}

/**
 * @brief Save the baud rate to be used for USB communication
 * @param rate The rate
 */
void ApplicationController::saveBaudRate(int rate)
{
    mSettings->setValue("baudRate", rate);
}

void ApplicationController::onValveStateChanged(int valveNumber, bool open)
{
    qInfo() << "Valve" << valveNumber << (open ? "opened" : "closed");

    if (mQmlValveSwitches.contains(valveNumber)) {
        for (auto v : mQmlValveSwitches[valveNumber])
            v->setState(open);
    }
}

void ApplicationController::onPumpStateChanged(int pumpNumber, bool on)
{
    qInfo() << "Pump" << pumpNumber << "switched" << (on ? "on" : "off");

    if (mQmlPumpSwitches.contains(pumpNumber))
        mQmlPumpSwitches[pumpNumber]->setState(on);
}

void ApplicationController::onPressureChanged(int controllerNumber, double pressure)
{
    //qInfo() << "Measured pressure (normalized) on controller" << controllerNumber << ":" << pressure;

    if (mQmlPressureControllers.contains(controllerNumber)) {
        for (auto p : mQmlPressureControllers[controllerNumber])
            p->setMeasuredValue(pressure);
    }
}

void ApplicationController::onPressureSetpointChanged(int controllerNumber, double pressure)
{
    if (mQmlPressureControllers.contains(controllerNumber)) {
        for (auto p : mQmlPressureControllers[controllerNumber])
            p->setSetPoint(pressure);
    }
}

void ApplicationController::onCommunicatorStatusChanged(Communicator::ConnectionStatus newStatus)
{
    qDebug() << "App controller: communicator status changed to" << mCommunicator->getConnectionStatusString();

    if (newStatus == Communicator::Connected)
        mCommunicator->requestStatus();

    emit connectionStatusChanged(mCommunicator->getConnectionStatusString());
}
