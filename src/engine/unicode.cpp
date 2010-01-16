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


// Local
#include "unicode.h"

#if defined(MAXCALC_UNICODE)
// STL
#include <cstdlib>
#include <cassert>
#include <cwctype>
#else
#include <cctype>
#endif

namespace MaxCalcEngine {

using namespace std;

#if defined(MAXCALC_UNICODE)

/*!
    Converts \a std::string to \a std::wstring.
    \ingroup MaxCalcEngine
*/
void stringToWideString(const string & from, wstring & to)
{
    wchar_t * toStr = new wchar_t[from.length() + 1];
    mbstowcs(toStr, from.c_str(), from.length() + 1);
    to.assign(toStr);
}

/*!
    Converts \a std::wstring to \a std::string.
    \ingroup MaxCalcEngine
*/
void wideStringToString(const wstring & from, string & to)
{
    char * toStr = new char[from.length() + 1];
    wcstombs(toStr, from.c_str(), from.length() + 1);
    to.assign(toStr);
}

#endif // #if defined(MAXCALC_UNICODE)

/*!
    Converts all characters of \a str to lower case.

    Returns \a str.

    \ingroup MaxCalcEngine
*/
tchar * strToLower(tchar * str)
{
    assert(str);

    for (tchar * pos = str; pos; ++pos) {
        *pos = totlower(*pos);
    }

    return str;
}

/*!
    Converts all characters of \a str to lower case.

    Returns \a str.

    \ingroup MaxCalcEngine
*/
tstring & strToLower(tstring & str)
{
    for (tstring::iterator pos = str.begin(); pos != str.end(); ++pos) {
        *pos = totlower(*pos);
    }
    return str;
}

// Characters which are recognized as space
static const tchar * space = _T(" \t\f\v\n\r");

/*!
    Removes all white spaces at the beginning of \a str.

    Returns \a str.

    \ingroup MaxCalcEngine
*/
tstring & ltrim(tstring & str)
{
    size_t found = str.find_first_not_of(space);
    if (found != string::npos) str.erase(0, found);
    else str.clear();
    return str;
}

/*!
    Removes all white spaces at the end of \a str.

    Returns \a str.

    \ingroup MaxCalcEngine
*/
tstring & rtrim(tstring & str)
{
    size_t found = str.find_last_not_of(space);
    if (found != string::npos) str.erase(found + 1);
    else str.clear();
    return str;
}

/*!
    Removes all white spaces at the beginning and at the end of \a str.

    Returns \a str.

    \ingroup MaxCalcEngine
*/
tstring & trim(tstring & str)
{
    return rtrim(ltrim(str));
}

} // namespace MaxCalcEngine
