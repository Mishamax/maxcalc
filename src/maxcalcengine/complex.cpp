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

Complex::Complex()
{
	re = 0;
	im = 0;
}

Complex::Complex(const char * real, const char * imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const std::string & real, const std::string & imaginary)
{
	re = real;
	im = imaginary;
}

#if defined(UNICODE)

Complex::Complex(const wchar_t * real, const wchar_t * imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const std::wstring & real, const std::wstring & imaginary)
{
	re = real;
	im = imaginary;
}

#endif // #if defined(UNICODE)

Complex::Complex(const BigDecimal & real, const BigDecimal & imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const int real, const int imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const unsigned real, const unsigned imaginary)
{
	re = real;
	im = imaginary;
}

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

std::wstring Complex::toWideString(const ComplexFormat & format) const
{
	const std::string str = toString(format);
	std::wstring wstr;
	stringToWideString(str, wstr);
	return wstr;
}

#endif // #if defined(UNICODE)

Complex Complex::operator+() const
{
	return *this;
}

Complex Complex::operator-() const
{
	return Complex(-re, -im);
}

Complex Complex::operator+(const Complex & num) const
{
	return Complex(re + num.re, im + num.im);
}

Complex Complex::operator-(const Complex & num) const
{
	return Complex(re - num.re, im - num.im);
}

Complex Complex::operator*(const Complex & num) const
{
	return Complex(re * num.re - im * num.im, re * num.im + im * num.re);
}

Complex Complex::operator/(const Complex & num) const
{
	BigDecimal sqrt = num.re * num.re + num.im * num.im;
	if (sqrt.isZero())
		throw DivisionByZeroException();
	return Complex((re * num.re + im * num.im) / sqrt, (im * num.re - re * num.im) / sqrt);
}

bool Complex::operator==(const Complex & num) const
{
	return (re == num.re) && (im == num.im);
}

bool Complex::operator!=(const Complex & num) const
{
	return (re != num.re) || (im != num.im);
}

BigDecimal Complex::sqr(const Complex & num)
{
	// z^2 = |z|^2 = Re^2(z) + Im^2(z)
	return (num.re * num.re + num.im * num.im);
}

BigDecimal Complex::abs(const Complex & num)
{
	return BigDecimal::sqrt(sqr(num));
}

} // namespace MaxCalcEngine
