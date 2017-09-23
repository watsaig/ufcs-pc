#ifndef GUIHELPER_H
#define GUIHELPER_H

#include <QObject>
#include <QDebug>


/*
 * Collection of helper classes, to provide a bidirectional interface between the C++ backend and QML gui
 */

class ApplicationController;

class PCHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double setPoint READ setPoint WRITE setSetPoint NOTIFY setPointChanged)
    Q_PROPERTY(double measuredValue READ measuredValue WRITE setMeasuredValue NOTIFY measuredValueChanged)
    Q_PROPERTY(int controllerNumber MEMBER mControllerNumber WRITE setControllerNumber)
    Q_PROPERTY(double setPointInPsi READ setPointInPsi)
    Q_PROPERTY(double measuredValueInPsi READ measuredValueInPsi)

public:
    double setPoint() const { return mSetPoint ; }
    double measuredValue() const { return mMeasuredValue; }
    double setPointInPsi() const;
    double measuredValueInPsi() const;

    void setControllerNumber(int controllerNumber);

public slots:
    void setSetPoint(double val);
    void setMeasuredValue(double val);

signals:
    void setPointChanged (double val);
    void measuredValueChanged(double val);

private:
    double mSetPoint;
    double mMeasuredValue;
    int mControllerNumber; // TODO: make this a property
};


class ValveSwitchHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(int valveNumber MEMBER mValveNumber WRITE setValveNumber)

public:
    bool state() { return mState; }
    void setValveNumber(int valveNumber);

public slots:
    void setState(bool newState);

signals:
    void stateChanged(bool newState);

private:
    bool mState; // true: open. false: closed
    int mValveNumber;
};

#endif // GUIHELPER_H
