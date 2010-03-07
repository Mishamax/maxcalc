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
#include "commandparser.h"
// STL
#include <iostream>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;
using namespace MaxCalcEngine;

/// Indentation used for output
static const tchar * indent = _T("    ");

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

    tstring expr;
    Parser parser;
    CommandParser cmdParser(tcout, parser.context());

    cmdParser.parse(_T("#version"));

    // Main working loop
    while (true) {
        tcout << _T("> ");
        getline(tcin, expr);

        if (tcin.eof()) {
            tcout << endl;
            break;
        }

        trim(expr);

        if (expr.empty() || cmdParser.parse(expr)) {
            continue;
        }

        parser.setExpression(expr);
        runParser(parser);
        tcout << endl;
    }

    return 0;
}
