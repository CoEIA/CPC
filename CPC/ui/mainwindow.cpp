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

#include "mainwindow.h"

// http://127.0.0.1/wajdy/info.php?serial=OTkwR-lVHQ0-w2NTg-3MTJ9
// http://127.0.0.1/wajdy/save.php?serial=asdf&ip=20.23.123

MainWindow::MainWindow(CommandLineParser parser, QWidget *parent)
    : QMainWindow(parent)
{
   // set windows size, title and icon image
   setupWindow();

   versionLabel = new QLabel(this);

   plugins = Utilities::readAllPlugins(":/Plugins");
   qDebug() << "Number of Plugins : " << plugins.size();

   createActions();
   createTrayIcon();

   welcomeWidget = new WelcomeWidget;
   optionWidget = new OptionsWidget(plugins);
   scheduleWidget = new ScheduleWidget ;
   shredWidget = new ShredWidget;
   aboutWidget = new AboutWidget;
   scanWidget = new ScanWidget;
   resultWidget = new ResultWidget;

   stackedWidget = new QStackedWidget(this);
   stackedWidget->addWidget(welcomeWidget);
   stackedWidget->addWidget(optionWidget);
   stackedWidget->addWidget(scheduleWidget);
   stackedWidget->addWidget(shredWidget);
   stackedWidget->addWidget(aboutWidget);
   stackedWidget->addWidget(scanWidget);
   stackedWidget->addWidget(resultWidget);

   this->setCentralWidget(stackedWidget);

   // Automatic registerat the application with license
   // quick fix for application lifetime for GPL version
   aboutWidget->validateKey("OTkwR-lVHQ0-w2NTg-3MTJ9");

   trayIcon->show();

   createDockWidget();
   createStatusBar();
   setStatusBarText();

   retranslate();

   this->setMaximumSize(QSize(this->width(), this->height()));

   connect(welcomeWidget, SIGNAL(scanProgressSignal()), this, SLOT(scanProgressSlot()));

   connect(scanWidget, SIGNAL(showWelcomeWidgetSignal()), this, SLOT(showWelcomeWidgetSlot()));
   connect(scanWidget, SIGNAL(finishScanProcessSignal()), this, SLOT(finishScanProcessSlot()));

   connect(resultWidget, SIGNAL(showWelcomeWidgetSignal()), this, SLOT(showWelcomeWidgetSlot()));
   connect(optionWidget, SIGNAL(selectItemSignal(int)), this, SLOT(selectItemSlot(int)));

   connect(aboutWidget, SIGNAL(updateSignal(bool)), this, SLOT(updateSlot(bool)));

   connect(&thread, SIGNAL(finishUpdateSignal(bool)), this, SLOT(finishUpdateSlot(bool)));
   connect(&thread, SIGNAL(errorUpdateSignal()), this, SLOT(errorUpdateSlot()));


   connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));   

   // lunch schedule if its not work
   SettingsHandler::writeApplicationInfo();
   Utilities::addScheduleInStartup();
   Utilities::lunchScheduleNow();

   if ( SettingsHandler::isFreeVersion() ) {
       if ( SettingsHandler::isExpireVersion() ) {

           //Utilities:: promptForFreeVersion();

           bool ok;
           QString text = QInputDialog::getText(0, tr("Trial Version Expired"),tr("Please Enter CPC Serial Number"), QLineEdit::Normal,"", &ok);

           if (!ok || text.isEmpty())
               exit(1);

           if ( !aboutWidget->validateKey(text) )
            exit(1);
       }
   }

   readSettings();
   handleCommandLineOptions(parser);
}

void MainWindow::handleCommandLineOptions(CommandLineParser parser) {
    if ( parser.isValidParameter() ) {
        if ( parser.isScanParameter() ) {
            doAutoScan();
        }
        else if ( parser.isShredQuickParameter() ) {
            QStringList paths = parser.getPaths();
            setShredFile(paths, 1);
        }
        else if ( parser.isShredSafeParameter() ) {
             QStringList paths = parser.getPaths();
             setShredFile(paths, 2);
        }
        else if ( parser.isShredThroughParameter() ) {
            QStringList paths = parser.getPaths();
            setShredFile(paths, 3);
        }
        else {
            //automaticCheckForUpdate();
        }
    }
    else { // if wrong parameter , just show the application and ignore the parameters
        //automaticCheckForUpdate();
    }
}

void MainWindow::setShredFile(QStringList paths, int shredLevel) {
    setCurrentWindow(Shred);

    foreach(QString p, paths) {
        shredWidget->addFile(p);
    }

    shredWidget->setShredLevel(shredLevel);
}

void MainWindow::changeEvent(QEvent* event) {

    if (event->type() == QEvent::LanguageChange) {
        retranslate();
    }
    else
        QMainWindow::changeEvent(event);
}


void MainWindow::closeEvent(QCloseEvent *event) {
    SettingsHandler::writeLastUsageTime(QDateTime::currentDateTime());
    hide();
    event->ignore();

    trayIcon->showMessage(
        tr("CPC"),
        tr("The program will keep running in the "
         "system tray")
    );
}
void MainWindow::setVisible(bool visible)
{
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}
void MainWindow::createActions()
{
    aboutAction = new QAction(tr("About"),this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutTraySlot()));

    scanAction = new QAction(tr("Begin Scan"),this);
    connect(scanAction, SIGNAL(triggered()), this, SLOT(scanTraySlot()));

    restoreAction = new QAction(tr("Open CPC Application"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void  MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(scanAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/images/cpcIcon.png");
    trayIcon->setIcon(icon);

    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::aboutTraySlot()
{
    aboutdialog *dialog = new aboutdialog(this);
    dialog->exec();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {

        case QSystemTrayIcon::Unknown:
        case QSystemTrayIcon::Trigger:
            trayIconMenu->hide();
            break;
         case QSystemTrayIcon::DoubleClick:
            showNormal();
            break;
         case QSystemTrayIcon::MiddleClick:

             break;
         default:
             ;
         }
}

void MainWindow::errorUpdateSlot() {
    if ( showPopupMessageWhenNoUpdate )
        Utilities::promptForErrorUpdate();
}

void MainWindow::doAutoScan() {
    scanning();
}

void MainWindow::readSettings() {
    aboutWidget->readSettings();
}

void MainWindow::automaticCheckForUpdate() {
    if ( ! SettingsHandler::isFreeVersion() ) {

        if (  ! SettingsHandler::isCleanState() ) {
            aboutWidget->validateKey(SettingsHandler::readKey().key);
        }

        showPopupMessageWhenNoUpdate = false;
        thread.setData(Utilities::getProgramVersion(),Utilities::getDatabaseVersion());
        thread.start();
    }
}

void MainWindow::updateSlot(bool showMessage) {
    if ( ! SettingsHandler::isFreeVersion()) {
        showPopupMessageWhenNoUpdate = showMessage;
        thread.setData(Utilities::getProgramVersion(),Utilities::getDatabaseVersion());
        thread.start();
    }
    else {
        Utilities::promptForFreeVersion();
    }
}

void MainWindow::finishUpdateSlot(bool state) {
    if ( state ) {
        if ( Utilities::promptForUpdate() ) {
            Utilities::lunchUpdate();
        }
    }
    else {
        if ( showPopupMessageWhenNoUpdate ) {
            Utilities::promptForNoUpdate();
        }
    }
}

void MainWindow::setupWindow() {
    QRect screenRect = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());
    QRect windowRect(0, 0, 800, 600);

    if (screenRect.width() < 800)
        windowRect.setWidth(screenRect.width());
    if (screenRect.height() < 600)
        windowRect.setHeight(screenRect.height());

    windowRect.moveCenter(screenRect.center());

    this->setGeometry(windowRect);
    this->setMinimumSize(80, 60);
    this->setWindowIcon(QIcon(":/images/cpcIcon.png"));
    this->setFixedSize(this->size().width(), this->size().height());
    this->setStyleSheet("MainWindow {background-image: url(:/images/cpc_background.PNG);}");
}

MainWindow::~MainWindow()
{

}

void MainWindow::showWelcomeWidgetSlot(){
    setCurrentWindow(Start);
    trunsButton(true);
}

void MainWindow::selectItemSlot(int index) {
    switch (index) {
    case 0:
        overwriteType = NO_OVERWRITE;
        break;
    case 1:
        overwriteType = ONE_OVERWRITE;
        break;
    case 2:
        overwriteType = THREE_OVERWRITE;
        break;
    case 3:
        overwriteType = SEVEN_OVERWRITE;
        break;
    default:
        overwriteType = NO_OVERWRITE;
    }
}

void MainWindow::finishScanProcessSlot() {
    setCurrentWindow(Result);

    QMultiMap<QString, PluginsResult> pluginsMap = scanWidget->getPluginsMap();

    // filter map
    QMultiMap<QString, PluginsResult> filterdMap = filterMap(pluginsMap);
    resultWidget->setItemsCount(scanWidget->getItemsCount());
    resultWidget->setPluginsMap(filterdMap);
}

QMultiMap<QString, PluginsResult> MainWindow:: filterMap(QMultiMap<QString, PluginsResult>& map) {
    QMultiMap<QString, PluginsResult> resultMap;

    for(int i=0; i<PLUGINS_SIZE; i++) {
        QList<PluginsResult> category = map.values( PluginsCategory[i]);
        QMap<QString,PluginsResult> programItems = convertToMap(category);

        QMapIterator<QString, PluginsResult> itr(programItems);
        while ( itr.hasNext() ) {
            PluginsResult r = itr.next().value();
            resultMap.insertMulti(PluginsCategory[i], r);
        }
    }

    return resultMap;
}

QMap<QString,PluginsResult> MainWindow:: convertToMap(QList<PluginsResult>& list) {
    QMap<QString, PluginsResult> map;

    foreach(PluginsResult plugins, list){
        if ( map.contains(plugins.pluginsName) ) {
            PluginsResult items = map.value(plugins.pluginsName);
            items.results.append(plugins.results);
            map.insert(plugins.pluginsName, items);
        }
        else{
            map.insert(plugins.pluginsName, plugins);
        }
    }

    return map;
}

void MainWindow::scanProgressSlot() {
    scanning();
}

void MainWindow::scanTraySlot(){
    show();
    scanning();
}

void MainWindow::scanning () {
    setCurrentWindow(Scan);

    trunsButton(false);
    scanWidget->clear();

    QModelIndex index;
    int item = optionWidget->getItemNumber();
    scanWidget->startScanning(optionWidget->pw->rootModel, index, "", item);
}

void MainWindow::trunsButton(bool flag) {
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    foreach (QPushButton* button,buttons) {
        if ( button->objectName().endsWith("_toolButton")) {
            button->setDisabled(! flag);
        }
    }
}

void MainWindow:: createDockWidget () {
    signalMapper = new QSignalMapper(this);

    QDockWidget *headerDockWidget = new QDockWidget(this);
    headerDockWidget->setObjectName("headerDockWidget");
    headerDockWidget->setFloating(false);
    headerDockWidget->setTitleBarWidget(new QWidget);
    headerDockWidget->setFixedWidth(220);
    headerDockWidget->setFixedHeight(490);

    headerWidget = new QWidget;
    headerDockWidget->setWidget(headerWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, headerDockWidget);

    int y = 120 ;
    int space = 5 ;
    int height = 72 ;
    int z = y ;

    scanButton = new QPushButton(this);
    scanButton->setCheckable(true);
    scanButton->setChecked(true);
    scanButton->setGeometry(19,y,175,height);
    scanButton->setObjectName("0_toolButton");
    connect(scanButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(scanButton, Start);

    optionsButton = new QPushButton(this);
    optionsButton->setCheckable(true);
    z += height + space;
    optionsButton->setGeometry(19,z,175,height);
    optionsButton->setObjectName("1_toolButton");
    connect(optionsButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(optionsButton, Options);

    scheduleButton = new QPushButton(this);
    z += height + space;
    scheduleButton->setGeometry(19,z,175,height);
    scheduleButton->setCheckable(true);
    scheduleButton->setObjectName("2_toolButton");
    connect(scheduleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(scheduleButton, Schedule);

    shredButton = new QPushButton(this);
    z += height + space;
    shredButton->setGeometry(19,z,175,height);
    shredButton->setCheckable(true);
    shredButton->setObjectName("3_toolButton");
    connect(shredButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(shredButton, Shred);

    aboutButton = new QPushButton(this);
    z += height + space;
    aboutButton->setGeometry(19,z,175,height);
    aboutButton->setCheckable(true);
    aboutButton->setObjectName("4_toolButton");
    connect(aboutButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(aboutButton, About);

    setButtonsText();

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setCurrentWindow(int)));
}

void MainWindow::createStatusBar() {
    this->statusBar()->showMessage("");
    this->statusBar()->addPermanentWidget(new QLabel(tr("CoEIA - Privacy Control - 2.0")));
    this->statusBar()->addPermanentWidget(versionLabel);

    helpLabel = new QLabel(this);
    helpLabel->setOpenExternalLinks(true);
    this->statusBar()->addPermanentWidget(helpLabel);

    QMenu *selectLanguageMenu = new QMenu(this);
    arabicAction = new QAction(this);
    englishAction = new QAction(this);

    QActionGroup* actions = new QActionGroup(this);
    connect(actions, SIGNAL(triggered(QAction*)), this, SLOT(setLanguage(QAction*)));

    arabicAction->setCheckable(true);
    arabicAction->setData("ar_SA");

    englishAction->setCheckable(true);
    englishAction->setData("en_US");

    if ( QLocale::system().name().startsWith("ar"))
        arabicAction->setChecked(true);
    else
        englishAction->setChecked(true);

    actions->addAction(arabicAction);
    actions->addAction(englishAction);

    selectLanguageMenu->addAction(arabicAction);
    selectLanguageMenu->addAction(englishAction);

    languageButton = new QToolButton(this);
    languageButton->setMenu(selectLanguageMenu);
    languageButton->setPopupMode(QToolButton::InstantPopup);

    this->statusBar()->addPermanentWidget(languageButton);
}

void MainWindow::setButtonsText() {
    scanButton->setToolTip(tr("Scan the whole window system and find all the traces of recorded history"));
    scanButton->setStatusTip(tr("Scan the whole window system and find all the traces of recorded history"));
    optionsButton->setToolTip(tr("User filter options for history scanning"));
    optionsButton->setStatusTip(tr("User filter options for history scanning"));
    scheduleButton->setToolTip(tr("Create personal schedule for system wide scanning and actions"));
    scheduleButton->setStatusTip(tr("Create personal schedule for system wide scanning and actions"));
    shredButton->setToolTip(tr("Shred files from the system to remove them permanently"));
    shredButton->setStatusTip(tr("Shred files from the system to remove them permanently"));
    aboutButton->setToolTip("About");
    aboutButton->setStatusTip("About");
}

void MainWindow::setStatusBarText() {
    QString link = "Manual\\CPC-Manual.pdf";
    helpLabel->setText(tr("<a href=\"%1\" style=\"color:yellow;\">User Manual</a></font>").arg(link));
    languageButton->setText(tr("Language..."));
    arabicAction->setText(tr("Arabic"));
    englishAction->setText(tr("English"));
}

void MainWindow::setCurrentWindow(int id){
    QString buttonName = QString::number(id) + "_toolButton";
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    foreach (QPushButton* button,buttons) {
        if (button->objectName() == buttonName)
            button->setChecked(true);
        else
            button->setChecked(false);
    }

    stackedWidget->setCurrentIndex(id);
}

void MainWindow::openAnotherInstanceMessage(const QString& message) {
    showNormal();

    QStringList arguments = message.split(",");

//    QString result = "";
//    for(int i=0; i<arguments.size(); i++) {
//        result += "Line: " + arguments.at(i) + "\n";
//    }

//    QMessageBox msg;
//    msg.setText(result);
//    msg.exec();

    CommandLineParser parser(arguments);
    handleCommandLineOptions(parser);
}

void MainWindow::setLanguage(QAction* action) {
    Application::setLanguage(action->data().toString());
    qApp->setLayoutDirection(QObject::tr("LTR")== "RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    // change appliation style depend on the language
    if ( action->data().toString() == "ar_SA" ) {
        Utilities::loadArabicStyle();
    }
    else {
        Utilities::loadEnglishStyle();
    }
}

void MainWindow:: retranslate () {
    QString cpcEdition ;
    if ( SettingsHandler::isFreeVersion() )
        cpcEdition = tr("Trial Version");
    else
        cpcEdition = QString("Version %1").arg(Utilities::getProgramVersion());

    versionLabel->setText(cpcEdition);
    this->setWindowTitle(tr("CoEIA Privacy Control - 2.0"));
    setStatusBarText();        

    aboutAction->setText(tr("About"));
    scanAction->setText(tr("Begin Scan"));
    restoreAction->setText(tr("Open CPC Application"));
    quitAction->setText(tr("Exit"));
    setButtonsText();
}
