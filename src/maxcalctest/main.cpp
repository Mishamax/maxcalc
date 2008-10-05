
// Local
#include "bigdecimaltest.h"
#include "complextest.h"
// Qt
#include <QTest>

int main()
{
	BigDecimalTest bigDecimalTest;
	QTest::qExec(&bigDecimalTest);

	ComplexTest complexTest;
	QTest::qExec(&complexTest);

	return 0;
}
