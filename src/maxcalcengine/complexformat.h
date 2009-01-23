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
	enum ImaginaryOne { iImaginaryOne, jImaginaryOne };


	///////////////////////////////////////////////////////////////////////////
	// Constructors

	explicit ComplexFormat(
		const int precision = MAX_IO_PRECISION,
		const NumberFormat numberFormat = GeneralFormat,
		const ExponentCase exponentCase = UpperCaseExponent,
		const DecimalSeparator decimalSeparator = PointDecimalSeparator,
		const ImaginaryOne imaginaryOne = iImaginaryOne);


	///////////////////////////////////////////////////////////////////////////
	// Public functions

	static inline ComplexFormat getDefault();


	///////////////////////////////////////////////////////////////////////////
	// Accessors

	inline ImaginaryOne imaginaryOne() const;
	inline void setImaginaryOne(const ImaginaryOne imaginaryOne);

	inline char imaginaryOneChar() const;
	inline wchar_t imaginaryOneWideChar() const;
	inline tchar imaginaryOneTChar() const;

private:

	///////////////////////////////////////////////////////////////////////////
	// Properties

	ImaginaryOne imaginaryOne_;		///< Letter used to represent imaginary one
};


///////////////////////////////////////////////////////////////////////////
// Inline functions

/*!
	Gets default format.
*/
inline ComplexFormat ComplexFormat::getDefault()
{
	return ComplexFormat();
}

/*!
	Gets letter used to represent imaginary one in Complex number.
*/
inline ComplexFormat::ImaginaryOne ComplexFormat::imaginaryOne() const
{
	return imaginaryOne_;
}

/*!
	Sets letter used to represent imaginary one in Complex number.
*/
inline void ComplexFormat::setImaginaryOne(const ImaginaryOne imaginaryOne)
{
	imaginaryOne_ = imaginaryOne;
}

/*!
	Gets letter used to represent imaginary one in Complex number as char.
*/
inline char ComplexFormat::imaginaryOneChar() const
{
	if (imaginaryOne_ == iImaginaryOne)
		return 'i';
	else
		return 'j';
}

/*!
	Gets letter used to represent imaginary one in Complex number as wchar_t.
*/
inline wchar_t ComplexFormat::imaginaryOneWideChar() const
{
	if (imaginaryOne_ == iImaginaryOne)
		return L'i';
	else
		return L'j';
}

/*!
	Gets letter used to represent imaginary one in Complex number as tchar.
*/
inline tchar ComplexFormat::imaginaryOneTChar() const
{
	if (imaginaryOne_ == iImaginaryOne)
		return _T('i');
	else
		return _T('j');
}

} // namespace MaxCalcEngine

#endif // COMPLEXFORMAT_H
