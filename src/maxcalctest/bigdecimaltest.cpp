
#include "bigdecimaltest.h"
#include "bigdecimal.h"
#include <QTest>
#include <string>

void BigDecimalTest::bigDecimalFormat()
{
	BigDecimalFormat format;
	BigDecimalFormat expected = BigDecimalFormat::getDefault();

	QCOMPARE(format.precision(), expected.precision());
	QCOMPARE(format.numberFormat(), expected.numberFormat());
	QCOMPARE(format.exponentCase(), expected.exponentCase());
}

void BigDecimalTest::fromString()
{
	BigDecimal dec(std::string("-1121.34E-2"));

	QVERIFY(dec.isNegative());
	QCOMPARE(dec.integer(), BigDecimal(-11));
	QCOMPARE(dec.fractional(), BigDecimal("0.2134"));

	dec = std::string("0");
	QVERIFY(dec.isZero());
	QCOMPARE(dec.integer(), BigDecimal(0));
	QCOMPARE(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromCharStr()
{
	BigDecimal dec("1121.34e3");

	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal(1121340));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	dec = "0";
	QVERIFY(dec.isZero());
	QCOMPARE(dec.integer(), BigDecimal(0));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	// Stress tests

	// 100-digit number with 5-digit exponent
	dec = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789E+100000"));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	// 110-digit number with 5-digit exponent
	dec = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e99999"));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	// 110-digit negative number with 5-digit negative exponent
	dec = "-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e-99999";
	QVERIFY(dec.isNegative());
	QCOMPARE(dec, BigDecimal("-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E-99999"));

	// 120-digit zero
	dec = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E+00000000000000";
	QVERIFY(dec.isZero());
	QCOMPARE(dec, BigDecimal(0));
}

void BigDecimalTest::fromBigDecimal()
{
	BigDecimal dec1("463326");
	BigDecimal dec2(dec1);

	QCOMPARE(dec1, dec2);
}

void BigDecimalTest::fromInt()
{
	BigDecimal dec(-2147483647);

	QVERIFY(dec.isNegative());
	QCOMPARE(dec.integer(), BigDecimal(-2147483647));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	dec = 0;

	QVERIFY(dec.isZero());
	QCOMPARE(dec.integer(), BigDecimal(0));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	dec = 2147483647;

	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal(2147483647));
	QCOMPARE(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromUInt()
{
	BigDecimal dec(4294967295u);

	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal(4294967295u));
	QCOMPARE(dec.fractional(), BigDecimal(0));

	dec = (unsigned)-1;
	QVERIFY(dec.isPositive());
	QCOMPARE(dec.integer(), BigDecimal(4294967295u));
	QCOMPARE(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::toString()
{
	BigDecimal dec = 100;
	QCOMPARE(dec.toString(), std::string("1E+2"));
	QCOMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("1E+2"));
	QCOMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("100"));
	QCOMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat, BigDecimalFormat::UpperCaseExponent)), std::string("100"));
	QCOMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat, BigDecimalFormat::LowerCaseExponent)).c_str(), std::string("1e+2").c_str());

	dec = "123.456";
	QCOMPARE(dec.toString(BigDecimalFormat(4)), BigDecimal("123.5").toString());
	QCOMPARE(BigDecimal(dec.toString(BigDecimalFormat(2))), BigDecimal("1.2E+2"));

	QCOMPARE(BigDecimal("1E-4378").toString(), std::string("1E-4378"));
}

void BigDecimalTest::toInt()
{
	BigDecimal dec(-2147483647);
	QCOMPARE(dec.toInt(), -2147483647);

	dec = 0;
	QCOMPARE(dec.toInt(), 0);

	dec = 2147483647;
	QCOMPARE(dec.toInt(), 2147483647);
}

void BigDecimalTest::toUInt()
{
	BigDecimal dec((unsigned)-1);
	QCOMPARE(dec.toUInt(), 4294967295u);

	dec = 0;
	QCOMPARE(dec.toUInt(), 0u);

	dec = 4294967295u;
	QCOMPARE(dec.toUInt(), 4294967295u);
}

void BigDecimalTest::unaryOperators()
{
	BigDecimal dec(10);
	QCOMPARE(+dec, BigDecimal(10));
	QCOMPARE(-dec, BigDecimal(-10));

	dec = -125;
	QCOMPARE(+dec, BigDecimal(-125));
	QCOMPARE(-dec, BigDecimal(+125));

	dec++;
	QCOMPARE(dec, BigDecimal(-124));

	dec--;
	QCOMPARE(dec, BigDecimal(-125));

	QCOMPARE(dec++, BigDecimal(-125));
	QCOMPARE(dec--, BigDecimal(-124));
	QCOMPARE(--dec, BigDecimal(-126));
	QCOMPARE(++dec, BigDecimal(-125));
}

void BigDecimalTest::binaryArithmeticOperators()
{
	BigDecimal dec1(-67), dec2(67);

	QCOMPARE(dec1 + dec2, BigDecimal(0));
	QCOMPARE(dec1 - dec2, BigDecimal(-134));
	QCOMPARE(dec1 * dec2, BigDecimal(-4489));
	QCOMPARE(dec1 / dec2, BigDecimal(-1));
	QCOMPARE(dec1 % dec2, BigDecimal(0));

	QCOMPARE(dec1 += dec2, BigDecimal(0));
	QCOMPARE(dec1 -= dec2, BigDecimal(-67));
	QCOMPARE(dec1 *= dec2, BigDecimal(-4489));
	QCOMPARE(dec1 /= dec2, BigDecimal(-67));
	QCOMPARE(dec1 %= dec2, BigDecimal(0));

	QCOMPARE(dec2 + 0, BigDecimal(67));
	QCOMPARE(dec2 - 0, BigDecimal(67));
	QCOMPARE(dec2 * 0, BigDecimal(0));

	try
	{
		dec2 /= 0;
		QFAIL("Division by zero");
	}
	catch (...)
	{
		QCOMPARE(dec2, BigDecimal("Infinity"));
	}

	try
	{
		dec1 %= 0;
		QFAIL("Division by zero");
	}
	catch (...)
	{
		QCOMPARE(dec2, BigDecimal("Infinity"));
	}

	QCOMPARE(BigDecimal(std::string("100000000000000000")) / BigDecimal(std::string("2")), BigDecimal("5E+16"));
	COMPARE_WITH_PRECISION((BigDecimal("12234.234") * BigDecimal("2434.23443") / BigDecimal("3.1")),
		BigDecimal("9.60677213789568387096774193548387096774193548387097e6"), DEFAULT_PRECISION);
}

void BigDecimalTest::binaryLogicalOperators()
{
	BigDecimal dec(1010);

	QCOMPARE(dec | 11, BigDecimal(1011));
	QCOMPARE(dec & 11, BigDecimal(10));
	QCOMPARE(dec ^ 11, BigDecimal(1001));
	QCOMPARE(dec << 2, BigDecimal(101000));
	QCOMPARE(dec >> 2, BigDecimal(10));
	QCOMPARE(~~dec, BigDecimal(1010));

	QCOMPARE(dec |= 11, BigDecimal(1011));
	QCOMPARE(dec &= 11, BigDecimal(11));
	QCOMPARE(dec ^= 11, BigDecimal(0));
	QCOMPARE(dec <<= 2, BigDecimal(0));
	QCOMPARE(dec >>= 2, BigDecimal(0));

	QCOMPARE(~dec & 0 + 1, BigDecimal(1));
}

void BigDecimalTest::comparisonOperators()
{
	BigDecimal dec1("67834E-2"), dec2("67834E-1");

	QCOMPARE(dec1 == dec2, false);
	QCOMPARE(dec1 != dec2, true);
	QCOMPARE(dec1 >= dec2, false);
	QCOMPARE(dec1 <= dec2, true);
	QCOMPARE(dec1 > dec2, false);
	QCOMPARE(dec1 < dec2, true);

	dec1 = 123;
	dec2 = "12.3E+1";

	QCOMPARE(dec1 == dec2, true);
	QCOMPARE(dec1 != dec2, false);
	QCOMPARE(dec1 >= dec2, true);
	QCOMPARE(dec1 <= dec2, true);
	QCOMPARE(dec1 > dec2, false);
	QCOMPARE(dec1 < dec2, false);
}

void BigDecimalTest::abs()
{
	QCOMPARE(BigDecimal::abs(-10), BigDecimal(10));
	QCOMPARE(BigDecimal::abs(10), BigDecimal(10));
	QCOMPARE(BigDecimal::abs(0), BigDecimal(0));
}

void BigDecimalTest::exp()
{
	QCOMPARE(BigDecimal::exp(0), BigDecimal(1));
	QCOMPARE(BigDecimal::exp(BigDecimal::ln(0)), BigDecimal(0));
	QCOMPARE(BigDecimal::exp(1), BigDecimal::E);
}

void BigDecimalTest::ln()
{
	QCOMPARE(BigDecimal::ln(1), BigDecimal(0));
	QCOMPARE(BigDecimal::ln(BigDecimal::exp(0)), BigDecimal(0));
	QCOMPARE(BigDecimal::ln(BigDecimal::exp("-3467.2")), BigDecimal("-3467.2"));
	COMPARE_WITH_PRECISION(BigDecimal::ln(BigDecimal::E), BigDecimal(1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::ln(BigDecimal::E * BigDecimal::E), BigDecimal(2), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::ln(BigDecimal::sqr(BigDecimal::E)), BigDecimal(2), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::ln(BigDecimal::pow(BigDecimal::E, 125)), BigDecimal(125), DEFAULT_PRECISION);
}

void BigDecimalTest::log10()
{
	QCOMPARE(BigDecimal::log10(1), BigDecimal(0));
	QCOMPARE(BigDecimal::log10(1000), BigDecimal(3));
}

void BigDecimalTest::sqr()
{
	QCOMPARE(BigDecimal::sqr(0), BigDecimal(0));
	QCOMPARE(BigDecimal::sqr(4), BigDecimal(16));
	QCOMPARE(BigDecimal::sqrt(BigDecimal::sqr("534.123")), BigDecimal("534.123"));
}

void BigDecimalTest::sqrt()
{
	QCOMPARE(BigDecimal::sqrt(0), BigDecimal(0));
	QCOMPARE(BigDecimal::sqrt(16), BigDecimal(4));
	QCOMPARE(BigDecimal::sqrt(BigDecimal::pow("534.123", 2)), BigDecimal("534.123"));
}

void BigDecimalTest::pow()
{
	QCOMPARE(BigDecimal::pow(4, 4), BigDecimal(256));
	QCOMPARE(BigDecimal::pow(BigDecimal::sqrt(625), 2), BigDecimal(625));
}

void BigDecimalTest::div()
{
	QCOMPARE(BigDecimal::div(4, 4), BigDecimal(1));
	QCOMPARE(BigDecimal::div("-124.213", 2), BigDecimal(-62));
}

void BigDecimalTest::max()
{
	QCOMPARE(BigDecimal::max(23, 13), BigDecimal(23));
	QCOMPARE(BigDecimal::max(-65, -65), BigDecimal(-65));
}

void BigDecimalTest::min()
{
	QCOMPARE(BigDecimal::min(23, 13), BigDecimal(13));
	QCOMPARE(BigDecimal::min(65, 65), BigDecimal(65));
}

void BigDecimalTest::fact()
{
	// Small factorials
	QCOMPARE(BigDecimal::fact(0), BigDecimal(1));
	QCOMPARE(BigDecimal::fact(1), BigDecimal(1));
	QCOMPARE(BigDecimal::fact(2), BigDecimal(2));
	QCOMPARE(BigDecimal::fact(3), BigDecimal(6));
	QCOMPARE(BigDecimal::fact(4), BigDecimal(24));
	QCOMPARE(BigDecimal::fact(5), BigDecimal(120));
	QCOMPARE(BigDecimal::fact(10), BigDecimal(3628800));
	QCOMPARE(BigDecimal::fact(25), BigDecimal("15511210043330985984000000"));

	// Stress test
	COMPARE_WITH_PRECISION(BigDecimal::fact(450), BigDecimal("1.73336873E+1000"), 9);
	COMPARE_WITH_PRECISION(BigDecimal::fact(200000), BigDecimal("1.4202253454703144049669463336823059760899653567464E+973350"), DEFAULT_PRECISION);

	try
	{
		BigDecimal::fact(-1);
		QFAIL("Factorial of -1");
	}
	catch (BigDecimal::InvalidNumberInFactorial)
	{
	}

	try
	{
		BigDecimal::fact("1.2");
		QFAIL("Factorial of 1.2");
	}
	catch (BigDecimal::InvalidNumberInFactorial)
	{
	}
}

void BigDecimalTest::sin()
{
	COMPARE_WITH_PRECISION(BigDecimal::sin(0), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI / 2), BigDecimal(1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin((BigDecimal::PI * 3) / 2), BigDecimal(-1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI * 2), BigDecimal(0), DEFAULT_PRECISION);

	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI * 742342346), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI * 436677271 / 2), BigDecimal(-1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI * 436677273 / 2), BigDecimal(1), DEFAULT_PRECISION);

	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999999")), BigDecimal(0), DEFAULT_PRECISION);

	// SpeedCrunch 0.10.1 fails this test
	// Reference number is calculated by Mathematica 6 with 100 digits precision
	COMPARE_WITH_PRECISION(BigDecimal::sin(BigDecimal::PI *
		BigDecimal("34678236483724678236482376437643478374837487384783874637846736473467346347E-15")),
		BigDecimal("0.7365077837912717180169241081842837128232214243058669031072233561867904779639270977189273961709260958"),
		DEFAULT_PRECISION);
}

void BigDecimalTest::cos()
{
	COMPARE_WITH_PRECISION(BigDecimal::cos(0), BigDecimal(1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI / 2), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI), BigDecimal(-1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos((BigDecimal::PI * 3) / 2), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI * 2), BigDecimal(1), DEFAULT_PRECISION);

	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI * 742342346), BigDecimal(1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI * 742342349), BigDecimal(-1), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::cos(BigDecimal::PI * 742342341 / 2), BigDecimal(0), DEFAULT_PRECISION);
}

void BigDecimalTest::tan()
{
	COMPARE_WITH_PRECISION(BigDecimal::tan(0), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::tan(BigDecimal::PI), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::tan(BigDecimal::PI * 2), BigDecimal(0), DEFAULT_PRECISION);
}

void BigDecimalTest::ctan()
{
	COMPARE_WITH_PRECISION(BigDecimal::ctan(BigDecimal::PI / 2), BigDecimal(0), DEFAULT_PRECISION);
	COMPARE_WITH_PRECISION(BigDecimal::ctan(BigDecimal::PI * 3 / 2), BigDecimal(0), DEFAULT_PRECISION);
}

void BigDecimalTest::consts()
{
	// Reference values are calculated by PowerCalc and have 128 digits precision

	// Check PI
	COMPARE_WITH_PRECISION(BigDecimal::PI,
		BigDecimal("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446"),
		DEFAULT_PRECISION);

	// Check E
	COMPARE_WITH_PRECISION(BigDecimal::E,
		BigDecimal("2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174136"),
		DEFAULT_PRECISION);
}
