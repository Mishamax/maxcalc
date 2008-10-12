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


#ifndef BIGDECIMALFORMAT_H
#define BIGDECIMALFORMAT_H

// Local
#include "settings.h"

// STL
#include <cassert>

namespace MaxCalcEngine {

//****************************************************************************
// BigDecimalFormat definition
//****************************************************************************

class BigDecimalFormat
{
public:
	///////////////////////////////////////////////////////////////////////////
	// Enums

	/// Specifies output formats of BigDecimal number (scientific or engineering).
	enum NumberFormat { ScientificFormat, EngineeringFormat };
	/// Specifies case of 'E' letter when exponent is needed.
	enum ExponentCase { UpperCaseExponent, LowerCaseExponent };


	///////////////////////////////////////////////////////////////////////////
	// Constructors

	BigDecimalFormat(
		const int precision = MAX_IO_PRECISION,
		const NumberFormat numberFormat = ScientificFormat,
		const ExponentCase exponentCase = UpperCaseExponent);


	///////////////////////////////////////////////////////////////////////////
	// Functions

	// Get default format
	static BigDecimalFormat getDefault() { return BigDecimalFormat(); };


	///////////////////////////////////////////////////////////////////////////
	// Accessors

	/// Gets output precision of BigDecimal number.
	int precision() const { return m_precision; };
	/// Sets output precision of BigDecimal number.
	void setPrecision(int precision) { assert(precision >= 1 && precision <= MAX_IO_PRECISION); m_precision = precision; };

	/// Gets format of BigDecimal number.
	NumberFormat numberFormat() const { return m_numberFormat; };
	/// Sets format of BigDecimal number.
	void setNumberFormat(NumberFormat numberFormat) { m_numberFormat = numberFormat; };

	/// Gets case of 'E' letter in BigDecimal number when exponent is needed.
	ExponentCase exponentCase() const { return m_exponentCase; };
	/// Sets case of 'E' letter in BigDecimal number when exponent is needed.
	void setExponentCase(ExponentCase exponentCase) { m_exponentCase = exponentCase; };

private:

	///////////////////////////////////////////////////////////////////////////
	// Properties

	int m_precision;				///< Output precision of BigDecimal number.
	NumberFormat m_numberFormat;	///< Format of BigDecimal Number (scientific or engineering)
	ExponentCase m_exponentCase;	///< Case of 'E' letter in BigDecimal number when exponent is needed.
};

} // namespace MaxCalcEngine

#endif // BIGDECIMALFORMAT_H
