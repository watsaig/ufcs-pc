import QtQuick 2.0
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import org.example.ufcs 1.0 // for the Style singleton

Item {
    GridLayout {
        //flow: width > height ? GridLayout.LeftToRight : GridLayout.TopToBottom
        columns: 3
        Component.onCompleted: {
            console.log("width: " + width + "; height: " + height)
        }



        Pane {
            id: controlLayerPane
            Material.elevation: Style.card.elevation
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: Style.card.padding

            ColumnLayout {
                spacing: 30
                Label {
                    text: qsTr("Control layer")
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                }

                RowLayout {
                    spacing: 50

                    PressureController {
                        controllerNumber: 1
                    }

                    GridLayout {
                        id: controlLayerValveGrid
                        columns: flowLayerValveGrid.columns

                        ValveSwitch {
                            valveNumber: 1
                        }
                        ValveSwitch {
                            valveNumber: 2
                        }
                        ValveSwitch {
                            valveNumber: 3
                        }
                        ValveSwitch {
                            valveNumber: 4
                        }
                        ValveSwitch {
                            valveNumber: 5
                        }
                        ValveSwitch {
                            valveNumber: 6
                        }
                        ValveSwitch {
                            valveNumber: 7
                        }
                        ValveSwitch {
                            valveNumber: 8
                        }
                        ValveSwitch {
                            valveNumber: 9
                        }
                        ValveSwitch {
                            valveNumber: 10
                        }
                        ValveSwitch {
                            valveNumber: 11
                        }
                        ValveSwitch {
                            valveNumber: 12
                        }
                        ValveSwitch {
                            valveNumber: 13
                        }
                        ValveSwitch {
                            valveNumber: 14
                        }
                        ValveSwitch {
                            valveNumber: 15
                        }
                        ValveSwitch {
                            valveNumber: 16
                        }
                    }
                }
            }
        }

        Pane {
            id: flowLayerPane
            Material.elevation: Style.card.elevation
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: Style.card.padding

            ColumnLayout {
                spacing: 30

                Label {
                    text: qsTr("Flow layer")
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                }

                RowLayout {
                    spacing: 50


                    PressureController {
                        controllerNumber: 2
                    }

                    GridLayout {
                        id: flowLayerValveGrid
                        columns: 4

                        ValveSwitch {
                            valveNumber: 17
                        }
                        ValveSwitch {
                            valveNumber: 18
                        }
                        ValveSwitch {
                            valveNumber: 19
                        }
                        ValveSwitch {
                            valveNumber: 20
                        }
                        ValveSwitch {
                            valveNumber: 21
                        }
                        ValveSwitch {
                            valveNumber: 22
                        }
                        ValveSwitch {
                            valveNumber: 23
                        }
                    }
                }
            }
        }

        Pane {
            id: vacuumPane
            Material.elevation: Style.card.elevation
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: Style.card.padding

            ColumnLayout {
                spacing: 30

                Label {
                    text: qsTr("Vacuum")
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                }

                PressureController {
                    controllerNumber: 3
                }
            }
        }

        Pane {
            id: pumpPane
            Material.elevation: Style.card.elevation
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: Style.card.padding
            ColumnLayout {
                Label {
                    text: qsTr("Pumps")
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                }

                RowLayout {
                    id: rowLayout
                    width: 100
                    height: 100

                    Switch {
                        id: switch1
                        text: qsTr("Pump 1")
                        onCheckedChanged: Backend.setPump(1, checked)
                    }

                    Switch {
                        id: switch2
                        text: qsTr("Pump 2")
                        onCheckedChanged: Backend.setPump(2, checked)
                    }
                }
            }
        }

        Pane {
            id: statusPane
            Layout.fillHeight: true
            Layout.fillWidth: true
            Material.elevation: Style.card.elevation
            padding: Style.card.padding

            ColumnLayout {
                Layout.fillWidth: true

                Label {
                    text: qsTr("Status")
                    font.pointSize: Style.title.fontSize
                    padding: Style.title.padding
                }
                Label {
                    text: "ESP32 is " + Backend.connectionStatus
                    padding: 12
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
