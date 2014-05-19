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

#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include "application.h"

QTranslator* Application::current = 0;
Translators Application::translators;

Application::Application(int& argc, char* argv[])
    : QtSingleApplication(argc, argv)
{
}

Application::~Application()
{
}

void Application::loadTranslations(const QString& dir)
{
    loadTranslations(QDir(dir));
}

void Application::loadTranslations(const QDir& dir)
{
    // <language>_<country>.qm
    QString filter = "*_*.qm";
    QDir::Filters filters = QDir::Files | QDir::Readable;
    QDir::SortFlags sort = QDir::Name;
    QFileInfoList entries = dir.entryInfoList(QStringList() << filter, filters, sort);
    foreach (QFileInfo file, entries)
    {
        // pick country and language out of the file name
        QStringList parts = file.baseName().split("_");
        QString language = parts.at(parts.count() - 2).toLower();
        QString country  = parts.at(parts.count() - 1).toUpper();

        // construct and load translator
        QTranslator* translator = new QTranslator(instance());
        if (translator->load(file.absoluteFilePath()))
        {
            QString locale = language + "_" + country;
            translators.insert(locale, translator);
        }
    }
}

const QStringList Application::availableLanguages()
{
    // the content won't get copied thanks to implicit sharing and constness
    return QStringList(translators.keys());
}

void Application::setLanguage(const QString& locale)
{
    // remove previous
    if (current)
    {
        removeTranslator(current);
    }

    // install new
    current = translators.value(locale, 0);
    if (current)
    {
        installTranslator(current);
    }
}
