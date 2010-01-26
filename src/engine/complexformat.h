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


#ifndef COMPLEXFORMAT_H
#define COMPLEXFORMAT_H

// Local
#include "bigdecimalformat.h"
#include "unicode.h"

namespace MaxCalcEngine {


/*!
    \class ComplexFormat
    \brief Represents format settings used for convertion from Complex to
    string.

    ComplexFormat inherits BigDecimalFormat which is used to format real and
    imaginary parts of the number.

    Character which represents imaginary one can be specified.
    By default 'i' is used.

    \sa Complex::toString(), BigDecimalFormat
    \ingroup MaxCalcEngine
*/
struct ComplexFormat : public BigDecimalFormat
{
    /// Determines if imaginary one is represented by 'i' or 'j'.
    enum ImaginaryOne { IMAGINARY_ONE_I, IMAGINARY_ONE_J };

    ImaginaryOne imaginaryOne;      ///< Letter used to represent imaginary one.

    /// Constructs a new instance of ComplexFormat.
    explicit ComplexFormat(
        const int precision_ = MAX_IO_PRECISION,
        const NumberFormat numberFormat_ = GENERAL_FORMAT,
        const ExponentCase exponentCase_ = UPPER_CASE_EXPONENT,
        const DecimalSeparator decimalSeparator_ = DOT_SEPARATOR,
        const ImaginaryOne imaginaryOne_ = IMAGINARY_ONE_I)
            : BigDecimalFormat(precision_, numberFormat_, exponentCase_,
                               decimalSeparator_), imaginaryOne(imaginaryOne_)
    {
    }

    /// Returns imaginary one as a character ('i' or 'j').
    char imaginaryOneChar() const
    {
        return (imaginaryOne == IMAGINARY_ONE_I) ? 'i' : 'j';
    }

    /// Returns imaginary one as a character ('i' or 'j').
    tchar imaginaryOneTChar() const
    {
        return (imaginaryOne == IMAGINARY_ONE_I) ? _T('i') : _T('j');
    }
};


} // namespace MaxCalcEngine

#endif // COMPLEXFORMAT_H
