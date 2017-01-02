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
#ifndef X11SUPPORT_H
#define X11SUPPORT_H

#include <QThread>
#include <QSemaphore>

struct X11SupportPrivateData;

class X11Support: public QThread {
	Q_OBJECT
public:
	explicit X11Support();
	~X11Support() override;
	void start();
	void stop();

	QString textForScanCode(int scanCode);
	bool numLockActive();
	bool capsLockActive();
	void fakeKeyEvent(int scanCode, bool pressed);
	void switchKeyboardLayout();
	void enableBlurForWidgetBackground(QWidget *widget, bool enabled);

signals:
	void error(const char *message);
	void keyEvent(int scanCode, bool pressed);
	void keyboardLayoutChanged();
	void indicatorsStateChanged();
	void initialized();

protected:
	void run() override;

private:
	volatile bool m_running;
	X11SupportPrivateData *m_privateData;

	void handleCapturedEvent();

};

#endif // X11SUPPORT_H
