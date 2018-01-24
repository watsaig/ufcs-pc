QT += qml quick core serialport testlib

HEADERS += \
    testroutines.h \
    ../src/cpp/communicator.h \
    ../src/cpp/constants.h \
    ../src/cpp/applicationcontroller.h \
    ../src/cpp/guihelper.h \
    ../src/cpp/routinecontroller.h

SOURCES += \
    testroutines.cpp \
    ../src/cpp/communicator.cpp \
    ../src/cpp/applicationcontroller.cpp \
    ../src/cpp/guihelper.cpp \
    ../src/cpp/routinecontroller.cpp

INCLUDEPATH += ../src/cpp/
