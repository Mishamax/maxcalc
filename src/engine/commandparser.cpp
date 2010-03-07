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

// Engine
#include "commandparser.h"
#include "unitconversion.h"
#include "version.h"
// STL
#include <iostream>
#include <vector>
#include <sstream>
#include <ostream>

namespace MaxCalcEngine {

using namespace std;

/// Indentation used for output
static const tchar * indent = _T("    ");


/*!
    \class CommandParser
    \brief Provides support for commands execution.

    Parses commands (see \link CommandParser#parse()) from the inputed string
    and prints results to \a ostream specified in constructor.
    Many commands also affect \a ParserContext.
    Commands usually start with '#' (except 'exit' and 'help' commands).

    \see ParserContext.
    \ingroup MaxCalcEngine
*/


/*!
    Prints list of supported functions.
*/
void CommandParser::printFunctions()
{
    mOut << indent << _T("abs") << endl;
    mOut << indent << _T("sqr") << endl;
    mOut << indent << _T("sqrt") << endl;
    mOut << indent << _T("pow") << endl;
    mOut << indent << _T("fact") << endl;
    mOut << indent << _T("sin") << endl;
    mOut << indent << _T("cos") << endl;
    mOut << indent << _T("tan") << endl;
    mOut << indent << _T("cot") << endl;
    mOut << indent << _T("asin") << endl;
    mOut << indent << _T("acos") << endl;
    mOut << indent << _T("atan") << endl;
    mOut << indent << _T("acot") << endl;
    mOut << indent << _T("sinh") << endl;
    mOut << indent << _T("cosh") << endl;
    mOut << indent << _T("tanh") << endl;
    mOut << indent << _T("coth") << endl;
    mOut << indent << _T("asinh") << endl;
    mOut << indent << _T("acosh") << endl;
    mOut << indent << _T("atanh") << endl;
    mOut << indent << _T("acoth") << endl;
    mOut << indent << _T("ln") << endl;
    mOut << indent << _T("log2") << endl;
    mOut << indent << _T("log10") << endl;
    mOut << indent << _T("exp") << endl;
}

/*!
    Prints list of unit conversions.
*/
void CommandParser::printUnitConversions()
{
    mOut << _T("Unit conversion syntax: <expression> [unit1->unit2]") << endl;
    mOut << _T("Example: 140[km->mi]") << endl << endl;
    mOut << _T("Supported units are:");

    UnitConversion::Type type = UnitConversion::NO_TYPE;
    const UnitConversion::UnitDef * c;
    for (c = UnitConversion::units(); c->unit != UnitConversion::NO_UNIT; ++c) {
        if (type != c->type) {
            type = c->type;
            switch (type) {
            case UnitConversion::LENGTH:
                mOut << endl << indent << _T("Length: ");
                break;
            case UnitConversion::WEIGHT:
                mOut << endl << indent << _T("Weight: ");
                break;
            case UnitConversion::TIME:
                mOut << endl << indent << _T("Time: ");
                break;
            case UnitConversion::SPEED:
                mOut << endl << indent << _T("Speed: ");
                break;
            case UnitConversion::TEMPERATURE:
                mOut << endl << indent << _T("Temperature: ");
                break;
            default:
                mOut << endl << indent << _T("Unknown units: ");
                break;
            }
        }

        mOut << c->name;
        if ((c+1)->type == type) mOut << _T(", ");
    }
    mOut << endl;
}

/*!
    Prints list of supported constants and res variable.
*/
void CommandParser::printConstants()
{
    mOut << indent << _T("e = ") << BigDecimal::E.toTString() << endl;
    mOut << indent << _T("pi = ") << BigDecimal::PI.toTString() << endl;
    if (mContext.resultExists()) {
        mOut << indent << _T("res = ") << mContext.result().toTString() << endl;
    }
}

/*!
    Prints list of defined variables.
*/
void CommandParser::printVariables()
{
    if (!mContext.resultExists() && mContext.variables().count() == 0) {
        mOut << indent << _T("No variables defined") << endl;
        return;
    }

    if (mContext.resultExists()) {
        mOut << indent << _T("res = ") << mContext.result().toTString() << endl;
    }

    Variables::const_iterator iter;
    Variables & vars = mContext.variables();
    for (iter = vars.begin(); iter != vars.end(); ++iter) {
        mOut << indent << iter->name << _T(" = ") << iter->value.toTString()
                << endl;
    }
}

/*!
    Prints version information and displays copyright information if
    \a displayCopyright is true.
*/
void CommandParser::printVersion(bool displayCopyright)
{
    mOut << _T("MaxCalc v") << VERSION << _T(" (");
    if (VERSION_LABEL[0] != 0) mOut << VERSION_LABEL << _T(", ");
    mOut << _T("built: ") << __DATE__ << _T(")") << endl;
    if (displayCopyright) mOut << COPYRIGHT << endl << endl;
    mOut << WEBSITE << endl;
}

/*!
    Prints help (list of supported command).
*/
void CommandParser::printHelp()
{
    mOut <<  _T("General commands:") << endl;
    mOut << indent << _T("#funcs - Display list of built-in functions.") << endl;
    mOut << indent << _T("#convs - Display list of unit conversions.") << endl;
    mOut << indent << _T("#consts - Display list of built-in constants.") << endl;
    mOut << indent << _T("#vars - Display list of variables.") << endl;
    mOut << indent << _T("#del [<var>] - Delete <var> (all variables if no argument specified).") << endl;
    mOut << indent << _T("#angles [radians / degrees / grads] - Display / set angle unit.") << endl;
    mOut << indent << _T("#output [, / . / i / j / <precision> / default] - Display / set output settings.") << endl;
    mOut << indent << _T("#ver - Display version information.") << endl;
    mOut << indent << _T("help - Display this help.") << endl;
    mOut << indent << _T("exit - Close the program.") << endl;
}

/*!
    Splits \a cmd into list of arguments (space-separated).
*/
vector<tstring> CommandParser::splitCommand(const tstring & cmd)
{
    tstring space = _T(" \t\f\v\n\r");
    vector<tstring> args;

    size_t space_pos = 0, start_pos = 0;

    while (true) {
        space_pos = cmd.find_first_of(space, start_pos);
        if (space_pos == tstring::npos) {
            args.push_back(cmd.substr(start_pos, cmd.length() - start_pos));
            break;
        } else {
            args.push_back(cmd.substr(start_pos, space_pos - start_pos));
        }

        start_pos = cmd.find_first_not_of(space, space_pos);
    }

    return args;
}

/*!
    Converts \a str to integer number.

    Returns 0 if conversion is impossible.
*/
int CommandParser::ttoi(const tstring & str)
{
    tstringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}

/*!
    Checks that \a expr contains a command and executes it. \a context is
    used to get parser state for commands like printing list of variables.

    Returns true if a command was found and parsed, false otherwise.
*/
bool CommandParser::parse(const tstring & expr)
{
    tstring cmd = expr;
    strToLower(cmd);

    if (cmd == _T("exit") || cmd == _T("quit") ||
        cmd == _T("#exit") || cmd == _T("#quit")) {
        exit(0);
    }

    if (cmd == _T("help") || cmd == _T("#help")) {
        printHelp();
        return true;
    }

    if (cmd[0] != _T('#')) {
        return false;
    }

    vector<tstring> args = splitCommand(cmd);
    tstring & name = args[0];

    if (name == _T("#funcs")) {
        printFunctions();
    } else if (name == _T("#convs")) {
        printUnitConversions();
    } else if (name == _T("#consts")) {
        printConstants();
    } else if (name == _T("#vars")) {
        printVariables();
    } else if (name == _T("#ver") || name == _T("#version")) {
        printVersion(true);
    } else if (name == _T("#del") || name == _T("#delete")) {
        if (args.size() == 1) {
            mContext.variables().removeAll();
            mOut << "Deleted all variables." << endl;
        } else {
            for (size_t i = 1; i < args.size(); ++i) {
                tstring & var = args[i];
                if (var == _T("res") || var == _T("result")) {
                    mOut << _T("Built-in variable '") << var <<
                            _T("' cannot be deleted.") << endl;
                    continue;
                }
                if (var == _T("e") || var == _T("pi")) {
                    mOut << _T("Built-in constant '") << var <<
                            _T("' cannot be deleted.") << endl;
                    continue;
                }
                try {
                    mContext.variables().remove(var);
                    mOut << "Deleted variable '" << var << "'." << endl;
                } catch (...) {
                    mOut << _T("Unknown variable '") << var << _T("'.") << endl;
                }
            }
        }
    } else if (name == _T("#angle") || name == _T("#angles")) {
        if (args.size() == 1) {
            if (mContext.angleUnit() == ParserContext::RADIANS) {
                mOut << _T("Radians") << endl;
            } else if (mContext.angleUnit() == ParserContext::DEGREES) {
                mOut << _T("Degrees") << endl;
            } else {
                mOut << _T("Grads") << endl;
            }
        } else {
            tstring & unit = args[1];
            if (unit == _T("radian") || unit == _T("radians")) {
                mContext.setAngleUnit(ParserContext::RADIANS);
                mOut << _T("Angle unit is set to radians.") << endl;
            } else if (unit == _T("degree") || unit == _T("degrees")) {
                mContext.setAngleUnit(ParserContext::DEGREES);
                mOut << _T("Angle unit is set to degrees.") << endl;
            } else if (unit == _T("grad") || unit == _T("grads")) {
                mContext.setAngleUnit(ParserContext::GRADS);
                mOut << _T("Angle unit is set to grads.") << endl;
            } else {
                mOut << _T("Unknown parameter '") << unit << _T("'") << endl;
            }
        }
    } else if (name == _T("#output")) {
        ComplexFormat & format = mContext.numberFormat();
        if (args.size() == 2 && args[1] == _T("default")) {
            parse(_T("#output . i 50"));
            return true;
        }
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == _T(".")) format.decimalSeparator = ComplexFormat::DOT_SEPARATOR;
            else if (args[i] == _T(",")) format.decimalSeparator = ComplexFormat::COMMA_SEPARATOR;
            else if (args[i] == _T("i")) format.imaginaryOne = ComplexFormat::IMAGINARY_ONE_I;
            else if (args[i] == _T("j")) format.imaginaryOne = ComplexFormat::IMAGINARY_ONE_J;
            else if (istdigit(args[i][0])) format.precision = ttoi(args[i].c_str());
            else mOut << _T("Unknown parameter '") <<  args[i] << _T("'") << endl;

            if (format.precision <= 0 || format.precision > MAX_IO_PRECISION) {
                format.precision = MAX_IO_PRECISION;
                mOut << _T("Invalid output precision '") << args[i] << _T("'") << endl;
            }
        }
        mOut << _T("Output settings:") << endl <<
            indent << _T("Precision = ") << format.precision << _T(" digits") << endl <<
            indent << _T("Decimal separator = '") << format.decimalSeparatorTChar() << _T("'") << endl <<
            indent << _T("Imaginary one = '") << format.imaginaryOneTChar() << _T("'") << endl;
    } else {
        mOut << indent << _T("Unknown command '") << cmd << _T("'.") << endl;
    }

    return true;
}

} // namespace MaxCalcEngine