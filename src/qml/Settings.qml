import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

import org.example.ufcs 1.0

Item {
    id: settings
    RowLayout {
        anchors.fill: parent
        anchors.margins: Style.view.margin

        ColumnLayout {
            id: settingsColumn

            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.preferredWidth: 800
            Layout.maximumWidth: 1000

            Label {
                text: qsTr("Settings")
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
            }

            RowLayout {
                id: rowLayout
                width: 100
                height: 100

                Label {
                    id: label
                    text: qsTr("Dark theme")
                    Layout.fillWidth: true
                }

                Switch {
                    id: themeSwitcher
                    text: qsTr("")
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    display: AbstractButton.TextBesideIcon
                    Layout.fillWidth: false
                    onCheckedChanged: Backend.darkMode = checked
                    Component.onCompleted: checked = Backend.darkMode
                }
            }
        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
