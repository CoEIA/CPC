#include "scannerthread.h"
#include "fileoperations.h"
#include "pluginsresult.h"
#include "registeryoperations.h"
#include "utilities.h"

#include <QDebug>
#include <QFileInfo>
#include <windows.h>

ScannerThread::ScannerThread()
{
    stopped = false;
    this->setTerminationEnabled(true);
    itemsCount = 0;
}

void ScannerThread::setData(QStandardItemModel *M, const QModelIndex &Top, const QString &S, int item){
    modelPtr = M;
    index = Top;
    text = S;
    items = item;
}

void ScannerThread::run() {

    dumpModel(modelPtr, index, text);

    if ( ! stopped )
        emit finishThreadSignal();
    else
        stopped = false;
}

void ScannerThread::clear(){
    pluginsMap.clear();
    itemsCount = 0;
    stopped = false;
}

void ScannerThread:: dumpModel(
        const QStandardItemModel *treeModel, const QModelIndex &topIndex, const  QString &str ) {

    QModelIndex modelIndex;

    for( int r = 0; r <treeModel->rowCount( topIndex ); r++ ) {
        for( int c = 0; c <treeModel->columnCount( topIndex ); c++ ) {

            if ( stopped ) {
                qDebug() << "return from dump";
                return;
            }

        modelIndex = treeModel->index( r, c, topIndex );

        if ( treeModel->data(modelIndex, Qt::CheckStateRole).toBool() ) {
            QString text = modelIndex.data( Qt::DisplayRole ).value<QString>();
            QStandardItem *sItem = static_cast<QStandardItem*> (modelIndex.internalPointer());

            Plugins plugins = sItem->data().value<Plugins>();
            QString tmp = plugins.metaData.category;
            if ( tmp != categoryName && tmp != "" ) {
                categoryName = tmp;
                emit addItemSignal(new QListWidgetItem(QIcon(getIcon(categoryName)), categoryName));
            }

            handlePlugins(plugins, text);
        }
       }

       for( int c = 0; c <treeModel->columnCount( topIndex ); c++ ) {
        if ( stopped ) return;
         modelIndex = treeModel->index( r, c, topIndex );
         dumpModel( treeModel, modelIndex, str + " " );
       }
     }
 }

QString ScannerThread:: getIcon (const QString& name) {
    QString result = ":/icons/" + name + ".PNG";
    return result;
}

void ScannerThread:: handlePlugins ( Plugins& plugins, const QString& text) {

    if ( text == "" || plugins.metaData.pluginsName == "") {
        //qDebug() << "test: " << text << " " << plugins.metaData.pluginsName;
        return;
    }

    foreach(PluginsItem item, plugins.pluginsItems) {
        //QList<QString> files ;
        QList<QPair<QString, QList<QString> > > files;
        bool flag = false ;

        foreach(PluginsObject object, item.pluginsObject) {
            if ( stopped ) return;

            if ( item.name == text) {
                emit progressBarSignal(0);

                if ( object.type == "Folder" ) {
                    if (object.command == "DeleteAllBellow" ) {
                        QList<QString> list = showAllBellow(object.path);
                        files.append(QPair<QString, QList<QString> > ("Folder", list) );
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }

                    if ( object.command == "FillAndDeleteAllBellow") {
                        QList<QString> list = showFillAllBellow(object.path);
                        files.append(QPair<QString, QList<QString> >("Folder",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }

                    if ( object.command == "DeleteMatchWithinSub") {
                        QList<QString> list = showMatchWithinSub(object.path);
                        files.append(QPair<QString, QList<QString> >("Folder",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }

                    if ( object.command == "DeleteFillMatchWithinSub" ||
                        object.command == "FillDeleteMatchWithinSub") {
                        QList<QString> list = showFillMatchWithinSub(object.path);
                        files.append(QPair<QString, QList<QString> >("Folder",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }
                }

                else if ( object.type == "File") {
                    if ( object.command == "Delete") {
                        QList<QString> list = showMatch(object.path);
                        files.append(QPair<QString, QList<QString> >("File",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }

                    if ( object.command == "FillAndDelete") {
                        QList<QString> list = showFillMatch(object.path);
                        files.append(QPair<QString, QList<QString> >("File",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }
                }

                else if ( object.type == "Key") {
                    if ( object.command == "Delete") {
                        QList<QString> list = showAllKeysBellow(object.path);
                        files.append(QPair<QString, QList<QString> >("Value",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }
                    else if ( object.command == "DeleteAllBellow") {
                        QList<QString> list = showAllKeysBellow(object.path);
                        files.append(QPair<QString, QList<QString> >("Value",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }
                }

                else if ( object.type == "Value") {
                    if ( object.command == "Delete") {
                        QList<QString> list = showAllValuesBellow(object.path);
                        files.append(QPair<QString, QList<QString> >("Value",list));
                        itemsCount += list.size();

                        if ( list.size() > 0 )
                            flag = true;
                    }
                }
            }
        }

        if ( flag) {
            PluginsResult pluginResult;
            pluginResult.pluginsName = plugins.metaData.pluginsName ;

            PluginsResultItems result ;
            result.itemName = item.name;
            result.result = files;

            pluginResult.results.append(result);
            pluginsMap.insertMulti(plugins.metaData.category, pluginResult);
        }
    }
}

QList<QString> ScannerThread:: showFillMatchWithinSub (PluginsPath& path) {
    QStringList locations = Utilities::handleLocation(path.location);
    QList<QString> files ;
    foreach(QString location , locations) {
        foreach(QString pattern, path.patterns){
            QList<QString> tmp = FileOperations::showAllBellow(location, pattern, true);
            files.append(tmp);
        }
    }

    return(files);
}

QList<QString> ScannerThread:: showMatchWithinSub (PluginsPath& path) {
    QString location = Utilities::replaceVariable(path.location);
    QList<QString> files ;

    foreach(QString pattern, path.patterns){
        QList<QString> tmp = FileOperations::showAllBellow(location, pattern, true);
        files.append(tmp);
    }

    return(files);
}

QList<QString> ScannerThread:: showFillMatch (PluginsPath& path){
    QStringList locations = Utilities::handleLocation(path.location);
    QList<QString> files ;
    foreach(QString location , locations) {
        foreach(QString pattern, path.patterns){
            QList<QString> tmp = FileOperations::showMatch(location, pattern);
            files.append(tmp);
        }
    }

    return(files);
}

QList<QString> ScannerThread:: showMatch (PluginsPath& path){
    QString location = Utilities::replaceVariable(path.location);
    QList<QString> files ;

    foreach(QString pattern, path.patterns){
        QList<QString> tmp = FileOperations::showMatch(location, pattern);
        files.append(tmp);
    }

    return(files);
}

QList<QString> ScannerThread:: showAllKeysBellow (PluginsPath& path){
    QString location = path.location;
    QList<QString> files ;

    foreach(QString pattern, path.patterns){
        QList<QString> tmp = RegisteryOperations::getAllValues(location + "\\" + pattern);
        //qDebug() << "key: " << location + "\\" + pattern;
        files.append(tmp);
    }

    return(files);
}

QList<QString> ScannerThread:: showFillAllBellow (PluginsPath& path){
    QStringList locations = Utilities::handleLocation(path.location);
    QList<QString> files ;

    foreach(QString location , locations) {
        foreach(QString pattern, path.patterns){
            QList<QString> tmp = FileOperations::showAllBellow(location, pattern);
            files.append(tmp);
        }
    }

    return(files);
}

QList<QString> ScannerThread:: showAllBellow (PluginsPath& path){
    QString location = Utilities::replaceVariable(path.location);
    QList<QString> files ;

    foreach(QString pattern, path.patterns){
        QList<QString> tmp = FileOperations::showAllBellow(location, pattern, false);
        files.append(tmp);
    }

    return(files);
}

QList<QString> ScannerThread:: showAllValuesBellow (PluginsPath& path){
    QString location = path.location;
    QList<QString> files ;

    foreach(QString pattern, path.patterns){
        QList<QString> tmp = RegisteryOperations::getAllValues(location + "\\" + pattern);
        //qDebug() << "value: " << location + "\\" + pattern;
        files.append(tmp);
    }

    return(files);
}

void ScannerThread::showInList(const QList<QString>& paths, Plugins& plugins) {
    foreach(QString path, paths){
        if ( isImage(path)) {
            showImage(path);
        }
        else
            ;
    }
}

bool ScannerThread:: isImage (const QString& path) {
    if ( QFile::exists(path))  {
        for (int i=0; i<ImageExtensionSize; i++) {
            if ( path.toLower().endsWith(ImageExtension[i]) )
                return true;
        }
    }

    return false;
}

void ScannerThread:: showImage(const QString& path){
//    QGraphicsScene scene;
//    QGraphicsView view(&scene);
//    QGraphicsPixmapItem item(QPixmap(path));
//    scene.addItem(&item);

//    QGraphicsScene scene;
//    scene.addPixmap(QPixmap(path));
//    QGraphicsView view(&scene);
//    view.show();

    qDebug() << "image here: " << path;

//    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addWidget(view);
//    ui->imageWidget->setStyle(layout);
}
