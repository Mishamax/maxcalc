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

	inline void setExpression(const tstring & expr) { m_expr = expr; }
	inline tstring getExpression() const { return m_expr; }

	inline void setContext(const ParserContext & context) { m_context = context; }
	inline ParserContext getContext() const { return m_context; }

private:

	///////////////////////////////////////////////////////////////////////////
	// Private variables
	tstring m_expr;
	ParserContext m_context;


	///////////////////////////////////////////////////////////////////////////
	// Lexical analyzer

	enum Tokens
	{
		NUMBER
	};

	struct Token
	{
		inline Token(const Tokens token_, const tstring & str_) { token = token_; str = str_; }

		Tokens token;
		tstring str;
	};

	std::list<Token> tokens;			// List of tokens
	tstring::const_iterator curChar;	// Current char of expression

	void lexicalAnalysis();
	bool analizeNumbers();
	bool skipSpaces();


	///////////////////////////////////////////////////////////////////////////
	// Syntax analyzer

	std::list<Token>::const_iterator curToken;	// Current token in the list of tokens

	void syntaxAnalysis();
	BigDecimal parseNumbers();
};

} // namespace MaxCalcEngine

#endif // PARSER_H
