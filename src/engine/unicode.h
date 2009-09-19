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

#ifndef UNICODE_H
#define UNICODE_H

// STL
#include <string>
#include <cctype>
#include <iostream>
#include <cwchar>

namespace MaxCalcEngine {

#ifdef _T
#undef _T
#endif

#if defined(MAXCALC_UNICODE)

///////////////////////////////////////////////////////////////////////////
// Unicode identifiers

#define _T(x) L ## x				///< String literal

typedef wchar_t tchar;				///< tchar definition
typedef std::wstring tstring;		///< tstring definition

// Character case conversion functions
#define totlower(c) towlower(c)		///< Converts \a c to lower case
#define totupper(c) towupper(c)		///< Converts \a c to upper case

// Character handling functions
#define istdigit(c) iswdigit(c)		///< Determines if \a c is a digit
#define istalpha(c) iswalpha(c)		///< Determines if \a c is a digit or letter
#define istspace(c) iswspace(c)		///< Determines if \a c is a space char

// IO
#define tcout wcout					///< Standard output stream
#define tcin wcin					///< Standard input stream
#define fgetts fgetws				///< Gets string from stream

// String functions
#define tstrcmp wcscmp
#define tmain wmain

// String conversion functions
void stringToWideString(const std::string & from,
											 std::wstring & to,
											 const char * localeName = "");
void wideStringToString(const std::wstring & from,
											 std::string & to,
											 const char * localeName = "");

void charToWideChar(const char * from,
										 size_t fromLength,
										 wchar_t * to,
										 size_t toLength,
										 const char * localeName = "");
void wideCharToChar(const wchar_t * from,
										 size_t fromLength,
										 char * to,
										 size_t toLength,
										 const char * localeName = "");

// TODO: Temp fix for Qt GUI compilation for Windows CE
#if defined(WINCE)
#define fromStdWString(str) fromWCharArray(str.c_str())
#endif

#else // #if defined(MAXCALC_UNICODE)

///////////////////////////////////////////////////////////////////////////
// Non-Unicode identifiers

#define _T(x) x						///< String literal

typedef char tchar;					///< tchar definition
typedef std::string tstring;		///< tstring definition


// Character case conversion functions
#define totlower(c) tolower(c)		///< Converts \a c to lower case
#define totupper(c) toupper(c)		///< Converts \a c to upper case

// Character handling functions
#define istdigit(c) isdigit(c)		///< Determines if \a c is a digit
#define istalpha(c) isalpha(c)		///< Determines if \a c is a digit or letter
#define istspace(c) isspace(c)		///< Determines if \a c is a space char

// IO
#define tcout cout					///< Standard output stream
#define tcin cin					///< Standard input stream
#define fgetts fgets				///< Gets string from stream

// String functions
#define tstrcmp strcmp
#define tmain main

#endif // #if defined(MAXCALC_UNICODE)


///////////////////////////////////////////////////////////////////////////
// Common functions

// Character case conversion functions
tchar *  strToLower(tchar * str);
tstring & strToLower(tstring & str);
tchar * strToUpper(tchar * str);
tstring & strToUpper(tstring & str);

// Trimming functions
tstring & ltrim(tstring & str);
tstring & rtrim(tstring & str);
tstring & trim(tstring & str);


} // namespace MaxCalcEngine

#endif // UNICODE_H
