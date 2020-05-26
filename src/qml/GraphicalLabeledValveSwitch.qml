import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import org.example.ufcs 1.0

Item {
    /*
      GraphicalLabeledValveSwitch is similar to a ValveSwitch, but includes a user-editable
      text label in addition to a valve number.

      This is especially useful for valves that control fluidic inputs, since different fluids
      may be connected by the user.

      By default, the switch functions just like a ValveSwitch: it is registered with the backend
      so that it can be updated when the microcontroller signals that the corresponding physical valve
      has changed state, as well as to signal to the backend to toggle that valve when the switch is
      clicked.

      However, it is possible to customize the function of the GraphicalLabeledValveSwitch: just set the
      'registerWithBackend' property to false, and define onClicked to do whatever you want.
      For example, this is used by MultiplexerControl to offer a multiplexer control switch with an
      editable label.
    */

    id: control
    property int valveNumber
    property string text: qsTr("Input label")

    // If true, the valve switch is registered with the backend, toggling the valve
    // that corresponds to valveNumber when clicked. Set to false to override the switch behavior.
    property bool registerWithBackend: true
    property bool autoExclusive: false
    // True if user can edit the label
    property bool editable: false

    implicitHeight: Style.labeledValveSwitch.defaultHeight
    implicitWidth: Style.labeledValveSwitch.defaultWidth
    Layout.fillWidth: true
    property bool dense : Backend.denseThemeEnabled

    signal clicked
    onClicked: registerWithBackend ? Backend.setValve(valveNumber, button.checked) : NULL

    Button {
        id: button
        text: valveNumber
        focusPolicy: Qt.ClickFocus
        autoRepeat: false
        display: AbstractButton.IconOnly // don't display the "text" property on the button
        anchors.fill: parent

        checkable: true
        checked: false
        autoExclusive: control.autoExclusive

        enabled: !control.editable && Backend.connectionStatus == "Connected"
        onClicked: control.clicked()
    }

    Text {
        id: element
        text: valveNumber

        anchors.top: control.top
        anchors.topMargin: dense ? 10 : 12
        anchors.horizontalCenter: control.horizontalCenter

        horizontalAlignment: Text.AlignHCenter
        font.pointSize: dense ? 6 : 8
        color: button.enabled ? Material.primaryTextColor : Material.hintTextColor
    }

    Rectangle {
        id: textInputBackground
        anchors.fill: textInput
        color: Material.backgroundColor
        visible: control.editable
    }

    TextInput {
        id: textInput
        width: control.width - 6
        height: 20

        anchors.horizontalCenter: control.horizontalCenter
        anchors.bottom: control.bottom
        anchors.bottomMargin: 12
        text: control.text

        selectByMouse: control.editable
        enabled: control.editable

        font.pointSize: dense ? 6 : 8
        autoScroll: false
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: button.enabled ? Material.primaryTextColor : Material.hintTextColor

        // To do: a better implementation of this where the max. width is set
        maximumLength: Math.max(12, width / fontMetrics.averageCharacterWidth)

        FontMetrics {
            id: fontMetrics
            font: textInput.font
        }

        onEditingFinished: {
            // Surrender focus to hide the editing cursor
            control.focus = true
        }

        onTextEdited: {
            Backend.setValveLabel(valveNumber, text);
        }

    }

    ValveSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: {
        if (registerWithBackend)
            Backend.registerValveSwitchHelper(valveNumber, helper)

        control.text = Backend.valveLabel(valveNumber);
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}D{i:2;flowX:"-563";flowY:"-64"}D{i:3;flowX:"-93.75";flowY:3.25}
D{i:4;flowX:"-93.75";flowY:3.25}
}
##^##*/
