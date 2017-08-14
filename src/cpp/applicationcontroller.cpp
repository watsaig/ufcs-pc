#include "applicationcontroller.h"

ApplicationController::ApplicationController(QObject *parent) : QObject(parent)
{
    mCommunicator.connect();

}
