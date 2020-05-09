pragma Singleton
import QtQuick 2.12


QtObject {

    property QtObject heading1: QtObject {
        property int fontSize: 16
        property int padding: 15
        property int paddingLeft: 0
    }

    property QtObject heading2: QtObject {
        property int fontSize: 13
        property int padding: 10
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
        property int defaultWidth: 72
        property int defaultHeight : 58
        property double widthToHeightRatio : 0.8
        property int maximumWidth : 150
        property int minimumWidth: 50
    }

}
