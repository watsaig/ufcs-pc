import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import org.example.ufcs 1.0

Item {
    property int controllerNumber
    width: grid1.width
    height: grid1.height

    GridLayout {
        id: grid1
        flow: GridLayout.TopToBottom
        rows: 2

        rowSpacing: 25
        columnSpacing: 50

        Slider {
            id: slider
            orientation: Qt.Vertical
            live: true

            onValueChanged: {
                helper.setPoint = value // putting this here means that setValue() is called when the slider changes
                setPointLabel.text = helper.setPointInPsi + " PSI"
            }
        }

        Label {
            id: setPointLabel
            text: "0 PSI"
            Layout.maximumWidth: 45
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: slider.horizontalCenter
        }

        Rectangle {
            id: measuredValueBackground
            height: slider.background.height
            width: slider.background.width
            color: "#000000"

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
            text: "0 PSI"
            Layout.maximumWidth: 45
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: measuredValueBackground.horizontalCenter
        }
    }

    PCHelper {
        id: helper

        onSetPointChanged: {
            slider.value = setPoint // this makes it possible for the C++ helper to update the slider position
        }

        onMeasuredValueChanged: {
            measuredValueLabel.text = measuredValueInPsi + " PSI"
        }
    }
    Component.onCompleted: {
        helper.controllerNumber = controllerNumber
    }
}
