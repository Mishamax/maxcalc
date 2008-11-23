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
	expr_ = expr;
	context_ = context;
	curChar_ = expr_.begin();
	curToken_ = tokens_.begin();
}

/*!
	Performs calculation of given expression.
*/
ParserContext Parser::parse()
{
	try
	{
		lexicalAnalysis();
		syntaxAnalysis();
	}
	catch (std::exception & ex)
	{
		reset();
		throw;
	}

	reset();
	return context_;
}

/*!
	Resets the state of the parser (but keeps the expression and the context).
*/
void Parser::reset()
{
	curChar_ = expr_.begin();
	curToken_ = tokens_.begin();
	tokens_.clear();
}

#pragma region Lexical analyzer

/*!
	Performs lexical analysis of given expression.
*/
void Parser::lexicalAnalysis()
{
	curChar_ = expr_.begin();

	while (curChar_ != expr_.end())
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
	if (_T('+') == *curChar_)
		tokens_.push_back(Token(ADDITION, _T("+")));
	else if (_T('-') == *curChar_)
		tokens_.push_back(Token(SUBTRACTION, _T("-")));
	else if (_T('*') == *curChar_)
		tokens_.push_back(Token(MULTIPLICATION, _T("*")));
	else if (_T('/') == *curChar_)
		tokens_.push_back(Token(DIVISION, _T("/")));
	else
		return false;

	++curChar_;
	return true;
}

/*!
	Lexical analysis of numbers (including decimal point, exponents and complex numbers).
*/
bool Parser::analyzeNumbers()
{
	// TODO: support for spaces in exponential part of the number

	tstring number = _T("");
	bool thereIsPoint = false;
	bool thereIsImaginaryOne = false;

	const tchar decimalSeparator = context_.numberFormat().decimalSeparatorTChar();
	const tchar imaginaryOne = context_.numberFormat().imaginaryOneTChar();

	// Check if it is a number
	if (!isdigit(*curChar_) && *curChar_ != decimalSeparator && *curChar_ != imaginaryOne)
		return false;

	// Process imaginary one at the beginning of the number
	if (imaginaryOne == *curChar_)
	{
		++curChar_;
		tokens_.push_back(Token(IMAGINARY_ONE, imaginaryOne));
		thereIsImaginaryOne = true;
	}

	// Process decimal point at the beginning of the number
	if (expr_.end() != curChar_ && decimalSeparator == *curChar_)
	{
		number += *curChar_++;
		thereIsPoint = true;

		// TODO: throw right exception
		if (expr_.end() == curChar_)
			throw std::exception();
	}

	// Process digits
	while (expr_.end() != curChar_ && isdigit(*curChar_))
		number += *curChar_++;

	// Process decimal point in the middle or at the end of the number
	if (expr_.end() != curChar_ && decimalSeparator == *curChar_)
	{
		// Throw an exception if there was a decimal point already
		if (thereIsPoint)
		{
			// TODO: throw right exception
			throw std::exception();
		}

		number += *curChar_++;

		// Process digits
		while(expr_.end() != curChar_ && isdigit(*curChar_))
			number += *curChar_++;
	}

	// Process exponential part
	if (expr_.end() != curChar_ && (_T('e') == *curChar_ || _T('E') == *curChar_))
	{
		// Add 'e'
		number += *curChar_++;
		// Add sign after 'e' if it exists
		if (expr_.end() != curChar_ && (_T('-') == *curChar_ || _T('+') == *curChar_))
			number += *curChar_++;
		// TODO: throw right exception
		if (expr_.end() == curChar_)
			throw std::exception();
		// Process exponent digits
		while(expr_.end() != curChar_ && isdigit(*curChar_))
			number += *curChar_++;
	}

	// Process imaginary one at the end of the number
	if (expr_.end() != curChar_ && imaginaryOne == *curChar_)
	{
		// Throw an exception if there was an imaginary one already
		if (thereIsImaginaryOne)
		{
			// TODO: throw right exception
			throw std::exception();
		}

		++curChar_;
		tokens_.push_back(Token(IMAGINARY_ONE, imaginaryOne));
	}

	if (number != _T(""))
		tokens_.push_back(Token(NUMBER, number));

	return true;
}

/*!
	Skip spaces in given expression during lexical analysis.
*/
bool Parser::skipSpaces()
{
	// Check if it is a space
	if (!isspace(*curChar_))
		return false;

	// Skip spaces
	while (expr_.end() != curChar_ && isspace(*curChar_))
		++curChar_;

	return true;
}

#pragma endregion

#pragma region Syntax analyzer

/*!
	Performs syntax analysis of given expression and calculates the result.
*/
void Parser::syntaxAnalysis()
{
	curToken_ = tokens_.begin();

	context_.setResult(parseAddSub());
}

/*!
	Parses addition and subtraction.
*/
Complex Parser::parseAddSub()
{
	Complex result = parseMulDiv();

	while (curToken_ != tokens_.end())
	{
		if (ADDITION == curToken_->token)
		{
			++curToken_;
			result += parseMulDiv();
		}
		else if (SUBTRACTION == curToken_->token)
		{
			++curToken_;
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

	while (curToken_ != tokens_.end())
	{
		if (MULTIPLICATION == curToken_->token)
		{
			++curToken_;
			result *= parseNumbers();
		}
		else if (DIVISION == curToken_->token)
		{
			++curToken_;
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
	BigDecimal result;
	bool thereIsResult = false;
	bool isComplex = false;

	if (curToken_ != tokens_.end() && IMAGINARY_ONE == curToken_->token)
	{
		result = 1;
		isComplex = true;
		thereIsResult = true;
		++curToken_;
	}

	if (curToken_ != tokens_.end() && NUMBER == curToken_->token)
	{
		result = BigDecimal(curToken_->str);
		thereIsResult = true;
		++curToken_;
	}
	
	if (curToken_ != tokens_.end() && IMAGINARY_ONE == curToken_->token)
	{
		if (!isComplex)
		{
			isComplex = true;
			thereIsResult = true;
			++curToken_;
		}
		else
		{
			// TODO: throw right exception
			throw std::exception();
		}
	}
	
	if (thereIsResult)
		return isComplex ? Complex(0, result) : result;

	// TODO: throw right exception
	throw std::exception();
}

#pragma endregion

} // namespace MaxCalcEngine
