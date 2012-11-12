#include "mainwindow.h"
#include "utilities.h"
#include "scannerthread.h"
#include "pluginsresult.h"

#include "keyvalidator.h"
#include "settingshandler.h"
#include "application.h"

#include "aboutdialog.h"


// http://127.0.0.1/wajdy/info.php?serial=OTkwR-lVHQ0-w2NTg-3MTJ9
// http://127.0.0.1/wajdy/save.php?serial=asdf&ip=20.23.123

MainWindow::MainWindow(bool scan, QWidget *parent)
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

   //Arif code added
   //connect(trayIcon, SIGNAL(trigger()),this, SLOT(hide()));

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

   if ( scan ) {
       readSettings();
       doAutoScan();
   }
   else {
       readSettings();
       automaticCheckForUpdate();
   }


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
    //event->accept();
    //exit(0);
    hide();
    event->ignore();
}
void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    //maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}
void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    aboutAction = new QAction(tr("About"),this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutTraySlot()));

    scanAction = new QAction(tr("Begin Scan"),this);
    connect(scanAction, SIGNAL(triggered()), this, SLOT(scanTraySlot()));

    //maximizeAction = new QAction(tr("Ma&ximize"), this);
    //connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("Open CPC Application"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void  MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(aboutAction);
    //trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(scanAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/images/cpcIcon.ico");
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
    this->setWindowIcon(QIcon(":/images/cpcIcon.ico"));
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

    QPushButton *scanButton = new QPushButton(this);
    scanButton->setCheckable(true);
    scanButton->setChecked(true);
    scanButton->setGeometry(19,y,175,height);
    scanButton->setObjectName("0_toolButton");
    scanButton->setToolTip("Strart Scanning");
    scanButton->setStatusTip("Start Scanning");
    connect(scanButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(scanButton, Start);

    QPushButton *optionsButton = new QPushButton(this);
    optionsButton->setCheckable(true);
    z += height + space;
    optionsButton->setGeometry(19,z,175,height);
    optionsButton->setObjectName("1_toolButton");
    optionsButton->setToolTip("Options");
    optionsButton->setStatusTip("Options");
    connect(optionsButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(optionsButton, Options);

    QPushButton *scheduleButton = new QPushButton(this);
    z += height + space;
    scheduleButton->setGeometry(19,z,175,height);
    scheduleButton->setCheckable(true);
    scheduleButton->setObjectName("2_toolButton");
    scheduleButton->setToolTip("Schedule");
    scheduleButton->setStatusTip("Schedule");
    connect(scheduleButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(scheduleButton, Schedule);

    QPushButton *shredButton = new QPushButton(this);
    z += height + space;
    shredButton->setGeometry(19,z,175,height);
    shredButton->setCheckable(true);
    shredButton->setObjectName("3_toolButton");
    shredButton->setToolTip("Shredded");
    shredButton->setStatusTip("Shredded");
    connect(shredButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(shredButton, Shred);

    QPushButton *aboutButton = new QPushButton(this);
    z += height + space;
    aboutButton->setGeometry(19,z,175,height);
    aboutButton->setCheckable(true);
    aboutButton->setObjectName("4_toolButton");
    aboutButton->setToolTip("About");
    aboutButton->setStatusTip("About");
    connect(aboutButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(aboutButton, About);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setCurrentWindow(int)));
}

void MainWindow::createStatusBar() {
    this->statusBar()->showMessage("");
    this->statusBar()->addPermanentWidget(new QLabel(tr("CoEIA - Privacy Controls")));
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
    this->setWindowTitle(tr("CoEIA Privacy Control"));
    setStatusBarText();
}
