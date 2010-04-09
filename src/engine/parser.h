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

#ifndef PARSER_H
#define PARSER_H

// Local
#include "parsercontext.h"
#include "complex.h"
#include "unitconversion.h"
#include "unicode.h"
// STL
#include <list>
#include <vector>


using std::list;
using std::vector;

class Parser
{
public:

    ///////////////////////////////////////////////////////////////////////////
    // Constructors

    Parser();
    Parser(const tstring & expr, const ParserContext & context);

    ///////////////////////////////////////////////////////////////////////////
    // Public functions

    ParserContext & parse();

    ///////////////////////////////////////////////////////////////////////////
    // Accessors

    tstring expression() const;
    void setExpression(const tstring & expr);

    ParserContext & context();
    void setContext(const ParserContext & context);

private:

    ///////////////////////////////////////////////////////////////////////////
    // Private variables

    tstring mExpr;                          ///< Expression to be parsed.
    ParserContext mContext;                 ///< Parser context.
    UnitConversion mUnitConversion;         ///< Unit conversion.


    ///////////////////////////////////////////////////////////////////////////
    // Utility functions

    void reset();


    ///////////////////////////////////////////////////////////////////////////
    // Lexical analyzer

    /// Enum of tokens.
    enum Tokens
    {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        POWER,
        OPENING_BRACKET,
        CLOSING_BRACKET,
        SEMICOLON,
        NUMBER,
        IMAGINARY_ONE,
        IDENTIFIER,
        ASSIGN,

        // Unit conversion
        UNIT,
        OPENING_SQUARE_BRACKET,
        CLOSING_SQUARE_BRACKET,
        ARROW,
    };

    /// Represents token with corresponding part of expression.
    struct Token
    {
        /// Constructs new Token from given \a token_ and \a str_
        Token(const Tokens token_, const tstring & str_)
        { token = token_; str = str_; }

        /// Constructs new Token from given \a token_ and \a char_
        Token(const Tokens token_, const tchar char_)
        { token = token_; str = char_; }

        Tokens token;                   ///< Token.
        tstring str;                    ///< String corresponding to token.
    };

    list<Token> mTokens;           ///< List of tokens
    tstring::const_iterator mCurChar;   ///< Current char of expression

    void lexicalAnalysis();
    bool analyzeAssignment();
    bool analyzeUnitConversions();
    bool analyzeOperators();
    bool analyzeNumbers();
    bool analyzeIdentifiers();
    bool skipSpaces();
    bool isIdentifierChar(tchar c, bool firstChar);
    bool isUnitChar(tchar c);
    bool isDecimalSeparator(tchar c);
    bool isImaginaryOne(tchar c);


    ///////////////////////////////////////////////////////////////////////////
    // Syntax analyzer

    list<Token>::const_iterator mCurToken; ///< Current token in the list of tokens

    void syntaxAnalysis();
    Complex parseAssign();
    Complex parseAddSub();
    Complex parseMulDiv();
    Complex parsePower();
    Complex parseUnitConversions();
    Complex parseUnaryPlusMinus();
    Complex parseBrackets();
    Complex parseFunctions();
    Complex parseConstsVars();
    Complex parseNumbers();

    bool parseFunctionArguments(vector<Complex> & args);
    Complex toRadians(Complex angle);
    Complex fromRadians(Complex angle);
};


#endif // PARSER_H
