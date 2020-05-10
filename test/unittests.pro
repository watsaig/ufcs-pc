QT += qml quick core serialport testlib bluetooth

HEADERS += \
    testcommunicator.h \
    ../src/cpp/bluetoothcommunicator.h \
    ../src/cpp/serialcommunicator.h \
    ../src/cpp/communicator.h \
    ../src/cpp/constants.h \
    ../src/cpp/applicationcontroller.h \
    ../src/cpp/guihelper.h \
    ../src/cpp/routinecontroller.h

SOURCES += \
    testcommunicator.cpp \
    ../src/cpp/bluetoothcommunicator.cpp \
    ../src/cpp/serialcommunicator.cpp \
    ../src/cpp/communicator.cpp \
    ../src/cpp/applicationcontroller.cpp \
    ../src/cpp/guihelper.cpp \
    ../src/cpp/routinecontroller.cpp

INCLUDEPATH += ../src/cpp/

DEFINES += TESTING
DEFINES += GIT_VERSION=0

CONFIG += c++14
