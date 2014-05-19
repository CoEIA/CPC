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

#include <QtGui/QApplication>

#include "ui\mainwindow.h"
#include "util\application.h"
#include "util\utilities.h"
#include "util\commandlineparser.h"

// CPC Application have 4 different Parameters:
// CPC.exe -Scan
// CPC.exe -Shred_Quick   filepath
// CPC.exe -Shred_Safe    filepath
// CPC.exe -Shred_Through filepath

int main(int argc, char *argv[])
{
    Application instance(argc, argv);

    if (instance.sendMessage(Utilities::buildQStringFromQStringList(instance.arguments())))
        return 0;

    Application::setLanguage(QLocale::system().name());
    instance.loadTranslations(":/translations");
    instance.setLayoutDirection(QObject::tr("LTR")=="RTL" ? Qt::RightToLeft : Qt::LeftToRight);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    Utilities::loadEnglishStyle();

    QStringList args = instance.arguments();
    CommandLineParser parser(args);

    MainWindow w(parser);
    w.show();

    instance.setActivationWindow(&w);
    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
             &w, SLOT(openAnotherInstanceMessage(const QString&)));

    return instance.exec();
}
