#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "communicator.h"

class SerialCommunicator : public Communicator
{
    Q_OBJECT

public:
    SerialCommunicator();
    virtual ~SerialCommunicator();

    void connect();

    QString devicePort() const;

private slots:
    void handleSerialError(QSerialPort::SerialPortError error);
    void onSerialReady();

protected:
    void sendMessage(QByteArray message);

private:
    void initSerialPort();

    QSerialPort * mSerialPort;
};

#endif // SERIALCOMMUNICATOR_H
