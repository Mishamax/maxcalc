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

// Local
#include "unicode.h"

namespace MaxCalcEngine {


/// General MaxCalc engine exception
class MaxCalcException
{
    const tstring mWhat;    ///< Additional info for exception.

public:
    MaxCalcException() : mWhat(_T("")) {}
    
    MaxCalcException(const tstring & what) : mWhat(what) {}

    /// Returns additional info for exception.
    virtual const tstring & what() const throw()
    {
        return mWhat;
    }
};

/// General parser exception
class ParserException : public MaxCalcException
{
public:
    ParserException() : MaxCalcException() {}

    ParserException(const tstring & what) : MaxCalcException(what) {}
};




/// General arithmetic exception
class ArithmeticException : public MaxCalcException
{
public:
    enum Reasons { UNKNOWN_REASON, DIVISION_BY_ZERO, DIVISION_IMPOSSIBLE,
                   ARITHMETIC_OVERFLOW, ARITHMETIC_UNDERFLOW,
                   CONVERSION_IMPOSSIBLE,
                   INVALID_OPERATION_ON_FRACTIONAL_NUMBER};

private:
    Reasons mReason;    ///< Exception reason.

public:
    ArithmeticException(const Reasons reason)
    {
        mReason = reason;
    }

    /// Returns reason of exception.
    Reasons reason() const throw()
    {
        return mReason;
    }
};


/// Invalid function argument exception.
class InvalidArgumentException : public MaxCalcException
{
public:
    enum Reasons { UNKNOWN, ZERO, NEGATIVE, ZERO_OR_NEGATIVE, POWER_FUNCTION,
                   FACTORIAL_FUNCTION, TANGENT_FUNCTION, COTANGENT_FUNCTION,
                   ARCSINE_FUNCTION, ARCCOSINE_FUNCTION,
                   HYPERBOLIC_TANGENT_FUNCTION, HYPERBOLIC_COTANGENT_FUNCTION,
                   COMPLEX_ANGLE };

private:
    Reasons mReason;    ///< Exception reason.

public:
    InvalidArgumentException(const tstring & function, const Reasons reason) :
            MaxCalcException(function)
    {
        mReason = reason;
    }

    /// Returns reason of exception.
    Reasons reason() const throw()
    {
        return mReason;
    }
};

/// Invalid unit conversion argument (complex number).
class InvalidUnitConversionArgumentException : public MaxCalcException
{
public:
    InvalidUnitConversionArgumentException(const tstring & conversion) :
            MaxCalcException(conversion) {}
};



///////////////////////////////////////////////////////////////////////////
// Parser exceptions

/// Result of previous calculation does not exist.
class ResultDoesNotExistException : public ParserException {};
/// Unknown token.
class UnknownTokenException : public ParserException
{
public:
    UnknownTokenException(const tstring & token) : ParserException(token) {}
};
/// Incorrect number.
class IncorrectNumberException : public ParserException
{
public:
    IncorrectNumberException() : ParserException() {}
    IncorrectNumberException(const tstring & num) : ParserException(num) {}
};
/// Incorrect expression.
class IncorrectExpressionException : public ParserException {};
/// No closing bracket.
class NoClosingBracketException : public ParserException {};
/// No closing bracket.
class TooManyClosingBracketsException : public ParserException {};
/// Unknown function.
class UnknownFunctionException : public ParserException
{
public:
    UnknownFunctionException(const tstring & func) : ParserException(func) {}
};
/// Unknown variable.
class UnknownVariableException : public ParserException
{
public:
    UnknownVariableException(const tstring & var) : ParserException(var) {}
};
/// Incorrect variable name.
class IncorrectVariableNameException : public ParserException {};
/// Incorrect unit conversion syntax.
class IncorrectUnitConversionSyntaxException : public ParserException {};
/// Unknown unit in unit conversion.
class UnknownUnitException : public ParserException
{
public:
    UnknownUnitException(const tstring & unit) : ParserException(unit) {}
};
/// Unknown unit conversion.
class UnknownUnitConversionException : public ParserException
{
public:
    UnknownUnitConversionException(const tstring & c) : ParserException(c) {}
};

} // namespace MaxCalcEngine

#endif // EXCEPTION_H
