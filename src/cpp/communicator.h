#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "constants.h"



/**
 * @brief The Communicator class provides an interface to the microcontroller
 *
 * Usage: simply construct a Communicator instance and call `connect()`. The appropriate serial port is automatically selected.
 * Connection status can be checked using the getConnectionStatus() and getConnectionStatusString() functions,
 * or better, by connecting to the connectionStatusChanged signal.
 *
 * The interface to the actual functionality of the microcontroller is provided by the setValve, setPump, setPressure, and refreshAll functions.
 * The first three tell the microcontroller to do something, e.g toggle a valve, while the refreshAll function requests an update of all components' statuses.
 *
 * The signals valveStateChanged, pumpStateChanged, and pressureChanged are emitted whenever the microcontroller communicates the current status of a component (valve, pump or pressure controller)
 *. Connect to these to know the current status of the hardware.
 *
 * In order to know how many components are available, and what pressures are supported by the pressure controllers,
 * use the nValves, nPumps, nPressureControllers, minPressure and maxPressure functions.
 *
 * Valves, pumps and pressure controllers are 1-indexed. I.e valveNumber will be between 1 and 32; pumpNumber between 1 and 2; controllerNumber between 1 and 3.
 *
 */
class Communicator : public QObject {
    Q_OBJECT

public:
    enum ConnectionStatus {
        Disconnected,
        Connecting,
        Connected
    };

    Communicator ();
    virtual ~Communicator ();

    void connect(); // No parameters yet; may change this to allow selecting what device we connect to

    ConnectionStatus getConnectionStatus() const;

    QString getConnectionStatusString() const;

    QString devicePort() const;

    void setValve(int valveNumber, bool open);
    void setPump(int pumpNumber, bool on);
    void setPressure(int controllerNumber, double pressure);
    void refreshAll(); // request status of all components

    int nValves() const { return N_VALVES; }
    int nPumps() const { return N_PUMPS; }
    int nPressureControllers() const { return N_PRS; }
    double minPressure(int controllerNumber) const;
    double maxPressure(int controllerNumber) const;


signals:
    void valveStateChanged(int valveNumber, bool open);
    void pumpStateChanged(int pumpNumber, bool on);
    void pressureChanged(int controllerNumber, double pressure);

    void connectionStatusChanged(ConnectionStatus newStatus);

protected slots:
    void handleSerialError(QSerialPort::SerialPortError error);
    void onSerialReady();

protected:
    void setComponentState(Component c, int val);

    void setConnectionStatus(ConnectionStatus status);

    ConnectionStatus mConnectionStatus;
    QSerialPort * mSerialPort;
};

#endif
