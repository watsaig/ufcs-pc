#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QtCore>

#include "constants.h"

class Communicator {
    Q_OBJECT

public:
    Communicator ();
    virtual ~Communicator ();

    /*
    int getState(Component); // returns the cached state (from mCurrentStates). Maybe, also polls the microcontroller for that value
    void setState(Component c, int val); // this should throw an exception if the uC isn't connected, for example
    */
    
public slots:
    void setValve(int valveNumber, bool on);
    void setPump(int pumpNumber, bool on);
    void setPressure(int controllerNumber, double pressure);

signals:
    valveStateChanged(int valveNumber, bool on);
    pumpStateChanged(int pumpNumber, bool on);
    pressureChanged(int controllerNumber, double pressure);

protected:
    std::map<Component, int> mCurrentStates;

    void initStates(); // called by constructor. loop through all Components, initialize mCurrentStates to -1 for each of them
};

#endif
