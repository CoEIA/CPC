#-------------------------------------------------
#
# Project created by QtCreator 2011-03-12T11:44:30
#
#-------------------------------------------------

QT       += core gui sql


TARGET = CPC-SERIAL-GENERATOR
TEMPLATE = app


SOURCES += main.cpp\
        generatorwidget.cpp \
    lunhalgorithm.cpp

HEADERS  += generatorwidget.h \
    lunhalgorithm.h \
    KeyGenerator.h \
    KeyValidator.h

FORMS    += generatorwidget.ui
