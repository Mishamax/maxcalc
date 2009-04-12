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
class MaxCalcEngineException : public std::exception {};
/// General arithmetic exception.
class ArithmeticException : public MaxCalcEngineException {};
/// General parser exception
class ParserException : public MaxCalcEngineException {};


///////////////////////////////////////////////////////////////////////////
// Arithmetic (BigDecimal / Complex) exceptions


// General exceptions


/// Division by zero.
class DivisionByZeroException : public ArithmeticException {};
/// Division impossible.
class DivisionImpossibleException : public ArithmeticException {};
/// Arithmetic overflow.
class OverflowException : public ArithmeticException {};
/// Arithmetic underflow.
class UnderflowException : public ArithmeticException {};
/// Conversion from string or to int is impossible.
class ConversionImpossibleException : public ArithmeticException {};
/// Logical operation on fractional number.
class LogicalOperationOnFractionalNumberException : public ArithmeticException {};


// Invalid argument exceptions

/// Invalid argument of a function.
class InvalidArgumentException : public ArithmeticException {};
/// Invalid argument of factorial() function (non-integer or negative).
class InvalidFactorialArgumentException : public InvalidArgumentException {};
/// Invalid argument of arcsin() function (more than 1 by absolute value).
class InvalidArcSinArgumentException : public InvalidArgumentException {};
/// Invalid argument of arccos() function (more than 1 by absolute value).
class InvalidArcCosArgumentException : public InvalidArgumentException {};
/// Invalid argument of pow() function (zero or negative number in negative power).
class InvalidPowArgumentException : public InvalidArgumentException {};
/// Invalid argument of logarithm (ln() and log10()) functions (negative or zero).
class InvalidLogArgumentException : public InvalidArgumentException {};
/// Invalid argument of sqrt() function (negative).
class InvalidSqrtArgumentException : public InvalidArgumentException {};
/// Invalid argument of tan() function (cos() == 0).
class InvalidTanArgumentException : public InvalidArgumentException {};
/// Invalid argument of cot() function (sin() == 0).
class InvalidCotArgumentException : public InvalidArgumentException {};
/// Invalid argument of tanh() function (cosh() == 0).
class InvalidTanhArgumentException : public InvalidArgumentException {};
/// Invalid argument of coth() function (sinh() == 0).
class InvalidCothArgumentException : public InvalidArgumentException {};
/// Invalid argument of unit conversion (complex number).
class InvalidUnitConversionArgumentException : public InvalidArgumentException {};


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
