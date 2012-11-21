#-------------------------------------------------
#
# Project created by QtCreator 2011-05-05T16:27:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CPC-SCHEDULE
CONFIG   -= console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    taskchecker.cpp

HEADERS += \
    taskchecker.h

win32:DESTDIR = ../bin
win32:OBJECTS_DIR = build/_obj
win32:MOC_DIR = build/_moc
win32:RCC_DIR = build/_rcc
win32:UI_DIR = build/_ui
