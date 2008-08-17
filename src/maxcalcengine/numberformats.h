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


#ifndef NUMBERFORMATS_H
#define NUMBERFORMATS_H

#include "settings.h"
#include <QChar>

//****************************************************************************
// BigDecimalFormat definition
//****************************************************************************

class BigDecimalFormat
{
public:
	// Properties
	int precision;
	bool engineeringFormat;
	bool lowerCaseE;

	// Constructor
	BigDecimalFormat(
		const int precision = MAX_IO_PRECISION,
		const bool engineeringFormat = false,
		const bool lowerCaseE = false);

	// Get default format
	static BigDecimalFormat getDefault();

private:
	// Default format
	static const BigDecimalFormat defaultBigDecimalFormat;
};


//****************************************************************************
// ComplexFormat definition
//****************************************************************************

class ComplexFormat : public BigDecimalFormat
{
public:
	// Properties
	bool iBeforeImaginaryPart;
	bool multiplyBetweenIAndImaginaryPart;
	bool spacesAroundSignes;
	QChar imaginaryOne;

	// Constructor
	ComplexFormat(
		const bool iBeforeImaginaryPart_ = false,
		const bool multiplyBetweenIAndImaginaryPart_ = false,
		const bool spacesAroundSignes_ = false,
		const QChar imaginaryOne_ = 'i',
		const int precision_ = MAX_IO_PRECISION,
		const bool engineeringFormat_ = false,
		const bool lowerCaseE_ = false);

	// Get default format
	static ComplexFormat getDefault();

private:
	// Default format
	static const ComplexFormat defaultComplexFormat;
};

#endif
