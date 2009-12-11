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

// Engine
#include "parser.h"
#include "parsercontext.h"
#include "precision.h"
#include "unicode.h"
#include "version.h"
#include "unitconversion.h"
// STL
#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>

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
	const UnitConversion::UnitDef * cur;
	for (cur = UnitConversion::units(); cur->unit != UnitConversion::NO_UNIT; ++cur)
	{
		if (type != cur->type)
		{
			type = cur->type;
			switch (type)
			{
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

		tcout << cur->name;
		if ((cur+1)->type == type)
			tcout << _T(", ");
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
	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;
}

/*!
	Prints list of defined variables.
*/
void printVariables(ParserContext & context)
{
	if (!context.resultExists() && context.variables().count() == 0)
	{
		tcout << indent << _T("No variables defined") << endl;
		return;
	}

	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;

	Variables::const_iterator iter;
	Variables & vars = context.variables();
	for (iter = vars.begin(); iter != vars.end(); ++iter)
	{
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
	if (VERSION_LABEL[0] != 0)
		tcout << VERSION_LABEL << _T(", ");
	tcout << _T("built: ") << __DATE__ << _T(")") << endl;
	if (displayCopyright)
		tcout << COPYRIGHT << endl << endl;
	tcout << WEBSITE << endl;
	tcout << endl;
}

/*!
	Prints help (list of supported command).
*/
void printHelp()
{
	tcout <<  _T("Commands:") << endl;
	tcout << indent << _T("#funcs - Display list of built-in functions.") << endl;
	tcout << indent << _T("#convs - Display list of unit conversions.") << endl;
	tcout << indent << _T("#consts - Display list of built-in constants.") << endl;
	tcout << indent << _T("#vars - Display list of variables.") << endl;
	tcout << indent << _T("#del <variable> - Delete <variable>.") << endl;
	tcout << indent << _T("#delall - Delete all variables.") << endl;
	tcout << indent << _T("#ver - Display version information.") << endl;
	tcout << indent << _T("help - Get this help.") << endl;
	tcout << indent << _T("exit - Close the program.") << endl;
	tcout << endl;
}

/*!
	Delete variable \a name from \a context.
*/
void deleteVariable(ParserContext & context, tstring name)
{
	trim(name);
	
	if (name == _T(""))
	{
		tcout << indent << _T("Syntax: #del <variable>") << endl;
		return;
	}

	if (context.resultExists() && (name == _T("res") || name == _T("result")))
	{
		tcout << indent << _T("Built-in variable '") << name <<
				_T("' cannot be deleted.") << endl;
		return;
	}

	try
	{
		context.variables().remove(name);
	}
	catch (...)
	{
		tcout << indent << _T("Unknown variable '") << name << _T("'.") << endl;
	}
}

/*!
	Checks that \a expr contains a command and executes it. \a context is
	used to get parser state for commands like printing list of variables.

	Returns true if a command was found and parsed, false otherwise.
*/
bool parseCommand(const tstring & expr, ParserContext & context)
{
	tstring cmd = expr;
	strToLower(cmd);

	if (cmd == _T("exit") || cmd == _T("quit") ||
		cmd == _T("#exit") || cmd == _T("#quit"))
		exit(0);

	if (cmd[0] != _T('#') && cmd != _T("help"))
		return false;

	if (cmd == _T("#funcs"))
		printFunctions();
	else if (cmd == _T("#convs"))
		printUnitConversions();
	else if (cmd == _T("#consts"))
		printConstants(context);
	else if (cmd == _T("#vars"))
		printVariables(context);
	else if (cmd == _T("#ver") || cmd == _T("#version"))
		printVersion(true);
	else if (cmd == _T("#help") || cmd == _T("help"))
		printHelp();
	else if (cmd == _T("#delall"))
		context.variables().removeAll();
	else if (cmd.substr(0, 4) == _T("#del"))
		deleteVariable(context, cmd.substr(4, cmd.length()-4));
	else
		tcout << indent << _T("Unknown command '") << cmd << _T("'.") << endl;

	return true;
}

/*!
	Runs \a parser and prints results to standard output.
*/
void runParser(Parser & parser)
{
	try
	{
		tcout << parser.parse().result().toString().c_str();
	}
	// Parser exceptions
	catch (ResultDoesNotExistException)
	{
		tcout << _T("No result of previous calculations");
	}
	catch (UnknownTokenException & ex)
	{
		tcout << _T("Unknown token '") << ex.what().c_str() <<
                _T("' in expression");
	}
    catch (IncorrectNumberException & ex)
	{
		tcout << _T("Incorrect number");
        if (ex.what() != _T(""))
            tcout << _T("'") << ex.what().c_str() << _T("'");
	}
	catch (IncorrectExpressionException)
	{
		tcout << _T("Incorrect expression");
	}
	catch (NoClosingBracketException)
	{
		tcout << _T("No closing bracket");
	}
    catch (TooManyClosingBracketsException)
    {
        tcout << _T("Too many closing brackets");
    }
    catch (UnknownFunctionException & ex)
	{
        tcout << _T("Unknown function '") << ex.what().c_str() << _T("'");
	}
    catch (UnknownVariableException & ex)
	{
        tcout << _T("Unknown variable '") << ex.what().c_str() << _T("'");
    }
    catch (IncorrectVariableNameException)
	{
		tcout << _T("Incorrect name of variable");
	}
	catch (IncorrectUnitConversionSyntaxException)
	{
		tcout << _T("Incorrect unit conversion syntax");
	}
    catch (UnknownUnitException & ex)
    {
        tcout << _T("Unknown unit '") << ex.what().c_str() << _T("'");
    }
    catch (UnknownUnitConversionException & ex)
    {
        tcout << _T("There is no unit conversion '") << ex.what().c_str() << _T("'");
    }
    // Invalid argument exceptions
	catch (InvalidArgumentException & ex)
	{
        tcout << _T("Invalid argument of function '") << ex.what().c_str() <<
                _T("'");
        switch (ex.reason())
        {
        case InvalidArgumentException::ZERO:
            tcout << _T(" (zero)");
            break;
        case InvalidArgumentException::NEGATIVE:
            tcout << _T(" (negative number)");
            break;
        case InvalidArgumentException::ZERO_OR_NEGATIVE:
            tcout << _T(" (zero or negative number)");
            break;
        case InvalidArgumentException::POWER_FUNCTION:
            tcout << _T(" (zero or negative number in negative degree)");
            break;
        case InvalidArgumentException::FACTORIAL_FUNCTION:
            tcout << _T(" (negative, fractional or complex number)");
            break;
        case InvalidArgumentException::TANGENT_FUNCTION:
            tcout << _T(" (cos(arg) = 0)");
            break;
        case InvalidArgumentException::COTANGENT_FUNCTION:
            tcout << _T(" (sin(arg) = 0)");
            break;
        case InvalidArgumentException::ARCSINE_FUNCTION:
        case InvalidArgumentException::ARCCOSINE_FUNCTION:
            tcout << _T(" (abs(arg) > 1)");
            break;
        case InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION:
            tcout << _T(" (cosh(arg) = 0)");
            break;
        case InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION:
            tcout << _T(" (sinh(arg) = 0)");
            break;
        case InvalidArgumentException::UNKNOWN:
        default:
            // Add nothing
            break;
        }
    }
	catch (InvalidUnitConversionArgumentException & ex)
	{
		tcout << _T("Complex argument in unit conversion '") <<
				ex.what().c_str() << _T("'");
	}
	// Arithmetic exception
	catch (ArithmeticException & ex)
	{
		tstring reason;
		switch (ex.reason())
		{
		case ArithmeticException::DIVISION_BY_ZERO:
			reason = _T("Division by zero");
			break;
		case ArithmeticException::DIVISION_IMPOSSIBLE:
			reason = _T("Division impossible");
			break;
		case ArithmeticException::OVERFLOW:
			reason = _T("Arithmetic overflow");
			break;
		case ArithmeticException::UNDERFLOW:
			reason = _T("Arithmetic underflow");
			break;
		case ArithmeticException::CONVERSION_IMPOSSIBLE:
			reason = _T("Arithmetic conversion impossible");
			break;
		case ArithmeticException::INVALID_OPERATION_ON_FRACTIONAL_NUMBER:
			reason = _T("Invalid operation on fractional number");
			break;
		default: // This includes UNKNOWN_REASON
			reason = _T("Unknown arithmetic error");
			break;
		}
		tcout << reason;
	}
	// Generic parser exception
	catch (ParserException)
	{
		tcout << _T("Unknown error");
	}
	// Generic MaxCalc exception
	catch (MaxCalcException)
	{
		tcout << _T("Unknown error");
	}
}

/*!
	Parse command line arguments.
*/
bool parseCmdLineArgs(int argc, char ** argv)
{
	if (argc < 2)
		return false;

    if (argc >= 3 && strcmp(argv[1], "-c") == 0)
	{
        char * expr = argv[2];
        string str(expr);
        tstring tstr;
#if defined(MAXCALC_UNICODE)
        stringToWideString(str, tstr);
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

	if (parseCmdLineArgs(argc, argv))
		return 0;

	const int exprLength = 1000;
	tchar charExpr[exprLength];
	tstring expr;
	Parser parser;

	printVersion(false);

	// Main working loop
	while (true)
	{
		tcout << _T("> ");
		if (fgetts(charExpr, exprLength, stdin) == NULL)
			continue;
		expr = charExpr;

		trim(expr);

		if (expr.empty())
			continue;
		if (parseCommand(expr, parser.context()))
			continue;

		parser.setExpression(expr);
		tcout << indent;
		runParser(parser);
		tcout << endl;
	}

	return 0;
}
