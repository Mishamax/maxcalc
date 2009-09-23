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

#ifndef PRECISION_H
#define PRECISION_H

namespace MaxCalcEngine {

/*!
	Maximum precision of decNumber in decimal digits.

	The maximum precision of decNumber and thus BigDecimal is hard-written by
	this macro and cannot be changed at runtime.
	This macro is used in decNumber.h, so precision.h must always be included
	before decNumber.h.

	The default value is 136 to use 512 bits (32 bytes) of data
	(see decNumber definition in decNumber.h for details).

	\sa BigDecimal
	\ingroup MaxCalcEngine
*/
#define DECNUMDIGITS 136

/*!
	Working precision of BigDecimal in decimal digits.

	The default value is equal to DECNUMDIGITS, so MaxCalcEngine use full
	precision of decNumber.
	
	\sa BigDecimal, DECNUMDIGITS
	\ingroup MaxCalcEngine
*/
static const int WORKING_PRECISION = DECNUMDIGITS;


/*!
	Default precision used to rounding during conversion from BigDecimal to
	string and from string to BigDecimal.

	This constant must be less than \a WORKING_PRECISION to eliminate rounding
	errors.	The default value is 50.

	\sa BigDecimal::toString(), WORKING_PRECISION
	\ingroup MaxCalcEngine
*/
static const int MAX_IO_PRECISION = 50;



} // namespace MaxCalcEngine

#endif // PRECISION_H
