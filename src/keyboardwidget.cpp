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
#include <QApplication>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QQmlContext>
#include <QQmlEngine>
#include <QMessageBox>
#include "keyboardwidget.h"
#include "vkbdapp.h"

KeyboardWidget::KeyboardWidget(QWidget *parent) : QWidget(parent), m_quickWidget(this) {
	setWindowIcon(QIcon(":/icons/keyboard.svg"));
	setFocusPolicy(Qt::NoFocus);
	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::ToolTip | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(&m_quickWidget);
	installEventHandlers();
	layoutChanged();
	loadKeyLayout("qrc:/layouts/standard.qml");
	m_quickWidget.setStyleSheet("KeyItem { color: yellow; }");
	connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(desktopResized()));
}

KeyboardWidget::~KeyboardWidget() {
	m_x11Support.stop();
}

void KeyboardWidget::loadKeyLayout(const QString &name) {
	m_quickWidget.setSource(name);
	setMinimumWidth(m_quickWidget.rootObject()->width());
	setMinimumHeight(m_quickWidget.rootObject()->height());
	updateLayout();
}

void KeyboardWidget::updateLayout() {
	m_buttons.clear();
	updateLayout(m_quickWidget.rootObject());
	updateLayoutLabels();
}

void KeyboardWidget::updateLayout(QQuickItem *item) {
	if (item == nullptr) return;
	auto children = item->childItems();
	for (auto it = children.begin(); it != children.end(); ++it) {
		updateLayout(*it);
	}
	QVariant scanCodeVariant = item->property("scanCode");
	if (!scanCodeVariant.isValid()) return;
	QVariant labelVariant = item->property("label");
	if (!labelVariant.isValid()) return;
	int scanCode = scanCodeVariant.toInt();
	if (m_hideHideButton && scanCode == -2) {
		item->setVisible(false);
		return;
	}
	m_buttons.insert(scanCode, item);
	connect(item, SIGNAL(xPressed(int)), this, SLOT(buttonPressed(int)));
	connect(item, SIGNAL(xReleased(int)), this, SLOT(buttonReleased(int)));
	QString label = labelVariant.toString();
	if (label.size() > 0) return;
	item->setProperty("dynamicLabel", QVariant(true));
}

void KeyboardWidget::updateLayoutLabels() {
	if (m_quickWidget.rootObject() == nullptr) return;
	m_quickWidget.rootObject()->setProperty("numLockActive", m_x11Support.numLockActive());
	updateLayoutLabels(m_quickWidget.rootObject());
}

void KeyboardWidget::updateLayoutLabels(QQuickItem *item) {
	if (item == nullptr) return;
	auto children = item->childItems();
	for (auto it = children.begin(); it != children.end(); ++it) {
		updateLayoutLabels(*it);
	}
	QVariant scanCodeVariant = item->property("scanCode");
	if (!scanCodeVariant.isValid()) return;
	QVariant labelVariant = item->property("label");
	if (!labelVariant.isValid()) return;
	int scanCode = scanCodeVariant.toInt();
	if (scanCode == 66) {
		item->setProperty("checked", QVariant(m_x11Support.capsLockActive()));
	} else if (scanCode == 77) {
		item->setProperty("checked", QVariant(m_x11Support.numLockActive()));
	}
	if (!item->property("dynamicLabel").toBool()) return;
	QString label = m_x11Support.textForScanCode(scanCode);
	item->setProperty("label", QVariant(label));\
}

void KeyboardWidget::layoutChanged() {
	updateLayoutLabels();
}

void KeyboardWidget::indicatorsStateChanged() {
	updateLayoutLabels();
}

void KeyboardWidget::eventListenerError(const char *message) {
	QMessageBox::critical(this, windowTitle(), message, QMessageBox::Ok, QMessageBox::Ok);
}

void KeyboardWidget::installEventHandlers() {
	connect(&m_x11Support, SIGNAL(error(const char*)), this, SLOT(eventListenerError(const char*)));
	connect(&m_x11Support, SIGNAL(keyEvent(int,bool)), this, SLOT(keyEventReceived(int,bool)));
	connect(&m_x11Support, SIGNAL(keyboardLayoutChanged()), this, SLOT(layoutChanged()));
	connect(&m_x11Support, SIGNAL(indicatorsStateChanged()), this, SLOT(indicatorsStateChanged()));
	m_x11Support.start();
}

void KeyboardWidget::keyEventReceived(int scanCode, bool pressed) {
	auto it = m_buttons.find(scanCode);
	if (it != m_buttons.end()) {
		(*it)->setProperty("checked", QVariant(pressed));
	}
	updateLayoutLabels();
}

void KeyboardWidget::buttonPressed(int scanCode) {
	if (scanCode >= 0) {
		auto it = m_buttons.find(scanCode);
		if (it != m_buttons.end()) {
			(*it)->setProperty("checked", true);
		}
		m_x11Support.fakeKeyEvent(scanCode, true);
	}
}

void KeyboardWidget::buttonReleased(int scanCode) {
	if (scanCode >= 0) {
		auto it = m_buttons.find(scanCode);
		if (it != m_buttons.end()) {
			(*it)->setProperty("checked", false);
		}
		m_x11Support.fakeKeyEvent(scanCode, false);
	} else if (scanCode == -2) {
		hide();
	} else if (scanCode == -3) {
		m_x11Support.switchKeyboardLayout();
	}
}

void KeyboardWidget::mousePressEvent(QMouseEvent *event) {
	m_clickPos = event->globalPos();
	if ((event->button() == Qt::LeftButton) && !m_dragging && !m_resizing) {
		QPoint invertedPos = QPoint(width(), height()) - event->pos();
		if ((invertedPos.x() < 10) || (invertedPos.y() < 10)) {
			m_resizing = true;
			m_imaginarySize = size();
		} else {
			m_dragging = true;
		}
	}
}

void KeyboardWidget::mouseReleaseEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	m_dragging = false;
	m_resizing = false;
	m_widgetSizes.insert(QApplication::desktop()->size(), QRect(x(), y(), width(), height()));
}

void KeyboardWidget::mouseMoveEvent(QMouseEvent *event) {
	QPoint delta = event->globalPos() - m_clickPos;
	m_clickPos = event->globalPos();
	if (m_dragging) {
		move(x() + delta.x(), y() + delta.y());
	} else if (m_resizing) {
		m_imaginarySize.setWidth(m_imaginarySize.width() + delta.x());
		m_imaginarySize.setHeight(m_imaginarySize.height() + delta.y());
		resize(m_imaginarySize.width(), m_imaginarySize.height());
	}
}

void KeyboardWidget::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);
	if (m_quickWidget.rootObject() == nullptr) return;
	QSize baseSize(m_quickWidget.rootObject()->width(), m_quickWidget.rootObject()->height());
	float scaleX = (float)m_quickWidget.width() / baseSize.width();
	float scaleY = (float)m_quickWidget.height() / baseSize.height();
	float scale = qMin(scaleX, scaleY);
	m_quickWidget.rootObject()->setTransformOriginPoint(QPointF(0, 0));
	m_quickWidget.rootObject()->setScale(scale);
}

void KeyboardWidget::desktopResized() {
	QSize desktopSize = QApplication::desktop()->size();
	auto it = m_widgetSizes.find(desktopSize);
	if (it != m_widgetSizes.end()) {
		move(it->x(), it->y());
		resize(it->width(), it->height());
	}
	if ((x() + width()) > desktopSize.width()) {
		move(desktopSize.width() - width(), y());
	} else if (x() < 0) {
		move(0, y());
	}
	if ((y() + height()) > desktopSize.height()) {
		move(x(), desktopSize.height() - height());
	} else if (y() < 0) {
		move(x(), 0);
	}
}

void KeyboardWidget::loadSettings() {
	QSettings& settings = VKbdApp::instance()->settings();
	setTransparentBackground(
			settings.value("transparentBackground", QVariant(true)).toBool(),
			settings.value("blurBackground", QVariant(true)).toBool()
	);
	QVariant sizesVariant = settings.value("widgetSizes");
	if (sizesVariant.isNull()) return;
	QVariantList sizes = sizesVariant.toList();
	m_widgetSizes.clear();
	for (auto it = sizes.begin(); it != sizes.end(); ++it) {
		QStringList data = it->toStringList();
		if (data.size() < 6) continue;
		QSize desktopSize(data[0].toInt(), data[1].toInt());
		QRect rect(data[2].toInt(), data[3].toInt(), data[4].toInt(), data[5].toInt());
		m_widgetSizes.insert(desktopSize, rect);
	}
	desktopResized();
}

void KeyboardWidget::storeSettings() {
	QSettings& settings = VKbdApp::instance()->settings();
	settings.setValue("transparentBackground", QVariant(m_transparentBackground));
	settings.setValue("blurBackground", QVariant(m_blurBackground));
	QVariantList sizes;
	for (auto it = m_widgetSizes.begin(); it != m_widgetSizes.end(); ++it) {
		QStringList data;
		data << QString::number(it.key().width()) << QString::number(it.key().height());
		data << QString::number(it.value().x()) << QString::number(it.value().y());
		data << QString::number(it.value().width()) << QString::number(it.value().height());
		sizes.append(QVariant(data));
	}
	settings.setValue("widgetSizes", QVariant(sizes));
}

void KeyboardWidget::setVisible(bool visible) {
	QWidget::setVisible(visible);
	emit visibilityChanged();
}

void KeyboardWidget::hideHideButton() {
	m_hideHideButton = true;
	updateLayout();
}

void KeyboardWidget::setTransparentBackground(bool transparent, bool blur) {
	m_transparentBackground = transparent;
	m_blurBackground = blur;
	m_x11Support.enableBlurForWidgetBackground(this, transparent && blur);
	m_quickWidget.setClearColor(transparent ? Qt::transparent : Qt::white);
	repaint();
}

static bool operator<(const QSize& first, const QSize& second) {
	int hash1 = (first.width() << 16) + first.height();
	int hash2 = (second.width() << 16) + second.height();
	return hash1 < hash2;
}
