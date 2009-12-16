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
#include "complextest.h"
#include "utility.h"
// MaxCalcEngine
#include "complex.h"
#include "exceptions.h"
// STL
#include <string>

using namespace MaxCalcEngine;

void ComplexTest::complexFormatDefault()
{
    ComplexFormat actual;
    ComplexFormat expected = ComplexFormat::getDefault();

    COMPARE(actual.precision(), expected.precision());
    COMPARE(actual.numberFormat(), expected.numberFormat());
    COMPARE(actual.exponentCase(), expected.exponentCase());
    COMPARE(actual.imaginaryOne(), expected.imaginaryOne());

    COMPARE(actual.precision(), MAX_IO_PRECISION);
    COMPARE(actual.numberFormat(), ComplexFormat::GENERAL_FORMAT);
    COMPARE(actual.exponentCase(), ComplexFormat::UPPER_CASE_EXPONENT);
    COMPARE(actual.imaginaryOneChar(), 'i');
}

void ComplexTest::complexFormatCustom()
{
    ComplexFormat actual(MAX_IO_PRECISION,
                         ComplexFormat::ENGINEERING_FORMAT,
                         ComplexFormat::LOWER_CASE_EXPONENT,
                         ComplexFormat::POINT_DECIMAL_SEPARATOR,
                         ComplexFormat::IMAGINARY_ONE_J);

    COMPARE(actual.precision(), MAX_IO_PRECISION);
    COMPARE(actual.numberFormat(), ComplexFormat::ENGINEERING_FORMAT);
    COMPARE(actual.exponentCase(), ComplexFormat::LOWER_CASE_EXPONENT);
    COMPARE(actual.decimalSeparatorChar(), '.');
    COMPARE(actual.imaginaryOneChar(), 'j');

    actual = ComplexFormat(1, ComplexFormat::SCIENTIFIC_FORMAT,
                           ComplexFormat::UPPER_CASE_EXPONENT,
                           ComplexFormat::COMMA_DECIMAL_SEPARATOR,
                           ComplexFormat::IMAGINARY_ONE_I);

    COMPARE(actual.precision(), 1);
    COMPARE(actual.numberFormat(), ComplexFormat::SCIENTIFIC_FORMAT);
    COMPARE(actual.exponentCase(), ComplexFormat::UPPER_CASE_EXPONENT);
    COMPARE(actual.decimalSeparatorChar(), ',');
    COMPARE(actual.imaginaryOneChar(), 'i');
}

void ComplexTest::complexFormatAccessors()
{
    ComplexFormat actual;

    actual.setPrecision(MAX_IO_PRECISION / 2);
    actual.setNumberFormat(ComplexFormat::ENGINEERING_FORMAT);
    actual.setExponentCase(ComplexFormat::LOWER_CASE_EXPONENT);

    COMPARE(actual.precision(), MAX_IO_PRECISION / 2);
    COMPARE(actual.numberFormat(), ComplexFormat::ENGINEERING_FORMAT);
    COMPARE(actual.exponentCase(), ComplexFormat::LOWER_CASE_EXPONENT);

    actual.setPrecision(MAX_IO_PRECISION);
    COMPARE(actual.precision(), MAX_IO_PRECISION);

    actual.setPrecision(1);
    COMPARE(actual.precision(), 1);

    actual.setImaginaryOne(ComplexFormat::IMAGINARY_ONE_J);
    COMPARE(actual.imaginaryOneChar(), 'j');
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
    COMPARE(Complex::i.toString().c_str(), "1i");
    COMPARE(Complex(1).toString().c_str(), "1");
    COMPARE(Complex(-1).toString().c_str(), "-1");
    COMPARE(Complex(-10000).toString().c_str(), "-10000");
    COMPARE(Complex(1000).toString().c_str(), "1000");
    COMPARE(Complex(0, 1).toString().c_str(), "1i");
    COMPARE(Complex(0, -1).toString().c_str(), "-1i");
    COMPARE(Complex(0, -1000).toString().c_str(), "-1000i");
    COMPARE(Complex(0, 10000).toString().c_str(), "10000i");
    COMPARE(Complex("0.512").toString().c_str(), "0.512");
    COMPARE(Complex("-0.0246").toString().c_str(), "-0.0246");
    COMPARE(Complex("0", "0.512").toString().c_str(), "0.512i");
    COMPARE(Complex("0", "-0.0246").toString().c_str(), "-0.0246i");
    COMPARE(Complex(1, 1).toString().c_str(), "1+1i");
    COMPARE(Complex(1, -1).toString().c_str(), "1-1i");
    COMPARE(Complex(1000, 1).toString().c_str(), "1000+1i");
    COMPARE(Complex(1, 10000).toString().c_str(), "1+10000i");
    COMPARE(Complex(-1, 1000).toString().c_str(), "-1+1000i");
    COMPARE(Complex(100000, -1).toString().c_str(), "100000-1i");
    COMPARE(Complex(100000, 1000000).toString().c_str(), "100000+1E+6i");
    COMPARE(Complex(-1, -1).toString().c_str(), "-1-1i");
    COMPARE(Complex("0.512", "0.256").toString().c_str(), "0.512+0.256i");
    COMPARE(Complex("-0.512", "0.256").toString().c_str(), "-0.512+0.256i");
    COMPARE(Complex("0.512", "-0.256").toString().c_str(), "0.512-0.256i");
    COMPARE(Complex("-0.512", "-0.256").toString().c_str(), "-0.512-0.256i");
}

void ComplexTest::toWideString()
{
    COMPARE(Complex("0.512").toWideString(), std::wstring(L"0.512"));
    COMPARE(Complex(L"-0.0246").toWideString(), std::wstring(L"-0.0246"));
    COMPARE(Complex("0.512", "0.256").toWideString(), std::wstring(L"0.512+0.256i"));
    COMPARE(Complex(L"-0.512", L"0.256").toWideString(), std::wstring(L"-0.512+0.256i"));
    COMPARE(Complex("0.512", "-0.256").toWideString(), std::wstring(L"0.512-0.256i"));
    COMPARE(Complex(L"-0.512", L"-0.256").toWideString(), std::wstring(L"-0.512-0.256i"));
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
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3, 4)), BigDecimal(25));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(-3)), BigDecimal(9));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3)), BigDecimal(9));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(-3, -4)), BigDecimal(25));
    COMPARE_BIGDECIMAL(Complex::sqr(Complex(3, -4)), BigDecimal(25));
}

void ComplexTest::abs()
{
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

void ComplexTest::ln()
{
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::E)), Complex(1));
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::sqr(BigDecimal::E))), Complex(2));
    COMPARE_COMPLEX(Complex::ln(Complex(BigDecimal::sqrt(BigDecimal::E))), Complex("0.5"));
}
