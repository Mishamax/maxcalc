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


#ifndef BIGDECIMALFORMAT_H
#define BIGDECIMALFORMAT_H

// Local
#include "precision.h"
#include "unicode.h"

// STL
#include <cassert>

namespace MaxCalcEngine {


class BigDecimalFormat
{
public:

    ///////////////////////////////////////////////////////////////////////////
    // Enums

    /// Specifies output formats of BigDecimal number (general, scientific or
    /// engineering).
    enum NumberFormat { GeneralFormat, ScientificFormat, EngineeringFormat };
    /// Specifies case of 'E' letter when exponent is needed.
    enum ExponentCase { UpperCaseExponent, LowerCaseExponent };
    /// Specifies decimal separator.
    enum DecimalSeparator { PointDecimalSeparator, CommaDecimalSeparator };


    ///////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit BigDecimalFormat(
        const int precision = MAX_IO_PRECISION,
        const NumberFormat numberFormat = GeneralFormat,
        const ExponentCase exponentCase = UpperCaseExponent,
        const DecimalSeparator decimalSeparator = PointDecimalSeparator);


    ///////////////////////////////////////////////////////////////////////////
    // Public functions

    static BigDecimalFormat getDefault();


    ///////////////////////////////////////////////////////////////////////////
    // Accessors

    int precision() const;
    void setPrecision(const int precision);

    NumberFormat numberFormat() const;
    void setNumberFormat(const NumberFormat numberFormat);

    ExponentCase exponentCase() const;
    void setExponentCase(const ExponentCase exponentCase);

    DecimalSeparator decimalSeparator() const;
    void setDecimalSeparator(const DecimalSeparator decimalSeparator);

    char decimalSeparatorChar() const;
    wchar_t decimalSeparatorWideChar() const;
    tchar decimalSeparatorTChar() const;

private:

    ///////////////////////////////////////////////////////////////////////////
    // Properties

    int mPrecision;                            ///< Output precision of number.
    NumberFormat mNumberFormat;                ///< Format of number.
    ExponentCase mExponentCase;                ///< Case of 'E' letter.
    DecimalSeparator mDecimalSeparator;        ///< Decimal separator.
};


} // namespace MaxCalcEngine

#endif // BIGDECIMALFORMAT_H
