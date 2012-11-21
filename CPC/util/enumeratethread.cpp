#include "enumeratethread.h"
#include <QDir>

EnumerateThread::EnumerateThread()
{
}

void EnumerateThread::setData(const QString &filePath, const QStringList &exts) {
    this->filePath = filePath;
    this->exts = exts;
}

void EnumerateThread::run() {
    showAllBellow(filePath, exts, true);

    if ( ! stopped )
        emit finishThreadSignal();
    else
        stopped = false;
}

void EnumerateThread:: showAllBellow (const QString& path, const QStringList& regx, bool subdir){
    QDir dir(path);

    QStringList filters;
    filters << regx ;

    dir.setNameFilters(filters);

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsLast)) {

            if ( info.isDir() && subdir ) {
                showAll(info.absoluteFilePath(), regx, subdir);
            }
            else if( info.isSymLink()) {
                emit addItemSignal(info.filePath());
            }
            else {
                emit addItemSignal(info.absoluteFilePath());
            }
        }
    }
}

void EnumerateThread:: showAll (const QString& path,  const QStringList& regx, bool subdir) {
    showAllBellow(path, regx, subdir);
}
