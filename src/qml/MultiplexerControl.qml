import QtQuick 2.0
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import org.example.ufcs 1.0 // for the Style singleton

Item {
        Layout.preferredHeight: button1.height
        RowLayout {
            id: multiplexerButtons

            Button {
                id: button1
                text: "1"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(1)
            }

            Button {
                text: "2"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(2)
            }
            Button {
                text: "3"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(3)
            }
            Button {
                text: "4"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(4)
            }
            Button {
                text: "5"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(5)
            }
            Button {
                text: "6"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(6)
            }
            Button {
                text: "7"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(7)
            }
            Button {
                text: "8"
                autoExclusive: true
                checkable: true
                onClicked: multiplexer(8)
            }
        }

   function multiplexer(n) {
        switch (n) {
            case 1:
                Backend.setValve(17, false)
                Backend.setValve(16, true)
                Backend.setValve(15, false)
                Backend.setValve(14, true)
                Backend.setValve(13, false)
                Backend.setValve(12, true)
                break;
            case 2:
                Backend.setValve(17, false)
                Backend.setValve(16, true)
                Backend.setValve(15, false)
                Backend.setValve(14, true)
                Backend.setValve(13, true)
                Backend.setValve(12, false)
                break;
            case 3:
                Backend.setValve(17, false)
                Backend.setValve(16, true)
                Backend.setValve(15, true)
                Backend.setValve(14, false)
                Backend.setValve(13, false)
                Backend.setValve(12, true)
                break;
            case 4:
                Backend.setValve(17, false)
                Backend.setValve(16, true)
                Backend.setValve(15, true)
                Backend.setValve(14, false)
                Backend.setValve(13, true)
                Backend.setValve(12, false)
                break;
            case 5:
                Backend.setValve(17, true)
                Backend.setValve(16, false)
                Backend.setValve(15, false)
                Backend.setValve(14, true)
                Backend.setValve(13, false)
                Backend.setValve(12, true)
                break;
            case 6:
                Backend.setValve(17, true)
                Backend.setValve(16, false)
                Backend.setValve(15, false)
                Backend.setValve(14, true)
                Backend.setValve(13, true)
                Backend.setValve(12, false)
                break;
            case 7:
                Backend.setValve(17, true)
                Backend.setValve(16, false)
                Backend.setValve(15, true)
                Backend.setValve(14, false)
                Backend.setValve(13, false)
                Backend.setValve(12, true)
                break;
            case 8:
                Backend.setValve(17, true)
                Backend.setValve(16, false)
                Backend.setValve(15, true)
                Backend.setValve(14, false)
                Backend.setValve(13, true)
                Backend.setValve(12, false)
                break;

            default:
                break;
        }
    }
}
