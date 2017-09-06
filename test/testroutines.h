#include <QtTest/QtTest>
#include <QtCore/QDebug>

#include "communicator.h"
#include "routinecontroller.h"

class TestRoutines : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testParsing();

    void onErrorReceived(QString errorString);
private:
    void createDummyRoutineFile(QString url);

    QString mTempFileLocation;
    int mNumberOfErrorsReceived;
};
