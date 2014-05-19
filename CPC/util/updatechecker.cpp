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

#include "updatechecker.h"
#include "internethandler.h"

#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QProcess>

UpdateChecker::UpdateChecker(const double programVersion, const double dataBaseVersion)
{
    this->programVersion = programVersion;
    this->dataBaseVersion = dataBaseVersion;
}

void UpdateChecker::run() {
    // read version from server
    InternetHandler *net = new InternetHandler;

    QString result = net->fecthVersion();

    if (net->hasError() ) {
        emit errorUpdateSignal();
        return ;
    }

    QString pVersion = (result.split("&")[0]).split(":")[1].trimmed();
    QString dVersion = (result.split("&")[1]).split(":")[1].trimmed();

    double pDouble = pVersion.toDouble();
    double dDouble = dVersion.toDouble();

    if ( pDouble > programVersion || dDouble > dataBaseVersion ) {
        emit finishUpdateSignal(true);  // there is an update
    }
    else {
        emit finishUpdateSignal(false); // no new update
    }
}

