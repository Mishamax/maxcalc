
// Local
#include "bigdecimaltest.h"
#include "complextest.h"
#include "parsertest.h"
#include "bigdecimalbench.h"
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

	BigDecimalBench bigDecimalBench;
	QTest::qExec(&bigDecimalBench);

	return 0;
}
