#-------------------------------------------------
#
# Project created by QtCreator 2010-11-19T10:31:58
#
#-------------------------------------------------

QT       += core gui

TARGET = tictactoe
TEMPLATE = app

CONFIG +=\
    warn_on

SOURCES +=\
    Window.cc \
    Button.cc \
    Board.cc \
    main.cc

HEADERS  += \
    Window.hh \
    Button.hh \
    Board.hh

RESOURCES += images.qrc

ICON = images/icon.icns
