
#ifndef BIGDECIMALTEST_H
#define BIGDECIMALTEST_H

#include "bigdecimal.h"
#include <QTest>

class BigDecimalTest : public QObject
{
	Q_OBJECT

private slots:
	// Test for default BigDecimalFormat
	void bigDecimalFormat();

	// Constructors
	void fromQString();
	void fromCharStr();
	void fromBigDecimal();
	void fromInt();
	void fromUInt();
	void constructionStress();

	// Conversions
	void toString();
	void toInt();
	void toUInt();

	// Operators
	void unaryOperators();
	void binaryArithmeticOperators();
	void binaryLogicalOperators();
	void comparisonOperators();

	// Functions
	void abs();
	void exp();
	void ln();
	void log10();
	void sqrt();
	void pow();
	void div();
	void max();
	void min();
};

#endif // BIGDECIMALTEST_H
