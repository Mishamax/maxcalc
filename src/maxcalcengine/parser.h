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

#ifndef PARSER_H
#define PARSER_H

// Local
#include "unicode.h"
#include "parsercontext.h"
#include "complex.h"
// STL
#include <list>
#include <vector>

namespace MaxCalcEngine {


class Parser
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Constructors

	Parser();
	Parser(const tstring & expr, const ParserContext & context);

	///////////////////////////////////////////////////////////////////////////
	// Public functions

	ParserContext parse();

	///////////////////////////////////////////////////////////////////////////
	// Accessors

	inline tstring expression() const;
	inline void setExpression(const tstring & expr);

	inline ParserContext & context();
	inline void setContext(const ParserContext & context);

private:

	///////////////////////////////////////////////////////////////////////////
	// Private variables

	tstring expr_;						///< Expression to be parsed.
	ParserContext context_;				///< Parser context.


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
		COMMA,
		NUMBER,
		IMAGINARY_ONE,
		IDENTIFIER,
		ASSIGN
	};

	/// Represents token with corresponding part of expression.
	struct Token
	{
		/// Constructs new Token from given \a token_ and \a str_
		inline Token(const Tokens token_, const tstring & str_)
		{ token = token_; str = str_; }

		/// Constructs new Token from given \a token_ and \a char_
		inline Token(const Tokens token_, const tchar char_)
		{ token = token_; str = char_; }

		Tokens token;					///< Token.
		tstring str;					///< String corresponding to token.
	};

	std::list<Token> tokens_;			///< List of tokens
	tstring::const_iterator curChar_;	///< Current char of expression

	void lexicalAnalysis();
	bool analyzeOperators();
	bool analyzeNumbers();
	bool analyzeIdentifiers();
	bool skipSpaces();
	inline bool isIdentifierChar(tchar c, bool firstChar);


	///////////////////////////////////////////////////////////////////////////
	// Syntax analyzer

	std::list<Token>::const_iterator curToken_;	///< Current token in the list of tokens

	void syntaxAnalysis();
	Complex parseAssign();
	Complex parseAddSub();
	Complex parseMulDiv();
	Complex parsePower();
	Complex parseUnaryPlusMinus();
	Complex parseBrackets();
	Complex parseFunctions();
	Complex parseConstsVars();
	Complex parseNumbers();

	bool parseFunctionArguments(std::vector<Complex> & args);
};


///////////////////////////////////////////////////////////////////////////
// Inline functions

/*!
	Gets expression.
*/
inline tstring Parser::expression() const
{
	return expr_;
}

/*!
	Sets expression and resets parser state to make new calculation.
*/
inline void Parser::setExpression(const tstring & expr)
{
	reset();
	expr_ = expr;
	strToLower(expr_);
}

/*!
	Gets context.
*/
inline ParserContext & Parser::context()
{
	return context_;
}

/*!
	Sets context.
*/
inline void Parser::setContext(const ParserContext & context)
{
	context_ = context;
}

/*!
	Determines if \a c is a character which can be part of an identifier.
*/
inline bool Parser::isIdentifierChar(tchar c, bool firstChar)
{
	return (c == _T('_') || istalpha(c) || (!firstChar && istdigit(c)));
}

} // namespace MaxCalcEngine

#endif // PARSER_H
