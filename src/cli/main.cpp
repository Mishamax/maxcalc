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
#include "constants.h"
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
// For mkdir()
#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif
// SimpleIni
#include "simpleini.h"

using namespace std;

// Ini file constants
#if !defined(MAXCALC_PORTABLE)
#if defined(_WIN32)
static const char * ENV_VAR = "APPDATA";
static const char * INI_FIRST_LEVEL_DIR = "maxcalc";
static const char * PATH_SEPARATOR = "\\";
#else
static const char * ENV_VAR = "HOME";
static const char * INI_FIRST_LEVEL_DIR = ".config";
static const char * INI_SECOND_LEVEL_DIR = "maxcalc";
static const char * PATH_SEPARATOR = "/";
#endif
#endif
static const char * INI_NAME = "maxcalc.ini";
static const tchar * INI_SECTION = _T("General");
static const int INI_PATH_LENGTH = 30;
#if defined(MAXCALC_PORTABLE) && !defined(_WIN32)
static char * sModulePath = 0;
#endif

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
    Returns path to .ini file with settings.
*/
char * getIniPath()
{
#if defined(MAXCALC_PORTABLE)
#if defined(_WIN32)
    char * path = new char[MAX_PATH];
    if (!GetModuleFileNameA(NULL, path, MAX_PATH)) { delete[] path; return 0; }
    char * lastSlash = strrchr(path, '\\');
    if (lastSlash == 0) { delete[] path; return 0; }
    lastSlash[1] = '\0';
    strcat(path, INI_NAME);
#else // _WIN32
    char * path = new char[strlen(sModulePath) + INI_PATH_LENGTH];
    strcpy(path, sModulePath);
    char * lastSlash = strrchr(path, '/');
    if (lastSlash == 0) { delete[] path; return 0; }
    lastSlash[1] = '\0';
    strcat(path, INI_NAME);
#endif // _WIN32
#else // MAXCALC_PORTABLE
    char * location = getenv(ENV_VAR);
    if (location == 0) return 0;
    char * path = new char[strlen(location) + INI_PATH_LENGTH];
    strcpy(path, location);
    strcat(path, PATH_SEPARATOR);
    strcat(path, INI_FIRST_LEVEL_DIR);
    strcat(path, PATH_SEPARATOR);
#if !defined(_WIN32)
    strcat(path, INI_SECOND_LEVEL_DIR);
    strcat(path, PATH_SEPARATOR);
#endif // _WIN32
    strcat(path, INI_NAME);
#endif // MAXCALC_PORTABLE
    return path;
}

/*!
    Creates directory for .ini file.
    Returns true if the directory was successfully created, false otherwise.
*/
bool createIniDir()
{
    bool result = false;
    // Do not create anything in portable version
#if !defined(MAXCALC_PORTABLE)
    char * location = getenv(ENV_VAR);
    if (location == 0) return false;
    char * path = new char[strlen(location) + INI_PATH_LENGTH];
    strcpy(path, location);
    strcat(path, PATH_SEPARATOR);
    strcat(path, INI_FIRST_LEVEL_DIR);
#if defined(_WIN32)
    result = (_mkdir(path) == 0);
#else // _WIN32
    result = (mkdir(path, S_IRWXU) == 0);
    if (!result) {
        strcat(path, PATH_SEPARATOR);
        strcat(path, INI_SECOND_LEVEL_DIR);
        result = (mkdir(path, S_IRWXU) == 0);
    }
#endif // _WIN32
    delete[] path;
#endif // MAXCALC_PORTABLE
    return result;
}

/*!
    Reads settings from \a iniFile into \a ParserContext using CSimpleIni class.
*/
void readSettings(CSimpleIni * ini, ParserContext & context)
{
    // Load .ini file
    char * iniPath = getIniPath();
    if (iniPath == 0) return;
    ini->LoadFile(iniPath);
    delete[] iniPath;

    ComplexFormat & format = context.numberFormat();
    context.setAngleUnit((ParserContext::AngleUnit)ini->GetLongValue(
        INI_SECTION, _T("AngleUnit"), ParserContext::RADIANS));
    format.precision = ini->GetLongValue(INI_SECTION, _T("Precision"),
        Constants::DEFAULT_IO_PRECISION);
    format.decimalSeparator = (ComplexFormat::DecimalSeparator)ini->GetLongValue(
        INI_SECTION, _T("DecimalSeparator"), ComplexFormat::DOT_SEPARATOR);
    format.imaginaryOne = (ComplexFormat::ImaginaryOne)ini->GetLongValue(
        INI_SECTION, _T("ImaginaryOne"), ComplexFormat::IMAGINARY_ONE_I);
}

/*!
    Saves settings from \a ParserContext into \a iniFile using CSimpleIni class.
*/
void saveSettings(CSimpleIni * ini, ParserContext & context)
{
    char * iniPath = getIniPath();
    if (iniPath == 0) return;

    ComplexFormat & format = context.numberFormat();
    ini->SetLongValue(INI_SECTION, _T("AngleUnit"), context.angleUnit());
    ini->SetLongValue(INI_SECTION, _T("Precision"), format.precision);
    ini->SetLongValue(INI_SECTION, _T("DecimalSeparator"), format.decimalSeparator);
    ini->SetLongValue(INI_SECTION, _T("ImaginaryOne"), format.imaginaryOne);

    // Save .ini file
    if (ini->SaveFile(iniPath) != SI_OK) {
        if (createIniDir()) ini->SaveFile(iniPath);
    }
    delete[] iniPath;
}

int main(int argc, char ** argv)
{
    // Without that locale may be set incorrecly on Linux
    // (non-latic characters may not work)
    setlocale(LC_ALL, "");

#if defined(MAXCALC_PORTABLE) && !defined(_WIN32)
    sModulePath = argv[0];
#endif

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
    readSettings(&simpleIni, parser.context());

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
            saveSettings(&simpleIni, parser.context());
            continue;
        }

        parser.setExpression(expr);
        runParser(parser);
        tcout << endl;
    }

    saveSettings(&simpleIni, parser.context());

    return 0;
}
