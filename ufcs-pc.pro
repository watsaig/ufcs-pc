QT += qml \
      quick \
      serialport \
      bluetooth

CONFIG += c++11
CONFIG += qtquickcompiler

HEADERS += \
    src/cpp/communicator.h \
    src/cpp/constants.h \
    src/cpp/applicationcontroller.h \
    src/cpp/logger.h \
    src/cpp/routinecontroller.h \
    src/cpp/guihelper.h \
    src/cpp/bluetoothcommunicator.h \
    src/cpp/serialcommunicator.h

SOURCES += \
    src/cpp/logger.cpp \
    src/cpp/main.cpp \
    src/cpp/communicator.cpp \
    src/cpp/applicationcontroller.cpp \
    src/cpp/routinecontroller.cpp \
    src/cpp/guihelper.cpp \
    src/cpp/bluetoothcommunicator.cpp \
    src/cpp/serialcommunicator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = C:/Qt/5.10.1/msvc2017_64/qml/QtQuick/Controls.2
QML_IMPORT_PATH += $$PWD/src/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
#QML_DESIGNER_IMPORT_PATH += C:/Qt/5.10.1/msvc2017_64/qml/QtQuick/Controls.2/designer

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/qml/GraphicalChipControl.qml \
    src/qml/GraphicalChipControlv5.qml \
    src/qml/GraphicalControl.qml \
    src/qml/GraphicalControlDropletGenerator.qml \
    src/qml/GraphicalControlv5Chip.qml \
    src/qml/LabeledValveSwitch.qml \
    src/qml/GraphicalValveSwitch.qml \
    src/qml/SettingsLabel.qml \
    src/qml/main.qml \
    res/qtquickcontrols2.conf \
    src/qml/ManualControl.qml \
    src/qml/PressureController.qml \
    src/qml/PressureControlPane.qml \
    src/qml/ValveSwitch.qml \
    src/qml/Settings.qml \
    src/qml/Style.qml \
    src/qml/RoutineControl.qml \
    src/qml/LogScreen.qml \
    src/qml/PumpSwitch.qml \
    src/qml/MultiplexerControl.qml

RC_ICONS = res/icons/icon.ico

# Use git to obtain the version number, so that it can be displayed within the application at run-time
win32 {
    GIT_BIN = $$system(where git)
}
unix|mac {
    GIT_BIN = $$system(which git)
}
isEmpty(GIT_BIN) {
    warning("Git not found. Version number will be missing from application. Add git to PATH to resolve this.")
    DEFINES += GIT_VERSION=\\\"UNKNOWN\\\"
} else {
   GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
   GIT_VERSION ~= s/g/"" # Remove the "g" which is prepended to the hash
   DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
}

#DEFINES += LOG_TO_TERMINAL # Write logs to terminal as well as to a file and to the application
