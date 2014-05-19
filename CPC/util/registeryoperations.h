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

#ifndef REGISTERYOPERATIONS_H
#define REGISTERYOPERATIONS_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include <shlwapi.h>
#include <stdio.h>

#include <QString>
#include <QList>
#include <QDebug>

using std::string;
using std::wstring;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

class RegisteryOperations
{
private:
    static const QString hiveStrings[];
    static const HKEY hiveKeys[] ;

public:
    static bool deleteRegValueObject(const QString& path); /* delete value */
    static bool deleteObject    (const QString& path, const QString& value); /* delete value */
    static bool deleteAll       (const QString& path); 	/* delete key and all subkeys and values */

    static bool deleteAllValueBellow    (const QString& path) ; /* delete all values under key */
    static bool deleteAllKeyBellow      (const QString& path) ; /* delete all keys with values under this key */

public:
    static QList<QString> getAllValues  (const QString& path);
    static QList<QString> getAllKeys    (const QString& path);

private:
    static HKEY getHKeyConstant (const QString& root) ;
};

#endif // REGISTERYOPERATIONS_H
