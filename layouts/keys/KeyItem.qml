/*
 * This file is part of the qvkbd project.
 * Copyright (C) 2016 Ivan Kolesnikov <kiv.apple@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
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
