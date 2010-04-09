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
#include "complextest.h"
#include "utility.h"
// MaxCalcEngine
#include "complex.h"
#include "exceptions.h"
#include "constants.h"
// STL
#include <string>


void ComplexTest::complexFormatDefault()
{
    ComplexFormat actual;

    COMPARE(actual.precision, Constants::DEFAULT_IO_PRECISION);
    COMPARE(actual.numberFormat, ComplexFormat::GENERAL_FORMAT);
    COMPARE(actual.exponentCase, ComplexFormat::UPPER_CASE_EXPONENT);
    COMPARE(actual.decimalSeparator, ComplexFormat::DOT_SEPARATOR);
    COMPARE(actual.imaginaryOneChar(), 'i');
}

void ComplexTest::fromString()
{
    Complex num(std::string("1e-5"), std::string("-1e+2"));

    COMPARE_BIGDECIMAL(num.re, BigDecimal("0.00001"));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-100));
}

void ComplexTest::fromCharStr()
{
    Complex num("1e-5", "-1e+2");

    COMPARE_BIGDECIMAL(num.re, BigDecimal(std::string("0.00001")));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-100));
}


void ComplexTest::fromWideString()
{
    Complex num(std::wstring(L"1e-5"), std::wstring(L"-1e+2"));

    COMPARE_BIGDECIMAL(num.re, BigDecimal("0.00001"));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-100));
}

void ComplexTest::fromWideCharStr()
{
    Complex num(L"1e-5", L"-1e+2");

    COMPARE_BIGDECIMAL(num.re, BigDecimal(std::string("0.00001")));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-100));
}

void ComplexTest::fromBigDecimal()
{
    Complex num(BigDecimal(10), BigDecimal(-5));

    COMPARE_BIGDECIMAL(num.re, BigDecimal(10));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-5));
}

void ComplexTest::fromInt()
{
    Complex num;
    COMPARE_BIGDECIMAL(num.re, BigDecimal(0));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(0));

    num = Complex(10);
    COMPARE_BIGDECIMAL(num.re, BigDecimal(10));

    num = Complex(20, 10);
    COMPARE_BIGDECIMAL(num.re, BigDecimal(20));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(10));
}

void ComplexTest::fromUInt()
{
    Complex num = Complex(20u, 10u);

    COMPARE_BIGDECIMAL(num.re, BigDecimal(20));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(10));
}

void ComplexTest::toString()
{
    COMPARE(Complex().toString().c_str(), "0");
    COMPARE(Complex::i.toString().c_str(), "0 + 1i");
    COMPARE(Complex(1).toString().c_str(), "1");
    COMPARE(Complex(-1).toString().c_str(), "-1");
    COMPARE(Complex(-10000).toString().c_str(), "-10000");
    COMPARE(Complex(1000).toString().c_str(), "1000");
    COMPARE(Complex(0, 1).toString().c_str(), "0 + 1i");
    COMPARE(Complex(0, -1).toString().c_str(), "0 - 1i");
    COMPARE(Complex(0, -1000).toString().c_str(), "0 - 1000i");
    COMPARE(Complex(0, 10000).toString().c_str(), "0 + 10000i");
    COMPARE(Complex("0.512").toString().c_str(), "0.512");
    COMPARE(Complex("-0.0246").toString().c_str(), "-0.0246");
    COMPARE(Complex("0", "0.512").toString().c_str(), "0 + 0.512i");
    COMPARE(Complex("0", "-0.0246").toString().c_str(), "0 - 0.0246i");
    COMPARE(Complex(1, 1).toString().c_str(), "1 + 1i");
    COMPARE(Complex(1, -1).toString().c_str(), "1 - 1i");
    COMPARE(Complex(1000, 1).toString().c_str(), "1000 + 1i");
    COMPARE(Complex(1, 10000).toString().c_str(), "1 + 10000i");
    COMPARE(Complex(-1, 1000).toString().c_str(), "-1 + 1000i");
    COMPARE(Complex(100000, -1).toString().c_str(), "100000 - 1i");
    COMPARE(Complex(100000, 1000000).toString().c_str(), "100000 + 1E+6i");
    COMPARE(Complex(-1, -1).toString().c_str(), "-1 - 1i");
    COMPARE(Complex("0.512", "0.256").toString().c_str(), "0.512 + 0.256i");
    COMPARE(Complex("-0.512", "0.256").toString().c_str(), "-0.512 + 0.256i");
    COMPARE(Complex("0.512", "-0.256").toString().c_str(), "0.512 - 0.256i");
    COMPARE(Complex("-0.512", "-0.256").toString().c_str(), "-0.512 - 0.256i");
}

void ComplexTest::toWideString()
{
    COMPARE(Complex("0.512").toWideString(), std::wstring(L"0.512"));
    COMPARE(Complex(L"-0.0246").toWideString(), std::wstring(L"-0.0246"));
    COMPARE(Complex("0.512", "0.256").toWideString(), std::wstring(L"0.512 + 0.256i"));
    COMPARE(Complex(L"-0.512", L"0.256").toWideString(), std::wstring(L"-0.512 + 0.256i"));
    COMPARE(Complex("0.512", "-0.256").toWideString(), std::wstring(L"0.512 - 0.256i"));
    COMPARE(Complex(L"-0.512", L"-0.256").toWideString(), std::wstring(L"-0.512 - 0.256i"));
}

void ComplexTest::unaryOperators()
{
    Complex num(1, 1);

    num = +num;
    COMPARE_BIGDECIMAL(num.re, BigDecimal(1));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(1));

    num = -num;
    COMPARE_BIGDECIMAL(num.re, BigDecimal(-1));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(-1));

    num = -Complex("0", "-10E+1");
    COMPARE_BIGDECIMAL(num.re, BigDecimal(0));
    COMPARE_BIGDECIMAL(num.im, BigDecimal(100));
}

void ComplexTest::binaryOperators()
{
    Complex num1(1, 2), num2(3, 4);
    Complex result = num1 + num2;

    COMPARE_BIGDECIMAL(result.re, BigDecimal(4));
    COMPARE_BIGDECIMAL(result.im, BigDecimal(6));

    result = num1 - num2;
    COMPARE_BIGDECIMAL(result.re, BigDecimal(-2));
    COMPARE_BIGDECIMAL(result.im, BigDecimal(-2));

    result = num1 * num2;
    COMPARE_BIGDECIMAL(result.re, BigDecimal(-5));
    COMPARE_BIGDECIMAL(result.im, BigDecimal(10));

    result = num1 / num2;
    COMPARE_BIGDECIMAL(result.re.toString().c_str(), "0.44");
    COMPARE_BIGDECIMAL(result.im.toString().c_str(), "0.08");

    COMPARE_COMPLEX(Complex(1) / Complex::i, -Complex::i);

    FAIL_TEST(num1 / Complex(), "Division by zero", ArithmeticException)

    result = 1;
    result += Complex::i;
    COMPARE_COMPLEX(result, Complex(1, 1));

    result -= Complex::i;
    COMPARE_COMPLEX(result, 1);

    result = 1;
    result /= Complex::i;
    COMPARE_COMPLEX(result, -Complex::i);

    result = 1;
    result *= 2;
    COMPARE_COMPLEX(result, 2);

    result *= Complex::i;
    COMPARE_COMPLEX(result, Complex::i * 2);

    FAIL_TEST(result /= 0, "Division by zero", ArithmeticException);

    result /= 2;
    COMPARE_COMPLEX(result, Complex::i);
    result += 2;
    COMPARE_COMPLEX(result, Complex(2, 1));
    result -= 2;
    result -= Complex::i;
    COMPARE_COMPLEX(result, 0);
}

void ComplexTest::comparisonOperators()
{
    VERIFY(Complex() == Complex());

    Complex num1(1, 2), num2(1, 2);
    VERIFY(num1 == num2);

    num2 = 1;
    VERIFY(num1 != num2);

    num2 = Complex(0, 2);
    VERIFY(num1 != num2);
}

void ComplexTest::isZero()
{
    VERIFY(Complex().isZero());
    VERIFY(!Complex(1).isZero());
    VERIFY(!Complex(0, 1).isZero());
    VERIFY(!Complex::i.isZero());
    VERIFY(!Complex(-1, -1).isZero());
    VERIFY(Complex(0, 0).isZero());
}

void ComplexTest::sqr()
{
    COMPARE_BIGDECIMAL(Complex::sqr(Complex()), BigDecimal());
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3, 4)), BigDecimal(25));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(-3)), BigDecimal(9));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3)), BigDecimal(9));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(-3, -4)), BigDecimal(25));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3, -4)), BigDecimal(25));
}

void ComplexTest::abs()
{
    COMPARE_BIGDECIMAL(Complex::abs(Complex(0)), BigDecimal(0));
    COMPARE_BIGDECIMAL(Complex::abs(Complex(3, 4)), BigDecimal(5));
    COMPARE_BIGDECIMAL(Complex::abs(Complex(-3)), BigDecimal(3));
    COMPARE_BIGDECIMAL(Complex::abs(Complex(3)), BigDecimal(3));
    COMPARE_BIGDECIMAL(Complex::abs(Complex(-3, -4)), BigDecimal(5));
    COMPARE_BIGDECIMAL(Complex::abs(Complex(3, -4)), BigDecimal(5));
}

void ComplexTest::arg()
{
    COMPARE_BIGDECIMAL(Complex::arg(Complex()), BigDecimal(0));
    COMPARE_BIGDECIMAL(Complex::arg(Complex(-3)), BigDecimal::PI);
    COMPARE_BIGDECIMAL(Complex::arg(Complex(3)), BigDecimal(0));
    COMPARE_BIGDECIMAL(Complex::arg(Complex(0, -4)), -BigDecimal::PI / 2);
    COMPARE_BIGDECIMAL(Complex::arg(Complex(0, 4)), BigDecimal::PI / 2);

    COMPARE_BIGDECIMAL(Complex::arg(Complex("2.56", "2.56")), BigDecimal::PI / 4);
    COMPARE_BIGDECIMAL(Complex::arg(Complex("-2.56", "2.56")), BigDecimal::PI * 3 / 4);
    COMPARE_BIGDECIMAL(Complex::arg(Complex("2.56", "-2.56")), -BigDecimal::PI / 4);
    COMPARE_BIGDECIMAL(Complex::arg(Complex("-2.56", "-2.56")), -BigDecimal::PI * 3 / 4);
}

void ComplexTest::factorial()
{
    COMPARE_COMPLEX(Complex::factorial(0), Complex(1));
    COMPARE_COMPLEX(Complex::factorial(1), Complex(1));
    COMPARE_COMPLEX(Complex::factorial(2), Complex(2));
    COMPARE_COMPLEX(Complex::factorial(3), Complex(6));
    FAIL_TEST(Complex::factorial("0.1"), "Invalid factorial", InvalidArgumentException);
    FAIL_TEST(Complex::factorial(Complex::i), "Invalid factorial", InvalidArgumentException);
    FAIL_TEST(Complex::factorial(-Complex::i), "Invalid factorial", InvalidArgumentException);
}

void ComplexTest::ln()
{
    COMPARE_COMPLEX(Complex::ln(1), Complex());
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::E)), Complex(1));
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::sqr(BigDecimal::E))), Complex(2));
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::sqrt(BigDecimal::E))), Complex("0.5"));
    COMPARE_COMPLEX(Complex::ln(Complex::i), Complex(0, BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::ln(-1), Complex(0, BigDecimal::PI));
    COMPARE_COMPLEX(Complex::ln(Complex::i * Complex::i), Complex(0, BigDecimal::PI));
    COMPARE_COMPLEX(Complex::ln(Complex(0, -1)), Complex(0, -BigDecimal::PI / 2));
    FAIL_TEST(Complex::ln(0), "Logarithm of zero", InvalidArgumentException);
}

void ComplexTest::log2()
{
    COMPARE_COMPLEX(Complex::log2(1), Complex());
    COMPARE_COMPLEX(Complex::log2(2), Complex(1));
    COMPARE_COMPLEX(Complex::log2(4), Complex(2));
    COMPARE_COMPLEX(Complex::log2(Complex(BigDecimal::sqrt(2))), Complex("0.5"));
    COMPARE_COMPLEX(Complex::log2(Complex::i), Complex("0", "2.266180070913596904813841472858333405085930733861897792093008274"));
    COMPARE_COMPLEX(Complex::log2(-1), Complex("0", "4.532360141827193809627682945716666810171861467723795584186016548"));
    COMPARE_COMPLEX(Complex::log2(Complex::i * Complex::i), Complex("0", "4.532360141827193809627682945716666810171861467723795584186016548"));
    COMPARE_COMPLEX(Complex::log2(Complex(0, -1)), Complex("0", "-2.266180070913596904813841472858333405085930733861897792093008274"));
    FAIL_TEST(Complex::log2(0), "Logarithm of zero", InvalidArgumentException);
}

void ComplexTest::log10()
{
    COMPARE_COMPLEX(Complex::log10(1), Complex());
    COMPARE_COMPLEX(Complex::log10(10), Complex(1));
    COMPARE_COMPLEX(Complex::log10(100), Complex(2));
    COMPARE_COMPLEX(Complex::log10(Complex(BigDecimal::sqrt(10))), Complex("0.5"));
    COMPARE_COMPLEX(Complex::log10(Complex::i), Complex("0", "0.682188176920920673742891812715677885105063741861962699950427366"));
    COMPARE_COMPLEX(Complex::log10(-1), Complex("0", "1.364376353841841347485783625431355770210127483723925399900854732"));
    COMPARE_COMPLEX(Complex::log10(Complex::i * Complex::i), Complex("0", "1.364376353841841347485783625431355770210127483723925399900854732"));
    COMPARE_COMPLEX(Complex::log10(Complex(0, -1)), Complex("0", "-0.682188176920920673742891812715677885105063741861962699950427366"));
    FAIL_TEST(Complex::log10(0), "Logarithm of zero", InvalidArgumentException);
}

void ComplexTest::exp()
{
    COMPARE_COMPLEX(Complex::exp(0), Complex(1));
    COMPARE_COMPLEX(Complex::exp(1), BigDecimal::E);
    COMPARE_COMPLEX(Complex::exp(2), Complex::pow(BigDecimal::E, 2));
    COMPARE_COMPLEX(Complex::exp(-1), Complex("0.3678794411714423215955237701614608674458111310317678345078368017"));
    COMPARE_COMPLEX(Complex::exp(Complex::i), Complex("0.5403023058681397174009366074429766037323104206179222276700972554", "0.84147098480789650665250232163029899962256306079837106567275171"));
    COMPARE_COMPLEX(Complex::exp(-Complex::i), Complex("0.5403023058681397174009366074429766037323104206179222276700972554", "-0.84147098480789650665250232163029899962256306079837106567275171"));
}

void ComplexTest::pow()
{
    COMPARE_COMPLEX(Complex::pow(1, 1), Complex(1));
    COMPARE_COMPLEX(Complex::pow(0, 1), Complex(0));
    COMPARE_COMPLEX(Complex::pow(1, 0), Complex(1));
    COMPARE_COMPLEX(Complex::pow(0, 0), Complex(1));
    COMPARE_COMPLEX(Complex::pow(1, 2), Complex(1));
    COMPARE_COMPLEX(Complex::pow(2, 1), Complex(2));
    COMPARE_COMPLEX(Complex::pow(2, 4), Complex(16));
    COMPARE_COMPLEX(Complex::pow(-1, 3), Complex(-1));
    COMPARE_COMPLEX(Complex::pow(-1, 4), Complex(1));
    COMPARE_COMPLEX(Complex::pow(2, -2), Complex("0.25"));
    COMPARE_COMPLEX(Complex::pow(-2, -2), Complex("0.25"));
    COMPARE_COMPLEX(Complex::pow(-2, -3), Complex("-0.125"));
    COMPARE_COMPLEX(Complex::pow(0, -2), Complex(0));
    COMPARE_COMPLEX(Complex::pow(-1, 0), Complex(1));
    COMPARE_COMPLEX(Complex::pow(-1, -1), Complex(-1));
    COMPARE_COMPLEX(Complex::pow(Complex::i, 2), Complex(-1));
    COMPARE_COMPLEX(Complex::pow(Complex::i, 3), -Complex::i);
    COMPARE_COMPLEX(Complex::pow(-1, "0.5"), Complex::i);
}

void ComplexTest::sqrt()
{
    COMPARE_COMPLEX(Complex::sqrt(0), Complex(0));
    COMPARE_COMPLEX(Complex::sqrt(1), Complex(1));
    COMPARE_COMPLEX(Complex::sqrt(2), Complex("1.414213562373095048801688724209698078569671875376948073176679738"));
    COMPARE_COMPLEX(Complex::sqrt(-1), Complex::i);
    COMPARE_COMPLEX(Complex::sqrt(Complex::i), Complex(BigDecimal::sqrt(2) / 2, BigDecimal::sqrt(2) / 2));
    COMPARE_COMPLEX(Complex::sqrt(-Complex::i), Complex(BigDecimal::sqrt(2) / 2, -BigDecimal::sqrt(2) / 2));
}

void ComplexTest::sin()
{
    COMPARE_COMPLEX(Complex::sin(0), Complex(0));
    COMPARE_COMPLEX(Complex::sin(BigDecimal::PI / 2), Complex(1));
    COMPARE_COMPLEX(Complex::sin(BigDecimal::PI), Complex(0));
    COMPARE_COMPLEX(Complex::sin(BigDecimal::PI * 3 / 2), Complex(-1));
    COMPARE_COMPLEX(Complex::sin(-BigDecimal::PI / 2), Complex(-1));
    COMPARE_COMPLEX(Complex::sin(Complex::i), Complex("0", "1.175201193643801456882381850595600815155717981334095870229565413"));
    COMPARE_COMPLEX(Complex::sin(-Complex::i), Complex("0", "-1.175201193643801456882381850595600815155717981334095870229565413"));
}

void ComplexTest::cos()
{
    COMPARE_COMPLEX(Complex::cos(0), Complex(1));
    COMPARE_COMPLEX(Complex::cos(BigDecimal::PI / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cos(BigDecimal::PI), Complex(-1));
    COMPARE_COMPLEX(Complex::cos(BigDecimal::PI * 3 / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cos(-BigDecimal::PI / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cos(Complex::i), Complex("1.543080634815243778477905620757061682601529112365863704737402215"));
    COMPARE_COMPLEX(Complex::cos(-Complex::i), Complex("1.543080634815243778477905620757061682601529112365863704737402215"));
}

void ComplexTest::tan()
{
    COMPARE_COMPLEX(Complex::tan(0), Complex(0));
    FAIL_TEST(Complex::tan(BigDecimal::PI / 2), "tan == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::tan(BigDecimal::PI), Complex(0));
    FAIL_TEST(Complex::tan(BigDecimal::PI * 3 / 2), "tan == infinity", InvalidArgumentException);
    FAIL_TEST(Complex::tan(-BigDecimal::PI / 2), "tan == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::tan(Complex::i), Complex("0", "0.7615941559557648881194582826047935904127685972579365515968105001"));
    COMPARE_COMPLEX(Complex::tan(-Complex::i), Complex("0", "-0.7615941559557648881194582826047935904127685972579365515968105001"));
}

void ComplexTest::cot()
{
    FAIL_TEST(Complex::cot(0), "cot == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::cot(BigDecimal::PI / 2), Complex(0));
    FAIL_TEST(Complex::cot(BigDecimal::PI), "cot == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::cot(BigDecimal::PI * 3 / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cot(-BigDecimal::PI / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cot(Complex::i), Complex("0", "-1.313035285499331303636161246930847832912013941240452655543152968"));
    COMPARE_COMPLEX(Complex::cot(-Complex::i), Complex("0", "1.313035285499331303636161246930847832912013941240452655543152968"));
}

void ComplexTest::arcsin()
{
    COMPARE_COMPLEX(Complex::arcsin(0), Complex(0));
    COMPARE_COMPLEX(Complex::arcsin(1), Complex(BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::arcsin(-1), Complex(-BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::arcsin(Complex("0", "1.175201193643801456882381850595600815155717981334095870229565413")), Complex::i);
    COMPARE_COMPLEX(Complex::arcsin(Complex("0", "-1.175201193643801456882381850595600815155717981334095870229565413")), -Complex::i);
    COMPARE_COMPLEX(Complex::arcsin(Complex::i), Complex("0", "0.8813735870195430252326093249797923090281603282616354107532956087"));
    COMPARE_COMPLEX(Complex::arcsin(-Complex::i), Complex("0", "-0.8813735870195430252326093249797923090281603282616354107532956087"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(1, 1)), Complex("0.6662394324925152551040048959777927206674901387259478428314738428", "1.061275061905035652033018916213573485806785498938633696397210282"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(1, -1)), Complex("0.6662394324925152551040048959777927206674901387259478428314738428", "-1.061275061905035652033018916213573485806785498938633696397210282"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(-1, 1)), Complex("-0.6662394324925152551040048959777927206674901387259478428314738428", "1.061275061905035652033018916213573485806785498938633696397210282"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(-1, -1)), Complex("-0.6662394324925152551040048959777927206674901387259478428314738428", "-1.061275061905035652033018916213573485806785498938633696397210282"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(5, 5)), Complex("0.7803985799853841227939993494273059497362726239558230753554575366", "2.649196177806471149607808531682981367767787003143897238035883266"));
    COMPARE_COMPLEX(Complex::arcsin(Complex(-5, -5)), Complex("-0.7803985799853841227939993494273059497362726239558230753554575366", "-2.649196177806471149607808531682981367767787003143897238035883266"));
}

void ComplexTest::arccos()
{
    COMPARE_COMPLEX(Complex::arccos(1), Complex(0));
    COMPARE_COMPLEX(Complex::arccos(0), Complex(BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::arccos(-1), Complex(BigDecimal::PI));
    COMPARE_COMPLEX(Complex::arccos("1.543080634815243778477905620757061682601529112365863704737402215"), Complex::i);
    COMPARE_COMPLEX(Complex::arccos(Complex::i), Complex("1.570796326794896619231321691639751442098584699687552910487472296", "-0.8813735870195430252326093249797923090281603282616354107532956087"));
    COMPARE_COMPLEX(Complex::arccos(-Complex::i), Complex("1.570796326794896619231321691639751442098584699687552910487472296", "0.8813735870195430252326093249797923090281603282616354107532956087"));
}

void ComplexTest::arctan()
{
    COMPARE_COMPLEX(Complex::arctan(0), Complex(0));
    COMPARE_COMPLEX(Complex::arctan(Complex("0", "0.7615941559557648881194582826047935904127685972579365515968105001")), Complex::i);
    COMPARE_COMPLEX(Complex::arctan(Complex("0", "-0.7615941559557648881194582826047935904127685972579365515968105001")), -Complex::i);
    COMPARE_COMPLEX(Complex::arctan(Complex(1, 1)), Complex("1.017221967897851367722788961550482922063560876986836587149202692", "0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arctan(Complex(-1, 1)), Complex("-1.017221967897851367722788961550482922063560876986836587149202692", "0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arctan(Complex(1, -1)), Complex("1.017221967897851367722788961550482922063560876986836587149202692", "-0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arctan(Complex(-1, -1)), Complex("-1.017221967897851367722788961550482922063560876986836587149202692", "-0.4023594781085250936501898333065469098814003385671294304781619729"));
}

void ComplexTest::arccot()
{
    COMPARE_COMPLEX(Complex::arccot(0), Complex(BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::arccot(Complex("0", "-1.313035285499331303636161246930847832912013941240452655543152968")), Complex::i);
    COMPARE_COMPLEX(Complex::arccot(Complex("0", "1.313035285499331303636161246930847832912013941240452655543152968")), -Complex::i);
    COMPARE_COMPLEX(Complex::arccot(Complex(1, 1)), Complex("0.5535743588970452515085327300892685200350238227007163233382696037", "-0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arccot(Complex(-1, 1)), Complex("-0.5535743588970452515085327300892685200350238227007163233382696037", "-0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arccot(Complex(1, -1)), Complex("0.5535743588970452515085327300892685200350238227007163233382696037", "0.4023594781085250936501898333065469098814003385671294304781619729"));
    COMPARE_COMPLEX(Complex::arccot(Complex(-1, -1)), Complex("-0.5535743588970452515085327300892685200350238227007163233382696037", "0.4023594781085250936501898333065469098814003385671294304781619729"));
}

void ComplexTest::sinh()
{
    COMPARE_COMPLEX(Complex::sinh(0), Complex(0));
    COMPARE_COMPLEX(Complex::sinh(BigDecimal::PI / 2), Complex("2.301298902307294873463040023434427178178146516516382665972839803"));
    COMPARE_COMPLEX(Complex::sinh(BigDecimal::PI), Complex("11.54873935725774837797733431538840968449518906639478945523216336"));
    COMPARE_COMPLEX(Complex::sinh(BigDecimal::PI * 3 / 2), Complex("55.6543975994175482994756714905404589587626686528453271855243772"));
    COMPARE_COMPLEX(Complex::sinh(-BigDecimal::PI / 2), Complex("-2.301298902307294873463040023434427178178146516516382665972839803"));
    COMPARE_COMPLEX(Complex::sinh(Complex::i), Complex("0", "0.84147098480789650665250232163029899962256306079837106567275171"));
    COMPARE_COMPLEX(Complex::sinh(-Complex::i), Complex("0", "-0.84147098480789650665250232163029899962256306079837106567275171"));
    COMPARE_COMPLEX(Complex::sinh(Complex::i * BigDecimal::PI / 2), Complex::i);
    COMPARE_COMPLEX(Complex::sinh(-Complex::i * BigDecimal::PI / 2), -Complex::i);
}

void ComplexTest::cosh()
{
    COMPARE_COMPLEX(Complex::cosh(0), Complex(1));
    COMPARE_COMPLEX(Complex::cosh(BigDecimal::PI / 2), Complex("2.509178478658056782009995643269405948212024358148152274047975686"));
    COMPARE_COMPLEX(Complex::cosh(BigDecimal::PI), Complex("11.59195327552152062775175205256013769577091717620542253821288305"));
    COMPARE_COMPLEX(Complex::cosh(BigDecimal::PI * 3 / 2), Complex("55.66338089043867772736533644244838421248400641905150915289214515"));
    COMPARE_COMPLEX(Complex::cosh(-BigDecimal::PI / 2), Complex("2.509178478658056782009995643269405948212024358148152274047975686"));
    COMPARE_COMPLEX(Complex::cosh(Complex::i), Complex("0.5403023058681397174009366074429766037323104206179222276700972554"));
    COMPARE_COMPLEX(Complex::cosh(-Complex::i), Complex("0.5403023058681397174009366074429766037323104206179222276700972554"));
    COMPARE_COMPLEX(Complex::cosh(Complex::i * BigDecimal::PI / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cosh(-Complex::i * BigDecimal::PI / 2), Complex(0));
    COMPARE_COMPLEX(Complex::cosh(Complex::i * BigDecimal::PI), Complex(-1));
    COMPARE_COMPLEX(Complex::cosh(-Complex::i * BigDecimal::PI), Complex(-1));
}

void ComplexTest::tanh()
{
    FAIL_TEST(Complex::tanh(Complex::i * BigDecimal::PI / 2), "tanh == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::tanh(0), Complex(0));
    COMPARE_COMPLEX(Complex::tanh(BigDecimal::PI / 2), Complex("0.9171523356672743463730929214426187753679271486010889453435741243"));
    COMPARE_COMPLEX(Complex::tanh(BigDecimal::PI), Complex("0.996272076220749944264690580012536711896899190804587614362612416"));
    COMPARE_COMPLEX(Complex::tanh(BigDecimal::PI * 3 / 2), Complex("0.9998386139886326507423007922366555204380144685365595660000788999"));
    COMPARE_COMPLEX(Complex::tanh(-BigDecimal::PI), Complex("-0.996272076220749944264690580012536711896899190804587614362612416"));
    COMPARE_COMPLEX(Complex::tanh(Complex::i), Complex("0", "1.557407724654902230506974807458360173087250772381520038383946606"));
    COMPARE_COMPLEX(Complex::tanh(-Complex::i), Complex("0", "-1.557407724654902230506974807458360173087250772381520038383946606"));
}

void ComplexTest::coth()
{
    FAIL_TEST(Complex::coth(0), "coth == infinity", InvalidArgumentException);
    COMPARE_COMPLEX(Complex::coth(BigDecimal::PI / 2), Complex("1.090331410727368230030012460946981259556977697390726381466085933"));
    COMPARE_COMPLEX(Complex::coth(BigDecimal::PI), Complex("1.003741873197321288201552691194800017462452422995907663404830028"));
    COMPARE_COMPLEX(Complex::coth(BigDecimal::PI * 3 / 2), Complex("1.000161412061016063223350546060084111015362041578603957810506314"));
    COMPARE_COMPLEX(Complex::coth(-BigDecimal::PI / 2), Complex("-1.090331410727368230030012460946981259556977697390726381466085933"));
    COMPARE_COMPLEX(Complex::coth(Complex::i), Complex("0", "-0.6420926159343307030064199865942656202302781139181713791011622804"));
    COMPARE_COMPLEX(Complex::coth(-Complex::i), Complex("0", "0.6420926159343307030064199865942656202302781139181713791011622804"));
    COMPARE_COMPLEX(Complex::coth(Complex::i * BigDecimal::PI / 2), Complex(0));
}

void ComplexTest::arcsinh()
{
    COMPARE_COMPLEX(Complex::arcsinh(0), Complex(0));
    COMPARE_COMPLEX(Complex::arcsinh(1), Complex("0.8813735870195430252326093249797923090281603282616354107532956087"));
    COMPARE_COMPLEX(Complex::arcsinh(-1), Complex("-0.8813735870195430252326093249797923090281603282616354107532956087"));
    COMPARE_COMPLEX(Complex::arcsinh(Complex::i), Complex(0, BigDecimal::PI / 2));
    COMPARE_COMPLEX(Complex::arcsinh(-Complex::i), Complex(0, -BigDecimal::PI / 2));
}

void ComplexTest::arccosh()
{
//    COMPARE_COMPLEX(Complex::arccosh(0), Complex::i * BigDecimal::PI / 2);
    COMPARE_COMPLEX(Complex::arccosh(1), Complex(0));
    COMPARE_COMPLEX(Complex::arccosh(-1), Complex::i * BigDecimal::PI);
    COMPARE_COMPLEX(Complex::arccosh(Complex::i), Complex("0.8813735870195430252326093249797923090281603282616354107532956087", "1.570796326794896619231321691639751442098584699687552910487472296"));
    COMPARE_COMPLEX(Complex::arccosh(-Complex::i), Complex("-0.8813735870195430252326093249797923090281603282616354107532956087", "1.570796326794896619231321691639751442098584699687552910487472296"));
}

void ComplexTest::arctanh()
{
    COMPARE_COMPLEX(Complex::arctanh(0), Complex(0));
    COMPARE_COMPLEX(Complex::arctanh("0.9171523356672743463730929214426187753679271486010889453435741243"), BigDecimal::PI / 2);
    COMPARE_COMPLEX(Complex::arctanh("0.996272076220749944264690580012536711896899190804587614362612416"), BigDecimal::PI);
    COMPARE_COMPLEX(Complex::arctanh("0.9998386139886326507423007922366555204380144685365595660000788999"), BigDecimal::PI * 3 / 2);
    COMPARE_COMPLEX(Complex::arctanh("-0.996272076220749944264690580012536711896899190804587614362612416"), Complex(-BigDecimal::PI));
//    COMPARE_COMPLEX(Complex::arctanh(Complex("0", "1.557407724654902230506974807458360173087250772381520038383946606")), Complex::i);
//    COMPARE_COMPLEX(Complex::arctanh(Complex("0", "-1.557407724654902230506974807458360173087250772381520038383946606")), -Complex::i);
}

void ComplexTest::arccoth()
{
    COMPARE_COMPLEX(Complex::arccoth(0), Complex::i * BigDecimal::PI / 2);
    COMPARE_COMPLEX(Complex::arccoth("1.090331410727368230030012460946981259556977697390726381466085933"), BigDecimal::PI / 2);
    COMPARE_COMPLEX(Complex::arccoth("1.003741873197321288201552691194800017462452422995907663404830028"), BigDecimal::PI);
    COMPARE_COMPLEX(Complex::arccoth("1.000161412061016063223350546060084111015362041578603957810506314"), BigDecimal::PI * 3 / 2);
    COMPARE_COMPLEX(Complex::arccoth("-1.090331410727368230030012460946981259556977697390726381466085933"), -BigDecimal::PI / 2);
//    COMPARE_COMPLEX(Complex::arccoth(Complex("0", "-0.6420926159343307030064199865942656202302781139181713791011622804")), Complex::i);
//    COMPARE_COMPLEX(Complex::arccoth(Complex("0", "0.6420926159343307030064199865942656202302781139181713791011622804")), -Complex::i);
}
