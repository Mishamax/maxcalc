
#include "bigdecimaltest.h"
#include "bigdecimal.h"
#include <QTest>

void BigDecimalTest::bigDecimalFormat()
{
	BigDecimalFormat format;
	BigDecimalFormat expected = defaultBigDecimalFormat;

	QCOMPARE(format.engineeringFormat, expected.engineeringFormat);
	QCOMPARE(format.lowerCaseE, expected.lowerCaseE);
	QCOMPARE(format.digitsAfterDecimalPoint, expected.digitsAfterDecimalPoint);
}

void BigDecimalTest::fromQString()
{
	BigDecimal dec(QString("-1121.34E-2"));

	QVERIFY(dec.isNegative());
	QCOMPARE(dec.integer(), BigDecimal(-11));
	QCOMPARE(dec.fractional(), BigDecimal("0.2134"));

	dec = QString("0");
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

void BigDecimalTest::constructionStress()
{
	// 100-digit number with 5-digit exponent
	BigDecimal dec = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
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

void BigDecimalTest::toString()
{
	BigDecimal dec = 100;
	QCOMPARE(dec.toString(), QString("1E+2"));
	QCOMPARE(dec.toString(BigDecimalFormat(false)), QString("1E+2"));
	QCOMPARE(dec.toString(BigDecimalFormat(true)), QString("100"));
	QCOMPARE(dec.toString(BigDecimalFormat(true, false, 10)), QString("100"));
	QCOMPARE(dec.toString(BigDecimalFormat(false, true)), QString("1e+2"));
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
}

void BigDecimalTest::ln()
{
	QCOMPARE(BigDecimal::ln(1), BigDecimal(0));
	QCOMPARE(BigDecimal::ln(BigDecimal::exp(0)), BigDecimal(0));
	QCOMPARE(BigDecimal::ln(BigDecimal::exp("-3467.2")), BigDecimal("-3467.2"));
}

void BigDecimalTest::log10()
{
	QCOMPARE(BigDecimal::log10(1), BigDecimal(0));
	QCOMPARE(BigDecimal::log10(1000), BigDecimal(3));
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
