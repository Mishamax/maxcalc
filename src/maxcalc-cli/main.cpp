/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2008 Michael Maximov (michael.maximov@gmail.com)
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
#include "settings.h"
#include "unicode.h"
#include "version.h"
// STL
#include <clocale>
#include <cstdlib>

using namespace std;
using namespace MaxCalcEngine;

static const tchar * indent = _T("    ");

void printFunctions()
{
	tcout << indent << _T("abs") << endl;
	tcout << indent << _T("sqr") << endl;
	tcout << indent << _T("sqrt") << endl;
	tcout << indent << _T("pow") << endl;
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

void printConstants(const ParserContext & context)
{
	tcout << indent << _T("e = ") << BigDecimal::E.toTString() << endl;
	tcout << indent << _T("pi = ") << BigDecimal::PI.toTString() << endl;
	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;
}

void printVariables(const ParserContext & context)
{
	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;
}

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

void printHelp()
{
	tcout <<  _T("Commands:") << endl;
	tcout << indent << _T("#funcs - Display list of built-in functions.") << endl;
	tcout << indent << _T("#consts - Display list of built-in constants.") << endl;
	tcout << indent << _T("#vars - Display list of variables.") << endl;
	tcout << indent << _T("#help - Get this help.") << endl;
	tcout << indent << _T("#ver - Display version information.") << endl;
	tcout << indent << _T("exit - Close the program.") << endl;
	tcout << endl;
}

bool parseCommand(const tstring & expr, const ParserContext & context)
{
	tstring cmd = expr;
	strToLower(cmd);

	if (cmd == _T("exit") || cmd == _T("quit") || cmd == _T("#exit") || cmd == _T("#quit"))
		exit(0);

	if (cmd[0] != _T('#') && cmd != _T("help"))
		return false;

	if (cmd == _T("#funcs"))
		printFunctions();
	else if (cmd == _T("#consts"))
		printConstants(context);
	else if (cmd == _T("#vars"))
		printVariables(context);
	else if (cmd == _T("#ver") || cmd == _T("#version"))
		printVersion(true);
	else if (cmd == _T("#help") || cmd == _T("help"))
		printHelp();
	else
		tcout << indent << _T("Unknown command") << endl;

	return true;
}

int main()
{
	const int exprLength = 1000;
	tchar charExpr[exprLength];
	tstring expr;

	// Without that locale may be set incorrecly on Linux (non-latic characters may not work)
	setlocale(LC_ALL, "");

	printVersion(false);

	Parser parser;
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
			tcout << indent << parser.parse().result().toTString().c_str() << endl;
		}
		catch (...)
		{
			tcout << indent << _T("Error!") << endl;
		}
	}

	return 0;
}
