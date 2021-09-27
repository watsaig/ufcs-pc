#ifndef TESTROUTINES_H
#define TESTROUTINES_H

#include <QtTest/QtTest>
#include <QtCore/QDebug>

#include "communicator.h"
#include "routinecontroller.h"
#include "applicationcontroller.h"

class TestRoutines : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testParsing();
    void testRunning();
private:
    void createDummyRoutineFile(QString url);

    QString mTempFileLocation;
    RoutineController* r;
};

class RoutineMockApplicationController : public ApplicationController
{
    // To do: use a mocking library instead of this
public:
    RoutineMockApplicationController() {}
    int nValves() { return 32; }
    int nPumps() { return 2; }
    int nPressureControllers() { return 2; }
    double minPressure(int controllerNumber) { Q_UNUSED(controllerNumber); return 0;}
    double maxPressure(int controllerNumber) { Q_UNUSED(controllerNumber); return 30;}
};

#endif
