#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T14:32:17
#
#-------------------------------------------------

QT       += core gui network
TARGET = CPC
TEMPLATE = app
LIBS += -lshlwapi

TRANSLATIONS = config/ar_sa.ts \
               config/en_us.ts

SOURCES += main.cpp\
    mainwindow.cpp \
    optionswidget.cpp \
    welcomewidget.cpp \
    aboutwidget.cpp \
    shredwidget.cpp \
    schedulewidget.cpp \
    pluginsparser.cpp \
    utilities.cpp \
    fileoperations.cpp \
    registeryoperations.cpp \
    pluginswidget.cpp \
    scanwidget.cpp \
    resultwidget.cpp \
    scannerthread.cpp \
    schedulerdialogue.cpp \
    internethandler.cpp \
    updatechecker.cpp \
    keyvalidator.cpp \
    settingshandler.cpp \
    advancedshreddialog.cpp \
    htmlreport.cpp \
    enumeratethread.cpp \
    application.cpp \
    aboutdialog.cpp \
    commandlineparser.cpp

HEADERS  += mainwindow.h \
    optionswidget.h \
    welcomewidget.h \
    aboutwidget.h \
    shredwidget.h \
    schedulewidget.h \
    pluginsparser.h \
    plugins.h \
    constants.h \
    utilities.h \
    fileoperations.h \
    registeryoperations.h \
    pluginswidget.h \
    scanwidget.h \
    resultwidget.h \
    scannerthread.h \
    pluginsresult.h \
    MyQTreeView.h \
    schedulerdialogue.h \
    CheckableItemModel.h \
    keyvalidator.h \
    internethandler.h \
    updatechecker.h \
    settingshandler.h \
    advancedshreddialog.h \
    htmlreport.h \
    enumeratethread.h \
    application.h \
    aboutdialog.h \
    commandlineparser.h

FORMS    += \
    optionswidget.ui \
    aboutwidget.ui \
    shredwidget.ui \
    schedulewidget.ui \
    pluginswidget.ui \
    scanwidget.ui \
    resultwidget.ui \
    welcomewidget.ui \
    schedulerdialogue.ui \
    advancedshreddialog.ui \
    aboutdialog.ui

RESOURCES += \
    reseorceFiles.qrc

OTHER_FILES += \
    config/ArabicGUI.qss \
    config/EnglishGUI.qss

 RC_FILE = iconpath.rc
