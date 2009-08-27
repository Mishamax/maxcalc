
#ifndef COMPLEXTEST_H
#define COMPLEXTEST_H

// Qt
#include <QTest>

class ComplexTest : public QObject
{
	Q_OBJECT

private slots:
	// ComplexFormat tests
	void complexFormatDefault();
	void complexFormatCustom();
	void complexFormatAccessors();

	// Constructors
	void fromString();
	void fromCharStr();
	void fromWideString();
	void fromWideCharStr();
	void fromBigDecimal();
	void fromInt();
	void fromUInt();

	// Conversions
	void toString();
	void toWideString();

	// Operators
	void unaryOperators();
	void binaryOperators();
	void comparisonOperators();

	// Misc functions
	void isZero();

	// Math functions
	void sqr();
	void abs();
	void arg();
	void ln();
};

#endif // COMPLEXTEST_H
