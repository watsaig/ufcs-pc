import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    id: control

    // When True: buttons are disabled; user can edit labels
    property bool editingMode : false

    property int minimumWidth: 700 // muxControl.implicitWidth + 2*Style.view.margin
    //property int maximumWidth: 1000

    GridLayout {
        id: topLevelLayout
        anchors.fill: parent
        anchors.margins: Style.view.margin
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        property bool singleColumn: width < 1000
        columns: singleColumn ? 1 : 2
        rows: singleColumn ? 2 : 1

        ChipLayoutV5 {
            id: chipLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.maximumWidth: topLevelLayout.singleColumn ? control.width : control.width - column.maxWidth

            enabled: !editingMode && Backend.connectionStatus == "Connected"
        }

        Column {
            id: column
            Layout.fillWidth: true
            property int maxWidth: Math.min(topLevelLayout.singleColumn ? control.width : control.width*0.67, 1000)
            Layout.maximumWidth: maxWidth
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: qsTr("Multiplexer")
                font.pointSize: Style.heading1.fontSize
                padding: Style.heading1.padding
                leftPadding: Style.heading1.paddingLeft
                Layout.alignment: Qt.AlignTop
            }

            MultiplexerControl {
                id: muxControl
                anchors.left: parent.left
                anchors.right: parent.right

                valves: [3, 4, 5, 6, 7, 8, 9, 10, 12, 13]
                muxModel: ListModel {
                    ListElement { label: "1"; config: "1010101010" }
                    ListElement { label: "2"; config: "1010101001" }
                    ListElement { label: "3"; config: "1010100110" }
                    ListElement { label: "4"; config: "1010100101" }
                    ListElement { label: "5"; config: "1010011010" }
                    ListElement { label: "6"; config: "1010011001" }
                    ListElement { label: "7"; config: "1010010110" }
                    ListElement { label: "8"; config: "1010010101" }
                    ListElement { label: "9"; config: "1001101010" }
                    ListElement { label: "10"; config: "1001101001" }
                    ListElement { label: "11"; config: "1001100110" }
                    ListElement { label: "12"; config: "1001100101" }
                    ListElement { label: "13"; config: "1001011010" }
                    ListElement { label: "14"; config: "1001011001" }
                    ListElement { label: "15"; config: "1001010110" }
                    ListElement { label: "16"; config: "1001010101" }
                    ListElement { label: "17"; config: "0110101010" }
                    ListElement { label: "18"; config: "0110101001" }
                    ListElement { label: "19"; config: "0110100110" }
                    ListElement { label: "20"; config: "0110100101" }
                    ListElement { label: "21"; config: "0110011010" }
                    ListElement { label: "22"; config: "0110011001" }
                    ListElement { label: "23"; config: "0110010110" }
                    ListElement { label: "24"; config: "0110010101" }
                    ListElement { label: "25"; config: "0101101010" }
                    ListElement { label: "26"; config: "0101101001" }
                    ListElement { label: "27"; config: "0101100110" }
                    ListElement { label: "28"; config: "0101100101" }
                    ListElement { label: "29"; config: "0101011010" }
                    ListElement { label: "30"; config: "0101011001" }
                    ListElement { label: "31"; config: "0101010110" }
                    ListElement { label: "32"; config: "0101010101" }
                    ListElement { label: "All"; config: "0000000000" }
                    ListElement { label: "None"; config: "1111111111" }
                    ListElement { label: "1-8"; config: "1010000000" }
                    ListElement { label: "9-16"; config: "1001000000" }
                    ListElement { label: "17-24"; config: "0110000000" }
                    ListElement { label: "25-32"; config: "0101000000" }
                    ListElement { label: "Odd"; config: "0000000010" }
                    ListElement { label: "Even"; config: "0000000001" }

                }
                columns: 8

                Connections {
                    target: RoutineController
                    onSetMultiplexer: {
                        //console.log("Caught setMultiplexer signal, channel: " + label)
                        muxControl.setMuxToLabel(label);
                    }
                }
            }


            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Inputs")
                    Layout.fillWidth: true
                    font.pointSize: Style.heading1.fontSize
                    padding: Style.heading1.padding
                    leftPadding: Style.heading1.paddingLeft
                }

                Button {
                    id: editingModeSwitch
                    text: control.editingMode ? qsTr("Done") : qsTr("Edit labels")
                    flat: true
                    Layout.alignment: Qt.AlignRight
                    onClicked: control.editingMode = !control.editingMode
                }
            }


            MultiplexerControl {
                id: inputMuxControl
                anchors.left: parent.left
                anchors.right: parent.right
                labeledSwitches: true

                valves: [22, 23, 24, 25, 26, 27, 28, 29]
                muxModel: ListModel {
                    ListElement { label: "1"; config: "01010101" }
                    ListElement { label: "2"; config: "01010110" }
                    ListElement { label: "3"; config: "01011001" }
                    ListElement { label: "4"; config: "01011010" }
                    ListElement { label: "5"; config: "01100101" }
                    ListElement { label: "6"; config: "01100110" }
                    ListElement { label: "7"; config: "01101001" }
                    ListElement { label: "8"; config: "01101010" }
                    ListElement { label: "9"; config: "10010101" }
                    ListElement { label: "10"; config: "10010110" }
                    ListElement { label: "11"; config: "10011001" }
                    ListElement { label: "12"; config: "10011010" }
                    ListElement { label: "13"; config: "10100101" }
                    ListElement { label: "14"; config: "10100110" }
                    ListElement { label: "15"; config: "10101001" }
                    ListElement { label: "16"; config: "10101010" }
                    //ListElement { label: "None"; config: "11111111" }
                }
                columns: 8
                Connections {
                    target: RoutineController
                    onSetInputMultiplexer: {
                        //console.log("Caught setMultiplexer signal, channel: " + label)
                        inputMuxControl.setMuxToLabel(label);
                    }
                }
            }

            PressureControlPane {
                id: pressureControlPane
                enabled: !editingMode

                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }

    Component.onCompleted: {
        // Note: it appears that a Qt bug causes the minimum window width to be miscalculated at
        // startup, on High DPI displays on Windows. For example: if minimumWidth is 800, and the app is
        // opened on a display with scaling set to 125%, then the window will be resized to (minimum) 1000.
        // (if the window was bigger than 1000, then nothing happens, as expected)
        // So, if you observe unexpected resizing of the window just after launch, this is probably why.
        mainWindow.minimumWidth = Qt.binding(function() { return minimumWidth })
    }
}







/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:14;flowX:"-324";flowY:161}
}
##^##*/
