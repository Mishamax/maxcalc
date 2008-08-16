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


// Defines precision of decNumber in decimal digits.
#define DECNUMDIGITS 100


#include "decNumber/decNumber.h"
#include "exception.h"

#include <QString>


//****************************************************************************
// BigDecimalFormat definition
//****************************************************************************

class BigDecimalFormat
{
public:
	// Properties
	bool engineeringFormat;
	bool lowerCaseE;
	int digitsAfterDecimalPoint;

	// Constructor
	BigDecimalFormat(
		bool engineeringFormat = false,
		bool lowerCaseE = false,
		const int digitsAfterDecimalPoint = -1);
};


// Default BigDecimal format
static const BigDecimalFormat defaultBigDecimalFormat = BigDecimalFormat();


//****************************************************************************
// BigDecimal definition
//****************************************************************************

class BigDecimal
{
public:

	// Constructors
	
	BigDecimal();
	BigDecimal(const QString & str);
	BigDecimal(const char * str);
	BigDecimal(const BigDecimal & num);
	BigDecimal(const int num);
	BigDecimal(const unsigned num);

	// Conversion functions
	
	QString toString(const BigDecimalFormat & format = defaultBigDecimalFormat) const;
	int toInt() const;
	unsigned toUInt() const;

	// Misc functions
	
	bool isZero() const;
	bool isNegative() const;
	bool isPositive() const;

	BigDecimal integer() const;
	BigDecimal fractional() const;

	// Operators

	BigDecimal operator+() const;
	BigDecimal operator-() const;

	BigDecimal operator++();
	BigDecimal operator++(int);
	BigDecimal operator--();
	BigDecimal operator--(int);

	BigDecimal operator+(const BigDecimal & num) const;
	BigDecimal operator-(const BigDecimal & num) const;
	BigDecimal operator*(const BigDecimal & num) const;
	BigDecimal operator/(const BigDecimal & num) const;
	BigDecimal operator%(const BigDecimal & num) const;

	BigDecimal operator+=(const BigDecimal & num);
	BigDecimal operator-=(const BigDecimal & num);
	BigDecimal operator*=(const BigDecimal & num);
	BigDecimal operator/=(const BigDecimal & num);
	BigDecimal operator%=(const BigDecimal & num);

	BigDecimal operator~() const;

	BigDecimal operator|(const BigDecimal & num) const;
	BigDecimal operator&(const BigDecimal & num) const;
	BigDecimal operator^(const BigDecimal & num) const;
	BigDecimal operator<<(const BigDecimal & shift) const;
	BigDecimal operator>>(const BigDecimal & shift) const;

	BigDecimal operator|=(const BigDecimal & num);
	BigDecimal operator&=(const BigDecimal & num);
	BigDecimal operator^=(const BigDecimal & num);
	BigDecimal operator<<=(const BigDecimal & shift);
	BigDecimal operator>>=(const BigDecimal & shift);

	bool operator==(const BigDecimal & num) const;
	bool operator!=(const BigDecimal & num) const;
	bool operator<(const BigDecimal & num) const;
	bool operator>(const BigDecimal & num) const;
	bool operator<=(const BigDecimal & num) const;
	bool operator>=(const BigDecimal & num) const;

	// Math functions

	static BigDecimal abs(const BigDecimal & num);
	static BigDecimal exp(const BigDecimal & num);
	static BigDecimal ln(const BigDecimal & num);
	static BigDecimal log10(const BigDecimal & num);
	static BigDecimal sqrt(const BigDecimal & num);
	static BigDecimal pow(const BigDecimal & num, const BigDecimal & power);
	static BigDecimal div(const BigDecimal & dividend, const BigDecimal & divisor);
	static BigDecimal max(const BigDecimal & n1, const BigDecimal & n2);
	static BigDecimal min(const BigDecimal & n1, const BigDecimal & n2);

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
	// Decimal number
	decNumber number;

	// Internal functions
	BigDecimal(const decNumber & num);
	static void checkContextStatus(const decContext & context);
	static int compare(const decNumber & n1, const decNumber & n2);
};

#endif // BIGDECIMAL_H