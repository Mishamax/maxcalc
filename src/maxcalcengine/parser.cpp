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

// Local
#include "parser.h"
// STL
#include <cctype>

namespace MaxCalcEngine {


//****************************************************************************
// Parser implementation
//****************************************************************************


Parser::Parser(const tstring & expr, const ParserContext & context)
{
	m_expr = expr;
	m_context = context;
}


ParserContext Parser::parse()
{
	lexicalAnalysis();
	syntaxAnalysis();
	return m_context;
}

#pragma region Lexical analyzer

void Parser::lexicalAnalysis()
{
	curChar = m_expr.begin();

	while (curChar != m_expr.end())
	{
		if (analizeNumbers())
			continue;
		if (skipSpaces())
			continue;
		// TODO: throw right exception
		throw std::exception();
	}
}

/*!
	Lexical analisys of numbers (including decimal point, exponents and complex numbers).
*/
bool Parser::analizeNumbers()
{
	// TODO: add exponents support
	// TODO: add complex numbers support

	tstring number = _T("");
	bool thereIsPoint = false;

	// Check if it is a number
	if (!isdigit(*curChar) && *curChar != _T('.'))
		return false;

	// Process decimal point at the beginning of the number
	if (_T('.') == *curChar)
	{
		number += *curChar++;
		thereIsPoint = true;

		// TODO: throw right exception
		if (m_expr.end() == curChar)
			throw std::exception();
	}

	// Process digits
	while (m_expr.end() != curChar && isdigit(*curChar))
		number += *curChar++;

	// Process decimal point in the middle or at the end of the number
	if (m_expr.end() != curChar && _T('.') == *curChar)
	{
		// Throw an exception if there was a decimal point already
		if (thereIsPoint)
		{
			// TODO: throw right exception
			throw std::exception();
		}
		else
		{
			number += *curChar++;
			thereIsPoint = true;
		}

		// Process digits
		while(m_expr.end() != curChar && isdigit(*curChar))
			number += *curChar++;
	}

	// Process exponential part
	if (m_expr.end() != curChar && (_T('e') == *curChar || _T('E') == *curChar))
	{
		// Add 'e'
		number += *curChar++;
		// Add sign after 'e' if it exists
		if (m_expr.end() != curChar && (_T('-') == *curChar || _T('+') == *curChar))
			number += *curChar++;
		// TODO: throw right exception
		if (m_expr.end() == curChar)
			throw std::exception();
		// Process exponent digits
		while(m_expr.end() != curChar && isdigit(*curChar))
			number += *curChar++;
	}

	tokens.push_back(Token(NUMBER, number));

	return true;
}

bool Parser::skipSpaces()
{
	// Check if it is a space
	if (!isspace(*curChar))
		return false;

	// Skip spaces
	while (m_expr.end() != curChar && isspace(*curChar))
		++curChar;

	return true;
}

#pragma endregion

#pragma region Syntax analyzer

void Parser::syntaxAnalysis()
{
	curToken = tokens.begin();

	m_context.setResult(parseNumbers());
}

BigDecimal Parser::parseNumbers()
{
	if (NUMBER == curToken->token)
	{
		return BigDecimal(curToken->str);
	}

	// TODO: throw right exception
	throw std::exception();
}

#pragma endregion

} // namespace MaxCalcEngine
