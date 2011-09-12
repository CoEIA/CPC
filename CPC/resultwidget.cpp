#include "resultwidget.h"
#include "ui_resultwidget.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QHBoxLayout>
#include <QPair>

#include "constants.h"
#include "fileoperations.h"
#include "registeryoperations.h"
#include "settingshandler.h"
#include "utilities.h"
#include "htmlreport.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);

    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(backSlot()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeSlot()));

    ui->treeView->setColumnWidth(0, 350);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setWordWrap(true);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->hideColumn(1);
    ui->itemsSelectedLabel->hide();
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::removeSlot() {
    // return if version is not registered
//    if ( SettingsHandler::isFreeVersion() ) {
//        Utilities::promptForFreeVersion();
//        return;
//    }

    // check for log folder in same directory & make it if not existing
    QDir currentDir;

    if ( !currentDir.exists("Logs"))
        currentDir.mkdir("Logs");

    // make new log files
    fileName = "Logs\\Items- " + QDate::currentDate().toString("dd-MM-yyyy") + " " + QTime::currentTime().toString("hh-mm-ss A");
    logFile = new QFile(fileName);
    if ( ! logFile->open(QIODevice::WriteOnly | QIODevice::Text) )
        qDebug() << "Error" ;

    writer = new QTextStream(logFile);
    writer->setCodec("UTF-8");

    count = 0;
    progressDailog = new QProgressDialog(this);
    progressDailog->setModal(true);
    progressDailog->setRange(0,itemsCount);
    progressDailog->setLabelText(tr("Please Wait Until Deleteing All Items Selected"));

    QModelIndex index;
    dumpModel( rootModel, index, "");

    progressDailog->setValue(itemsCount);
    progressDailog->cancel();

    logFile->close();

    // ask for showing report
    bool ret = Utilities::promptForReport();

    if ( ret ) {
        if ( HTMLReport::generateItemsRemovalReport(fileName) )
            qDebug() << "Generate Ok";
        else
            qDebug() << "Error Generating";
    }

    // after finish return to main window
    emit showWelcomeWidgetSignal();
}

void ResultWidget::backSlot(){
    //logFile->close();
    emit showWelcomeWidgetSignal();
}

void ResultWidget:: dumpModel(const QStandardItemModel *treeModel, const QModelIndex &topIndex, const QString &S ) {
    QModelIndex modelIndex;
    static QString oldPluginsName = "" ;
    static PluginsResult oldPlugins ;

    for( int r = 0; r < treeModel->rowCount( topIndex ); r++ ) {
       for( int c = 0; c < treeModel->columnCount( topIndex ); c++ ) {
        modelIndex = treeModel->index( r, c, topIndex );

        if ( treeModel->data(modelIndex, Qt::CheckStateRole).toBool() ) {

            count++;
            progressDailog->setValue(count);
            qApp->processEvents();
            if (progressDailog->wasCanceled()) {
               return ;
            }

            QStandardItem *sit = static_cast<QStandardItem*> (modelIndex.internalPointer());
            PluginsResult plugins = sit->data().value<PluginsResult>();

            if ( plugins.pluginsName != "" ) {
                oldPluginsName = plugins.pluginsName ;
                oldPlugins = plugins;
            }

            QString text = modelIndex.data( Qt::DisplayRole ).value<QString>();

            if ( ! text.contains("*") )
                continue ;

            QString path = text.split("*")[0];
            QString type = text.split("*")[1];

            if ( type == "File" || type == "Folder") {
                if ( QFile::exists(path) ) {
                    QFileInfo info(path);

                    if ( info.isDir() ){
                        bool ret = FileOperations::deleteAll(path);
                        QString msg = ret ? "Remove True:" : "Remove Error:";
                        QString resultMsg = msg + path ;
                        logDeletedFile(resultMsg);
                    }
                    else {
                        bool ret = FileOperations::deleteObject(path);
                        QString msg = ret ? "Remove True:" : "Remove Error:";
                        QString resultMsg = msg + path ;
                        logDeletedFile(resultMsg);
                    }
                }
            }

            else if ( type == "Key") {
                bool ret = RegisteryOperations::deleteAll(path);
                QString msg = ret ? "Remove True:" : " Remove Error:";
                QString resultMsg = msg + path ;
                logDeletedFile(resultMsg);
            }
            else if ( type == "Value") {
                bool ret = RegisteryOperations::deleteRegValueObject(path);
                QString msg = ret ? "Remove True:" : " Remove Error:";
                QString resultMsg = msg + path ;
                logDeletedFile(resultMsg);
            }
        }
       }

       for( int c = 0; c < treeModel->columnCount( topIndex );c ++ ) {
         modelIndex = treeModel->index( r, c, topIndex );

         count++;
         progressDailog->setValue(count);
         qApp->processEvents();
         if (progressDailog->wasCanceled()) {
            return ;
         }

         dumpModel( treeModel, modelIndex, S + " " );
       }
     }
}

void ResultWidget::logDeletedFile(const QString &filePath) {
    writer->operator <<(filePath);
    writer->operator <<("\n");
}

void ResultWidget:: setPluginsMap(QMultiMap<QString, PluginsResult> plugins){
    pluginsMap.clear();
    pluginsMap = plugins;

    count = 0;
    ui->treeView->setModel(createTreeModel());
    ui->treeView->setColumnWidth(0, 350);
    ui->treeView->hideColumn(1);
    ui->itemsCountLabel->setText(QString::number(itemsCount) + " Item(s) Found");
    itemsCount = 0;
    //ui->itemsSelectedLabel->setText(QString::number(itemsCount) + " Item(s) to Remove");
}

QStandardItemModel* ResultWidget::createTreeModel() {
    int cols = 2;
    int rows =  getRowsNumber();

    rootModel = new CheckableItemModel(rows, cols);

    for (int i=0, j=0; i<rows; j++) {
        if (pluginsMap.values(PluginsCategory[j]).size() == 0 ) {
            continue;
        }

        QStandardItem *plugins = new QStandardItem(PluginsCategory[j]);
        plugins->setIcon(QIcon(":/icons/" + PluginsCategory[j] + ".PNG"));
        plugins->setEditable(false);
        plugins->setCheckable(true);
        plugins->setCheckState(Qt::Checked);
        plugins->appendRows(getChildItems(PluginsCategory[j]));
        rootModel->setItem(i++, 0, plugins);
    }

    rootModel->setHorizontalHeaderLabels(QStringList() << "Plugins" << "Status");
    return (rootModel);
}

QList<QStandardItem*> ResultWidget::getChildItems (const QString& categoryName) {
    QList<QStandardItem*> list;
    QList<PluginsResult> plugins = pluginsMap.values(categoryName);
    int rows = plugins.size();
    QStandardItemModel *model = new QStandardItemModel(rows, PLUGINS_SIZE);

    for (int i=0; i<rows; i++){
        QStandardItem *item = new QStandardItem(plugins.at(i).pluginsName);
        item->setData(QVariant::fromValue(plugins.at(i)));
        item->setEditable(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->appendRows(getSubChildItems(plugins.at(i)));
        list.append(item);
        model->setItem(i, 0, item);
    }

    return (list);
}

QList<QStandardItem*> ResultWidget::getSubChildItems (const PluginsResult& plugins) {
    QList<QStandardItem*> list;

    int row = plugins.results.size();
    for (int i=0; i<row; i++){

        if ( plugins.results.at(i).result.size() == 0 )
            continue;

        QStandardItem *item = new QStandardItem(plugins.results.at(i).itemName);
        item->setEditable(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        item->appendRows(getResultSubChildItems(plugins.results.at(i)));
        list.append(item);
    }

    return (list);
}

QList<QStandardItem*> ResultWidget::getResultSubChildItems (const PluginsResultItems& plugins) {
    QList<QStandardItem*> list;

    int row = plugins.result.size();
    for (int i=0; i<row; i++){
        for (int j=0; j<plugins.result.at(i).second.size(); j++) {

            QString first  = plugins.result.at(i).first ;
            QString second = plugins.result.at(i).second.at(j) ;

            QStandardItem *item = new QStandardItem(second + "*" + first);

            item->setEditable(false);
            item->setCheckable(true);
            item->setCheckState(Qt::Checked);
            item->setToolTip(plugins.result.at(i).second.at(j));
            list.append(item);
        }
    }

    return (list);
}

int ResultWidget::getRowsNumber () {
    int sum = 0;

    for(int i=0; i<PLUGINS_SIZE; i++) {
        if (pluginsMap.values(PluginsCategory[i]).size() == 0 )
            continue;
        else
            sum++;
    }

    return sum;
}

void ResultWidget::retranslateUi () {
    ui->retranslateUi(this);
}

void ResultWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        //retranslate();
    }
    else
        QWidget::changeEvent(event);
}
