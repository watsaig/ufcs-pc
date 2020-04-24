import QtQuick 2.12
import QtQuick.Controls 2.12

import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import org.example.ufcs 1.0 // for the Style singleton

Item {
    implicitHeight: button1.implicitHeight

    RowLayout {
        id: multiplexerButtons
        anchors.fill: parent

        MultiplexerButton { id: button1; number: 1}
        MultiplexerButton { number: 2}
        MultiplexerButton { number: 3}
        MultiplexerButton { number: 4}
        MultiplexerButton { number: 5}
        MultiplexerButton { number: 6}
        MultiplexerButton { number: 7}
        MultiplexerButton { number: 8}
    }
}
