
#ifndef BIGDECIMALTEST_H
#define BIGDECIMALTEST_H

#include "bigdecimal.h"
#include <QTest>

// Compares two BigDecimals with specified precision
#define COMPARE_WITH_PRECISION(n1, n2, precision) \
	QCOMPARE(BigDecimal(n1.toString(BigDecimalFormat(precision))), \
		BigDecimal(n2.toString(BigDecimalFormat(precision))));

// Default precision for comparison
// All functions except for trigonometrical pass with DECNUMDIGITS - 2 precision
#define DEFAULT_PRECISION DECNUMDIGITS - 3

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
	void sqr();
	void sqrt();
	void pow();
	void div();
	void max();
	void min();
	void fact();
	void sin();
	void cos();
	void tan();
	void ctan();

	// Misc
	void consts();
};

#endif // BIGDECIMALTEST_H
