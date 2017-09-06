#ifndef ROUTINECONTROLLER_H
#define ROUTINECONTROLLER_H

#include <thread>
#include <mutex>

#include <QtCore>
#include <QStringList>

#include "communicator.h"

/**
 * @brief The RoutineController class loads and runs routines, i.e pre-programmed sequences of actions.
 *
 * Routines are simple text files containing a list of actions to take such as opening or closing a given valve,
 * or setting the current pressure.
 *
 * These routines should be written in a text file, with one action per line. See below for the accepted syntax.
 * After creating a RoutineController instance, load your routine with the loadFile function, and verify that it
 * contains no errors by calling verify(). This verifies every action without executing them, and returns the number
 * of errors found.
 *
 * You can then safely call begin() to run the routine. It is run in a separate thread to prevent blocking. Status
 * can be checked with the status() and currentStep() functions. When execution is over, the finished() signal is emitted.
 *
 * TODO: documentation on syntax
 */
class RoutineController : public QObject
{
    Q_OBJECT

public:
    enum RunStatus {
        NotReady,
        Ready,
        Running,
        Finished
    };

    RoutineController(Communicator *communicator);
    virtual ~RoutineController() {}

    bool loadFile(QString fileUrl);
    int verify();
    void begin();

    RunStatus status();

    int currentStep();
    int numberOfSteps();
    const QStringList &steps();

signals:
    /// Emitted whenever an error is encountered
    void error(QString errorString);

    /// Emitted when the execution of the routine is over
    void finished();

private:
    void run(bool dummyRun);
    void reportError(const QString& errorString);

    std::atomic<RunStatus> mRunStatus;
    std::atomic<int> mCurrentStep;
    std::atomic<int> mErrorCount;

    /// The raw contents of the routine file, including empty lines and comments
    QStringList mSteps;

    int mNumberOfSteps;

    Communicator * mCommunicator;
};

#endif // ROUTINECONTROLLER_H
