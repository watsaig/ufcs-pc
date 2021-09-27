import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Item {
    // A simple 2-row label, comprised of a main label with an optional
    // explanatory label underneath it.

    // To do if switching to Qt 6: make primaryText a required property
    property var primaryText
    property var secondaryText

    implicitWidth: column.implicitWidth
    implicitHeight: column.implicitHeight

    Column{
        id: column

        Label {
            text: primaryText
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            text: secondaryText ? secondaryText : ""
            color: Material.hintTextColor
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            visible: secondaryText !== undefined
        }
    }
}
