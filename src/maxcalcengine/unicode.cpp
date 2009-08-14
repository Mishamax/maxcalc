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

#if defined(MAXCALC_UNICODE)
#ifndef WINCE
#include <locale>
#else
#include <windows.h>
#endif
#endif

// Local
#include "unicode.h"

namespace MaxCalcEngine {

using namespace std;

#if defined(MAXCALC_UNICODE)

// STL
#include <clocale>
#include <cassert>

// TODO: do something with unreferenced localeName parameters under WinCE

using namespace std;

#ifndef WINCE
typedef codecvt<wchar_t, char, mbstate_t> CodeCvt;
#endif

/*!
	Converts \a std::string to \a std::wstring using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\ingroup MaxCalcEngine
*/
void stringToWideString(const string & from, wstring & to, const char * localeName)
{
	assert(localeName);

#ifndef WINCE
	mbstate_t state = mbstate_t();
	const char * c1 = 0;
	wchar_t * c2 = 0;
	size_t len = from.length();
	if (to.length() < len)
		to.resize(len);
	use_facet<CodeCvt>(locale(localeName)).in(state, &from[0], &from[len], c1, &to[0], &to[len], c2);
#else
	localeName;
	size_t length = from.length() + 1;
	wchar_t * str = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, from.c_str(), length, str, length);
	to = str;
#endif
}

/*!
	Converts \a std::wstring to \a std::string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\ingroup MaxCalcEngine
*/
void wideStringToString(const wstring & from, string & to, const char * localeName)
{
	assert(localeName);

#ifndef WINCE
	mbstate_t state = mbstate_t();
	const wchar_t * c1 = 0;
	char * c2 = 0;
	size_t len = from.length();
	if (to.length() < len)
		to.resize(len);
	use_facet<CodeCvt>(locale(localeName)).out(state, &from[0], &from[len], c1, &to[0], &to[len], c2);
#else
	localeName;
	size_t length = from.length() + 1;
	char * str = new char[length];
	WideCharToMultiByte(CP_ACP, 0, from.c_str(), length, str, length, NULL, NULL);
	to = str;
#endif
}

/*!
	Converts \a char* string to \a wchar_t * string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\a toLength (length of \a to) must be no less than than \a fromLength (length of \a from).

	\ingroup MaxCalcEngine
*/
void charToWideChar(const char * from, size_t fromLength, wchar_t * to, size_t toLength, const char * localeName)
{
	assert(fromLength <= toLength);
	assert(to);
	assert(from);
	assert(localeName);

#ifndef WINCE
	mbstate_t state = mbstate_t();
	const char * c1 = 0;
	wchar_t * c2 = 0;
	use_facet<CodeCvt>(locale(localeName)).in(state, &from[0], &from[fromLength], c1, &to[0], &to[toLength], c2);
	to[fromLength] = 0;
#else
	localeName;
	MultiByteToWideChar(CP_ACP, 0, from, fromLength, to, toLength);
#endif
}

/*!
	Converts \a wchar_t* string to \a char * string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\a toLength (length of \a to) must be no less than than \a fromLength (length of \a from).

	\ingroup MaxCalcEngine
*/
void wideCharToChar(const wchar_t * from, size_t fromLength, char * to, size_t toLength, const char * localeName)
{
	assert(fromLength <= toLength);
	assert(to);
	assert(from);
	assert(localeName);

#ifndef WINCE
	mbstate_t state = mbstate_t();
	const wchar_t * c1 = 0;
	char * c2 = 0;
	use_facet<CodeCvt>(locale(localeName)).out(state, &from[0], &from[fromLength], c1, &to[0], &to[toLength], c2);
	to[fromLength] = 0;
#else
	localeName;
	WideCharToMultiByte(CP_ACP, 0, from, fromLength, to, toLength, NULL, NULL);
#endif
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

	for (tchar * pos = str; pos; ++pos)
		*pos = totlower(*pos);

	return str;
}

/*!
	Converts all characters of \a str to lower case.

	Returns \a str.

	\ingroup MaxCalcEngine
*/
tstring & strToLower(tstring & str)
{
	for (tstring::iterator pos = str.begin(); pos != str.end(); ++pos)
		*pos = totlower(*pos);
	return str;
}

/*!
	Converts all characters of \a str to upper case.

	Returns \a str.

	\ingroup MaxCalcEngine
*/
tchar * strToUpper(tchar * str)
{
	assert(str);

	for (tchar * pos = str; pos; ++pos)
		*pos = totupper(*pos);

	return str;
}

/*!
	Converts all characters of \a str to upper case.

	Returns \a str.

	\ingroup MaxCalcEngine
*/
tstring & strToUpper(tstring & str)
{
	for (tstring::iterator pos = str.begin(); pos != str.end(); ++pos)
		*pos = totupper(*pos);
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
	if (found != string::npos)
		str.erase(0, found);
	else
		str.clear();
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
	if (found != string::npos)
		str.erase(found + 1);
	else
		str.clear();
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
