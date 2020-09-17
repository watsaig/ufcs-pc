import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: element
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Rectangle {
            id: rectangle
            implicitHeight: 200
            color: "#e24444"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        }

        Column {
            id: column
            Layout.fillWidth: true

            Rectangle {
                id: rectangle1
                height: 200
                color: "#2acd46"
                anchors.right: parent.right
                anchors.left: parent.left
            }

            Rectangle {
                id: rectangle2
                height: 200
                color: "#4a8df9"
                anchors.right: parent.right
                anchors.left: parent.left

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    Connections {
                        target: mouseArea
                        onClicked: { element.state = (element.state == "expanded" ? "" : "expanded") }
                    }
                }
            }
        }
    }
    states: [
        State {
            name: "expanded"

            PropertyChanges {
                target: rectangle2
                height: 500
            }
        }
    ]

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_width:200}D{i:6;anchors_height:100;anchors_width:100}
D{i:5;anchors_width:200}D{i:1;anchors_height:100;anchors_width:100}
}
##^##*/
