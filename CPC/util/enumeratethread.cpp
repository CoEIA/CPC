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

#include "enumeratethread.h"
#include <QDir>

EnumerateThread::EnumerateThread()
{
}

void EnumerateThread::setData(const QString &filePath, const QStringList &exts) {
    this->filePath = filePath;
    this->exts = exts;
}

void EnumerateThread::run() {
    showAllBellow(filePath, exts, true);

    if ( ! stopped )
        emit finishThreadSignal();
    else
        stopped = false;
}

void EnumerateThread:: showAllBellow (const QString& path, const QStringList& regx, bool subdir){
    QDir dir(path);

    QStringList filters;
    filters << regx ;

    dir.setNameFilters(filters);

    if ( dir.exists(path) ) {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsLast)) {

            if ( info.isDir() && subdir ) {
                showAll(info.absoluteFilePath(), regx, subdir);
            }
            else if( info.isSymLink()) {
                emit addItemSignal(info.filePath());
            }
            else {
                emit addItemSignal(info.absoluteFilePath());
            }
        }
    }
}

void EnumerateThread:: showAll (const QString& path,  const QStringList& regx, bool subdir) {
    showAllBellow(path, regx, subdir);
}
