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

// Local
#include "parser.h"
#include "exceptions.h"

namespace MaxCalcEngine {


/*!
	\class Parser
	\brief Main class of MaxCalcEngine which is used for parsing and
	calculating expressions.

	This is a recursive descendant parser. Parser grammar is described in
	"doc/MaxCalc Parser specification.odt" document.

	State of Parser including result of calculation is stored in ParserContext.
	It also defines behavior of Parser like number format used for conversions.

	Parser works with arbitrary-precision Complex numbers and uses Complex
	class for storing numbers and making calculations. Number format is
	determined by ComplexFormat class.

	\sa ParserContext, Complex, ComplexFormat.
	\ingroup MaxCalcEngine
*/


//****************************************************************************
// Constructors
//****************************************************************************

/*!
	Constructs a new instance of Parser with empty expression and default
	context.
*/
Parser::Parser()
{
	expr_ = _T("");
	context_ = ParserContext();
	reset();
}

/*!
	Constructs a new instance of Parser with specified \a expr and \a context.
*/
Parser::Parser(const tstring & expr, const ParserContext & context)
{
	expr_ = expr;
	strToLower(expr_);
	context_ = context;
	reset();
}


//****************************************************************************
// Public functions
//****************************************************************************

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
	catch (...)
	{
		reset();
		throw;
	}

	reset();
	return context_;
}


//****************************************************************************
// Utility functions
//****************************************************************************

/*!
	Resets the state of the parser (but keeps the expression and the context).
*/
void Parser::reset()
{
	curChar_ = expr_.begin();
	tokens_.clear();
}


//****************************************************************************
// Lexical analyzer
//****************************************************************************

/*!
	Performs lexical analysis of given expression.
*/
void Parser::lexicalAnalysis()
{
	curChar_ = expr_.begin();

	while (curChar_ != expr_.end())
	{
		// It is important to analyze identifiers before numbers
		// to prevent recognizing of "i<something>" as number "i" and
		// following identifier "<something>".
		// Also assignment operators need to be analyzed before
		// other operators to correctly recognize operators like "+=".
		if (analyzeAssignment())
			continue;
		if (analyzeOperators())
			continue;
		if (analyzeIdentifiers())
			continue;
		if (analyzeNumbers())
			continue;
		if (analyzeUnitConversion())
			continue;
		if (skipSpaces())
			continue;
		// Token couldn't be parsed
		throw UnknownTokenException();
	}
}

/*!
	Lexical analysis of assignment operators (=, +=, -=, *=, /=, ^=).
*/
bool Parser::analyzeAssignment()
{
	if (_T('=') == *curChar_)
	{
		tokens_.push_back(Token(ASSIGN, _T("=")));
	}
	else
	{
		tchar prevChar = *curChar_++;
		if (curChar_ != expr_.end() && _T('=') == *curChar_ &&
			(_T('+') == prevChar || _T('-') == prevChar ||
			 _T('*') == prevChar || _T('/') == prevChar ||
			 _T('^') == prevChar))
		{
			tokens_.push_back(Token(ASSIGN, prevChar + tstring(_T("="))));
		}
		else
		{
			--curChar_;
			return false;
		}
	}

	++curChar_;
	return true;
}

/*!
	Lexical analysis of operators (+, -, *, /, ^) and brackets.
*/
bool Parser::analyzeOperators()
{
	if (_T('+') == *curChar_)
		tokens_.push_back(Token(PLUS, _T("+")));
	else if (_T('-') == *curChar_)
		tokens_.push_back(Token(MINUS, _T("-")));
	else if (_T('*') == *curChar_)
		tokens_.push_back(Token(MULTIPLY, _T("*")));
	else if (_T('/') == *curChar_)
		tokens_.push_back(Token(DIVIDE, _T("/")));
	else if (_T('^') == *curChar_)
		tokens_.push_back(Token(POWER, _T("^")));
	else if (_T('(') == *curChar_)
		tokens_.push_back(Token(OPENING_BRACKET, _T("(")));
	else if (_T(')') == *curChar_)
		tokens_.push_back(Token(CLOSING_BRACKET, _T(")")));
	else if (_T(';') == *curChar_)
		tokens_.push_back(Token(SEMICOLON, _T(";")));
	else
		return false;

	++curChar_;
	return true;
}

/*!
	Lexical analysis of identifiers (function and variable names).
*/
bool Parser::analyzeIdentifiers()
{
	if (isIdentifierChar(*curChar_, true))
	{
		tstring identifier;
		identifier = *curChar_++;
		while (curChar_ != expr_.end() && isIdentifierChar(*curChar_, false))
			identifier += *curChar_++;

		tstring imaginaryOne = _T("");
		imaginaryOne += context_.numberFormat().imaginaryOneTChar();
		// Imaginary one is not an identifier
		if (identifier == imaginaryOne)
		{
			--curChar_;
			return false;
		}

		tokens_.push_back(Token(IDENTIFIER, identifier));

		return true;
	}

	return false;
}

/*!
	Lexical analysis of numbers (including decimal point, exponents and complex numbers).
*/
bool Parser::analyzeNumbers()
{
	tstring number = _T("");
	bool thereIsPoint = false;

	const tchar decimalSeparator = context_.numberFormat().decimalSeparatorTChar();
	const tchar imaginaryOne = context_.numberFormat().imaginaryOneTChar();

	// Check if it is a number
	if (!istdigit(*curChar_) && *curChar_ != decimalSeparator && *curChar_ != imaginaryOne)
		return false;

	// Process decimal point at the beginning of the number
	if (expr_.end() != curChar_ && decimalSeparator == *curChar_)
	{
		number += *curChar_++;
		thereIsPoint = true;

		if (expr_.end() == curChar_)
			throw IncorrectNumberException();
	}

	// Process digits
	while (expr_.end() != curChar_ && istdigit(*curChar_))
		number += *curChar_++;

	// Process decimal point in the middle or at the end of the number
	if (expr_.end() != curChar_ && decimalSeparator == *curChar_)
	{
		// Throw an exception if there was a decimal point already
		if (thereIsPoint)
			throw IncorrectNumberException();

		number += *curChar_++;

		// Process digits
		while(expr_.end() != curChar_ && istdigit(*curChar_))
			number += *curChar_++;
	}

	skipSpaces();

	// Process exponential part
	if (expr_.end() != curChar_ && (_T('e') == *curChar_ || _T('E') == *curChar_))
	{
		// Add 'e'
		number += *curChar_++;
		skipSpaces();
		// Add sign after 'e' if it exists
		if (expr_.end() != curChar_ && (_T('-') == *curChar_ || _T('+') == *curChar_))
			number += *curChar_++;
		skipSpaces();
		if (expr_.end() == curChar_)
			throw IncorrectNumberException();
		// Process exponent digits
		while(expr_.end() != curChar_ && istdigit(*curChar_))
			number += *curChar_++;
	}

	if (number != _T(""))
		tokens_.push_back(Token(NUMBER, number));

	skipSpaces();

	// Process imaginary one at the end of the number
	if (expr_.end() != curChar_ && imaginaryOne == *curChar_)
	{
		++curChar_;
		tokens_.push_back(Token(IMAGINARY_ONE, imaginaryOne));
	}

	return true;
}

/*!
	Lexical analysis of unit conversion.

	Looks for expressions like "[<conv>]" (limited by square brackets).
	Only "<conv>" (without brackets) is written to \a tokens_ list.

	\exception IncorrectUnitConversionSyntaxException Thrown when closing bracket ']' not found.
*/
bool Parser::analyzeUnitConversion()
{
	if (*curChar_ == _T('['))
	{
		++curChar_;
		tstring conv = _T("");
		for (; curChar_ != expr_.end() && *curChar_ != _T(']'); ++curChar_)
		{
			conv += *curChar_;
		}
		if (curChar_ == expr_.end() || *curChar_ != _T(']'))
			throw IncorrectUnitConversionSyntaxException();
		tokens_.push_back(Token(UNIT_CONVERSION, conv));
		return true;
	}

	return false;
}

/*!
	Skip spaces in given expression during lexical analysis.
*/
bool Parser::skipSpaces()
{
	// Check if it is a space
	if (expr_.end() == curChar_ || !isspace(*curChar_))
		return false;

	// Skip spaces
	while (expr_.end() != curChar_ && isspace(*curChar_))
		++curChar_;

	return true;
}


//****************************************************************************
// Syntax analyzer
//****************************************************************************

/*!
	Performs syntax analysis of given expression and calculates the result.
*/
void Parser::syntaxAnalysis()
{
	curToken_ = tokens_.begin();

	Complex result = parseAssign();

	if (tokens_.end() == curToken_)
		context_.setResult(result);
	else
		throw IncorrectExpressionException();
}

/*!
	Parses variable assignment.
*/
Complex Parser::parseAssign()
{
	if (curToken_ != tokens_.end() && curToken_->token == IDENTIFIER)
	{
		tstring name = curToken_->str;
		++curToken_;
		if (curToken_ != tokens_.end() && curToken_->token == ASSIGN)
		{
			if (name == _T("e") || name == _T("pi") || name == _T("res") ||
				name == _T("result") || name == _T("i") || name == _T("j") ||
				name == _T("exit") || name == _T("quit") ||
				name == _T("help"))
			{
				throw IncorrectVariableNameException();
			}

			tchar op = curToken_->str[0];
			Complex var;
			if (op != _T('='))
				var = context_.variables()[name];
			++curToken_;
			Complex value = parseAssign();
			switch (op)
			{
			case _T('='):
				var = value;
				break;
			case _T('+'):
				var += value;
				break;
			case _T('-'):
				var -= value;
				break;
			case _T('*'):
				var *= value;
				break;
			case _T('/'):
				var /= value;
				break;
			case _T('^'):
				var = Complex::pow(var, value);
				break;
			}
			context_.variables().add(name, var);
			return var;
		}
		--curToken_;
	}

	return parseAddSub();
}

/*!
	Parses addition and subtraction.
*/
Complex Parser::parseAddSub()
{
	Complex result = parseMulDiv();

	while (curToken_ != tokens_.end())
	{
		if (PLUS == curToken_->token)
		{
			++curToken_;
			result += parseMulDiv();
		}
		else if (MINUS == curToken_->token)
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
	Complex result = parsePower();

	while (curToken_ != tokens_.end())
	{
		if (MULTIPLY == curToken_->token)
		{
			++curToken_;
			result *= parsePower();
		}
		else if (DIVIDE == curToken_->token)
		{
			++curToken_;
			result /= parsePower();
		}
		else
		{
			break;
		}
	}

	return result;
}

/*!
	Parses power ('^') operator.
*/
Complex Parser::parsePower()
{
	Complex result = parseUnaryPlusMinus();

	while (curToken_ != tokens_.end())
	{
		if (POWER == curToken_->token)
		{
			++curToken_;
			result = Complex::pow(result, parseUnaryPlusMinus());
		}
		else
		{
			break;
		}
	}

	return result;
}

/*!
	Parses unary plus and minus operators.
*/
Complex Parser::parseUnaryPlusMinus()
{
	bool negative = false;

	while (curToken_ != tokens_.end())
	{
		if (PLUS == curToken_->token)
		{
			++curToken_;
		}
		else if (MINUS == curToken_->token)
		{
			++curToken_;
			negative = !negative;
		}
		else
		{
			break;
		}
	}

	return negative ? -parseBrackets() : parseBrackets();
}

/*!
	Parses brackets.
*/
Complex Parser::parseBrackets()
{
	if (curToken_ != tokens_.end() && OPENING_BRACKET == curToken_->token)
	{
		++curToken_;
		Complex result = parseAddSub();
		if (tokens_.end() == curToken_ || CLOSING_BRACKET != curToken_->token)
			throw NoClosingBracketException();
		++curToken_;
		return result;
	}

	return parseFunctions();
}

/*!
	Parses built-in functions.
*/
Complex Parser::parseFunctions()
{
	if (curToken_ != tokens_.end() && IDENTIFIER == curToken_->token)
	{
		tstring name = curToken_->str;
		++curToken_;

		std::vector<Complex> args;

		if (!parseFunctionArguments(args))
		{
			// Go back if it is not a function
			--curToken_;
		}
		else
		{
			// Process functions
			if (name == _T("abs") && args.size() == 1)
				return Complex::abs(args[0]);
			else if (name == _T("sqr") && args.size() == 1)
				return Complex::sqr(args[0]);
			else if (name == _T("sqrt") && args.size() == 1)
				return Complex::sqrt(args[0]);
			else if (name == _T("pow") && args.size() == 2)
				return Complex::pow(args[0], args[1]);
			else if ((name == _T("fact") || name == _T("factorial")) && args.size() == 1)
				return Complex::factorial(args[0]);
			else if (name == _T("sin") && args.size() == 1)
				return Complex::sin(args[0]);
			else if (name == _T("cos") && args.size() == 1)
				return Complex::cos(args[0]);
			else if ((name == _T("tan") || name == _T("tg")) && args.size() == 1)
				return Complex::tan(args[0]);
			else if ((name == _T("cot") || name == _T("ctg")) && args.size() == 1)
				return Complex::cot(args[0]);
			else if ((name == _T("asin") || name == _T("arcsin")) && args.size() == 1)
				return Complex::arcsin(args[0]);
			else if ((name == _T("acos") || name == _T("arccos")) && args.size() == 1)
				return Complex::arccos(args[0]);
			else if ((name == _T("atan") || name == _T("arctan") || name == _T("atg") || name == _T("arctg")) && args.size() == 1)
				return Complex::arctan(args[0]);
			else if ((name == _T("acot") || name == _T("arccot") || name == _T("actg") || name == _T("arcctg")) && args.size() == 1)
				return Complex::arccot(args[0]);
			else if (name == _T("sinh") && args.size() == 1)
				return Complex::sinh(args[0]);
			else if (name == _T("cosh") && args.size() == 1)
				return Complex::cosh(args[0]);
			else if ((name == _T("tanh") || name == _T("th")) && args.size() == 1)
				return Complex::tanh(args[0]);
			else if ((name == _T("coth") || name == _T("cth")) && args.size() == 1)
				return Complex::coth(args[0]);
			else if ((name == _T("asinh") || name == _T("arcsinh")) && args.size() == 1)
				return Complex::arcsinh(args[0]);
			else if ((name == _T("acosh") || name == _T("arccosh")) && args.size() == 1)
				return Complex::arccosh(args[0]);
			else if ((name == _T("atanh") || name == _T("arctanh") || name == _T("ath") || name == _T("arcth")) && args.size() == 1)
				return Complex::arctanh(args[0]);
			else if ((name == _T("acoth") || name == _T("arccoth") || name == _T("acth") || name == _T("arccth")) && args.size() == 1)
				return Complex::arccoth(args[0]);
			else if (name == _T("ln") && args.size() == 1)
				return Complex::ln(args[0]);
			else if (name == _T("log2") && args.size() == 1)
				return Complex::log2(args[0]);
			else if (name == _T("log10") && args.size() == 1)
				return Complex::log10(args[0]);
			else if (name == _T("exp") && args.size() == 1)
				return Complex::exp(args[0]);
			else
				// Unknown function
				throw UnknownFunctionException();
		}
	}

	return parseConstsVars();
}

/*!
	Parses constants and variables.
*/
Complex Parser::parseConstsVars()
{
	if (curToken_ != tokens_.end() && IDENTIFIER == curToken_->token)
	{
		if (_T("pi") == curToken_->str)
		{
			++curToken_;
			return BigDecimal::PI;
		}
		else if (_T("e") == curToken_->str)
		{
			++curToken_;
			return BigDecimal::E;
		}
		else if (_T("res") == curToken_->str || _T("result") == curToken_->str)
		{
			++curToken_;
			if (context_.resultExists())
				return context_.result();
			else
				throw ResultDoesNotExistException();
		}
		else
		{
			// Variables.operator[] will throw UnknownVariableException if
			// curToken_->str doesn't exist
			Complex value = context_.variables()[curToken_->str];
			++curToken_;
			return value;
		}
	}

	return parseNumbers();
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
			throw IncorrectNumberException();
		}
	}
	
	if (thereIsResult)
		return isComplex ? Complex(0, result) : result;

	throw IncorrectExpressionException();
}

/*!
	Parses function arguments including opening and closing brackets.
*/
bool Parser::parseFunctionArguments(std::vector<Complex> & args)
{
	if (curToken_ != tokens_.end() && OPENING_BRACKET == curToken_->token)
	{
		++curToken_;
		
		// Parse arguments
		args.push_back(parseAddSub());
		while (curToken_ != tokens_.end() && curToken_->token == SEMICOLON)
		{
			++curToken_;
			args.push_back(parseAddSub());
		}

		// Check for closing bracket
		if (curToken_ == tokens_.end() || curToken_->token != CLOSING_BRACKET)
			throw NoClosingBracketException();

		++curToken_;

		return true;
	}
	else
	{
		return false;
	}
}

} // namespace MaxCalcEngine
