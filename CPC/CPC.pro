#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T14:32:17
#
#-------------------------------------------------

QT       += core gui network

TARGET = CPC
TEMPLATE = app

LIBS += -lshlwapi

include(3rdparty/qtsingleapplication.pri)

SOURCES += main.cpp \
    ui/welcomewidget.cpp \
    ui/shredwidget.cpp \
    ui/schedulewidget.cpp \
    ui/schedulerdialogue.cpp \
    ui/scanwidget.cpp \
    ui/resultwidget.cpp \
    ui/pluginswidget.cpp \
    ui/optionswidget.cpp \
    ui/mainwindow.cpp \
    ui/advancedshreddialog.cpp \
    ui/aboutwidget.cpp \
    ui/aboutdialog.cpp \
    util/utilities.cpp \
    util/updatechecker.cpp \
    util/settingshandler.cpp \
    util/scannerthread.cpp \
    util/registeryoperations.cpp \
    util/pluginsparser.cpp \
    util/keyvalidator.cpp \
    util/internethandler.cpp \
    util/htmlreport.cpp \
    util/fileoperations.cpp \
    util/enumeratethread.cpp \
    util/commandlineparser.cpp \
    util/application.cpp

RESOURCES += \
    reseorceFiles.qrc

OTHER_FILES += \
    config/ArabicGUI.qss \
    config/EnglishGUI.qss

RC_FILE = iconpath.rc

win32:DESTDIR = ../bin
win32:OBJECTS_DIR = build/_obj
win32:MOC_DIR = build/_moc
win32:RCC_DIR = build/_rcc
win32:UI_DIR = build/_ui

HEADERS += \
    ui/welcomewidget.h \
    ui/shredwidget.h \
    ui/schedulewidget.h \
    ui/schedulerdialogue.h \
    ui/scanwidget.h \
    ui/resultwidget.h \
    ui/pluginswidget.h \
    ui/optionswidget.h \
    ui/mainwindow.h \
    ui/advancedshreddialog.h \
    ui/aboutwidget.h \
    ui/aboutdialog.h \
    util/utilities.h \
    util/updatechecker.h \
    util/settingshandler.h \
    util/scannerthread.h \
    util/registeryoperations.h \
    util/pluginsresult.h \
    util/pluginsparser.h \
    util/plugins.h \
    util/MyQTreeView.h \
    util/keyvalidator.h \
    util/internethandler.h \
    util/htmlreport.h \
    util/fileoperations.h \
    util/enumeratethread.h \
    util/constants.h \
    util/commandlineparser.h \
    util/CheckableItemModel.h \
    util/application.h

FORMS += \
    ui/welcomewidget.ui \
    ui/shredwidget.ui \
    ui/schedulewidget.ui \
    ui/schedulerdialogue.ui \
    ui/scanwidget.ui \
    ui/resultwidget.ui \
    ui/pluginswidget.ui \
    ui/OptionsWidget.ui \
    ui/advancedshreddialog.ui \
    ui/aboutwidget.ui \
    ui/aboutdialog.ui
