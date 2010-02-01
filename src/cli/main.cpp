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
#include "parser.h"
#include "parsercontext.h"
#include "precision.h"
#include "version.h"
#include "unitconversion.h"
#include "unicode.h"
// STL
#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;
using namespace MaxCalcEngine;

/// Indentation used for output
static const tchar * indent = _T("    ");

/*!
    Prints list of supported functions.
*/
void printFunctions()
{
    tcout << indent << _T("abs") << endl;
    tcout << indent << _T("sqr") << endl;
    tcout << indent << _T("sqrt") << endl;
    tcout << indent << _T("pow") << endl;
    tcout << indent << _T("fact") << endl;
    tcout << indent << _T("sin") << endl;
    tcout << indent << _T("cos") << endl;
    tcout << indent << _T("tan") << endl;
    tcout << indent << _T("cot") << endl;
    tcout << indent << _T("asin") << endl;
    tcout << indent << _T("acos") << endl;
    tcout << indent << _T("atan") << endl;
    tcout << indent << _T("acot") << endl;
    tcout << indent << _T("sinh") << endl;
    tcout << indent << _T("cosh") << endl;
    tcout << indent << _T("tanh") << endl;
    tcout << indent << _T("coth") << endl;
    tcout << indent << _T("asinh") << endl;
    tcout << indent << _T("acosh") << endl;
    tcout << indent << _T("atanh") << endl;
    tcout << indent << _T("acoth") << endl;
    tcout << indent << _T("ln") << endl;
    tcout << indent << _T("log2") << endl;
    tcout << indent << _T("log10") << endl;
    tcout << indent << _T("exp") << endl;
    tcout << endl;
}

/*!
    Prints list of unit conversions.
*/
void printUnitConversions()
{
    tcout << _T("Unit conversion syntax: <expression> [unit1->unit2]") << endl;
    tcout << _T("Example: 140[km->mi]") << endl << endl;
    tcout << _T("Supported units are:");

    UnitConversion::Type type = UnitConversion::NO_TYPE;
    const UnitConversion::UnitDef * c;
    for (c = UnitConversion::units(); c->unit != UnitConversion::NO_UNIT; ++c) {
        if (type != c->type) {
            type = c->type;
            switch (type) {
            case UnitConversion::LENGTH:
                tcout << endl << indent << _T("Length: ");
                break;
            case UnitConversion::WEIGHT:
                tcout << endl << indent << _T("Weight: ");
                break;
            case UnitConversion::TIME:
                tcout << endl << indent << _T("Time: ");
                break;
            case UnitConversion::SPEED:
                tcout << endl << indent << _T("Speed: ");
                break;
            case UnitConversion::TEMPERATURE:
                tcout << endl << indent << _T("Temperature: ");
                break;
            default:
                tcout << endl << indent << _T("Unknown units: ");
                break;
            }
        }

        tcout << c->name;
        if ((c+1)->type == type) tcout << _T(", ");
    }
    tcout << endl << endl;
}

/*!
    Prints list of supported constants and res variable.
*/
void printConstants(ParserContext & context)
{
    tcout << indent << _T("e = ") << BigDecimal::E.toTString() << endl;
    tcout << indent << _T("pi = ") << BigDecimal::PI.toTString() << endl;
    if (context.resultExists()) {
        tcout << indent << _T("res = ") << context.result().toTString() << endl;
    }
}

/*!
    Prints list of defined variables.
*/
void printVariables(ParserContext & context)
{
    if (!context.resultExists() && context.variables().count() == 0) {
        tcout << indent << _T("No variables defined") << endl;
        return;
    }

    if (context.resultExists()) {
        tcout << indent << _T("res = ") << context.result().toTString() << endl;
    }

    Variables::const_iterator iter;
    Variables & vars = context.variables();
    for (iter = vars.begin(); iter != vars.end(); ++iter) {
        tcout << indent << iter->name << _T(" = ") << iter->value.toTString()
                << endl;
    }
}

/*!
    Prints version information and displays copyright information if
    \a displayCopyright is true.
*/
void printVersion(bool displayCopyright)
{
    tcout << _T("MaxCalc v") << VERSION << _T(" (");
    if (VERSION_LABEL[0] != 0) tcout << VERSION_LABEL << _T(", ");
    tcout << _T("built: ") << __DATE__ << _T(")") << endl;
    if (displayCopyright) tcout << COPYRIGHT << endl << endl;
    tcout << WEBSITE << endl;
    tcout << endl;
}

/*!
    Prints help (list of supported command).
*/
void printHelp()
{
    tcout <<  _T("General commands:") << endl;
    tcout << indent << _T("#funcs - Display list of built-in functions.") << endl;
    tcout << indent << _T("#convs - Display list of unit conversions.") << endl;
    tcout << indent << _T("#consts - Display list of built-in constants.") << endl;
    tcout << indent << _T("#vars - Display list of variables.") << endl;
    tcout << indent << _T("#del [<var>] - Delete <var> (all variables if no argument specified).") << endl;
    tcout << indent << _T("#angles [radians / degrees / grads] - Display / set current angle unit.") << endl;
    tcout << indent << _T("#ver - Display version information.") << endl;
    tcout << indent << _T("help - Display this help.") << endl;
    tcout << indent << _T("exit - Close the program.") << endl;
    tcout << endl;
}

/*!
    Splits \a cmd into list of arguments (space-separated).
*/
vector<tstring> splitCommand(const tstring & cmd)
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
    Checks that \a expr contains a command and executes it. \a context is
    used to get parser state for commands like printing list of variables.

    Returns true if a command was found and parsed, false otherwise.
*/
bool executeCommand(const tstring & expr, ParserContext & context)
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
        printConstants(context);
    } else if (name == _T("#vars")) {
        printVariables(context);
    } else if (name == _T("#ver") || name == _T("#version")) {
        printVersion(true);
    } else if (name == _T("#del") || name == _T("#delete")) {
        if (args.size() == 1) {
            context.variables().removeAll();
            tcout << "Deleted all variables." << endl;
        } else {
            for (size_t i = 1; i < args.size(); ++i) {
                tstring & var = args[i];
                if (var == _T("res") || var == _T("result")) {
                    tcout << _T("Built-in variable '") << var <<
                            _T("' cannot be deleted.") << endl;
                    continue;
                }
                if (var == _T("e") || var == _T("pi")) {
                    tcout << _T("Built-in constant '") << var <<
                            _T("' cannot be deleted.") << endl;
                    continue;
                }
                try {
                    context.variables().remove(var);
                    tcout << "Deleted variable '" << var << "'." << endl;
                } catch (...) {
                    tcout << _T("Unknown variable '") << var << _T("'.") << endl;
                }
            }
        }
    } else if (name == _T("#angle") || name == _T("#angles")) {
        if (args.size() == 1) {
            if (context.angleUnit() == ParserContext::RADIANS) {
                tcout << _T("Radians") << endl;
            } else if (context.angleUnit() == ParserContext::DEGREES) {
                tcout << _T("Degrees") << endl;
            } else {
                tcout << _T("Grads") << endl;
            }
        } else {
            tstring & unit = args[1];
            if (unit == _T("radian") || unit == _T("radians")) {
                context.setAngleUnit(ParserContext::RADIANS);
                tcout << _T("Angle unit is set to radians.") << endl;
            } else if (unit == _T("degree") || unit == _T("degrees")) {
                context.setAngleUnit(ParserContext::DEGREES);
                tcout << _T("Angle unit is set to degrees.") << endl;
            } else if (unit == _T("grad") || unit == _T("grads")) {
                context.setAngleUnit(ParserContext::GRADS);
                tcout << _T("Angle unit is set to grads.") << endl;
            }
        }
    } else {
        tcout << indent << _T("Unknown command '") << cmd << _T("'.") << endl;
    }

    return true;
}

/*!
    Runs \a parser and prints results to standard output.
*/
void runParser(Parser & parser)
{
    try {
        ParserContext & context = parser.parse();
        tcout << context.result().toTString(context.numberFormat()).c_str();
    } catch (MaxCalcException & ex) {
        tcout << ex.toString().c_str();
    }
}

/*!
    Parse command line arguments.

    Returns \a true if program needs to be closed. \a false if it needs
    continue execution.
*/
bool parseCmdLineArgs(int argc, char ** argv)
{
    if (argc < 2) return false;

    if (argc >= 3 && strcmp(argv[1], "-c") == 0) {
        char * expr = argv[2];
        string str(expr);
        tstring tstr;
#if defined(MAXCALC_UNICODE)
        tstr = stringToWideString(str);
#else
        tstr = str;
#endif

        Parser parser;
        parser.setExpression(tstr);
        runParser(parser);

        return true;
    }

    return false;
}

int main(int argc, char ** argv)
{
    // Without that locale may be set incorrecly on Linux
    // (non-latic characters may not work)
    setlocale(LC_ALL, "");

    if (parseCmdLineArgs(argc, argv)) {
        return 0;
    }

    printVersion(false);

    tstring expr;
    Parser parser;

    // Main working loop
    while (true) {
        tcout << _T("> ");
        getline(tcin, expr);

        if (tcin.eof()) {
            tcout << endl;
            break;
        }

        trim(expr);

        if (expr.empty() || executeCommand(expr, parser.context())) {
            continue;
        }

        parser.setExpression(expr);
        runParser(parser);
        tcout << endl;
    }

    return 0;
}
