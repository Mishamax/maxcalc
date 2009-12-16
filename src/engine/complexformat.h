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


#ifndef COMPLEXFORMAT_H
#define COMPLEXFORMAT_H

// Local
#include "bigdecimalformat.h"
#include "unicode.h"

namespace MaxCalcEngine {


class ComplexFormat : public BigDecimalFormat
{
public:

    ///////////////////////////////////////////////////////////////////////////
    // Enums

    /// Determines if imaginary one is represented by 'i' or 'j'.
    enum ImaginaryOne { IMAGINARY_ONE_I, IMAGINARY_ONE_J };


    ///////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit ComplexFormat(
        const int precision = MAX_IO_PRECISION,
        const NumberFormat numberFormat = GENERAL_FORMAT,
        const ExponentCase exponentCase = UPPER_CASE_EXPONENT,
        const DecimalSeparator decimalSeparator = POINT_DECIMAL_SEPARATOR,
        const ImaginaryOne imaginaryOne = IMAGINARY_ONE_I);


    ///////////////////////////////////////////////////////////////////////////
    // Public functions

    static ComplexFormat getDefault();


    ///////////////////////////////////////////////////////////////////////////
    // Accessors

    ImaginaryOne imaginaryOne() const;
    void setImaginaryOne(const ImaginaryOne imaginaryOne);

    char imaginaryOneChar() const;
    wchar_t imaginaryOneWideChar() const;
    tchar imaginaryOneTChar() const;

private:

    ///////////////////////////////////////////////////////////////////////////
    // Properties

    ImaginaryOne mImaginaryOne;      ///< Letter used to represent imaginary one
};


} // namespace MaxCalcEngine

#endif // COMPLEXFORMAT_H
