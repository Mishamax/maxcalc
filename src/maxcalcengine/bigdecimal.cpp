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

#include "bigdecimal.h"

// Macro for creating new decContext with default settings
#define NEW_CONTEXT(context) \
	decContext context; \
	context.digits	= DECNUMDIGITS; \
	context.emax	= DEC_MAX_EMAX; \
	context.emin	= DEC_MIN_EMIN; \
	context.round	= DEC_ROUND_HALF_UP; \
	context.traps	= 0; \
	context.status	= 0; \
	context.clamp	= 0;


/*!
	\defgroup MaxCalcEngine MaxCalc Engine
*/

/*!
	\class BigDecimal
	\brief Represents an arbitrary-precision decimal number.

	\c BigDecimal uses decNumber library as its backend.
	Precision is hard-written and defined by \a DECNUMDIGITS macros in bigdecimal.h, so
	MaxCalcEngine has to be recompiled in order to change precision.

	\ingroup MaxCalcEngine
*/


/******************************************************************************
 * Constructors
 *****************************************************************************/

/*!
	Default constructor (initializes the number with zero).
*/
BigDecimal::BigDecimal()
{
	decNumberFromInt32(&number, 0);
}

/*!
	Constructs a new instance of \c BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const QString & str)
{
	NEW_CONTEXT(context);
	// Construct number
	decNumberFromString(&number, str.toAscii().data(), &context);
	checkContextStatus(context);
	// Remove trailing zeros
	decNumberReduce(&number, &number, &context);
	checkContextStatus(context);
}

/*!
	Constructs a new instance of \c BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const char * str)
{
	NEW_CONTEXT(context);
	// Construct number
	decNumberFromString(&number, str, &context);
	checkContextStatus(context);
	// Remove trailing zeros
	decNumberReduce(&number, &number, &context);
	checkContextStatus(context);
}

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const BigDecimal & num)
{
	decNumberCopy(&number, &num.number);
}

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const decNumber & num)
{
	decNumberCopy(&number, &num);
}

/*!
	Constructs a \c BigDecimal from \c int \a num.
*/
BigDecimal::BigDecimal(const int num)
{
	decNumberFromInt32(&number, num);
}

/*!
	Constructs a \c BigDecimal from \c unsigned \a num.
*/
BigDecimal::BigDecimal(const unsigned num)
{
	decNumberFromUInt32(&number, num);
}


/******************************************************************************
 * Conversion functions
 *****************************************************************************/


/*!
	Converts this number to \c QString.
	
	\a engineeringFormat determines if scientific (default) or engineering format is used.
	
	If \a digitsAfterDecimalPoint is -1 the default precision is used.
	Raises \c InvalidOperationException if specified precision is too high and cannot be provided.
*/
QString BigDecimal::toString(bool engineeringFormat, const int digitsAfterDecimalPoint) const
{
	Q_ASSERT(digitsAfterDecimalPoint >= -1);

	NEW_CONTEXT(context);

	char * str = new char[DECNUMDIGITS + 14];
	decNumber numberToConvert = number;

	// Rescale number if needed
	decNumber result;
	if (digitsAfterDecimalPoint != -1)
	{
		decNumber exp;
		decNumberFromInt32(&exp, -digitsAfterDecimalPoint);
		decNumberRescale(&result, &number, &exp, &context);
		checkContextStatus(context);
		numberToConvert = result;
	}

	// Remove trailing zeros
	decNumberReduce(&numberToConvert, &numberToConvert, &context);
	checkContextStatus(context);

	// Make conversion
	if (engineeringFormat)
		decNumberToEngString(&numberToConvert, str);
	else
		decNumberToString(&numberToConvert, str);

	// Convert char* to QString
	QString qstr(str);
	delete str;
	return qstr;
}

/*!
	Converts this number to \c int.
	Raises \c InvalidOperationException if a number cannot be represented as int
		(does not have exponent of 0 or out-of-range).
*/
int BigDecimal::toInt() const
{
	NEW_CONTEXT(context);
	int result = decNumberToInt32(&number, &context);
	checkContextStatus(context);
	return result;
}

/*!
	Converts this number to \c unsigned.
	Raises \c InvalidOperationException if a number cannot be represented as unsigned
		(does not have exponent of 0 or out-of-range).
*/
unsigned BigDecimal::toUInt() const
{
	NEW_CONTEXT(context);
	unsigned result = decNumberToUInt32(&number, &context);
	checkContextStatus(context);
	return result;
}


/******************************************************************************
 * Misc functions
 *****************************************************************************/


/*!
	Returns \c true if the number is zero.
*/
bool BigDecimal::isZero() const
{
	return decNumberIsZero(&number);
}

/*!
	Returns \c true if the number is negative.
*/
bool BigDecimal::isNegative() const
{
	return decNumberIsNegative(&number);
}

/*!
	Returns \c true if the number is positive.
*/
bool BigDecimal::isPositive() const
{
	return (!decNumberIsZero(&number) && !decNumberIsNegative(&number));
}


/******************************************************************************
 * Operators
 *****************************************************************************/


/*!
	Returns the same \c BigDecimal as this number.
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
	Returns \c true if two numbers are equal.
*/
bool BigDecimal::operator==(const BigDecimal & num) const
{
	return (compare(number, num.number) == 0);
}

/*!
	Returns \c true if two numbers are not equal.
*/
bool BigDecimal::operator!=(const BigDecimal & num) const
{
	return (compare(number, num.number) != 0);
}

/*!
	Returns \c true if the first number is less than the second.
*/
bool BigDecimal::operator<(const BigDecimal & num) const
{
	return (compare(number, num.number) < 0);
}

/*!
	Returns \c true if the first number is more than the second.
*/
bool BigDecimal::operator>(const BigDecimal & num) const
{
	return (compare(number, num.number) > 0);
}

/*!
	Returns \c true if the first number is less or equal than the second.
*/
bool BigDecimal::operator<=(const BigDecimal & num) const
{
	return (compare(number, num.number) <= 0);
}

/*!
	Returns \c true if the first number is more or equal than the second.
*/
bool BigDecimal::operator>=(const BigDecimal & num) const
{
	return (compare(number, num.number) >= 0);
}


/******************************************************************************
 * Math functions
 *****************************************************************************/


/*!
	Returns integer part of \a num.
*/
BigDecimal BigDecimal::integer(const BigDecimal & num)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberToIntegralValue(&result, &num.number, &context);
	checkContextStatus(context);
	return result;
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


/******************************************************************************
 * Internal functions
 *****************************************************************************/


/*!
	Checks \a context.status and throws an exception is there is an error
*/
void BigDecimal::checkContextStatus(const decContext & context)
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
	Returns -1 is \a n1 is less than \a n2, 1 if more and 0 if they are equal.
*/
int BigDecimal::compare(const decNumber & n1, const decNumber & n2)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberCompare(&result, &n1, &n2, &context);
	checkContextStatus(context);
	
	return (decNumberIsZero(&result) ? 0 : (decNumberIsNegative(&result) ? -1 : 1));
}
