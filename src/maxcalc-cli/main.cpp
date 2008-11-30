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

bool parseCommand(const tstring & expr)
{
	tstring cmd = expr;
	strToLower(cmd);

	if (cmd == _T("exit") || cmd == _T("quit") || cmd == _T("#exit") || cmd == _T("#quit"))
		exit(0);

	return false;
}

int main()
{
	const int exprLength = 1000;
	tchar charExpr[exprLength];
	tstring expr;

	// Without that locale may be set incorrecly on Linux (non-latic characters may not work)
	setlocale(LC_ALL, "");

	ParserContext context;
	Parser parser;
	parser.setContext(context);
	while (true)
	{
		if (fgetts(charExpr, exprLength, stdin) == NULL)
			continue;
		expr = charExpr;
		// Remove '\n'
		if (expr.length() > 0)
			expr.erase(expr.length() - 1, 1);

		if (parseCommand(expr))
			continue;

		// TODO: better check for empty expression
		if (expr.empty())
			continue;

		parser.setExpression(expr);

		try
		{
			tcout << _T("  ") << parser.parse().result().toString().c_str() << endl;
		}
		catch (...)
		{
			tcout << _T("Error!") << endl;
		}
	}

	return 0;
}
