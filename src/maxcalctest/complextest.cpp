
// Local
#include "complextest.h"
#include "bigdecimalcompare.h"
// MaxCalcEngine
#include "complex.h"
// STL
#include <string>

using namespace MaxCalcEngine;

void ComplexTest::complexFormatDefault()
{
	ComplexFormat actual;
	ComplexFormat expected = ComplexFormat::getDefault();

	QCOMPARE(actual.precision(), expected.precision());
	QCOMPARE(actual.numberFormat(), expected.numberFormat());
	QCOMPARE(actual.exponentCase(), expected.exponentCase());
	QCOMPARE(actual.imaginaryOne(), expected.imaginaryOne());

	QCOMPARE(actual.precision(), MAX_IO_PRECISION);
	QCOMPARE(actual.numberFormat(), ComplexFormat::ScientificFormat);
	QCOMPARE(actual.exponentCase(), ComplexFormat::UpperCaseExponent);
	QCOMPARE(actual.imaginaryOne(), 'i');
}

void ComplexTest::complexFormatCustom()
{
	ComplexFormat actual(MAX_IO_PRECISION, ComplexFormat::EngineeringFormat, ComplexFormat::LowerCaseExponent, 'j');

	QCOMPARE(actual.precision(), MAX_IO_PRECISION);
	QCOMPARE(actual.numberFormat(), ComplexFormat::EngineeringFormat);
	QCOMPARE(actual.exponentCase(), ComplexFormat::LowerCaseExponent);
	QCOMPARE(actual.imaginaryOne(), 'j');

	actual = ComplexFormat(1, ComplexFormat::ScientificFormat, ComplexFormat::UpperCaseExponent, 'i');

	QCOMPARE(actual.precision(), 1);
	QCOMPARE(actual.numberFormat(), ComplexFormat::ScientificFormat);
	QCOMPARE(actual.exponentCase(), ComplexFormat::UpperCaseExponent);
	QCOMPARE(actual.imaginaryOne(), 'i');
}

void ComplexTest::complexFormatAccessors()
{
	ComplexFormat actual;

	actual.setPrecision(MAX_IO_PRECISION / 2);
	actual.setNumberFormat(ComplexFormat::EngineeringFormat);
	actual.setExponentCase(ComplexFormat::LowerCaseExponent);

	QCOMPARE(actual.precision(), MAX_IO_PRECISION / 2);
	QCOMPARE(actual.numberFormat(), ComplexFormat::EngineeringFormat);
	QCOMPARE(actual.exponentCase(), ComplexFormat::LowerCaseExponent);

	actual.setPrecision(MAX_IO_PRECISION);
	QCOMPARE(actual.precision(), MAX_IO_PRECISION);

	actual.setPrecision(1);
	QCOMPARE(actual.precision(), 1);

	actual.setImaginaryOne('j');
	QCOMPARE(actual.imaginaryOne(), 'j');
}


void ComplexTest::fromString()
{
	Complex num(std::string("1e-5"), std::string("-1e+2"));

	QCOMPARE(num.re, BigDecimal("0.00001"));
	QCOMPARE(num.im, BigDecimal(-100));
}

void ComplexTest::fromCharStr()
{
	Complex num("1e-5", "-1e+2");

	QCOMPARE(num.re, BigDecimal(std::string("0.00001")));
	QCOMPARE(num.im, BigDecimal(-100));
}


void ComplexTest::fromWideString()
{
	Complex num(std::wstring(L"1e-5"), std::wstring(L"-1e+2"));

	QCOMPARE(num.re, BigDecimal("0.00001"));
	QCOMPARE(num.im, BigDecimal(-100));
}

void ComplexTest::fromWideCharStr()
{
	Complex num(L"1e-5", L"-1e+2");

	QCOMPARE(num.re, BigDecimal(std::string("0.00001")));
	QCOMPARE(num.im, BigDecimal(-100));
}

void ComplexTest::fromBigDecimal()
{
	Complex num(BigDecimal(10), BigDecimal(-5));

	QCOMPARE(num.re, BigDecimal(10));
	QCOMPARE(num.im, BigDecimal(-5));
}

void ComplexTest::fromInt()
{
	Complex num;
	QCOMPARE(num.re, BigDecimal(0));
	QCOMPARE(num.im, BigDecimal(0));

	num = Complex(10);
	QCOMPARE(num.re, BigDecimal(10));

	num = Complex(20, 10);
	QCOMPARE(num.re, BigDecimal(20));
	QCOMPARE(num.im, BigDecimal(10));
}

void ComplexTest::fromUInt()
{
	Complex num = Complex(20u, 10u);

	QCOMPARE(num.re, BigDecimal(20));
	QCOMPARE(num.im, BigDecimal(10));
}

void ComplexTest::toString()
{
	Complex num;
	QCOMPARE(num.toString().c_str(), "0");

	num = 1;
	QCOMPARE(num.toString().c_str(), "1");

	num = -1;
	QCOMPARE(num.toString().c_str(), "-1");

	num = -10000;
	QCOMPARE(num.toString().c_str(), "-1E+4");

	num = 1000;
	QCOMPARE(num.toString().c_str(), "1E+3");

	num = Complex(0, 1);
	QCOMPARE(num.toString().c_str(), "1i");

	num = Complex(0, -1);
	QCOMPARE(num.toString().c_str(), "-1i");

	num = Complex(0, -1000);
	QCOMPARE(num.toString().c_str(), "-1E+3i");

	num = Complex(0, 10000);
	QCOMPARE(num.toString().c_str(), "1E+4i");

	num = "0.512";
	QCOMPARE(num.toString().c_str(), "0.512");

	num = "-0.0246";
	QCOMPARE(num.toString().c_str(), "-0.0246");

	num = Complex("0", "0.512");
	QCOMPARE(num.toString().c_str(), "0.512i");

	num = Complex("0", "-0.0246");
	QCOMPARE(num.toString().c_str(), "-0.0246i");

	num = Complex(1, 1);
	QCOMPARE(num.toString().c_str(), "1+1i");

	num = Complex(1, -1);
	QCOMPARE(num.toString().c_str(), "1-1i");

	num = Complex(1000, 1);
	QCOMPARE(num.toString().c_str(), "1E+3+1i");

	num = Complex(1, 10000);
	QCOMPARE(num.toString().c_str(), "1+1E+4i");

	num = Complex(-1, 1000);
	QCOMPARE(num.toString().c_str(), "-1+1E+3i");

	num = Complex(100000, -1);
	QCOMPARE(num.toString().c_str(), "1E+5-1i");

	num = Complex(100000, 1000000);
	QCOMPARE(num.toString().c_str(), "1E+5+1E+6i");

	num = Complex(-1, -1);
	QCOMPARE(num.toString().c_str(), "-1-1i");

	num = Complex("0.512", "0.256");
	QCOMPARE(num.toString().c_str(), "0.512+0.256i");

	num = Complex("-0.512", "0.256");
	QCOMPARE(num.toString().c_str(), "-0.512+0.256i");

	num = Complex("0.512", "-0.256");
	QCOMPARE(num.toString().c_str(), "0.512-0.256i");

	num = Complex("-0.512", "-0.256");
	QCOMPARE(num.toString().c_str(), "-0.512-0.256i");
}

void ComplexTest::toWideString()
{
	Complex num = "0.512";
	QCOMPARE(num.toWideString(), std::wstring(L"0.512"));

	num = L"-0.0246";
	QCOMPARE(num.toWideString(), std::wstring(L"-0.0246"));

	num = Complex("0.512", "0.256");
	QCOMPARE(num.toWideString(), std::wstring(L"0.512+0.256i"));

	num = Complex(L"-0.512", L"0.256");
	QCOMPARE(num.toWideString(), std::wstring(L"-0.512+0.256i"));

	num = Complex("0.512", "-0.256");
	QCOMPARE(num.toWideString(), std::wstring(L"0.512-0.256i"));

	num = Complex(L"-0.512", L"-0.256");
	QCOMPARE(num.toWideString(), std::wstring(L"-0.512-0.256i"));
}

void ComplexTest::unaryOperators()
{
	Complex num(1, 1);

	num = +num;
	QCOMPARE(num.re, BigDecimal(1));
	QCOMPARE(num.im, BigDecimal(1));

	num = -num;
	QCOMPARE(num.re, BigDecimal(-1));
	QCOMPARE(num.im, BigDecimal(-1));
}

void ComplexTest::binaryOperators()
{
	Complex num1(1, 2), num2(3, 4);
	Complex result = num1 + num2;

	QCOMPARE(result.re, BigDecimal(4));
	QCOMPARE(result.im, BigDecimal(6));

	result = num1 - num2;
	QCOMPARE(result.re, BigDecimal(-2));
	QCOMPARE(result.im, BigDecimal(-2));

	result = num1 * num2;
	QCOMPARE(result.re, BigDecimal(-5));
	QCOMPARE(result.im, BigDecimal(10));

	result = num1 / num2;
	QCOMPARE(result.re.toString().c_str(), "0.44");
	QCOMPARE(result.im.toString().c_str(), "0.08");

	try
	{
		result = num1 / Complex();
		QFAIL("Division by zero!");
	}
	catch (...)
	{
	}
}

void ComplexTest::comparisonOperators()
{
	Complex num1(1, 2), num2(1, 2);
	QVERIFY(num1 == num2);

	num2 = 1;
	QVERIFY(num1 != num2);

	num2 = Complex(0, 2);
	QVERIFY(num1 != num2);
}

void ComplexTest::abs()
{
	Complex num(3, 4);
	QCOMPARE(Complex::abs(num), BigDecimal(5));

	num = -3;
	QCOMPARE(Complex::abs(num), BigDecimal(3));

	num = 3;
	QCOMPARE(Complex::abs(num), BigDecimal(3));

	num = Complex(-3, -4);
	QCOMPARE(Complex::abs(num), BigDecimal(5));

	num = Complex(3, -4);
	QCOMPARE(Complex::abs(num), BigDecimal(5));
}

void ComplexTest::sqr()
{
	Complex num(3, 4);
	QCOMPARE(Complex::sqr(num), BigDecimal(25));

	num = -3;
	QCOMPARE(Complex::sqr(num), BigDecimal(9));

	num = 3;
	QCOMPARE(Complex::sqr(num), BigDecimal(9));

	num = Complex(-3, -4);
	QCOMPARE(Complex::sqr(num), BigDecimal(25));

	num = Complex(3, -4);
	QCOMPARE(Complex::sqr(num), BigDecimal(25));
}
