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

#ifndef PLUGINSPARSER_H
#define PLUGINSPARSER_H

#include <QtXml/QXmlStreamReader>
#include "plugins.h"

class PluginsParser
{
public:
    PluginsParser(Plugins* aPlugins, const QString& os);

public:
    bool readFile(const QString& fileName);

private:
    void skipUnknownElement();
    void readItemsElement ();
    void readItemElement(PluginsItem& item);
    void readObjectsElement (PluginsItem& item);
    void readObjectElement(PluginsObject& object);
    void readPathElement(PluginsPath& path);
    void readOSElement(PluginsPath& path);
    void readPatternsElement(PluginsPath& path);

private:
    QXmlStreamReader reader;
    Plugins* plugins ;
    QString os;
};

#endif // PLUGINSPARSER_H
