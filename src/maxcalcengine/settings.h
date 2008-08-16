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

#ifndef SETTINGS_H
#define SETTINGS_H


/*!
	Working precision of BigDecimal in decimal digits.

	The maximum precision of BigDecimal is hard-written by this macro and cannot be changed at runtime.
	This macro is used in decNumber.h, so settings.h must always be included before decNumber.h.

	The default value is 94 to use 512 bits (32 bytes) of data
	(see decNumber definition in decNumber.h for details).

	\sa BigDecimal
	\ingroup MaxCalcEngine
*/
#define DECNUMDIGITS 94


/*!
	Precision of calculations.
	
	This constant is used in math functions in BigDecimal class and should match to \a DECNUMDIGITS macro.
	The default value is "1E-94".

	\sa BigDecimal, DECNUMDIGITS
	\ingroup MaxCalcEngine
*/
static const char * calculationPrecision = "1E-94";

/*!
	Precision of PI calculation in BigDecimal::pi() function.

	This variable must have value enough to calculate PI using Brent–Salamin algorithm
	(http://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm) with \a DECNUMDIGITS precision.
	The default value is "1E-43" which is enough to calculate PI with about 100 digits precision.

	\sa BigDecimal::pi(), DECNUMDIGITS
	\sa http://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_algorithm
	\ingroup MaxCalcEngine
*/
static const char * piCalculationPrecision = "1E-43";

/*!
	Default precision used to rounding during conversion from BigDecimal to string.

	This constant should be less than \a DECNUMDIGITS.
	The default value is 90.

	\sa BigDecimal::toString(), DECNUMDIGITS
	\ingroup MaxCalcEngine
*/
static const int defaultOutputPrecision = 90;


#endif // SETTINGS_H
