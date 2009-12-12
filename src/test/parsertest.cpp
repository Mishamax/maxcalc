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
#include "parsertest.h"
#include "utility.h"
// Engine
#include "parser.h"
#include "exceptions.h"
// STL
#include <ctime>
#include <cstdlib>
#include <sstream>

void ParserTest::basic()
{
    ParserContext context;
    Parser parser(_T(""), context);
    FAIL_TEST(parser.parse(), "Empty expression", IncorrectExpressionException);
    FAIL_TEST(parser.context().result(), "No result", ResultDoesNotExistException);
    context = parser.context();
    FAIL_TEST(context.result(), "No result", ResultDoesNotExistException);

    PARSER_TEST(parser, _T("0"), 0);
    PARSER_FAIL_TEST(parser, _T("     "), "Empty expression", IncorrectExpressionException);
    PARSER_TEST(parser, _T("  0 "), 0);
}

void ParserTest::numbers()
{
    Parser parser;

    PARSER_TEST(parser, _T("1"), 1);
    PARSER_TEST(parser, _T("0"), 0);
    PARSER_TEST(parser, _T("2.56"), "2.56");
    PARSER_TEST(parser, _T("2e-3"), "0.002");
    PARSER_TEST(parser, _T("3e+4"), 30000);
    PARSER_TEST(parser, _T("2.546e-3"), "0.002546");
    PARSER_TEST(parser, _T("2.11e+1"), "21.1");
    PARSER_TEST(parser, _T(".1"), "0.1");
    PARSER_TEST(parser, _T(".0"), 0);
    PARSER_TEST(parser, _T("0."), 0);
    PARSER_TEST(parser, _T(".256e-2"), "0.00256");
}

void ParserTest::complexNumbers()
{
    Parser parser;

    PARSER_TEST(parser, _T("i"), Complex::i);
    PARSER_TEST(parser, _T("1i"), Complex::i);
    PARSER_TEST(parser, _T("0i"), 0);
    PARSER_TEST(parser, _T("2.56i"), Complex("0", "2.56"));
    PARSER_TEST(parser, _T("2e-3i"), Complex("0", "0.002"));
    PARSER_TEST(parser, _T("3e+4i"), Complex(0, 30000));
    PARSER_TEST(parser, _T("2.546e-3i"), Complex("0", "0.002546"));
    PARSER_TEST(parser, _T("2.11e+1i"), Complex("0", "21.1"));
    PARSER_TEST(parser, _T(".0i"), 0);
    PARSER_TEST(parser, _T("0.i"), 0);
    PARSER_TEST(parser, _T(".1i"), Complex("0", "0.1"));
    PARSER_TEST(parser, _T("1.i"), Complex(0, 1));
    PARSER_TEST(parser, _T(".27863e-2i"), Complex("0", "0.0027863"));
}

void ParserTest::addSub()
{
    Parser parser;

    PARSER_TEST(parser, _T("1-2+1"), 0);
    PARSER_TEST(parser, _T("  1  +  2  "), 3);
    PARSER_TEST(parser, _T("  .1  +  2e-1 - i.1 + 2e-1i  "), Complex("0.3", "0.1"));
    PARSER_TEST(parser, _T("1-2i+1e-2-.2"), Complex("0.81", "-2"));
    PARSER_TEST(parser, _T("0+0"), "0");
    PARSER_TEST(parser, _T("0+1"), "1");
    PARSER_TEST(parser, _T("1+0"), "1");
    PARSER_TEST(parser, _T("1+1"), "2");
}

void ParserTest::mulDiv()
{
    Parser parser;

    PARSER_TEST(parser, _T("1/2"), "0.5");
    PARSER_FAIL_TEST(parser, _T("1 / 0"), "Division by zero", ArithmeticException);
    COMPARE_COMPLEX(parser.context().result(), "0.5");
    PARSER_TEST(parser, _T("1 / i"), -Complex::i);
    PARSER_FAIL_TEST(parser, _T("1 / sin(0)"), "Division by zero", ArithmeticException);
    PARSER_FAIL_TEST(parser, _T("1/sin(pi)"), "Division by zero", ArithmeticException);
    PARSER_FAIL_TEST(parser, _T(" 1 / cos ( pi / 2 ) "), "Division by zero", ArithmeticException);
    PARSER_FAIL_TEST(parser, _T("1 / tan(0)"), "Division by zero", ArithmeticException);
    PARSER_FAIL_TEST(parser, _T("1 / cot(pi/2)"), "Division by zero", ArithmeticException);
}

void ParserTest::unaryPlusMinus()
{
    Parser parser;

    PARSER_TEST(parser, _T("-1"), "-1");
    PARSER_TEST(parser, _T("+1"), "1");
    PARSER_TEST(parser, _T("+++++1"), "1");
    PARSER_TEST(parser, _T("-----1"), "-1");
    PARSER_TEST(parser, _T("------1"), "1");
    PARSER_TEST(parser, _T("+-+-+-+-1"), "1");
    PARSER_TEST(parser, _T("-0"), 0);
    PARSER_TEST(parser, _T("--1"), 1);
    PARSER_TEST(parser, _T("---1"), -1);
    PARSER_TEST(parser, _T("----1"), 1);
    PARSER_TEST(parser, _T("-abs(1)"), -1);
    PARSER_TEST(parser, _T("-abs(-2)"), -2);
    PARSER_TEST(parser, _T("--abs(-3)"), 3);
    PARSER_TEST(parser, _T("---abs(-4)"), -4);
}

void ParserTest::power()
{
    Parser parser;

    PARSER_TEST(parser, _T("2^4"), "16");

    // SpeedCrunch fails this test
    PARSER_TEST(parser, _T("2^2^3"), "64");

    // SpeedCrunch fails this test
    PARSER_TEST(parser, _T("2^3^2"), "64");

    PARSER_TEST(parser, _T("4 ^ 0.5"), "2");

    PARSER_TEST(parser, _T("0^0"), 1);
    PARSER_TEST(parser, _T("1^0"), "1" );
    PARSER_TEST(parser, _T("1^1"), "1" );
    PARSER_TEST(parser, _T("1^2"), "1" );
    PARSER_TEST(parser, _T("1^3"), "1" );

    PARSER_TEST(parser, _T("-1^0"), "1"  );
    PARSER_TEST(parser, _T("-1^1"), "-1" );
    PARSER_TEST(parser, _T("-1^2"), "1"  );
    PARSER_TEST(parser, _T("-1^3"), "-1" );

    PARSER_TEST(parser, _T("2^0"), "1"     );
    PARSER_TEST(parser, _T("2^1"), "2"     );
    PARSER_TEST(parser, _T("2^2"), "4"     );
    PARSER_TEST(parser, _T("2^3"), "8"     );
    PARSER_TEST(parser, _T("2^4"), "16"    );
    PARSER_TEST(parser, _T("2^5"), "32"    );
    PARSER_TEST(parser, _T("2^6"), "64"    );
    PARSER_TEST(parser, _T("2^7"), "128"   );
    PARSER_TEST(parser, _T("2^8"), "256"   );
    PARSER_TEST(parser, _T("2^9"), "512"   );
    PARSER_TEST(parser, _T("2^10"), "1024" );

    PARSER_TEST(parser, _T("-2^0"), "1"     );
    PARSER_TEST(parser, _T("-2^1"), "-2"     );
    PARSER_TEST(parser, _T("-2^2"), "4"     );
    PARSER_TEST(parser, _T("-2^3"), "-8"     );
    PARSER_TEST(parser, _T("-2^4"), "16"    );
    PARSER_TEST(parser, _T("-2^5"), "-32"    );
    PARSER_TEST(parser, _T("-2^6"), "64"    );
    PARSER_TEST(parser, _T("-2^7"), "-128"   );
    PARSER_TEST(parser, _T("-2^8"), "256"   );
    PARSER_TEST(parser, _T("-2^9"), "-512"   );
    PARSER_TEST(parser, _T("-2^10"), "1024" );

    PARSER_TEST(parser, _T("(5-7)^2"),  "4"  );
    PARSER_TEST(parser, _T("-(5-7)^2"), "4" );
}

void ParserTest::powerAndUnaryPlusMinus()
{
    Parser parser;

    PARSER_TEST(parser, _T("2^+4"), "16");
    PARSER_TEST(parser, _T("+2^+2^+3"), "64");
    PARSER_TEST(parser, _T("2^-1"), "0.5");
    PARSER_TEST(parser, _T("-2^-1"), "-0.5");
    PARSER_TEST(parser, _T("2^-2"), "0.25");
    PARSER_TEST(parser, _T("-4 ^ 2"), "16");
    PARSER_TEST(parser, _T("-1^0.5"), Complex(0, 1));
    PARSER_TEST(parser, _T(" - -+- 1 ^ +  ++- - 0.5"), Complex(0, 1));
    PARSER_TEST(parser, _T("-1^0.5^2"), -1);
    PARSER_TEST(parser, _T("-1^-0.5^-2"), -1);
}

void ParserTest::brackets()
{
    Parser parser;

    PARSER_TEST(parser, _T("(1)"), 1);
    PARSER_TEST(parser, _T("2*(1+3)"), 8);
    PARSER_TEST(parser, _T("2^(3^2)"), 512);
    PARSER_TEST(parser, _T("2^-(2)"), "0.25");
    PARSER_FAIL_TEST(parser, _T("(1)(2)"), "Incorrect expression", IncorrectExpressionException);
    PARSER_FAIL_TEST(parser, _T("(1))"), "Too many brackets", TooManyClosingBracketsException);
    PARSER_FAIL_TEST(parser, _T("(1)+2)"), "Too many brackets", TooManyClosingBracketsException);
    PARSER_FAIL_TEST(parser, _T("(1)+2)/2"), "Too many brackets", TooManyClosingBracketsException);
    PARSER_FAIL_TEST(parser, _T("sin(pi/2))"), "Too many brackets", TooManyClosingBracketsException);
    PARSER_FAIL_TEST(parser, _T("sin(pi)/2))"), "Too many brackets", TooManyClosingBracketsException);
    PARSER_FAIL_TEST(parser, _T("sin(cos(pi))+1)"), "Too many brackets", TooManyClosingBracketsException);
}

void ParserTest::constsAndVars()
{
    Parser parser;

    PARSER_FAIL_TEST(parser, _T("res"), "No result", ResultDoesNotExistException);
    PARSER_FAIL_TEST(parser, _T("result"), "No result", ResultDoesNotExistException);
    PARSER_TEST(parser, _T("e"), BigDecimal::E);
    PARSER_TEST(parser, _T("pi"), BigDecimal::PI);
    PARSER_TEST(parser, _T("res"), BigDecimal::PI);
    PARSER_TEST(parser, _T("result"), BigDecimal::PI);
}

void ParserTest::userVars()
{
    Parser parser;

    PARSER_FAIL_TEST(parser, _T("x"), "Unknown variable", UnknownVariableException);
    PARSER_TEST(parser, _T("x=2"), 2);
    PARSER_TEST(parser, _T("x"), 2);
    PARSER_TEST(parser, _T("X=pi/e"), BigDecimal::PI / BigDecimal::E);
    PARSER_TEST(parser, _T("x=pi/e"), BigDecimal::PI / BigDecimal::E);
    PARSER_TEST(parser, _T("  x  =  X  "), BigDecimal::PI / BigDecimal::E);
    PARSER_TEST(parser, _T("Var=e*res"), BigDecimal::PI);
    PARSER_TEST(parser, _T("vAR=VAR*1"), BigDecimal::PI);
    PARSER_TEST(parser, _T("var"), BigDecimal::PI);
    PARSER_TEST(parser, _T("x"), BigDecimal::PI / BigDecimal::E);
    PARSER_TEST(parser, _T("Imaginary1=i"), Complex::i);
    PARSER_TEST(parser, _T("imaginary1"), Complex::i);
    PARSER_TEST(parser, _T("_fh8743yr43_ry847yr_=0"), 0);
    PARSER_TEST(parser, _T("_fh8743yr43_ry847yr_"), 0);
    PARSER_FAIL_TEST(parser, _T("1x=1"), "Incorrect variable name", IncorrectExpressionException);
    PARSER_TEST(parser, _T("_1x=0"), 0);
    PARSER_TEST(parser, _T("res"), 0);
    PARSER_TEST(parser, _T("_res=10"), 10);
    PARSER_TEST(parser, _T("_res"), 10);
    PARSER_FAIL_TEST(parser, _T("res=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("e=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("pi=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("i=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("j=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("result=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("quit=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("exit=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_FAIL_TEST(parser, _T("help=1"), "Incorrect variable name", IncorrectVariableNameException);
    PARSER_TEST(parser, _T("res_=10"), 10);
    PARSER_TEST(parser, _T("res_"), 10);
    PARSER_TEST(parser, _T("erespii=101"), 101);
    PARSER_TEST(parser, _T("erespii"), 101);

    // Multiple assignment
    PARSER_TEST(parser, _T("x = y = 1"), 1);
    PARSER_TEST(parser, _T("x"), 1);
    PARSER_TEST(parser, _T("y"), 1);
    PARSER_FAIL_TEST(parser, _T("x = y+1 = 1"), "Incorrent expression", IncorrectExpressionException);
    PARSER_FAIL_TEST(parser, _T("x/2 = y = 1"), "Incorrent expression", IncorrectExpressionException);

    tstring var = _T("");
    for (int i = 0; i < 100; ++i) {
        std::tstringstream ss;
        ss << i;
        var += _T("x");
        var += ss.str();
        var += _T("=");
    }
    var += _T("11.1+1.1i");
    PARSER_TEST(parser, var, Complex("11.1", "1.1"));
    for (int i = 0; i < 100; ++i) {
        std::tstringstream ss;
        ss << i;
        var = _T("x");
        var += ss.str();
        PARSER_TEST(parser, var, Complex("11.1", "1.1"));
    }

    // C-like assignment operators
    PARSER_TEST(parser, _T("x = 2"), 2);
    PARSER_TEST(parser, _T("x *= 2"), 4);
    PARSER_TEST(parser, _T("x /= 2"), 2);
    PARSER_TEST(parser, _T("x += 2"), 4);
    PARSER_TEST(parser, _T("x -= 2"), 2);
    PARSER_TEST(parser, _T("x ^= 2"), 4);
    PARSER_TEST(parser, _T("x *= y = z = 2"), 8);
    PARSER_TEST(parser, _T("x *= y += z ^= 2"), 48);
    PARSER_FAIL_TEST(parser, _T("new_var *= 2"), "Unknown variable", UnknownVariableException);
    PARSER_FAIL_TEST(parser, _T("x = new_var *= 2"), "Unknown variable", UnknownVariableException);
}

void ParserTest::functions()
{
    Parser parser;

    PARSER_TEST(parser, _T("abs(-1)"), 1);
    PARSER_FAIL_TEST(parser, _T("abs(1"), "Incorrect expression", NoClosingBracketException);
    PARSER_TEST(parser, _T("sqr(-2)"), 4);
    PARSER_TEST(parser, _T("sqrt(-4)"), Complex::i * 2);
    PARSER_TEST(parser, _T("pow(2; -2)"), "0.25");
    PARSER_FAIL_TEST(parser, _T("pow(2 -2)"), "Incorrect expression", UnknownFunctionException);
    PARSER_FAIL_TEST(parser, _T("pow(2;; -2)"), "Incorrect expression", IncorrectExpressionException);
    PARSER_FAIL_TEST(parser, _T("pow(2; -2; 3)"), "Incorrect expression", UnknownFunctionException);
    PARSER_FAIL_TEST(parser, _T("pow(2; 3"), "Incorrect expression", NoClosingBracketException);
    PARSER_FAIL_TEST(parser, _T("sin 3"), "Incorrect expression", UnknownVariableException);
    PARSER_FAIL_TEST(parser, _T("sin3)"), "Incorrect expression", UnknownVariableException);
    PARSER_FAIL_TEST(parser, _T("sin 3)"), "Incorrect expression", UnknownVariableException);
}

void ParserTest::functionsBasic()
{
    Parser parser;

    PARSER_TEST(parser, _T("ABS(0)"),    "0" );
    PARSER_TEST(parser, _T("ABS(1)"),    "1" );
    PARSER_TEST(parser, _T("ABS(-1)"),   "1" );
    PARSER_TEST(parser, _T("ABS(--1)"),  "1" );
    PARSER_TEST(parser, _T("ABS(---1)"), "1" );
    PARSER_TEST(parser, _T("ABS((1))"),  "1" );
    PARSER_TEST(parser, _T("ABS((-1))"), "1" );

    PARSER_TEST(parser, _T("ABS(1/2)"),   "0.5" );
    PARSER_TEST(parser, _T("ABS(-1/2)"),  "0.5" );
    PARSER_TEST(parser, _T("ABS(-1/-2)"), "0.5" );
    PARSER_TEST(parser, _T("ABS(1/-2)"),  "0.5" );
}

void ParserTest::functionsTrig()
{
    Parser parser;

    PARSER_TEST(parser, _T("pi"),    "3.14159265358979323846264338327950288419716939937511" );

    PARSER_TEST(parser, _T("sin(0)"), "0" );
    PARSER_TEST(parser, _T("cos(0)"), "1" );
    PARSER_TEST(parser, _T("tan(0)"), "0" );

    PARSER_TEST(parser, _T("sin(pi)"), "0"  );
    PARSER_TEST(parser, _T("cos(pi)"), "-1" );
    PARSER_TEST(parser, _T("tan(pi)"), "0"  );

    PARSER_TEST(parser, _T("sin(-pi)"), "0"  );
    PARSER_TEST(parser, _T("cos(-pi)"), "-1" );
    PARSER_TEST(parser, _T("tan(-pi)"), "0"  );

    PARSER_TEST(parser, _T("sin(--pi)"), "0"  );
    PARSER_TEST(parser, _T("cos(--pi)"), "-1" );
    PARSER_TEST(parser, _T("tan(--pi)"), "0"  );

    PARSER_TEST(parser, _T("sin(pi/2)"), "1" );
    PARSER_TEST(parser, _T("cos(pi/2)"), "0" );

    PARSER_TEST(parser, _T("sin(-pi/2)"), "-1" );
    PARSER_TEST(parser, _T("cos(-pi/2)"), "0"  );

    PARSER_TEST(parser, _T("sin(-pi/2) + sin(pi/2)"), "0"  );
    PARSER_TEST(parser, _T("sin(-pi/2) - sin(pi/2)"), "-2" );
    PARSER_TEST(parser, _T("cos(-pi/2) + cos(pi/2)"), "0"  );
    PARSER_TEST(parser, _T("cos(-pi/2) - cos(pi/2)"), "0"  );

    PARSER_TEST(parser, _T("asin(sin(1))"), "1" );
    PARSER_TEST(parser, _T("acos(cos(1))"), "1" );
}

void ParserTest::functionsLog()
{
    Parser parser;

    PARSER_TEST(parser, _T("exp((1)/2) + exp((1)/2)"),    "3.29744254140025629369730157562832714330755220142030" );
}

void ParserTest::fails()
{
    Parser parser;
    PARSER_FAIL_TEST(parser, _T("/"), "Incorrect expression", IncorrectExpressionException);
    FAIL_TEST(parser.context().result(), "No result", ResultDoesNotExistException);
    PARSER_FAIL_TEST(parser, _T("1+"), "Incorrect expression", IncorrectExpressionException);
    PARSER_FAIL_TEST(parser, _T(" 1 * "), "Incorrect expression", IncorrectExpressionException);
}

void ParserTest::realWorld()
{
    Parser parser;

    PARSER_TEST(parser, _T("-0.25-2.43*sqrt(0.52)"), "-2.00229791987549879645594153599066102987813013488879");
    PARSER_TEST(parser, _T("-1/(1000-1)"), "-0.001001001001001001001001001001001001001001001001001001001001001001");
    PARSER_TEST(parser, _T("(1000^2)/(((1000-1)^2)/(1000-2))"), "999.99899799699599499399299199098998898798698598498398");
    PARSER_TEST(parser, _T("sqrt(res)"), "31.62276075862124968674132991108077921161867982304985");
    PARSER_TEST(parser, _T("sqrt((1000^2)/(((1000-1)^2)*(1000-2)))"), "0.03168613302467059086847828648404887696554977938181347253796979483");
    PARSER_TEST(parser, _T("sqrt(result)"), "0.1780059915414944421035302560547959211378834659058566185708033849");
    PARSER_TEST(parser, _T("19*19"), "361");
    PARSER_TEST(parser, _T("81+16*18"), "369");
    PARSER_TEST(parser, _T("18 * 81"), "1458");
    PARSER_TEST(parser, _T("(1440-171)/361"), "3.51523545706371191135734072022160664819944598337950");
    PARSER_TEST(parser, _T("(2708-1064-1269)/361"), "1.03878116343490304709141274238227146814404432132964");
    PARSER_TEST(parser, _T("0.88*0.95"), "0.836");
    PARSER_TEST(parser, _T("log10(110)/1.05"), "1.944183509674500038809714258326689754006383038539479137710989542");
    PARSER_TEST(parser, _T("log10(110)/1.0528748"), "1.93887505443023713811955606805578805923240084240448");
    PARSER_TEST(parser, _T("0.4444 + 1.084*exp(-0.2034*2) - 0.09168*sin(1*2+0.1123) - 0.565*exp(-1.067*2) + 0.06262*exp(-5.086*2) - 0.01573*exp(-8.578*2) + 0.000413*exp(-18.07*2)"), "1.02066622043541853227435085151712578946216128386806");
    PARSER_TEST(parser, _T("2/0.05"), "40");
    PARSER_TEST(parser, _T("0.4444 + 1.084*(-0.2034)*exp(-0.2034*2) - 0.09168*cos(1*2+0.1123) - 0.565*(-1.067)*exp(-1.067*2) + 0.06262*(-5.086)*exp(-5.086*2) - 0.01573*(-8.578)*exp(-8.578*2) + 0.000413*(-18.07)*exp(-18.07*2)"), "0.41620335366185292064835042124058419491838127532895");
    PARSER_TEST(parser, _T("0.4444 + 1.084*((-0.2034)^4)*exp(-0.2034*2) -0.09168*sin(1*2+0.1123) - 0.565*((-1.067)^4)*exp(-1.067*2) + 0.06262*((-5.086)^4)*exp(-5.086*2) - 0.01573*((-8.578)^4)*exp(-8.578*2) + 0.000413*((-18.07)^4)*exp(-18.07*2)"), "0.28198960676373864448299646367266679390878957236408");
    PARSER_TEST(parser, _T("(0.02+9*0.01+0.1)/19"), "0.01105263157894736842105263157894736842105263157894736842105263158");
    PARSER_TEST(parser, _T("((492-0.25*1000)^2)/(0.25*1000)"), "234.25600000000000000000000000000000000000000000000000");
    PARSER_TEST(parser, _T("0.6396*ln(0.9784)^2*(0.9784)^(3*10) - 0.121*ln(0.911)^2*(0.911)^(3*10) - 0.7423*ln(0.9048)^2*(0.9048)^(3*10) + 0.4112*ln(0.5778)^2*(0.5778)^(3*10) - 0.187*ln(0.4351)^2*(0.4351)^(3*10) - 0.0004501*ln(0.1583)^2*(0.1583)^(3*10)"), "-2.751631655951235923961285515679582556626416401198469537065650108e-4");
}

void ParserTest::unitConversions()
{
    Parser parser;

    PARSER_FAIL_TEST(parser, _T("1[qwe]"), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("1[qwe"), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("1["), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("1[in->]"), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("1[in-]"), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("[in->ft]"), "Incorrect expression", IncorrectExpressionException);
    PARSER_FAIL_TEST(parser, _T("0 [in->ft] [c->f] [ft->in"), "Incorrect conversion", IncorrectUnitConversionSyntaxException);
    PARSER_FAIL_TEST(parser, _T("0[unit->unit]"), "Unknown unit", UnknownUnitException);
    PARSER_FAIL_TEST(parser, _T("0[unit->km]"), "Unknown unit", UnknownUnitException);
    PARSER_FAIL_TEST(parser, _T("0[mile->unit]"), "Unknown unit", UnknownUnitException);
    PARSER_FAIL_TEST(parser, _T("0[in->c]"), "Unknown conversion", UnknownUnitConversionException);
    PARSER_FAIL_TEST(parser, _T("i[in->c]"), "Invalid conversion argument", InvalidUnitConversionArgumentException);
    PARSER_TEST(parser, _T("1[ft->in]"), "12");
    PARSER_TEST(parser, _T("  1  [  ft  ->  in  ]  "), "12");
    PARSER_TEST(parser, _T("0 [c->f]"), "32");
    PARSER_TEST(parser, _T("0 [in->ft] [c->f] [ft->in]"), "384");
    PARSER_TEST(parser, _T("0 [km/h->mi/h]"), "0");
    PARSER_TEST(parser, _T("0 [knot->km/h]"), "0");
    PARSER_TEST(parser, _T("0 [kNoT->kM/H]"), "0");
    PARSER_TEST(parser, _T("1[c->f]"), "33.8");
    PARSER_TEST(parser, _T("-1[c->f]"), "30.2");
    PARSER_TEST(parser, _T("(-1)[c->f]"), "30.2");
    PARSER_TEST(parser, _T("-(1[c->f])"), "-33.8");
    PARSER_TEST(parser, _T("2^1[ft->in]"), "4096");
    PARSER_TEST(parser, _T("(2^1)[ft->in]"), "24");
}

void ParserTest::stress()
{
    Parser parser;

    PARSER_TEST(parser, _T("0"), "0");
    for (int i = 0; i < 1000; ++i) {
        PARSER_TEST(parser, _T("res*2"), i*2);
        PARSER_TEST(parser, _T("res/2"), i);
        PARSER_TEST(parser, _T("res-1"), i-1);
        PARSER_TEST(parser, _T("res+2"), i+1);
    }
}

void ParserTest::random()
{
    Parser parser;
    tchar expr[56];

    srand((unsigned)time(0));
    for (int i = 0; i < 50; ++i) {
        int length = 5 + rand() % 50;
        for (int i = 0; i < length; ++i) {
            expr[i] = (tchar)rand();
        }
        expr[length] = 0;
        PARSER_FAIL_TEST(parser, expr, "Random input passed", MaxCalcException);
    }
}
