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
