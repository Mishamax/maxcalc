/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2010 Michael Maximov (michael.maximov@gmail.com)
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


//------------------------------------------------------------------------------
/// General MaxCalc engine exception.
class MaxCalcException
{
    /// Additional info for exception.
    const tstring mWhat;

public:
    MaxCalcException() : mWhat(_T("")) {}
    
    MaxCalcException(const tstring & what) : mWhat(what) {}

    /// Returns additional info for exception.
    virtual const tstring & what() const throw()
    {
        return mWhat;
    }
};


//------------------------------------------------------------------------------
/// Parser exception.
class ParserException : public MaxCalcException
{
public:
    /// Reasons for exception.
    enum Reasons
    {
        NO_PREVIOUS_RESULT,                 ///< Result of previous calculation does not exist.
        UNKNOWN_TOKEN,                      ///< Unknown token.
        INVALID_NUMBER,                     ///< Invalid number.
        INVALID_EXPRESSION,                 ///< Invalid expression.
        NO_CLOSING_BRACKET,                 ///< No closing bracket.
        TOO_MANY_CLOSING_BRACKETS,          ///< Too many closing brackets.
        UNKNOWN_FUNCTION,                   ///< Unknown function.
        UNKNOWN_VARIABLE,                   ///< Unknown variable.
        INVALID_VARIABLE_NAME,              ///< Invalid variable name.
        INVALID_UNIT_CONVERSION_SYNTAX,     ///< Invalid unit conversion syntax.
        UNKNOWN_UNIT,                       ///< Unknown unit in unit conversion.
        UNKNOWN_UNIT_CONVERSION,            ///< Unknown unit conversion.
        INVALID_UNIT_CONVERSION_ARGUMENT    ///< Invalid unit conversion argument (complex number).
    };

private:
    /// Reason for exception.
    Reasons mReason;

public:
    ParserException(const Reasons reason)
    {
        mReason = reason;
    }

    ParserException(const Reasons reason, const tstring & what) : MaxCalcException(what)
    {
        mReason = reason;
    }

    /// Returns reason of exception.
    Reasons reason() const throw()
    {
        return mReason;
    }
};


//------------------------------------------------------------------------------
/// Arithmetic exception.
/// It can be generated in BigDecimal and Complex classes. Most arithmetic
/// exceptions correspond to decNumber errors which are checked in BigDecimal.
class ArithmeticException : public MaxCalcException
{
public:
    /// Reasons for exception.
    enum Reasons
    {
        GENERIC,                            ///< Generic reason (DEC_Invalid_context or DEC_Invalid_operation).
        DIVISION_BY_ZERO,                   ///< Division by zero (DEC_Division_by_zero or DEC_Division_undefined).
        DIVISION_IMPOSSIBLE,                ///< Division impossible (DEC_Division_impossible).
        OVERFLOW,                           ///< Arithmetic overflow (DEC_Overflow).
        UNDERFLOW,                          ///< Arithmetic underflow (DEC_Overflow).
        CONVERSION_IMPOSSIBLE,              ///< Conversion to different type is impossible (DEC_Conversion_syntax).
        INVALID_OPERATION_ON_FRACTIONAL_NUMBER  ///< Invalid operation on fractional number (like logical operation).
    };

private:
    /// Reason for exception.
    Reasons mReason;

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


//------------------------------------------------------------------------------
/// Invalid function argument exception.
class InvalidArgumentException : public MaxCalcException
{
public:
    /// Reasons for exception.
    enum Reasons
    {
        ZERO,                               ///< Zero.
        NEGATIVE,                           ///< Negative number.
        ZERO_OR_NEGATIVE,                   ///< Zero or negative number.
        POWER_FUNCTION,                     ///< Invalid argument in pow().
        FACTORIAL_FUNCTION,                 ///< Invalid argument in fact().
        TANGENT_FUNCTION,                   ///< Invalid argument in tan().
        COTANGENT_FUNCTION,                 ///< Invalid argument in cot().
        ARCSINE_FUNCTION,                   ///< Invalid argument in asin().
        ARCCOSINE_FUNCTION,                 ///< Invalid argument in acos().
        HYPERBOLIC_TANGENT_FUNCTION,        ///< Invalid argument in tanh().
        HYPERBOLIC_COTANGENT_FUNCTION,      ///< Invalid argument in coth().
        COMPLEX_ANGLE                       ///< Complex angle.
    };

private:
    /// Reason for exception.
    Reasons mReason;

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


} // namespace MaxCalcEngine

#endif // EXCEPTION_H
