import QtQuick 2.0
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import org.example.ufcs 1.0 // for the Style singleton

Item {
    Flickable {
        anchors.fill: parent
        contentHeight: topLevelColumnLayout.implicitHeight
        contentWidth: width
        clip: true
        ScrollBar.vertical: ScrollBar {}

        ColumnLayout {
            id: topLevelColumnLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            Component.onCompleted: {
                //Backend.connect()
            }

            Label {
                text: qsTr("Valves")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
                topPadding: 0
            }


            GridLayout {
                id: controlLayerValveGrid
                Layout.fillWidth: true

                columnSpacing: 5
                rowSpacing: 5
                columns: 8

                ValveSwitch { valveNumber: 1 }
                ValveSwitch { valveNumber: 2 }
                ValveSwitch { valveNumber: 3 }
                ValveSwitch { valveNumber: 4 }
                ValveSwitch { valveNumber: 5 }
                ValveSwitch { valveNumber: 6 }
                ValveSwitch { valveNumber: 7 }
                ValveSwitch { valveNumber: 8 }
                ValveSwitch { valveNumber: 9 }
                ValveSwitch { valveNumber: 10 }
                ValveSwitch { valveNumber: 11 }
                ValveSwitch { valveNumber: 12 }
                ValveSwitch { valveNumber: 13 }
                ValveSwitch { valveNumber: 14 }
                ValveSwitch { valveNumber: 15 }
                ValveSwitch { valveNumber: 16 }
                ValveSwitch { valveNumber: 17 }
                ValveSwitch { valveNumber: 18 }
                ValveSwitch { valveNumber: 19 }
                ValveSwitch { valveNumber: 20 }
                ValveSwitch { valveNumber: 21 }
                ValveSwitch { valveNumber: 22 }
                ValveSwitch { valveNumber: 23 }
                ValveSwitch { valveNumber: 24 }
                ValveSwitch { valveNumber: 25 }
                ValveSwitch { valveNumber: 26 }
                ValveSwitch { valveNumber: 27 }
                ValveSwitch { valveNumber: 28 }
                ValveSwitch { valveNumber: 29 }
                ValveSwitch { valveNumber: 30 }
                ValveSwitch { valveNumber: 31 }
                ValveSwitch { valveNumber: 32 }
            }

            Label {
                text: qsTr("Multiplexer")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
            }

            MultiplexerControl {
                id: multiplexerControlElement
            }

            Label {
                text: qsTr("Pressure control")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
            }

            GridLayout {
                flow: GridLayout.TopToBottom
                rows: 2
                columnSpacing: 70
                Layout.maximumWidth: controlLayerValveGrid.width

                Label {
                    text: qsTr("Control layer")
                    font.pointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                PressureController { controllerNumber: 1 }

                Label {
                    text: qsTr("Flow layer")
                    font.pointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                PressureController { controllerNumber: 2 }

                Label {
                    text: qsTr("Pumps")
                    font.pointSize: 14
                    bottomPadding: 10
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                ColumnLayout {
                    PumpSwitch { pumpNumber: 1 }
                    PumpSwitch { pumpNumber: 2 }
                }
            }


            Label {
                text: qsTr("Status")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
            }

            Label {
                text: "Microcontroller is " + Backend.connectionStatus
                //padding: 12
                leftPadding: Style.title.paddingLeft
                Layout.alignment: Qt.AlignLeft
            }

            Button {
                id: reconnectButton
                text: qsTr("Reconnect")
                onClicked: Backend.connect()
                padding: 12
                visible: Backend.connectionStatus == "Disconnected"
            }

            Button {
                id: refreshButton
                text: qsTr("Refresh component statuses")
                onClicked: Backend.requestRefresh()
                padding: 12
                visible: Backend.connectionStatus == "Connected"
            }


        }
    }
}
