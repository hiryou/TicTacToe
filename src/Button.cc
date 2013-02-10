/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#include <QPainter>
#include <QEvent>
#include <QTimer>
#include "Button.hh"

Button::Button(QWidget *parent) : QAbstractButton(parent) {
    setCursor(Qt::PointingHandCursor);
}

void Button::setDefaultImage(const QImage &img) {
    this->_imgDefault = img;
}

void Button::setCheckedImage(const QImage &img) {
    this->_imgChecked = img;
}

void Button::changeEvent(QEvent* e) {
    QAbstractButton::changeEvent(e);
    if (e->type() == QEvent::EnabledChange) {
        this->_blinkTimer.stop();
        if (isEnabled())
            setCursor(Qt::PointingHandCursor);
        else
            setCursor(Qt::ArrowCursor);
    }
}

void Button::paintEvent(QPaintEvent* ) {
    const qreal marginW = width() * 0.05;
    const qreal marginH = height() * 0.05;
    const QRectF rect(marginW, marginH,
                      width() - 2 * marginW, height() - 2 * marginH);

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);

    if (isChecked() && !this->_imgChecked.isNull())
        painter.drawImage(rect, this->_imgChecked);
    else if (!this->_imgDefault.isNull())
        painter.drawImage(rect, this->_imgDefault);
}
