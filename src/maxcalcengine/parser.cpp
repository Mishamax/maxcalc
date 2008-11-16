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
#include <iostream>
#include <cctype>

namespace MaxCalcEngine {


//****************************************************************************
// Parser implementation
//****************************************************************************


/*!
	Constructs a new instance of Parser with specified \a expr and \a context.
*/
Parser::Parser(const tstring & expr, const ParserContext & context)
{
	m_expr = expr;
	m_context = context;
}

/*!
	Performs calculation of given expression.
*/
ParserContext Parser::parse()
{
	lexicalAnalysis();
	syntaxAnalysis();
	return m_context;
}

#pragma region Lexical analyzer

/*!
	Performs lexical analysis of given expression.
*/
void Parser::lexicalAnalysis()
{
	curChar = m_expr.begin();

	while (curChar != m_expr.end())
	{
		if (analyzeBinaryOperators())
			continue;
		if (analyzeNumbers())
			continue;
		if (skipSpaces())
			continue;
		// TODO: throw right exception
		throw std::exception();
	}
}

/*!
	Lexical analysis of binary arithmetic operators (+, -, *, /).
*/
bool Parser::analyzeBinaryOperators()
{
	if (_T('+') == *curChar)
		tokens.push_back(Token(ADDITION, _T("+")));
	else if (_T('-') == *curChar)
		tokens.push_back(Token(SUBTRACTION, _T("-")));
	else if (_T('*') == *curChar)
		tokens.push_back(Token(MULTIPLICATION, _T("*")));
	else if (_T('/') == *curChar)
		tokens.push_back(Token(DIVISION, _T("/")));
	else
		return false;

	++curChar;
	return true;
}

/*!
	Lexical analysis of numbers (including decimal point, exponents and complex numbers).
*/
bool Parser::analyzeNumbers()
{
	// TODO: add exponents support
	// TODO: add complex numbers support
	// TODO: support for spaces in exponential part of the number

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

/*!
	Skip spaces in given expression during lexical analysis.
*/
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

/*!
	Performs syntax analysis of given expression and calculates the result.
*/
void Parser::syntaxAnalysis()
{
	curToken = tokens.begin();

	m_context.setResult(parseAddSub());
}

/*!
	Parses addition and subtraction.
*/
Complex Parser::parseAddSub()
{
	Complex result = parseMulDiv();

	while (curToken != tokens.end())
	{
		if (ADDITION == curToken->token)
		{
			++curToken;
			result += parseMulDiv();
		}
		else if (SUBTRACTION == curToken->token)
		{
			++curToken;
			result -= parseMulDiv();
		}
		else
		{
			break;
		}
	}
	return result;
}

/*!
	Parses multiplication and division.
*/
Complex Parser::parseMulDiv()
{
	Complex result = parseNumbers();

	while (curToken != tokens.end())
	{
		if (MULTIPLICATION == curToken->token)
		{
			++curToken;
			result *= parseNumbers();
		}
		else if (DIVISION == curToken->token)
		{
			++curToken;
			result /= parseNumbers();
		}
		else
		{
			break;
		}
	}
	return result;
}

/*!
	Parses numbers (including complex numbers and exponential notation).
*/
Complex Parser::parseNumbers()
{
	if (curToken != tokens.end() && NUMBER == curToken->token)
	{
		BigDecimal result = BigDecimal(curToken->str);
		++curToken;
		return result;
	}

	// TODO: throw right exception
	throw std::exception();
}

#pragma endregion

} // namespace MaxCalcEngine
