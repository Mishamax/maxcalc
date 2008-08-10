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

/*!
	\defgroup MaxCalcEngine MaxCalc Engine
*/

/*!
	\class BigDecimal
	\brief Represents an arbitrary-precision decimal number.

	\c BigDecimal uses decNumber library as its backend.
	Precision is hard-written and defined by DECNUMDIGITS macros in bigdecimal.h, so
	MaxCalcEngine has to be recompiled in order to change precision.

	\ingroup MaxCalcEngine
*/

/*!
	Constructs a new instance of \c BigDecimal class from given \a str.
*/
BigDecimal::BigDecimal(const QString & str)
{
	initContext();
	decNumberFromString(&number, str.toAscii().data(), &context);
	checkContextStatus();
	decNumberReduce(&number, &number, &context);
	checkContextStatus();
}

/*!
	Constructs a copy of \a decimal.
*/
BigDecimal::BigDecimal(const BigDecimal & decimal)
{
	decNumberCopy(&number, &decimal.number);
	context = decimal.context;
}

/*!
	Constructs a copy of \a num.
*/
BigDecimal::BigDecimal(const decNumber & num)
{
	initContext();
	decNumberCopy(&number, &num);
}

/*!
	Constructs a BigDecimal from \c int \a num.
*/
BigDecimal::BigDecimal(const int num)
{
	initContext();
	decNumberFromInt32(&number, num);
}

/*!
	Constructs a BigDecimal from \c unsigned \a num.
*/
BigDecimal::BigDecimal(const unsigned num)
{
	initContext();
	decNumberFromUInt32(&number, num);
}

/*!
	Converts \c BigDecimal to string.
	\a engineeringFormat determines if scientific (default) or engineering format is used.
	If \a digitsAfterDecimalPoint is -1 the default precision is used.
	If specified precision is too high and cannot be provided the default precision is used as well.
*/
QString BigDecimal::toString(bool engineeringFormat, const int digitsAfterDecimalPoint)
{
	Q_ASSERT(digitsAfterDecimalPoint >= -1);

	char * str = new char[DECNUMDIGITS + 14];
	decNumber * numberToConvert = &number;

	// Rescale number if needed
	decNumber result;
	if (digitsAfterDecimalPoint != -1)
	{
		decNumber exp;
		decNumberFromInt32(&exp, -digitsAfterDecimalPoint);
		decNumberRescale(&result, &number, &exp, &context);
		checkContextStatus();
		numberToConvert = &result;
	}

	decNumberReduce(&number, &number, &context);
	checkContextStatus();

	if (engineeringFormat)
		decNumberToEngString(numberToConvert, str);
	else
		decNumberToString(numberToConvert, str);

	QString qstr(str);
	delete str;
	return qstr;
}

/*!
	Converts BigDecimal to \c int.
	Raises \c InvalidOperationException if a number cannot be represented as int
		(does not have exponent of 0 or out-of-range).
*/
int BigDecimal::toInt()
{
	int result = decNumberToInt32(&number, &context);
	checkContextStatus();
	return result;
}

/*!
	Converts BigDecimal to \c unsigned.
	Raises \c InvalidOperationException if a number cannot be represented as int
		(does not have exponent of 0 or out-of-range).
*/
int BigDecimal::toUInt()
{
	int result = decNumberToUInt32(&number, &context);
	checkContextStatus();
	return result;
}

/*!
	Returns \c true if the number is zero.
*/
bool BigDecimal::isZero()
{
	return decNumberIsZero(&number);
}

/*!
	Returns \c true if the number is negative.
*/
bool BigDecimal::isNegative()
{
	return decNumberIsNegative(&number);
}

/*!
	Returns \c true if the number is positive.
*/
bool BigDecimal::isPositive()
{
	return (!decNumberIsZero(&number) && !decNumberIsNegative(&number));
}

/*!
	Returns the same BigDecimal.
*/
BigDecimal BigDecimal::operator+()
{
	return *this;
}

/*!
	Returns negated BigDecimal.
*/
BigDecimal BigDecimal::operator-()
{
	decNumber result;
	decNumberMinus(&result, &number, &context);
	checkContextStatus();
	return result;
}

// TODO: more efficient ++ and -- implementations

/*!
	Increases BigDecimal by one (prefix increment).
*/
BigDecimal BigDecimal::operator++()
{
	return (*this = *this + 1);
}

/*!
	Increases BigDecimal by one (postfix increment).
*/
BigDecimal BigDecimal::operator++(int)
{
	BigDecimal result = *this;
	*this = *this + 1;
	return result;
}

/*!
	Decreases BigDecimal by one (prefix increment).
*/
BigDecimal BigDecimal::operator--()
{
	return (*this = *this - 1);
}

/*!
	Decreases BigDecimal by one (postfix increment).
*/
BigDecimal BigDecimal::operator--(int)
{
	BigDecimal result = *this;
	*this = *this - 1;
	return result;
}

/*!
	Adds two BigDecimals.
*/
BigDecimal BigDecimal::operator+(const BigDecimal & decimal)
{
	decNumber result;
	decNumberAdd(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Subtracts two BigDecimals.
*/
BigDecimal BigDecimal::operator-(const BigDecimal & decimal)
{
	decNumber result;
	decNumberSubtract(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Multiplies two BigDecimals.
*/
BigDecimal BigDecimal::operator*(const BigDecimal & decimal)
{
	decNumber result;
	decNumberMultiply(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Divides two BigDecimals.
*/
BigDecimal BigDecimal::operator/(const BigDecimal & decimal)
{
	decNumber result;
	decNumberDivide(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates remainder of division of two BigDecimals.
*/
BigDecimal BigDecimal::operator%(const BigDecimal & decimal)
{
	decNumber result;
	decNumberRemainder(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Adds two BigDecimals.
*/
BigDecimal BigDecimal::operator+=(const BigDecimal & decimal)
{
	decNumberAdd(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Subtracts two BigDecimals.
*/
BigDecimal BigDecimal::operator-=(const BigDecimal & decimal)
{
	decNumberSubtract(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Multiplies two BigDecimals.
*/
BigDecimal BigDecimal::operator*=(const BigDecimal & decimal)
{
	decNumberMultiply(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Divides two BigDecimals.
*/
BigDecimal BigDecimal::operator/=(const BigDecimal & decimal)
{
	decNumberDivide(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Calculates remainder of division of two BigDecimals.
*/
BigDecimal BigDecimal::operator%=(const BigDecimal & decimal)
{
	decNumberRemainder(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Calculates digit-wise invertion of BigDecimal.
*/
BigDecimal BigDecimal::operator~()
{
	decNumber result;
	decNumberInvert(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates digit-wise logical OR of two BigDecimals.
*/
BigDecimal BigDecimal::operator|(const BigDecimal & decimal)
{
	decNumber result;
	decNumberOr(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates digit-wise logical AND of two BigDecimals.
*/
BigDecimal BigDecimal::operator&(const BigDecimal & decimal)
{
	decNumber result;
	decNumberAnd(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates digit-wise logical XOR of two BigDecimals.
*/
BigDecimal BigDecimal::operator^(const BigDecimal & decimal)
{
	decNumber result;
	decNumberXor(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Shifts \a this to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<(const BigDecimal & shift)
{
	decNumber result;
	decNumberShift(&result, &number, &shift.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Shifts \a this to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>(const BigDecimal & shift)
{
	decNumber result;
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number, &context);
	decNumberShift(&result, &number, &negativeShift, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates digit-wise logical OR of two BigDecimals.
*/
BigDecimal BigDecimal::operator|=(const BigDecimal & decimal)
{
	decNumberOr(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Calculates digit-wise logical AND of two BigDecimals.
*/
BigDecimal BigDecimal::operator&=(const BigDecimal & decimal)
{
	decNumberAnd(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Calculates digit-wise logical XOR of two BigDecimals.
*/
BigDecimal BigDecimal::operator^=(const BigDecimal & decimal)
{
	decNumberXor(&number, &number, &decimal.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Shifts \a this to the left by \a shift.
*/
BigDecimal BigDecimal::operator<<=(const BigDecimal & shift)
{
	decNumberShift(&number, &number, &shift.number, &context);
	checkContextStatus();
	return *this;
}

/*!
	Shifts \a this to the right by \a shift.
*/
BigDecimal BigDecimal::operator>>=(const BigDecimal & shift)
{
	decNumber negativeShift;
	decNumberMinus(&negativeShift, &shift.number, &context);
	decNumberShift(&number, &number, &negativeShift, &context);
	checkContextStatus();
	return *this;
}

/*!
	Returns \c true if two BigDecimals are equal.
*/
BigDecimal BigDecimal::operator==(const BigDecimal & decimal)
{
	return (compare(decimal) == 0);
}

/*!
	Returns \c true if two BigDecimals are not equal.
*/
BigDecimal BigDecimal::operator!=(const BigDecimal & decimal)
{
	return (compare(decimal) != 0);
}

/*!
	Returns \c true if \a this number is less than \a decimal.
*/
BigDecimal BigDecimal::operator<(const BigDecimal & decimal)
{
	return (compare(decimal) < 0);
}

/*!
	Returns \c true if \a this number is more than \a decimal.
*/
BigDecimal BigDecimal::operator>(const BigDecimal & decimal)
{
	return (compare(decimal) > 0);
}

/*!
	Returns \c true if \a this number is less or equal than \a decimal.
*/
BigDecimal BigDecimal::operator<=(const BigDecimal & decimal)
{
	return (compare(decimal) <= 0);
}

/*!
	Returns \c true if \a this number is more or equal than \a decimal.
*/
BigDecimal BigDecimal::operator>=(const BigDecimal & decimal)
{
	return (compare(decimal) >= 0);
}

/*!
	Returns integer part of the number.
*/
BigDecimal BigDecimal::toIntegral()
{
	decNumber result;
	decNumberToIntegralValue(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates absolute value.
*/
BigDecimal BigDecimal::abs()
{
	decNumber result;
	decNumberAbs(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates exponent.
*/
BigDecimal BigDecimal::exp()
{
	decNumber result;
	decNumberExp(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates natural logarithm.
*/
BigDecimal BigDecimal::ln()
{
	decNumber result;
	decNumberLn(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates base-10 logarithm.
*/
BigDecimal BigDecimal::log10()
{
	decNumber result;
	decNumberLog10(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates square root.
*/
BigDecimal BigDecimal::sqrt()
{
	decNumber result;
	decNumberSquareRoot(&result, &number, &context);
	checkContextStatus();
	return result;
}

/*!
	Raises \a this BigDecimal to \a power.
*/
BigDecimal BigDecimal::pow(const BigDecimal & power)
{
	decNumber result;
	decNumberPower(&result, &number, &power.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Calculates integer part of the result of dividing \a this by \a decimal.
*/
BigDecimal BigDecimal::div(const BigDecimal & decimal)
{
	decNumber result;
	decNumberDivideInteger(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Returns max of \a this and \a decimal.
*/
BigDecimal BigDecimal::max(const BigDecimal & decimal)
{
	decNumber result;
	decNumberMax(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Returns min of \a this and \a decimal.
*/
BigDecimal BigDecimal::min(const BigDecimal & decimal)
{
	decNumber result;
	decNumberMin(&result, &number, &decimal.number, &context);
	checkContextStatus();
	return result;
}

/*!
	Inits decContext with default settings and DECNUMDIGITS precision.
*/
void BigDecimal::initContext()
{
	// Init context with default settings (max 9-digit exponents, 0.5 rounds up)
	decContextDefault(&context, DEC_INIT_BASE);
	// Disable traps
	context.traps = 0;
	// Set precision
	context.digits = DECNUMDIGITS;
}

/*!
	Checks \a decContext.status and throws an exception is there is an error
*/
void BigDecimal::checkContextStatus()
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
	context.status = 0;
}

/*!
	Returns -1 is \a this number is less than \a decimal, 1 if more and 0 if they are equal.
*/
int BigDecimal::compare(const BigDecimal & decimal)
{
	decNumber result;
	decNumberCompare(&result, &number, &decimal.number, &context);
	checkContextStatus();
	
	return (decNumberIsZero(&result) ? 0 : (decNumberIsNegative(&result) ? -1 : 1));
}
