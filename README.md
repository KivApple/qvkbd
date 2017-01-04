# QVKbd

A virtual keyboard written in C++ and QML.

![Screenshot](misc/screenshot.png)

## Dependecies

You are need X11, Qt and QML packages installed.

## Building

    qmake
    make

## Installing

    install -D -m 0755 qvkbd "$pkgdir/usr/bin/qvkbd"
    install -D -m 0755 qvkbd.desktop "$pkgdir/usr/share/applications/qvkbd.desktop"
    install -D -m 0644 icons/keyboard.svg "$pkgdir/usr/share/icons/hicolor/scalable/qvkbd.svg"

## Usage

All settings can be accessed via context menu of the system tray icon.
Also there is some command-line options:

* --show Run application in visible mode or show already running
* --hide Run application in hidden mode or hide already running
* --no-hide Disable hide button

## License

Program is distributed under terms of GNU GPLv3 license.
