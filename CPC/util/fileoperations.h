#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QString>
#include "constants.h"
#include <QList>

class FileOperations
{
public:
    static bool deleteAllBellow (const QString& path, const QString& regx = "*.*", bool subdir=true, OVERWRITE_TYPE type=NO_OVERWRITE);
    static bool deleteAll (const QString& path,  const QString& regx="*.*", bool subdir=true, OVERWRITE_TYPE type=NO_OVERWRITE) ;
    static bool deleteObject (const QString& path, OVERWRITE_TYPE type=NO_OVERWRITE);
    static bool deleteMatch (const QString& path, QString regx, OVERWRITE_TYPE type=NO_OVERWRITE);
    static bool deleteMatchWithSub (const QString& path, QString regx, OVERWRITE_TYPE type=NO_OVERWRITE);

    static QList<QString> showAllBellow (const QString& path, const QString& regx = "*.*", bool subdir=true);
    static QList<QString> showAll (const QString& path,  const QString& regx="*.*", bool subdir=true);
    static QList<QString> showMatch (const QString& path, QString regx);
    static QList<QString> showMatchWithSub (const QString& path, QString regx);
    static QList<QString> showFile (const QString& path, QString& regx);

    static QList<QString> showFolders (const QString& path);

private:
    static bool overWriteFile(const QString& path, const int itr);
    static bool deleteFile (const QString& path);
};

#endif // FILEOPERATIONS_H
