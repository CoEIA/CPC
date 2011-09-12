#include "fileoperations.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QList>

bool FileOperations:: deleteAllBellow (const QString& path, const QString& regx, bool subdir, OVERWRITE_TYPE type){
    QDir dir(path);
    bool result = true;

    QStringList filters;
    filters << regx ;

    dir.setNameFilters(filters);

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsLast)) {
            if ( info.isDir() && subdir ) {
                result = deleteAll(info.absoluteFilePath(), regx, subdir, type);
            }
            else {
                result = deleteObject(info.absoluteFilePath(), type);
            }

            if ( ! result )
                return result;
        }
    }

    return result;
}

bool FileOperations:: deleteAll (const QString& path, const QString& regx, bool subdir, OVERWRITE_TYPE type) {
    bool result = deleteAllBellow(path, regx, subdir, type);

    if ( result ) {
        QDir dir(path);
        dir.rmdir(path);
    }

    return result;
}

bool FileOperations:: deleteObject (const QString& path, OVERWRITE_TYPE type){
    bool result = false;

    switch (type) {
    case NO_OVERWRITE:
        result = deleteFile(path);
        break;

    case ONE_OVERWRITE:
        result = overWriteFile(path, 1);
        result = deleteFile(path);
        break;

    case THREE_OVERWRITE:
        result = overWriteFile(path, 3);
        result = deleteFile(path);
        break;

    case SEVEN_OVERWRITE:
        result = overWriteFile(path, 7);
        result = deleteFile(path);
        break;

    default:
        break;
    }

    return result;
}

bool FileOperations:: overWriteFile (const QString& path, const int itr) {
    bool result = false;

    if ( QFile::exists(path)) {
        for (int i=0; i<itr; i++){
            QFile file(path);

            if ( !file.open(QIODevice::ReadWrite)) {
                qDebug() << "cannot open file for read and write";
                result = false;
                return result;
            }

            QByteArray contentInBytes = file.readAll();
            QByteArray nullBytes(contentInBytes.size(), '0');

            file.reset();

            file.write(nullBytes);
            file.close();
        }

        result = true;
    }

    return (result);
}

bool FileOperations:: deleteFile (const QString& path) {
    bool result = false;

    if ( QFile::exists(path))
        result = QFile::remove(path);

    return result;
}

bool FileOperations:: deleteMatch (const QString& path, QString regx, OVERWRITE_TYPE type){
    bool result = false;

    result = deleteAllBellow(path, regx, false, type);

    return result;
}

bool FileOperations:: deleteMatchWithSub (const QString& path, QString regx, OVERWRITE_TYPE type){
    bool result = false;

    result = deleteAllBellow(path, regx, true, type);

    return result;
}

QList<QString> FileOperations:: showAllBellow (const QString& path, const QString& regx, bool subdir){
    QDir dir(path);
    QList<QString> result ;

    QStringList filters;
    filters << regx ;

    dir.setNameFilters(filters);

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsLast)) {
            if ( info.isDir() && subdir ) {
                result.append(showAll(info.absoluteFilePath(), regx, subdir));
            }
            else if( info.isSymLink()) {
                result.append(info.filePath());
            }
            else {
                result.append(info.absoluteFilePath());
            }
        }
    }

    return result;
}

QList<QString> FileOperations:: showFile (const QString& path, QString& regx) {
    QList<QString> result ;

    if ( QFile::exists(path + "\\" + regx))
        result.append(path + "\\" + regx);

    return result;
}

QList<QString> FileOperations:: showMatch (const QString& path, QString regx) {
    QList<QString> result ;

    if ( regx.contains("*") )
        result.append(showAllBellow(path, regx, false));
    else
        result.append(showFile(path, regx));

    return result;
}

QList<QString> FileOperations:: showMatchWithSub (const QString& path, QString regx){
    QList<QString> result ;

    if ( regx.contains("*") )
        result.append(showAllBellow(path, regx, true));
    else
        result.append(showFile(path, regx));

    return result;
}

QList<QString> FileOperations:: showAll (const QString& path,  const QString& regx, bool subdir) {
    QList<QString> result = showAllBellow(path, regx, subdir);
    return result;
}

QList<QString> FileOperations:: showFolders (const QString& path) {
    QDir dir(path);
    QList<QString> result ;

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs , QDir::DirsLast)) {
            if ( info.isDir()) {
                result.append(info.absoluteFilePath());
            }
        }
    }

    return result;
}
