import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

Item {
    id: control

    height: column.implicitHeight

    Column {
        id: column
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                id: pressureLabel
                text: qsTr("Pressure")
                Layout.rightMargin: 100
                Layout.fillWidth: true
                font.pointSize: 15

            }

            Label {
                id: label1
                text: qsTr("Control")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Label {
                id: label2
                text: pressureControllerControl.pv + " PSI"
                Layout.rightMargin: 15
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Label {
                id: label3
                text: qsTr("Flow")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Label {
                id: label4
                text: pressureControllerFlow.pv + " PSI"
                Layout.rightMargin: 15
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Button {
                id: showHideButton
                text: pressureControllerRow.shown ? qsTr("Hide") : qsTr("Show")
                Layout.leftMargin: 20
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                flat: true
                onClicked: {
                    pressureControllerRow.shown = !pressureControllerRow.shown
                }
            }

        }

        Row {
            id: pressureControllerRow
            anchors.left: parent.left
            anchors.right: parent.right
            property bool shown: false
            height: shown ? implicitHeight : 0

            Behavior on height {
                NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
            }

            PressureController {
                id: pressureControllerControl
                controllerNumber: 1
                visible: pressureControllerRow.shown
            }

            PressureController {
                id: pressureControllerFlow
                controllerNumber: 2
                visible: pressureControllerRow.shown
            }


        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:11;flowX:-216;flowY:167}D{i:12;flowX:-207;flowY:173}
D{i:13;flowX:-191;flowY:177}D{i:14;flowX:-191;flowY:188}D{i:15;flowX:-196;flowY:116}
D{i:16;flowX:-214;flowY:148}D{i:10;anchors_x:8;flowX:"-207";flowY:173}D{i:18;flowX:-212;flowY:0}
D{i:19;flowX:-195;flowY:1}D{i:9;anchors_height:100;anchors_width:100}D{i:1;anchors_x:8}
D{i:3;flowX:"-207";flowY:173}D{i:4;flowX:"-191";flowY:177}D{i:5;flowX:"-191";flowY:188}
D{i:6;flowX:"-196";flowY:116}D{i:7;flowX:"-214";flowY:148}D{i:2;flowX:"-216";flowY:167}
}
##^##*/
