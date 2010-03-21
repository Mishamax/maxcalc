/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2009 Michael Maximov (michael.maximov@gmail.com)
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

#ifndef VERSION_H
#define VERSION_H

#include "unicode.h"

namespace MaxCalcEngine {

/*!
    Version number.
*/
static const tchar * VERSION = _T("2.0.0");
/*!
    Version label.
*/
static const tchar * VERSION_LABEL = _T("");
/*!
    Web site.
*/
static const tchar * WEBSITE = _T("http://code.google.com/p/maxcalc/");
/*!
    Copyright.
*/
static const tchar * COPYRIGHT = _T("Copyright (c) 2005-2010 Michael Maximov");

} // namespace MaxCalcEngine

#endif // VERSION_H
