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
#include "complex.h"

namespace MaxCalcEngine {

//****************************************************************************
// Complex implementation
//****************************************************************************

	
/*!
	\class Complex
	\brief Represents a complex number.

	BigDecimal class is used to represent real and imaginary parts of the complex number.

	When converting to string, number format is specified by ComplexFormat class.

	\sa BigDecimal, ComplexFormat
	\ingroup MaxCalcEngine
*/


//****************************************************************************
// Constructors
//****************************************************************************

/*!
	Default constructor (initializes the number with zero).
*/
Complex::Complex()
{
	re = 0;
	im = 0;
}

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const char * real, const char * imaginary)
{
	re = real;
	im = imaginary;
}

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const std::string & real, const std::string & imaginary)
{
	re = real;
	im = imaginary;
}

#if defined(UNICODE)

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const wchar_t * real, const wchar_t * imaginary)
{
	re = real;
	im = imaginary;
}

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const std::wstring & real, const std::wstring & imaginary)
{
	re = real;
	im = imaginary;
}

#endif // #if defined(UNICODE)

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const BigDecimal & real, const BigDecimal & imaginary)
{
	re = real;
	im = imaginary;
}

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const int real, const int imaginary)
{
	re = real;
	im = imaginary;
}

/*!
	Constructs a new instance of Complex class from given \a real and \a imaginary parts.
*/
Complex::Complex(const unsigned real, const unsigned imaginary)
{
	re = real;
	im = imaginary;
}

/*!
	Constructs a copy of \a num.
*/
Complex::Complex(const Complex & num)
{
	re = num.re;
	im = num.im;
}

//****************************************************************************
// Conversion functions
//****************************************************************************

/*!
	Converts this number to std::string using given ComplexFormat.

	If \a format is not specified, the default ComplexFormat is used.

	\sa ComplexFormat
*/
std::string Complex::toString(const ComplexFormat & format) const
{
	std::string rePart = re.toString(format);
	std::string imPart = BigDecimal::abs(im).toString(format);
	bool isReZero = re.isZero();
	bool isImZero = im.isZero();
	bool isImNegative = im.isNegative();

	std::string result = "";

	if (isReZero && isImZero)
		return "0";

	if (!isReZero)
		result = rePart;

	if (!isImZero)
	{
		if (!isReZero && !isImNegative)
		{
			result += '+';
		}
		else if (isImNegative)
		{
			result += '-';
		}

		result += imPart + format.imaginaryOne();
	}

	return result;
}

#if defined(UNICODE)

/*!
	Converts this number to std::wstring using given ComplexFormat.

	If \a format is not specified, the default ComplexFormat is used.

	\sa ComplexFormat
*/
std::wstring Complex::toWideString(const ComplexFormat & format) const
{
	const std::string str = toString(format);
	std::wstring wstr;
	stringToWideString(str, wstr);
	return wstr;
}

#endif // #if defined(UNICODE)


//****************************************************************************
// Operators
//****************************************************************************

/*!
	Returns the same Complex number as this number.
*/
Complex Complex::operator+() const
{
	return *this;
}

/*!
	Returns negated number.
*/
Complex Complex::operator-() const
{
	return Complex(-re, -im);
}

/*!
	Adds two numbers.
*/
Complex Complex::operator+(const Complex & num) const
{
	return Complex(re + num.re, im + num.im);
}

/*!
	Subtracts two numbers.
*/
Complex Complex::operator-(const Complex & num) const
{
	return Complex(re - num.re, im - num.im);
}

/*!
	Multiplies two numbers.
*/
Complex Complex::operator*(const Complex & num) const
{
	return Complex(re * num.re - im * num.im, re * num.im + im * num.re);
}

/*!
	Divides two numbers.
*/
Complex Complex::operator/(const Complex & num) const
{
	BigDecimal sqrt = num.re * num.re + num.im * num.im;
	if (sqrt.isZero())
		throw DivisionByZeroException();
	return Complex((re * num.re + im * num.im) / sqrt, (im * num.re - re * num.im) / sqrt);
}

/*!
	Returns \a true if two numbers are equal.
*/
bool Complex::operator==(const Complex & num) const
{
	return (re == num.re) && (im == num.im);
}

/*!
	Returns \a true if two numbers are not equal.
*/
bool Complex::operator!=(const Complex & num) const
{
	return (re != num.re) || (im != num.im);
}

//****************************************************************************
// Math functions
//****************************************************************************

/*!
	Calculates square of \a num.

	sqr(num) = Re(num)^2 + Im(num)^2.
*/
BigDecimal Complex::sqr(const Complex & num)
{
	// z^2 = |z|^2 = Re^2(z) + Im^2(z)
	return (num.re * num.re + num.im * num.im);
}

/*!
	Calculates absolute value of \a num.

	abs(num) = sqrt(sqr(num)).
*/
BigDecimal Complex::abs(const Complex & num)
{
	return BigDecimal::sqrt(sqr(num));
}

} // namespace MaxCalcEngine
