import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.0

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
                font.pointSize: Style.heading1.fontSize
                padding: Style.heading1.padding
                leftPadding: Style.heading1.paddingLeft
            }

            RowLayout {
                SettingsLabel {
                    Layout.fillWidth: true
                    primaryText: "Dark theme"
                }

                Switch {
                    id: themeSwitcher
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    onCheckedChanged: Backend.darkMode = checked
                    Component.onCompleted: checked = Backend.darkMode
                }
            }

            RowLayout {
                SettingsLabel {
                    Layout.fillWidth: true
                    primaryText: "Show graphical control screen"
                    secondaryText: "Requires restart"
                }

                Switch {
                    id: showGraphicalControlSwitch
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    onCheckedChanged: Backend.graphicalControlEnabled = checked
                    Component.onCompleted: checked = Backend.graphicalControlEnabled
                }
            }

            RowLayout {
                id: rowLayout
                width: 100
                height: 100
                visible: showGraphicalControlSwitch.checked

                SettingsLabel {
                    Layout.fillWidth: true
                    primaryText: "Graphical control layout"
                }

                ComboBox {
                    id: graphicalControlComboBox
                    model: Backend.graphicalControlScreenLabels()
                    onActivated: Backend.setCurrentGraphicalControlScreen(currentValue)
                    Component.onCompleted: currentIndex = indexOfValue(Backend.currentGraphicalControlScreenLabel())
                    Layout.preferredWidth: width

                    // Below: resize combo box to its contents
                    TextMetrics { id: textMetrics }
                    property int modelWidth

                    width: modelWidth + leftPadding + rightPadding + implicitIndicatorWidth

                    onModelChanged: {
                        textMetrics.font = graphicalControlComboBox.font
                        for(var i = 0; i < model.length; i++){
                            textMetrics.text = model[i]
                            modelWidth = Math.max(textMetrics.width, modelWidth)
                        }
                    }
                }
            }

            RowLayout {
                visible: !Backend.bluetoothEnabled

                SettingsLabel {
                    Layout.fillWidth: true
                    primaryText: "Serial communication baud rate"
                    secondaryText: "Must match the microcontroller's baud rate"
                }

                ComboBox {
                    model: [38400, 57600, 115200]
                    onActivated: Backend.baudRate = currentValue
                    Component.onCompleted: currentIndex = indexOfValue(Backend.baudRate)
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
