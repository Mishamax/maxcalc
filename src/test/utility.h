/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2010 Michael Maximov (michael.maximov@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *****************************************************************************/

#ifndef COMPARE_H
#define COMPARE_H

// MaxCalc Engine
#include "complex.h"
#include "constants.h"
// Qt
#include <QTest>


// Default precision for number comparison
#define DEFAULT_PRECISION Constants::MAX_IO_PRECISION

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
    try { \
        { code; } \
        QFAIL(error_message); \
    } catch (exception) { \
    } catch (...) { \
        QFAIL("incorrect exception caught"); \
    }

// Executes parser to evaluate expression and compares it with result
#define PARSER_TEST(parser, expression, expression_result) \
    parser.setExpression(expression); \
    parser.parse(); \
    COMPARE_COMPLEX(parser.context().result(), expression_result);

// Executes parser to evaluate expression and checks for fail
#define PARSER_FAIL_TEST(parser, expression, error_message, exception) \
    parser.setExpression(expression); \
    FAIL_TEST(parser.parse(), error_message, exception);

#if QT_VERSION >= 0x040500
#define BENCHMARK(expression) QBENCHMARK(expression)
#else
#define BENCHMARK(expression)
#endif

#endif // COMPARE_H
