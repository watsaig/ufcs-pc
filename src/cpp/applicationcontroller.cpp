#include "applicationcontroller.h"
#include "guihelper.h"

ApplicationController::ApplicationController(QObject *parent) : QObject(parent)
{
    // Initialize mCommunicator. Can be either USB ("Serial") or Bluetooth. Windows
    // doesn't support Bluetooth, and Android doesn't support serial over USB (at least,
    // not without rooting your phone), so we can set defaults for those platforms.
    // For other platforms, use whichever one you prefer.
    // This has to be specified at compile time for now; run-time switching may be supported later.

#if defined(Q_OS_ANDROID)
    mBluetoothEnabled = true;
#else
    mBluetoothEnabled = false;
#endif

    if (mBluetoothEnabled)
        mCommunicator = new BluetoothCommunicator(this);
    else
        mCommunicator = new SerialCommunicator(this);

    QObject::connect(mCommunicator, &Communicator::valveStateChanged, this, &ApplicationController::onValveStateChanged);
    QObject::connect(mCommunicator, &Communicator::pressureChanged, this, &ApplicationController::onPressureChanged);
    QObject::connect(mCommunicator, &Communicator::pressureSetpointChanged, this, &ApplicationController::onPressureSetpointChanged);
    QObject::connect(mCommunicator, &Communicator::pumpStateChanged, this, &ApplicationController::onPumpStateChanged);
    QObject::connect(mCommunicator, &Communicator::connectionStatusChanged, this, &ApplicationController::onCommunicatorStatusChanged);
    QObject::connect(mCommunicator, &Communicator::uptimeChanged, this, &ApplicationController::onUptimeChanged);

    mRoutineController = new RoutineController(this);

    QObject::connect(mRoutineController, &RoutineController::setValve,
                     this, &ApplicationController::setValve);
    QObject::connect(mRoutineController, &RoutineController::setPressure,
                     this, &ApplicationController::setPressure);

    mSettings = new QSettings();

    if (isDenseThemeEnabled())
        qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
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

/**
 * @brief Return the number of valves defined in the GUI
 *
 * This reflects the ValveSwitches defined on the QML side
 */
int ApplicationController::nValves()
{
    return mQmlValveSwitches.size();
}

/**
 * @brief Return the number of pumps defined in the GUI
 *
 * This reflects the PumpSwitches defined on the QML side
 */
int ApplicationController::nPumps()
{
    return mQmlPumpSwitches.size();
}

/**
 * @brief Return the number of pressure controllers defined in the GUI
 *
 * This reflects the PressureControllers defined on the QML side
 */
int ApplicationController::nPressureControllers()
{
    return mQmlPressureControllers.size();
}

/**
 * @brief Return the minimum pressure supported by the given pressure controller.
 */
double ApplicationController::minPressure(int controllerNumber)
{
    QList<PCHelper*> pcs = mQmlPressureControllers[controllerNumber];

    if (pcs.isEmpty()) {
        qCritical() << "Tried to access undefined pressure controller";
        return 0;
    }

    return pcs.first()->minPressure();
}

/**
 * @brief Return the maximum pressure supported by the given pressure controller.
 */
double ApplicationController::maxPressure(int controllerNumber)
{
    QList<PCHelper*> pcs = mQmlPressureControllers[controllerNumber];

    if (pcs.isEmpty()) {
        qCritical() << "Tried to access undefined pressure controller";
        return 0;
    }

    return pcs.first()->maxPressure();
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

bool ApplicationController::isDarkModeEnabled()
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
void ApplicationController::setDarkModeEnabled(bool enabled)
{
    mSettings->setValue("darkMode", enabled);
    qInfo() << "Setting theme to" << (enabled ? "dark" : "light") << "mode";
    emit darkModeChanged(enabled);
}

bool ApplicationController::isDenseThemeEnabled()
{
    return mSettings->value("denseThemeEnabled", false).toBool();
}

void ApplicationController::setDenseThemeEnabled(bool enabled)
{
    mSettings->setValue("denseThemeEnabled", enabled);

    // The only way to set the "Dense" variant of the Qt Quick controls material theme
    // is on application startup. So this signal is not emitted since it would cause
    // only some UI elements to update to the dense theme. But if it is one day possible
    // to set the variant at run time, this line should be uncommented.
    // emit denseThemeChanged(enabled);
}

/**
 * @brief Load the window width from settings
 */
int ApplicationController::windowWidth()
{
    return mSettings->value("windowWidth", 580).toInt();
}

/**
 * @brief Persist the window width to settings
 */
void ApplicationController::setWindowWidth(int width)
{
    mSettings->setValue("windowWidth", width);
}

/**
 * @brief Load the window height from settings
 */
int ApplicationController::windowHeight()
{
    return mSettings->value("windowHeight", 850).toInt();
}

/**
 * @brief Persist the window height to settings
 */
void ApplicationController::setWindowHeight(int height)
{
    mSettings->setValue("windowHeight", height);
}

/**
 * @brief Load the last location from which a routine file was opened
 *
 * This is the path that the dialog box for loading routines will first display
 */
QUrl ApplicationController::routineFolder()
{
    return mSettings->value("routineFolder", "").toUrl();
}

/**
 * @brief Save the location from which routine files are loaded
 *
 * This is the path that the dialog box for loading routines will first display
 */
void ApplicationController::setRoutineFolder(QUrl folder)
{
    mSettings->setValue("routineFolder", folder);
}


/**
 * @brief Load the label associated with a given valve
 * @param valveNumber The _valveNumber_ attribute of the QML GraphicalLabeledValveSwitch
 * @return The user-defined label, or "Unlabeled input" if none is set
 */
QString ApplicationController::valveLabel(int valveNumber)
{
    QString key = QString("valveLabels/") + QString::number(valveNumber);
    return mSettings->value(key, "Unlabeled input").toString();
}

/**
 * @brief Persist a user-defined label for a given valve
 * @param valveNumber The _valveNumber_ attribute of the QML GraphicalLabeledValveSwitch
 * @param label The label to be saved
 */
void ApplicationController::setValveLabel(int valveNumber, QString label)
{
    QString key = QString("valveLabels/") + QString::number(valveNumber);
    mSettings->setValue(key, label);
}

/**
 * @brief Get setting for showing or hiding the graphical control view
 * @return True if the view should be shown
 */
bool ApplicationController::isGraphicalControlEnabled()
{
    return mSettings->value("graphicalControl/enabled", false).toBool();
}

/**
 * @brief Show or hide the graphical control view
 */
void ApplicationController::setGraphicalControlEnabled(bool show)
{
    mSettings->setValue("graphicalControl/enabled", show);
}

QVariantList ApplicationController::graphicalControlScreenLabels()
{
    QVariantList labels;
    for (auto key : graphicalControlScreenSources().keys())
        labels.push_back(key);

    return labels;
}

QMap<QString, QUrl> ApplicationController::graphicalControlScreenSources()
{
    QMap<QString, QUrl> sources;

    mSettings->beginGroup("graphicalControl");
    mSettings->beginGroup("sources");

    for (QString const& k : mSettings->childKeys())
        sources[k] = mSettings->value(k).toString();

    mSettings->endGroup();
    mSettings->endGroup();

    // Default values, in case no sources are specified
    if (sources.isEmpty()) {
        sources["Co-culture chip v4"] = "qrc:/src/qml/GraphicalControl.qml";
        setGraphicalControlScreenSources(sources);
    }

    return sources;
}

void ApplicationController::setGraphicalControlScreenSources(QMap<QString, QUrl> sources)
{
    mSettings->beginGroup("graphicalControl");
    mSettings->beginGroup("sources");

    for (QString const& key : sources.keys())
        // Saved as string rather than QUrl to make it more human-friendly
        mSettings->setValue(key, sources[key].toString());

    mSettings->endGroup();
    mSettings->endGroup();
}

QString ApplicationController::currentGraphicalControlScreenLabel()
{
    QMap<QString, QUrl> allSources = graphicalControlScreenSources();

    return mSettings->value("graphicalControl/currentLabel", allSources.keys().first()).toString();

}

QUrl ApplicationController::currentGraphicalControlScreenURL()
{
    return graphicalControlScreenSources()[currentGraphicalControlScreenLabel()];
}

void ApplicationController::setCurrentGraphicalControlScreen(QString label)
{
    mSettings->setValue("graphicalControl/currentLabel", label);
}


/**
 * @brief Load the baud rate for USB communication from settings
 * @return The baud rate; default value is 115200
 */
uint ApplicationController::serialBaudRate()
{
    return mSettings->value("baudRate", 115200).toUInt();
}

/**
 * @brief Save the baud rate to be used for USB communication
 * @param rate The rate
 */
void ApplicationController::setSerialBaudRate(int rate)
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

void ApplicationController::onUptimeChanged(ulong seconds)
{
    int h = seconds/3600;
    int m = (seconds % 3600)/60;
    int s = seconds % 60;
    qInfo() << "Current uptime:" << h << "h" << m << "min" << s << "s";
}

void ApplicationController::onCommunicatorStatusChanged(Communicator::ConnectionStatus newStatus)
{
    qDebug() << "App controller: communicator status changed to" << mCommunicator->getConnectionStatusString();

    if (newStatus == Communicator::Connected)
        mCommunicator->requestStatus();

    emit connectionStatusChanged(mCommunicator->getConnectionStatusString());
}
