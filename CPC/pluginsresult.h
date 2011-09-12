#ifndef PLUGINSRESULT_H
#define PLUGINSRESULT_H

#include <QString>
#include <QList>
#include <QMetaType>
#include <QPair>

struct PluginsResultItems {
    QString itemName;
    QList<QPair<QString, QList<QString> > > result;
};

struct PluginsResult {
    QString pluginsName;
    QList<PluginsResultItems> results;
};
Q_DECLARE_METATYPE(PluginsResult)

#endif // PLUGINSRESULT_H
