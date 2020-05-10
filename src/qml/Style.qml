pragma Singleton
import QtQuick 2.12


QtObject {

    property bool dense : Backend.denseThemeEnabled

    property QtObject heading1: QtObject {
        property int fontSize: dense ? 12 : 16
        property int padding: dense ? 5 : 15
        property int paddingLeft: 0
    }

    property QtObject heading2: QtObject {
        property int fontSize: dense ? 9 : 13
        property int padding: dense ? 4 : 10
        property int paddingLeft: 0
    }

    property QtObject card: QtObject {
        property int elevation: 1
        property int padding: 8
    }

    property QtObject view: QtObject {
        // A view of the SwipeView, aka an individual page
        property int margin: 30
    }

    property QtObject valveSwitch : QtObject {
        property int defaultWidth: dense ? 50 : 72
        property int defaultHeight : dense ? 40 : 58
        property double widthToHeightRatio : 0.8
        property int maximumWidth : dense ? 120 : 150
        property int minimumWidth: dense ? 28 : 50
    }

    property QtObject text: QtObject {
        // General text, e.g. log messages or routine steps
        property int fontSize: dense ? 8 : 10
    }

}
