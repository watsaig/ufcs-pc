import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: mainWindow
    visible: true
    width: Backend.windowWidth
    height: Backend.windowHeight
    title: qsTr("Microfluidics control system")

    // if necessary, set this to be the max of the minimum widths of each view
    minimumWidth: 300
    minimumHeight: 500

    Material.theme: Backend.darkMode ? Material.Dark : Material.Light

    onWidthChanged: Backend.windowWidth = width
    onHeightChanged: Backend.windowHeight = height

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        ManualControl{
            id: manualControlView
        }

        RoutineControl {
            id: routineControlView
        }

        LogScreen {
            id: logScreenView
        }

        Settings {
            id: settingsView
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Manual control")
        }

        TabButton {
            text: qsTr("Routines")
        }

        TabButton {
            text: qsTr("Log")
        }

        TabButton {
            width: 50
            icon.name: "settings"
            icon.source: Backend.darkMode ? "qrc:/res/images/settings_icon_dark_theme.png" : "qrc:/res/images/settings_icon_light_theme.png"
        }
    }

    Component {
        id: graphicalControlTab
        TabButton { text: "Graphical control" }
    }

    Component {
        id: graphicalControlViewComponent
        GraphicalControl {
            Component.onCompleted: {
                // Note: it appears that a Qt bug causes the minimum window width to be miscalculated at
                // startup, on High DPI displays. For example: if minimumWidth is 800, and the app is
                // opened on a display with scaling set to 125%, then the window will be resized to (minimum) 1000.
                // (if the window was bigger than 1000, then nothing happens, as expected)
                // So, if you observe unexpected resizing of the window just after launch, this is probably why.
                mainWindow.minimumWidth = Qt.binding(function() { return minimumWidth })
            }
        }
    }

    Component.onCompleted: {
        console.info("Application version: " + Backend.appVersion)
        console.info("Writing to log file at: " + Backend.logFilePath)

        if (Backend.showGraphicalControl) {
            swipeView.insertItem(1, graphicalControlViewComponent.createObject(swipeView))
            tabBar.insertItem(1, graphicalControlTab.createObject(tabBar))
        }
    }
}
