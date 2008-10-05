
#ifndef BIGDECIMALTEST_H
#define BIGDECIMALTEST_H

// Qt
#include <QTest>

class BigDecimalTest : public QObject
{
	Q_OBJECT

private slots:
	// BigDecimalFormat tests
	void bigDecimalFormatDefault();
	void bigDecimalFormatCustom();
	void bigDecimalFormatAccessors();

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
	void factorial();
	void sin();
	void cos();
	void tan();
	void ctan();
	void arcsin();
	void arccos();
	void arctan();
	void arccot();

	// Misc
	void consts();
};

#endif // BIGDECIMALTEST_H
