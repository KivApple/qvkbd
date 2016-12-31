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
#include <QDebug>
#include <QDBusConnection>
#include <QCommandLineParser>
#include "aboutdialog.h"
#include "qvkbd_interface.h"
#include "vkbdapp.h"

VKbdApp* VKbdApp::m_instance = nullptr;

VKbdApp::VKbdApp(int &argc, char *argv[]): QApplication(argc, argv),
		m_settings(QLatin1String("Eternal Search"), QLatin1String("QVKbd")),
		m_singletonSemaphore("QVKbd_sem", 1), m_singletonMemory("QVKbd_mem"),
		m_DBusAdaptor(this) {
	m_instance = this;
	setApplicationName("QVKbd");
	setApplicationVersion("0.1");
	m_trayIcon.setIcon(QIcon(":/icons/keyboard.svg.png"));
	m_trayIcon.setToolTip(m_keyboardWidget.windowTitle());
	m_quitAction.setText(tr("Quit"));
	m_toggleAction.setText(tr("Toggle visible"));
	m_aboutAction.setText(tr("About..."));
	m_transparentBackgroundAction.setText(tr("Transparent background"));
	m_transparentBackgroundAction.setCheckable(true);
	m_blurBackgroundAction.setText(tr("Blur background"));
	m_blurBackgroundAction.setCheckable(true);
	connect(&m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
	connect(&m_quitAction, SIGNAL(triggered(bool)), this, SLOT(quitActionTriggered()));
	connect(&m_toggleAction, SIGNAL(triggered(bool)), this, SLOT(toggleActionTriggered()));
	connect(&m_aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutActionTriggered()));
	m_trayIconMenu.addAction(&m_toggleAction);
	m_trayIconMenu.addAction(&m_aboutAction);
	m_trayIconMenu.addSeparator();
	m_trayIconMenu.addAction(&m_transparentBackgroundAction);
	m_trayIconMenu.addAction(&m_blurBackgroundAction);
	m_trayIconMenu.addSeparator();
	m_trayIconMenu.addAction(&m_quitAction);
	m_trayIcon.setContextMenu(&m_trayIconMenu);
	m_trayIcon.show();
	m_keyboardWidget.loadSettings();
	m_transparentBackgroundAction.setChecked(m_keyboardWidget.transparentBackground());
	m_blurBackgroundAction.setChecked(m_keyboardWidget.blurBackground());
	m_blurBackgroundAction.setEnabled(m_keyboardWidget.transparentBackground());
	connect(&m_transparentBackgroundAction, SIGNAL(toggled(bool)), this, SLOT(transparentBackgroundToggled(bool)));
	connect(&m_blurBackgroundAction, SIGNAL(toggled(bool)), this, SLOT(blurBackgroundToggled(bool)));
	QCommandLineParser parser;
	parser.setApplicationDescription("Qt Virtual Keyboard");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOptions({
		{"show", tr("Run the application in visible mode, or show already running")},
		{"hide", tr("Run the application in hidden mode or hide already running")},
		{"no-hide", tr("Disable hide button")}
	});
	parser.process(*this);
	if (parser.isSet("hide")) {
		checkSingleton(CheckSingletonAction::HIDE);
	} else if (parser.isSet("show")) {
		checkSingleton(CheckSingletonAction::SHOW);
	} else {
		checkSingleton(CheckSingletonAction::TOGGLE);
	}
	connect(&m_keyboardWidget, SIGNAL(visibilityChanged()), SIGNAL(visibleChanged()));
	if (parser.isSet("no-hide")) {
		m_keyboardWidget.hideHideButton();
	}
	if (!parser.isSet("hide")) {
		m_keyboardWidget.show();
	}
}

void VKbdApp::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::Trigger:
			m_toggleAction.trigger();
			break;
		default:
			;
	}
}

void VKbdApp::quitActionTriggered() {
	m_keyboardWidget.storeSettings();
	quit();
}

void VKbdApp::toggleActionTriggered() {
	if (m_keyboardWidget.isVisible()) {
		m_keyboardWidget.hide();
	} else {
		m_keyboardWidget.show();
	}
}

void VKbdApp::aboutActionTriggered() {
	AboutDialog dialog;
	dialog.exec();
}

void VKbdApp::transparentBackgroundToggled(bool state) {
	m_keyboardWidget.setTransparentBackground(state, m_blurBackgroundAction.isChecked());
	m_blurBackgroundAction.setEnabled(state);
}

void VKbdApp::blurBackgroundToggled(bool state) {
	m_keyboardWidget.setTransparentBackground(m_transparentBackgroundAction.isChecked(), state);
}

void VKbdApp::checkSingleton(CheckSingletonAction action) {
	m_singletonSemaphore.acquire();
	if (m_singletonMemory.attach()) {
		m_singletonMemory.detach();
	}
	bool singleInstance = m_singletonMemory.create(100);
	if (singleInstance) {
		if (!QDBusConnection::sessionBus().registerObject("/KbdWidget", this)) {
			qDebug() << "Failed to register D-Bus object!";
		} else {
			if (!QDBusConnection::sessionBus().registerService("xyz.etse.QVKbd")) {
				qDebug() << "Failed to register D-Bus service!";
			}
		}
	} else {
		XyzEtseQVKbdInterface iface("xyz.etse.QVKbd", "/KbdWidget", QDBusConnection::sessionBus(), this);
		switch (action) {
			case CheckSingletonAction::SHOW:
				iface.setVisible(true);
				break;
			case CheckSingletonAction::HIDE:
				iface.setVisible(false);
				break;
			case CheckSingletonAction::TOGGLE:
				iface.toggleVisible();
				break;
		}
	}
	m_singletonSemaphore.release();
	if (!singleInstance) {
		::exit(0);
	}
}
