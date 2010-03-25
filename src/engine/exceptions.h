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


//------------------------------------------------------------------------------
/// General MaxCalc engine exception.
class MaxCalcException
{
protected:
    /// Additional info for the exception.
    const tstring mWhat;

public:
    /// Constructs new MaxCalc exception with empty informational string.
    MaxCalcException() : mWhat(_T("")) {}
    
    /// Constructs new MaxCalc exception with specified informational string.
    MaxCalcException(const tstring & what) : mWhat(what) {}
    
    /// Destructs MaxCalcException.
    virtual ~MaxCalcException() {}

    /// Returns additional info for the exception.
    virtual const tstring & what() const throw()
    {
        return mWhat;
    }

    /// Returns user-friendly string message for the exception.
    virtual const tstring toString() const throw()
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

protected:
    /// Reason for exception.
    Reasons mReason;

public:
    /// Constructs new parser exception with specified reason and empty info string.
    ParserException(const Reasons reason)
    {
        mReason = reason;
    }

    /// Constructs new parser exception with specified reason and info string.
    ParserException(const Reasons reason, const tstring & what) : MaxCalcException(what)
    {
        mReason = reason;
    }

    /// Returns reason of exception.
    Reasons reason() const throw()
    {
        return mReason;
    }

    virtual const tstring toString() const throw()
    {
        tstring str;
        switch (mReason)
        {
        case NO_PREVIOUS_RESULT:
            str = _("Previous result does not exist");
            break;
        case UNKNOWN_TOKEN:
            str = format(_("Unknown token '%1'"), &mWhat);
            break;
        case INVALID_NUMBER:
            if (mWhat == _T("")) str = _("Invalid number");
            else str = format(_("Invalid number '%1'"), &mWhat);
            break;
        case NO_CLOSING_BRACKET:
            str = _("No closing bracket");
            break;
        case TOO_MANY_CLOSING_BRACKETS:
            str = _("Too many closing brackets");
            break;
        case UNKNOWN_FUNCTION:
            str = format(_("Unknown function '%1'"), &mWhat);
            break;
        case UNKNOWN_VARIABLE:
            str = format(_("Unknown variable '%1'"), &mWhat);
            break;
        case INVALID_VARIABLE_NAME:
            str = _("Invalid variable name");
            break;
        case INVALID_UNIT_CONVERSION_SYNTAX:
            str = _("Invalid unit conversion syntax");
            break;
        case UNKNOWN_UNIT:
            str = format(_("Unknown unit '%1'"), &mWhat);
            break;
        case UNKNOWN_UNIT_CONVERSION:
            str = format(_("Unknown unit conversion '%1'"), &mWhat);
            break;
        case INVALID_UNIT_CONVERSION_ARGUMENT:
            str = format(_("Complex argument in unit conversion '%1'"), &mWhat);
            break;
        case INVALID_EXPRESSION:
        default:
            str = _("Error in expression");
            break;
        }
        return str;
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
        GENERIC_REASON,                     ///< Generic reason (DEC_Invalid_context or DEC_Invalid_operation).
        DIVISION_BY_ZERO,                   ///< Division by zero (DEC_Division_by_zero or DEC_Division_undefined).
        DIVISION_IMPOSSIBLE,                ///< Division impossible (DEC_Division_impossible).
        ARITHMETIC_OVERFLOW,                ///< Arithmetic overflow (DEC_Overflow).
        ARITHMETIC_UNDERFLOW,               ///< Arithmetic underflow (DEC_Overflow).
        CONVERSION_IMPOSSIBLE,              ///< Conversion to different type is impossible (DEC_Conversion_syntax).
        INVALID_OPERATION_ON_FRACTIONAL_NUMBER  ///< Invalid operation on fractional number (like logical operation).
    };

protected:
    /// Reason for exception.
    Reasons mReason;

public:
    /// Constructs new parser exception with specified reason.
    /// Informational string is generated on the fly by toString() function.
    ArithmeticException(const Reasons reason)
    {
        mReason = reason;
    }

    /// Returns reason of exception.
    Reasons reason() const throw()
    {
        return mReason;
    }

    virtual const tstring toString() const throw()
    {
        tstring str;
        switch (mReason)
        {
        case ArithmeticException::DIVISION_BY_ZERO:
            str = _("Division by zero");
            break;
        case ArithmeticException::DIVISION_IMPOSSIBLE:
            str = _("Division impossible");
            break;
        case ArithmeticException::ARITHMETIC_OVERFLOW:
            str = _("Arithmetic overflow");
            break;
        case ArithmeticException::ARITHMETIC_UNDERFLOW:
            str = _("Arithmetic underflow");
            break;
        case ArithmeticException::CONVERSION_IMPOSSIBLE:
            str = _("Invalid number");
            break;
        case ArithmeticException::INVALID_OPERATION_ON_FRACTIONAL_NUMBER:
            str = _("Invalid operation on fractional number");
            break;
        case ArithmeticException::GENERIC_REASON:
        default:
            str = _("Arithmetic error");
            break;
        }
        return str;
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

protected:
    /// Reason for exception.
    Reasons mReason;

public:
    /// Constructs new parser exception with specified function and reason.
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

    virtual const tstring toString() const throw()
    {
        tstring arg;
        switch (mReason)
        {
        case InvalidArgumentException::ZERO:
            arg = _("zero");
            break;
        case InvalidArgumentException::NEGATIVE:
            arg = _("negative number");
            break;
        case InvalidArgumentException::ZERO_OR_NEGATIVE:
            arg = _("zero or negative number");
            break;
        case InvalidArgumentException::POWER_FUNCTION:
            arg = _("zero or negative number in negative degree");
            break;
        case InvalidArgumentException::FACTORIAL_FUNCTION:
            arg = _("negative, fractional or complex number");
            break;
        case InvalidArgumentException::TANGENT_FUNCTION:
            arg = _("cos(argument) = 0");
            break;
        case InvalidArgumentException::COTANGENT_FUNCTION:
            arg = _("sin(argument) = 0");
            break;
        case InvalidArgumentException::ARCSINE_FUNCTION:
        case InvalidArgumentException::ARCCOSINE_FUNCTION:
            arg = _("abs(argument) > 1");
            break;
        case InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION:
            arg = _("cosh(argument) = 0");
            break;
        case InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION:
            arg = _("sinh(argument) = 0");
            break;
        case InvalidArgumentException::COMPLEX_ANGLE:
            arg = _("cannot convert complex angle from/to radians");
            break;
        default:
            // Add nothing
            return format(_("Invalid argument of function '%1'"), &mWhat);
        }
        return format(_("Invalid argument of function '%1' (%2)"), &mWhat, &arg);
    }
};


#endif // EXCEPTION_H
