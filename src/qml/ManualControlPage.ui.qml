import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 720

    RowLayout {
        width: 691
        height: 317
        anchors.horizontalCenterOffset: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 64
        anchors.top: parent.top

        ColumnLayout {
            id: columnLayout1
            width: 465
            height: 364
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

            Text {
                id: text2
                text: qsTr("Valves")
                font.pixelSize: 22
                wrapMode: Text.WordWrap
            }

            GridLayout {
                id: gridLayout
                width: 100
                height: 100
                rows: 5
                columns: 5

                Switch {
                    id: switch3
                    width: 80
                    height: 40
                    text: qsTr("1")
                }

                Switch {
                    id: switch4
                    width: 80
                    height: 40
                    text: qsTr("2")
                }

                Switch {
                    id: switch5
                    width: 80
                    height: 40
                    text: qsTr("3")
                }

                Switch {
                    id: switch6
                    width: 80
                    height: 40
                    text: qsTr("4")
                }

                Switch {
                    id: switch7
                    width: 80
                    height: 40
                    text: qsTr("5")
                }

                Switch {
                    id: switch8
                    width: 80
                    height: 40
                    text: qsTr("6")
                }

                Switch {
                    id: switch9
                    width: 80
                    height: 40
                    text: qsTr("7")
                }

                Switch {
                    id: switch10
                    width: 80
                    height: 40
                    text: qsTr("8")
                }

                Switch {
                    id: switch11
                    width: 80
                    height: 40
                    text: qsTr("9")
                }

                Switch {
                    id: switch12
                    width: 80
                    height: 40
                    text: qsTr("10")
                }

                Switch {
                    id: switch13
                    width: 80
                    height: 40
                    text: qsTr("11")
                }

                Switch {
                    id: switch14
                    width: 80
                    height: 40
                    text: qsTr("12")
                }

                Switch {
                    id: switch15
                    width: 80
                    height: 40
                    text: qsTr("13")
                }

                Switch {
                    id: switch16
                    width: 80
                    height: 40
                    text: qsTr("14")
                }

                Switch {
                    id: switch17
                    width: 80
                    height: 40
                    text: qsTr("15")
                }

                Switch {
                    id: switch18
                    width: 80
                    height: 40
                    text: qsTr("16")
                }

                Switch {
                    id: switch19
                    width: 80
                    height: 40
                    text: qsTr("17")
                }

                Switch {
                    id: switch20
                    width: 80
                    height: 40
                    text: qsTr("18")
                }

                Switch {
                    id: switch21
                    width: 80
                    height: 40
                    text: qsTr("19")
                }

                Switch {
                    id: switch22
                    width: 80
                    height: 40
                    text: qsTr("20")
                }

                Switch {
                    id: switch23
                    width: 80
                    height: 40
                    text: qsTr("21")
                }

                Switch {
                    id: switch24
                    width: 80
                    height: 40
                    text: qsTr("22")
                }

                Switch {
                    id: switch25
                    width: 80
                    height: 40
                    text: qsTr("23")
                }

                Switch {
                    id: switch26
                    width: 80
                    height: 40
                    text: qsTr("24")
                }

                Switch {
                    id: switch27
                    width: 80
                    height: 40
                    text: qsTr("25")
                }
            }
        }

        ColumnLayout {
            id: columnLayout
            width: 196
            height: 364
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

            Text {
                id: text1
                text: qsTr("Pressure controllers")
                wrapMode: Text.WordWrap
                font.pixelSize: 22
            }

            Slider {
                id: pressure1slider
                stepSize: 0.2
                to: 15
                orientation: Qt.Horizontal
                value: 0
            }

            Slider {
                id: pressure2slider
                stepSize: 0.2
                to: 50
                value: 0
                orientation: Qt.Horizontal
            }

            Slider {
                id: pressure3slider
                to: 10
                value: 0
                orientation: Qt.Horizontal
            }
        }

    }

    Switch {
        id: switch1
        x: 14
        y: 426
        text: qsTr("Pump 1")
    }

    Connections {
        target: switch1
        onToggled: Backend.setPump(1, switch1.checked)
    }

    Switch {
        id: switch2
        x: 156
        y: 426
        text: qsTr("Pump 2")
    }
}
