/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#ifndef BUTTON_HH

#define BUTTON_HH

#include <QAbstractButton>
#include <QTimer>

class Button : public QAbstractButton {
    Q_OBJECT
  
public:
    explicit Button(QWidget *parent = 0);
    
    /**
     * set default button image.
     * @param QImage img
     */
    void setDefaultImage(const QImage &img);

    /**
     * set checked button image.
     * @param QImage img
     */
    void setCheckedImage(const QImage &img);

protected:
    /**
     * repaint itself when event changes
     */
    void paintEvent(QPaintEvent* e);
    
    /**
     * turn to default/checked image due to mouse event changes (hover, click)
     */
    void changeEvent(QEvent* e);

private:
    // The images.
    QImage _imgDefault;
    QImage _imgChecked;
    QTimer _blinkTimer;
};

#endif // BUTTON_HH
