#ifndef ROUTINECONTROLLER_H
#define ROUTINECONTROLLER_H

#include <thread>
#include <mutex>
#include <atomic>

#include <QtCore>
#include <QStringList>

class ApplicationController;

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
 * Supported syntax
 * ---------------------
 *
 * valve X [open/close]
 *      Open or close valve X, where X is a number between 1 and appController->nValves(),
 *      or "all", to toggle all valves at once.
 *
 *      Example: valve 12 open
 *
 * pressure X Y
 *      Set pressure regulator X to a pressure of Y PSI.
 *
 *      Example: pressure 1 4.5
 *
 * wait X Y
 *      Pause for some time X. Y defines the units, can be milliseconds, seconds, minutes or hours.
 *      Default is seconds, in case Y is ommitted or does not match any other unit.
 *      See run function for complete list of supported unit formats.
 *
 *      Example: wait 2 minutes
 *
 *
 * multiplexer X
 *      Open multiplexer to channel X, where X is 1-8 or "all".
 *
 */
class RoutineController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentStep READ currentStep NOTIFY currentStepChanged)
    Q_PROPERTY(RunStatus runStatus READ status NOTIFY runStatusChanged)
    Q_PROPERTY(QStringList errorList READ errors NOTIFY error)
    Q_PROPERTY(QStringList stepsList READ steps NOTIFY stepsListChanged)
    Q_PROPERTY(long totalRunTime READ totalRunTime NOTIFY totalRunTimeChanged)

public:
    enum RunStatus {
        NotReady,
        Ready,
        Running,
        Finished,
        Paused
    }; Q_ENUM(RunStatus)

    RoutineController(ApplicationController* applicationController);
    virtual ~RoutineController() {}

    Q_INVOKABLE bool loadFile(QString fileUrl);
    Q_INVOKABLE int verify();
    Q_INVOKABLE void begin();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();

    RunStatus status();

    int currentStep();
    Q_INVOKABLE int numberOfSteps();
    Q_INVOKABLE int numberOfErrors();

    Q_INVOKABLE const QStringList &steps();

    const QStringList &fileContents();
    const QStringList& errors();

    Q_INVOKABLE QString routineName() { return mRoutineName; }
    Q_INVOKABLE long totalRunTime() { return mTotalWaitTime; }

signals:
    /// Emitted when the list of steps is updated
    void stepsListChanged();

    /// Emitted whenever an error is encountered
    void error(QString errorString);

    /// Emitted when switching to a new (valid) step in the routine
    void currentStepChanged(int newStepNumber);

    /// Emitted when the status has changed
    void runStatusChanged(RunStatus newStatus);

    /// Emitted when the routine is finished
    void finished();

    /// Emitted when the routine is paused
    void paused();

    /// Emitted when the routine is resumed
    void resumed();

    /// Emitted when the estimated total run time is updated
    void totalRunTimeChanged(long time);

    void setValve(uint valveNumber, bool open);
    void setPressure(uint controllerNumber, double value);
    void setMultiplexer(QString label);

private:
    void reset();
    void run(bool dummyRun);
    void reportError(const QString& errorString);
    void setCurrentStep(int stepNumber);

    std::atomic<RunStatus> mRunStatus;
    std::atomic<int> mCurrentStep;
    std::atomic<int> mErrorCount;

    /// If true, routine execution stops after the current step
    std::atomic<bool> mStopRequested;

    /// If true, routine execution is paused after the current step
    std::atomic<bool> mPauseRequested;

    /// Mutex used by pause functionality
    std::mutex mPauseMutex;

    /// Condition variable used by pause functionality
    std::condition_variable mPauseConditionVariable;

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

    /// Estimated run time of the routine (sum of wait times)
    long mTotalWaitTime;

    ApplicationController* appController;
};

#endif // ROUTINECONTROLLER_H
