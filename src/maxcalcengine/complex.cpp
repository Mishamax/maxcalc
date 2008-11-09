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
#include "exceptions.h"

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


/*!
	Imaginary one.
*/
const Complex Complex::i = Complex(0, 1);

#pragma region Constructors

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

#pragma endregion

#pragma region Conversion functions

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

#pragma endregion

#pragma region Operators

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
	return (num.re == re) && (num.im == im);
}

/*!
	Returns \a true if two numbers are not equal.
*/
bool Complex::operator!=(const Complex & num) const
{
	return (re != num.re) || (im != num.im);
}

#pragma endregion

#pragma region Misc functions

//****************************************************************************
// Misc functions
//****************************************************************************

/*!
	Returns true if this is zero.
*/
bool Complex::isZero() const
{
	return re.isZero() && im.isZero();
}

#pragma endregion

#pragma region Math functions

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

/*!
	Calculates argument of \a num.

	arg(num) = arccos(re(z) / |z|).
*/
BigDecimal Complex::arg(const Complex & num)
{
	if (num.im.isZero())
		return (num.re >= 0) ? 0 : BigDecimal::PI;

	BigDecimal arg = BigDecimal::arccos(num.re / abs(num));
	return (num.im >= 0) ? arg : -arg;
}

/*!
	Calculates natural logarithm of \a num.

	Ln(num) = ln(|num|) + i*arg(num) + 2*Pi*k*i (k = 0).
*/
Complex Complex::ln(const Complex & num)
{
	if (num.im.isZero() && num.re >= 0)
		return BigDecimal::ln(num.re);
	return Complex(0, arg(num)) + BigDecimal::ln(abs(num));
}

/*!
	Calculates exponent of \a num.

	exp(num) = exp(re(num) + i*im(num)) = exp(re(num)) * exp(i*im(num)) =
	exp(re(z)) * (cos(im(z)) + i*sin(im(z))).
*/
Complex Complex::exp(const Complex & num)
{
	if (num.im.isZero())
		return BigDecimal::exp(num.re);
	return Complex(BigDecimal::cos(num.im), BigDecimal::sin(num.im)) * BigDecimal::exp(num.re);
}

/*!
	Calculates \a num raised in \a power (num^power).

	pow(num, power) = exp(power * ln(num))
*/
Complex Complex::pow(const Complex & num, const Complex & power)
{
	if (num.im.isZero() && power.im.isZero() && num.re >= 0 && power.re >= 0)
		return BigDecimal::pow(num.re, power.re);
	// for pow(0, x) - ln(0) will return negative infinity
	if (num.isZero())
		return 0;
	return exp(power * ln(num));	
}

/*!
	Calculates square root of \a num.

	sqrt(num) = pow(num, 0.5)
*/
Complex Complex::sqrt(const Complex & num)
{
	return pow(num, Complex("0.5"));
}

/*!
	Calculates sine of \a num.

	sin(num) = (exp(i*num) - exp(-i*num)) / 2
*/
Complex Complex::sin(const Complex & num)
{
	if (num.im.isZero())
		return BigDecimal::sin(num.re);
	return (exp(i * num) - exp(-i * num)) / 2;
}

/*!
	Calculates cosine of \a num.

	cos(num) = (exp(i*num) + exp(-i*num)) / 2
*/
Complex Complex::cos(const Complex & num)
{
	if (num.im.isZero())
		return BigDecimal::cos(num.re);
	return (exp(i * num) + exp(-i * num)) / 2;
}

/*!
	Calculates arcsine of \a num.

	arcsin(num) = -i * ln(i * (num + sqrt(num^2 - 1)))
	
	We use square root with minus sign to correctly
	calculate arcsin for real arguments -1..1.
	If we use +sqrt(), arcsin(-1..1) will be in range 0..Pi.
	With -sqrt() it will be -Pi/2..Pi/2 as needed.
*/
Complex Complex::arcsin(const Complex & num)
{
	if (num.im.isZero() && num.re >= -1 && num.re <= 1)
		return BigDecimal::arcsin(num.re);
	return -i * ln(i * (num - sqrt(num * num - 1)));
}

/*!
	Calculates arccosine of \a num.

	arccos(num) = -i * ln(num + sqrt(num^2 - 1))
	
	We use +sqrt() to achieve range 0..Pi (see arcsin() description for explanation)
*/
Complex Complex::arccos(const Complex & num)
{
	if (num.im.isZero() && num.re >= -1 && num.re <= 1)
		return BigDecimal::arccos(num.re);
	return -i * ln(num + sqrt(num * num - 1));
}

/*!
	Calculates arctangent of \a num.

	arctan(num) = (i / 2) * ln((i + num) / (i - num))
	
	We calculate logarithm as: ln(num) = ln(|num|) + i*arg(num) with k = 0
	(for arctan() it will work and we will have correct range for real arguments -
	see arccot() description for explanation)
*/
Complex Complex::arctan(const Complex & num)
{
	// TODO: handle positive infinite when re(num) == 0 and abs(im(a) + im(i)) == 0
	if (num.im.isZero())
		return BigDecimal::arctan(num.re);
	return (i / 2) * ln((i + num) / (i - num));
}

/*!
	Calculates arccotangent of \a num.

	arccot(a) = (i / 2) * ln((num - i) / (num + i))

	This formula doesn't work correctly for real arguments.
	Maybe we should calculate logarithm as Ln(z) = ln(r) + i*arg(z) + 2*Pi*k*i
	with correct 'k' but it's simpler to use formula arccot(num) = Pi/2 - arctan(num).
	This works fine for any argument.
*/
Complex Complex::arccot(const Complex & num)
{
	// TODO: check this implementation again (see description)
	return -arctan(num) + BigDecimal::PI / 2;
}

/*!
	Calculates hyperbolical sine of \a num.

	sinh(num) = (exp(num) - exp(-num)) / 2
*/
Complex Complex::sinh(const Complex & num)
{
	// TODO: maybe add sinh() and cosh() to BigDecimal
//	if (num.im.isZero())
//		return BigDecimal::sinh(num.re);
	return (exp(num) - exp(-num)) / 2;
}

/*!
	Calculates hyperbolical cosine of \a num.

	cosh(num) = (exp(num) + exp(-num)) / 2
*/
Complex Complex::cosh(const Complex & num)
{
//	if (num.im.isZero())
//		return BigDecimal::cosh(num.re);
	return (exp(num) + exp(-num)) / 2;
}

/*!
	Calculates hyperbolical arcsine of \a num.

	arcsinh(num) = ln(num + sqrt(num^2 + 1))
*/
Complex Complex::arcsinh(const Complex & num)
{
	return ln(num + sqrt(num * num + 1));
}

/*!
	Calculates hyperbolical arccosine of \a num.

	arccosh(num) = ln(num + sqrt(num^2 - 1))
*/
Complex Complex::arccosh(const Complex & num)
{
	return ln(num + sqrt(num * num - 1));
}

/*!
	Calculates hyperbolical arctangent of \a num.

	arctanh(num) = ln((1 + num) / (1 - num)) / 2
*/
Complex Complex::arctanh(const Complex & num)
{
	return ln((num + 1) / (-num + 1)) / 2;
}

/*!
	Calculates hyperbolical arccotangent of \a num.

	arccoth(num) = ln((num + 1) / (num - 1)) / 2
*/
Complex Complex::arccoth(const Complex & num)
{
	return ln((num + 1) / (num - 1)) / 2;
}

#pragma endregion

} // namespace MaxCalcEngine
