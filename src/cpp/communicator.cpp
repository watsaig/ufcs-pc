#include "communicator.h"


Communicator::Communicator()
    : mConnectionStatus(Disconnected)
{
    mSerialPort = new QSerialPort(this);

    QObject::connect(mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleSerialError(QSerialPort::SerialPortError)));
    QObject::connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(onSerialReady()));
}

Communicator::~Communicator()
{
    if (mSerialPort->isOpen())
        mSerialPort->close();
}

/**
 * @brief Connect to the microcontroller.
 *
 * The appropriate serial port is automatically selected, based on the device description.
 */
void Communicator::connect()
{
    // The following code is based on https://github.com/peteristhegreat/qt-serialport-arduino

    setConnectionStatus(Connecting);

    qInfo() << "Connecting to ESP32... ";

    qDebug() << "List of all serial devices:";
    qDebug() << "---------------------------";

    QSerialPortInfo portToUse;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString s = "Port:" + info.portName() + "\n"
                    "Location:" + info.systemLocation() + "\n"
                    "Description:" + info.description() + "\n"
                    "Manufacturer:" + info.manufacturer() + "\n"
                    "Serial number:" + info.serialNumber() + "\n"
                    "Vendor Identifier:" + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    "Product Identifier:" + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    "Busy:" + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        qDebug().noquote() << s;

        // The following line may need to be customized depending on your specific ESP32 board.
        if(!info.isBusy() && (info.description().contains("UART Bridge") || info.manufacturer().contains("Silicon Labs"))) {
            portToUse = info;
            break;
        }
    }

    if(portToUse.isNull() || !portToUse.isValid()) {
        qWarning() << "Serial port unknown or not valid:" << portToUse.portName();
        setConnectionStatus(Disconnected);
        return;
    }

    mSerialPort->setPortName(portToUse.portName());
    mSerialPort->setBaudRate(QSerialPort::Baud115200);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (mSerialPort->open(QIODevice::ReadWrite)) {
        qInfo() << "Connected to" << portToUse.description() << "on" << portToUse.portName();
        setConnectionStatus(Connected);
    }
    else {
        qWarning() << "Could not open serial port: " << mSerialPort->errorString();
        setConnectionStatus(Disconnected);
    }
}


Communicator::ConnectionStatus Communicator::getConnectionStatus()
{
    return mConnectionStatus;
}

/**
 * @brief Return the connection status in human-readable form
 */
QString Communicator::getConnectionStatusString()
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


QString Communicator::devicePort()
{
    return mSerialPort->portName();
}

/**
 * @brief Open or close a specific valve
 * @param valveNumber The valve number, between 1 and 32
 * @param open If true, valve will be opened; otherwise, valve will be closed
 */
void Communicator::setValve(int valveNumber, bool open)
{
    qDebug() << "Communicator: setting valve " << valveNumber << " " << (open ? "open" : "closed");

    Component c;
    ValveStates state;

    if (valveNumber >= 1 && valveNumber <=32)
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
void Communicator::setPump(int pumpNumber, bool on)
{
    qDebug() << "Communicator: setting pump" << pumpNumber << (on ? "on" : "off");

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
void Communicator::setPressure(int controllerNumber, double pressure)
{
    qDebug() << "Communicator: setting pressure controller" << controllerNumber << " to " << pressure << " psi";

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

void Communicator::refreshAll()
{
    char toSend[2] = {(uint8_t)STATUS, (uint8_t)ALL_COMPONENTS};
    mSerialPort->write(toSend, 2);
}

/**
 * @brief Return the minimum pressure supported by the given pressure controller.
 */
double Communicator::minPressure(int controllerNumber)
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
double Communicator::maxPressure(int controllerNumber)
{
    switch(controllerNumber) {
        case 1: return PR1_MAX_PRESSURE;
        case 2: return PR2_MAX_PRESSURE;
        case 3: return PR3_MAX_PRESSURE;
    }
    return 0; // TODO: throw exception
}

void Communicator::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
        qWarning() << "Serial port error: " << mSerialPort->errorString();
}

/**
 * @brief This slot should be called when data is ready on the serial port
 *
 * Data is communicated between the microcontroller and the PC in two-byte packets;
 * this function reads the received bytes and emits signals depending on what was received.
 * Since the microcontroller only communicates the status of the various hardware components,
 * the signals emitted can be `valveStateChanged`, `pumpStateChanged`, or `pressureChanged`.
 */
void Communicator::onSerialReady()
{
    QByteArray buffer = mSerialPort->readAll();

    qDebug() << "Received" << buffer.size() << "bytes on serial port";


    for (int i(0); i < buffer.size() - 1; ++i) {
        int item = buffer[i];
        int value = buffer[i+1];

        if ((item >= VALVE1 && item <= VALVE32) && (value == OPEN || value == CLOSED))
            emit valveStateChanged((item - VALVE1 + 1), value == OPEN);

        else if ((item == PUMP1 || item == PUMP1) && (value == ON || value == OFF))
            emit pumpStateChanged(item - PUMP1 + 1, value == ON);

        else if (item == PR1 || item == PR2 || item == PR3) {
            double pressure = double(value)/double(PR_MAX_VALUE);
            int index = 1;
            if (item == PR2)
                index = 2;
            else if (item == PR3)
                index = 3;

            emit pressureChanged(index, pressure);
        }

        else
            qWarning() << "Unknown data received: " << item << " ; " << value;
    }
}


void Communicator::setConnectionStatus(ConnectionStatus status)
{
    mConnectionStatus = status;
    emit connectionStatusChanged(status);
}

void Communicator::setComponentState(Component c, int val)
{
    qDebug() << "setComponentState: setting component" << c << "to" << val;

    char toSend[2] = {(uint8_t)c, (uint8_t)val};
    mSerialPort->write(toSend, 2);
}
