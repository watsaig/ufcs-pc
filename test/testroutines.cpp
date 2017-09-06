#include "testroutines.h"

void noMessageOutput(QtMsgType, const QMessageLogContext&, const QString&)
{}

void TestRoutines::initTestCase()
{
    // Suppress all debug output
    qInstallMessageHandler(noMessageOutput);

    mTempFileLocation = "./dummyroutine.txt";
    createDummyRoutineFile(mTempFileLocation);

    mNumberOfErrorsReceived = 0;
}

void TestRoutines::cleanupTestCase()
{
    // delete mTempFileLocation
}

void TestRoutines::testParsing()
{
    Communicator c;
    RoutineController r(&c);

    connect(&r, SIGNAL(error(QString)), this, SLOT(onErrorReceived(QString)));

    r.loadFile(mTempFileLocation);
    r.verify();


    // The dummy file defined below has 17 lines, including empty lines and comments.
    // It has 6 valid commands, and 3 errors.

    QVERIFY(r.steps().length() == 17);
    QVERIFY(r.numberOfSteps() == 6);
    QVERIFY(mNumberOfErrorsReceived == 3);

}


void TestRoutines::createDummyRoutineFile(QString url)
{
    const char * dummyRoutine = R"(
# This is a comment
valve 14 open
wait 10
    valve 14 close
wait 2 minutes
#And another comment

# an invalid wait command
    wait 2 24 seconds

# a couple of invalid valve numbers
valve 43 open
    valve -1 close

pressure 1 7.5 # psi
pressure 2 5)";


    QFile file(url);
    file.open(QIODevice::WriteOnly);

    file.write(dummyRoutine);
    file.close();
}

void TestRoutines::onErrorReceived(QString errorString)
{
    mNumberOfErrorsReceived++;
}




QTEST_MAIN(TestRoutines)

