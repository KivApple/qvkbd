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
#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QQuickWidget>
#include <QQuickItem>
#include "x11support.h"

class KeyboardWidget : public QWidget
{
	Q_OBJECT
public:
	explicit KeyboardWidget(QWidget *parent = 0);
	~KeyboardWidget() override;
	void loadSettings();
	void storeSettings();
	void setVisible(bool visible) override;
	void hideHideButton();
	void setTransparentBackground(bool transparent, bool blur);
	bool transparentBackground() { return m_transparentBackground; }
	bool blurBackground() { return m_blurBackground; }

signals:
	void visibilityChanged();

private slots:
	void layoutChanged();
	void buttonPressed(int scanCode);
	void buttonReleased(int scanCode);
	void keyEventReceived(int scanCode, bool pressed);
	void desktopResized();
	void eventListenerError(const char *message);
	void indicatorsStateChanged();
	void x11SupportInitialized();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	QQuickWidget m_quickWidget;
	QMap<int, QQuickItem*> m_buttons;
	QPoint m_clickPos;
	bool m_dragging;
	bool m_resizing;
	QSize m_imaginarySize;
	X11Support m_x11Support;
	QMap<QSize, QRect> m_widgetSizes;
	bool m_hideHideButton = false;
	bool m_transparentBackground;
	bool m_blurBackground;

	void loadKeyLayout(const QString& name);
	void updateLayout();
	void updateLayout(QQuickItem *item);
	void updateLayoutLabels();
	void updateLayoutLabels(QQuickItem *item);
	void installEventHandlers();

};

#endif // KEYBOARDWIDGET_H
