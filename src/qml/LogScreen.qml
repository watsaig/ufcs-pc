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
                height: 20
                width: 100
                Text { text: modelData }
            }

            onCountChanged: positionViewAtEnd()
        }
    }

}
