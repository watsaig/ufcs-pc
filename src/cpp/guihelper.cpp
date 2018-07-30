#include "guihelper.h"
#include "applicationcontroller.h"

/**
 * @brief Set the controller's number
 * @param controllerNumber This should correspond to the pins that the actual (physical) pressure controller is plugged into, on the PCB.
 */
void PCHelper::setControllerNumber(int controllerNumber)
{
    mControllerNumber = controllerNumber;
    ApplicationController::appController()->registerPCHelper(controllerNumber, this);
}

/**
 * @brief Return the value in PSI of the controller's setpoint, rounded to 2 decimal places
 */
double PCHelper::setPointInPsi() const
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    return round(100.0*(mSetPoint*(max-min) + min))/100.0;
}

/**
 * @brief Return the value in PSI of the controller's measured pressure, rounded to 2 decimal places
 */
double PCHelper::measuredValueInPsi() const
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    return round(100.0*(mMeasuredValue*(max-min) + min))/100.0;
}

/**
 * @brief Update the display of the controller's setpoint, i.e. target value. This affects only the GUI component.
 * @param val A double between 0 and 1, representing the minimum and maximum possible pressure, respectively
 *
 * This should be called when the setpoint is changed (e.g. by a routine) to update the GUI component.
 */
void PCHelper::setSetPoint(double val)
{
    qDebug() << "PCHelper: Setting pressure to " << val;
    mSetPoint = val;
    emit setPointChanged(val);
}

/**
 * @brief Update the display of the controller's setpoint, i.e. target value. This affects only the GUI component.
 * @param val A value in PSI. If it is not within the bounds of the pressure controller's min and max values, it is ignored.
 */
void PCHelper::setSetPointInPsi(double val)
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    if (val > max || val < min) {
        qDebug() << "PCHelper::setSetPointInPsi: value out of bounds";
        return;
    }

    double setPoint = (val - min)/(max - min);
    this->setSetPoint(setPoint);
}

/**
 * @brief Set the measured value. This should be called when the (physical) pressure controller sends a new value.
 * @param val A double between 0 and 1, representing the minimum and maximum possible pressure, respectively
 *
 * This updates the GUI component.
 */
void PCHelper::setMeasuredValue(double val)
{
    if (mMeasuredValue != val) {
        mMeasuredValue = val;
        emit measuredValueChanged(val);
    }
}

/**
 * @brief Set the measured value. This should be called when the (physical) pressure controller sends a new value.
 * @param val A value in PSI. If it is not within the bounds of the pressure controller's min and max values, it is ignored.
 */
void PCHelper::setMeasuredValueInPsi(double val)
{
    double max = ApplicationController::appController()->maxPressure(mControllerNumber);
    double min = ApplicationController::appController()->minPressure(mControllerNumber);

    if (val > max || val < min) {
        qDebug() << "PCHelper::setMeasuredValueInPsi: value out of bounds";
        return;
    }

    double setPoint = (val - min)/(max - min);
    this->setMeasuredValue(setPoint);
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
        emit stateChanged(newState);
    }
}
