#include "registeryoperations.h"

const HKEY      RegisteryOperations::  hiveKeys[] = {HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS};
const QString   RegisteryOperations::  hiveStrings[]  = {"HKEY_CLASSES_ROOT", "HKEY_CURRENT_USER", "HKEY_LOCAL_MACHINE", "HKEY_USERS" };

bool RegisteryOperations :: deleteAllValueBellow (const QString& qPath) {
    QList<QString> values = getAllValues(qPath);

    for(int i=0; i<values.size(); i++) {
        deleteObject(qPath, values[i]);
    }

    return (true);
}

bool RegisteryOperations :: deleteAllKeyBellow (const QString& qPath) {
    QList<QString> values = getAllKeys(qPath);

    for(int i=0; i<values.size(); i++) {
        QString tmp = qPath + "\\" + values[i] ;
        deleteAll(tmp);
    }

    return (true);
}

QList<QString> RegisteryOperations :: getAllKeys (const QString& path) {

    int index = path.indexOf("\\");

    QString root = path.left(index);
    QString key = path.mid(index+1);

    QList<QString> list;

    HKEY hKey;
    if ( RegOpenKeyEx(getHKeyConstant(root), key.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS ) {
        // code form: MSDN QueryKey example

        TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
        DWORD    cbName;                   // size of name string
        TCHAR    achClass[MAX_PATH] ;  // buffer for class name
        DWORD    cchClassName = MAX_PATH;  // size of class string
        DWORD    cSubKeys=0;               // number of subkeys
        DWORD    cbMaxSubKey;              // longest subkey size
        DWORD    cchMaxClass;              // longest class string
        DWORD    cValues;              // number of values for key
        DWORD    cchMaxValue;          // longest value name
        DWORD    cbMaxValueData;       // longest value data
        DWORD    cbSecurityDescriptor; // size of security descriptor
        FILETIME ftLastWriteTime;      // last write time

        DWORD i, retCode;

        // Get the class name and the value count.
        retCode = RegQueryInfoKey(
             hKey,                    // key handle
            achClass,                // buffer for class name
            &cchClassName,           // size of class string
            NULL,                    // reserved
            &cSubKeys,               // number of subkeys
            &cbMaxSubKey,            // longest subkey size
            &cchMaxClass,            // longest class string
            &cValues,                // number of values for this key
            &cchMaxValue,            // longest value name
            &cbMaxValueData,         // longest value data
            &cbSecurityDescriptor,   // security descriptor
            &ftLastWriteTime);       // last write time

        // Enumerate the subkeys, until RegEnumKeyEx fails.

        if (cSubKeys)
        {
            //qDebug() << "Number of subkeys: " << cSubKeys ;

            for (i=0; i<cSubKeys; i++)
            {
                cbName = MAX_KEY_LENGTH;
                retCode = RegEnumKeyEx(hKey, i,
                     achKey,
                     &cbName,
                     NULL,
                     NULL,
                     NULL,
                     &ftLastWriteTime);
                if (retCode == ERROR_SUCCESS)
                {
                    QString tmp = QString::fromWCharArray(achKey);
                    //qDebug() << "Full Reg Path: " << path + "\\" + tmp;
                    list.push_back( path + "\\" + tmp);
                }
            }
        }

         RegCloseKey(hKey);
    }
    else {
        //qDebug() << "not found key" ;
    }

    return list;
}

QList<QString> RegisteryOperations :: getAllValues (const QString& path) {
    int index = path.indexOf("\\");
    QString root = path.left(index);
    QString key = path.mid(index+1);

    QList<QString> list;

    HKEY hKey;
    if ( RegOpenKeyEx(getHKeyConstant(root), key.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS ) {
        // code form: MSDN QueryKey example

        TCHAR    achClass[MAX_PATH]; // buffer for class name
        DWORD    cchClassName = MAX_PATH;  // size of class string
        DWORD    cSubKeys=0;               // number of subkeys
        DWORD    cbMaxSubKey;              // longest subkey size
        DWORD    cchMaxClass;              // longest class string
        DWORD    cValues;              // number of values for key
        DWORD    cchMaxValue;          // longest value name
        DWORD    cbMaxValueData;       // longest value data
        DWORD    cbSecurityDescriptor; // size of security descriptor
        FILETIME ftLastWriteTime;      // last write time

        DWORD i, retCode;

        TCHAR  achValue[MAX_VALUE_NAME];
        DWORD cchValue = MAX_VALUE_NAME;

        // Get the class name and the value count.
        retCode = RegQueryInfoKey(
            hKey,                    // key handle
            achClass,                // buffer for class name
            &cchClassName,           // size of class string
            NULL,                    // reserved
            &cSubKeys,               // number of subkeys
            &cbMaxSubKey,            // longest subkey size
            &cchMaxClass,            // longest class string
            &cValues,                // number of values for this key
            &cchMaxValue,            // longest value name
            &cbMaxValueData,         // longest value data
            &cbSecurityDescriptor,   // security descriptor
            &ftLastWriteTime);       // last write time

        // Enumerate the key values.

        if (cValues)
        {
            //qDebug() << "Number of Values: " << cValues ;

            for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++)
            {
                cchValue = MAX_VALUE_NAME;
                achValue[0] = '\0';
                retCode = RegEnumValue(hKey, i,
                    achValue,
                    &cchValue,
                    NULL,
                    NULL,
                    NULL,
                    NULL);

                if (retCode == ERROR_SUCCESS )
                {
                    QString tmp = QString::fromWCharArray(achValue);
                    //qDebug() << "Full Reg Path: " << path + "\\" + tmp;
                    list.push_back( path + "\\" + tmp);
                }
            }
        }

         RegCloseKey(hKey);
    }
    else {
         //qDebug() << "not found Value" ;
    }

    return list;
}

bool RegisteryOperations :: deleteAll (const QString& path) {
    int index = path.indexOf("\\");
    int lastIndex = path.lastIndexOf("\\");

    QString root = path.left(index);
    QString subKey = path.mid(lastIndex+1);
    QString key = path.mid(index+1, path.length() - root.length() - subKey.length()-2) ;

    qDebug() << root;
    qDebug() << key;
    qDebug() << subKey;

    HKEY hKey;
    if ( RegOpenKeyEx(getHKeyConstant(root), key.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS ) {
        //qDebug() << "open key";
        if ( SHDeleteKey(hKey, subKey.toStdWString().c_str()) != ERROR_SUCCESS ) {
            DWORD e = GetLastError();
            qDebug() << "cannot delete value : " << e ;
            return (false);
        }

        RegCloseKey(hKey);
        return (true);
    }
    else {
        qDebug() << "not found key" ;
    }

    return (false);
}

bool RegisteryOperations :: deleteRegValueObject (const  QString& path ) {
   int index = path.lastIndexOf("\\");
   QString left = path.left(index);
   QString right = path.mid(index+1);

   return deleteObject(left, right);
}

bool RegisteryOperations :: deleteObject (const  QString& path, const  QString& value) {
    int index = path.indexOf("\\");

    QString root = path.left(index);
    QString key = path.mid(index+1);

    HKEY hKey;
    if ( RegOpenKeyEx(getHKeyConstant(root), key.toStdWString().c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS ) {
        if ( RegDeleteValue(hKey, value.toStdWString().c_str()) != ERROR_SUCCESS ) {
            qDebug() << "cannot delete value: " << value;
            return (false);
         }

        RegCloseKey(hKey);
        return (true);
    }
    else {
         qDebug() << "not found key" ;
    }

    return (false);
}

HKEY RegisteryOperations :: getHKeyConstant (const QString& root) {
    for (int i=0; i<4; i++ ) {
        if ( root == hiveStrings[i] )
            return hiveKeys[i];
    }

    return NULL;
}
