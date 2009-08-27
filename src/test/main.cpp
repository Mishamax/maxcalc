
// Local
#include "bigdecimaltest.h"
#include "complextest.h"
#include "parsertest.h"
#include "bigdecimalbench.h"
#include "variablestest.h"
#include "unitconversiontest.h"
// Qt
#include <QTest>

int main()
{
	BigDecimalTest bigDecimalTest;
	QTest::qExec(&bigDecimalTest);

	ComplexTest complexTest;
	QTest::qExec(&complexTest);

	ParserTest parserTest;
	QTest::qExec(&parserTest);

	VariablesTest variablesTest;
	QTest::qExec(&variablesTest);

	UnitConversionTest unitConversionTest;
	QTest::qExec(&unitConversionTest);

//	BigDecimalBench bigDecimalBench;
//	QTest::qExec(&bigDecimalBench);

	return 0;
}
