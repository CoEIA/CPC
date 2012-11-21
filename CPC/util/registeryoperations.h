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
