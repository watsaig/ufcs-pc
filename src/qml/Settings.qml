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
                font.pointSize: Style.title.fontSize
                padding: Style.title.padding
                leftPadding: Style.title.paddingLeft
            }

            RowLayout {
                Label {
                    text: qsTr("Dark theme")
                    Layout.fillWidth: true
                }

                Switch {
                    id: themeSwitcher
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    onCheckedChanged: Backend.darkMode = checked
                    Component.onCompleted: checked = Backend.darkMode
                }
            }

            RowLayout {
                Label {
                    text: qsTr("Show graphical control screen (requires restart)")
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                Switch {
                    id: showGraphicalControlSwitch
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    onCheckedChanged: Backend.showGraphicalControl = checked
                    Component.onCompleted: checked = Backend.showGraphicalControl
                }
            }

            RowLayout {
                id: rowLayout
                width: 100
                height: 100
                visible: showGraphicalControlSwitch.checked

                Label {
                    text: qsTr("Graphical control layout")
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
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
                visible: !Backend.useBluetooth
                ColumnLayout {
                    Label {
                        text: qsTr("Serial communication baud rate (requires restart)")
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                    Label {
                        text: qsTr("Must match the microcontroller's baud rate")
                        wrapMode: Text.WordWrap
                        font.pointSize: 10
                        color: Material.hintTextColor
                    }
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
