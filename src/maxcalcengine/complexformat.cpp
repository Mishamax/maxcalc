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


#include "complexformat.h"


//****************************************************************************
// ComplexFormat implementation
//****************************************************************************


/*!
	\class ComplexFormat
	\brief Represents format settings used for convertion from Complex to string.

	ComplexFormat inherits BigDecimalFormat which is used to format real and
	imaginary parts of the number.

	Character which represents imaginary one can be specified.
	By default 'i' is used.

	\sa Complex::toString(), BigDecimalFormat
	\ingroup MaxCalcEngine
*/

/*!
	\fn ComplexFormat::getDefault()
	\brief Returns ComplexFormat with default settings.
	
	The defaults are: imaginaryOne = 'i', precision = MAX_IO_PRECISION,
	numberFormat = ScientificFormat, exponentCase = UpperCaseExponent.
*/

/*!
	Constructs a new instance of ComplexFormat.
*/
ComplexFormat::ComplexFormat(const char imaginaryOne, const int precision,
	const NumberFormat numberFormat, const ExponentCase exponentCase)
	: BigDecimalFormat(precision, numberFormat, exponentCase)
{
	m_imaginaryOne = imaginaryOne;
}
