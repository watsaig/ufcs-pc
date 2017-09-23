import QtQuick 2.7
import QtQuick.Controls 2.0
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

        BetterManualControl{
            id: manualControlView
        }

        Page {
            Label {
                text: qsTr("Routines will be loaded and executed from here")
                anchors.centerIn: parent
            }
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
