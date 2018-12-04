#include "communicator.h"


Communicator::Communicator()
    : mConnectionStatus(Disconnected)
{
}

Communicator::~Communicator()
{
    qDebug() << "deleting communicator";
}

Communicator::ConnectionStatus Communicator::getConnectionStatus() const
{
    return mConnectionStatus;
}

/**
 * @brief Return the connection status in human-readable form
 */
QString Communicator::getConnectionStatusString() const
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
 * @param valveNumber The valve number, between 1 and N_VALVES (32)
 * @param open If true, valve will be opened; otherwise, valve will be closed
 */
void Communicator::setValve(int valveNumber, bool open)
{
    //qDebug() << "Communicator: setting valve " << valveNumber << " " << (open ? "open" : "closed");

    Component c;
    ValveStates state;

    if (valveNumber >= 1 && valveNumber <= N_VALVES)
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
    //qDebug() << "Communicator: setting pump" << pumpNumber << (on ? "on" : "off");

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
    //qDebug() << "Communicator: setting pressure controller" << controllerNumber << " to " << pressure;

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
 * @brief Communicator::parseBuffer
 *
 * Parse the buffer of received serial messages; execute any valid commands
 * and print any other data to the console.
 */
void Communicator::parseBuffer(QByteArray buffer)
{
    // How it works:
    // - Scan through buffer until we find a start byte
    // - If there is a valid command following it, execute the command and delete that data
    // - If there was any data at the beginning of the buffer, before the start byte, print it
    //   then delete it

    // Valid messages are of the form: START_BYTE, ITEM, VALUE, END_BYTE
    // An exception to this is the "uptime" message, which requires 4 bytes to represent the value
    // (time in seconds since boot). In that case, the message is of the form:
    // START_BYTE, ITEM, VALUE0, VALUE1, VALUE2, VALUE3, END_BYTE
    // where VALUE0 is the MSB.

    uint8_t START_BYTE = 249;
    uint8_t END_BYTE = 250;

    for (int i(0); i < buffer.size() - 3; ++i) {
        if ((uint8_t)(buffer[i]) == START_BYTE) {
            bool decrementI = false;

            // Check for uptime message
            if ((uint8_t)(buffer[i+1]) == UPTIME) {

                if ((i < buffer.size() - 6) && (uint8_t)(buffer[i+6]) == END_BYTE) {
                    uint8_t value0 = buffer[i+2];
                    uint8_t value1 = buffer[i+3];
                    uint8_t value2 = buffer[i+4];
                    uint8_t value3 = buffer[i+5];

                    long uptime = value0 << 24 | value1 << 16 | value2 << 8 | value3;

                    int hours = uptime/3600;
                    int minutes = (uptime % 3600)/60;
                    int seconds = uptime % 60;

                    qInfo() << "Current uptime:" << hours << "h" << minutes << "min" << seconds << "s";

                    buffer.remove(i, 4);
                    decrementI = true;
                }

            }

            else if ((uint8_t)(buffer[i+3]) == END_BYTE) {

                uint8_t item = buffer[i+1];
                uint8_t value = buffer[i+2];

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

                else if (item == ERROR) {
                    switch (value) {
                        case PRESSURE_REGULATOR_NOT_RESPONDING:
                            qWarning() << "Pressure regulator not responding";
                            break;
                        default:
                            qWarning() << "Unkown error signaled by microcontroller";
                            break;
                    }
                }

                else
                    qWarning() << "Unknown command: " << item << " ; " << value;

                buffer.remove(i, 4);
                decrementI = true;
            }

            if (i > 0) {
                // Extra characters before the actual message => print them and remove them.
                qInfo() << "Unknown message received: " << buffer.left(i);
                buffer.remove(0, i);
                decrementI = false;
                i = -1; // return to beginning of array when i is incremented
            }

            if (decrementI)
                i -= 1;
        }
    }
}

/**
 * @brief Return the minimum pressure supported by the given pressure controller.
 */
double Communicator::minPressure(int controllerNumber) const
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
double Communicator::maxPressure(int controllerNumber) const
{
    switch(controllerNumber) {
        case 1: return PR1_MAX_PRESSURE;
        case 2: return PR2_MAX_PRESSURE;
        case 3: return PR3_MAX_PRESSURE;
    }
    return 0; // TODO: throw exception
}



void Communicator::setConnectionStatus(ConnectionStatus status)
{
    if (status != mConnectionStatus) {
        mConnectionStatus = status;
        emit connectionStatusChanged(status);
    }
}
