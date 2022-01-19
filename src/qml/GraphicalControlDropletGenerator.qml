import QtQuick 2.0

import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0 // for the Style singleton
Item {
    id: control
    RowLayout {
        id: pressureControllerRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        Column {
            Layout.alignment: Qt.AlignHCenter

            Label {
                text : "Oil (valve manifolds)"
                font.pointSize: Style.heading2.fontSize
                bottomPadding: 10
                anchors.right: parent.right
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
            }

            PressureController {
                id: pressureControllerControl
                controllerNumber: 1
                minPressure: 0
                maxPressure: 29.5
                sliderHeight: 500
                largeHandle: true
            }
        }

        Column {
            Layout.alignment: Qt.AlignHCenter

            Label {
                text : "Water (flow line connector)"
                font.pointSize: Style.heading2.fontSize
                bottomPadding: 10
                anchors.right: parent.right
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
            }

            PressureController {
                id: pressureControllerFlow
                controllerNumber: 2
                minPressure: 0
                maxPressure: 4.8
                sliderHeight: 500
                largeHandle: true
            }
        }
    }
}
