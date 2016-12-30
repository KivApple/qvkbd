/*
 * This file is part of the qvkbd project.
 * Copyright (C) 2016 <kiv.apple@gmail.com>
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
#ifndef X11EVENTLISTENER_H
#define X11EVENTLISTENER_H

#include <QThread>
#include <QKeyEvent>

struct X11EventListenerPrivateData;

class X11EventListener: public QThread {
	Q_OBJECT
public:
	explicit X11EventListener();
	~X11EventListener() override;
	void stop();

signals:
	void error(const char *message);
	void keyEvent(int scanCode, bool pressed);

protected:
	void run() override;

private:
	volatile bool m_running;
	X11EventListenerPrivateData *m_privateData;
	void handleCapturedEvent();

};

#endif // X11EVENTLISTENER_H
