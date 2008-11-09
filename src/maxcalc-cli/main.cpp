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
#include <iostream>
#include <string>

using namespace std;
using namespace MaxCalcEngine;

int main()
{
	tstring expr = _T("");

	ParserContext context;
	while (true)
	{
		tcin >> expr;
		
		if (_T("exit") == expr)
			break;

		Parser parser(expr, context);

		try
		{
			tcout << parser.parse().getResult().toString().c_str() << endl;
		}
		catch (...)
		{
			tcout << _T("Error!") << endl;
		}
	}

	return 0;
}
