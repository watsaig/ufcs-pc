import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

import org.example.ufcs 1.0 // for the Style singleton

Item {

    property int number
    property bool resizeProportionally : true

    implicitWidth: Style.valveSwitch.defaultWidth
    implicitHeight: Style.valveSwitch.defaultWidth * Style.valveSwitch.widthToHeightRatio

    Layout.fillWidth: true
    Layout.maximumWidth: Style.valveSwitch.maximumWidth

    Button {
        anchors.fill: parent
        text: number
        autoExclusive: true
        checkable: true
        enabled: Backend.connectionStatus == "Connected"
        onClicked: Backend.setMultiplexer(number)
    }

    Component.onCompleted: {
        if (resizeProportionally) {
            height = Qt.binding(function() { return width * Style.valveSwitch.widthToHeightRatio })
            implicitHeight = Qt.binding(function() { return height })
        }
    }

}
