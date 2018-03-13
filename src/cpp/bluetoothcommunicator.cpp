#include "bluetoothcommunicator.h"


BluetoothCommunicator::BluetoothCommunicator()
    : mConnectionStatus(Disconnected)
{
}

BluetoothCommunicator::~BluetoothCommunicator()
{
    qDebug() << "deleting bluetoothCommunicator";

    // TODO: close and kill socket, if necessary
}

/**
 * @brief Connect to the microcontroller.
 *
 *
 */
void BluetoothCommunicator::connect(const QLatin1String &uuid)
{
    setConnectionStatus(Connecting);
    qDebug() << "Connecting to UUID...";

    if (!mDiscoveryAgent) {
        mDiscoveryAgent = new QBluetoothServiceDiscoveryAgent();
        qDebug() << "Discovery agent created";

        QObject::connect(mDiscoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
                         this, SLOT(onServiceDiscovered(QBluetoothServiceInfo)));
        QObject::connect(mDiscoveryAgent, SIGNAL(finished()), this, SLOT(onDiscoveryFinished()));
        QObject::connect(mDiscoveryAgent, SIGNAL(canceled()), this, SLOT(onDiscoveryFinished()));
        QObject::connect(mDiscoveryAgent, SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)),
                         this, SLOT(onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error)));

    }

    qDebug() << "Starting service discovery...";
    mDiscoveryAgent->setUuidFilter(QBluetoothUuid(uuid));
    mDiscoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void BluetoothCommunicator::connect(const QBluetoothServiceInfo &serviceInfo)
{
    setConnectionStatus(Connecting);
    qDebug() << "Connecting to service...";

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

    mSocket->connectToService(serviceInfo);
    qDebug() << "Connect to service done";
}

void BluetoothCommunicator::onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error)
{
    qWarning() << "Bluetooth connection error:" << mDiscoveryAgent->errorString();
}

void BluetoothCommunicator::onDiscoveryFinished()
{
    qDebug() << "Bluetooth discovery finished";
    connect(mService);
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


    mService = serviceInfo; // assuming I will only find one service.
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

void BluetoothCommunicator::refreshAll()
{
    char toSend[2] = {(uint8_t)STATUS, (uint8_t)ALL_COMPONENTS};
    mSocket->write(toSend, 2);
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

void BluetoothCommunicator::onSocketError(QBluetoothSocket::SocketError error)
{
    if (!mSocket)
        return;

    if (error != QBluetoothSocket::NoSocketError) {
        qWarning() << "Bluetooth socket error: " << mSocket->errorString();
    }
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
    qDebug() << "Received" << mSocket->bytesAvailable() << "bytes on serial port";

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


void BluetoothCommunicator::setConnectionStatus(ConnectionStatus status)
{
    if (status != mConnectionStatus) {
        mConnectionStatus = status;
        emit connectionStatusChanged(status);
    }
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
