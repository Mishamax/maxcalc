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

#include "stringconv.h"

// Exclude the entire StringConv class when UNICODE is not defined (it's not needed)
#ifdef UNICODE

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

/*!
	\defgroup MaxCalcEngine MaxCalc Engine
*/

/*!
	\class StringConv
	\brief Provides conversion between narrow (8-bit) and wide (16-bit) strings.

	Supports C null-terminated strings (conversions between \c char* and \c wchar_t*) and STL strings (\c string and \c wstring).

	\ingroup MaxCalcEngine
*/

/*!
	\enum StringConv::Locale
	\brief Defines locales for 8-bit characters.
*/
/*!
	\var StringConv::LOCAL_8BIT
	\brief Local 8-bit character encoding (ANSI on Windows).
*/
/*!
	\var StringConv::UTF8
	\brief UTF-8 character encoding.
*/


/*!
	Converts narrow string \c char* \a src with specified length \a srcSize to
	wide string \c wchar_t* \a dest with specified max length \a destSize.
	Specified \a locale is used.

	Returns number of characters written to \a dest if successful or 0 if failed.
*/
size_t StringConv::multiByteToWideChar(const Locale locale, const char * src, const size_t srcSize,
									 wchar_t * dest, const size_t destSize)
{
	_ASSERTE(src);
	_ASSERTE(dest);
	_ASSERTE(destSize >= srcSize);

#if defined(WIN32)
	return MultiByteToWideChar((locale == UTF8) ? CP_UTF8 : CP_ACP, 0, src, srcSize + 1, dest, destSize + 1);
#else
	// TODO: Linux code using iconv
#endif
}

/*!
	Converts narrow string \c char* \a src to wide string \c wchar_t* \a dest with specified
	max length \a destSize.
	Length of \a src is calculated. Specified \a locale is used.

	Returns number of characters written to \a dest if successful or 0 if failed.
*/
size_t StringConv::multiByteToWideChar(const Locale locale, const char * src, wchar_t * dest, const size_t destSize)
{
	return multiByteToWideChar(locale, src, strlen(src), dest, destSize);
}

/*!
	Converts STL narrow \c string \a src to wide \c wstring \a dest.
	Specified \a locale is used.

	Returns number of characters written to \a dest if successful or 0 if failed.
*/
size_t StringConv::multiByteToWideChar(const Locale locale, const string & src, wstring & dest)
{
	int length = src.length();
	wchar_t * tempStr = new wchar_t[length + 1];
	size_t result = multiByteToWideChar(locale, src.c_str(), length, tempStr, length);
	dest = tempStr;
	delete tempStr;
	return result;
}

/*!
	Converts wide string \c wchar_t* \a src with specified length \a srcSize to
	narrow string \c char* \a dest with specified max length \a destSize.
	Specified \a locale is used.

	Returns number of bytes written to \a dest if successful or 0 if failed.
*/
size_t StringConv::wideCharToMultiByte(const Locale locale, const wchar_t * src, const size_t srcSize,
									 char * dest, const size_t destSize)
{
	_ASSERTE(src);
	_ASSERTE(dest);
	_ASSERTE(destSize >= srcSize);

#ifdef WIN32
	return WideCharToMultiByte((locale == UTF8) ? CP_UTF8 : CP_ACP, 0, src, srcSize + 1, dest, destSize + 1, NULL, NULL);
#else
	// TODO: Linux code using iconv
#endif
}

/*!
	Converts wide string \c wchar_t* \a src to narrow string \c char* \a dest with specified
	max length \a destSize.
	Length of \a src is calculated. Specified \a locale is used.

	Returns number of bytes written to \a dest if successful or 0 if failed.
*/
size_t StringConv::wideCharToMultiByte(const Locale locale, const wchar_t * src, char * dest, const size_t destSize)
{
	return wideCharToMultiByte(locale, src, wcslen(src), dest, destSize);
}

/*!
	Converts STL wide \c wstring \a src to narrow \c string \a dest.
	Specified \a locale is used.

	Returns number of bytes written to \a dest if successful or 0 if failed.
*/
size_t StringConv::wideCharToMultiByte(const Locale locale, const wstring & src, string & dest)
{
	int length = src.length();
	char * str = new char[length + 1];
	size_t result = wideCharToMultiByte(locale, src.c_str(), length, str, length);
	dest = str;
	delete str;
	return result;
}

#endif // UNICODE
