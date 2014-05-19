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

#ifndef KEYVALIDATOR_H
#define KEYVALIDATOR_H

#include <QString>

#include "constants.h"

class KeyValidator {
public:
    static bool validate (QString key);
    static Server_Query_Response internetValidator (const QString& key);
    static Server_Save_Response writeKeyToServer (const QString& key) ;

private:
    static bool lunhValidation (QString input);
};

#endif // KEYVALIDATOR_H
