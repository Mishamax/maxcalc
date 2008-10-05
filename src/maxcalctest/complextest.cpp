
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
