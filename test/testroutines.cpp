#include "testroutines.h"

void TestRoutines::initTestCase()
{
    mTempFileLocation = "file:./dummyroutine.txt";
    createDummyRoutineFile(mTempFileLocation);

    ApplicationController* controller = new RoutineMockApplicationController();
    r = new RoutineController(controller);
}

void TestRoutines::cleanupTestCase()
{
    // delete mTempFileLocation
}

void TestRoutines::testParsing()
{
    QSignalSpy spy(r, SIGNAL(error(QString)));

    r->loadFile(mTempFileLocation);
    r->verify();

    // The dummy file defined below has 17 lines, including empty lines and comments.
    // It has 6 valid commands, and 3 errors.
    QCOMPARE(r->fileContents().length(), 17);
    QCOMPARE(r->numberOfSteps(), 6);
    QCOMPARE(spy.count(), 3);
}


void TestRoutines::testRunning()
{
    QSignalSpy errorSpy(r, SIGNAL(error(QString)));
    QSignalSpy valveSpy(r, SIGNAL(setValve(uint, bool)));
    QSignalSpy pressureSpy(r, SIGNAL(setPressure(uint, double)));

    r->loadFile(mTempFileLocation);
    r->begin();

    while(r->status() != RoutineController::Finished)
        QTest::qSleep(100);

    // There are 2 valid valve commands and 2 valid pressure commands
    QCOMPARE(valveSpy.count(), 2);
    QCOMPARE(valveSpy[0][0].toUInt(), 14);
    QCOMPARE(valveSpy[0][1].toBool(), true);
    QCOMPARE(valveSpy[1][0].toUInt(), 14);
    QCOMPARE(valveSpy[1][1].toBool(), false);

    QCOMPARE(pressureSpy.count(), 2);
    QCOMPARE(pressureSpy[0][0].toUInt(), 1);
    QCOMPARE(pressureSpy[0][1].toDouble(), 7.5/30);
    QCOMPARE(pressureSpy[1][0].toUInt(), 2);
    QCOMPARE(pressureSpy[1][1].toDouble(), 3.1/30);
}


void TestRoutines::createDummyRoutineFile(QString url)
{
    const char * dummyRoutine = R"(
# This is a comment
valve 14 open
wait 1
    valve 14 close
wait 1 seconds
#And another comment

# an invalid wait command
wait 2 24 seconds

# a couple of invalid valve numbers
valve 43 open
valve -1 close

pressure 1 7.5 # psi
pressure 2 3.1)";


    QFile file(QUrl(url).toLocalFile());
    file.open(QIODevice::WriteOnly);

    file.write(dummyRoutine);
    file.close();
}
