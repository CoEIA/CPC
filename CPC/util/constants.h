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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QObject>

//enum PluginsCategory{
//    WINDOWS, INTERNET,      CHAT,       MS_OFFICE, MUTLIMEDIA, FILE_SHARING,
//    EMAIL,  DESKTOP_SEARCH, TOOLBAR,    THIRD_PART
//};

const QString SCAN_PARAM = "-Scan";
const QString SHRED_QUICK_PARAM = "-Shred_Quick";
const QString SHRED_SAFE_PARAM = "-Shred_Safe";
const QString SHRED_THROUGH_PARAM = "-Shred_Through";

#define STARTUP TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
#define HKLM HKEY_LOCAL_MACHINE

enum Server_Query_Response { CLEAN_KEY, USED_KEY, ILLEGAL_KEY, UNKNOWN_KEY };
enum Server_Save_Response  { SAVING_DONE, SAVING_ERROR };
enum KeyState { KNOWN_STATE, UNKNOWN_STATE } ;

const int PLUGINS_SIZE = 12;
const int TREE_COLUMNS_SIZE = 2 ;

const double PROGRAM_VERSION = 1.1 ;
const double DATABASE_VERSION = 1.1 ;

const int FREE_DAYS = 30;

const QString PluginsCategory[PLUGINS_SIZE] = {
    QObject::tr("Windows Cache and History"),
    QObject::tr("Internet Browsing History"),
    QObject::tr("Chat and Instant Messaging History"),
    QObject::tr("MS-Office History"),
    QObject::tr("Multimedia History"),
    QObject::tr("File Sharing History"),
    QObject::tr("Email Trash"),
    QObject::tr("Desktop Search History"),
    QObject::tr("ToolBar Application History"),
    QObject::tr("Security Application History"),
    QObject::tr("Download and Upload History"),
    QObject::tr("Thrid-Party Application History")
};

const int ImageExtensionSize = 6;
const QString ImageExtension[ImageExtensionSize] = { "jpg", "jpeg", "png", "bmp", "gif", "pbm"};

//enum Type {
//    NORMAL_FILE, FOLDER, KEY, VALUE
//};

//enum Command {
//    DELETE, DELETE_ALL_BELLOW, DELETE_ALL, DELETE_MATCH, DELETE_MATCH_WITH_SUB,
//    FILL_DELETE, FILL_DELETE_ALL_BELLOW, FILL_DELETE_ALL, FILL_DELETE_MATCH, FILL_DELETE_MATCH_WITH_SUB,
//};

//enum OperatingSystem {
//    XP, SEVEN, ALL
//};

enum OVERWRITE_TYPE {
    NO_OVERWRITE, ONE_OVERWRITE, THREE_OVERWRITE, SEVEN_OVERWRITE
};

const QString TEST_INFO = "http://127.0.0.1/wajdy/info.php?serial=%1";
const QString SER_INFO = "http://licenses.coeia.edu.sa/software/cpc/info.php?serial=%1";

const QString TEST_SAVE = "http://127.0.0.1/wajdy/save.php?serial=%1&ip=%2";
const QString SER_SAVE = "http://licenses.coeia.edu.sa/software/cpc/save.php?serial=%1&ip=%2";

const QString TEST_VERSION = "http://127.0.0.1/wajdy/version.txt";
const QString SER_VERSION = "http://licenses.coeia.edu.sa/software/cpc/bin/version.txt";

const QString TEST_EXE = "http://127.0.0.1/wajdy/CPC.exe";
const QString SER_EXE = "http://licenses.coeia.edu.sa/software/cpc/bin/CPC.exe";

const QString COEIA_PLUGINS = "CoEIA-Plugins";
const QString METADATA = "Metadata";
const QString PLUGINS_NAME = "Plugin-Name";
const QString ITEMS = "Items";
const QString ITEM = "Item";
const QString ITEM_NAME = "Name" ;
const QString OBJECTS = "Objects";
const QString OBJECT = "Object";
const QString OBJECT_TYPE = "Type";
const QString OBJECT_COMMAND = "Command";
const QString OBJECT_PATH = "Path";
const QString OBJECT_OS = "OS" ;
const QString OBJECT_LOCATION = "Location";
const QString OBJECT_PATTERNS = "Patterns" ;
const QString OBJECT_PATTERN = "Pattern" ;
const QString OS_NAME = "name";

#endif // CONSTANTS_H
