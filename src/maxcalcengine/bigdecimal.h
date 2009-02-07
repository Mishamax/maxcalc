/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2009 Michael Maximov (michael.maximov@gmail.com)
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

// Local
#include "settings.h" // settings.h must be included before decNumber.h
#include "decNumber/decNumber.h"
#include "bigdecimalformat.h"
#include "unicode.h"

namespace MaxCalcEngine {


class BigDecimal
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Math constants
	static const BigDecimal E;
	static const BigDecimal PI;


	///////////////////////////////////////////////////////////////////////////
	// Constructors
	
	BigDecimal();
	BigDecimal(const std::string & str, const BigDecimalFormat & format =
		BigDecimalFormat::getDefault());
	BigDecimal(const char * str, const BigDecimalFormat & format =
		BigDecimalFormat::getDefault());
#if defined(MAXCALC_UNICODE)
	BigDecimal(const std::wstring & str, const BigDecimalFormat & format =
		BigDecimalFormat::getDefault());
	BigDecimal(const wchar_t * str, const BigDecimalFormat & format =
		BigDecimalFormat::getDefault());
#endif
	BigDecimal(const BigDecimal & num);
	BigDecimal(const int num);
	BigDecimal(const unsigned num);
	BigDecimal(const double num);


	///////////////////////////////////////////////////////////////////////////
	// Conversion functions
	
	std::string toString(const BigDecimalFormat & format =
		BigDecimalFormat::getDefault()) const;
#if defined(MAXCALC_UNICODE)
	std::wstring toWideString(const BigDecimalFormat & format =
		BigDecimalFormat::getDefault()) const;
#endif
	inline tstring toTString(const BigDecimalFormat & format =
		BigDecimalFormat::getDefault()) const;

	int toInt() const;
	unsigned toUInt() const;


	///////////////////////////////////////////////////////////////////////////
	// Misc functions
	
	bool isZero() const;
	bool isNegative() const;
	bool isPositive() const;

	BigDecimal round() const;
	BigDecimal integer() const;
	BigDecimal fractional() const;
	BigDecimal floor() const;
	BigDecimal ceil() const;


	///////////////////////////////////////////////////////////////////////////
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


	///////////////////////////////////////////////////////////////////////////
	// Math functions

	static BigDecimal abs(const BigDecimal & num);
	static BigDecimal exp(const BigDecimal & num);
	static BigDecimal ln(const BigDecimal & num);
	static BigDecimal log10(const BigDecimal & num);
	static BigDecimal sqr(const BigDecimal & num);
	static BigDecimal sqrt(const BigDecimal & num);
	static BigDecimal pow(const BigDecimal & num, const BigDecimal & power);
	static BigDecimal div(const BigDecimal & dividend, const BigDecimal & divisor);
	static BigDecimal max(const BigDecimal & n1, const BigDecimal & n2);
	static BigDecimal min(const BigDecimal & n1, const BigDecimal & n2);
	static BigDecimal factorial(const BigDecimal & num);
	static BigDecimal sin(const BigDecimal & num);
	static BigDecimal cos(const BigDecimal & num);
	static BigDecimal tan(const BigDecimal & num);
	static BigDecimal cot(const BigDecimal & num);
	static BigDecimal arcsin(const BigDecimal & num);
	static BigDecimal arccos(const BigDecimal & num);
	static BigDecimal arctan(const BigDecimal & num);
	static BigDecimal arccot(const BigDecimal & num);


private:

	// Decimal number
	DecNumber::decNumber number_;

	///////////////////////////////////////////////////////////////////////////
	// Internal functions
	
	BigDecimal(const DecNumber::decNumber & num);
	void construct(const std::string & str, const BigDecimalFormat & format);

	static void checkContextStatus(const DecNumber::decContext & context);
	static int compare(const DecNumber::decNumber & n1,
		const DecNumber::decNumber & n2);
	static void rescale(DecNumber::decNumber & number, const int exp,
		DecNumber::decContext & context);
	
	static BigDecimal pi();
	static BigDecimal FMA(const BigDecimal & multiplier1,
		const BigDecimal & multiplier2, const BigDecimal & summand);
};


///////////////////////////////////////////////////////////////////////////
// Inline functions


/*!
	Converts this number to tstring using given BigDecimalFormat.

	If \a format is not specified, the default BigDecimalFormat is used.

	\sa BigDecimalFormat, InvalidOperationException, tstring
*/
inline tstring BigDecimal::toTString(const BigDecimalFormat & format) const
{
#if defined (MAXCALC_UNICODE)
		return toWideString(format);
#else
		return toString(format);
#endif
}


} // namespace MaxCalcEngine

#endif // BIGDECIMAL_H
