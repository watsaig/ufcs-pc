import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 1200
    height: 1000
    title: qsTr("Microfluidics control system")

    SwipeView {
        id: swipeView
        padding: 20
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        ManualControl{
            id: manualControlView
        }

        RoutineControl {
            id: routineControlView
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
    }
}
