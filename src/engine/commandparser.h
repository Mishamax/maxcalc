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


#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

// Local
#include "unicode.h"
#include "parsercontext.h"
// STL
#include <vector>
#include <ostream>

class CommandParser
{
private:
    tostream & mOut;            ///< Stream where output is printed.
    ParserContext & mContext;   ///< Parser context which is modified command processing.

public:

    /// Possible results of command parsing.
    enum Result { NO_COMMAND, COMMAND_PARSED, EXIT_COMMAND };

    /// Constructs new command parser which will use specified \a context and
    /// output results into \a outStream.
    CommandParser(tostream & outStream, ParserContext & context)
        : mOut(outStream), mContext(context) {};
    Result parse(const tstring & cmd);

private:
    int ttoi(const tstring & str);
    std::vector<tstring> splitCommand(const tstring & cmd);
    void printHelp();
    void printVersion(bool displayCopyright);
    void printVariables();
    void printConstants();
    void printUnitConversions();
    void printFunctions();
};


#endif // COMMANDPARSER_H
