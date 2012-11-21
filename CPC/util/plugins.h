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
