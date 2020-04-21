import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12

import org.example.ufcs 1.0

Item {
    id: control
    property int valveNumber

    property color normalColor: "#e60000"
    property color highlightedColor: "#d10000"

    // True if user can edit the label
    property bool editable: false

    height: 100
    width: 100

    Button {
        id: button
        text: valveNumber
        focusPolicy: Qt.ClickFocus
        autoRepeat: false
        display: AbstractButton.IconOnly // don't display the "text" property on the button
        anchors.fill: parent

        checkable: true
        checked: false

        enabled: !control.editable

        onClicked: Backend.setValve(valveNumber, checked);

            /*
        background: Rectangle {
            anchors.fill: button

            color: control.normalColor
            opacity: button.checked ? 1 : 0.5

            Ripple {
                clipRadius: 2
                width: button.width
                height: button.height
                pressed: button.pressed
                anchor: button
                active: button.down || button.visualFocus || button.hovered
                color: control.highlightedColor
            }
        }

            */
    }

    Text {
        id: element
        x: 47
        y: 12
        text: valveNumber
        anchors.horizontalCenter: control.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 13
    }

    Rectangle {
        id: textInputBackground
        anchors.fill: textInput
        color: "#ffffff"
        visible: control.editable
    }

    TextInput {
        id: textInput
        x: 12
        y: 60
        width: control.width - 6
        height: 20
        anchors.horizontalCenter: control.horizontalCenter

        selectByMouse: control.editable
        enabled: control.editable

        text: qsTr("Input label")
        verticalAlignment: Text.AlignVCenter
        autoScroll: false
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12

        onEditingFinished: {
            // Surrender focus to hide the editing cursor
            control.focus = true
        }

        onTextEdited: {
            Backend.saveValveLabel(valveNumber, text);
        }
    }



    ValveSwitchHelper {
        id: helper
        onStateChanged: button.checked = state
    }

    Component.onCompleted: {
        helper.valveNumber = valveNumber
        textInput.text = Backend.loadValveLabel(valveNumber);
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}D{i:2;flowX:"-563";flowY:"-64"}D{i:3;flowX:"-93.75";flowY:3.25}
D{i:4;flowX:"-93.75";flowY:3.25}
}
##^##*/
