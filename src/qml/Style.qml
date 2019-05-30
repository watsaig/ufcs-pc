pragma Singleton
import QtQuick 2.0


QtObject {

    property QtObject title: QtObject {
        property int fontSize: 18
        property int padding: 15
        property int paddingLeft: 0
    }

    property QtObject card: QtObject {
        property int elevation: 1
        property int padding: 8
    }

}
