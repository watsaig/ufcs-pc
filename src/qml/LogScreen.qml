import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.example.ufcs 1.0 // for the Style singleton

Item {

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 50
        anchors.leftMargin: 40
        anchors.rightMargin: anchors.leftMargin

        ListView {
            id: logMessageList
            visible: true
            width: parent.width
            height: parent.height
            model: Backend.logMessageList

            delegate: Rectangle {
                height: timestamp.contentHeight
                width: parent.width
                color: "transparent"

                // Logs are stored as a list of strings (represented in a QVariant). The elements are:
                // 0: timestamp
                // 1: message type ("debug", "warning", ...)
                // 2: the actual message text

                function messageTypeColor()
                {
                    var text = modelData[1]
                    switch(text) {
                        case "Debug":
                            return "gray"
                        case "Warning":
                            return "orange"
                        case "Info":
                            return "blue"
                        default:
                            return "black"
                    }
                }

                Row {

                    Text {
                        id: timestamp
                        font.pointSize: 10
                        color: "gray"
                        text: modelData[0] + " "
                    }

                    Text {
                        id: messageType
                        font.pointSize: 10
                        font.bold: true
                        text: modelData[1] + ": "
                        color: messageTypeColor()
                    }

                    Text {
                        id: messageText
                        font.pointSize: 10
                        text: modelData[2]
                    }

                }
            }

            onCountChanged: positionViewAtEnd()
        }
    }

}
