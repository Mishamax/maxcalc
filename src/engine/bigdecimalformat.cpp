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

// Local
#include "bigdecimalformat.h"
// STL
#include <cassert>

namespace MaxCalcEngine {


/*!
    \class BigDecimalFormat
    \brief Represents format settings used for convertion from BigDecimal
    to string.

    Format of BigDecimal number includes precision used for rounding, format
    (general, scientific or engineering) and case of 'E' letter when exponent
    is needed.

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

    \sa BigDecimal::toString()
    \sa MAX_IO_PRECISION
    \ingroup MaxCalcEngine
*/

/*!
    \fn BigDecimalFormat::getDefault()
    \brief Returns BigDecimalFormat with default settings.

    The defaults are: precision = MAX_IO_PRECISION, numberFormat =
    GeneralFormat, exponentCase = UpperCaseExponent.
*/

/*!
    Contructs a new instance of BigDecimalFormat with given settings.
*/
BigDecimalFormat::BigDecimalFormat(const int precision,
                                   const NumberFormat numberFormat,
                                   const ExponentCase exponentCase,
                                   const DecimalSeparator decimalSeparator)
{
    assert(precision >= 1 && precision <= MAX_IO_PRECISION);

    mPrecision = precision;
    mNumberFormat = numberFormat;
    mExponentCase = exponentCase;
    mDecimalSeparator = decimalSeparator;
}

/*!
    Gets default format.
*/
BigDecimalFormat BigDecimalFormat::getDefault()
{
    return BigDecimalFormat();
}

/*!
    Gets output precision of BigDecimal number.
*/
int BigDecimalFormat::precision() const
{
    return mPrecision;
}

/*!
    Sets output precision of BigDecimal number.
*/
void BigDecimalFormat::setPrecision(const int precision)
{
    assert(precision >= 1 && precision <= MAX_IO_PRECISION);
    mPrecision = precision;
}

/*!
    Gets format of BigDecimal number.
*/
BigDecimalFormat::NumberFormat BigDecimalFormat::numberFormat() const
{
    return mNumberFormat;
}

/*!
    Sets format of BigDecimal number.
*/
void BigDecimalFormat::setNumberFormat(const NumberFormat numberFormat)
{
    mNumberFormat = numberFormat;
}

/*!
    Gets case of 'E' letter in BigDecimal number when exponent is needed.
*/
BigDecimalFormat::ExponentCase BigDecimalFormat::exponentCase() const
{
    return mExponentCase;
}

/*!
    Sets case of 'E' letter in BigDecimal number when exponent is needed.
*/
void BigDecimalFormat::setExponentCase(const ExponentCase exponentCase)
{
    mExponentCase = exponentCase;
}

/*!
    Gets decimal separator.
*/
BigDecimalFormat::DecimalSeparator BigDecimalFormat::decimalSeparator() const
{
    return mDecimalSeparator;
}

/*!
    Sets decimal separator.
*/
void BigDecimalFormat::setDecimalSeparator(
    const DecimalSeparator decimalSeparator)
{
    mDecimalSeparator = decimalSeparator;
}

/*!
    Gets decimal separator as char.
*/
char BigDecimalFormat::decimalSeparatorChar() const
{
    return (mDecimalSeparator == POINT_DECIMAL_SEPARATOR) ? '.' : ',';
}

/*!
    Gets decimal separator as wchar_t.
*/
wchar_t BigDecimalFormat::decimalSeparatorWideChar() const
{
    return (mDecimalSeparator == POINT_DECIMAL_SEPARATOR) ? L'.' : L',';
}

/*!
    Gets decimal separator as tchar.
*/
tchar BigDecimalFormat::decimalSeparatorTChar() const
{
    return (mDecimalSeparator == POINT_DECIMAL_SEPARATOR) ? _T('.') : _T(',');
}

} // namespace MaxCalcEngine
