#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include "communicator.h"
#include <QQmlEngine>

class ApplicationController : public QObject
{
    Q_OBJECT

public:
    ApplicationController(QObject *parent = nullptr);

    Q_INVOKABLE void setPump(int pumpNumber, bool on) { mCommunicator.setPump(pumpNumber, on); }

private:

    Communicator mCommunicator;
};

#endif // APPLICATIONCONTROLLER_H
