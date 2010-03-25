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

// Local
#include "parser.h"
#include "exceptions.h"


using std::vector;

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
    mExpr = _T("");
    mContext = ParserContext();
    reset();
}

/*!
    Constructs a new instance of Parser with specified \a expr and \a context.
*/
Parser::Parser(const tstring & expr, const ParserContext & context)
{
    mExpr = expr;
    strToLower(mExpr);
    mContext = context;
    reset();
}


//****************************************************************************
// Public functions
//****************************************************************************

/*!
    Performs calculation of given expression.

    \exception ParserException parse() throws many exceptions based on ParserException.
*/
ParserContext & Parser::parse()
{
    try {
        lexicalAnalysis();
        syntaxAnalysis();
    } catch (...) {
        reset();
        throw;
    }

    reset();
    return mContext;
}


//****************************************************************************
// Utility functions
//****************************************************************************

/*!
    Resets the state of the parser (but keeps the expression and the context).
*/
void Parser::reset()
{
    mCurChar = mExpr.begin();
    mTokens.clear();
}


//****************************************************************************
// Lexical analyzer
//****************************************************************************

/*!
    Performs lexical analysis of given expression.

    \exception UnknownTokenException Unknown token.
*/
void Parser::lexicalAnalysis()
{
    mCurChar = mExpr.begin();

    while (mCurChar != mExpr.end()) {
        // It is important to analyze identifiers before numbers
        // to prevent recognizing of "i<something>" as number "i" and
        // following identifier "<something>".
        // Also assignment operators need to be analyzed before
        // other operators to correctly recognize operators like "+=".
        if (analyzeAssignment()) continue;
        if (analyzeUnitConversions()) continue;
        if (analyzeOperators()) continue;
        if (analyzeIdentifiers()) continue;
        if (analyzeNumbers()) continue;
        if (skipSpaces()) continue;
        // Token couldn't be parsed
        tstring ex = _T("");
        ex += *mCurChar;
        throw ParserException(ParserException::UNKNOWN_TOKEN, ex);
    }
}

/*!
    Lexical analysis of assignment operators (=, +=, -=, *=, /=, ^=).
*/
bool Parser::analyzeAssignment()
{
    if (_T('=') == *mCurChar) {
        mTokens.push_back(Token(ASSIGN, _T("=")));
    } else {
        tchar prevChar = *mCurChar++;
        if (mCurChar != mExpr.end() && _T('=') == *mCurChar &&
            (_T('+') == prevChar || _T('-') == prevChar ||
             _T('*') == prevChar || _T('/') == prevChar ||
             _T('^') == prevChar)) {
            mTokens.push_back(Token(ASSIGN, prevChar + tstring(_T("="))));
        } else {
            --mCurChar;
            return false;
        }
    }

    ++mCurChar;
    return true;
}

/*!
    Lexical analysis of unit conversions (syntax "[unit1 -> unit2]").

    \exception IncorrectUnitConversionSyntaxException Incorrect conversion syntax.
*/
bool Parser::analyzeUnitConversions()
{
    if (_T('[') == *mCurChar) {
        mTokens.push_back(Token(OPENING_SQUARE_BRACKET, _T("[")));
        ++mCurChar;

        skipSpaces();

        tstring unit = _T("");
        while (mCurChar != mExpr.end() && isUnitChar(*mCurChar))
            unit += *mCurChar++;
        if (unit == _T(""))
            throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
        mTokens.push_back(Token(UNIT, unit));

        skipSpaces();

        if (mCurChar != mExpr.end() && _T('-') == *mCurChar) {
            ++mCurChar;
            if (mCurChar != mExpr.end() && _T('>') == *mCurChar) {
                mTokens.push_back(Token(ARROW, _T("->")));
                ++mCurChar;
            } else {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
            }
        } else {
            throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
        }

        skipSpaces();

        unit = _T("");
        while (mCurChar != mExpr.end() && isUnitChar(*mCurChar)) {
            unit += *mCurChar++;
        }
        if (unit == _T("")) {
            throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
        }
        mTokens.push_back(Token(UNIT, unit));

        skipSpaces();

        if (mCurChar != mExpr.end() && _T(']') == *mCurChar) {
            mTokens.push_back(Token(CLOSING_SQUARE_BRACKET, _T("]")));
        } else {
            throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
        }
        ++mCurChar;
        return true;
    }


    return false;
}

/*!
    Lexical analysis of operators (+, -, *, /, ^), brackets and arrow (->).
*/
bool Parser::analyzeOperators()
{
    if (_T('+') == *mCurChar) mTokens.push_back(Token(PLUS, _T("+")));
    else if (_T('-') == *mCurChar) mTokens.push_back(Token(MINUS, _T("-")));
    else if (_T('*') == *mCurChar) mTokens.push_back(Token(MULTIPLY, _T("*")));
    else if (_T('/') == *mCurChar) mTokens.push_back(Token(DIVIDE, _T("/")));
    else if (_T('^') == *mCurChar) mTokens.push_back(Token(POWER, _T("^")));
    else if (_T('(') == *mCurChar) mTokens.push_back(Token(OPENING_BRACKET, _T("(")));
    else if (_T(')') == *mCurChar) mTokens.push_back(Token(CLOSING_BRACKET, _T(")")));
    else if (_T(';') == *mCurChar) mTokens.push_back(Token(SEMICOLON, _T(";")));
    else return false;

    ++mCurChar;
    return true;
}

/*!
    Lexical analysis of identifiers (function and variable names).
*/
bool Parser::analyzeIdentifiers()
{
    if (isIdentifierChar(*mCurChar, true)) {
        tstring identifier;
        identifier = *mCurChar++;
        while (mCurChar != mExpr.end() && isIdentifierChar(*mCurChar, false)) {
            identifier += *mCurChar++;
        }

        // Imaginary one is not an identifier
        if (identifier.length() == 1 && isImaginaryOne(identifier[0])) {
            --mCurChar;
            return false;
        }

        mTokens.push_back(Token(IDENTIFIER, identifier));

        return true;
    }

    return false;
}

/*!
    Lexical analysis of numbers (including decimal point, exponents and
    complex numbers).

    \exception IncorrectNumberException Cannot parse the number.
*/
bool Parser::analyzeNumbers()
{
    tstring number = _T("");
    bool thereIsPoint = false;

    // Check if it is a number
    if (!istdigit(*mCurChar) && !isDecimalSeparator(*mCurChar) &&
        !isImaginaryOne(*mCurChar)) {
        return false;
    }

    // Process decimal point at the beginning of the number
    if (mExpr.end() != mCurChar && isDecimalSeparator(*mCurChar)) {
        number += *mCurChar++;
        thereIsPoint = true;

        if (mExpr.end() == mCurChar) {
            throw ParserException(ParserException::INVALID_NUMBER);
        }
    }

    // Process digits
    while (mExpr.end() != mCurChar && istdigit(*mCurChar)) {
        number += *mCurChar++;
    }

    // Process decimal point in the middle or at the end of the number
    if (mExpr.end() != mCurChar && isDecimalSeparator(*mCurChar)) {
        // Throw an exception if there was a decimal point already
        if (thereIsPoint) {
            throw ParserException(ParserException::INVALID_NUMBER, number);
        }

        number += *mCurChar++;

        // Process digits
        while(mExpr.end() != mCurChar && istdigit(*mCurChar)) {
            number += *mCurChar++;
        }
    }

    skipSpaces();

    // Process exponential part
    if (mExpr.end() != mCurChar && (_T('e') == *mCurChar || _T('E') == *mCurChar)) {
        // Add 'e'
        number += *mCurChar++;
        skipSpaces();
        // Add sign after 'e' if it exists
        if (mExpr.end() != mCurChar && (_T('-') == *mCurChar || _T('+') == *mCurChar)) {
            number += *mCurChar++;
        }
        skipSpaces();
        if (mExpr.end() == mCurChar) {
            throw ParserException(ParserException::INVALID_NUMBER, number);
        }
        // Process exponent digits
        while(mExpr.end() != mCurChar && istdigit(*mCurChar)) {
            number += *mCurChar++;
        }
    }

    if (number != _T("")) {
        mTokens.push_back(Token(NUMBER, number));
    }

    skipSpaces();

    // Process imaginary one at the end of the number
    if (mExpr.end() != mCurChar && isImaginaryOne(*mCurChar)) {
        mTokens.push_back(Token(IMAGINARY_ONE, *mCurChar++));
    }

    return true;
}

/*!
    Skip spaces in given expression during lexical analysis.
*/
bool Parser::skipSpaces()
{
    // Check if it is a space
    if (mExpr.end() == mCurChar || !istspace(*mCurChar)) {
        return false;
    }

    // Skip spaces
    while (mExpr.end() != mCurChar && istspace(*mCurChar)) {
        ++mCurChar;
    }

    return true;
}


//****************************************************************************
// Syntax analyzer
//****************************************************************************

/*!
    Performs syntax analysis of given expression and calculates the result.

    \exception TooManyClosingBracketsException Too many closing brackets.
    \exception IncorrectExpressionException Something unparsed is left after parsing.
*/
void Parser::syntaxAnalysis()
{
    mCurToken = mTokens.begin();

    Complex result = parseAssign();

    if (mTokens.end() == mCurToken) mContext.setResult(result);
    else if (CLOSING_BRACKET == mCurToken->token) throw ParserException(ParserException::TOO_MANY_CLOSING_BRACKETS);
    else throw ParserException(ParserException::INVALID_EXPRESSION);
}

/*!
    Parses variable assignment.

    \exception IncorrectVariableNameException Incorrect var name
*/
Complex Parser::parseAssign()
{
    if (mCurToken != mTokens.end() &&
        (mCurToken->token == IDENTIFIER || mCurToken->token == IMAGINARY_ONE)) {
        tstring name = mCurToken->str;
        ++mCurToken;
        if (mCurToken != mTokens.end() && mCurToken->token == ASSIGN) {
            if (name == _T("e") || name == _T("pi") || name == _T("res") ||
                name == _T("result") || name == _T("i") || name == _T("j") ||
                name == _T("exit") || name == _T("quit") ||
                name == _T("help")) {
                throw ParserException(ParserException::INVALID_VARIABLE_NAME);
            }

            tchar op = mCurToken->str[0];
            Complex var;
            if (op != _T('=')) {
                var = mContext.variables()[name];
            }
            ++mCurToken;
            Complex value = parseAssign();
            switch (op)  {
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
            mContext.variables().add(name, var);
            return var;
        }
        --mCurToken;
    }

    return parseAddSub();
}

/*!
    Parses addition and subtraction.
*/
Complex Parser::parseAddSub()
{
    Complex result = parseMulDiv();

    while (mCurToken != mTokens.end()) {
        if (PLUS == mCurToken->token) {
            ++mCurToken;
            result += parseMulDiv();
        } else if (MINUS == mCurToken->token) {
            ++mCurToken;
            result -= parseMulDiv();
        } else {
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

    while (mCurToken != mTokens.end()) {
        if (MULTIPLY == mCurToken->token) {
            ++mCurToken;
            result *= parsePower();
        } else if (DIVIDE == mCurToken->token) {
            ++mCurToken;
            result /= parsePower();
        } else {
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
    Complex result = parseUnitConversions();

    while (mCurToken != mTokens.end()) {
        if (POWER == mCurToken->token) {
            ++mCurToken;
            result = Complex::pow(result, parseUnitConversions());
        } else {
            break;
        }
    }

    return result;
}

/*!
    Parses unit conversions.

    \exception IncorrectUnitConversionSyntaxException Incorrect conversion syntax.
    \exception IncorrectUnitConversionSyntaxException Incorrect conversion arg.
*/
Complex Parser::parseUnitConversions()
{
    Complex result = parseUnaryPlusMinus();

    while (mCurToken != mTokens.end()) {
        if (OPENING_SQUARE_BRACKET == mCurToken->token) {
            ++mCurToken;
            if (mCurToken == mTokens.end() || mCurToken->token != UNIT) {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
            }
            tstring unit1 = mCurToken->str;
            ++mCurToken;
            if (mCurToken == mTokens.end() || mCurToken->token != ARROW) {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
            }
            ++mCurToken;
            if (mCurToken == mTokens.end() || mCurToken->token != UNIT) {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
            }
            tstring unit2 = mCurToken->str;
            ++mCurToken;
            if (mCurToken == mTokens.end() || mCurToken->token != CLOSING_SQUARE_BRACKET) {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_SYNTAX);
            }
            ++mCurToken;

            if (!result.im.isZero()) {
                throw ParserException(ParserException::INVALID_UNIT_CONVERSION_ARGUMENT,
                        _T("[") + unit1 + _T("->") + unit2 + _T("]"));
            }

            result = mUnitConversion.convert(result.re, unit1, unit2);
        } else {
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

    while (mCurToken != mTokens.end()) {
        if (PLUS == mCurToken->token) {
            ++mCurToken;
        } else if (MINUS == mCurToken->token) {
            ++mCurToken;
            negative = !negative;
        } else {
            break;
        }
    }

    return negative ? -parseBrackets() : parseBrackets();
}

/*!
    Parses brackets.

    \exception NoClosingBracketException Closing bracket is missing.
*/
Complex Parser::parseBrackets()
{
    if (mCurToken != mTokens.end() && OPENING_BRACKET == mCurToken->token) {
        ++mCurToken;
        Complex result = parseAddSub();
        if (mTokens.end() == mCurToken || CLOSING_BRACKET != mCurToken->token) {
            throw ParserException(ParserException::NO_CLOSING_BRACKET);
        }
        ++mCurToken;
        return result;
    }

    return parseFunctions();
}

/*!
    Parses built-in functions.

    \exception UnknownFunctionException Unknown function found.
*/
Complex Parser::parseFunctions()
{
    if (mCurToken != mTokens.end() && IDENTIFIER == mCurToken->token) {
        tstring name = mCurToken->str;
        ++mCurToken;

        vector<Complex> args;

        if (!parseFunctionArguments(args)) {
            // Go back if it is not a function
            --mCurToken;
        } else {
            // Process functions
            if (name == _T("abs") && args.size() == 1) return Complex::abs(args[0]);
            else if (name == _T("sqr") && args.size() == 1) return Complex::sqr(args[0]);
            else if (name == _T("sqrt") && args.size() == 1) return Complex::sqrt(args[0]);
            else if (name == _T("pow") && args.size() == 2) return Complex::pow(args[0], args[1]);
            else if ((name == _T("fact") || name == _T("factorial")) && args.size() == 1) return Complex::factorial(args[0]);
            else if (name == _T("sin") && args.size() == 1) return Complex::sin(toRadians(args[0], name));
            else if (name == _T("cos") && args.size() == 1) return Complex::cos(toRadians(args[0], name));
            else if ((name == _T("tan") || name == _T("tg")) && args.size() == 1) return Complex::tan(toRadians(args[0], name));
            else if ((name == _T("cot") || name == _T("ctg")) && args.size() == 1) return Complex::cot(toRadians(args[0], name));
            else if ((name == _T("asin") || name == _T("arcsin")) && args.size() == 1) return fromRadians(Complex::arcsin(args[0]), name);
            else if ((name == _T("acos") || name == _T("arccos")) && args.size() == 1) return fromRadians(Complex::arccos(args[0]), name);
            else if ((name == _T("atan") || name == _T("arctan") || name == _T("atg") || name == _T("arctg")) && args.size() == 1) return fromRadians(Complex::arctan(args[0]), name);
            else if ((name == _T("acot") || name == _T("arccot") || name == _T("actg") || name == _T("arcctg")) && args.size() == 1) return fromRadians(Complex::arccot(args[0]), name);
            else if (name == _T("sinh") && args.size() == 1) return Complex::sinh(toRadians(args[0], name));
            else if (name == _T("cosh") && args.size() == 1) return Complex::cosh(toRadians(args[0], name));
            else if ((name == _T("tanh") || name == _T("th")) && args.size() == 1) return Complex::tanh(toRadians(args[0], name));
            else if ((name == _T("coth") || name == _T("cth")) && args.size() == 1) return Complex::coth(toRadians(args[0], name));
            else if ((name == _T("asinh") || name == _T("arcsinh")) && args.size() == 1) return fromRadians(Complex::arcsinh(args[0]), name);
            else if ((name == _T("acosh") || name == _T("arccosh")) && args.size() == 1) return fromRadians(Complex::arccosh(args[0]), name);
            else if ((name == _T("atanh") || name == _T("arctanh") || name == _T("ath") || name == _T("arcth")) && args.size() == 1) return fromRadians(Complex::arctanh(args[0]), name);
            else if ((name == _T("acoth") || name == _T("arccoth") || name == _T("acth") || name == _T("arccth")) && args.size() == 1) return fromRadians(Complex::arccoth(args[0]), name);
            else if (name == _T("ln") && args.size() == 1) return Complex::ln(args[0]);
            else if (name == _T("log2") && args.size() == 1) return Complex::log2(args[0]);
            else if (name == _T("log10") && args.size() == 1) return Complex::log10(args[0]);
            else if (name == _T("exp") && args.size() == 1) return Complex::exp(args[0]);
            else throw ParserException(ParserException::UNKNOWN_FUNCTION, name);

            // TODO: correctly report known function with incorrect number
            // of arguments
        }
    }

    return parseConstsVars();
}

/*!
    Parses constants and variables.

    \exception ResultDoesNotExistException No result of prev. calculation.
*/
Complex Parser::parseConstsVars()
{
    if (mCurToken != mTokens.end() && IDENTIFIER == mCurToken->token) {
        if (_T("pi") == mCurToken->str) {
            ++mCurToken;
            return BigDecimal::PI;
        } else if (_T("e") == mCurToken->str) {
            ++mCurToken;
            return BigDecimal::E;
        } else if (_T("res") == mCurToken->str || _T("result") == mCurToken->str) {
            ++mCurToken;
            if (mContext.resultExists()) return mContext.result();
            else throw ParserException(ParserException::NO_PREVIOUS_RESULT);
        } else {
            // Variables.operator[] will throw UnknownVariableException if
            // curToken_->str doesn't exist
            Complex value = mContext.variables()[mCurToken->str];
            ++mCurToken;
            return value;
        }
    }

    return parseNumbers();
}

/*!
    Parses numbers (including complex numbers and exponential notation).

    \exception IncorrectNumberException Cannot parse the number.
    \exception IncorrectExpressionException Parsing hasn't completed on number.
*/
Complex Parser::parseNumbers()
{
    BigDecimal result;
    bool thereIsResult = false;
    bool isComplex = false;

    if (mCurToken != mTokens.end() && IMAGINARY_ONE == mCurToken->token) {
        result = 1;
        isComplex = true;
        thereIsResult = true;
        ++mCurToken;
    }

    if (mCurToken != mTokens.end() && NUMBER == mCurToken->token) {
        result = BigDecimal(mCurToken->str);
        thereIsResult = true;
        ++mCurToken;
    }
    
    if (mCurToken != mTokens.end() && IMAGINARY_ONE == mCurToken->token) {
        if (!isComplex) {
            isComplex = true;
            thereIsResult = true;
            ++mCurToken;
        } else {
            throw ParserException(ParserException::INVALID_NUMBER);
        }
    }
    
    if (thereIsResult) {
        return isComplex ? Complex(0, result) : result;
    }

    throw ParserException(ParserException::INVALID_EXPRESSION);
}

/*!
    Parses function arguments including opening and closing brackets.

    \exception NoClosingBracketException Closing bracket is missing.
*/
bool Parser::parseFunctionArguments(vector<Complex> & args)
{
    if (mCurToken != mTokens.end() && OPENING_BRACKET == mCurToken->token) {
        ++mCurToken;
        
        // Parse arguments
        args.push_back(parseAddSub());
        while (mCurToken != mTokens.end() && mCurToken->token == SEMICOLON) {
            ++mCurToken;
            args.push_back(parseAddSub());
        }

        // Check for closing bracket
        if (mCurToken == mTokens.end() || mCurToken->token != CLOSING_BRACKET) {
            throw ParserException(ParserException::NO_CLOSING_BRACKET);
        }

        ++mCurToken;

        return true;
    } else {
        return false;
    }
}

/*!
    Gets expression.
*/
tstring Parser::expression() const
{
    return mExpr;
}

/*!
    Sets expression and resets parser state to make new calculation.
*/
void Parser::setExpression(const tstring & expr)
{
    reset();
    mExpr = expr;
    strToLower(mExpr);
}

/*!
    Gets context.
*/
ParserContext & Parser::context()
{
    return mContext;
}

/*!
    Sets context.
*/
void Parser::setContext(const ParserContext & context)
{
    mContext = context;
}

/*!
    Determines if \a c is a character which can be part of an identifier.
*/
bool Parser::isIdentifierChar(tchar c, bool firstChar)
{
    return (c == _T('_') || istalpha(c) || (!firstChar && istdigit(c)));
}

/*!
    Determines if \a c is a character which can be part of an unit in unit conversion.
*/
bool Parser::isUnitChar(tchar c)
{
    return (istalpha(c) || c == _T('/'));
}

/*!
    Determines if \a c is a decamal separator ('.' or ',').
*/
bool Parser::isDecimalSeparator(tchar c)
{
    return (c == _T(',') || c == _T('.'));
}

/*!
    Determines if \a c is an imaginary one ('i' or 'j').
*/
bool Parser::isImaginaryOne(tchar c)
{
    return (c == _T('i') || c == _T('j'));
}

/*!
    Converts \a angle from current unit (ParserContext::angleUnit()) to radians.

    \throw InvalidArgumentException(COMPLEX_ANGLE) Complex angle is given.
*/
Complex Parser::toRadians(Complex angle, const tstring & functionName)
{
    if (mContext.angleUnit() != ParserContext::RADIANS) {
        if (!angle.im.isZero()) {
            throw InvalidArgumentException(functionName,
                InvalidArgumentException::COMPLEX_ANGLE);
        }
        if (mContext.angleUnit() == ParserContext::DEGREES) {
            return angle * BigDecimal::PI / 180;
        } else if (mContext.angleUnit() == ParserContext::GRADS) {
            return angle * BigDecimal::PI / 200;
        }
    }
    return angle;
}

/*!
    Converts \a angle from radians to current unit (ParserContext::angleUnit()).

    \throw InvalidArgumentException(COMPLEX_ANGLE) Complex angle is given.
*/
Complex Parser::fromRadians(Complex angle, const tstring & functionName)
{
    if (mContext.angleUnit() != ParserContext::RADIANS) {
        if (!angle.im.isZero()) {
            throw InvalidArgumentException(functionName,
                InvalidArgumentException::COMPLEX_ANGLE);
        }
        if (mContext.angleUnit() == ParserContext::DEGREES) {
            return angle * 180 / BigDecimal::PI;
        } else if (mContext.angleUnit() == ParserContext::GRADS) {
            return angle * 200 / BigDecimal::PI;
        }
    }
    return angle;
}

