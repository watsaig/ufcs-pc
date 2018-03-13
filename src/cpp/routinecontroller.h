#ifndef ROUTINECONTROLLER_H
#define ROUTINECONTROLLER_H

#include <thread>
#include <mutex>

#include <QtCore>
#include <QStringList>

#include "bluetoothcommunicator.h"

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

    Q_PROPERTY(int currentStep READ currentStep NOTIFY currentStepChanged)
    Q_PROPERTY(RunStatus runStatus READ status NOTIFY runStatusChanged)
    Q_PROPERTY(QStringList errorList READ errors NOTIFY error)
    Q_PROPERTY(QStringList stepsList READ steps NOTIFY currentStepChanged) // TODO add notification

public:
    enum RunStatus {
        NotReady,
        Ready,
        Running,
        Finished
    }; Q_ENUM(RunStatus)

    RoutineController(BluetoothCommunicator *communicator); // TODO: make this private (=> singleton). The parameter can be passed to the "getInstance" function.
    virtual ~RoutineController() {}

    Q_INVOKABLE bool loadFile(QString fileUrl);
    Q_INVOKABLE int verify();
    Q_INVOKABLE void begin();

    RunStatus status();

    int currentStep();
    Q_INVOKABLE int numberOfSteps();
    Q_INVOKABLE int numberOfErrors();

    Q_INVOKABLE const QStringList &steps();

    const QStringList &fileContents();
    const QStringList& errors();

    Q_INVOKABLE QString routineName() { return mRoutineName; }

signals:
    /// Emitted whenever an error is encountered
    void error(QString errorString);

    /// Emitted when switching to a new (valid) step in the routine
    void currentStepChanged(int newStepNumber);

    /// Emitted when the status has changed
    void runStatusChanged(RunStatus newStatus);

    /// Emitted when the routine is finished
    void finished();


    void setValve(int valveNumber, bool open);
    void setPressure(int controllerNumber, double value);

private:
    void reset();
    void run(bool dummyRun);
    void reportError(const QString& errorString);
    void setCurrentStep(int stepNumber);

    /// The serial port interface
    BluetoothCommunicator * mCommunicator;

    std::atomic<RunStatus> mRunStatus;
    std::atomic<int> mCurrentStep;
    std::atomic<int> mErrorCount;

    /// The raw contents of the routine file, including empty lines and comments
    QStringList mLines;

    /// The valid steps of the routine. This is initialized only after verify() has run.
    QStringList mValidSteps;

    /// Number of valid steps in the routine
    int mNumberOfSteps;

    /// The list of errors encountered during verification or execution
    QStringList mErrors;

    /// The routine name (derived from the file name)
    QString mRoutineName;
};

#endif // ROUTINECONTROLLER_H
