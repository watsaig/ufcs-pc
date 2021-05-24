import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.12

import QtQml.StateMachine 1.0 as DSM

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
        anchors.margins: Style.view.margin
        anchors.topMargin: 50

        Column {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.fillWidth: true
            Label {
                id: title
                visible: true
                text: qsTr("Title")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: Style.heading1.fontSize

                anchors.left: parent.left
                anchors.right: parent.right
                padding: Style.heading1.padding
                leftPadding: 0
                rightPadding: 0
            }

            Label {
                id: description
                visible: true
                text: qsTr("This description field gives more information to the user about the current state of execution.")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignHCenter

                anchors.left: parent.left
                anchors.right: parent.right
            }
        }

        Switch {
            id: runForeverSwitch
            visible: false
            text: qsTr("Run continuously")
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            id: stepCounter
            visible: false
            Layout.alignment: Qt.AlignHCenter

            text : "Step " +  (RoutineController.currentStep + 1 ) + " of " + RoutineController.numberOfSteps()
        }

        Label {
            id: runTime
            visible: false
            Layout.alignment: Qt.AlignHCenter
            text: "Estimated run time: " + formatTime(RoutineController.totalRunTime);
            function formatTime(seconds) {
                var hours = Math.floor(seconds/3600);
                var minutes = Math.floor((seconds%3600)/60);
                var modulo = seconds%60;
                if (hours !== 0)
                    return hours + "h " + minutes + "min " + modulo + "s";
                if (minutes !== 0)
                    return minutes + "min " + modulo + "s";
                else
                    return seconds + "s";
            }
        }


        Rectangle {
            id: listViewBackground
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 10
            color: Material.dialogColor
            visible: false

            ListView {
                id: stepsList
                visible: false
                anchors.fill: parent
                anchors.margins: 20
                model: RoutineController.stepsList
                currentIndex: RoutineController.currentStep

                delegate: Text {
                    id: delegateText
                    text: modelData

                    font.pointSize: Style.text.fontSize
                    font.bold: RoutineController.currentStep == index
                    color: RoutineController.currentStep == index ? Material.foreground : "gray"

                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                ScrollBar.vertical: ScrollBar {}
            }

            ListView {
                id: errorList
                visible: false
                anchors.fill: parent
                anchors.margins: 20

                model: RoutineController.errorList
                delegate: Text {
                    id: errorText
                    text: modelData
                    bottomPadding: 3

                    font.pointSize: Style.text.fontSize
                    color: Material.foreground

                    wrapMode: Text.WordWrap
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                /*
                Connections {
                    target: RoutineController
                    onError: {
                        errorList.model = RoutineController.errorList
                        console.log("New error received")
                    }
                }
                */
            }
        }

        Button {
            id: browseButton
            visible: false
            text: "Browse"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            onClicked: fileDialog.open()
        }

        Row {
            id: stopAndPauseButtons
            visible: false
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                id: pauseButton
                text: "Pause routine"
            }
            Button {
                id: resumeButton
                text: "Resume"
                visible: false
            }
            Button {
                id: stopButton
                text: "Stop routine"
            }

        }

        Row {
            id: yesNoButtons
            visible: false
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                id: yesButton
                text: "Yes"
            }

            Button {
                id: noButton
                text: "No"
            }
        }
}

DSM.StateMachine {
    id: stateMachine
    initialState: noFileLoaded
    running: true

    DSM.State {
        id: noFileLoaded
        onEntered: {
            console.log("Routine UI: Entered state 'noFileLoaded'")
            title.text = "Load routine"
            description.text = "Choose a file to run"
            browseButton.visible = true
        }

        onExited: {
            browseButton.visible = false
        }

        DSM.SignalTransition {
            targetState: checkingRoutine
            signal: fileDialog.fileOpened
        }

    }

    DSM.State {
        id: checkingRoutine
        property int nErrors

        signal noErrorsFound
        signal errorsFound

        onEntered: {
            console.log("Routine UI: Entered state 'checkingRoutine'")
            description.text = "Checking routine for errors..."

            if (RoutineController.verify() > 0)
                errorsFound()
            else
                noErrorsFound()
        }

        DSM.SignalTransition {
            targetState: routineLoadedSuccessfully
            signal: checkingRoutine.noErrorsFound
        }

        DSM.SignalTransition {
            targetState: routineLoadedWithErrors
            signal: checkingRoutine.errorsFound
        }

    }

    DSM.State {
        id: routineLoadedSuccessfully

        onEntered: {
            console.log("Routine UI: Entered state 'routineLoadedSuccessfully'")
            title.text = "Routine loaded"
            description.text = "The routine was loaded successfully. Click below to launch it."
            runTime.visible = true
            listViewBackground.visible = true
            stepsList.visible = true

            yesNoButtons.visible = true
            yesButton.text = "Run"
            noButton.text = "Cancel"
        }

        onExited: {
            runTime.visible = false
            listViewBackground.visible = false
            stepsList.visible = false
            yesNoButtons.visible = false
        }

        DSM.SignalTransition {
            targetState: beginRoutine
            signal: yesButton.clicked
        }

        DSM.SignalTransition {
            targetState: noFileLoaded
            signal: noButton.clicked
        }
    }

    DSM.State {
        id: routineLoadedWithErrors

        onEntered: {
            console.log("Routine UI: Entered state 'routineLoadedWithErrors'")

            title.text = RoutineController.numberOfErrors() + " errors found"
            description.text = "The routine was loaded, but some errors were detected:"

            listViewBackground.visible = true
            errorList.visible = true
            yesNoButtons.visible = true
            yesButton.text = "Run routine anyway"
            noButton.text = "Cancel"
        }
        onExited: {
            listViewBackground.visible = false
            errorList.visible = false
            yesNoButtons.visible = false
        }

        DSM.SignalTransition {
            targetState: beginRoutine
            signal: yesButton.clicked
        }
        DSM.SignalTransition {
            targetState: noFileLoaded
            signal: noButton.clicked
        }
    }

    DSM.State {
        id: beginRoutine
        // This state simply starts the routine then switches to "runningRoutine"
        signal routineStarted
        onEntered: {
            console.log("Routine UI: Entered state 'beginRoutine'")

            RoutineController.begin()
            routineStarted();
        }

        DSM.SignalTransition {
            targetState: runningRoutine
            signal: beginRoutine.routineStarted
        }
    }

    DSM.State {
        // The routine can be running normally, or running while
        // waiting to either pause or stop at the end of the current step
        id: runningRoutine
        initialState: activelyRunning


        DSM.SignalTransition {
            targetState: finishedRunning
            signal: RoutineController.finished
        }

        DSM.SignalTransition {
            targetState: routinePaused
            signal: RoutineController.paused
        }

        DSM.SignalTransition {
            targetState: stopRequested
            signal: stopButton.clicked
        }

        DSM.SignalTransition {
            targetState: pauseRequested
            signal: pauseButton.clicked
        }

        onEntered: {
            stepCounter.visible = true
            listViewBackground.visible = true
            stepsList.visible = true
            runForeverSwitch.visible = true
            stopAndPauseButtons.visible = true
        }

        onExited: {
            stepCounter.visible = false
            runForeverSwitch.visible = false
            stopAndPauseButtons.visible = false
            stopAndPauseButtons.enabled = true
        }

        DSM.State {
            id: activelyRunning
            onEntered: {
                console.log("Routine UI: Entered state 'activelyRunning'")
                title.text = "Running routine"
                description.text = RoutineController.routineName()
                stepsList.positionViewAtBeginning()
            }
        }

        DSM.State {
            id: stopRequested
            onEntered: {
                console.log("Routine UI: stop requested")
                title.text = "Stop requested"
                description.text = "Routine will end after current step"
                runForeverSwitch.checked = false
                runForeverSwitch.visible = false
                stopAndPauseButtons.enabled = false

                // RoutineController then emits finished signal after the current step
                // to transition to next state (this may take some time)
                RoutineController.stop()
            }
        }

        DSM.State {
            id: pauseRequested
            onEntered: {
                console.log("Routine UI: pause requested")
                title.text = "Pause requested"
                description.text = "Routine will pause after the current step"
                RoutineController.pause()
                stopAndPauseButtons.enabled = false
            }
        }
    }

    DSM.State {
        id: routinePaused
        onEntered: {
            console.log("Routine UI: Entered state 'routinePaused'");
            title.text = "Routine paused"
            description.text = "Routine execution paused by user"
            stopAndPauseButtons.visible = true
            pauseButton.visible = false
            resumeButton.visible = true
        }

        Connections {
            target: resumeButton
            onClicked: {
                console.log("Routine UI: resume requested")
                RoutineController.resume()
                resumeButton.visible = false
            }
        }

        DSM.SignalTransition {
            targetState: stopRequested
            signal: stopButton.clicked
        }

        DSM.SignalTransition {
            targetState: runningRoutine
            signal: RoutineController.resumed
        }

    }

    DSM.State {
        id: finishedRunning
        signal restartRoutine

        onEntered: {
            if (runForeverSwitch.checked) {
                console.log("Routine restarting automatically")
                restartRoutine()
            }

            console.log("Routine UI: Entered state 'finishedRunning'")
            title.text = "Finished"
            description.text = "The execution of the routine has ended."
            yesNoButtons.visible = true
            yesButton.text = "Re-run"
            noButton.text = "Done"
        }

        onExited: {
            yesNoButtons.visible = false
            stepsList.visible = false
        }

        DSM.SignalTransition {
            targetState: noFileLoaded
            signal: noButton.clicked
        }

        DSM.SignalTransition {
            targetState: beginRoutine
            signal: yesButton.clicked
        }

        DSM.SignalTransition {
            targetState: beginRoutine
            signal: finishedRunning.restartRoutine
        }
    }
}

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: Backend.routineFolder()
        selectMultiple: false
        visible: false
        onAccepted: {
            Backend.setRoutineFolder(folder)
            if (RoutineController.loadFile(fileUrl))
                fileOpened()
            else {
                description.text = "The selected file could not be opened. Check that you have read permissions on the file, and try again."
            }
        }

        signal fileOpened ()
    }

}
