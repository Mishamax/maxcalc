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
#include "settings.h"
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

	static inline BigDecimalFormat getDefault();


	///////////////////////////////////////////////////////////////////////////
	// Accessors

	inline int precision() const;
	inline void setPrecision(int precision);

	inline NumberFormat numberFormat() const;
	inline void setNumberFormat(NumberFormat numberFormat);

	inline ExponentCase exponentCase() const;
	inline void setExponentCase(ExponentCase exponentCase);

	inline DecimalSeparator decimalSeparator() const;
	inline void setDecimalSeparator(DecimalSeparator decimalSeparator);

	inline char decimalSeparatorChar() const;
	inline wchar_t decimalSeparatorWideChar() const;
	inline tchar decimalSeparatorTChar() const;

private:

	///////////////////////////////////////////////////////////////////////////
	// Properties

	int precision_;							///< Output precision of number.
	NumberFormat numberFormat_;				///< Format of number.
	ExponentCase exponentCase_;				///< Case of 'E' letter.
	DecimalSeparator decimalSeparator_;		///< Decimal separator.
};


///////////////////////////////////////////////////////////////////////////
// Inline functions

/*!
	Gets default format.
*/
inline BigDecimalFormat BigDecimalFormat::getDefault()
{
	return BigDecimalFormat();
}

/*!
	Gets output precision of BigDecimal number.
*/
inline int BigDecimalFormat::precision() const
{
	return precision_;
}

/*!
	Sets output precision of BigDecimal number.
*/
inline void BigDecimalFormat::setPrecision(int precision)
{
	assert(precision >= 1 && precision <= MAX_IO_PRECISION);
	precision_ = precision;
}

/*!
	Gets format of BigDecimal number.
*/
inline BigDecimalFormat::NumberFormat BigDecimalFormat::numberFormat() const
{
	return numberFormat_;
}

/*!
	Sets format of BigDecimal number.
*/
inline void BigDecimalFormat::setNumberFormat(NumberFormat numberFormat)
{
	numberFormat_ = numberFormat;
}

/*!
	Gets case of 'E' letter in BigDecimal number when exponent is needed.
*/
inline BigDecimalFormat::ExponentCase BigDecimalFormat::exponentCase() const
{
	return exponentCase_;
}

/*!
	Sets case of 'E' letter in BigDecimal number when exponent is needed.
*/
inline void BigDecimalFormat::setExponentCase(ExponentCase exponentCase)
{
	exponentCase_ = exponentCase;
}

/*!
	Gets decimal separator.
*/
inline BigDecimalFormat::DecimalSeparator BigDecimalFormat::decimalSeparator() const
{
	return decimalSeparator_;
}

/*!
	Sets decimal separator.
*/
inline void BigDecimalFormat::setDecimalSeparator(
	DecimalSeparator decimalSeparator)
{
	decimalSeparator_ = decimalSeparator;
}

/*!
	Gets decimal separator as char.
*/
inline char BigDecimalFormat::decimalSeparatorChar() const
{
	if (decimalSeparator_ == PointDecimalSeparator)
		return '.';
	else
		return ',';
}

/*!
	Gets decimal separator as wchar_t.
*/
inline wchar_t BigDecimalFormat::decimalSeparatorWideChar() const
{
	if (decimalSeparator_ == PointDecimalSeparator)
		return L'.';
	else
		return L',';
}

/*!
	Gets decimal separator as tchar.
*/
inline tchar BigDecimalFormat::decimalSeparatorTChar() const
{
	if (decimalSeparator_ == PointDecimalSeparator)
		return _T('.');
	else
		return _T(',');
}

} // namespace MaxCalcEngine

#endif // BIGDECIMALFORMAT_H
