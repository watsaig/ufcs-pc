#include "routinecontroller.h"

RoutineController::RoutineController(Communicator * communicator)
    : mCommunicator(communicator)
    , mRunStatus(NotReady)
    , mCurrentStep(-1)
{

}

/**
 * @brief Load and parse the routine stored in the specified file
 * @param fileUrl The URL of the text file containing the routine
 * @return True if the routine was loaded successfully, false otherwise.
 *
 * Note that the routine is not checked by this function, only loaded. Use the
 * verify() function for sanity checking.
 */
bool RoutineController::loadFile(QString fileUrl)
{
    QFile file(fileUrl);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Could not load file" << fileUrl << ":" << file.errorString();
        return false; // TODO: throw exception ?
    }

    QTextStream stream(&file);

    while (!stream.atEnd())
        mSteps << stream.readLine();

    file.close();

    mRunStatus = Ready;

    return true;
}

/**
 * @brief Check the routine for errors
 * @return The number of errors found in the routine.
 *
 * When an error is found, the `error` signal is emitted. You must connect to this signal in order to retrieve
 * the error strings.
 */
int RoutineController::verify()
{
    run(true);
    return mErrorCount;
}

/**
 * @brief Start the routine. This function returns immediately; the routine is launched in a separate thread.
 */
void RoutineController::begin()
{
    std::thread t([this] { run(false); });
    t.detach();
}

/**
 * @brief Return the index of the routine step currently being executed
 * @return The current step's index, or -1 if not currently executing a routine.
 */
int RoutineController::currentStep()
{
    return mCurrentStep;
}

/**
 * @brief Return the number of valid steps in the routine
 */
int RoutineController::numberOfSteps()
{
    return mNumberOfSteps;
}

const QStringList& RoutineController::steps()
{
    return mSteps;
}

/**
 * @brief Run the routine
 * @param dummyRun If true, all steps will be checked for errors but not executed.
 *
 * Errors found during execution are emitted by the error() signal (see the reportError function)
 *
 * This function can be run directly (recommended for error-checking) or in a thread (recommended for
 * actual execution).
 *
 */
void RoutineController::run(bool dummyRun)
{
    // Counter for the number of valid steps in the routine
    int nValidSteps(0);

    mErrorCount = 0;
    mCurrentStep = -1;

    if (!dummyRun)
        mRunStatus = Running;

    for (int i(0); i < mSteps.size(); ++i) {
        if (!dummyRun)
            mCurrentStep = i;

        QString line = mSteps[i];
        line.remove(QRegExp("#.*")); // remove hashes and all following characters.
        line = line.simplified(); // remove excess whitespace

        if (line.isEmpty())
            continue;

        QStringList list = line.split(' ');
        int length = list.size();

        if (list[0] == "valve") {
            // Expected format: valve <number> <open / close>. e.g: valve 12 open
            if (length != 3) {
                reportError("Line " + QString::number(i+1) + ": line starting with \"valve\" should contain 3 arguments. For example, \"valve 12 open\"");
                continue;
            }

            bool ok;
            int valveNumber = list[1].toInt(&ok);
            if (!ok || valveNumber < 1 || valveNumber > mCommunicator->nValves()) {
                reportError("Line " + QString::number(i+1) + ": invalid valve ID: " + list[1]
                            + ". Must be an integer between 1 and " + QString::number(mCommunicator->nValves()));
                continue;
            }

            QString state = list[2];
            if (state != "open" && state != "close") {
                reportError("Line " + QString::number(i+1) + ": valve status not recognized: " + state);
                continue;
            }

            nValidSteps++;
            if (!dummyRun)
                mCommunicator->setValve(valveNumber, (state == "open"));
        }

        else if (list[0] == "pressure") {
            // Expected format: pressure <number> <value>. e.g: pressure 1 8.2
            if (length != 3) {
                reportError("Line " + QString::number(i+1) + ": line starting with \"pressure\" should contain 3 arguments. For example, \"pressure 2 6.3\"");
                continue;
            }
            bool ok;
            int controllerNumber = list[1].toInt(&ok);
            if (!ok || controllerNumber < 1 || controllerNumber > mCommunicator->nPressureControllers()) {
                reportError("Line " + QString::number(i+1) + ": invalid pressure controller ID: " + list[1]
                            + ". Must be an integer between 1 and " + QString::number(mCommunicator->nPressureControllers()));
                continue;
            }

            double pressure = list[2].toDouble(&ok);
            if (!ok || pressure < 0) {
                reportError("Line " + QString::number(i+1) + ": Pressure value invalid: " + list[2]);
                continue;
            }
            else if (pressure < mCommunicator->minPressure(controllerNumber) || pressure > mCommunicator->maxPressure(controllerNumber)) {
                reportError("Line " + QString::number(i+1) + ": Pressure value out of bounds for this controller: " + list[2]);
                continue;
                // TODO: not make this an error, but just a warning?
            }

            nValidSteps++;
            if (!dummyRun)
                mCommunicator->setPressure(controllerNumber, pressure); // TODO: this expects a value between 0 and 1.0. Fix this.

        }

        else if (list[0] == "wait") {
            // Expected format:  wait <time> <unit> . <unit> defaults to seconds. e.g: `wait 10 minutes`, `wait 60`
            if (length != 2 && length != 3) {
                reportError("Line " + QString::number(i+1) + ": line starting with \"wait\" should contain 2 or 3 arguments. For example, \"wait 2 min\"");
                continue;
            }

            bool ok;
            double time = list[1].toDouble(&ok);
            if (!ok) {
                reportError("Line " + QString::number(i+1) + ": could not parse wait time argument: " + list[1]);
                continue;
            }

            if (length == 3) {
                double multiplier = 1.0;

                if (list[2] == "ms" || list[2] == "milliseconds" || list[2] == "millisecond" || list[2] == "msec")
                    multiplier = 0.001;
                else if (list[2] == "minutes" || list[2] == "minute" || list[2] == "min" || list[2] == "mins" || list[2] == "min")
                    multiplier = 60;
                else if (list[2] == "hours" || list[2] == "hour" || list[2] == "hrs" || list[2] == "hr" || list[2] == "h")
                    multiplier = 3600;

                time *= multiplier;
            }

            nValidSteps++;
            if (!dummyRun)
                std::this_thread::sleep_for(std::chrono::milliseconds(uint64_t(time*1000)));
        }
    }

    mNumberOfSteps = nValidSteps;

    if (!dummyRun) {
        mRunStatus = Finished;
        emit finished();
    }
}


void RoutineController::reportError(const QString &errorString)
{
    emit error(errorString);
    mErrorCount++;
    // Errors could also be logged or output to terminal here, but beware of
    // race conditions due to run() being executed in a separate thread.
}
