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
	decNumberTrim(&number);
}

/*!
	Constructs a copy of \a decimal.
*/
BigDecimal::BigDecimal(const BigDecimal & decimal)
{
	decNumberCopy(&number, &decimal.number);
	context = decimal.context;
}

BigDecimal::BigDecimal(const decNumber & num)
{
	initContext();
	decNumberCopy(&number, &num);
}

/*!
	Converts \c BigDecimal to string.
	\engineeringFormat determines if scientific (default) or engineering format is used.
	If \a digitsAfterDecimalPoint is -1 the default precision is used.
	If specified precision is too high and cannot be provided the default precision is used as well.
*/
QString BigDecimal::toString(bool engineeringFormat, const int digitsAfterDecimalPoint)
{
	_ASSERTE(digitsAfterDecimalPoint >= -1);

	char * str = new char[DECNUMDIGITS + 14];
	decNumber * numberToConvert = &number;

	if (digitsAfterDecimalPoint == -1)
	{
	}
	else
	{
		decNumber exp, result;
		decNumberFromInt32(&exp, -digitsAfterDecimalPoint);
		decNumberRescale(&result, &number, &exp, &context);
		if (decNumberIsNaN(&result))
			decNumberToString(&number, str);
		else
			decNumberToString(&result, str);
	}

	if (engineeringFormat)
		decNumberToEngString(numberToConvert, str);
	else
		decNumberToString(numberToConvert, str);

	QString qstr(str);
	delete str;
	return qstr;
}

/*!
	Adds two BigDecimals.
*/
BigDecimal BigDecimal::operator+(const BigDecimal & decimal)
{
	decNumber result;
	decNumberAdd(&result, &number, &decimal.number, &context);
	return BigDecimal(result);
}

/*!
	Subtracts two BigDecimals.
*/
BigDecimal BigDecimal::operator-(const BigDecimal & decimal)
{
	decNumber result;
	decNumberSubtract(&result, &number, &decimal.number, &context);
	return BigDecimal(result);
}

/*!
	Multiplies two BigDecimals.
*/
BigDecimal BigDecimal::operator*(const BigDecimal & decimal)
{
	decNumber result;
	decNumberMultiply(&result, &number, &decimal.number, &context);
	return BigDecimal(result);
}

/*!
	Divides two BigDecimals.
*/
BigDecimal BigDecimal::operator/(const BigDecimal & decimal)
{
	decNumber result;
	decNumberDivide(&result, &number, &decimal.number, &context);
	return BigDecimal(result);
}

/*
	Inits decContext with default settings and DECNUMDIGITS precision.
*/
void BigDecimal::initContext()
{
	decContextDefault(&context, DEC_INIT_BASE);
	context.traps = 0;
	context.digits = DECNUMDIGITS;
}
