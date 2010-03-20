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

#ifndef UNICODE_H
#define UNICODE_H

// STL
#include <string>

namespace MaxCalcEngine {

#ifdef _T
#undef _T
#endif

#if defined(MAXCALC_UNICODE)

///////////////////////////////////////////////////////////////////////////
// Unicode identifiers

#define _T(x) L ## x                    ///< String literal

// Type definitions
typedef wchar_t tchar;
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
typedef std::wostream tostream;

// Character case conversion functions
#define totlower(c) towlower(c)         ///< Converts \a c to lower case
#define totupper(c) towupper(c)         ///< Converts \a c to upper case

// Character handling functions
#define istdigit(c) iswdigit(c)         ///< Determines if \a c is a digit
#define istalpha(c) iswalpha(c)         ///< Determines if \a c is a digit or letter
#define istspace(c) iswspace(c)         ///< Determines if \a c is a space char

// IO
#define tcout std::wcout                ///< Standard output stream
#define tcin std::wcin                  ///< Standard input stream

// String functions
#define tstrcmp wcscmp


// String conversion functions
std::wstring stringToWideString(const std::string & from);
std::string wideStringToString(const std::wstring & from);

#else // #if defined(MAXCALC_UNICODE)

///////////////////////////////////////////////////////////////////////////
// Non-Unicode identifiers

#define _T(x) x                         ///< String literal

// Type definitions
typedef char tchar;
typedef std::string tstring;
typedef std::stringstream tstringstream;
typedef std::ostream tostream;


// Character case conversion functions
#define totlower(c) tolower(c)          ///< Converts \a c to lower case
#define totupper(c) toupper(c)          ///< Converts \a c to upper case

// Character handling functions
#define istdigit(c) isdigit(c)          ///< Determines if \a c is a digit
#define istalpha(c) isalpha(c)          ///< Determines if \a c is a digit or letter
#define istspace(c) isspace(c)          ///< Determines if \a c is a space char

// IO
#define tcout cout                      ///< Standard output stream
#define tcin cin                        ///< Standard input stream

// String functions
#define tstrcmp strcmp

#endif // #if defined(MAXCALC_UNICODE)


///////////////////////////////////////////////////////////////////////////
// Common functions

// Character case conversion functions
tchar *  strToLower(tchar * str);
tstring & strToLower(tstring & str);

// Trimming functions
tstring & ltrim(tstring & str);
tstring & rtrim(tstring & str);
tstring & trim(tstring & str);


///////////////////////////////////////////////////////////////////////////
// Localization


#if defined(MAXCALC_GETTEXT)

#if defined(_WIN32)
#include "../intl_win/libintl.h"
#else
#include <libintl.h>
#endif

#if defined(MAXCALC_UNICODE)
#define _(str) stringToWideString(gettext(str)).c_str()
#else
#define _(str) gettext(str)
#endif

#else // defined(MAXCALC_GETTEXT)

#define _(str) _T(str)

#endif // defined(MAXCALC_GETTEXT)

// Formatting function
tstring format(const tstring & str, const tstring * arg, ...);

} // namespace MaxCalcEngine

#endif // UNICODE_H
