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

#ifndef PLUGINS_H
#define PLUGINS_H

#include <QString>
#include <QMetaType>
#include <QStringList>

#include "constants.h"

struct PluginsPath
{
    QString os;
    QString location;
    QStringList patterns;
};

struct PluginsObject
{
    QString type;
    QString command;
    PluginsPath path;
};

struct PluginsItem
{
    QString name ;
    QList<PluginsObject> pluginsObject;
};

struct PluginsMetadata
{
    QString pluginsName ;
    QString category;
    QString pluginsVersion;
};

struct Plugins
{
    PluginsMetadata metaData ;
    QList<PluginsItem> pluginsItems;
};
Q_DECLARE_METATYPE(Plugins)


#endif // PLUGINS_H
