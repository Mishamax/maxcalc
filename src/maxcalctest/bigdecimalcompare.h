
#ifndef BIGDECIMALCOMPARE_H
#define BIGDECIMALCOMPARE_H

// MaxCalc Engine
#include "bigdecimal.h"
// Qt
#include <QTest>

using namespace MaxCalcEngine;

// Compares two BigDecimals with specified precision
#define COMPARE_WITH_PRECISION(n1, n2, precision) \
	QCOMPARE(BigDecimal((n1).toString(BigDecimalFormat(precision)).c_str()), \
		BigDecimal((n2).toString(BigDecimalFormat(precision)).c_str()));

// Default precision for comparison
#define DEFAULT_PRECISION MAX_IO_PRECISION

#endif // BIGDECIMALCOMPARE_H
