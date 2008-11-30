
// Local
#include "bigdecimaltest.h"
#include "complextest.h"
#include "parsertest.h"
// Qt
#include <QTest>

int main()
{
	BigDecimalTest bigDecimalTest;
//	QTest::qExec(&bigDecimalTest);

	ComplexTest complexTest;
	QTest::qExec(&complexTest);

	ParserTest parserTest;
	QTest::qExec(&parserTest);

	return 0;
}
