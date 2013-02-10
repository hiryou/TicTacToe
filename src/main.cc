/**
 * Author:      Long Nguyen
 * Project:     CS300 - Assignment 4
 * License:     MIT license, see COPYING
 *
 * Copyright (C) 2010 Long Nguyen
 */

#include <QtGui/QApplication>
#include "Window.hh"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window win;

    app.setWindowIcon(QIcon(":/images/game_icon.png"));
    win.show();

    return app.exec();
}
