/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2008 Michael Maximov (michael.maximov@gmail.com)
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

#ifndef TSTRING_H
#define TSTRING_H

#include <string>

#if defined(UNICODE)

// String literal
#define _T(x) L ## x

// tchar and tstring definitions
typedef wchar_t tchar;
typedef std::wstring tstring;

// String conversion functions
std::codecvt_base::result stringToWideString(const std::string & from, std::wstring & to, const char * localeName = "");
std::codecvt_base::result wideStringToString(const std::wstring & from, std::string & to, const char * localeName = "");

std::codecvt_base::result charToWideChar(const char * from, size_t fromLength, wchar_t * to, size_t toLength, const char * localeName = "");
std::codecvt_base::result wideCharToChar(const wchar_t * from, size_t fromLength, char * to, size_t toLength, const char * localeName = "");

#else // #if defined(UNICODE)

// String literal
#define _T(x) x

// tchar and tstring definitions
typedef char tchar;
typedef std::string tstring;

#endif // #if defined(UNICODE)

#endif // TSTRING_H
