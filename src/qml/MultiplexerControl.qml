import QtQuick 2.12
import QtQuick.Controls 2.12

import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    implicitHeight: button1.implicitHeight

    RowLayout {
        id: multiplexerButtons
        anchors.fill: parent

        Button {
            id: button1
            text: "1"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(1)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }

        Button {
            text: "2"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(2)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "3"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(3)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "4"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(4)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "5"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(5)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "6"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(6)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "7"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(7)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
        Button {
            text: "8"
            autoExclusive: true
            checkable: true
            enabled: Backend.connectionStatus == "Connected"
            onClicked: Backend.setMultiplexer(8)

            implicitWidth: Style.valveSwitch.defaultWidth
            implicitHeight: height
            height: width * Style.valveSwitch.widthToHeightRatio
            Layout.fillWidth: true
            Layout.maximumWidth: Style.valveSwitch.maximumWidth
        }
    }
}
