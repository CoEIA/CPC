/*
    CoEIA Privacy Control (CPC) - CPC provides easy and efficient methods of finding and
    erasing unwanted and unnecessary items on your computer.

    Copyright (C) 2014 Center of excellence in Information Assurance, All rights reserved.

    This program is free software; you can redistribute it and/or modify it under the terms
    of the GNU General Public License as published by the Free Software Foundation;
    either version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program;
    if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui>

#include "welcomewidget.h"
#include "optionswidget.h"
#include "schedulewidget.h"
#include "shredwidget.h"
#include "aboutwidget.h"
#include "scanwidget.h"
#include "resultwidget.h"
#include "aboutdialog.h"

#include "..\util\utilities.h"
#include "..\util\scannerthread.h"
#include "..\util\pluginsresult.h"
#include "..\util\keyvalidator.h"
#include "..\util\settingshandler.h"
#include "..\util\application.h"
#include "..\util\plugins.h"
#include "..\util\constants.h"
#include "..\util\updatechecker.h"
#include "..\util\commandlineparser.h"

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CommandLineParser parser, QWidget *parent = 0);
    void setVisible(bool visible);
    ~MainWindow();

public:
    enum MenuButton {
        Start, Options, Schedule, Shred, About, Scan, Result
    };

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent* event);

private slots:
    void setCurrentWindow(int id);
    void scanProgressSlot();
    void scanTraySlot();
    void finishScanProcessSlot();
    void showWelcomeWidgetSlot();
    void selectItemSlot(int index);
    void updateSlot(bool showMessage);
    void finishUpdateSlot(bool);
    void errorUpdateSlot();
    void setLanguage(QAction* action) ;
    void handleCommandLineOptions(CommandLineParser parser);
    void openAnotherInstanceMessage(const QString& message);
    void aboutTraySlot();//Tray Slot added
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QStackedWidget *stackedWidget;
    QLabel *versionLabel;
    QSignalMapper *signalMapper;
    QMultiMap<QString, Plugins> plugins;
    QLabel *helpLabel ;
    QAction *arabicAction, *englishAction;

private:
    void createStatusBar();
    void setStatusBarText();
    void setButtonsText();
    void createDockWidget();
    void setupWindow();
    void drawBackgroundToPixmap();
    void trunsButton(bool flag);
    void doAutoScan();
    void scanning();
    void readSettings();
    void automaticCheckForUpdate();
    void retranslate();
    void setShredFile(QStringList paths, int shredLevel);

private:
    WelcomeWidget *welcomeWidget ;
    OptionsWidget *optionWidget;
    ScheduleWidget *scheduleWidget;
    ShredWidget *shredWidget;
    AboutWidget *aboutWidget;

    ScanWidget *scanWidget;
    ResultWidget *resultWidget;

    QWidget* headerWidget;

    QPixmap background;
    QGraphicsScene *scene;

private:
    QMultiMap<QString, PluginsResult> filterMap(QMultiMap<QString, PluginsResult>& map);
    QMap<QString,PluginsResult> convertToMap(QList<PluginsResult>& list) ;
    OVERWRITE_TYPE overwriteType;
    bool registerdVersion ;
    QToolButton *languageButton;

private:
    UpdateChecker thread;
    bool showPopupMessageWhenNoUpdate ;

private:
    void createActions();
    void createTrayIcon();

    QPushButton *scanButton;
    QPushButton *optionsButton;
    QPushButton *scheduleButton;
    QPushButton *shredButton ;
    QPushButton *aboutButton;

    QAction *minimizeAction;
    QAction *aboutAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *scanAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
