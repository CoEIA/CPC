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

#include "commandlineparser.h"

CommandLineParser::CommandLineParser(QStringList args)
{
    this->m_isValidParameter = false;
    this->m_optionName = "";

    if ( args.count() > 1 ) {
        QString option = args.at(1);

        if ( args.count() == 2 && option == SCAN_PARAM ) {
           this->m_isValidParameter = true;
           this->m_optionName = option;
        }
        else if ( args.count() == 3) {

            // check the name of parameters
            if ( option == SHRED_QUICK_PARAM ||
                     option == SHRED_SAFE_PARAM ||
                     option == SHRED_THROUGH_PARAM ) {
                QStringList paths = args.at(2).split(",");

                foreach(QString aPath, paths) {
                    this->m_filePaths.append(aPath);
                }

                this->m_optionName = option;
                this->m_isValidParameter = true;
            }
        }
    }
    else {
        this->m_isValidParameter = true;
    }
}

bool CommandLineParser::isValidParameter() const {
    return m_isValidParameter;
}

bool CommandLineParser::isScanParameter() const {
    return this->m_optionName == SCAN_PARAM;
}

bool CommandLineParser::isShredQuickParameter() const {
    return this->m_optionName == SHRED_QUICK_PARAM;
}

bool CommandLineParser::isShredSafeParameter() const{
    return this->m_optionName == SHRED_SAFE_PARAM;
}

bool CommandLineParser::isShredThroughParameter() const {
    return this->m_optionName == SHRED_THROUGH_PARAM;
}

QStringList CommandLineParser::getPaths() const{
    return this->m_filePaths;
}
