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
 * Connection status can be checked using the getStatus() and getStatusString() functions, or better, by connecting to the statusChanged slot.
 *
 * The interface to the actual functionality of the microcontroller is provided by the setValve, setPump, setPressure, and refreshAll functions.
 * The first three tell the microcontroller to do something, e.g toggle a valve, while the refreshAll function requests an update of all components' statuses.
 *
 * The signals valveStateChanged, pumpStateChanged, and pressureChanged are emitted whenever the microcontroller communicates the current status of a component (valve, pump or pressure controller)
 *. Connect to these to know the current status of the hardware.
 *
 * Valves, pumps and pressure controllers are 1-indexed. I.e valveNumber will be between 1 and 32; pumpNumber between 1 and 2; controllerNumber between 1 and 3.
 *
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

    ConnectionStatus getStatus();
    QString getStatusString();

    QString devicePort();

    void setValve(int valveNumber, bool open);
    void setPump(int pumpNumber, bool on);
    void setPressure(int controllerNumber, double pressure);
    void refreshAll(); // request status of all components

signals:
    void valveStateChanged(int valveNumber, bool open);
    void pumpStateChanged(int pumpNumber, bool on);
    void pressureChanged(int controllerNumber, double pressure);

    void statusChanged(ConnectionStatus newStatus);

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
