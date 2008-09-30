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

#include "settings.h"
#include <cassert>

//****************************************************************************
// BigDecimalFormat definition
//****************************************************************************

class BigDecimalFormat
{
public:
	// Enums
	enum NumberFormat { ScientificFormat, EngineeringFormat };
	enum ExponentCase { UpperCaseExponent, LowerCaseExponent };

	// Constructor
	BigDecimalFormat(
		const int precision = MAX_IO_PRECISION,
		const NumberFormat numberFormat = ScientificFormat,
		const ExponentCase exponentCase = UpperCaseExponent);

	// Get default format
	static BigDecimalFormat getDefault() { return BigDecimalFormat(); };

	// Accessors
	int precision() const { return m_precision; };
	void setPrecision(int precision) { assert(precision >= 1 && precision <= MAX_IO_PRECISION); m_precision = precision; };

	NumberFormat numberFormat() const { return m_numberFormat; };
	void setNumberFormat(NumberFormat numberFormat) { m_numberFormat = numberFormat; };

	ExponentCase exponentCase() const { return m_exponentCase; };
	void setExponentCase(ExponentCase exponentCase) { m_exponentCase = exponentCase; };

private:
	// Properties
	int m_precision;
	NumberFormat m_numberFormat;
	ExponentCase m_exponentCase;
};

#endif // BIGDECIMALFORMAT_H
