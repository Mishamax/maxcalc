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


// Macro for creating new decContext with default settings
#define NEW_CONTEXT(context) \
	decContext context; \
	context.digits	= DECNUMDIGITS; \
	context.emax	= DEC_MAX_MATH; \
	context.emin	= -DEC_MAX_MATH; \
	context.round	= DEC_ROUND_HALF_UP; \
	context.traps	= 0; \
	context.status	= 0; \
	context.clamp	= 0;


/*!
	\var DECNUMDIGITS
	Defines precision of BigDecimal in decimal digits.

	The maximum precision of BigDecimal is hard-written by this macro in bigdecimal.h
	and cannot be changed at runtime.
	This macro must be defined before inclusion of decNumber.h,
	otherwise the precision will be set to default value 1.

	\sa BigDecimal
	\ingroup MaxCalcEngine
*/


//****************************************************************************
// BigDecimalFormat implementation
//****************************************************************************


/*!
	\class BigDecimalFormat
	\brief Represents format settings used to convert BigDecimal to string.

	\sa BigDecimal::toString()
	\ingroup MaxCalcEngine
*/

/*!
	\var BigDecimalFormat::engineeringFormat
	\brief Determines whether scientific (default) or engineering number format is used.

	In scientific format there is just one digit before decimal point when exponent is needed.
	In engineering format the exponent is multiple of three and there may be up to
		three digits before decimal point.
	When exponent is not needed scientific and engineering formats are the same.
*/

/*!
	\var BigDecimalFormat::lowerCaseE
	\brief Determines whether lower or upper case 'E' is used when exponent is needed.

	By default upper case 'E' is used.
*/

/*!
	\var BigDecimalFormat::digitsAfterDecimalPoint
	\brief Determines maximum number of digits shown after decimal point.

	By default it is set to -1 which means maximum possible number of digits is shown.
	If specified \a digitsAfterDecimalPoint is too high and cannot be provided BigDecimal::toString()
	will raise BigDecimal::InvalidOperationException exception.
*/

/*!
	\var defaultBigDecimalFormat
	\brief An instance of BigDecimalFormat with default settings
	(engineeringFormat = false, lowerCaseE = false, digitsAfterDecimalPoint = -1).
*/

/*!
	Contructs a new instance of BigDecimalFormat.
*/
BigDecimalFormat::BigDecimalFormat(
	bool engineeringFormat_,
	bool lowerCaseE_,
	const int digitsAfterDecimalPoint_)
{
	Q_ASSERT(digitsAfterDecimalPoint_ >= -1);

	engineeringFormat = engineeringFormat_;
	lowerCaseE = lowerCaseE_;
	digitsAfterDecimalPoint = digitsAfterDecimalPoint_;
}


//****************************************************************************
// BigDecimal implementation
//****************************************************************************


/*!
	\class BigDecimal
	\brief Represents an arbitrary-precision decimal number.

	BigDecimal uses decNumber library as its backend which is ANSI C
	implementation of General Decimal Arithmetic (http://speleotrove.com/decimal/).
	Package from Internation Components for Unicode is used (http://icu-project.org/).
	
	Maximum precision of BigDecimal is hard-written and defined by
	\a DECNUMDIGITS macro in bigdecimal.h, so MaxCalcEngine has to be
	recompiled in order to change precision.

	When converting to string, number format is specified by BigDecimalFormat class.

	\sa DECNUMDIGITS, BigDecimalFormat
	\sa http://speleotrove.com/decimal/decnumber.html
	\sa http://www.alphaworks.ibm.com/tech/decnumber
	\sa http://speleotrove.com/decimal/
	\sa http://icu-project.org/
	\ingroup MaxCalcEngine
*/

/*!
	\class BigDecimal::BigDecimalException
	\brief General BigDecimal exception.
*/
/*!
	\class BigDecimal::DivisionByZeroException
	\brief Division by zero exception. Represents \a DEC_Division_by_zero error in decNumber.
*/
/*!
	\class BigDecimal::OverflowException
	\brief Overflow exception. Represents \a DEC_Overflow error in decNumber.
*/
/*!
	\class BigDecimal::UnderflowException
	\brief Underflow exception. Represents \a DEC_Underflow error in decNumber.
*/
/*!
	\class BigDecimal::ConvertionSyntaxException
	\brief Convertion syntax exception. Represents \a DEC_Conversion_syntax error in decNumber.
*/
/*!
	\class BigDecimal::DivisionImpossibleException
	\brief Division impossible exception. Represents \a DEC_Division_impossible error in decNumber.
*/
/*!
	\class BigDecimal::DivisionUndefinedException
	\brief Division undefined exception. Represents \a DEC_Division_undefined error in decNumber.
*/
/*!
	\class BigDecimal::InsufficientStorageException
	\brief Insufficient storage exception. Represents \a DEC_Insufficient_storage error in decNumber.
*/
/*!
	\class BigDecimal::InvalidContextException
	\brief Invalid context exception. Represents \a DEC_Invalid_context error in decNumber.
*/
/*!	
	\class BigDecimal::InvalidOperationException
	\brief Invalid operation exception. Represents \a DEC_Invalid_operation error in decNumber.
*/

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
	Constructs a new instance of BigDecimal class from given \a str.
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
	Converts this number to QString using given BigDecimalFormat.
	
	The following parameters can be specified in BigDecimalFormat:

	- BigDecimalFormat::engineeringFormat -
	  determines whether scientific (default) or engineering number format is used.
	  In scientific format there is just one digit before decimal point when exponent is needed.
	  In engineering format the exponent is multiple of three and there may be up to
			three digits before decimal point.
	  When exponent is not needed scientific and engineering formats are the same.

	- BigDecimalFormat::lowerCaseE -
	  determines whether lower or upper case 'E' is used when exponent is needed.
	  By default upper case 'E' is used.

	- BigDecimalFormat::digitsAfterDecimalPoint -
	  determines maximum number of digits shown after decimal point.
	  By default it is set to -1 which means maximum possible number of digits is shown.
	  If specified \a digitsAfterDecimalPoint is too high and cannot be provided BigDecimal::toString()
	  will raise InvalidOperationException exception.

	If format is not specified, defaultBigDecimalFormat is used
	(\a engineeringFormat = false, \a lowerCaseE = false, \a digitsAfterDecimalPoint = -1).

	\sa BigDecimalFormat, InvalidOperationException
*/
QString BigDecimal::toString(const BigDecimalFormat & format) const
{
	NEW_CONTEXT(context);

	char * str = new char[DECNUMDIGITS + 14];
	decNumber numberToConvert = number;

	// Rescale number if needed
	decNumber result;
	if (format.digitsAfterDecimalPoint != -1)
	{
		decNumber exp;
		decNumberFromInt32(&exp, -format.digitsAfterDecimalPoint);
		decNumberRescale(&result, &number, &exp, &context);
		checkContextStatus(context);
		numberToConvert = result;
	}

	// Remove trailing zeros
	decNumberReduce(&numberToConvert, &numberToConvert, &context);
	checkContextStatus(context);

	// Make conversion
	if (format.engineeringFormat)
		decNumberToEngString(&numberToConvert, str);
	else
		decNumberToString(&numberToConvert, str);

	// Convert char* to QString
	QString qstr(str);
	delete str;

	// Replace 'E' with 'e' if needed
	if (format.lowerCaseE && qstr.contains('E', Qt::CaseSensitive))
		qstr.replace('E', 'e');

	return qstr;
}

/*!
	Converts this number to int.

	Raises InvalidOperationException if the number cannot be represented as int
		(does not have exponent of 0 or out-of-range).

	\sa InvalidOperationException
*/
int BigDecimal::toInt() const
{
	NEW_CONTEXT(context);
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
	NEW_CONTEXT(context);
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


//****************************************************************************
// Internal functions
//****************************************************************************


/*!
	Checks \a context.status and throws an exception is there is an error.
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
	Compares two numbers and returns -1 is \a n1 is less than \a n2, 1 if more and 0 if they are equal.
*/
int BigDecimal::compare(const decNumber & n1, const decNumber & n2)
{
	NEW_CONTEXT(context);
	decNumber result;
	decNumberCompare(&result, &n1, &n2, &context);
	checkContextStatus(context);
	
	return (decNumberIsZero(&result) ? 0 : (decNumberIsNegative(&result) ? -1 : 1));
}
