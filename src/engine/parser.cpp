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
ParserContext Parser::parse()
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
    curChar_ = mExpr.begin();
    tokens_.clear();
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
    curChar_ = mExpr.begin();

    while (curChar_ != mExpr.end()) {
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
        ex += *curChar_;
        throw UnknownTokenException(ex);
    }
}

/*!
    Lexical analysis of assignment operators (=, +=, -=, *=, /=, ^=).
*/
bool Parser::analyzeAssignment()
{
    if (_T('=') == *curChar_) {
        tokens_.push_back(Token(ASSIGN, _T("=")));
    } else {
        tchar prevChar = *curChar_++;
        if (curChar_ != mExpr.end() && _T('=') == *curChar_ &&
            (_T('+') == prevChar || _T('-') == prevChar ||
             _T('*') == prevChar || _T('/') == prevChar ||
             _T('^') == prevChar)) {
            tokens_.push_back(Token(ASSIGN, prevChar + tstring(_T("="))));
        } else {
            --curChar_;
            return false;
        }
    }

    ++curChar_;
    return true;
}

/*!
    Lexical analysis of unit conversions (syntax "[unit1 -> unit2]").

    \exception IncorrectUnitConversionSyntaxException Incorrect conversion syntax.
*/
bool Parser::analyzeUnitConversions()
{
    if (_T('[') == *curChar_) {
        tokens_.push_back(Token(OPENING_SQUARE_BRACKET, _T("[")));
        ++curChar_;

        skipSpaces();

        tstring unit = _T("");
        while (curChar_ != mExpr.end() && isUnitChar(*curChar_))
            unit += *curChar_++;
        if (unit == _T(""))
            throw IncorrectUnitConversionSyntaxException();
        tokens_.push_back(Token(UNIT, unit));

        skipSpaces();

        if (curChar_ != mExpr.end() && _T('-') == *curChar_) {
            ++curChar_;
            if (curChar_ != mExpr.end() && _T('>') == *curChar_) {
                tokens_.push_back(Token(ARROW, _T("->")));
                ++curChar_;
            } else {
                throw IncorrectUnitConversionSyntaxException();
            }
        } else {
            throw IncorrectUnitConversionSyntaxException();
        }

        skipSpaces();

        unit = _T("");
        while (curChar_ != mExpr.end() && isUnitChar(*curChar_)) {
            unit += *curChar_++;
        }
        if (unit == _T("")) {
            throw IncorrectUnitConversionSyntaxException();
        }
        tokens_.push_back(Token(UNIT, unit));

        skipSpaces();

        if (curChar_ != mExpr.end() && _T(']') == *curChar_) {
            tokens_.push_back(Token(CLOSING_SQUARE_BRACKET, _T("]")));
        } else {
            throw IncorrectUnitConversionSyntaxException();
        }
        ++curChar_;
        return true;
    }


    return false;
}

/*!
    Lexical analysis of operators (+, -, *, /, ^), brackets and arrow (->).
*/
bool Parser::analyzeOperators()
{
    if (_T('+') == *curChar_) tokens_.push_back(Token(PLUS, _T("+")));
    else if (_T('-') == *curChar_) tokens_.push_back(Token(MINUS, _T("-")));
    else if (_T('*') == *curChar_) tokens_.push_back(Token(MULTIPLY, _T("*")));
    else if (_T('/') == *curChar_) tokens_.push_back(Token(DIVIDE, _T("/")));
    else if (_T('^') == *curChar_) tokens_.push_back(Token(POWER, _T("^")));
    else if (_T('(') == *curChar_) tokens_.push_back(Token(OPENING_BRACKET, _T("(")));
    else if (_T(')') == *curChar_) tokens_.push_back(Token(CLOSING_BRACKET, _T(")")));
    else if (_T(';') == *curChar_) tokens_.push_back(Token(SEMICOLON, _T(";")));
    else return false;

    ++curChar_;
    return true;
}

/*!
    Lexical analysis of identifiers (function and variable names).
*/
bool Parser::analyzeIdentifiers()
{
    if (isIdentifierChar(*curChar_, true)) {
        tstring identifier;
        identifier = *curChar_++;
        while (curChar_ != mExpr.end() && isIdentifierChar(*curChar_, false)) {
            identifier += *curChar_++;
        }

        tstring imOne = _T("");
        imOne += mContext.numberFormat().imaginaryOneTChar();
        // Imaginary one is not an identifier
        if (identifier == imOne) {
            --curChar_;
            return false;
        }

        tokens_.push_back(Token(IDENTIFIER, identifier));

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

    const tchar decSeparator = mContext.numberFormat().decimalSeparatorTChar();
    const tchar imOne = mContext.numberFormat().imaginaryOneTChar();

    // Check if it is a number
    if (!istdigit(*curChar_) && *curChar_ != decSeparator && *curChar_ != imOne) {
        return false;
    }

    // Process decimal point at the beginning of the number
    if (mExpr.end() != curChar_ && decSeparator == *curChar_) {
        number += *curChar_++;
        thereIsPoint = true;

        if (mExpr.end() == curChar_) {
            throw IncorrectNumberException(number);
        }
    }

    // Process digits
    while (mExpr.end() != curChar_ && istdigit(*curChar_)) {
        number += *curChar_++;
    }

    // Process decimal point in the middle or at the end of the number
    if (mExpr.end() != curChar_ && decSeparator == *curChar_) {
        // Throw an exception if there was a decimal point already
        if (thereIsPoint) {
            throw IncorrectNumberException(number);
        }

        number += *curChar_++;

        // Process digits
        while(mExpr.end() != curChar_ && istdigit(*curChar_)) {
            number += *curChar_++;
        }
    }

    skipSpaces();

    // Process exponential part
    if (mExpr.end() != curChar_ && (_T('e') == *curChar_ || _T('E') == *curChar_)) {
        // Add 'e'
        number += *curChar_++;
        skipSpaces();
        // Add sign after 'e' if it exists
        if (mExpr.end() != curChar_ && (_T('-') == *curChar_ || _T('+') == *curChar_)) {
            number += *curChar_++;
        }
        skipSpaces();
        if (mExpr.end() == curChar_) {
            throw IncorrectNumberException(number);
        }
        // Process exponent digits
        while(mExpr.end() != curChar_ && istdigit(*curChar_)) {
            number += *curChar_++;
        }
    }

    if (number != _T("")) {
        tokens_.push_back(Token(NUMBER, number));
    }

    skipSpaces();

    // Process imaginary one at the end of the number
    if (mExpr.end() != curChar_ && imOne == *curChar_) {
        ++curChar_;
        tokens_.push_back(Token(IMAGINARY_ONE, imOne));
    }

    return true;
}

/*!
    Skip spaces in given expression during lexical analysis.
*/
bool Parser::skipSpaces()
{
    // Check if it is a space
    if (mExpr.end() == curChar_ || !istspace(*curChar_)) {
        return false;
    }

    // Skip spaces
    while (mExpr.end() != curChar_ && istspace(*curChar_)) {
        ++curChar_;
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
    mCurToken = tokens_.begin();

    Complex result = parseAssign();

    if (tokens_.end() == mCurToken) mContext.setResult(result);
    else if (CLOSING_BRACKET == mCurToken->token) throw TooManyClosingBracketsException();
    else throw IncorrectExpressionException();
}

/*!
    Parses variable assignment.

    \exception IncorrectVariableNameException Incorrect var name
*/
Complex Parser::parseAssign()
{
    if (mCurToken != tokens_.end() &&
        (mCurToken->token == IDENTIFIER || mCurToken->token == IMAGINARY_ONE)) {
        tstring name = mCurToken->str;
        ++mCurToken;
        if (mCurToken != tokens_.end() && mCurToken->token == ASSIGN) {
            if (name == _T("e") || name == _T("pi") || name == _T("res") ||
                name == _T("result") || name == _T("i") || name == _T("j") ||
                name == _T("exit") || name == _T("quit") ||
                name == _T("help")) {
                throw IncorrectVariableNameException();
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

    while (mCurToken != tokens_.end()) {
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

    while (mCurToken != tokens_.end()) {
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

    while (mCurToken != tokens_.end()) {
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

    while (mCurToken != tokens_.end()) {
        if (OPENING_SQUARE_BRACKET == mCurToken->token) {
            ++mCurToken;
            if (mCurToken == tokens_.end() || mCurToken->token != UNIT) {
                throw IncorrectUnitConversionSyntaxException();
            }
            tstring unit1 = mCurToken->str;
            ++mCurToken;
            if (mCurToken == tokens_.end() || mCurToken->token != ARROW) {
                throw IncorrectUnitConversionSyntaxException();
            }
            ++mCurToken;
            if (mCurToken == tokens_.end() || mCurToken->token != UNIT) {
                throw IncorrectUnitConversionSyntaxException();
            }
            tstring unit2 = mCurToken->str;
            ++mCurToken;
            if (mCurToken == tokens_.end() || mCurToken->token != CLOSING_SQUARE_BRACKET) {
                throw IncorrectUnitConversionSyntaxException();
            }
            ++mCurToken;

            if (!result.im.isZero()) {
                throw InvalidUnitConversionArgumentException(
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

    while (mCurToken != tokens_.end()) {
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
    if (mCurToken != tokens_.end() && OPENING_BRACKET == mCurToken->token) {
        ++mCurToken;
        Complex result = parseAddSub();
        if (tokens_.end() == mCurToken || CLOSING_BRACKET != mCurToken->token) {
            throw NoClosingBracketException();
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
    if (mCurToken != tokens_.end() && IDENTIFIER == mCurToken->token) {
        tstring name = mCurToken->str;
        ++mCurToken;

        std::vector<Complex> args;

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
            else if (name == _T("sin") && args.size() == 1) return Complex::sin(args[0]);
            else if (name == _T("cos") && args.size() == 1) return Complex::cos(args[0]);
            else if ((name == _T("tan") || name == _T("tg")) && args.size() == 1) return Complex::tan(args[0]);
            else if ((name == _T("cot") || name == _T("ctg")) && args.size() == 1) return Complex::cot(args[0]);
            else if ((name == _T("asin") || name == _T("arcsin")) && args.size() == 1) return Complex::arcsin(args[0]);
            else if ((name == _T("acos") || name == _T("arccos")) && args.size() == 1) return Complex::arccos(args[0]);
            else if ((name == _T("atan") || name == _T("arctan") || name == _T("atg") || name == _T("arctg")) && args.size() == 1) return Complex::arctan(args[0]);
            else if ((name == _T("acot") || name == _T("arccot") || name == _T("actg") || name == _T("arcctg")) && args.size() == 1) return Complex::arccot(args[0]);
            else if (name == _T("sinh") && args.size() == 1) return Complex::sinh(args[0]);
            else if (name == _T("cosh") && args.size() == 1) return Complex::cosh(args[0]);
            else if ((name == _T("tanh") || name == _T("th")) && args.size() == 1) return Complex::tanh(args[0]);
            else if ((name == _T("coth") || name == _T("cth")) && args.size() == 1) return Complex::coth(args[0]);
            else if ((name == _T("asinh") || name == _T("arcsinh")) && args.size() == 1) return Complex::arcsinh(args[0]);
            else if ((name == _T("acosh") || name == _T("arccosh")) && args.size() == 1) return Complex::arccosh(args[0]);
            else if ((name == _T("atanh") || name == _T("arctanh") || name == _T("ath") || name == _T("arcth")) && args.size() == 1) return Complex::arctanh(args[0]);
            else if ((name == _T("acoth") || name == _T("arccoth") || name == _T("acth") || name == _T("arccth")) && args.size() == 1) return Complex::arccoth(args[0]);
            else if (name == _T("ln") && args.size() == 1) return Complex::ln(args[0]);
            else if (name == _T("log2") && args.size() == 1) return Complex::log2(args[0]);
            else if (name == _T("log10") && args.size() == 1) return Complex::log10(args[0]);
            else if (name == _T("exp") && args.size() == 1) return Complex::exp(args[0]);
            else throw UnknownFunctionException(name); // Unknown function

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
    if (mCurToken != tokens_.end() && IDENTIFIER == mCurToken->token) {
        if (_T("pi") == mCurToken->str) {
            ++mCurToken;
            return BigDecimal::PI;
        } else if (_T("e") == mCurToken->str) {
            ++mCurToken;
            return BigDecimal::E;
        } else if (_T("res") == mCurToken->str || _T("result") == mCurToken->str) {
            ++mCurToken;
            if (mContext.resultExists()) return mContext.result();
            else throw ResultDoesNotExistException();
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

    if (mCurToken != tokens_.end() && IMAGINARY_ONE == mCurToken->token) {
        result = 1;
        isComplex = true;
        thereIsResult = true;
        ++mCurToken;
    }

    if (mCurToken != tokens_.end() && NUMBER == mCurToken->token) {
        result = BigDecimal(mCurToken->str);
        thereIsResult = true;
        ++mCurToken;
    }
    
    if (mCurToken != tokens_.end() && IMAGINARY_ONE == mCurToken->token) {
        if (!isComplex) {
            isComplex = true;
            thereIsResult = true;
            ++mCurToken;
        } else {
            throw IncorrectNumberException();
        }
    }
    
    if (thereIsResult) {
        return isComplex ? Complex(0, result) : result;
    }

    throw IncorrectExpressionException();
}

/*!
    Parses function arguments including opening and closing brackets.

    \exception NoClosingBracketException Closing bracket is missing.
*/
bool Parser::parseFunctionArguments(std::vector<Complex> & args)
{
    if (mCurToken != tokens_.end() && OPENING_BRACKET == mCurToken->token) {
        ++mCurToken;
        
        // Parse arguments
        args.push_back(parseAddSub());
        while (mCurToken != tokens_.end() && mCurToken->token == SEMICOLON) {
            ++mCurToken;
            args.push_back(parseAddSub());
        }

        // Check for closing bracket
        if (mCurToken == tokens_.end() || mCurToken->token != CLOSING_BRACKET) {
            throw NoClosingBracketException();
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

} // namespace MaxCalcEngine
