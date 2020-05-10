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
 * @brief Return the value in PSI of the controller's setpoint, rounded to 1 or 2 decimal places
 */
double PCHelper::setPointInPsi() const
{

    // Number of decimal points displayed. Since there are only 255 steps, it doesn't make
    // sense to display 2 decimal points for pressure regulators with more than  25.5 PSI range.

    int decimals = 1;
    if ((mMaxPressure - mMinPressure) < 25.5)
        decimals = 2;
    double precision = pow(10, decimals);

    return round(precision*(mSetPoint*(mMaxPressure - mMinPressure) + mMinPressure))/precision;
}

/**
 * @brief Return the value in PSI of the controller's measured pressure, rounded to 1 or 2 decimal places
 */
double PCHelper::measuredValueInPsi() const
{
    // Number of decimal points displayed
    int decimals = 1;
    if ((mMaxPressure - mMinPressure) < 25.5)
        decimals = 2;
    double precision = pow(10, decimals);

    return round(precision*(mMeasuredValue*(mMaxPressure - mMinPressure) + mMinPressure))/precision;
}

/**
 * @brief Update the display of the controller's setpoint, i.e. target value. This affects only the GUI component.
 * @param val A double between 0 and 1, representing the minimum and maximum possible pressure, respectively
 *
 * This should be called when the setpoint is changed (e.g. by a routine) to update the GUI component.
 */
void PCHelper::setSetPoint(double val)
{
    //qDebug() << "PCHelper: Setting pressure to " << val;
    mSetPoint = val;
    emit setPointChanged(val);
}

/**
 * @brief Update the display of the controller's setpoint, i.e. target value. This affects only the GUI component.
 * @param val A value in PSI. If it is not within the bounds of the pressure controller's min and max values, it is ignored.
 */
void PCHelper::setSetPointInPsi(double val)
{
    if (val > mMaxPressure || val < mMinPressure) {
        qDebug() << "PCHelper::setSetPointInPsi: value out of bounds";
        return;
    }

    double setPoint = (val - mMinPressure)/(mMaxPressure - mMinPressure);
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
    if (val > mMaxPressure || val < mMinPressure) {
        qDebug() << "PCHelper::setMeasuredValueInPsi: value out of bounds";
        return;
    }

    double pv = (val - mMinPressure)/(mMaxPressure - mMinPressure);
    this->setMeasuredValue(pv);
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

void PumpSwitchHelper::setPumpNumber(int pumpNumber)
{
    mPumpNumber = pumpNumber;
    ApplicationController::appController()->registerPumpSwitchHelper(pumpNumber, this);
}

void PumpSwitchHelper::setState(bool newState)
{
    if (mState != newState) {
        mState = newState;
        emit stateChanged(newState);
    }
}
