#include "bluetoothcommunicator.h"


BluetoothCommunicator::BluetoothCommunicator()
    : mConnectionStatus(Disconnected)
    , mServiceDiscoveryAgent(NULL)
    , mDeviceDiscoveryAgent(NULL)
    , mSocket(NULL)
{
}

BluetoothCommunicator::~BluetoothCommunicator()
{
    qDebug() << "deleting bluetoothCommunicator";

    // TODO: close and kill socket, if necessary
}

BluetoothCommunicator::ConnectionStatus BluetoothCommunicator::getConnectionStatus() const
{
    return mConnectionStatus;
}

/**
 * @brief Return the connection status in human-readable form
 */
QString BluetoothCommunicator::getConnectionStatusString() const
{
    switch (mConnectionStatus) {
        case Disconnected:
            return "Disconnected";
        case Connecting:
            return "Connecting";
        case Connected:
            return "Connected";
        default:
            return QString();
    }
}

/**
 * @brief Return the minimum pressure supported by the given pressure controller.
 */
double BluetoothCommunicator::minPressure(int controllerNumber) const
{
    switch(controllerNumber) {
        case 1: return PR1_MIN_PRESSURE;
        case 2: return PR2_MIN_PRESSURE;
        case 3: return PR3_MIN_PRESSURE;
    }

    return 0; // TODO: throw exception
}

/**
 * @brief Return the maximum pressure supported by the given pressure controller.
 */
double BluetoothCommunicator::maxPressure(int controllerNumber) const
{
    switch(controllerNumber) {
        case 1: return PR1_MAX_PRESSURE;
        case 2: return PR2_MAX_PRESSURE;
        case 3: return PR3_MAX_PRESSURE;
    }
    return 0; // TODO: throw exception
}

/**
 * @brief Open or close a specific valve
 * @param valveNumber The valve number, between 1 and 32
 * @param open If true, valve will be opened; otherwise, valve will be closed
 */
void BluetoothCommunicator::setValve(int valveNumber, bool open)
{
    //qDebug() << "BluetoothCommunicator: setting valve " << valveNumber << " " << (open ? "open" : "closed");

    Component c;
    ValveStates state;

    if (valveNumber >= 1 && valveNumber <= 32)
        c = (Component)(VALVE1 + valveNumber - 1);

    else {
        qWarning() << "Unknown valve ID: " << valveNumber;
        return;
    }

    state = (open ? OPEN : CLOSED);

    setComponentState(c, state);
}
/**
 * @brief Set the pressure setpoint of a given controller
 * @param controllerNumber The controller number, between 1 and 3
 * @param pressure A double between 0 and 1.0, with 0 being the minimum and 1 being the maximum pressures allowed by the controller
 */
void BluetoothCommunicator::setPressure(int controllerNumber, double pressure)
{
    //qDebug() << "BluetoothCommunicator: setting pressure controller" << controllerNumber << " to " << pressure;

    Component c;

    switch (controllerNumber) {
        case 1:
            c = PR1;
            break;
        case 2:
            c = PR2;
            break;
        case 3:
            c = PR3;
            break;
        default:
            qWarning() << "Unknown pressure controller ID: " << controllerNumber;
            return;
    }

    if (pressure < 0 || pressure > 1) {
        qWarning() << "Invalid pressure. Must be between 0 and 1.";
        return;
    }

    setComponentState(c, pressure*PR_MAX_VALUE);
}

/**
 * @brief Switch a given pump on or off.
 * @param pumpNumber Either 1 or 2
 * @param on If true, the pump will be turned on; otherwise, the pump will be turned off.
 *
 *
 * Note: this functionality may be abandoned soon, as the pumps will be switched on and off automatically based on pressure requirements.
 */
void BluetoothCommunicator::setPump(int pumpNumber, bool on)
{
    //qDebug() << "BluetoothCommunicator: setting pump" << pumpNumber << (on ? "on" : "off");

    Component c;
    PumpStates state;

    if (pumpNumber == 1)
        c = PUMP1;
    else if (pumpNumber == 2)
        c = PUMP2;
    else {
        qWarning() << "Unknown pump ID: " << pumpNumber;
        return;
    }

    state = (on ? ON : OFF);

    setComponentState(c, state);
}

void BluetoothCommunicator::refreshAll()
{
    char toSend[2] = {(uint8_t)STATUS, (uint8_t)ALL_COMPONENTS};
    mSocket->write(toSend, 2);
}

/**
 * @brief Connect to the ESP32, if it's available
 *
 * This function searches for the ESP32 and connects to it, if possible.
 */
void BluetoothCommunicator::connect()
{
    setConnectionStatus(Connecting);
    qDebug() << "Searching for ESP32...";

    /*
    initDiscoveryAgent();

    qDebug() << "Starting service discovery...";
    //mServiceDiscoveryAgent->setUuidFilter(QBluetoothUuid(uuid));
    mServiceDiscoveryAgent->setUuidFilter(QBluetoothUuid::SerialPort); //TODO: more filters, to target ESP32 specifically (or at least Espressif)
    mServiceDiscoveryAgent->start();//QBluetoothServiceDiscoveryAgent::FullDiscovery);
    */
    initDeviceDiscoveryAgent();
    mDeviceDiscoveryAgent->start();
}

/**
 * @brief Connect to the microcontroller, given the service information
 * @param serviceInfo ServiceInfo describing the Serial Port Profile to connect to.
 */
void BluetoothCommunicator::connect(const QBluetoothServiceInfo &serviceInfo)
{
    setConnectionStatus(Connecting);
    qDebug() << "Connecting to service...";

    initSocket();

    mSocket->connectToService(serviceInfo);
    qDebug() << "Connect to service done";
}

void BluetoothCommunicator::connect(const QBluetoothAddress &address, quint16 port)
{
    setConnectionStatus(Connecting);
    qDebug() << "Connecting to address" << address << ", port" << port;
    initSocket();

    mSocket->connectToService(address, port);
}

/**
 * @brief This slot should be called when data is ready on the serial port
 *
 * Data is communicated between the microcontroller and the PC in two-byte packets;
 * this function reads the received bytes and emits signals depending on what was received.
 * Since the microcontroller only communicates the status of the various hardware components,
 * the signals emitted can be `valveStateChanged`, `pumpStateChanged`, or `pressureChanged`.
 */
void BluetoothCommunicator::onSocketReady()
{
    //qDebug() << "Received" << mSocket->bytesAvailable() << "bytes on serial port";

    QByteArray buffer = mSocket->readAll();

    for (int i(0); i < buffer.size() - 1; i+=2) {
        uint8_t item = buffer[i];
        uint8_t value = buffer[i+1];

        if ((item >= VALVE1 && item <= VALVE32) && (value == OPEN || value == CLOSED))
            emit valveStateChanged((item - VALVE1 + 1), value == OPEN);

        else if ((item == PUMP1 || item == PUMP2) && (value == ON || value == OFF))
            emit pumpStateChanged(item - PUMP1 + 1, value == ON);

        else if (item == PR1 || item == PR2 || item == PR3) {
            double pressure = double(value)/double(PR_MAX_VALUE);
            if (pressure < 0)
                qDebug() << "Pressure invalid:" << value;
            int index = 1;
            if (item == PR2)
                index = 2;
            else if (item == PR3)
                index = 3;

            emit pressureChanged(index, pressure);
        }

        else {
            qWarning() << "Unknown data received: " << item << " ; " << value;
        }
    }
}

void BluetoothCommunicator::onSocketError(QBluetoothSocket::SocketError error)
{
    if (!mSocket)
        return;

    if (error != QBluetoothSocket::NoSocketError) {
        qWarning() << "Bluetooth socket error: " << mSocket->errorString();
    }
}

void BluetoothCommunicator::onSocketConnected()
{
    setConnectionStatus(Connected);
    qDebug() << "Socket connected";
}

void BluetoothCommunicator::onSocketDisconnected()
{
    setConnectionStatus(Disconnected);
    qDebug() << "Socket disconnected";
}

void BluetoothCommunicator::onServiceDiscovered(QBluetoothServiceInfo serviceInfo)
{
    qDebug() << "==============================================";
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
    qDebug() << "==============================================";


    // TODO: if the various attributes above correspond to the ESP32 serial port profile, stop the scanning and connect to it.
    // or continue scanning but set mService = serviceInfo .
}

void BluetoothCommunicator::onServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error)
{
    qWarning() << "Bluetooth connection error:" << mServiceDiscoveryAgent->errorString();
}

void BluetoothCommunicator::onServiceDiscoveryFinished()
{
    qDebug() << "Bluetooth service discovery finished";
    //connect(mService);
}

void BluetoothCommunicator::onDeviceDiscovered(QBluetoothDeviceInfo deviceInfo)
{
    /*
    qDebug() << "-------------------------------------------------------------------------------";
    qDebug() << "Bluetooth device discovered:" << deviceInfo.name() << "(" << deviceInfo.address() << ")";
    qDebug() << "Device UUID:" << deviceInfo.deviceUuid();
    qDebug() << "Major device class:" << deviceInfo.majorDeviceClass();
    qDebug() << "Minor device class:" << deviceInfo.minorDeviceClass();
    qDebug() << "RSSI:" << deviceInfo.rssi();
    qDebug() << "Service classes:" << deviceInfo.serviceClasses();

    qDebug() << "Service UUIDs:";
    QList<QBluetoothUuid> uuids = deviceInfo.serviceUuids();
    foreach(QBluetoothUuid id, uuids) {
        qDebug() << id;
    }
    */

    if (deviceInfo.name() == "Microfluidics control system") {
        qDebug() << "Found control system; stopping discovery";
        mDeviceInfo = deviceInfo;
        mDeviceDiscoveryAgent->stop();
    }

}

void BluetoothCommunicator::onDeviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qDebug() << "Device discovery error:" << mDeviceDiscoveryAgent->errorString();
}

void BluetoothCommunicator::onDeviceDiscoveryFinished()
{
    qDebug() << "Device discovery finished";
    if (mDeviceInfo.isValid())
        connect(mDeviceInfo.address(), 2); // TODO: detect which channel the SPP is actually on (can't currently check for that)
    else
        setConnectionStatus(Disconnected);

}

void BluetoothCommunicator::setComponentState(Component c, int val)
{
    if (mConnectionStatus == Disconnected) {
        qWarning() << "Can't set requested component state: device is disconnected";
        return; // TODO: (?) throw exception
    }
    //qDebug() << "setComponentState: setting component" << c << "to" << val;

    char toSend[2] = {(uint8_t)c, (uint8_t)val};
    mSocket->write(toSend, 2);
}

void BluetoothCommunicator::setConnectionStatus(ConnectionStatus status)
{
    if (status != mConnectionStatus) {
        mConnectionStatus = status;
        emit connectionStatusChanged(status);
    }
}

void BluetoothCommunicator::initSocket()
{
    if (!mSocket) {
        mSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        qDebug() << "Socket created";

        QObject::connect(mSocket, SIGNAL(connected()),
                         this, SLOT(onSocketConnected()));
        QObject::connect(mSocket, SIGNAL(disconnected()),
                         this, SLOT(onSocketDisconnected()));
        QObject::connect(mSocket, SIGNAL(readyRead()),
                         this, SLOT(onSocketReady()));
        QObject::connect(mSocket, SIGNAL(error(QBluetoothSocket::SocketError)),
                         this, SLOT(onSocketError(QBluetoothSocket::SocketError)));

    }
}

void BluetoothCommunicator::initServiceDiscoveryAgent()
{
    if (!mServiceDiscoveryAgent) {
        mServiceDiscoveryAgent = new QBluetoothServiceDiscoveryAgent();
        qDebug() << "Discovery agent created";

        QObject::connect(mServiceDiscoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                         this, SLOT(onServiceDiscovered(QBluetoothServiceInfo)));
        QObject::connect(mServiceDiscoveryAgent, SIGNAL(finished()), this, SLOT(onServiceDiscoveryFinished()));
        QObject::connect(mServiceDiscoveryAgent, SIGNAL(canceled()), this, SLOT(onServiceDiscoveryFinished()));
        QObject::connect(mServiceDiscoveryAgent, SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)),
                         this, SLOT(onServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error)));

    }
}

void BluetoothCommunicator::initDeviceDiscoveryAgent()
{
    if (!mDeviceDiscoveryAgent) {
        mDeviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent();
        qDebug() << "Device discovery agent created";

        QObject::connect(mDeviceDiscoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                         this, SLOT(onDeviceDiscovered(QBluetoothDeviceInfo)));
        QObject::connect(mDeviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
                         this, SLOT(onDeviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error)));
        QObject::connect(mDeviceDiscoveryAgent, SIGNAL(finished()),
                         this, SLOT(onDeviceDiscoveryFinished()));
        QObject::connect(mDeviceDiscoveryAgent, SIGNAL(canceled()),
                         this, SLOT(onDeviceDiscoveryFinished()));

    }
}
