import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: mainWindow
    visible: true
    width: Backend.windowWidth
    height: Backend.windowHeight
    title: qsTr("Microfluidics control system")

    Material.theme: Backend.darkMode ? Material.Dark : Material.Light

    onWidthChanged: Backend.windowWidth = width
    onHeightChanged: Backend.windowHeight = height

    SwipeView {
        id: swipeView
        padding: 20
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        ManualControl{
            id: manualControlView
        }

        GraphicalControl {
            id: graphicalControlView
        }

        RoutineControl {
            id: routineControlView
        }

        LogScreen {
            id: logScreenView
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Manual control")
        }

        TabButton {
            text: qsTr("Graphical control")
        }

        TabButton {
            text: qsTr("Routines")
        }

        TabButton {
            text: qsTr("Log")
        }
    }

    Component.onCompleted: {
        console.info("Application version: " + Backend.appVersion)
        console.info("Writing to log file at: " + Backend.logFilePath)
    }
}
