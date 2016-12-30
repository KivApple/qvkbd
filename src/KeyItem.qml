import QtQuick 2.0

Rectangle {
    property int scanCode: -1
    property bool modifier: false
    property string label: ""

    property bool dynamicLabel: false
    property bool checked: false

    signal xPressed(int scanCode)
    signal xReleased(int scanCode)

    width: 50
    height: 50

    border.width: 1
    border.color: "darkgray"
    color: checked ? "red" : "white"

    Text {
        anchors.fill: parent
        text: label
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            if (scanCode < 0) {
                checked = true;
            }
            if (modifier) {
                if (!checked) {
                    xPressed(scanCode);
                } else {
                    xReleased(scanCode);
                }
            } else {
                xPressed(scanCode);
            }
        }
        onReleased: {
            if (scanCode < 0) {
                checked = false;
            }
            if (!modifier) {
                xReleased(scanCode);
            }
        }
    }

}
