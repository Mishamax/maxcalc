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
// SimpleIni
#include "simpleini.h"

// To get location of program's executable file
#if defined(WIN32)
#include <windows.h>    // For GetModuleFileName(), GetModuleHandle()
#else
#include "binreloc.h"   // Binreloc
#endif

using namespace std;
using namespace MaxCalcEngine;

/// Name of .ini file with settings.
static const char * INI_FILE_NAME = "maxcalc.ini";
/// Section in .ini file.
static const tchar * INI_SECTION = _T("General");

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

/*!
    Returns path to .ini file with settings (which is in the same dir as
    program executable).
*/
char * getIniPath()
{
#if defined(WIN32)
    int length = MAX_PATH + 1;
    char * name = (char*)malloc(length);
    if (name == 0) return 0;
    HMODULE handle = GetModuleHandle(NULL);
    if (handle == 0 || !GetModuleFileNameA(handle, name, length)) {
        delete[] name;
        return 0;
    }
    char * lastBackslash = strrchr(name, '\\') + 1;
    strcpy(lastBackslash, INI_FILE_NAME);
    return name;
#else
    char * exeDir = br_find_exe_dir(NULL);
    if (exeDir != NULL) {
        char * iniPath = br_build_path(exeDir, INI_FILE_NAME);
        free(exeDir);
        return iniPath;
    }
    return NULL;
#endif
}

/*!
    Reads settings from \a iniFile into \a ParserContext using CSimpleIni class.
*/
void readSettings(CSimpleIni * ini, ParserContext & context, char * iniFile)
{
    ini->LoadFile(iniFile);
    ComplexFormat & format = context.numberFormat();
    context.setAngleUnit((ParserContext::AngleUnit)ini->GetLongValue(
        INI_SECTION, _T("AngleUnit"), ParserContext::RADIANS));
    format.precision = ini->GetLongValue(INI_SECTION, _T("Precision"),
        MAX_IO_PRECISION);
    format.decimalSeparator = (ComplexFormat::DecimalSeparator)ini->GetLongValue(
        INI_SECTION, _T("DecimalSeparator"), ComplexFormat::DOT_SEPARATOR);
    format.imaginaryOne = (ComplexFormat::ImaginaryOne)ini->GetLongValue(
        INI_SECTION, _T("ImaginaryOne"), ComplexFormat::IMAGINARY_ONE_I);
}

/*!
    Saves settings from \a ParserContext into \a iniFile using CSimpleIni class.
*/
void saveSettings(CSimpleIni * ini, ParserContext & context, char * iniFile)
{
    ComplexFormat & format = context.numberFormat();
    ini->SetLongValue(INI_SECTION, _T("AngleUnit"), context.angleUnit());
    ini->SetLongValue(INI_SECTION, _T("Precision"), format.precision);
    ini->SetLongValue(INI_SECTION, _T("DecimalSeparator"), format.decimalSeparator);
    ini->SetLongValue(INI_SECTION, _T("ImaginaryOne"), format.imaginaryOne);
    ini->SaveFile(iniFile);
}

int main(int argc, char ** argv)
{
    // Get path to .ini file
#if !defined(WIN32)
    if (!br_init(0)) return 1;
#endif
    char * iniFileName = getIniPath();

    // Without that locale may be set incorrecly on Linux
    // (non-latic characters may not work)
    setlocale(LC_ALL, "");

    // Parse command line args
    if (parseCmdLineArgs(argc, argv)) {
        return 0;
    }

    tstring expr;
    Parser parser;
    CommandParser cmdParser(tcout, parser.context());

    // Display version information
    cmdParser.parse(_T("#version"));

    // Read settings from .ini file
    CSimpleIni simpleIni;
    readSettings(&simpleIni, parser.context(), iniFileName);

    // Main working loop
    while (true) {
        tcout << _T("> ");
        getline(tcin, expr);

        if (tcin.eof()) {
            tcout << endl;
            break;
        }

        trim(expr);

        if (expr.empty()) continue;

        CommandParser::Result res = cmdParser.parse(expr);
        if (res == CommandParser::EXIT_COMMAND) break;
        else if (res == CommandParser::COMMAND_PARSED) {
            saveSettings(&simpleIni, parser.context(), iniFileName);
            continue;
        }

        parser.setExpression(expr);
        runParser(parser);
        tcout << endl;
    }

    saveSettings(&simpleIni, parser.context(), iniFileName);
    free(iniFileName);

    return 0;
}
