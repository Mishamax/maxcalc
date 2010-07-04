/*****************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2010 Michael Maximov (michael.maximov@gmail.com)
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

#include "constants.h"
#include "decNumber/decNumber.h"


/*!
    \class Constants
    \brief Defines various constants like precision and version information.
    \ingroup MaxCalcEngine
*/

/*!
    Working precision of BigDecimal in decimal digits.

    The default value is equal to DECNUMDIGITS, so MaxCalcEngine use full
    precision of decNumber.
    
    \sa BigDecimal, DECNUMDIGITS
*/
const int Constants::WORKING_PRECISION = DECNUMDIGITS;


/*!
    Maximum precision used to rounding during conversion from BigDecimal to
    string and from string to BigDecimal.

    This constant must be less than \a WORKING_PRECISION to eliminate rounding
    errors. The default value is 50.

    \sa BigDecimal::toString(), WORKING_PRECISION
*/
const int Constants::MAX_IO_PRECISION = 50;

/*!
    Default precision used to rounding during conversion from BigDecimal to
    string and from string to BigDecimal.

    This constant must be less than \a MAX_IO_PRECISION. The default value is 25.

    \sa BigDecimal::toString(), WORKING_PRECISION
*/
const int Constants::DEFAULT_IO_PRECISION = 25;

/*!
    Working precision of BigDecimal represented as string.

    The default value is 1E-WORKING_PRECISION.
    This is used internally in BigDecimal.

    \sa BigDecimal, WORKING_PRECISION
*/
const char * Constants::WORKING_PRECISION_STRING = "1E-136";

/*!
    Default precision used to rounding during conversion from BigDecimal to
    string and from string to BigDecimal represented as string.

    The default value is 1E-MAX_IO_PRECISION.
    This is used internally in BigDecimal.

    \sa BigDecimal, WORKING_PRECISION
*/
const char * Constants::MAX_IO_PRECISION_STRING = "1E-50";

/*!
    Version number.
*/
const tchar * Constants::VERSION = _T("2.0.2");
/*!
    Web site.
*/
const tchar * Constants::WEBSITE = _T("http://code.google.com/p/maxcalc/");
/*!
    Copyright.
*/
const tchar * Constants::COPYRIGHT = _T("Copyright (c) 2005-2010 Michael Maximov");

