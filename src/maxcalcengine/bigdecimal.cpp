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
#include "tstring.h"

// STL
#include <locale>
#include <cassert>

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


// Constructs decNumber from WORKING_PRECISION
static decNumber setCalculationPrecision()
{
	char prec[4], str[7] = "1E-";

#if _MSC_VER > 1400
	// Safe functions for MSVC 2005+ compliers
	_itoa_s(WORKING_PRECISION, prec, 4, 10);
	strcat_s(str, 7, prec);
#elif defined(WIN32)
	// Other Windows compilers
	itoa(WORKING_PRECISION, prec, 10);
	strcat(str, prec);
#else
	// Linux
	sprintf(prec, "%d", WORKING_PRECISION);
	strcat(str, prec);
#endif

	NEW_CONTEXT(context);
	decNumber precision;
	decNumberFromString(&precision, str, &context);
	return precision;
}

// Calculation precision for math functions
static const decNumber calculationPrecision = setCalculationPrecision();


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
/*!
	PI / 2 number.
*/
const BigDecimal BigDecimal::PIDiv2 = BigDecimal::PI / 2;
/*!
	PI * 2 number.
*/
const BigDecimal BigDecimal::PIMul2 = BigDecimal::PI * 2;


//****************************************************************************
// Constructors
//****************************************************************************

/*!
	Default constructor (initializes the number with zero).
*/
BigDecimal::BigDecimal()
{
	decNumberFromInt32(&number, 0);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const std::string & str)
{
	construct(str.c_str());
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const char * str)
{
	assert(str);

	construct(str);
}

#if defined(UNICODE)

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const std::wstring & str)
{
	std::string s;
	wideStringToString(str, s);
	construct(s.c_str());
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const wchar_t * str)
{
	assert(str);

	std::string s;
	wideStringToString(std::wstring(str), s);
	construct(s.c_str());
}

#endif // #if defined(UNICODE)

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const BigDecimal & num)
{
	decNumberCopy(&number, &num.number);
}

/*!
	Constructs a new instance of BigDecimal class from given \a num.
*/
BigDecimal::BigDecimal(const int num)
{
	decNumberFromInt32(&number, num);
}

/*!
	Constructs a new instance of BigDecimal class from given \a num.
*/
BigDecimal::BigDecimal(const unsigned num)
{
	decNumberFromUInt32(&number, num);
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
	decNumberReduce(&num, &number, &context);
	checkContextStatus(context);

	// To prevent "-0" output
	if (decNumberIsZero(&num) && decNumberIsNegative(&num))
	{
		decNumberMinus(&num, &num, &context);
		checkContextStatus(context);
	}

	char * str = new char[format.precision() + 14];

	// Make conversion to engineering or scientific format
	if (format.numberFormat() == BigDecimalFormat::EngineeringFormat)
		decNumberToEngString(&num, str);
	else
		decNumberToString(&num, str);

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
	int result = decNumberToInt32(&number, &context);
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
	unsigned result = decNumberToUInt32(&number, &context);
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
	return decNumberIsZero(&number);
}

/*!
	Returns \a true if the number is negative.
*/
bool BigDecimal::isNegative() const
{
	return decNumberIsNegative(&number);
}

/*!
	Returns \a true if the number is positive.
*/
bool BigDecimal::isPositive() const
{
	return (!decNumberIsZero(&number) && !decNumberIsNegative(&number));
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
	decNumberMinus(&result, &number, &context);
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
	decNumberAdd(&result, &number, &num.number, &context);
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
	decNumberSubtract(&result, &number, &num.number, &context);
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
	decNumberMultiply(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Divides two numbers.
*/
BigDecimal BigDecimal::operator/(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberDivide(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates remainder of division of two numbers.
*/
BigDecimal BigDecimal::operator%(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberRemainder(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Adds \a num to this number.
*/
BigDecimal BigDecimal::operator+=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberAdd(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Subtracts \a num from this number.
*/
BigDecimal BigDecimal::operator-=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberSubtract(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Multiplies this number by \a num.
*/
BigDecimal BigDecimal::operator*=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberMultiply(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Divides this number by \a num.
*/
BigDecimal BigDecimal::operator/=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberDivide(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates remainder of division of this number by \a num.
*/
BigDecimal BigDecimal::operator%=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberRemainder(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise invertion of this number.
*/
BigDecimal BigDecimal::operator~() const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberInvert(&result, &number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical OR of two numbers.
*/
BigDecimal BigDecimal::operator|(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberOr(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical AND of two numbers.
*/
BigDecimal BigDecimal::operator&(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberAnd(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical XOR of two numbers.
*/
BigDecimal BigDecimal::operator^(const BigDecimal & num) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberXor(&result, &number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Shifts this number to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<(const BigDecimal & shift) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberShift(&result, &number, &shift.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Shifts this number to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>(const BigDecimal & shift) const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number, &context);
	checkContextStatus(context);
	decNumberShift(&result, &number, &negativeShift, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates digit-wise logical OR of this number and \a num.
*/
BigDecimal BigDecimal::operator|=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberOr(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise logical AND of this number and \a num.
*/
BigDecimal BigDecimal::operator&=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberAnd(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Calculates digit-wise logical XOR of this number and \a num.
*/
BigDecimal BigDecimal::operator^=(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumberXor(&number, &number, &num.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Shifts this number to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<=(const BigDecimal & shift)
{
	NEW_CONTEXT(context);
	decNumberShift(&number, &number, &shift.number, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Shifts this number to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>=(const BigDecimal & shift)
{
	NEW_CONTEXT(context);
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number, &context);
	checkContextStatus(context);
	decNumberShift(&number, &number, &negativeShift, &context);
	checkContextStatus(context);
	return *this;
}

/*!
	Returns \a true if two numbers are equal.
*/
bool BigDecimal::operator==(const BigDecimal & num) const
{
	return (compare(number, num.number) == 0);
}

/*!
	Returns \a true if two numbers are not equal.
*/
bool BigDecimal::operator!=(const BigDecimal & num) const
{
	return (compare(number, num.number) != 0);
}

/*!
	Returns \a true if the first number is less than the second.
*/
bool BigDecimal::operator<(const BigDecimal & num) const
{
	return (compare(number, num.number) < 0);
}

/*!
	Returns \a true if the first number is more than the second.
*/
bool BigDecimal::operator>(const BigDecimal & num) const
{
	return (compare(number, num.number) > 0);
}

/*!
	Returns \a true if the first number is less or equal than the second.
*/
bool BigDecimal::operator<=(const BigDecimal & num) const
{
	return (compare(number, num.number) <= 0);
}

/*!
	Returns \a true if the first number is more or equal than the second.
*/
bool BigDecimal::operator>=(const BigDecimal & num) const
{
	return (compare(number, num.number) >= 0);
}


//****************************************************************************
// Math functions
//****************************************************************************


/*!
	Returns integer part of \a num.
*/
BigDecimal BigDecimal::integer() const
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberToIntegralValue(&result, &number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Returns fractional part of \a num.
	The return value is always positive.
*/
BigDecimal BigDecimal::fractional() const
{
	return abs(*this - integer());
}

/*!
	Calculates absolute value of \a num.
*/
BigDecimal BigDecimal::abs(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberAbs(&result, &num.number, &context);
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
	decNumberExp(&result, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates natural logarithm of \a num.
*/
BigDecimal BigDecimal::ln(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberLn(&result, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates base-10 logarithm of \a num.
*/
BigDecimal BigDecimal::log10(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberLog10(&result, &num.number, &context);
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
	decNumberMultiply(&result, &num.number, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates square root of \a num.
*/
BigDecimal BigDecimal::sqrt(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberSquareRoot(&result, &num.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Raises \a num to \a power.
*/
BigDecimal BigDecimal::pow(const BigDecimal & num, const BigDecimal & power)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberPower(&result, &num.number, &power.number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Calculates integer part of the result of dividing \a dividend by \a divisor.
*/
BigDecimal BigDecimal::div(const BigDecimal & dividend, const BigDecimal & divisor)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberDivideInteger(&result, &dividend.number, &divisor.number, &context);
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
	decNumberMax(&result, &num.number, &decimal.number, &context);
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
	decNumberMin(&result, &num.number, &decimal.number, &context);
	checkContextStatus(context);
	return result;
}

// TODO: faster calculation of PI, Fact, Sin, Cos, etc by using decNumber functions
//		instead of BigDecimal functions and operators

// TODO: fact() - add support for non-integer and negative factorials

/*!
	Calculates factorial of \a num.
*/
BigDecimal BigDecimal::factorial(const BigDecimal & num)
{
	if (num.isZero())
		return 1;
	if (num.isNegative() || !num.fractional().isZero())
		throw InvalidArgumentInFactorialException();

	BigDecimal result = 1;
	unsigned max = num.toUInt();

	for (unsigned i = 1; i <= max; i++)
		result *= i;

	return result;
}

/*!
	Calculates sine of \a num (measured in radians).
	This function uses Taylor serie.
*/
BigDecimal BigDecimal::sin(const BigDecimal & num)
{
	BigDecimal angle = num % PIMul2;
	BigDecimal result = angle, fraction = angle, count = 2, numerator = angle, denominator = 1;
	BigDecimal sqrNum = sqr(angle);

	while (fraction > calculationPrecision)
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
	decNumberReduce(&reduced, &result.number, &context);
	if (reduced.exponent < -MAX_IO_PRECISION)
		result = 0;

	return result;
}

/*!
	Calculates cosine of \a num (measured in radians).
	This function uses Taylor serie.
*/
BigDecimal BigDecimal::cos(const BigDecimal & num)
{
	BigDecimal angle = num % PIMul2;
	BigDecimal result = 1, fraction = 1, count = 1, numerator = 1, denominator = 1;
	BigDecimal sqrNum = sqr(angle);

	while (fraction > calculationPrecision)
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
	decNumberReduce(&reduced, &result.number, &context);
	if (reduced.exponent < -MAX_IO_PRECISION)
		result = 0;

	return result;
}

/*!
	Calculates tan of \a num (measured in radians).
	This functions calculates tan(num) as sin(num) / cos(num).
*/
BigDecimal BigDecimal::tan(const BigDecimal & num)
{
	BigDecimal angle = num % PIMul2;
	BigDecimal cosine = cos(angle);

	if (cosine.isZero())
		throw DivisionByZeroException();

	return sin(angle) / cosine;
}

/*!
	Calculates ctan of \a num (measured in radians).
	This functions calculates ctan(num) as cos(num) / sin(num).
*/
BigDecimal BigDecimal::ctan(const BigDecimal & num)
{
	BigDecimal angle = num % PIMul2;
	BigDecimal sine = sin(angle);

	if (sine.isZero())
		throw DivisionByZeroException();

	return cos(angle) / sine;
}

/*!
	Calculates arcsine of \a num (measured in radians).
	This function uses formula arcsin(x) = arctan(x / sqrt(1 - x*x)).
*/
BigDecimal BigDecimal::arcsin(const BigDecimal & num)
{
	if (num == BigDecimal(1))
		return PI / 2;
	else if (num == BigDecimal(-1))
		return -PI / 2;

	if (abs(num) > BigDecimal(1))
		throw InvalidArgumentInArcSinException();

	return arctan(num / sqrt(-sqrt(num) + 1));
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
	formula arctan(x) = 2 * arctan(x / (1 + sqrt(1 + x*x)) for bigger num.
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

		while (abs(fraction) > calculationPrecision)
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
	decNumberCopy(&number, &num);
}

/*!
	Constructs a new instance of BigDecimal class from given \a str.
*/
void BigDecimal::construct(const char * str)
{
	assert(str);

	NEW_CONTEXT(context);
	
	// Construct number
	decNumberFromString(&number, str, &context);
	checkContextStatus(context);
	
	// Remove trailing zeros
	decNumberReduce(&number, &number, &context);
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
			throw InsufficientStorageException();
		else if (context.status & DEC_Invalid_context)
			throw InvalidContextException();
		else if (context.status & DEC_Invalid_operation)
			throw InvalidOperationException();
		else
			throw BigDecimalException();
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

	This function uses Brent�Salamin algorithm
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
	BigDecimal b = BigDecimal(1) / BigDecimal::sqrt(2);
	BigDecimal t = BigDecimal("0.25");
	BigDecimal p = 1;

	while (BigDecimal::abs(a - b) > calculationPrecision)
	{
		a_old = a;
		a = (a + b) / 2;
		b = BigDecimal::sqrt(a_old * b);
		t = t - p * sqr(a_old - a);
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
	decNumberFMA(&result, &multiplier1.number, &multiplier2.number, &summand.number, &context);
	checkContextStatus(context);
	return result;
}

} // namespace MaxCalcEngine
