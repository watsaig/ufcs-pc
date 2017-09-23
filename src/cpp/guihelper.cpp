#include "guihelper.h"
#include "applicationcontroller.h"

void PCHelper::setControllerNumber(int controllerNumber)
{
    mControllerNumber = controllerNumber;
    ApplicationController::appController()->registerPCHelper(controllerNumber, this);
}

/**
 * @brief Return the value in PSI of the controller's setpoint, rounded to 1 decimal place
 */
double PCHelper::setPointInPsi() const
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    return round(10.0*(mSetPoint*(max-min) + min))/10.0;
}

/**
 * @brief Return the value in PSI of the controller's measured pressure, rounded to 1 decimal place
 */
double PCHelper::measuredValueInPsi() const
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    return round(10.0*(mMeasuredValue*(max-min) + min))/10.0;
}

/**
 * @brief Set the pressure controller's setpoint, i.e. target value
 * @param val A double between 0 and 1, representing the minimum and maximum possible pressure, respectively
 *
 * This sends the new requested setpoint to the microcontroller, and updates the GUI component (thus this function can
 * be called both from QML and C++ with the same result).
 */
void PCHelper::setSetPoint(double val)
{
    qDebug() << "Setting pressure to " << val;
    mSetPoint = val;

    ApplicationController::appController()->setPressure(mControllerNumber, val);
    emit setPointChanged(val);


    // for testing purposes only. Remove the following when done.
    /*
    double measured = double(qrand()) / double(RAND_MAX);
    qDebug() << "simulated measured value: " << measured;
    setMeasuredValue(measured);
    */
}

/**
 * @brief Set the measured value. This should be called when the (physical) pressure controller sends a new value.
 * @param val A double between 0 and 1, representing the minimum and maximum possible pressure, respectively
 *
 * This updates the GUI component.
 */
void PCHelper::setMeasuredValue(double val)
{
    qDebug() << "Setting measured value";
    if (mMeasuredValue != val) {
        mMeasuredValue = val;
        emit measuredValueChanged(val);
    }
}

void ValveSwitchHelper::setValveNumber(int valveNumber)
{
    mValveNumber = valveNumber;
    ApplicationController::appController()->registerValveSwitchHelper(valveNumber, this);
}

void ValveSwitchHelper::setState(bool newState)
{
    if (mState != newState) {
        mState = newState;
        ApplicationController::appController()->setValve(mValveNumber, newState);

        emit stateChanged(newState);
    }
}
