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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace MaxCalcEngine {


/// General MaxCalc engine exception
class MaxCalcException : public std::exception
{
	const char * const what_;

public:
	MaxCalcException() : what_(0) { }
	
	MaxCalcException(const char * const & what) : what_(what) {}

	virtual const char * what() const
	{
		return what_;
	}
};

/// General parser exception
class ParserException : public MaxCalcException {};



/// General arithmetic exception
class ArithmeticException : public MaxCalcException
{
public:
	enum ArithmeticExceptions { UNKNOWN_REASON, DIVISION_BY_ZERO,
								DIVISION_IMPOSSIBLE, OVERFLOW, UNDERFLOW,
								CONVERSION_IMPOSSIBLE,
								INVALID_OPERATION_ON_FRACTIONAL_NUMBER};

	ArithmeticExceptions reason_;

public:
	ArithmeticException(const ArithmeticExceptions reason)
	{
		reason_ = reason;
	}

	ArithmeticExceptions what()
	{
		return reason_;
	}
};


/// Invalid function argument exception.
class InvalidArgumentException : public MaxCalcException
{
public:
	InvalidArgumentException(const char * const & function) : MaxCalcException(function) { }
};

/// Invalid unit conversion argument (complex number).
class InvalidUnitConversionArgumentException : public MaxCalcException {};


///////////////////////////////////////////////////////////////////////////
// Parser exceptions

/// Result of previous calculation does not exist.
class ResultDoesNotExistException : public ParserException {};
/// Unknown token.
class UnknownTokenException : public ParserException {};
/// Incorrect number.
class IncorrectNumberException : public ParserException {};
/// Incorrect expression.
class IncorrectExpressionException : public ParserException {};
/// No closing bracket.
class NoClosingBracketException : public ParserException {};
/// Unknown function.
class UnknownFunctionException : public ParserException {};
/// Unknown variable.
class UnknownVariableException : public ParserException {};
/// Incorrect variable name.
class IncorrectVariableNameException : public ParserException {};
/// Incorrect unit conversion syntax.
class IncorrectUnitConversionSyntaxException : public ParserException {};
/// Unknown unit conversion.
class UnknownUnitConversionException : public ParserException {};

}; // namespace MaxCalcEngine

#endif // EXCEPTION_H
