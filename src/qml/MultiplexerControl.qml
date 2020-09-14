import QtQuick 2.12
import QtQuick.Controls 2.12

import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    /*
       MultiplexerControl handles both the display and logic of a multiplexer.

       A microfluidic multiplexer consists of a number of control valves that control
       flow through a larger number of flow channels. For example, 6 control lines
       can direct flow into 8 channels.

       In the schematic below, one input flow channel is split into 8 channels. 6 control
       lines are used to direct flow down each of the branches: valves are represented by A-F,
       where each letter is a separate control line.

                                           E--1
                                        C--|
                                        |  F--2
                                     A--|
                                     |  |  E--3
                                     |  D--|
                                     |     F--4
                          Input -> --|
                                     |     E--5
                                     |  C--|
                                     |  |  F--6
                                     B--|
                                        |  E--7
                                        D--|
                                           F--8

       Here, by pressurizing B, D and F while laving A, C and E unpressurized, fluids will flow
       down to output #1.
       If we represent the states of A-F with 0 (unpressurized) and 1 (pressurized), we can sum up
       the state of the multiplexer as an array. For example, to flow through to output #1, the
       state would be represented as [010101].
       Similarly, to flow to output #2, the state would be [010110].

       It is also possible to flow to several channels at once: [000000] would mean that all valves
       are unpressurized, and fluids flow to all 8 outputs at once.
       [000010] would open 2, 4, 6 and 8. And so on.

       In practice, this is represented as a string. While this isn't efficient, it does make for
       a concise definition. See muxModel below for an example definition. This property should
       be redefined to match whatever setup your particular chip uses.

       Likewise, the list of valves should be set based on your setup.

       For each entry in the muxModel list, a button is created with the given label. When clicked,
       it sets the valves (given in the `valves` property) to the state specified by `config`, where
       0 is closed and 1 is open.

       Only one button can be active at a time.

       These buttons can be clicked by the user, or activated by the routine controller backend.
       However, they are not updated if individual valves of the multiplexer are toggled. For example,
       if the multiplexer uses valves 10 through 18, and valve #16 is toggled via the manual control
       screen or elsewhere, then the buttons on this screen will not update.

       The buttons in MultiplexerControl can have either a simple, pre-defined label, or a label along
       with a second, user-editable label (used primarily for fluidic inputs to the chip). In the former
       case, muxDelegate manages the buttons. In the latter case, muxDelegateLabeled manages them.
    */
    id: muxControl

    /// The control valves associated with the multiplexer
    property var valves : [13, 14, 15, 16, 17, 18]

    /// The list of possible states, with a label and configuration
    property ListModel muxModel: ListModel {
        ListElement { label: "1"; config: "010101" }
        ListElement { label: "2"; config: "010110" }
        ListElement { label: "3"; config: "011001" }
        ListElement { label: "4"; config: "011010" }
        ListElement { label: "5"; config: "100101" }
        ListElement { label: "6"; config: "100110" }
        ListElement { label: "7"; config: "101001" }
        ListElement { label: "8"; config: "101010" }
        ListElement { label: "All"; config: "000000" }
        ListElement { label: "None"; config: "111111" }
    }

    property int columns: 8
    property bool labeledSwitches: false

    property string currentLabel: ""

    implicitHeight: muxGridView.implicitHeight

    Component {
        id: muxDelegate
        Button {
            checkable: true
            autoExclusive: true
            enabled: Backend.connectionStatus == "Connected"
            text: label
            font.capitalization: Font.MixedCase
            onClicked: setMuxToConfig(config)

            width: muxGridView.cellWidth - 6
            height: muxGridView.cellHeight

            Component.onCompleted: {
                if (config.length !== valves.length)
                    console.error("Multiplexer channel configuration for channel '" + label + "' does not match number of valves defined")
            }

            Connections {
                target: muxControl
                onCurrentLabelChanged: {
                    // Slightly hacky way to get the labels to update when setMuxToLabel is called
                    if (muxControl.currentLabel.toUpperCase() === label.toUpperCase())
                        checked = true
                }
            }
        }

    }

    ButtonGroup {
        id: labeledButtonGroup
        property LabeledValveSwitch lastButtonChecked: null
    }

    Component {
        id: muxDelegateLabeled
        LabeledValveSwitch {
            id: lvs
            valveNumber: parseInt(label)
            width: muxGridView.cellWidth - 6
            height: muxGridView.cellHeight
            editable: editingMode
            registerWithBackend: false
            buttonGroup: labeledButtonGroup
            onClicked: {
                if (labeledButtonGroup.lastButtonChecked === this) {
                    labeledButtonGroup.checkState = Qt.Unchecked
                    labeledButtonGroup.lastButtonChecked = null
                    closeAllValves()
                }
                else {
                    setMuxToConfig(config)
                    labeledButtonGroup.lastButtonChecked = this
                }
            }
            Component.onCompleted: {
                if (config.length !== valves.length)
                    console.error("Multiplexer channel configuration for channel '" + label + "' does not match number of valves defined")
                if (isNaN(parseInt(label)))
                    console.error("Label for multiplexer channel '" + label + "' must be an integer")
            }
            Connections {
                target: muxControl
                onCurrentLabelChanged: {
                    if (muxControl.currentLabel.toUpperCase() === label.toUpperCase()) {
                        setChecked(true)
                        labeledButtonGroup.lastButtonChecked = lvs
                    }
                }
            }
        }
    }

    GridView {
        id: muxGridView
        model: muxModel
        delegate: labeledSwitches ? muxDelegateLabeled : muxDelegate

        anchors.fill: parent
        cellWidth: width/muxControl.columns
        cellHeight: labeledSwitches ? Style.labeledValveSwitch.defaultHeight : Style.valveSwitch.defaultHeight
        interactive: false

        implicitHeight: cellHeight * (Math.ceil(count/muxControl.columns))

    }

    function closeAllValves() {
        setMuxToConfig("1".repeat(valves.length))
    }

    function setMuxToConfig(configuration) {
        console.log("Setting multiplexer to config: " + configuration)
        for (var i = 0; i < configuration.length; i++) {
            Backend.setValve(valves[i], parseInt(configuration[i]))
        }
    }

    function setMuxToLabel(label) {
        console.log("Setting multiplexer to label: " + label)
        var found = false
        for (var i = 0; i < muxModel.count; i++) {
            if (muxModel.get(i).label.toUpperCase() === label.toUpperCase()) {
                currentLabel = label
                setMuxToConfig(muxModel.get(i).config)
                found = true
                break
            }
        }
        if (!found) {
            if (label.toUpperCase() === "NONE") {
                console.log("Closing all multiplexer channels")
                closeAllValves()
            }
            else
                console.warn("No multiplexer channel found with label: " + label)
        }
    }

}
