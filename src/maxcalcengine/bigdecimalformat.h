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


#ifndef BIGDECIMALFORMAT_H
#define BIGDECIMALFORMAT_H

// Local
#include "settings.h"
#include "tstring.h"

// STL
#include <cassert>

namespace MaxCalcEngine {

//****************************************************************************
// BigDecimalFormat definition
//****************************************************************************

class BigDecimalFormat
{
public:
	///////////////////////////////////////////////////////////////////////////
	// Enums

	/// Specifies output formats of BigDecimal number (scientific or engineering).
	enum NumberFormat { ScientificFormat, EngineeringFormat };
	/// Specifies case of 'E' letter when exponent is needed.
	enum ExponentCase { UpperCaseExponent, LowerCaseExponent };
	/// Specifies decimal separator.
	enum DecimalSeparator { PointDecimalSeparator, CommaDecimalSeparator };


	///////////////////////////////////////////////////////////////////////////
	// Constructors

	explicit BigDecimalFormat(
		const int precision = MAX_IO_PRECISION,
		const NumberFormat numberFormat = ScientificFormat,
		const ExponentCase exponentCase = UpperCaseExponent,
		const DecimalSeparator decimalSeparator = PointDecimalSeparator);


	///////////////////////////////////////////////////////////////////////////
	// Functions

	// Get default format
	static inline BigDecimalFormat getDefault() { return BigDecimalFormat(); };


	///////////////////////////////////////////////////////////////////////////
	// Accessors

	/// Gets output precision of BigDecimal number.
	inline int precision() const { return precision_; };
	/// Sets output precision of BigDecimal number.
	inline void setPrecision(int precision) { assert(precision >= 1 && precision <= MAX_IO_PRECISION); precision_ = precision; };

	/// Gets format of BigDecimal number.
	inline NumberFormat numberFormat() const { return numberFormat_; };
	/// Sets format of BigDecimal number.
	inline void setNumberFormat(NumberFormat numberFormat) { numberFormat_ = numberFormat; };

	/// Gets case of 'E' letter in BigDecimal number when exponent is needed.
	inline ExponentCase exponentCase() const { return exponentCase_; };
	/// Sets case of 'E' letter in BigDecimal number when exponent is needed.
	inline void setExponentCase(ExponentCase exponentCase) { exponentCase_ = exponentCase; };

	/// Gets decimal separator.
	inline DecimalSeparator decimalSeparator() const { return decimalSeparator_; };
	/// Sets decimal separator.
	inline void setdecimalSeparator(DecimalSeparator decimalSeparator) { decimalSeparator_ = decimalSeparator; };

	/// Gets decimal separator as char.
	inline char decimalSeparatorChar() const { if (decimalSeparator_ == PointDecimalSeparator) return '.'; else return ','; };
	/// Gets decimal separator as wchar_t.
	inline wchar_t decimalSeparatorWideChar() const { if (decimalSeparator_ == PointDecimalSeparator) return L'.'; else return L','; };
	/// Gets decimal separator as tchar.
	inline tchar decimalSeparatorTChar() const { if (decimalSeparator_ == PointDecimalSeparator) return _T('.'); else return _T(','); };

private:

	///////////////////////////////////////////////////////////////////////////
	// Properties

	int precision_;				///< Output precision of BigDecimal number.
	NumberFormat numberFormat_;	///< Format of BigDecimal Number (scientific or engineering)
	ExponentCase exponentCase_;	///< Case of 'E' letter in BigDecimal number when exponent is needed.
	DecimalSeparator decimalSeparator_;		///< Decimal separator.
};

} // namespace MaxCalcEngine

#endif // BIGDECIMALFORMAT_H
