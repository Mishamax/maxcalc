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
#include "bigdecimal.h"
#include "unicode.h"
#include "exceptions.h"

// STL
#include <clocale>
#include <cassert>
#include <sstream>

namespace MaxCalcEngine {

using namespace DecNumber;

/*!
	\defgroup MaxCalcEngine MaxCalc Engine
*/


// Macro for creating new decContext with default settings and specified precision
#define NEW_CONTEXT_WITH_SPECIFIED_PRECISION(context, precision) \
	decContext context; \
	context.digits	= precision; \
	context.emax	= DEC_MAX_MATH; \
	context.emin	= -DEC_MAX_MATH; \
	context.round	= DEC_ROUND_HALF_UP; \
	context.traps	= 0; \
	context.status	= 0; \
	context.clamp	= 0;


// Macro for creating new decContext with default settings and working precision
#define NEW_CONTEXT(context) NEW_CONTEXT_WITH_SPECIFIED_PRECISION(context, WORKING_PRECISION)

// Macro for creating new decContext with default settings and max IO precision
#define NEW_IO_CONTEXT(context) NEW_CONTEXT_WITH_SPECIFIED_PRECISION(context, MAX_IO_PRECISION)


//****************************************************************************
// BigDecimal implementation
//****************************************************************************


/*!
	\class BigDecimal
	\brief Represents an arbitrary-precision decimal number.

	BigDecimal uses decNumber library as its backend which is ANSI C
	implementation of General Decimal Arithmetic (http://speleotrove.com/decimal/).
	Package from Internation Components for Unicode is used (http://icu-project.org/).
	
	Precision settings of BigDecimal are hard-written and defined in settings.h,
	so MaxCalcEngine has to be recompiled in order to change precision.

	When converting to string, number format is specified by BigDecimalFormat class.

	\sa BigDecimalFormat
	\sa DECNUMDIGITS, FULL_PRECISION, WORKING_PRECISION, MAX_IO_PRECISION
	\sa http://speleotrove.com/decimal/decnumber.html
	\sa http://www.alphaworks.ibm.com/tech/decnumber
	\sa http://speleotrove.com/decimal/
	\sa http://icu-project.org/
	\ingroup MaxCalcEngine
*/


/*!
	E number.
*/
const BigDecimal BigDecimal::E = BigDecimal::exp(1);
/*!
	PI number.
*/
const BigDecimal BigDecimal::PI = BigDecimal::pi();


//****************************************************************************
// Constructors
//****************************************************************************

/*!
	Default constructor (initializes the number with zero).
*/
BigDecimal::BigDecimal()
{
	decNumberFromInt32(&number_, 0);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const std::string & str, const BigDecimalFormat & format)
{
	construct(str, format);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const char * str, const BigDecimalFormat & format)
{
	assert(str);

	construct(str, format);
}

#if defined(UNICODE)

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const std::wstring & str, const BigDecimalFormat & format)
{
	std::string s;
	wideStringToString(str, s);
	construct(s, format);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const wchar_t * str, const BigDecimalFormat & format)
{
	assert(str);

	std::string s;
	wideStringToString(std::wstring(str), s);
	construct(s, format);
}

#endif // #if defined(UNICODE)

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const BigDecimal & num)
{
	decNumberCopy(&number_, &num.number_);
}

/*!
	Constructs a new instance of BigDecimal class from given \a num.
*/
BigDecimal::BigDecimal(const int num)
{
	decNumberFromInt32(&number_, num);
}

/*!
	Constructs a new instance of BigDecimal class from given \a num.
*/
BigDecimal::BigDecimal(const unsigned num)
{
	decNumberFromUInt32(&number_, num);
}

/*!
	Constructs a new instance of BigDecimal class from given \a num.
*/
BigDecimal::BigDecimal(const double num)
{
	std::stringstream stream;
	stream << num;
	construct(stream.str().c_str(), BigDecimalFormat::getDefault());
}


//****************************************************************************
// Conversion functions
//****************************************************************************


/*!
	Converts this number to std::string using given BigDecimalFormat.

	If \a format is not specified, the default BigDecimalFormat is used.

	\sa BigDecimalFormat, InvalidOperationException
*/
std::string BigDecimal::toString(const BigDecimalFormat & format) const
{
	NEW_CONTEXT_WITH_SPECIFIED_PRECISION(context, format.precision());

	decNumber num;

	// Remove trailing zeros and round to needed precision
	decNumberReduce(&num, &number_, &context);
	checkContextStatus(context);

	// To prevent "-0" output
	if (decNumberIsZero(&num) && decNumberIsNegative(&num))
	{
		decNumberMinus(&num, &num, &context);
		checkContextStatus(context);
	}

	size_t size = format.precision() + 14;
	char * str = new char[size];

	// Make conversion
	decNumberToString(&num, str, size, (uint8_t)format.numberFormat());

	// Convert char* to std::string
	std::string s(str);
	
	delete[] str;

	// Replace 'E' with 'e' if needed
	if (format.exponentCase() == BigDecimalFormat::LowerCaseExponent)
	{
		size_t expPos = s.find('E');
		if (expPos != std::string::npos)
			s.replace(expPos, 1, "e");
	}

	return s;
}

#if defined(UNICODE)

/*!
	Converts this number to std::wstring using given BigDecimalFormat.
	
	If \a format is not specified, the default BigDecimalFormat is used.

	\sa BigDecimalFormat, InvalidOperationException
*/
std::wstring BigDecimal::toWideString(const BigDecimalFormat & format) const
{
	const std::string str = toString(format);
	std::wstring wstr;
	stringToWideString(str, wstr);
	return wstr;
}

#endif // #if defined(UNICODE)

/*!
	Converts this number to int.

	Raises InvalidOperationException if the number cannot be represented as int
		(does not have exponent of 0 or out-of-range).

	\sa InvalidOperationException
*/
int BigDecimal::toInt() const
{
	NEW_IO_CONTEXT(context);
	int result = decNumberToInt32(&number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Converts this number to unsigned.

	Raises InvalidOperationException if the number cannot be represented as unsigned
		(does not have exponent of 0 or out-of-range).

	\sa InvalidOperationException
*/
unsigned BigDecimal::toUInt() const
{
	NEW_IO_CONTEXT(context);
	unsigned result = decNumberToUInt32(&number_, &context);
	checkContextStatus(context);
	return result;
}


//****************************************************************************
// Misc functions
//****************************************************************************


/*!
	Returns \a true if the number is zero.
*/
bool BigDecimal::isZero() const
{
	return decNumberIsZero(&number_);
}

/*!
	Returns \a true if the number is negative.
*/
bool BigDecimal::isNegative() const
{
	return decNumberIsNegative(&number_);
}

/*!
	Returns \a true if the number is positive.
*/
bool BigDecimal::isPositive() const
{
	return (!decNumberIsZero(&number_) && !decNumberIsNegative(&number_));
}


/*!
	Rounds \a num to nearest integer.

	The function behaves like floor() if \a num > 0 and like ceil() if \a num < 0.
*/
BigDecimal BigDecimal::round() const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberToIntegralValue(&result, &number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Returns integer part of \a num (removes fractional part).
*/
BigDecimal BigDecimal::integer() const
{
	BigDecimal rounded = round();

	if (isPositive() && rounded > *this)
		--rounded;
	if (isNegative() && rounded < *this)
		++rounded;

	return rounded;
}

/*!
	Returns fractional part of \a num.

	frac(num) = num - integer(num).
	The return value is always positive.
*/
BigDecimal BigDecimal::fractional() const
{
	return BigDecimal::abs(*this - integer());
}

/*!
	Returns largest integer less than or equal to \a num (integral value of \a num).
*/
BigDecimal BigDecimal::floor() const
{
	BigDecimal integral = integer();
	return ((*this - integral).isZero() || isPositive()) ? integral : integral - 1;
}

/*!
	Returns smallest integer more than or equal to \a num.
*/
BigDecimal BigDecimal::ceil() const
{
	BigDecimal integral = integer();
	return ((*this - integral).isZero() || isNegative()) ? integral : integral + 1;
}


//****************************************************************************
// Operators
//****************************************************************************

/*!
	Returns the same BigDecimal as this number.
*/
BigDecimal BigDecimal::operator+() const
{
	return *this;
}

/*!
	Returns negated number.
*/
BigDecimal BigDecimal::operator-() const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberMinus(&result, &number_, &context);
	checkContextStatus(context);
	return result;
}

// TODO: more efficient ++ and -- implementations

/*!
	Increases this number by one (prefix increment).
*/
BigDecimal BigDecimal::operator++()
{
	return (*this = *this + 1);
}

/*!
	Increases this number by one (postfix increment).
*/
BigDecimal BigDecimal::operator++(int)
{
	BigDecimal result = *this;
	*this = *this + 1;
	return result;
}

/*!
	Decreases this number by one (prefix increment).
*/
BigDecimal BigDecimal::operator--()
{
	return (*this = *this - 1);
}

/*!
	Decreases this number by one (postfix increment).
*/
BigDecimal BigDecimal::operator--(int)
{
	BigDecimal result = *this;
	*this = *this - 1;
	return result;
}

/*!
	Adds two numbers.
*/
BigDecimal BigDecimal::operator+(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberAdd(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Subtracts two numbers.
*/
BigDecimal BigDecimal::operator-(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberSubtract(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Multiplies two numbers.
*/
BigDecimal BigDecimal::operator*(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberMultiply(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Divides two numbers.
*/
BigDecimal BigDecimal::operator/(const BigDecimal & num) const
{
	if (num.isZero())
		throw DivisionByZeroException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberDivide(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates remainder of division of two numbers.
*/
BigDecimal BigDecimal::operator%(const BigDecimal & num) const
{
	if (num.isZero())
		throw DivisionByZeroException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberRemainder(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Adds \a num to this number.
*/
BigDecimal BigDecimal::operator+=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberAdd(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Subtracts \a num from this number.
*/
BigDecimal BigDecimal::operator-=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberSubtract(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Multiplies this number by \a num.
*/
BigDecimal BigDecimal::operator*=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberMultiply(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Divides this number by \a num.
*/
BigDecimal BigDecimal::operator/=(const BigDecimal & num)
{
	if (num.isZero())
		throw DivisionByZeroException();

	NEW_CONTEXT(context);
	decNumberDivide(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates remainder of division of this number by \a num.
*/
BigDecimal BigDecimal::operator%=(const BigDecimal & num)
{
	if (num.isZero())
		throw DivisionByZeroException();

	NEW_CONTEXT(context);
	decNumberRemainder(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise invertion of this number.
*/
BigDecimal BigDecimal::operator~() const
{
	if (!fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberInvert(&result, &number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical OR of two numbers.
*/
BigDecimal BigDecimal::operator|(const BigDecimal & num) const
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberOr(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical AND of two numbers.
*/
BigDecimal BigDecimal::operator&(const BigDecimal & num) const
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberAnd(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical XOR of two numbers.
*/
BigDecimal BigDecimal::operator^(const BigDecimal & num) const
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberXor(&result, &number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Shifts this number to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<(const BigDecimal & shift) const
{
	if (!fractional().isZero() || !shift.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberShift(&result, &number_, &shift.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Shifts this number to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>(const BigDecimal & shift) const
{
	if (!fractional().isZero() || !shift.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number_, &context);
	checkContextStatus(context);
	decNumberShift(&result, &number_, &negativeShift, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical OR of this number and \a num.
*/
BigDecimal BigDecimal::operator|=(const BigDecimal & num)
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumberOr(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise logical AND of this number and \a num.
*/
BigDecimal BigDecimal::operator&=(const BigDecimal & num)
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumberAnd(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise logical XOR of this number and \a num.
*/
BigDecimal BigDecimal::operator^=(const BigDecimal & num)
{
	if (!fractional().isZero() || !num.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumberXor(&number_, &number_, &num.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Shifts this number to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<=(const BigDecimal & shift)
{
	if (!fractional().isZero() || !shift.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumberShift(&number_, &number_, &shift.number_, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Shifts this number to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>=(const BigDecimal & shift)
{
	if (!fractional().isZero() || !shift.fractional().isZero())
		throw LogicalOperationOnFractionalNumberException();

	NEW_CONTEXT(context);
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number_, &context);
	checkContextStatus(context);
	decNumberShift(&number_, &number_, &negativeShift, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Returns \a true if two numbers are equal.
*/
bool BigDecimal::operator==(const BigDecimal & num) const
{
	return (compare(number_, num.number_) == 0);
}

/*!
	Returns \a true if two numbers are not equal.
*/
bool BigDecimal::operator!=(const BigDecimal & num) const
{
	return (compare(number_, num.number_) != 0);
}

/*!
	Returns \a true if the first number is less than the second.
*/
bool BigDecimal::operator<(const BigDecimal & num) const
{
	return (compare(number_, num.number_) < 0);
}

/*!
	Returns \a true if the first number is more than the second.
*/
bool BigDecimal::operator>(const BigDecimal & num) const
{
	return (compare(number_, num.number_) > 0);
}

/*!
	Returns \a true if the first number is less or equal than the second.
*/
bool BigDecimal::operator<=(const BigDecimal & num) const
{
	return (compare(number_, num.number_) <= 0);
}

/*!
	Returns \a true if the first number is more or equal than the second.
*/
bool BigDecimal::operator>=(const BigDecimal & num) const
{
	return (compare(number_, num.number_) >= 0);
}


//****************************************************************************
// Math functions
//****************************************************************************

/*!
	Calculates absolute value of \a num.
*/
BigDecimal BigDecimal::abs(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberAbs(&result, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates exponent of \a num.
*/
BigDecimal BigDecimal::exp(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberExp(&result, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates natural logarithm of \a num.

	\a num must be > 0.
*/
BigDecimal BigDecimal::ln(const BigDecimal & num)
{
	if (num.isZero() || num.isNegative())
		throw InvalidArgumentInLogException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberLn(&result, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates base-10 logarithm of \a num.

	\a num must be > 0.
*/
BigDecimal BigDecimal::log10(const BigDecimal & num)
{
	if (num.isZero() || num.isNegative())
		throw InvalidArgumentInLogException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberLog10(&result, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates square of \a num.
*/
BigDecimal BigDecimal::sqr(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberMultiply(&result, &num.number_, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates square root of \a num.

	\a num must be >= 0.
*/
BigDecimal BigDecimal::sqrt(const BigDecimal & num)
{
	if (num.isNegative())
		throw InvalidArgumentInSqrtException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberSquareRoot(&result, &num.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Raises \a num to \a power.

	Note: 0^0 = 1.
	If \a num <= 0 then \a power must be >= 0.
*/
BigDecimal BigDecimal::pow(const BigDecimal & num, const BigDecimal & power)
{
	if (num.isZero() && power.isZero())
		return BigDecimal(1);
	if ((num.isZero() || num.isNegative()) && power.isNegative())
		throw InvalidArgumentInPowException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberPower(&result, &num.number_, &power.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates integer part of the result of dividing \a dividend by \a divisor.

	\a divisor must be not zero.
*/
BigDecimal BigDecimal::div(const BigDecimal & dividend, const BigDecimal & divisor)
{
	if (divisor.isZero())
		throw DivisionByZeroException();

	NEW_CONTEXT(context);
	decNumber result;
	decNumberDivideInteger(&result, &dividend.number_, &divisor.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Returns max of two numbers.
*/
BigDecimal BigDecimal::max(const BigDecimal & num, const BigDecimal & decimal)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberMax(&result, &num.number_, &decimal.number_, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Returns min of two numbers.
*/
BigDecimal BigDecimal::min(const BigDecimal & num, const BigDecimal & decimal)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberMin(&result, &num.number_, &decimal.number_, &context);
	checkContextStatus(context);
	return result;
}

// TODO: faster calculation of PI, Fact, Sin, Cos, etc by using decNumber functions
//		instead of BigDecimal functions and operators

// TODO: fact() - add support for non-integer and negative factorials

/*!
	Calculates factorial of \a num.

	\a num must be integer and >= 0.
*/
BigDecimal BigDecimal::factorial(const BigDecimal & num)
{
	if (num.isZero())
		return 1;
	if (num.isNegative() || !num.fractional().isZero())
		throw InvalidArgumentInFactorialException();

	BigDecimal result = 1;
	unsigned max = num.toUInt();

	for (unsigned i = 1; i <= max; ++i)
		result *= i;

	return result;
}

/*!
	Calculates sine of \a num (measured in radians).
	This function uses Taylor serie.
*/
BigDecimal BigDecimal::sin(const BigDecimal & num)
{
	BigDecimal angle = num % (PI * 2);
	BigDecimal result = angle, fraction = angle, count = 2, numerator = angle, denominator = 1;
	BigDecimal sqrNum = sqr(angle);

	while (abs(fraction) > WORKING_PRECISION_STRING)
	{
		numerator *= sqrNum;
		denominator *= FMA(count, count, count);
		count += 2;
		fraction = numerator / denominator;
		result -= fraction;

		numerator *= sqrNum;
		denominator *= FMA(count, count, count);
		count += 2;
		fraction = numerator / denominator;
		result += fraction;
	}

	NEW_IO_CONTEXT(context);
	decNumber reduced;
	decNumberReduce(&reduced, &result.number_, &context);
	if (BigDecimal::abs(reduced) < MAX_IO_PRECISION_STRING)
		result = 0;

	return result;
}

/*!
	Calculates cosine of \a num (measured in radians).
	This function uses Taylor serie.
*/
BigDecimal BigDecimal::cos(const BigDecimal & num)
{
	BigDecimal angle = num % (PI * 2);
	BigDecimal result = 1, fraction = 1, count = 1, numerator = 1, denominator = 1;
	BigDecimal sqrNum = sqr(angle);

	while (abs(fraction) > WORKING_PRECISION_STRING)
	{
		numerator *= sqrNum;
		denominator *= FMA(count, count, count);
		count += 2;
		fraction = numerator / denominator;
		result -= fraction;

		numerator *= sqrNum;
		denominator *= FMA(count, count, count);
		count += 2;
		fraction = numerator / denominator;
		result += fraction;
	}

	NEW_IO_CONTEXT(context);
	decNumber reduced;
	decNumberReduce(&reduced, &result.number_, &context);
	if (BigDecimal::abs(reduced) < MAX_IO_PRECISION_STRING)
		result = 0;

	return result;
}

/*!
	Calculates tangent of \a num (measured in radians).
	This functions calculates tan(num) as sin(num) / cos(num).
*/
BigDecimal BigDecimal::tan(const BigDecimal & num)
{
	BigDecimal angle = num % (PI * 2);
	BigDecimal cosine = cos(angle);

	if (cosine.isZero())
		throw InvalidArgumentInTanException();

	return sin(angle) / cosine;
}

/*!
	Calculates cotangent of \a num (measured in radians).
	This functions calculates cot(num) as cos(num) / sin(num).
*/
BigDecimal BigDecimal::cot(const BigDecimal & num)
{
	BigDecimal angle = num % (PI * 2);
	BigDecimal sine = sin(angle);

	if (sine.isZero())
		throw InvalidArgumentInCotException();

	return cos(angle) / sine;
}

/*!
	Calculates arcsine of \a num (measured in radians).
	This function uses formula arcsin(x) = arctan(x / sqrt(1 - x*x)).
*/
BigDecimal BigDecimal::arcsin(const BigDecimal & num)
{
	if (BigDecimal(1) == num)
		return PI / 2;
	else if (BigDecimal(-1) == num)
		return -PI / 2;

	if (abs(num) > BigDecimal(1))
		throw InvalidArgumentInArcSinException();

	return arctan(num / sqrt(-sqr(num) + 1));
}

/*!
	Calculates arccosine of \a num (measured in radians).
	This function uses formula arccos(x) = pi / 2 - arcsin(x).
*/
BigDecimal BigDecimal::arccos(const BigDecimal & num)
{
	if (abs(num) > BigDecimal(1))
		throw InvalidArgumentInArcCosException();

	return PI / 2 - arcsin(num);
}

/*!
	Calculates arctangent of \a num (measured in radians).
	This function uses Taylor serie for -0.5 <= num <= 0.5 and
	formula arctan(x) = 2 * arctan(x / (1 + sqrt(1 + x*x)) for bigger \a num.
*/
BigDecimal BigDecimal::arctan(const BigDecimal & num)
{
	if (abs(num) > BigDecimal("0.5"))
	{
		return arctan(num / (sqrt(sqr(num) + 1) + 1)) * 2;
	}
	else
	{
		BigDecimal fraction = num, result = num;
		BigDecimal numerator = num, denominator = 1;

		while (abs(fraction) > WORKING_PRECISION_STRING)
		{
			numerator *= -num * num;
			denominator += 2;
			fraction = numerator / denominator;
			result += fraction;
		}

		return result;
	}
}

/*!
	Calculates arccotangent of \a num (measured in radians).
	This function uses formula arccot(x) = pi / 2 - arctan(x).
*/
BigDecimal BigDecimal::arccot(const BigDecimal & num)
{
	return PI / 2 - arctan(num);
}


//****************************************************************************
// Internal functions
//****************************************************************************

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const DecNumber::decNumber & num)
{
	decNumberCopy(&number_, &num);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
void BigDecimal::construct(const std::string & str, const BigDecimalFormat & format)
{
	NEW_CONTEXT(context);

	std::string s = str;
	if (format.decimalSeparator() != BigDecimalFormat::PointDecimalSeparator)
	{
		size_t pos = str.find(format.decimalSeparatorChar());
		if (pos != std::string::npos)
			s.replace(pos, 1, ".");
	}

	// Construct number
	decNumberFromString(&number_, s.c_str(), &context);
	checkContextStatus(context);
	
	// Remove trailing zeros
	decNumberReduce(&number_, &number_, &context);
	checkContextStatus(context);
}

/*!
	Checks \a context.status and throws an exception is there is an error.
*/
void BigDecimal::checkContextStatus(const DecNumber::decContext & context)
{
	if (context.status & DEC_Errors)
	{
		if (context.status & DEC_Division_by_zero)
			throw DivisionByZeroException();
		else if (context.status & DEC_Overflow)
			throw OverflowException();
		else if (context.status & DEC_Underflow)
			throw UnderflowException();
		else if (context.status & DEC_Conversion_syntax)
			throw ConvertionSyntaxException();
		else if (context.status & DEC_Division_impossible)
			throw DivisionImpossibleException();
		else if (context.status & DEC_Division_undefined)
			throw DivisionUndefinedException();
		else if (context.status & DEC_Insufficient_storage)
			throw std::bad_alloc();
		else if (context.status & DEC_Invalid_context)
			throw InvalidContextException();
		else if (context.status & DEC_Invalid_operation)
			throw InvalidOperationException();
		else
			throw ArithmeticException();
	}
}

/*!
	Compares two numbers and returns -1 is \a n1 is less than \a n2, 1 if more and 0 if they are equal.
*/
int BigDecimal::compare(const DecNumber::decNumber & n1, const DecNumber::decNumber & n2)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberCompare(&result, &n1, &n2, &context);
	checkContextStatus(context);
	
	return (decNumberIsZero(&result) ? 0 : (decNumberIsNegative(&result) ? -1 : 1));
}

/*!
	Calculates PI number.

	This function uses Brent–Salamin algorithm
	(http://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm).
	Calculation continies while difference between a(n) and b(n)
	is more than calculationPrecision. calculationPrecision is
	1E-N, where N is WORKING_PRECISION.

	\sa WORKING_PRECISION
	\sa http://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm
*/
BigDecimal BigDecimal::pi()
{
	BigDecimal a = 1, a_old;
	BigDecimal b = BigDecimal(1) / sqrt(2);
	BigDecimal t = BigDecimal("0.25");
	BigDecimal p = 1;

	while (abs(a - b) > WORKING_PRECISION_STRING)
	{
		a_old = a;
		a = (a + b) / 2;
		b = sqrt(a_old * b);
		t -= p * sqr(a_old - a);
		p *= 2;
	}

	return sqr(a + b) / (BigDecimal(4) * t);
}

/*!
	Calculates multiplier1 * multiplier2 + summand
*/
BigDecimal BigDecimal::FMA(const BigDecimal & multiplier1, const BigDecimal & multiplier2, const BigDecimal & summand)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberFMA(&result, &multiplier1.number_, &multiplier2.number_, &summand.number_, &context);
	checkContextStatus(context);
	return result;
}


} // namespace MaxCalcEngine
