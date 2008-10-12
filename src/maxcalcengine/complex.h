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

#ifndef COMPLEX_H
#define COMPLEX_H

// Local
#include "bigdecimal.h"
#include "complexformat.h"

// STL
#include <exception>
#include <string>

namespace MaxCalcEngine {

//****************************************************************************
// Complex definition
//****************************************************************************

class Complex
{
public:

	/// Real part of the complex number.
	BigDecimal re;
	/// Imaginary part of the complex number.
	BigDecimal im;


	///////////////////////////////////////////////////////////////////////////
	// Constructors

	Complex();
	Complex(const char * real, const char * imaginary = "0");
	Complex(const std::string & real, const std::string & imaginary = "0");
#if defined(UNICODE)
	Complex(const wchar_t * real, const wchar_t * imaginary = L"0");
	Complex(const std::wstring & real, const std::wstring & imaginary = L"0");
#endif
	Complex(const BigDecimal & real, const BigDecimal & imaginary = 0);
	Complex(const int real, const int imaginary = 0);
	Complex(const unsigned real, const unsigned imaginary = 0);
	Complex(const Complex & num);


	///////////////////////////////////////////////////////////////////////////
	// Conversions to string

	std::string toString(const ComplexFormat & format = ComplexFormat::getDefault()) const;
#if defined(UNICODE)
	std::wstring toWideString(const ComplexFormat & format = ComplexFormat::getDefault()) const;
#endif


	///////////////////////////////////////////////////////////////////////////
	// Operators

	Complex operator+() const;
	Complex operator-() const;

	Complex operator+(const Complex & num) const;
	Complex operator-(const Complex & num) const;
	Complex operator*(const Complex & num) const;
	Complex operator/(const Complex & num) const;

	bool operator==(const Complex & num) const;
	bool operator!=(const Complex & num) const;


	///////////////////////////////////////////////////////////////////////////
	// Math functions

	static BigDecimal sqr(const Complex & num);
	static BigDecimal abs(const Complex & num);


	///////////////////////////////////////////////////////////////////////////
	// Exception classes

	/// General Complex exception.
	class ComplexException : public std::exception {}; // TODO: should it be based on std::exception or BigDecimalException?
	/// Division by zero exception.
	class DivisionByZeroException : public ComplexException {};
};

} // namespace MaxCalcEngine

#endif
