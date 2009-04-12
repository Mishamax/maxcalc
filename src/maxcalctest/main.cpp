
// Local
#include "bigdecimaltest.h"
#include "complextest.h"
#include "parsertest.h"
#include "bigdecimalbench.h"
#include "variablestest.h"
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

//	BigDecimalBench bigDecimalBench;
//	QTest::qExec(&bigDecimalBench);

	return 0;
}
