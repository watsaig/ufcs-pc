import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import QtQml.StateMachine 1.0

import org.example.ufcs 1.0 // for the Style singleton

/*
  Graphical interface for the routine controller C++ backend.


  The interface is described by a finite state machine: different user actions (or backend actions)
  cause a state change, which results in different elements being displayed.

  Some of the graphical elements (such as the title and description text boxes) are re-used between different states,
  while some are used only by one specific state.


  TODO ? Ascii diagram of the state machine
*/

Item {
    ColumnLayout {
        anchors.fill: parent

        Label {
            id: title
            visible: false
            text: qsTr("Title")
            Layout.alignment: Qt.AlignHCenter

            font.pointSize: Style.title.fontSize
            padding: Style.title.padding
        }

        Label {
            id: description
            visible: false
            text: qsTr("This description field gives more information to the user about the current state of execution.")
            Layout.alignment: Qt.AlignHCenter
        }

        // add an expandable text box, to show the list of errors found in a routine

        // add a scrolling text box, to show the current step (and previous and following 1-2 steps)

        Button {
            id: browseButton
            visible: false
            text: "Browse"
            Layout.alignment: Qt.AlignHCenter
            onClicked: fileDialog.open()
        }

        ColumnLayout {
            id: runYesNoButtons
            visible: false

            Layout.alignment: Qt.AlignHCenter

            Label {
                text: "Would you still like to run the routine?"
            }

            RowLayout {

                Button {
                    id: yes
                    text: "Yes"
                    padding: 12
                }

                Button {
                    id: no
                    text: "No"
                    padding: 12
                }
            }
        }
    }

    StateMachine {
        id: stateMachine
        initialState: noFileLoaded
        running: true

        State {
            id: noFileLoaded
            onEntered: {
                console.log("Entered state 'noFileLoaded'")
                title.text = "Load routine"
                description.text = "Choose a file to run"
                title.visible = true
                description.visible = true
                browseButton.visible = true

                //fileSelectionScreen.visible = true
            }

            onExited: {
                title.text = ""
                title.visible = false
                description.text = ""
                description.visible = false
                browseButton.visible = false
            }

            SignalTransition {
                targetState: checkingRoutine
                signal: fileDialog.fileOpened
            }

        }
        /*
        State {
            id: fileReadError
            onEntered: console.log("Entered state 'fileReadError'")
        }
        */
        State {
            id: checkingRoutine
            property int nErrors

            signal noErrorsFound
            signal errorsFound

            onEntered: {
                console.log("Entered state 'checkingRoutine'")
                description.text = "Checking routine for errors..."
                description.visible = true

                if (RoutineController.verify() > 0)
                    errorsFound()
                else
                    noErrorsFound()
            }

            onExited: {
                description.text = ""
                description.visible = false
            }

            SignalTransition {
                targetState: routineLoadedSuccessfully
                signal: checkingRoutine.noErrorsFound
            }

            SignalTransition {
                targetState: routineLoadedWithErrors
                signal: checkingRoutine.errorsFound
            }

        }

        State {
            id: routineLoadedSuccessfully

            onEntered: {
                console.log("Entered state 'routineLoadedSuccessfully'")
                description.text = "Routine loaded successfully."
                description.visible = true
                // display a "run" button
            }

            onExited: {
                description.visible = false
            }
        }

        State {
            id: routineLoadedWithErrors

            onEntered: {
                console.log("Entered state 'routineLoadedWithErrors'")
                // get number of errors from backend
                title.text = RoutineController.numberOfErrors() + " errors found"
                title.visible = true
                description.text = "Would you still ike to run the routine?"
                description.visible = true
                // show the expandable errors text box
                // show the yes/no buttons

                runYesNoButtons.visible = true
            }
            onExited: {
                title.visible = false
                description.visible = false
                runYesNoButtons.visible = false
            }

            SignalTransition {
                targetState: runningRoutine
                signal: yes.clicked
            }
            SignalTransition {
                targetState: noFileLoaded
                signal: no.clicked
            }
        }

        State {
            id: runningRoutine
            onEntered: {
                console.log("Entered state 'routineRunning'")
                // get name of routine from backend
                title.text = "Running routine" // + name
                title.visible = true
                description.text = "Step " + (RoutineController.currentStep + 1) + " of " + RoutineController.numberOfSteps()
                description.visible = true
                // show the scrolling list of steps

                RoutineController.begin()
            }
            onExited: {
                title.visible = false
                description.visible = false
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectMultiple: false
        visible: false
        onAccepted: {
            if (RoutineController.loadFile(fileUrl))
                fileOpened()
            else {
                description.text = "Error. Error. Error"
                //TODO: clean this up, obviously.
            }
        }

        onRejected: {
            console.log("Canceled")
        }

        signal fileOpened ()
    }

}
