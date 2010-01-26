/******************************************************************************
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


#ifndef BIGDECIMALFORMAT_H
#define BIGDECIMALFORMAT_H

// Local
#include "precision.h"

namespace MaxCalcEngine {

/*!
    \class BigDecimalFormat
    \brief Represents format settings used for convertion from BigDecimal
    to string.

    Format of BigDecimal number includes precision used for rounding, format
    (general, scientific or engineering), case of 'E' letter when exponent
    is needed and decimal separator (dot or comma).

    Precision may not be less than 1 and more than MAX_IO_PRECISION.
    Default value is MAX_IO_PRECISION.

    In general format exponential form is used when exponent is more than 5
    by absolute    value, otherwise it is not used.
    Scientific format means that there is just one digit before decimal point
    when exponent is needed.
    In engineering format the exponent is multiple of three and there may be
    up to three digits before decimal point.
    When exponent is not needed all the formats are the same.
    By default general format is used.

    When exponent is needed, it can be represented as lower or upper case
    'E' letter.
    By default upper case 'E' is used.

    Default decimal separator is dot ('.').

    \sa BigDecimal::toString()
    \sa MAX_IO_PRECISION
    \ingroup MaxCalcEngine
*/
struct BigDecimalFormat
{
public:

    /// Specifies output formats of BigDecimal number.
    enum NumberFormat { GENERAL_FORMAT, SCIENTIFIC_FORMAT, ENGINEERING_FORMAT };
    /// Specifies case of 'E' letter when exponent is needed.
    enum ExponentCase { UPPER_CASE_EXPONENT, LOWER_CASE_EXPONENT };
    /// Specifies decimal separator.
    enum DecimalSeparator { DOT_SEPARATOR, COMMA_SEPARATOR };


    int precision;                          ///< Output precision of number.
    NumberFormat numberFormat;              ///< Format of number.
    ExponentCase exponentCase;              ///< Case of 'E' letter.
    DecimalSeparator decimalSeparator;      ///< Decimal separator.


    /// Contructs a new instance of BigDecimalFormat with given settings.
    explicit BigDecimalFormat(
        const int precision_ = MAX_IO_PRECISION,
        const NumberFormat numberFormat_ = GENERAL_FORMAT,
        const ExponentCase exponentCase_ = UPPER_CASE_EXPONENT,
        const DecimalSeparator decimalSeparator_ = DOT_SEPARATOR)
            : precision(precision_), numberFormat(numberFormat_),
              exponentCase(exponentCase_), decimalSeparator(decimalSeparator_)
    {
    }
};


} // namespace MaxCalcEngine

#endif // BIGDECIMALFORMAT_H
