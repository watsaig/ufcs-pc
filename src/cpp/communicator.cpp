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
{}

/**
 * @brief Connect to the microcontroller.
 *
 * The appropriate serial port is automatically selected, based on the device description.
 */
void Communicator::connect()
{
    // The following code is based on https://github.com/peteristhegreat/qt-serialport-arduino

    setConnectionStatus(Connecting);

    qDebug() << "Connecting to ESP32... ";
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
        qDebug() << "port is not valid:" << portToUse.portName();
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
        qDebug() << "Connected to" << portToUse.description() << "on" << portToUse.portName();
        setConnectionStatus(Connected);
    }
    else {
        qCritical() << "Could not open serial port: " << mSerialPort->errorString();
        setConnectionStatus(Disconnected);
    }
}


Communicator::ConnectionStatus Communicator::getStatus()
{
    return mConnectionStatus;
}

/**
 * @brief Return the connection status in human-readable form
 */
QString Communicator::getStatusString()
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

void Communicator::setValve(int valveNumber, bool open)
{
    qDebug() << "Communicator: setting valve " << valveNumber << " " << (open ? "open" : "closed");

    Component c;

    if (valveNumber >= 1 && valveNumber <=32)
        c = (Component)(VALVE1 + valveNumber - 1);

    else {
        qDebug() << "Unknown valve ID";
        return;
    }

    setComponentState(c, open);
}

void Communicator::setPump(int pumpNumber, bool on)
{
    qDebug() << "Communicator: setting pump" << pumpNumber << " " << (on ? "on" : "off");

    Component c;
    PumpStates state;

    if (pumpNumber == 1)
        c = PUMP1;
    else if (pumpNumber == 2)
        c = PUMP2;
    else {
        qDebug() << "Unknown pump ID";
        return;
    }

    state = (on ? ON : OFF);

    setComponentState(c, state);
}

/* pressure : double between 0 and 1 */
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
            qDebug() << "Unknown pressure controller ID";
            return;
    }

    if (pressure < 0 || pressure > 1) {
        qDebug() << "Invalid pressure. Must be between 0 and 1.";
        return;
    }

    setComponentState(c, pressure*PR_MAX_VALUE);
}

void Communicator::refreshAll()
{
    char toSend[2] = {(uint8_t)STATUS, (uint8_t)ALL_COMPONENTS};
    mSerialPort->write(toSend, 2);
}

void Communicator::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError)
        qDebug() << "Serial port error: " << mSerialPort->errorString();
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

    qDebug() << "Received " << buffer.size() << " bytes on serial port";


    for (int i(0); i < buffer.size() - 1; ++i) {
        int item = buffer[i];
        int value = buffer[i+1];

        if ((item >= VALVE1 && item <= VALVE32) && (value == OPEN || value == CLOSED))
            emit valveStateChanged((item - VALVE1 + 1), value == OPEN);

        else if ((item == PUMP1 || item == PUMP1) && (value == ON || value == OFF))
            emit pumpStateChanged(item - PUMP1 + 1, value == ON);

        else if (item == PR1 || item == PR2 || item == PR3) {
            double pressure = (double)value/(double)PR_MAX_VALUE;
            int index = 1;
            if (item == PR2)
                index = 2;
            else if (item == PR3)
                index = 3;

            emit pressureChanged(index, pressure);
        }

        else
            qDebug() << "Unknown data received: " << item << " ; " << value;
    }
}


void Communicator::setConnectionStatus(ConnectionStatus status)
{
    mConnectionStatus = status;
    emit statusChanged(status);
}

void Communicator::setComponentState(Component c, int val)
{
    qDebug() << "setState: setting component " << c << " to " << val;

    char toSend[2] = {(uint8_t)c, (uint8_t)val};
    mSerialPort->write(toSend, 2);
}
