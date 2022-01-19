import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import org.example.ufcs 1.0

Item {
    id: control
    property int controllerNumber
    // To do Qt6: make these properties required
    property double minPressure
    property double maxPressure
    property string unitLabel: "PSI"
    property int sliderHeight: 200
    property bool largeHandle: false

    width: grid1.implicitWidth
    height: grid1.implicitHeight
    enabled: Backend.connectionStatus == "Connected"

    property double sp : helper.setPointInPsi
    property double pv : helper.measuredValueInPsi

    GridLayout {
        id: grid1
        flow: GridLayout.TopToBottom
        rows: 2

        rowSpacing: 25
        columnSpacing: 25

        Slider {
            id: slider
            orientation: Qt.Vertical
            live: true
            stepSize: 1./256.
            background.implicitHeight: control.sliderHeight

            onMoved: {
                Backend.setPressure(controllerNumber, value);
            }
            onValueChanged: {
                helper.setPoint = value
                setPointLabel.text = helper.setPointInPsi + " " + unitLabel
            }
            Component.onCompleted: {
                if (control.largeHandle) {
                    handle.implicitHeight = 25
                    handle.implicitWidth = 25
                }
            }
        }

        Label {
            id: setPointLabel
            text: minPressure + " " + unitLabel
            Layout.maximumWidth: 45
            horizontalAlignment: Text.AlignHCenter
            //anchors.horizontalCenter: slider.horizontalCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            id: measuredValueBackground
            height: slider.background.height
            width: slider.background.width
            color: Material.foreground
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                id: measuredValueForeground
                anchors.bottom: measuredValueBackground.bottom
                height: measuredValueBackground.height * helper.measuredValue
                width: measuredValueBackground.width * 3
                color: Material.accent
                anchors.horizontalCenter: measuredValueBackground.horizontalCenter
            }
        }

        Label {
            id: measuredValueLabel
            text: helper.measuredValueInPsi + " " + unitLabel
            Layout.maximumWidth: 45
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }
    }

    PCHelper {
        id: helper
        minPressure: control.minPressure
        maxPressure: control.maxPressure

        onSetPointChanged: {
            slider.value = setPoint // this makes it possible for the C++ helper to update the slider position
        }

        onMeasuredValueChanged: {
            measuredValueLabel.text = measuredValueInPsi + " " + unitLabel
            //console.log("Updating measured value to " + measuredValueLabel.text)
        }
    }
    Component.onCompleted: {
        Backend.registerPCHelper(controllerNumber, helper)
    }
}
