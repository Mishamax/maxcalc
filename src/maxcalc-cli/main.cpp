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
	for (const UnitConversion::UnitDef * cur = UnitConversion::units(); cur->unit != UnitConversion::NO_UNIT; ++cur)
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
	for (iter = context.variables().begin(); iter != context.variables().end(); ++iter)
	{
		tcout << indent << iter->name << _T(" = ") << iter->value.toTString() << endl;
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
		tcout << indent << _T("Built-in variable '") << name << _T("' cannot be deleted.") << endl;
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

bool parseCmdLineArgs(int argc, tchar ** argv)
{
	if (argc < 2)
		return false;

	if (argc >= 3 && tstrcmp(argv[1], _T("-c")) == 0)
	{
		tchar * expr = argv[2];

		Parser parser;
		parser.setExpression(expr);
		tcout << parser.parse().result().toString().c_str();

		return true;
	}

	return false;
}

int tmain(int argc, tchar ** argv)
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

		try
		{
			tcout << indent << parser.parse().result().toTString() << endl;
		}
		catch (...)
		{
			tcout << indent << _T("Error!") << endl;
		}
	}

	return 0;
}
