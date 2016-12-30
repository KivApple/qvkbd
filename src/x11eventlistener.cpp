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
#include "x11eventlistener.h"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>

struct X11EventListenerPrivateData {
	Display *controlDisplay;
	Display *dataDisplay;
	union XRecordDatum {
		unsigned char type;
		xEvent event;
		xResourceReq     req   ;
		xGenericReply    reply ;
		xError           error ;
		xConnSetupPrefix setup;
	} *datum;
};

X11EventListener::X11EventListener() {
	m_privateData = new X11EventListenerPrivateData;
	m_running = true;
}

X11EventListener::~X11EventListener() {
	delete m_privateData;
}

void X11EventListener::stop() {
	m_running = false;
	wait();
}

void X11EventListener::run() {
	m_privateData->controlDisplay = XOpenDisplay(NULL);
	m_privateData->dataDisplay = XOpenDisplay(NULL);
	if (m_privateData->controlDisplay && m_privateData->dataDisplay) {
		XSynchronize(m_privateData->controlDisplay, True);
		int major, minor;
		if (!XRecordQueryVersion(m_privateData->controlDisplay, &major, &minor)) {
			emit error("XRecord extension not supported on this X Server");
		} else {
			XRecordRange *rr = XRecordAllocRange();
			if (!rr) {
				emit error("Failed to alloc XRecord range object");
			} else {
				rr->device_events.first = KeyPress;
				rr->device_events.last = KeyRelease;
				XRecordClientSpec rcs = XRecordAllClients;
				XRecordContext rc = XRecordCreateContext(m_privateData->controlDisplay, 0, &rcs, 1, &rr, 1);
				if (!rc) {
					emit error("Failed to create XRecord context");
				} else {
					if (!XRecordEnableContextAsync(m_privateData->dataDisplay, rc, [](XPointer priv,
							XRecordInterceptData *hook) {
						X11EventListener *that = (X11EventListener*)priv;
						X11EventListenerPrivateData *privateData = that->m_privateData;
						if (hook->category == XRecordFromServer) {
							privateData->datum = (X11EventListenerPrivateData::XRecordDatum*)hook->data;
							that->handleCapturedEvent();
						} else {
							XRecordFreeData(hook);
						}
					}, (char*)this)) {
						emit error("Failed to enable XRecord context");
					} else {
						int dataFd = XConnectionNumber(m_privateData->dataDisplay);
						while (m_running) {
							fd_set fds;
							FD_ZERO(&fds);
							FD_SET(dataFd, &fds);
							timeval timeout;
							timeout.tv_sec = 2;
							timeout.tv_usec = 0;
							select(dataFd + 1, &fds, NULL, &fds, &timeout);
							XRecordProcessReplies(m_privateData->dataDisplay);
						}
						XRecordDisableContext(m_privateData->controlDisplay, rc);
					}
					XRecordFreeContext(m_privateData->controlDisplay, rc);
				}
				XFree(rr);
			}
		}
	} else {
		emit error("Failed to open control or data display for XRecord");
	}
	XCloseDisplay(m_privateData->dataDisplay);
	XCloseDisplay(m_privateData->controlDisplay);
}

void X11EventListener::handleCapturedEvent() {
	switch (m_privateData->datum->type) {
		case KeyPress: {
			emit keyEvent(m_privateData->datum->event.u.u.detail, true);
			break;
		}
		case KeyRelease: {
			emit keyEvent(m_privateData->datum->event.u.u.detail, false);
			break;
		}
	}
}
