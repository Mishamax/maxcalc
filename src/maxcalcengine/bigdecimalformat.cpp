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


#include "bigdecimalformat.h"
#include <cassert>


//****************************************************************************
// BigDecimalFormat implementation
//****************************************************************************


/*!
	\class BigDecimalFormat
	\brief Represents format settings used for convertion from BigDecimal to string.

	Format of BigDecimal number includes precision used for rounding, format
	(scientific or engineering) and case of 'E' letter when exponent is needed.

	Precision may not be less than 1 and more than MAX_IO_PRECISION.
	Default value is MAX_IO_PRECISION.

	Scientific format means that there is just one digit before decimal point when
	exponent is needed.
	In engineering format the exponent is multiple of three and there may be up to
	three digits before decimal point.
	When exponent is not needed scientific and engineering formats are the same.
	By default scientific format is used.

	When exponent is needed, it can be represented as lower or upper case 'E' letter.
	By default upper case 'E' is used.

	\sa BigDecimal::toString()
	\sa MAX_IO_PRECISION
	\ingroup MaxCalcEngine
*/

/*!
	\fn BigDecimalFormat::getDefault()
	\brief Returns BigDecimalFormat with default settings.

	The defaults are: precision = MAX_IO_PRECISION, numberFormat = ScientificFormat, exponentCase = UpperCaseExponent.
*/

/*!
	Contructs a new instance of BigDecimalFormat.
*/
BigDecimalFormat::BigDecimalFormat(const int precision, const NumberFormat numberFormat, const ExponentCase exponentCase)
{
	assert(precision >= 1 && precision <= MAX_IO_PRECISION);

	m_precision = precision;
	m_numberFormat = numberFormat;
	m_exponentCase = exponentCase;
}