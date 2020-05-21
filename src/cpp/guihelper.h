#ifndef GUIHELPER_H
#define GUIHELPER_H

#include <QObject>
#include <QDebug>


/*
 * Collection of helper classes, to provide a bidirectional interface between the C++ backend and QML gui elements
 *
 * They are intended to be used as attributes of QML elements. For example PCHelper is registered to QML in main.cpp,
 * and should be instantiated within the actual graphical element it supports.
 */

/**
 * @brief Backend for pressure controller GUI elements
 */
class PCHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double setPoint MEMBER mSetPoint NOTIFY setPointChanged)
    Q_PROPERTY(double measuredValue READ measuredValue WRITE setMeasuredValue NOTIFY measuredValueChanged)
    Q_PROPERTY(double setPointInPsi READ setPointInPsi NOTIFY setPointChanged)
    Q_PROPERTY(double measuredValueInPsi READ measuredValueInPsi NOTIFY measuredValueChanged)
    Q_PROPERTY(double minPressure MEMBER mMinPressure);
    Q_PROPERTY(double maxPressure MEMBER mMaxPressure);

public:
    PCHelper();
    double setPoint() const { return mSetPoint ; }
    double measuredValue() const { return mMeasuredValue; }
    double setPointInPsi() const;
    double measuredValueInPsi() const;
    double minPressure() const { return mMinPressure; };
    double maxPressure() const { return mMaxPressure; };

public slots:
    void setSetPoint(double val);
    void setSetPointInPsi(double val);
    void setMeasuredValue(double val);
    void setMeasuredValueInPsi(double val);

signals:
    void setPointChanged (double val);
    void measuredValueChanged(double val);

private:
    double mSetPoint;
    double mMeasuredValue;

    // Min and max pressure in PSI
    double mMinPressure;
    double mMaxPressure;
};


class ValveSwitchHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged)

public:
    bool state() { return mState; }

public slots:
    void setState(bool newState);

signals:
    void stateChanged(bool newState);

private:
    bool mState; // true: open. false: closed
};

class PumpSwitchHelper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool state READ state WRITE setState NOTIFY stateChanged)

public:
    bool state() { return mState; }

public slots:
    void setState(bool newState);

signals:
    void stateChanged(bool newState);

private:
    bool mState; // true: on. false: off
};

#endif // GUIHELPER_H
