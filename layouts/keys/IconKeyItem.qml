import QtQuick 2.0

KeyItem {
    property url icon
    property real iconMargins: 4

    Image {
        anchors.fill: parent
        anchors.margins: iconMargins
        source: icon
    }
}
