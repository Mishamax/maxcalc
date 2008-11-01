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


#ifndef COMPLEXFORMAT_H
#define COMPLEXFORMAT_H

// Local
#include "bigdecimalformat.h"
#include "tstring.h"

namespace MaxCalcEngine {

//****************************************************************************
// ComplexFormat definition
//****************************************************************************

class ComplexFormat : public BigDecimalFormat
{
public:

	// Constructor
	explicit ComplexFormat(
		const int precision = MAX_IO_PRECISION,
		const NumberFormat numberFormat = ScientificFormat,
		const ExponentCase exponentCase = UpperCaseExponent,
		const char imaginaryOne = 'i');

	// Get default format
	static ComplexFormat getDefault() { return ComplexFormat(); };

	// Accessors
	/// Gets letter used to represent imaginary one in Complex number.
	char imaginaryOne() const { return m_imaginaryOne; };
	/// Sets letter used to represent imaginary one in Complex number.
	void setImaginaryOne(const char imaginaryOne) { m_imaginaryOne = imaginaryOne; };

private:
	// Properties
	char m_imaginaryOne;		///< Letter used to represent imaginary one in Complex number.
};

} // namespace MaxCalcEngine

#endif // COMPLEXFORMAT_H
