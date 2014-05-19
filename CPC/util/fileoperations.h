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
