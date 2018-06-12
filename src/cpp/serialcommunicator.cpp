#include "serialcommunicator.h"

SerialCommunicator::SerialCommunicator()
    : Communicator()
    , mSerialPort(NULL)
{

}

SerialCommunicator::~SerialCommunicator()
{
    if (mSerialPort && mSerialPort->isOpen())
        mSerialPort->close();
}
/**
 * @brief Connect to the microcontroller.
 *
 * The appropriate serial port is automatically selected, based on the device description.
 */
void SerialCommunicator::connect()
{
    // The following code is based on https://github.com/peteristhegreat/qt-serialport-arduino

    initSerialPort();

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
                    // The call to isBusy() takes a second or two; so only uncomment the following line for debug purposes
                    //"Busy:" + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
                    ;

        qDebug().noquote() << s;

        // The following line may need to be customized depending on your specific ESP32 board.
        if((info.description().contains("UART Bridge") || info.description().contains("USB Serial Port") || info.manufacturer().contains("Silicon Labs")) && !info.isBusy()) {
            portToUse = info;
            break;
        }
    }

    if(portToUse.isNull()) {
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

        // The following two lines are necessary with Sparkfun's ESP32 thing (which uses an FTDI chip);
        // not necessary (and maybe won't work -- to be tested) with the Espressif ESP32 DevKitC
        mSerialPort->setDataTerminalReady(false);
        mSerialPort->setRequestToSend(false);

        setConnectionStatus(Connected);
    }
    else {
        qWarning() << "Could not open serial port: " << mSerialPort->errorString();
        setConnectionStatus(Disconnected);
    }
}

/**
 * @brief Return the name of the port to which the microcontroller is connected
 */
QString SerialCommunicator::devicePort() const
{
    return mSerialPort->portName();
}

void SerialCommunicator::refreshAll()
{
    char toSend[2] = {(uint8_t)STATUS, (uint8_t)ALL_COMPONENTS};
    mSerialPort->write(toSend, 2);
}

void SerialCommunicator::handleSerialError(QSerialPort::SerialPortError error)
{
    if (!mSerialPort)
        return;

    if (error != QSerialPort::NoError) {
        qWarning() << "Serial port error: " << mSerialPort->errorString();
        mSerialPort->clearError();

        // Currently assuming that any error means that the device disconnected -- there may be a better way of doing this
        setConnectionStatus(Disconnected);

        if (mSerialPort->isOpen())
            mSerialPort->close();

        //TODO: set event listener for USB connections, to detect if & when USB is reconnected (if this is even possible)
        // see https://github.com/wang-bin/qdevicewatcher
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
void SerialCommunicator::onSerialReady()
{
    mBuffer.append(mSerialPort->readAll());

    if (mBuffer.size() >= 4)
        parseBuffer();
}

/**
 * @brief SerialCommunicator::parseBuffer
 *
 * Parse the buffer of received serial messages; execute any valid commands
 * and print any other data to the console.
 */
void SerialCommunicator::parseBuffer()
{
    // How it works:
    // - Scan through buffer until we find a start byte
    // - If there is a valid command following it, execute the command and delete that data
    // - If there was any data at the beginning of the buffer, before the start byte, print it
    //   then delete it

    // Valid commands are of the form: START_BYTE, ITEM, VALUE, END_BYTE

    uint8_t START_BYTE = 249;
    uint8_t END_BYTE = 250;

    for (int i(0); i < mBuffer.size(); ++i) {
        if ((uint8_t)(mBuffer[i]) == START_BYTE) {
            bool decrementI = false;
            if (i < (mBuffer.size() - 3) && (uint8_t)(mBuffer[i+3]) == END_BYTE) {

                uint8_t item = mBuffer[i+1];
                uint8_t value = mBuffer[i+2];

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

                else
                    qWarning() << "Unknown command: " << item << " ; " << value;

                mBuffer.remove(i, 4);
                decrementI = true;
            }

            if (i > 0) {
                qDebug() << "Unknown message received: " << mBuffer.left(i);
                mBuffer.remove(0, i);
                i = -1; // return to beginning of array when i is incremented
                decrementI = false;
            }

            if (decrementI)
                i -= 1;
        }
    }
}

void SerialCommunicator::setComponentState(Component c, int val)
{
    if (mConnectionStatus == Disconnected) {
        qWarning() << "Can't set requested component state: device is disconnected";
        return; // TODO: (?) throw exception
    }
    //qDebug() << "setComponentState: setting component" << c << "to" << val;

    char toSend[2] = {(uint8_t)c, (uint8_t)val};
    mSerialPort->write(toSend, 2);
}


void SerialCommunicator::initSerialPort()
{
    if (!mSerialPort) {
        mSerialPort = new QSerialPort(this);

        QObject::connect(mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this,
                SLOT(handleSerialError(QSerialPort::SerialPortError)));
        QObject::connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(onSerialReady()));
    }
}


