
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace MaxCalcEngine {


//****************************************************************************
// MaxCalcEngine exceptions
//****************************************************************************


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
/// Invalid argument of pow() function (zero or negative number raised in negative power).
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


}; // namespace MaxCalcEngine

#endif // EXCEPTION_H
