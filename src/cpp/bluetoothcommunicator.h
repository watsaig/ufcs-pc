#ifndef BLUETOOTHCOMMUNICATOR_H
#define BLUETOOTHCOMMUNICATOR_H

#include <QtCore>
#include <QBluetoothSocket>
#include <QBluetoothServiceDiscoveryAgent>

#include "constants.h"

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

    void connect(const QLatin1String &uuid);
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
    QBluetoothSocket * mSocket;
    QBluetoothServiceInfo mService;
    QBluetoothServiceDiscoveryAgent * mDiscoveryAgent;

};

#endif // BLUETOOTHCOMMUNICATOR_H
