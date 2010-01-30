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

#include "i18n.h"
#include "unitconversion.h"

#if defined(MAXCALC_NO_I18N)
#define tr(s) _T(s)
#endif

namespace MaxCalcEngine {

/*!
    \class I18n
    \brief Internationalization helper for engine.

    This class is compiled in frond-ends (both CLI and GUI) and contains
    functions which are front-end-dependend and therefore cannot be compiled in
    the engine.

    The problem is that different front-ends use different i18n methods
    (Qt or gettext or absence of i18n) and also different types to represent
    translated strings (STL string, QString). However it is not convenient
    to put string from engine into front-end because it will cause their
    duplication in every front-end. To avoid this all such strings and
    related functions are put into this class.

    Translated string has \a i18n_string type which is defined as
    \a MaxCalcEngine::tstring (no i18n) or \a QString (Qt i18n).

    Behavior of this class depends on a macro with which it was compiled.
    To disable i18n use \a MAXCALC_NO_I18N, to enable Qt i18n use
    \a MAXCALC_QT_I18N.

    When no i18n is used all strings are returned as they are.
    For Qt all strings are translated using \a tr() macro with context \a I18n.

    \ingroup MaxCalcEngine
*/

/*!
    Returns string describing \a MaxCalcException.
*/
i18n_string I18n::maxCalcExceptionToString(MaxCalcException & /*ex*/)
{
    return tr("Error in expression");
}

/*!
    Returns string describing \a ParserException.
*/
i18n_string I18n::parserExceptionToString(ParserException & ex)
{
    i18n_string str;
    switch (ex.reason())
    {
    case ParserException::NO_PREVIOUS_RESULT:
        str = tr("Previous result does not exist");
        break;
    case ParserException::UNKNOWN_TOKEN:
        str = addArg(tr("Unknown token '%1'"), ex.what());
        break;
    case ParserException::INVALID_NUMBER:
        if (ex.what() == _T("")) str = tr("Invalid number");
        else str = addArg(tr("Invalid number '%1'"), ex.what());
        break;
    case ParserException::NO_CLOSING_BRACKET:
        str = tr("No closing bracket");
        break;
    case ParserException::TOO_MANY_CLOSING_BRACKETS:
        str = tr("Too many closing brackets");
        break;
    case ParserException::UNKNOWN_FUNCTION:
        str = addArg(tr("Unknown function '%1'"), ex.what());
        break;
    case ParserException::UNKNOWN_VARIABLE:
        str = addArg(tr("Unknown variable '%1'"), ex.what());
        break;
    case ParserException::INVALID_VARIABLE_NAME:
        str = tr("Invalid variable name");
        break;
    case ParserException::INVALID_UNIT_CONVERSION_SYNTAX:
        str = tr("Invalid unit conversion syntax");
        break;
    case ParserException::UNKNOWN_UNIT:
        str = addArg(tr("Unknown unit '%1'"), ex.what());
        break;
    case ParserException::UNKNOWN_UNIT_CONVERSION:
        str = addArg(tr("Unknown unit conversion '%1'"), ex.what());
        break;
    case ParserException::INVALID_UNIT_CONVERSION_ARGUMENT:
        str = addArg(tr("Complex argument in unit conversion '%1'"), ex.what());
        break;
    case ParserException::INVALID_EXPRESSION:
    default:
        str = tr("Error in expression");
        break;
    }
    return str;
}

/*!
    Returns string describing \a ArithmeticException.
*/
i18n_string I18n::arithmeticExceptionToString(ArithmeticException & ex)
{
    i18n_string str;
    switch (ex.reason())
    {
    case ArithmeticException::DIVISION_BY_ZERO:
        str = tr("Division by zero");
        break;
    case ArithmeticException::DIVISION_IMPOSSIBLE:
        str = tr("Division impossible");
        break;
    case ArithmeticException::OVERFLOW:
        str = tr("Arithmetic overflow");
        break;
    case ArithmeticException::UNDERFLOW:
        str = tr("Arithmetic underflow");
        break;
    case ArithmeticException::CONVERSION_IMPOSSIBLE:
        str = tr("Conversion impossible");
        break;
    case ArithmeticException::INVALID_OPERATION_ON_FRACTIONAL_NUMBER:
        str = tr("Invalid operation on fractional number");
        break;
    case ArithmeticException::GENERIC:
    default:
        // This includes ArithmeticException::GENERIC
        str = tr("Arithmetic error");
        break;
    }
    return str;
}

/*!
    Returns string describing \a InvalidArgumentException.
*/
i18n_string I18n::invalidArgumentExceptionToString(InvalidArgumentException & ex)
{
    i18n_string arg;
    switch (ex.reason())
    {
    case InvalidArgumentException::ZERO:
        arg = tr("zero");
        break;
    case InvalidArgumentException::NEGATIVE:
        arg = tr("negative number");
        break;
    case InvalidArgumentException::ZERO_OR_NEGATIVE:
        arg = tr("zero or negative number");
        break;
    case InvalidArgumentException::POWER_FUNCTION:
        arg = tr("zero or negative number in negative degree");
        break;
    case InvalidArgumentException::FACTORIAL_FUNCTION:
        arg = tr("negative, fractional or complex number");
        break;
    case InvalidArgumentException::TANGENT_FUNCTION:
        arg = tr("cos(argument) = 0");
        break;
    case InvalidArgumentException::COTANGENT_FUNCTION:
        arg = tr("sin(argument) = 0");
        break;
    case InvalidArgumentException::ARCSINE_FUNCTION:
    case InvalidArgumentException::ARCCOSINE_FUNCTION:
        arg = tr("abs(argument) > 1");
        break;
    case InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION:
        arg = tr("cosh(argument) = 0");
        break;
    case InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION:
        arg = tr("sinh(argument) = 0");
        break;
    case InvalidArgumentException::COMPLEX_ANGLE:
        arg = tr("cannot convert complex angle from/to radians");
        break;
    default:
        // Add nothing
        return addArg(tr("Invalid argument of function '%1'"), ex.what());
    }
    return addArg(tr("Invalid argument of function '%1' (%2)"), ex.what(), arg);
}

/*!
    Replaces '%1' in \a str with \a arg.
*/
i18n_string I18n::addArg(const i18n_string str, const tstring & arg)
{
#if defined(MAXCALC_NO_I18N)
    tstring result = str;
    size_t pos = result.find(_T("%1"));
    if (pos != tstring::npos) {
        result.replace(pos, 2, arg);
    }
    return result;
#elif defined(MAXCALC_QT_I18N)
    return str.arg(QString::fromWCharArray(arg.c_str()));
#endif
}

/*!
    Replaces '%1' and '%2' in \a str with \a arg1 and \a arg2.
*/
i18n_string I18n::addArg(const i18n_string str, const tstring & arg1,
                         const i18n_string arg2)
{
#if defined(MAXCALC_NO_I18N)
    tstring result = addArg(str, arg1);
    size_t pos = result.find(_T("%2"));
    if (pos != tstring::npos) {
        result.replace(pos, 2, arg2);
    }
    return result;
#elif defined(MAXCALC_QT_I18N)
    return str.arg(QString::fromWCharArray(arg1.c_str()), arg2);
#endif
}

} // namespace MaxCalcEngine
