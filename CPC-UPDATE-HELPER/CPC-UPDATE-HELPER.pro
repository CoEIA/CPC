#-------------------------------------------------
#
# Project created by QtCreator 2011-03-14T15:32:39
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = CPC-UPDATE-HELPER
CONFIG   -= console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    keyvalidatornet.cpp

HEADERS += \
    keyvalidatornet.h

win32:DESTDIR = ../bin
win32:OBJECTS_DIR = build/_obj
win32:MOC_DIR = build/_moc
win32:RCC_DIR = build/_rcc
win32:UI_DIR = build/_ui
