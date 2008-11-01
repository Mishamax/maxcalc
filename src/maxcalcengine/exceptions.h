
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace MaxCalcEngine {


//****************************************************************************
// Arithmetic exceptions
//****************************************************************************


/// General arithmetic exception.
class ArithmeticException : public std::exception {};


///////////////////////////////////////////////////////////////////////////
// BigDecimal exceptions


// TODO: better exception handling for BigDecimal



// decNumber exceptions


/// Division by zero exception. Represents \a DEC_Division_by_zero error in decNumber.
class DivisionByZeroException : public ArithmeticException {};
/// Overflow exception. Represents \a DEC_Overflow error in decNumber.
class OverflowException : public ArithmeticException {};
/// Underflow exception. Represents \a DEC_Underflow error in decNumber.
class UnderflowException : public ArithmeticException {};
/// Convertion syntax exception. Represents \a DEC_Conversion_syntax error in decNumber.
class ConvertionSyntaxException : public ArithmeticException {};
/// Division impossible exception. Represents \a DEC_Division_impossible error in decNumber.
class DivisionImpossibleException : public ArithmeticException {};
/// Division undefined exception. Represents \a DEC_Division_undefined error in decNumber.
class DivisionUndefinedException : public ArithmeticException {};
/// Invalid context exception. Represents \a DEC_Invalid_context error in decNumber.
class InvalidContextException : public ArithmeticException {};
/// Invalid operation exception. Represents \a DEC_Invalid_operation error in decNumber.
class InvalidOperationException : public ArithmeticException {};


/// Logical operation on fractional number exception.
class LogicalOperationOnFractionalNumberException : public ArithmeticException {};

// Invalid argument exceptions


/// Invalid argument in function.
class InvalidArgumentException : public ArithmeticException {};
/// Invalid (non-integer or negative) argument in factorial() function.
class InvalidArgumentInFactorialException : public InvalidArgumentException {};
/// Invalid argument in arcsin() function.
class InvalidArgumentInArcSinException : public InvalidArgumentException {};
/// Invalid argument in arccos() function.
class InvalidArgumentInArcCosException : public InvalidArgumentException {};
/// Invalid argument in pow() function (zero or negative number in negative power).
class InvalidArgumentInPowException : public InvalidArgumentException {};
/// Invalid (negative or zero) argument in logarithm functions (ln() or log10()).
class InvalidArgumentInLogException : public InvalidArgumentException {};
/// Invalid (negative) argument in sqrt() function.
class InvalidArgumentInSqrtException : public InvalidArgumentException {};
/// Invalid (divisible by pi/2) argument in tan() function.
class InvalidArgumentInTanException : public InvalidArgumentException {};
/// Invalid (zero or divisible by pi) argument in cot() function.
class InvalidArgumentInCotException : public InvalidArgumentException {};


}; // namespace MaxCalcEngine

#endif // EXCEPTION_H
