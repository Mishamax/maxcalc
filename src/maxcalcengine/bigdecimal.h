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

#ifndef BIGDECIMAL_H
#define BIGDECIMAL_H

/*!
	Defines precision of BigDecimal in decimal digits.
	MaxCalcEngine has to be recompiled in order to change precision.
*/
#define DECNUMDIGITS 100

#include "decNumber/decNumber.h"
#include "exception.h"

#include <QString>


class BigDecimal
{
public:

	// Constructors
	BigDecimal(const QString & str);
	BigDecimal(const BigDecimal & decimal);
	BigDecimal(const decNumber & num);
	BigDecimal(const int num);
	BigDecimal(const unsigned num);

	// Conversion functions
	QString toString(bool engineeringFormat = false, const int digitsAfterDecimalPoint = -1);
	int toInt();
	int toUInt();

	// Operators
	BigDecimal operator+();
	BigDecimal operator-();

	BigDecimal operator++();
	BigDecimal operator++(int);
	BigDecimal operator--();
	BigDecimal operator--(int);

	BigDecimal operator+(const BigDecimal & decimal);
	BigDecimal operator-(const BigDecimal & decimal);
	BigDecimal operator*(const BigDecimal & decimal);
	BigDecimal operator/(const BigDecimal & decimal);
	BigDecimal operator%(const BigDecimal & decimal);

	BigDecimal operator+=(const BigDecimal & decimal);
	BigDecimal operator-=(const BigDecimal & decimal);
	BigDecimal operator*=(const BigDecimal & decimal);
	BigDecimal operator/=(const BigDecimal & decimal);
	BigDecimal operator%=(const BigDecimal & decimal);

	BigDecimal operator~();

	BigDecimal operator|(const BigDecimal & decimal);
	BigDecimal operator&(const BigDecimal & decimal);
	BigDecimal operator^(const BigDecimal & decimal);
	BigDecimal operator<<(const BigDecimal & shift);
	BigDecimal operator>>(const BigDecimal & shift);

	BigDecimal operator|=(const BigDecimal & decimal);
	BigDecimal operator&=(const BigDecimal & decimal);
	BigDecimal operator^=(const BigDecimal & decimal);
	BigDecimal operator<<=(const BigDecimal & shift);
	BigDecimal operator>>=(const BigDecimal & shift);

	BigDecimal operator==(const BigDecimal & decimal);
	BigDecimal operator!=(const BigDecimal & decimal);
	BigDecimal operator<(const BigDecimal & decimal);
	BigDecimal operator>(const BigDecimal & decimal);
	BigDecimal operator<=(const BigDecimal & decimal);
	BigDecimal operator>=(const BigDecimal & decimal);

	// Misc functions
	bool isZero();
	bool isNegative();
	bool isPositive();

	BigDecimal toIntegral();
	BigDecimal abs();
	BigDecimal exp();
	BigDecimal ln();
	BigDecimal log10();
	BigDecimal sqrt();

	BigDecimal pow(const BigDecimal & power);
	BigDecimal div(const BigDecimal & decimal);
	BigDecimal max(const BigDecimal & decimal);
	BigDecimal min(const BigDecimal & decimal);

	// Exception classes

	class BigDecimalException : public Exception {};
	class DivisionByZeroException : public BigDecimalException {};
	class OverflowException : public BigDecimalException {};
	class UnderflowException : public BigDecimalException {};
	class ConvertionSyntaxException : public BigDecimalException {};
	class DivisionImpossibleException : public BigDecimalException {};
	class DivisionUndefinedException : public BigDecimalException {};
	class InsufficientStorageException : public BigDecimalException {};
	class InvalidContextException : public BigDecimalException {};
	class InvalidOperationException : public BigDecimalException {};

private:
	decNumber number;
	decContext context;

	// Internal functions
	void initContext();
	void checkContextStatus();
	int compare(const BigDecimal & decimal);
};

#endif
