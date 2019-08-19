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
#include "x11support.h"
#include <QDebug>
#include <QX11Info>
#include <QWidget>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/record.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#define explicit dont_use_cxx_explicit
#include <xcb/xkb.h>
#undef explicit
#include "keysym2ucs.h"

struct X11SupportPrivateData {
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
	int xkb_event_type;
	int xkb_error_code;
	int cur_layout;
	bool shift_state;
	bool caps_lock_state;
	bool num_lock_state;
	QSet<int> pressed_keys;
};

X11Support::X11Support() {
	m_privateData = new X11SupportPrivateData;
	m_running = false;
}

X11Support::~X11Support() {
	delete m_privateData;
}

void X11Support::start() {
	m_running = true;
	QThread::start();
}

void X11Support::stop() {
	m_running = false;
	wait();
}

void X11Support::run() {
	m_privateData->controlDisplay = XOpenDisplay(NULL);
	m_privateData->dataDisplay = XOpenDisplay(NULL);
	if (m_privateData->controlDisplay && m_privateData->dataDisplay) {
		XSynchronize(m_privateData->controlDisplay, True);
		int major, minor;
		int opcode;
		if (!XkbQueryExtension(m_privateData->controlDisplay, &opcode, &m_privateData->xkb_event_type,
						  &m_privateData->xkb_error_code, nullptr, nullptr)) {
			emit error("No XKB extension present");
		} else {
			XkbStateRec xkbState;
			XkbGetState(m_privateData->controlDisplay, XkbUseCoreKbd, &xkbState);
			m_privateData->cur_layout = xkbState.group;
			unsigned int n;
			XkbGetIndicatorState(m_privateData->controlDisplay, XkbUseCoreKbd, &n);
			m_privateData->caps_lock_state = n & 1;
			m_privateData->num_lock_state = n & 2;
#define XKB_EVT_MASK (XkbStateNotifyMask | XkbIndicatorStateNotifyMask)
			XkbSelectEvents(m_privateData->controlDisplay, XkbUseCoreKbd, XKB_EVT_MASK, XKB_EVT_MASK);
		}
		if (!XRecordQueryVersion(m_privateData->controlDisplay, &major, &minor)) {
			emit error("XRecord extension not supported on this X Server");
		} else {
			XRecordRange *rr[1];
			rr[0] = XRecordAllocRange();
			if (!rr[0]) {
				emit error("Failed to alloc XRecord range object");
			} else {
				rr[0]->device_events.first = KeyPress;
				rr[0]->device_events.last = KeyRelease;
				XRecordClientSpec rcs = XRecordAllClients;
				XRecordContext rc = XRecordCreateContext(m_privateData->controlDisplay,
														 0, &rcs, 1, rr, 1);
				if (!rc) {
					emit error("Failed to create XRecord context");
				} else {
					if (!XRecordEnableContextAsync(m_privateData->dataDisplay, rc, [](XPointer priv,
							XRecordInterceptData *hook) {
						X11Support *that = (X11Support*)priv;
						X11SupportPrivateData *privateData = that->m_privateData;
						if (hook->category == XRecordFromServer) {
							privateData->datum = (X11SupportPrivateData::XRecordDatum*)hook->data;
							that->handleCapturedEvent();
						} else {
							XRecordFreeData(hook);
						}
					}, (char*)this)) {
						emit error("Failed to enable XRecord context");
					} else {
						int controlFd = XConnectionNumber(m_privateData->controlDisplay);
						int dataFd = XConnectionNumber(m_privateData->dataDisplay);
						emit initialized();
						while (m_running) {
							fd_set fds;
							FD_ZERO(&fds);
							FD_SET(controlFd, &fds);
							FD_SET(dataFd, &fds);
							timeval timeout;
							timeout.tv_sec = 2;
							timeout.tv_usec = 0;
							int retval = select(qMax(controlFd, dataFd) + 1,
												&fds, NULL, NULL, &timeout);
							(void)retval;
							if (FD_ISSET(dataFd, &fds)) {
								XRecordProcessReplies(m_privateData->dataDisplay);
							}
							if (FD_ISSET(controlFd, &fds)) {
								XEvent event;
								XNextEvent(m_privateData->controlDisplay, &event);
								if (event.type == m_privateData->xkb_event_type) {
									XkbEvent *kevent = (XkbEvent*)&event;
									switch (kevent->any.xkb_type) {
										case XkbStateNotify:
											if (kevent->state.changed & XkbGroupStateMask) {
												m_privateData->cur_layout = kevent->state.group;
												emit keyboardLayoutChanged();
											}
											break;
										case XkbIndicatorStateNotify:
											m_privateData->caps_lock_state =
													kevent->indicators.state & 1;
											m_privateData->num_lock_state =
													kevent->indicators.state & 2;
											emit indicatorsStateChanged();
											break;
									}
								}
							}
						}
						XRecordDisableContext(m_privateData->controlDisplay, rc);
					}
					XRecordFreeContext(m_privateData->controlDisplay, rc);
				}
			}
			if (rr[0]) XFree(rr[0]);
		}
	} else {
		emit error("Failed to open control or data display for XRecord");
	}
	XCloseDisplay(m_privateData->dataDisplay);
	XCloseDisplay(m_privateData->controlDisplay);
}

void X11Support::handleCapturedEvent() {
	switch (m_privateData->datum->type) {
		case KeyPress: {
			m_privateData->pressed_keys.insert(m_privateData->datum->event.u.u.detail);
			m_privateData->shift_state = m_privateData->pressed_keys.contains(50) ||
					m_privateData->pressed_keys.contains(62);
			emit keyEvent(m_privateData->datum->event.u.u.detail, true);
			break;
		}
		case KeyRelease: {
			m_privateData->pressed_keys.remove(m_privateData->datum->event.u.u.detail);
			m_privateData->shift_state = m_privateData->pressed_keys.contains(50) ||
					m_privateData->pressed_keys.contains(62);
			emit keyEvent(m_privateData->datum->event.u.u.detail, false);
			break;
		}
	}
}

QString X11Support::textForScanCode(int scanCode) {
	int keySymCount = 0;
	KeySym *keySym = XGetKeyboardMapping(QX11Info::display(), scanCode, 1, &keySymCount);
	int normalIndex = m_privateData->cur_layout * 2;
	int shiftedIndex = normalIndex + 1;
	KeySym normal = keySym[normalIndex];
	KeySym shifted = keySym[shiftedIndex];
	QString text = QChar((uint)keysym2ucs(m_privateData->shift_state ? shifted : normal));
	if (m_privateData->caps_lock_state) {
		if (m_privateData->shift_state) {
			text = text.toLower();
		} else {
			text = text.toUpper();
		}
	}
	XFree((char*)keySym);
	return text;
}

bool X11Support::numLockActive() {
	return m_privateData->num_lock_state;
}

bool X11Support::capsLockActive() {
	return m_privateData->caps_lock_state;
}

void X11Support::fakeKeyEvent(int scanCode, bool pressed) {
	XTestFakeKeyEvent(QX11Info::display(), scanCode, pressed, 0);
}

void X11Support::enableBlurForWidgetBackground(QWidget *widget, bool enabled) {
	Atom net_wm_blur_region = XInternAtom(QX11Info::display(),
										  "_KDE_NET_WM_BLUR_BEHIND_REGION", False);
	if (enabled) {
		XChangeProperty(QX11Info::display(), widget->winId(), net_wm_blur_region,
						XA_CARDINAL, 32, PropModeReplace, 0, 0);
	} else {
		XDeleteProperty(QX11Info::display(), widget->winId(), net_wm_blur_region);
	}
}

void X11Support::switchKeyboardLayout() {
	if (!XkbLockGroup(QX11Info::display(), XkbUseCoreKbd, m_privateData->cur_layout + 1)) {
		XkbLockGroup(QX11Info::display(), XkbUseCoreKbd, 0);
	}
}
