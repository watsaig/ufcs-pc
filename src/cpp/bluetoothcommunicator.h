#ifndef BLUETOOTHCOMMUNICATOR_H
#define BLUETOOTHCOMMUNICATOR_H

#include <QtCore>
#include <QBluetoothSocket>
#include <QBluetoothServiceDiscoveryAgent>

#include "constants.h"

/**
 * @brief Communication interface between the GUI and microcontroller, using Bluetooth Classic (serial port profile).
 *
 * The communication is virtually identical to that of the Communicator class. The main differences are due to the more complex connection process for bluetooth.
 * This class implements 3 ways of doing so (for now).
 * The most straightforward is connect(QBluetoothAddress, quint16). This connects to the profile at the given address and port.
 * While this is quick and easy for development purposes, it is not very useful on its own otherwise.
 *
 * connect(QBluetoothServiceInfo) connects to the provided ServiceInfo object. This is likely what
 * should be used if e.g. a bluetooth scanner is implemented in QML, where the user would select the device to connect to.
 *
 * Finally, connect() mimicks Communicator::connect: it searches for available devices, and guesses at which one to connect to based on vendor information.
 */
class BluetoothCommunicator : public QObject // : public Communicator // this will have to come later; maybe Communicator should become an abstract class, inherited by SerialCommunicator and BluetoothCommunicator...
{
    Q_OBJECT

public:
    enum ConnectionStatus {
        Disconnected,
        Connecting,
        Connected
    };

    BluetoothCommunicator();
    ~BluetoothCommunicator();


    ConnectionStatus getConnectionStatus() const;

    QString getConnectionStatusString() const;

    void setPump(int pumpNumber, bool on);
    void refreshAll(); // request status of all components

    int nValves() const { return N_VALVES; }
    int nPumps() const { return N_PUMPS; }
    int nPressureControllers() const { return N_PRS; }
    double minPressure(int controllerNumber) const;
    double maxPressure(int controllerNumber) const;

public slots:
    void setValve(int valveNumber, bool open);
    void setPressure(int controllerNumber, double pressure);

    void connect();
    void connect(const QBluetoothAddress &address, quint16 port);
    void connect(const QBluetoothServiceInfo &serviceInfo);


signals:
    void valveStateChanged(int valveNumber, bool open);
    void pumpStateChanged(int pumpNumber, bool on);
    void pressureChanged(int controllerNumber, double pressure);

    void connectionStatusChanged(ConnectionStatus newStatus);

protected slots:
    void onSocketError(QBluetoothSocket::SocketError error);
    void onSocketReady();

    void onServiceDiscovered(QBluetoothServiceInfo serviceInfo);
    void onDiscoveryFinished();
    void onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error);
    void onSocketConnected();
    void onSocketDisconnected();

protected:
    void setComponentState(Component c, int val);

    void setConnectionStatus(ConnectionStatus status);

    ConnectionStatus mConnectionStatus;

private:
    void initSocket();
    void initDiscoveryAgent();

    QBluetoothSocket * mSocket;
    QBluetoothServiceInfo mService;
    QBluetoothServiceDiscoveryAgent * mDiscoveryAgent;

};

#endif // BLUETOOTHCOMMUNICATOR_H
