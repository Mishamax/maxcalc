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
// Local
#include "unicode.h"
// STL
#include <clocale>
#include <cstdlib>

using namespace std;
using namespace MaxCalcEngine;

const tchar * indent = _T("    ");

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
}

void printConstants(ParserContext & context)
{
	tcout << indent << _T("e = ") << BigDecimal::E.toTString() << endl;
	tcout << indent << _T("pi = ") << BigDecimal::PI.toTString() << endl;
	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;
}

void printVariables(ParserContext & context)
{
	if (context.resultExists())
		tcout << indent << _T("res = ") << context.result().toTString() << endl;
}

bool parseCommand(const tstring & expr, ParserContext & context)
{
	tstring cmd = expr;
	strToLower(cmd);

	if (cmd == _T("exit") || cmd == _T("quit") || cmd == _T("#exit") || cmd == _T("#quit"))
		exit(0);

	if (cmd[0] != _T('#'))
		return false;

	if (cmd == _T("#funcs"))
		printFunctions();
	else if (cmd == _T("#consts"))
		printConstants(context);
	else if (cmd == _T("#vars"))
		printVariables(context);
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

	Parser parser;
	while (true)
	{
		tcout << _T("> ");
		if (fgetts(charExpr, exprLength, stdin) == NULL)
			continue;
		expr = charExpr;
		// Remove '\n'
		if (expr.length() > 0)
			expr.erase(expr.length() - 1, 1);

		if (parseCommand(expr, parser.context()))
			continue;

		// TODO: better check for empty expression
		if (expr.empty())
			continue;

		parser.setExpression(expr);

		try
		{
			tcout << indent << parser.parse().result().toString().c_str() << endl;
		}
		catch (...)
		{
			tcout << indent << _T("Error!") << endl;
		}
	}

	return 0;
}
