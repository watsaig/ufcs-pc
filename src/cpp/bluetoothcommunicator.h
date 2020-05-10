#ifndef BLUETOOTHCOMMUNICATOR_H
#define BLUETOOTHCOMMUNICATOR_H

#include <QtCore>
#include <QBluetoothSocket>
#include <QBluetoothServiceDiscoveryAgent>

#include "constants.h"
#include "communicator.h"

/**
 * @brief Communication interface between the GUI and microcontroller, using Bluetooth Classic (serial port profile).
 *
 * The communication is virtually identical to that of the SerialCommunicator class. The main differences are due to the more complex connection process for bluetooth.
 * This class implements 3 ways of doing so (for now).
 * The most straightforward is connect(QBluetoothAddress, quint16). This connects to the profile at the given address and port.
 *
 * connect(QBluetoothAddress, QBluetoothUuid) is similar but requires the specific UUID of the SPP service
 *
 * connect(QBluetoothServiceInfo) connects to the provided ServiceInfo object. This is likely what
 * should be used if e.g. a bluetooth scanner is implemented in QML, where the user would select the device to connect to.
 *
 * Finally, connect() mimicks Communicator::connect: it searches for available devices, and guesses at which one to connect to based on the device name.
 */
class BluetoothCommunicator : public Communicator
{
    Q_OBJECT

public:
    BluetoothCommunicator();
    ~BluetoothCommunicator();

public slots:
    void connect();
    void connect(const QBluetoothServiceInfo &serviceInfo);
    void connect(const QBluetoothAddress &address, quint16 port);
    void connect(const QBluetoothAddress &address, const QBluetoothUuid& uuid);

private slots:
    void onSocketReady();
    void onSocketError(QBluetoothSocket::SocketError error);
    void onSocketConnected();
    void onSocketDisconnected();

    void onServiceDiscovered(QBluetoothServiceInfo serviceInfo);
    void onServiceDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error);
    void onServiceDiscoveryFinished();

    void onDeviceDiscovered(QBluetoothDeviceInfo deviceInfo);
    void onDeviceDiscoveryError(QBluetoothDeviceDiscoveryAgent::Error error);
    void onDeviceDiscoveryFinished();

protected:
    //void setComponentState(Component c, int val);
    void sendMessage(QByteArray message);

private:
    void initSocket();
    void initServiceDiscoveryAgent();
    void initDeviceDiscoveryAgent();

    QBluetoothSocket * mSocket;

    QBluetoothServiceInfo mService;
    QBluetoothServiceDiscoveryAgent * mServiceDiscoveryAgent;

    QBluetoothDeviceInfo mDeviceInfo;
    QBluetoothDeviceDiscoveryAgent * mDeviceDiscoveryAgent;

    bool mConnectingToSavedDevice;
    bool mFailedToConnectToSavedDevice;

};

#endif // BLUETOOTHCOMMUNICATOR_H
