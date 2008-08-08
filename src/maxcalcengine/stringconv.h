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

#ifndef STRINGCONV_H
#define STRINGCONV_H

// Exclude the entire StringConv class when UNICODE is not defined (it's not needed)
#ifdef UNICODE

#include <string>

using namespace std;

class StringConv
{
public:
	enum Locale { LOCAL_8BIT, UTF8 };
	
	static size_t multiByteToWideChar(const Locale locale, const char * src, const size_t srcSize,
		wchar_t * dest, const size_t destSize);
	static size_t multiByteToWideChar(const Locale locale, const char * src, wchar_t * dest, const size_t destSize);
	static size_t multiByteToWideChar(const Locale locale, const string & src, wstring & dest);

	static size_t wideCharToMultiByte(const Locale locale, const wchar_t * src, const size_t srcSize,
		char * dest, const size_t destSize);
	static size_t wideCharToMultiByte(const Locale locale, const wchar_t * src, char * dest, const size_t destSize);
	static size_t wideCharToMultiByte(const Locale locale, const wstring & src, string & dest);
};

#endif // UNICODE

#endif // STRINGCONV_H
