
#ifndef COMPARE_H
#define COMPARE_H

// MaxCalc Engine
#include "complex.h"
// Qt
#include <QTest>

using namespace MaxCalcEngine;

// Default precision for number comparison
#define DEFAULT_PRECISION MAX_IO_PRECISION

#define COMPARE(a, b) QCOMPARE(a, b)

#define VERIFY(a) QVERIFY(a)

// Compares two BigDecimal numbers with default precision (DEFAULT_PRECISION)
#define COMPARE_BIGDECIMAL(n1, n2) \
	QCOMPARE(BigDecimal(n1).toString(BigDecimalFormat(DEFAULT_PRECISION)).c_str(), \
		BigDecimal(n2).toString(BigDecimalFormat(DEFAULT_PRECISION)).c_str());

// Compares two BigDecimal numbers with specified precision
#define COMPARE_BIGDECIMAL_PRECISION(n1, n2, precision) \
	QCOMPARE(BigDecimal(n1).toString(BigDecimalFormat(precision)).c_str(), \
		BigDecimal(n2).toString(BigDecimalFormat(precision)).c_str());

// Compares two Complex numbers with specified precision
#define COMPARE_COMPLEX(n1, n2) \
	QCOMPARE(Complex(n1).toString(ComplexFormat(DEFAULT_PRECISION)).c_str(), \
		Complex(n2).toString(ComplexFormat(DEFAULT_PRECISION)).c_str());

// Compares two Complex numbers with specified precision
#define COMPARE_COMPLEX_PRECISION(n1, n2, precision) \
	QCOMPARE(Complex(n1).toString(ComplexFormat(precision)).c_str(), \
		Complex(n2).toString(ComplexFormat(precision)).c_str());

// Fail test
#define FAIL_TEST(code, error_message, exception) \
	try \
	{ \
		{ code; } \
		QFAIL(error_message); \
	} \
	catch (exception) {}

// Executes parser to evaluate expression and compares it with result
#define PARSER_TEST(parser, expression, expression_result) \
	parser.setExpression(expression); \
	parser.parse(); \
	COMPARE_COMPLEX(parser.context().result(), expression_result);

// Executes parser to evaluate expression and checks for fail
#define PARSER_FAIL_TEST(parser, expression, error_message, exception) \
	parser.setExpression(expression); \
	FAIL_TEST(parser.parse(), error_message, exception);

#endif // COMPARE_H
