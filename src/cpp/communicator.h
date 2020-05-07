#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QtCore>

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

    ConnectionStatus getConnectionStatus() const;
    QString getConnectionStatusString() const;

    int nValves() const { return N_VALVES; }
    int nPumps() const { return N_PUMPS; }
    int nPressureControllers() const { return N_PRS; }
    double minPressure(int controllerNumber) const;
    double maxPressure(int controllerNumber) const;

public slots:
    virtual void connect() = 0;
    void setValve(uint valveNumber, bool open);
    void setPressure(uint controllerNumber, double pressure);
    void setPump(uint pumpNumber, bool on);
    void requestStatus();

signals:
    void valveStateChanged(uint valveNumber, bool open);
    void pumpStateChanged(uint pumpNumber, bool on);
    void pressureChanged(uint controllerNumber, double pressure);
    void pressureSetpointChanged(uint controllerNumber, double pressure);

    void connectionStatusChanged(ConnectionStatus newStatus);

protected:
    //virtual void setComponentState(Component c, int val) = 0;
    void setConnectionStatus(ConnectionStatus status);
    //virtual void parseBuffer(QByteArray buffer);
    QByteArray frameMessage(QByteArray message);
    virtual void sendMessage(QByteArray message) = 0;

    ConnectionStatus mConnectionStatus;

    /// The buffer of incoming data, populated by the serial port backend
    QByteArray mBuffer;

    // Message parser-related members
    QByteArray decodeBuffer();
    void parseDecodedBuffer(QByteArray buffer);
    void handleCommand(uint8_t command, QList<QByteArray> parameters);

    QByteArray mDecodedBuffer;
    bool mDecoderRecording;
    bool mDecoderEscaped;
    int mDecoderIndex;

#ifdef TESTING
    friend class TestCommunicator;
#endif

};

#endif
