
// Local
#include "bigdecimaltest.h"
#include "utility.h"
// MaxCalcEngine
#include "bigdecimal.h"
#include "exceptions.h"
// STL
#include <string>

using namespace MaxCalcEngine;

void BigDecimalTest::bigDecimalFormatDefault()
{
	BigDecimalFormat actual;
	BigDecimalFormat expected = BigDecimalFormat::getDefault();

	COMPARE(actual.precision(), expected.precision());
	COMPARE(actual.numberFormat(), expected.numberFormat());
	COMPARE(actual.exponentCase(), expected.exponentCase());
	COMPARE(actual.decimalSeparator(), expected.decimalSeparator());

	COMPARE(actual.precision(), MAX_IO_PRECISION);
	COMPARE(actual.numberFormat(), BigDecimalFormat::GeneralFormat);
	COMPARE(actual.exponentCase(), BigDecimalFormat::UpperCaseExponent);
	COMPARE(actual.decimalSeparator(), BigDecimalFormat::PointDecimalSeparator);
}

void BigDecimalTest::bigDecimalFormatCustom()
{
	BigDecimalFormat actual(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat, BigDecimalFormat::LowerCaseExponent,
		BigDecimalFormat::CommaDecimalSeparator);

	COMPARE(actual.precision(), MAX_IO_PRECISION);
	COMPARE(actual.numberFormat(), BigDecimalFormat::EngineeringFormat);
	COMPARE(actual.exponentCase(), BigDecimalFormat::LowerCaseExponent);
	COMPARE(actual.decimalSeparator(), BigDecimalFormat::CommaDecimalSeparator);

	actual = BigDecimalFormat(1, BigDecimalFormat::ScientificFormat, BigDecimalFormat::UpperCaseExponent,
		BigDecimalFormat::PointDecimalSeparator);

	COMPARE(actual.precision(), 1);
	COMPARE(actual.numberFormat(), BigDecimalFormat::ScientificFormat);
	COMPARE(actual.exponentCase(), BigDecimalFormat::UpperCaseExponent);
	COMPARE(actual.decimalSeparator(), BigDecimalFormat::PointDecimalSeparator);
}

void BigDecimalTest::bigDecimalFormatAccessors()
{
	BigDecimalFormat actual;

	actual.setPrecision(MAX_IO_PRECISION / 2);
	actual.setNumberFormat(BigDecimalFormat::EngineeringFormat);
	actual.setExponentCase(BigDecimalFormat::LowerCaseExponent);

	COMPARE(actual.precision(), MAX_IO_PRECISION / 2);
	COMPARE(actual.numberFormat(), BigDecimalFormat::EngineeringFormat);
	COMPARE(actual.exponentCase(), BigDecimalFormat::LowerCaseExponent);

	actual.setPrecision(MAX_IO_PRECISION);

	COMPARE(actual.precision(), MAX_IO_PRECISION);

	actual.setPrecision(1);

	COMPARE(actual.precision(), 1);
}

void BigDecimalTest::fromString()
{
	BigDecimal dec(std::string("-1121.34E-2"));

	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal("0.2134"));

	dec = std::string("0");
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromCharStr()
{
	BigDecimal dec("1121.34e3");

	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(1121340));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = "0";
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// Stress tests

	// 100-digit number with 5-digit exponent
	dec = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789E+100000"));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// 110-digit number with 5-digit exponent
	dec = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e99999"));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// 110-digit negative number with 5-digit negative exponent
	dec = "-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e-99999";
	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec, BigDecimal("-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E-99999"));

	// 120-digit zero
	dec = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E+00000000000000";
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec, BigDecimal(0));
}


void BigDecimalTest::fromWideString()
{
	BigDecimal dec(std::wstring(L"-1121.34E-2"));

	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(L"0.2134"));

	dec = std::wstring(L"0");
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromWideCharStr()
{
	BigDecimal dec(L"1121.34e3");

	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(1121340));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = L"0";
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// Stress tests

	// 100-digit number with 5-digit exponent
	dec = L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(L"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789E+100000"));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// 110-digit number with 5-digit exponent
	dec = L"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E+99999";
	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(L"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e99999"));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	// 110-digit negative number with 5-digit negative exponent
	dec = L"-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890e-99999";
	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec, BigDecimal(L"-12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890E-99999"));

	// 120-digit zero
	dec = L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E+00000000000000";
	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec, BigDecimal(0));
}

void BigDecimalTest::fromBigDecimal()
{
	BigDecimal dec1("463326");
	BigDecimal dec2(dec1);

	COMPARE_BIGDECIMAL(dec1, dec2);
}

void BigDecimalTest::fromInt()
{
	BigDecimal dec(-2147483647);

	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(-2147483647));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = 0;

	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = 2147483647;

	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(2147483647));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromUInt()
{
	BigDecimal dec(4294967295u);

	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(4294967295u));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = (unsigned)-1;
	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(4294967295u));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));
}

void BigDecimalTest::fromDouble()
{
	BigDecimal dec(0.0f);

	VERIFY(dec.isZero());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal(0));

	dec = 1.2;

	VERIFY(dec.isPositive());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(1));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal("0.2"));

	dec = -25.6;

	VERIFY(dec.isNegative());
	COMPARE_BIGDECIMAL(dec.integer(), BigDecimal(-25));
	COMPARE_BIGDECIMAL(dec.fractional(), BigDecimal("0.6"));
}

void BigDecimalTest::toString()
{
	BigDecimal dec = 100;
	COMPARE(dec.toString(), std::string("100"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("1E+2"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("100"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat, BigDecimalFormat::UpperCaseExponent)), std::string("100"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat, BigDecimalFormat::LowerCaseExponent)).c_str(), std::string("1e+2").c_str());

	dec = 100000;
	COMPARE(dec.toString(), std::string("100000"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("1E+5"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("100E+3"));

	dec = 1000000;
	COMPARE(dec.toString(), std::string("1E+6"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("1E+6"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("1E+6"));

	dec = "1E-5";
	COMPARE(dec.toString(), std::string("0.00001"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("0.00001"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("0.00001"));

	dec = "1E-6";
	COMPARE(dec.toString(), std::string("1E-6"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::string("1E-6"));
	COMPARE(dec.toString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::string("1E-6"));

	dec = "123.456";
	COMPARE(dec.toString(BigDecimalFormat(4)), BigDecimal("123.5").toString());
	COMPARE_BIGDECIMAL(BigDecimal(dec.toString(BigDecimalFormat(2))), BigDecimal("1.2E+2"));

	COMPARE(BigDecimal("1E-4378").toString(), std::string("1E-4378"));

	dec = 1000;
	dec -= 1000;
	COMPARE(dec.toString(), std::string("0"));
}

void BigDecimalTest::toWideString()
{
	BigDecimal dec = 100;
	COMPARE(dec.toWideString(), std::wstring(L"100"));
	COMPARE(dec.toWideString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat)), std::wstring(L"1E+2"));
	COMPARE(dec.toWideString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat)), std::wstring(L"100"));
	COMPARE(dec.toWideString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::EngineeringFormat, BigDecimalFormat::UpperCaseExponent)), std::wstring(L"100"));
	COMPARE(dec.toWideString(BigDecimalFormat(MAX_IO_PRECISION, BigDecimalFormat::ScientificFormat, BigDecimalFormat::LowerCaseExponent)), std::wstring(L"1e+2"));

	dec = "123.456";
	COMPARE(dec.toWideString(BigDecimalFormat(4)), BigDecimal(L"123.5").toWideString());
	COMPARE_BIGDECIMAL(BigDecimal(dec.toWideString(BigDecimalFormat(2))), BigDecimal(L"1.2E+2"));

	COMPARE(BigDecimal("1E-4378").toWideString(), std::wstring(L"1E-4378"));
}

void BigDecimalTest::toInt()
{
	BigDecimal dec(-2147483647);
	COMPARE(dec.toInt(), -2147483647);

	dec = 0;
	COMPARE(dec.toInt(), 0);

	dec = 2147483647;
	COMPARE(dec.toInt(), 2147483647);
}

void BigDecimalTest::toUInt()
{
	BigDecimal dec((unsigned)-1);
	COMPARE(dec.toUInt(), 4294967295u);

	dec = 0;
	COMPARE(dec.toUInt(), 0u);

	dec = 4294967295u;
	COMPARE(dec.toUInt(), 4294967295u);
}

void BigDecimalTest::unaryOperators()
{
	BigDecimal dec(10);
	COMPARE_BIGDECIMAL(+dec, BigDecimal(10));
	COMPARE_BIGDECIMAL(-dec, BigDecimal(-10));

	dec = -125;
	COMPARE_BIGDECIMAL(+dec, BigDecimal(-125));
	COMPARE_BIGDECIMAL(-dec, BigDecimal(+125));

	dec++;
	COMPARE_BIGDECIMAL(dec, BigDecimal(-124));

	dec--;
	COMPARE_BIGDECIMAL(dec, BigDecimal(-125));

	COMPARE_BIGDECIMAL(dec++, BigDecimal(-125));
	COMPARE_BIGDECIMAL(dec--, BigDecimal(-124));
	COMPARE_BIGDECIMAL(--dec, BigDecimal(-126));
	COMPARE_BIGDECIMAL(++dec, BigDecimal(-125));
}

void BigDecimalTest::binaryArithmeticOperators()
{
	BigDecimal dec1(-67), dec2(67);

	COMPARE_BIGDECIMAL(dec1 + dec2, BigDecimal(0));
	COMPARE_BIGDECIMAL(dec1 - dec2, BigDecimal(-134));
	COMPARE_BIGDECIMAL(dec1 * dec2, BigDecimal(-4489));
	COMPARE_BIGDECIMAL(dec1 / dec2, BigDecimal(-1));
	COMPARE_BIGDECIMAL(dec1 % dec2, BigDecimal(0));

	COMPARE_BIGDECIMAL(dec1 += dec2, BigDecimal(0));
	COMPARE_BIGDECIMAL(dec1 -= dec2, BigDecimal(-67));
	COMPARE_BIGDECIMAL(dec1 *= dec2, BigDecimal(-4489));
	COMPARE_BIGDECIMAL(dec1 /= dec2, BigDecimal(-67));
	COMPARE_BIGDECIMAL(dec1 %= dec2, BigDecimal(0));

	COMPARE_BIGDECIMAL(dec2 + 0, BigDecimal(67));
	COMPARE_BIGDECIMAL(dec2 - 0, BigDecimal(67));
	COMPARE_BIGDECIMAL(dec2 * 0, BigDecimal(0));

	FAIL_TEST(dec2 /= 0, "Division by zero", DivisionByZeroException);
	FAIL_TEST(dec1 %= 0, "Division by zero", ArithmeticException);

	COMPARE_BIGDECIMAL(BigDecimal(std::string("100000000000000000")) / BigDecimal(std::string("2")), BigDecimal("5E+16"));
	COMPARE_BIGDECIMAL((BigDecimal("12234.234") * BigDecimal("2434.23443") / BigDecimal("3.1")),
		BigDecimal("9.60677213789568387096774193548387096774193548387097e6"));
}

void BigDecimalTest::binaryLogicalOperators()
{
	BigDecimal dec(1010);

	COMPARE_BIGDECIMAL(dec | 11, BigDecimal(1011));
	COMPARE_BIGDECIMAL(dec & 11, BigDecimal(10));
	COMPARE_BIGDECIMAL(dec ^ 11, BigDecimal(1001));
	COMPARE_BIGDECIMAL(dec << 2, BigDecimal(101000));
	COMPARE_BIGDECIMAL(dec >> 2, BigDecimal(10));
	COMPARE_BIGDECIMAL(~~dec, BigDecimal(1010));

	COMPARE_BIGDECIMAL(dec |= 11, BigDecimal(1011));
	COMPARE_BIGDECIMAL(dec &= 11, BigDecimal(11));
	COMPARE_BIGDECIMAL(dec ^= 11, BigDecimal(0));
	COMPARE_BIGDECIMAL(dec <<= 2, BigDecimal(0));
	COMPARE_BIGDECIMAL(dec >>= 2, BigDecimal(0));

	COMPARE_BIGDECIMAL((~dec & 0) + 1, BigDecimal(1));

	FAIL_TEST(dec | 1.1, "dec | 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec & 1.1, "dec & 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec ^ 1.1, "dec ^ 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec << 1.1, "dec << 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec >> 1.1, "dec >> 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(~BigDecimal(-2.56), "~BigDecimal(-2.56)", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) | 1, "BigDecimal(1.1) | 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) & 1, "BigDecimal(1.1) & 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) ^ 1, "BigDecimal(1.1) ^ 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) << 1, "BigDecimal(1.1) << 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) >> 1, "BigDecimal(1.1) >> 1", LogicalOperationOnFractionalNumberException);

	FAIL_TEST(dec |= 1.1, "dec |= 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec &= 1.1, "dec &= 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec ^= 1.1, "dec ^= 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec <<= 1.1, "dec <<= 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(dec >>= 1.1, "dec >>= 1.1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) |= 1, "BigDecimal(1.1) |= 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) &= 1, "BigDecimal(1.1) &= 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) ^= 1, "BigDecimal(1.1) ^= 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) <<= 1, "BigDecimal(1.1) <<= 1", LogicalOperationOnFractionalNumberException);
	FAIL_TEST(BigDecimal(1.1) >>= 1, "BigDecimal(1.1) >>= 1", LogicalOperationOnFractionalNumberException);
}

void BigDecimalTest::comparisonOperators()
{
	BigDecimal dec1("67834E-2"), dec2("67834E-1");

	COMPARE(dec1 == dec2, false);
	COMPARE(dec1 != dec2, true);
	COMPARE(dec1 >= dec2, false);
	COMPARE(dec1 <= dec2, true);
	COMPARE(dec1 > dec2, false);
	COMPARE(dec1 < dec2, true);

	dec1 = 123;
	dec2 = "12.3E+1";

	COMPARE(dec1 == dec2, true);
	COMPARE(dec1 != dec2, false);
	COMPARE(dec1 >= dec2, true);
	COMPARE(dec1 <= dec2, true);
	COMPARE(dec1 > dec2, false);
	COMPARE(dec1 < dec2, false);
}

void BigDecimalTest::round()
{
	COMPARE_BIGDECIMAL(BigDecimal(0).round(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(101).round(), BigDecimal(101));
	COMPARE_BIGDECIMAL(BigDecimal(-101).round(), BigDecimal(-101));
	COMPARE_BIGDECIMAL(BigDecimal(10.1).round(), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal(10.5).round(), BigDecimal(11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.1).round(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.4).round(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.5).round(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.6).round(), BigDecimal(-11));
}

void BigDecimalTest::integer()
{
	COMPARE_BIGDECIMAL(BigDecimal(0).integer(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(101).integer(), BigDecimal(101));
	COMPARE_BIGDECIMAL(BigDecimal(-101).integer(), BigDecimal(-101));
	COMPARE_BIGDECIMAL(BigDecimal(10.1).integer(), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal(10.5).integer(), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.1).integer(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.4).integer(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.5).integer(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.6).integer(), BigDecimal(-10));
}

void BigDecimalTest::fractional()
{
	COMPARE_BIGDECIMAL(BigDecimal(0).fractional(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(101).fractional(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(-101).fractional(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(10.1).fractional(), BigDecimal(0.1));
	COMPARE_BIGDECIMAL(BigDecimal(10.5).fractional(), BigDecimal(0.5));
	COMPARE_BIGDECIMAL(BigDecimal(-10.1).fractional(), BigDecimal(0.1));
	COMPARE_BIGDECIMAL(BigDecimal(-10.4).fractional(), BigDecimal(0.4));
	COMPARE_BIGDECIMAL(BigDecimal(-10.5).fractional(), BigDecimal(0.5));
	COMPARE_BIGDECIMAL(BigDecimal(-10.6).fractional(), BigDecimal(0.6));
}

void BigDecimalTest::floor()
{
	COMPARE_BIGDECIMAL(BigDecimal(0).floor(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(101).floor(), BigDecimal(101));
	COMPARE_BIGDECIMAL(BigDecimal(-101).floor(), BigDecimal(-101));
	COMPARE_BIGDECIMAL(BigDecimal(10.1).floor(), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal(10.5).floor(), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.1).floor(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.4).floor(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.5).floor(), BigDecimal(-11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.6).floor(), BigDecimal(-11));
}

void BigDecimalTest::ceil()
{
	COMPARE_BIGDECIMAL(BigDecimal(0).ceil(), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal(101).ceil(), BigDecimal(101));
	COMPARE_BIGDECIMAL(BigDecimal(-101).ceil(), BigDecimal(-101));
	COMPARE_BIGDECIMAL(BigDecimal(10.1).ceil(), BigDecimal(11));
	COMPARE_BIGDECIMAL(BigDecimal(10.5).ceil(), BigDecimal(11));
	COMPARE_BIGDECIMAL(BigDecimal(-10.1).ceil(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.4).ceil(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.5).ceil(), BigDecimal(-10));
	COMPARE_BIGDECIMAL(BigDecimal(-10.6).ceil(), BigDecimal(-10));
}

void BigDecimalTest::abs()
{
	COMPARE_BIGDECIMAL(BigDecimal::abs(-10), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal::abs(10), BigDecimal(10));
	COMPARE_BIGDECIMAL(BigDecimal::abs(0), BigDecimal(0));
}

void BigDecimalTest::exp()
{
	COMPARE_BIGDECIMAL(BigDecimal::exp(0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::exp(1), BigDecimal::E);
	COMPARE_BIGDECIMAL(BigDecimal::exp(1), BigDecimal("2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174136"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(BigDecimal::PI), BigDecimal("23.140692632779269005729086367948547380266106242600211993445046409524342350690452783516971997067549219675952704801087773144428044"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(1234), BigDecimal("8.3059759373617942182585212913113567351910010438992024991210999545492331500820324922862233228716890078380720323694149020571906115e535"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(12345), BigDecimal("2.3194183868394946817299619047370658982820899095773852409438853274041215035312397525436856903417051288471826112417654132042519174e5361"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(1234567), BigDecimal("4.32155308254331923284838206708920614321292288588048e536165"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(-1234), BigDecimal("1.20395244043727320131189513515920304450725863881338e-536"));
	COMPARE_BIGDECIMAL(BigDecimal::exp(-1234567), BigDecimal("2.31398291516872974457803107831416286758592836651656e-536166"));
	
	FAIL_TEST(BigDecimal::exp(12345678), "Exponent overflow", OverflowException);
	FAIL_TEST(BigDecimal::exp(BigDecimal::ln(0)), "ln(0)", InvalidArgumentInLogException);
}

void BigDecimalTest::ln()
{
	COMPARE_BIGDECIMAL(BigDecimal::ln(1), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::exp(0)), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::exp("-3467.2")), BigDecimal("-3467.2"));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::E), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::E * BigDecimal::E), BigDecimal(2));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::sqr(BigDecimal::E)), BigDecimal(2));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal::pow(BigDecimal::E, 125)), BigDecimal(125));

	COMPARE_BIGDECIMAL(BigDecimal::ln(10), BigDecimal("2.30258509299404568401799145468436420760110148862877"));
	COMPARE_BIGDECIMAL(BigDecimal::ln(1234567), BigDecimal("14.02623085927966025643327828545583354742843395757568"));
	COMPARE_BIGDECIMAL(BigDecimal::ln(BigDecimal("1234567e-100000")), BigDecimal("-230244.48306854528874154271219015096492656272042891972193"));

	FAIL_TEST(BigDecimal::ln(0), "ln(0)", InvalidArgumentInLogException);
	FAIL_TEST(BigDecimal::ln(-5), "ln(-5)", InvalidArgumentInLogException);
}

void BigDecimalTest::log10()
{
	COMPARE_BIGDECIMAL(BigDecimal::log10(1), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::log10(1000), BigDecimal(3));
	COMPARE_BIGDECIMAL(BigDecimal::log10(BigDecimal("1e-3")), BigDecimal(-3));
	COMPARE_BIGDECIMAL(BigDecimal::log10(BigDecimal("1e-123456")), BigDecimal(-123456));
	COMPARE_BIGDECIMAL(BigDecimal::log10(BigDecimal("1e123456")), BigDecimal(123456));
	COMPARE_BIGDECIMAL(BigDecimal::log10(BigDecimal("12345678901234567890")), BigDecimal("19.09151497721269989570648733341528865609576842916003"));
	
	FAIL_TEST(BigDecimal::log10(0), "log10(0)", InvalidArgumentInLogException);
	FAIL_TEST(BigDecimal::log10(-5), "log10(-5)", InvalidArgumentInLogException);
}

void BigDecimalTest::sqr()
{
	COMPARE_BIGDECIMAL(BigDecimal::sqr(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sqr(4), BigDecimal(16));
	COMPARE_BIGDECIMAL(BigDecimal::sqr(-25), BigDecimal(625));
	COMPARE_BIGDECIMAL(BigDecimal::sqrt(BigDecimal::sqr("534.123")), BigDecimal("534.123"));
	COMPARE_BIGDECIMAL(BigDecimal::sqr(BigDecimal::sqrt("534.123")), BigDecimal("534.123"));
	COMPARE_BIGDECIMAL(BigDecimal::sqr(123456789), BigDecimal("1.52415787501905210000000000000000000000000000000000e16"));
	COMPARE_BIGDECIMAL(BigDecimal::sqr(-999999999), BigDecimal("9.99999998000000001000000000000000000000000000000000e17"));
}

void BigDecimalTest::sqrt()
{
	COMPARE_BIGDECIMAL(BigDecimal::sqrt(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sqrt(16), BigDecimal(4));
	COMPARE_BIGDECIMAL(BigDecimal::sqrt(BigDecimal::pow("534.123", 2)), BigDecimal("534.123"));
	COMPARE_BIGDECIMAL(BigDecimal::sqrt(123456789), BigDecimal("11111.11106055555544054166614335346924587840986013435107"));

	FAIL_TEST(BigDecimal::sqrt(-10), "sqrt(-10)", InvalidArgumentInSqrtException);
}

void BigDecimalTest::pow()
{
	COMPARE_BIGDECIMAL(BigDecimal::pow(0, 0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::pow(-2938, 0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::pow(542, 0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::pow(0, 213), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::pow(4, 4), BigDecimal(256));
	COMPARE_BIGDECIMAL(BigDecimal::pow(BigDecimal::sqrt(625), 2), BigDecimal(625));
	COMPARE_BIGDECIMAL(BigDecimal::pow(BigDecimal("384.432786"), BigDecimal("283.1242")), BigDecimal("6.68672201644936322360548332196885485617596022166251e731"));
	COMPARE_BIGDECIMAL(BigDecimal::pow(BigDecimal("384.432786"), BigDecimal("-283.1242")), BigDecimal("1.49550108040979713633149639757169963489036723785070e-732"));

	FAIL_TEST(BigDecimal::pow(0, -437), "pow(0, -437)", InvalidArgumentInPowException);
	FAIL_TEST(BigDecimal::pow(-12, -437), "pow(-12, -437)", InvalidArgumentInPowException);
}

void BigDecimalTest::div()
{
	COMPARE_BIGDECIMAL(BigDecimal::div(4, 4), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::div("-124.213", 2), BigDecimal(-62));

	FAIL_TEST(BigDecimal::div(4, 0), "div(4, 0)", DivisionByZeroException);
	FAIL_TEST(BigDecimal::div(0, 0), "div(0, 0)", DivisionByZeroException);
}

void BigDecimalTest::max()
{
	COMPARE_BIGDECIMAL(BigDecimal::max(23, 13), BigDecimal(23));
	COMPARE_BIGDECIMAL(BigDecimal::max(-23, 13), BigDecimal(13));
	COMPARE_BIGDECIMAL(BigDecimal::max(23, -13), BigDecimal(23));
	COMPARE_BIGDECIMAL(BigDecimal::max(-23, -13), BigDecimal(-13));
	COMPARE_BIGDECIMAL(BigDecimal::max(-65, -65), BigDecimal(-65));
}

void BigDecimalTest::min()
{
	COMPARE_BIGDECIMAL(BigDecimal::min(23, 13), BigDecimal(13));
	COMPARE_BIGDECIMAL(BigDecimal::min(-23, 13), BigDecimal(-23));
	COMPARE_BIGDECIMAL(BigDecimal::min(23, -13), BigDecimal(-13));
	COMPARE_BIGDECIMAL(BigDecimal::min(-23, -13), BigDecimal(-23));
	COMPARE_BIGDECIMAL(BigDecimal::min(65, 65), BigDecimal(65));
}

void BigDecimalTest::factorial()
{
	// Small factorials
	COMPARE_BIGDECIMAL(BigDecimal::factorial(0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(1), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(2), BigDecimal(2));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(3), BigDecimal(6));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(4), BigDecimal(24));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(5), BigDecimal(120));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(10), BigDecimal(3628800));
	COMPARE_BIGDECIMAL(BigDecimal::factorial(25), BigDecimal("15511210043330985984000000"));

	// Stress test
	COMPARE_BIGDECIMAL_PRECISION(BigDecimal::factorial(450), BigDecimal("1.73336873E+1000"), 9);
	COMPARE_BIGDECIMAL(BigDecimal::factorial(200000), BigDecimal("1.4202253454703144049669463336823059760899653567464E+973350"));

	FAIL_TEST(BigDecimal::factorial(-1), "factorial(-1)", InvalidArgumentInFactorialException);
	FAIL_TEST(BigDecimal::factorial("1.2"), "factorial(1.2)", InvalidArgumentInFactorialException);
	FAIL_TEST(BigDecimal::factorial("-1.2"), "factorial(-1.2)", InvalidArgumentInFactorialException);
}

void BigDecimalTest::sin()
{
	COMPARE_BIGDECIMAL(BigDecimal::sin(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 2), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 3 / 2), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI / 2), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI * 3 / 2), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI * 2), BigDecimal(0));

	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 742342346), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 436677271 / 2), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 436677273 / 2), BigDecimal(1));

	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999999")), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * BigDecimal("1000000000000000000000000000000000000000000000000000000000000000000000000000")), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999997") / 2), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999999") / 2), BigDecimal(-1));

	// SpeedCrunch 0.10.1 fails this test
	// Reference number is calculated by Mathematica 6 with 100 digits precision
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI *
		BigDecimal("34678236483724678236482376437643478374837487384783874637846736473467346347E-15")),
		BigDecimal("0.7365077837912717180169241081842837128232214243058669031072233561867904779639270977189273961709260958"));

	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 4), BigDecimal::sqrt(2) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 4),
		BigDecimal("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 3 / 4),
		BigDecimal("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI / 4).toString().c_str(),
		BigDecimal("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803").toString().c_str());
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI / 4),
		BigDecimal("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI * 3 / 4),
		BigDecimal("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 6), BigDecimal("0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 3), BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI / 6), -BigDecimal("0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(-BigDecimal::PI / 3), -BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 5 / 6), BigDecimal("0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 2 / 3), BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 7 / 6), BigDecimal("-0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI * 4 / 3), BigDecimal::sqrt(3) / -2);
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 3),
		BigDecimal("0.86602540378443864676372317075293618347140262690519031402790348972596650845440001854057309337862428783781307070770335151498497255"));

	COMPARE_BIGDECIMAL(BigDecimal::sin("0.01"), "0.009999833334166664682542438269099729038964385360169151033879112479");
	COMPARE_BIGDECIMAL(BigDecimal::sin("0.001"), "9.999998333333416666664682539710097001513147348086584190048145103e-4");
	COMPARE_BIGDECIMAL(BigDecimal::sin("1e-30"), "9.999999999999999999999999999999999999999999999999999999999998333e-31");
	COMPARE_BIGDECIMAL(BigDecimal::sin("1e-50"), "0.00000000000000000000000000000000000000000000000001");

	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 2 - "0.01"), "0.9999500004166652777802579337522066732124705839802771111222757686");
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 2 - "0.001"), "0.9999995000000416666652777778025793648037918892128961458698562351");
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 2 - "1e-30"), "0.9999999999999999999999999999999999999999999999999999999999995");
	COMPARE_BIGDECIMAL(BigDecimal::sin(BigDecimal::PI / 2 - "1e-50"), "1");
}

void BigDecimalTest::cos()
{
	COMPARE_BIGDECIMAL(BigDecimal::cos(0), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 3 / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 2), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI * 3 / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI * 2), BigDecimal(1));

	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 742342346), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 742342349), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 742342341 / 2), BigDecimal(0));

	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 4),
		BigDecimal("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 3 / 4),
		BigDecimal("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI / 4),
		BigDecimal("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI * 3 / 4),
		BigDecimal("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 3), BigDecimal("0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 6), BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI / 3), BigDecimal("0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(-BigDecimal::PI / 6), BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 2 / 3), BigDecimal("-0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 5 / 6), BigDecimal::sqrt(3) / -2);
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 4 / 3), BigDecimal("-0.5"));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * 7 / 6), -BigDecimal::sqrt(3) / 2);
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 6),
		BigDecimal("0.86602540378443864676372317075293618347140262690519031402790348972596650845440001854057309337862428783781307070770335151498497255"));

	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999999")), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * BigDecimal("99999999999999999999999999999999999999999999999999999999999999999999999999") / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI * BigDecimal("1000000000000000000000000000000000000000000000000000000000000000000000000000")), BigDecimal(1));

	COMPARE_BIGDECIMAL(BigDecimal::cos("0.01"), "0.9999500004166652777802579337522066732124705839802771111222757686");
	COMPARE_BIGDECIMAL(BigDecimal::cos("0.001"), "0.9999995000000416666652777778025793648037918892128961458698562351");
	COMPARE_BIGDECIMAL(BigDecimal::cos("1e-30"), "0.9999999999999999999999999999999999999999999999999999999999995");
	COMPARE_BIGDECIMAL(BigDecimal::cos("1e-50"), "1");

	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 2 - "0.01"), "0.009999833334166664682542438269099729038964385360169151033879112479");
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 2 - "0.001"), "9.999998333333416666664682539710097001513147348086584190048145103e-4");
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 2 - "1e-30"), "9.999999999999999999999999999999999999999999999999999999999998333e-31");
	COMPARE_BIGDECIMAL(BigDecimal::cos(BigDecimal::PI / 2 - "1e-50"), "0.00000000000000000000000000000000000000000000000001");
}

void BigDecimalTest::tan()
{
	COMPARE_BIGDECIMAL(BigDecimal::tan(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI * 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI * 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 4), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 4 * 3), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI / 4), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI / 4 * 3), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 6),
		BigDecimal("0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"));
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 3),
		BigDecimal("1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI / 6),
		BigDecimal("-0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"));
	COMPARE_BIGDECIMAL(BigDecimal::tan(-BigDecimal::PI / 3),
		BigDecimal("-1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"));

	COMPARE_BIGDECIMAL(BigDecimal::tan("0.01"), "0.01000033334666720637107672410198013564984972103620060379847236395");
	COMPARE_BIGDECIMAL(BigDecimal::tan("1e-30"), "1.000000000000000000000000000000000000000000000000000000000000333e-30");
	COMPARE_BIGDECIMAL(BigDecimal::tan("1e-50"), "1e-50");
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 2 - "0.01"), "99.99666664444423280211638073838651820448540766316976");
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 2 - "1e-30"), "1e+30");
	COMPARE_BIGDECIMAL(BigDecimal::tan(BigDecimal::PI / 2 - "1e-50"), "1e+50");

	FAIL_TEST(BigDecimal::tan(-BigDecimal::PI / 2), "tan(-pi/2)", InvalidArgumentInTanException);
	FAIL_TEST(BigDecimal::tan(BigDecimal::PI / 2), "tan(pi/2)", InvalidArgumentInTanException);
	FAIL_TEST(BigDecimal::tan(BigDecimal::PI / 2 * 3), "tan(3*pi/2)", InvalidArgumentInTanException);
	FAIL_TEST(BigDecimal::tan(BigDecimal::PI / 2 * 101), "tan(101*pi/2)", InvalidArgumentInTanException);
	FAIL_TEST(BigDecimal::tan(BigDecimal::PI / 2 * 10000001), "tan(10000001*pi/2)", InvalidArgumentInTanException);
}

void BigDecimalTest::cot()
{
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI * 3 / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI * 3 / 2), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 4), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI * 3/ 4), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI / 4), BigDecimal(-1));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI * 3 / 4), BigDecimal(1));
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 3),
		BigDecimal("0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"));
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 6),
		BigDecimal("1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI / 3),
		BigDecimal("-0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"));
	COMPARE_BIGDECIMAL(BigDecimal::cot(-BigDecimal::PI / 6),
		BigDecimal("-1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"));

	COMPARE_BIGDECIMAL(BigDecimal::cot("0.01"), "99.99666664444423280211638073838651820448540766316976");
	COMPARE_BIGDECIMAL(BigDecimal::cot("1e-30"), "1e+30");
	COMPARE_BIGDECIMAL(BigDecimal::cot("1e-50"), "1e+50");
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 2 - "0.01"), "0.01000033334666720637107672410198013564984972103620060379847236395");
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 2 - "1e-30"), "1.000000000000000000000000000000000000000000000000000000000000333e-30");
	COMPARE_BIGDECIMAL(BigDecimal::cot(BigDecimal::PI / 2 - "1e-50"), "1e-50");

	FAIL_TEST(BigDecimal::cot(-BigDecimal::PI), "cot(-pi)", InvalidArgumentInCotException);
	FAIL_TEST(BigDecimal::cot(0), "cot(0)", InvalidArgumentInCotException);
	FAIL_TEST(BigDecimal::cot(BigDecimal::PI), "cot(pi)", InvalidArgumentInCotException);
	FAIL_TEST(BigDecimal::cot(BigDecimal::PI * 3), "cot(3*pi)", InvalidArgumentInCotException);
	FAIL_TEST(BigDecimal::cot(BigDecimal::PI * 101), "cot(101*pi)", InvalidArgumentInCotException);
	FAIL_TEST(BigDecimal::cot(BigDecimal::PI * 10000001), "cot(10000001*pi)", InvalidArgumentInCotException);
}

void BigDecimalTest::arcsin()
{
	COMPARE_BIGDECIMAL(BigDecimal::arcsin(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::arcsin(1), BigDecimal(BigDecimal::PI / 2));
	COMPARE_BIGDECIMAL(BigDecimal::arcsin(-1), BigDecimal(- BigDecimal::PI / 2));
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"),
		BigDecimal::PI / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"),
		-BigDecimal::PI / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("0.5"), BigDecimal::PI / 6);
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("-0.5"), -BigDecimal::PI / 6);

	COMPARE_BIGDECIMAL(BigDecimal::arcsin("0.01"), "0.01000016667416711312562227707199038367857039364300919838632951405");
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("1e-30"), "1e-30");
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("1e-50"), "1e-50");
	COMPARE_BIGDECIMAL(BigDecimal::arcsin("0.99"), "1.429256853470469400485532334664724427104601769147799717179321293");

	FAIL_TEST(BigDecimal::arcsin("1.1"), "arcsin(1.1)", InvalidArgumentInArcSinException);
	FAIL_TEST(BigDecimal::arcsin(-2), "arcsin(-2)", InvalidArgumentInArcSinException);
}

void BigDecimalTest::arccos()
{
	COMPARE_BIGDECIMAL(BigDecimal::arccos(0), BigDecimal(BigDecimal::PI / 2));
	COMPARE_BIGDECIMAL(BigDecimal::arccos(1), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::arccos(-1), BigDecimal(BigDecimal::PI));
	COMPARE_BIGDECIMAL(BigDecimal::arccos("0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"),
		BigDecimal(BigDecimal::PI / 4));
	COMPARE_BIGDECIMAL(BigDecimal::arccos("-0.70710678118654752440084436210484903928483593768847403658833986899536623923105351942519376716382078636750692311545614851246241803"),
		BigDecimal(BigDecimal::PI * 3 / 4));
	COMPARE_BIGDECIMAL(BigDecimal::arccos("0.5"), BigDecimal::PI / 3);
	COMPARE_BIGDECIMAL(BigDecimal::arccos("-0.5"), BigDecimal::PI * 2 / 3);

	COMPARE_BIGDECIMAL(BigDecimal::arccos("0.01"), "1.560796160120729506105699414567761058420014306044543712101142782");
	COMPARE_BIGDECIMAL(BigDecimal::arccos("1e-30"), BigDecimal::PI / 2 - "1e-30");
	COMPARE_BIGDECIMAL(BigDecimal::arccos("1e-50"), BigDecimal::PI / 2 - "1e-50");
	COMPARE_BIGDECIMAL(BigDecimal::arccos("0.99"), "0.1415394733244272187457893569750270149939829305397531933081510031");

	FAIL_TEST(BigDecimal::arccos("1.1"), "arccos(1.1)", InvalidArgumentInArcCosException);
	FAIL_TEST(BigDecimal::arccos(-2), "arccos(-2)", InvalidArgumentInArcCosException);
}

void BigDecimalTest::arctan()
{
	COMPARE_BIGDECIMAL(BigDecimal::arctan(0), BigDecimal(0));
	COMPARE_BIGDECIMAL(BigDecimal::arctan(1), BigDecimal::PI / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arctan(-1), -BigDecimal::PI / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arctan("0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"),
		BigDecimal::PI / 6);
	COMPARE_BIGDECIMAL(BigDecimal::arctan("1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"),
		BigDecimal::PI / 3);
	COMPARE_BIGDECIMAL(BigDecimal::arctan("-0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"),
		-BigDecimal::PI / 6);
	COMPARE_BIGDECIMAL(BigDecimal::arctan("-1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"),
		BigDecimal::PI / -3);
}

void BigDecimalTest::arccot()
{
	COMPARE_BIGDECIMAL(BigDecimal::arccot(0), BigDecimal::PI / 2);
	COMPARE_BIGDECIMAL(BigDecimal::arccot(1), BigDecimal::PI / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arccot(-1), BigDecimal::PI * 3 / 4);
	COMPARE_BIGDECIMAL(BigDecimal::arccot("0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"),
		BigDecimal::PI / 3);
	COMPARE_BIGDECIMAL(BigDecimal::arccot("1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"),
		BigDecimal::PI / 6);
	COMPARE_BIGDECIMAL(BigDecimal::arccot("-0.57735026918962576450914878050195745564760175127012687601860232648397767230293334569371539558574952522520871380513556767665664836"),
		BigDecimal::PI * 2 / 3);
	COMPARE_BIGDECIMAL(BigDecimal::arccot("-1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756756261414154067030299699451"),
		BigDecimal::PI * 5 / 6);
}

void BigDecimalTest::consts()
{
	// Reference values are calculated by PowerCalc and have 128 digits precision

	// Check PI
	COMPARE_BIGDECIMAL(BigDecimal::PI,
		BigDecimal("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446"));

	// Check PI / 2
	COMPARE_BIGDECIMAL(BigDecimal::PI / 2,
		BigDecimal("1.5707963267948966192313216916397514420985846996875529104874722961539082031431044993140174126710585339910740432566411533235469223"));

	// Check PI / 4
	COMPARE_BIGDECIMAL(BigDecimal::PI / 4,
		BigDecimal("0.78539816339744830961566084581987572104929234984377645524373614807695410157155224965700870633552926699553702162832057666177346115"));

	// Check PI * 2
	COMPARE_BIGDECIMAL(BigDecimal::PI * 2,
		BigDecimal("6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359642961730265646132941876892"));

	// Check E
	COMPARE_BIGDECIMAL(BigDecimal::E,
		BigDecimal("2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274274663919320030599218174136"));
}
