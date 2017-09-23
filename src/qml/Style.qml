pragma Singleton
import QtQuick 2.0


QtObject {

    property QtObject title: QtObject {
        property int fontSize: 24
        property int padding: 8
    }

    property QtObject card: QtObject {
        property int elevation: 1
        property int padding: 8
    }

}
