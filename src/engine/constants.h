/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2010 Michael Maximov (michael.maximov@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *****************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "unicode.h"

class Constants
{
public:
    static const int WORKING_PRECISION;
    static const int MAX_IO_PRECISION;
    static const int DEFAULT_IO_PRECISION;
    static const char * WORKING_PRECISION_STRING;
    static const char * MAX_IO_PRECISION_STRING;
    static const tchar * VERSION;
    static const tchar * WEBSITE;
    static const tchar * COPYRIGHT;
};

#endif // CONSTANTS_H
