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
#ifndef VKBDAPP_H
#define VKBDAPP_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "keyboardwidget.h"
#include "qvkbd_adaptor.h"

class VKbdApp : public QApplication {
	Q_OBJECT

	Q_PROPERTY(bool visible READ visible WRITE setVisible)

public:
	VKbdApp(int& argc, char *argv[]);

	static VKbdApp* instance() {
		return m_instance;
	}

	QSettings& settings() {
		return m_settings;
	}

	KeyboardWidget& keyboardWidget() {
		return m_keyboardWidget;
	}

	bool visible() {
		return m_keyboardWidget.isVisible();
	}

	void setVisible(bool visible) {
		return m_keyboardWidget.setVisible(visible);
	}

signals:
	void visibleChanged();

public slots:
	void toggleVisible() {
		setVisible(!visible());
	}

private slots:
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void toggleActionTriggered();
	void quitActionTriggered();
	void aboutActionTriggered();
	void transparentBackgroundToggled(bool state);
	void blurBackgroundToggled(bool state);
	void resetWidgetSize();

private:
	enum class CheckSingletonAction {
		TOGGLE,
		SHOW,
		HIDE
	};

	static VKbdApp* m_instance;
	KeyboardWidget m_keyboardWidget;
	QSystemTrayIcon m_trayIcon;
	QMenu m_trayIconMenu;
	QAction m_quitAction;
	QAction m_toggleAction;
	QAction m_aboutAction;
	QAction m_transparentBackgroundAction;
	QAction m_blurBackgroundAction;
	QAction m_resetSizeAction;
	QSettings m_settings;
	QSystemSemaphore m_singletonSemaphore;
	QSharedMemory m_singletonMemory;
	QVKbdAdaptor m_DBusAdaptor;

	void checkSingleton(CheckSingletonAction action);

};

#endif // VKBDAPP_H
