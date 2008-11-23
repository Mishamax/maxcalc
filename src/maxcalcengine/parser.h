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

#ifndef PARSER_H
#define PARSER_H

// Local
#include "tstring.h"
#include "parsercontext.h"
#include "complex.h"
// STL
#include <list>

namespace MaxCalcEngine {

//****************************************************************************
// Parser definition
//****************************************************************************

class Parser
{
public:
	Parser(const tstring & expr, const ParserContext & context);

	ParserContext parse();

	inline tstring expression() const { return expr_; }
	inline void setExpression(const tstring & expr) { expr_ = expr; }

	inline ParserContext context() const { return context_; }
	inline void setContext(const ParserContext & context) { context_ = context; }

private:

	///////////////////////////////////////////////////////////////////////////
	// Private variables
	tstring expr_;
	ParserContext context_;


	///////////////////////////////////////////////////////////////////////////
	// Utility functions
	void reset();


	///////////////////////////////////////////////////////////////////////////
	// Lexical analyzer

	enum Tokens
	{
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,
		NUMBER,
		IMAGINARY_ONE
	};

	struct Token
	{
		inline Token(const Tokens token_, const tstring & str_) { token = token_; str = str_; }
		inline Token(const Tokens token_, const tchar char_) { token = token_; str = char_; }

		Tokens token;
		tstring str;
	};

	std::list<Token> tokens_;			// List of tokens
	tstring::const_iterator curChar_;	// Current char of expression

	void lexicalAnalysis();
	bool analyzeBinaryOperators();
	bool analyzeNumbers();
	bool skipSpaces();


	///////////////////////////////////////////////////////////////////////////
	// Syntax analyzer

	std::list<Token>::const_iterator curToken_;	// Current token in the list of tokens

	void syntaxAnalysis();
	Complex parseAddSub();
	Complex parseMulDiv();
	Complex parseNumbers();
};

} // namespace MaxCalcEngine

#endif // PARSER_H
