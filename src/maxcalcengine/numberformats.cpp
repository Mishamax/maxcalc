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


#include "numberformats.h"


//****************************************************************************
// BigDecimalFormat implementation
//****************************************************************************


/*!
	\class BigDecimalFormat
	\brief Represents format settings used for convertion from BigDecimal to string.

	\sa BigDecimal::toString()
	\ingroup MaxCalcEngine
*/

/*!
	\var BigDecimalFormat::precision
	\brief Determines precision used for rounding.

	This value may not be more than MAX_IO_PRECISION.
	By default it is set to MAX_IO_PRECISION.

	\sa MAX_IO_PRECISION
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
	An instance of BigDecimalFormat with default settings
	(precision = MAX_IO_PRECISION, engineeringFormat = false, lowerCaseE = false).
*/
const BigDecimalFormat BigDecimalFormat::defaultBigDecimalFormat = BigDecimalFormat();

/*!
	Contructs a new instance of BigDecimalFormat.
*/
BigDecimalFormat::BigDecimalFormat(const int precision_, const bool engineeringFormat_, const bool lowerCaseE_)
{
	Q_ASSERT(precision_ >= 1 && precision_ <= MAX_IO_PRECISION);

	precision = precision_;
	engineeringFormat = engineeringFormat_;
	lowerCaseE = lowerCaseE_;
}

/*!
	Returns BigDecimalFormat with default settings.

	The defaults are: precision = MAX_IO_PRECISION, engineeringFormat = false, lowerCaseE = false.
*/
BigDecimalFormat BigDecimalFormat::getDefault()
{
	return defaultBigDecimalFormat;
}



//****************************************************************************
// ComplexFormat implementation
//****************************************************************************


/*!
	\class ComplexFormat
	\brief Represents format settings used for convertion from Complex to string.

	\sa Complex::toString()
	\ingroup MaxCalcEngine
*/

/*!
	\var ComplexFormat::iBeforeImaginaryPart
	\brief Determines whether imaginary one will be set before or after imaginary part.

	By default imaginary one is set after imaginary part.
*/

/*!
	\var ComplexFormat::multiplyBetweenIAndImaginaryPart
	\brief Determines whether there will be multiplication sign between imaginary one
	and imaginary part.

	By default there is no multiplication sign.
*/

/*!
	\var ComplexFormat::spacesAroundSignes
	\brief Determines whether there will be set spaces around '+' or '-' sign
	between real and imaginary parts.

	By default there is no spaces.
*/

/*!
	\var ComplexFormat::imaginaryOne
	\brief Determines what character will be used to represent imaginary one.

	By default 'i' is used.
*/

/*!
	An instance of ComplexFormat with default settings
	(precision = MAX_IO_PRECISION, engineeringFormat = false, lowerCaseE = false).
*/
const ComplexFormat ComplexFormat::defaultComplexFormat = ComplexFormat();

/*!
	Constructs a new instance of ComplexFormat.
*/
ComplexFormat::ComplexFormat(const bool iBeforeImaginaryPart_, const bool multiplyBetweenIAndImaginaryPart_,
	const bool spacesAroundSignes_, const QChar imaginaryOne_, const int precision_,
	const bool engineeringFormat_, const bool lowerCaseE_)
	: BigDecimalFormat(precision_, engineeringFormat_, lowerCaseE_)
{
	iBeforeImaginaryPart = iBeforeImaginaryPart_;
	multiplyBetweenIAndImaginaryPart = multiplyBetweenIAndImaginaryPart_;
	spacesAroundSignes = spacesAroundSignes_;
	imaginaryOne = imaginaryOne_;
}

/*!
	Returns ComplexFormat with default settings.
	
	The defaults are: iBeforeImaginaryPart = false, multiplyBetweenIAndImaginaryPart = false,
	spacesAroundSignes = false, imaginaryOne = 'i', precision = MAX_IO_PRECISION,
	engineeringFormat = false, lowerCaseE = false.
*/
ComplexFormat ComplexFormat::getDefault()
{
	return defaultComplexFormat;
}
