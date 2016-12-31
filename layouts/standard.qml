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
import "keys"

Rectangle {
    width: childrenRect.width + 20
    height: childrenRect.height + 20
    color: "#00000000"
    border.width: 2

    Column {
        id: column
        x: 10
        y: 10
        width: childrenRect.width
        height: childrenRect.height
        spacing: 4

        Row {
            id: row
            width: childrenRect.width
            height: 30
            spacing: 4

            KeyItem {
                id: keyItem
                width: 40
                height: 30
                color: "#ffffff"
                scanCode: 9
                label: "Esc"
            }

            Item {
                id: spacer
                width: 20
                height: 30
            }

            KeyItem {
                id: keyItem1
                width: 40
                height: 30
                scanCode: 67
                label: "F1"
            }

            KeyItem {
                id: keyItem2
                width: 40
                height: 30
                scanCode: 68
                label: "F2"
            }

            KeyItem {
                id: keyItem3
                width: 40
                height: 30
                scanCode: 69
                label: "F3"
            }

            KeyItem {
                id: keyItem4
                width: 40
                height: 30
                scanCode: 70
                label: "F4"
            }

            Item {
                id: spacer2
                width: 6
                height: 30
            }

            KeyItem {
                id: keyItem5
                width: 40
                height: 30
                scanCode: 71
                label: "F5"
            }

            KeyItem {
                id: keyItem6
                width: 40
                height: 30
                scanCode: 72
                label: "F6"
            }

            KeyItem {
                id: keyItem7
                width: 40
                height: 30
                scanCode: 73
                label: "F7"
            }

            KeyItem {
                id: keyItem8
                width: 40
                height: 30
                scanCode: 74
                label: "F8"
            }

            Item {
                id: spacer3
                width: 6
                height: 30
            }

            KeyItem {
                id: keyItem9
                width: 40
                height: 30
                scanCode: 75
                label: "F9"
            }

            KeyItem {
                id: keyItem10
                width: 40
                height: 30
                scanCode: 76
                label: "F10"
            }

            KeyItem {
                id: keyItem11
                width: 40
                height: 30
                scanCode: 77
                label: "F11"
            }

            KeyItem {
                id: keyItem12
                width: 40
                height: 30
                scanCode: 78
                label: "F12"
            }

            Item {
                id: spacer5
                width: 22
                height: 30
            }

            KeyItem {
                id: keyItem27
                width: 30
                height: 30
                scanCode: -2
                label: "X"
            }

        }

        Item {
            id: spacer4
            width: parent.width
            height: 10
        }

        Row {
            id: row1
            width: childrenRect.width
            height: 40
            spacing: 4

            KeyItem {
                id: keyItem13
                x: 0
                width: 40
                height: 40
                scanCode: 49
            }

            KeyItem {
                id: keyItem14
                x: 0
                width: 40
                height: 40
                color: "#ffffff"
                scanCode: 10
            }

            KeyItem {
                id: keyItem15
                x: 0
                width: 40
                height: 40
                scanCode: 11
            }

            KeyItem {
                id: keyItem16
                x: 0
                width: 40
                height: 40
                scanCode: 12
            }

            KeyItem {
                id: keyItem17
                x: 0
                width: 40
                height: 40
                scanCode: 13
            }

            KeyItem {
                id: keyItem18
                x: 0
                width: 40
                height: 40
                scanCode: 14
            }

            KeyItem {
                id: keyItem19
                x: 0
                width: 40
                height: 40
                scanCode: 15
            }

            KeyItem {
                id: keyItem20
                x: 0
                width: 40
                height: 40
                scanCode: 16
            }

            KeyItem {
                id: keyItem21
                x: 0
                width: 40
                height: 40
                scanCode: 17
            }

            KeyItem {
                id: keyItem22
                x: 0
                width: 40
                height: 40
                scanCode: 18
            }

            KeyItem {
                id: keyItem23
                x: 0
                width: 40
                height: 40
                scanCode: 19
            }

            KeyItem {
                id: keyItem24
                x: 0
                width: 40
                height: 40
                scanCode: 20
            }

            KeyItem {
                id: keyItem25
                x: 0
                width: 40
                height: 40
                scanCode: 21
            }

            KeyItem {
                id: keyItem26
                x: 0
                width: 100
                height: 40
                scanCode: 22
                label: "⤌"
            }

        }

        Row {
            id: row2
            width: childrenRect.width
            height: 40
            spacing: 4

            KeyItem {
                id: keyItem28
                x: 0
                width: 60
                height: 40
                scanCode: 23
                label: "Tab"
            }

            KeyItem {
                id: keyItem29
                x: 0
                width: 40
                height: 40
                scanCode: 24
            }

            KeyItem {
                id: keyItem30
                x: 0
                width: 40
                height: 40
                scanCode: 25
            }

            KeyItem {
                id: keyItem31
                x: 0
                width: 40
                height: 40
                scanCode: 26
            }

            KeyItem {
                id: keyItem32
                x: 0
                width: 40
                height: 40
                scanCode: 27
            }

            KeyItem {
                id: keyItem33
                x: 0
                width: 40
                height: 40
                scanCode: 28
            }

            KeyItem {
                id: keyItem34
                x: 0
                width: 40
                height: 40
                scanCode: 29
            }

            KeyItem {
                id: keyItem35
                x: 0
                width: 40
                height: 40
                scanCode: 30
            }

            KeyItem {
                id: keyItem36
                x: 0
                width: 40
                height: 40
                scanCode: 31
            }

            KeyItem {
                id: keyItem37
                x: 0
                width: 40
                height: 40
                scanCode: 32
            }

            KeyItem {
                id: keyItem38
                x: 0
                width: 40
                height: 40
                scanCode: 33
            }

            KeyItem {
                id: keyItem39
                x: 0
                width: 40
                height: 40
                scanCode: 34
            }

            KeyItem {
                id: keyItem40
                x: 0
                width: 40
                height: 40
                scanCode: 35
            }

            KeyItem {
                id: keyItem41
                x: 0
                width: 80
                height: 40
                scanCode: 51
            }
        }

        Row {
            id: row3
            width: childrenRect.width
            height: 40
            spacing: 4

            KeyItem {
                id: keyItem42
                x: 0
                width: 80
                height: 40
                scanCode: 66
                label: "Caps"
            }

            KeyItem {
                id: keyItem43
                x: 0
                width: 40
                height: 40
                scanCode: 38
            }

            KeyItem {
                id: keyItem44
                x: 0
                width: 40
                height: 40
                scanCode: 39
            }

            KeyItem {
                id: keyItem45
                x: 0
                width: 40
                height: 40
                scanCode: 40
            }

            KeyItem {
                id: keyItem46
                x: 0
                width: 40
                height: 40
                scanCode: 41
            }

            KeyItem {
                id: keyItem47
                x: 0
                width: 40
                height: 40
                scanCode: 42
            }

            KeyItem {
                id: keyItem48
                x: 0
                width: 40
                height: 40
                scanCode: 43
            }

            KeyItem {
                id: keyItem49
                x: 0
                width: 40
                height: 40
                scanCode: 44
            }

            KeyItem {
                id: keyItem50
                x: 0
                width: 40
                height: 40
                scanCode: 45
            }

            KeyItem {
                id: keyItem51
                x: 0
                width: 40
                height: 40
                scanCode: 46
            }

            KeyItem {
                id: keyItem52
                x: 0
                width: 40
                height: 40
                scanCode: 47
            }

            KeyItem {
                id: keyItem53
                x: 0
                width: 40
                height: 40
                scanCode: 48
            }

            KeyItem {
                id: keyItem54
                x: 0
                width: 104
                height: 40
                scanCode: 36
                label: "Enter"
            }
        }

        Row {
            id: row4
            width: childrenRect.width
            height: 40
            spacing: 4

            KeyItem {
                id: keyItem55
                x: 0
                width: 90
                height: 40
                modifier: true
                scanCode: 50
                label: "⤊"
            }

            KeyItem {
                id: keyItem56
                x: 0
                width: 40
                height: 40
                scanCode: 52
            }

            KeyItem {
                id: keyItem57
                x: 0
                width: 40
                height: 40
                scanCode: 53
            }

            KeyItem {
                id: keyItem58
                x: 0
                width: 40
                height: 40
                scanCode: 54
            }

            KeyItem {
                id: keyItem59
                x: 0
                width: 40
                height: 40
                scanCode: 55
            }

            KeyItem {
                id: keyItem60
                x: 0
                width: 40
                height: 40
                scanCode: 56
            }

            KeyItem {
                id: keyItem61
                x: 0
                width: 40
                height: 40
                scanCode: 57
            }

            KeyItem {
                id: keyItem62
                x: 0
                width: 40
                height: 40
                scanCode: 58
            }

            KeyItem {
                id: keyItem63
                x: 0
                width: 40
                height: 40
                scanCode: 59
            }

            KeyItem {
                id: keyItem64
                x: 0
                width: 40
                height: 40
                scanCode: 60
            }

            KeyItem {
                id: keyItem65
                x: 0
                width: 40
                height: 40
                scanCode: 61
            }

            KeyItem {
                id: keyItem66
                x: 0
                width: 138
                height: 40
                modifier: true
                scanCode: 62
                label: "⤊"
            }
        }

        Row {
            id: row5
            width: childrenRect.width
            height: 40
            spacing: 4

            KeyItem {
                id: keyItem67
                x: 0
                width: 70
                height: 40
                modifier: true
                scanCode: 37
                label: "Ctrl"
            }

            KeyItem {
                id: keyItem68
                x: 0
                width: 40
                height: 40
                modifier: true
                scanCode: 133
                label: "Win"
            }

            KeyItem {
                id: keyItem69
                x: 0
                width: 60
                height: 40
                modifier: true
                scanCode: 64
                label: "Alt"
            }

            KeyItem {
                id: keyItem70
                x: 0
                width: 264
                height: 40
                scanCode: 65
                label: " "
            }

            KeyItem {
                id: keyItem71
                x: 0
                width: 60
                height: 40
                modifier: true
                scanCode: 108
                label: "Alt Gr"
            }

            IconKeyItem {
                id: keyItem74
                x: 0
                width: 40
                height: 40
                icon: "qrc:///icons/grid-world.svg"
                label: " "
                scanCode: -3
                modifier: false
            }

            KeyItem {
                id: keyItem72
                x: 0
                width: 40
                height: 40
                modifier: true
                scanCode: 134
                label: "Win"
            }

            KeyItem {
                id: keyItem73
                x: 0
                width: 70
                height: 40
                modifier: true
                scanCode: 105
                label: "Ctrl"
            }

        }

    }
    
}
