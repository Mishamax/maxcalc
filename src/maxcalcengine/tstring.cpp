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

// Local
#include "tstring.h"

namespace MaxCalcEngine {

#if defined(UNICODE)

// STL
#include <locale>
#include <cassert>

using namespace std;


typedef codecvt<wchar_t, char, mbstate_t> CodeCvt;

/*!
	Converts \a std::string to \a std::wstring using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\ingroup MaxCalcEngine
*/
codecvt_base::result stringToWideString(const string & from, wstring & to, const char * localeName)
{
	assert(localeName);

	mbstate_t state = mbstate_t();
	const char * c1 = 0;
	wchar_t * c2 = 0;
	size_t len = from.length();
	if (to.length() < len)
		to.resize(len);
	return use_facet<CodeCvt>(locale(localeName)).in(state, &from[0], &from[len], c1, &to[0], &to[len], c2);
}

/*!
	Converts \a std::wstring to \a std::string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\ingroup MaxCalcEngine
*/
codecvt_base::result wideStringToString(const wstring & from, string & to, const char * localeName)
{
	assert(localeName);

	mbstate_t state = mbstate_t();
	const wchar_t * c1 = 0;
	char * c2 = 0;
	size_t len = from.length();
	if (to.length() < len)
		to.resize(len);
	return use_facet<CodeCvt>(locale(localeName)).out(state, &from[0], &from[len], c1, &to[0], &to[len], c2);
}

/*!
	Converts \a char* string to \a wchar_t * string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\a toLength (length of \a to) must be no less than than \a fromLength (length of \a from).

	\ingroup MaxCalcEngine
*/
codecvt_base::result charToWideChar(const char * from, size_t fromLength, wchar_t * to, size_t toLength, const char * localeName)
{
	assert(fromLength <= toLength);
	assert(to);
	assert(from);
	assert(localeName);
	
	mbstate_t state = mbstate_t();
	const char * c1 = 0;
	wchar_t * c2 = 0;
	codecvt_base::result res = use_facet<CodeCvt>(locale(localeName)).in(state, &from[0], &from[fromLength], c1, &to[0], &to[toLength], c2);
	to[fromLength] = 0;
	return res;
}

/*!
	Converts \a wchar_t* string to \a char * string using specified \a locale.

	If \a locale is empty string, the default system locale is used.

	\a toLength (length of \a to) must be no less than than \a fromLength (length of \a from).

	\ingroup MaxCalcEngine
*/
codecvt_base::result wideCharToChar(const wchar_t * from, size_t fromLength, char * to, size_t toLength, const char * localeName)
{
	assert(fromLength <= toLength);
	assert(to);
	assert(from);
	assert(localeName);

	mbstate_t state = mbstate_t();
	const wchar_t * c1 = 0;
	char * c2 = 0;
	codecvt_base::result res = use_facet<CodeCvt>(locale(localeName)).out(state, &from[0], &from[fromLength], c1, &to[0], &to[toLength], c2);
	to[fromLength] = 0;
	return res;
}

#endif // #if defined(UNICODE)

void strToLower(tchar * str)
{
	for (tchar * pos = str; pos; ++pos)
		*pos = totlower(*pos);
}

void strToLower(tstring & str)
{
	for (tstring::iterator pos = str.begin(); pos != str.end(); ++pos)
		*pos = totlower(*pos);
}

void strToUpper(tchar * str)
{
	for (tchar * pos = str; pos; ++pos)
		*pos = totupper(*pos);
}

void strToUpper(tstring & str)
{
	for (tstring::iterator pos = str.begin(); pos != str.end(); ++pos)
		*pos = totupper(*pos);
}

} // namespace MaxCalcEngine
