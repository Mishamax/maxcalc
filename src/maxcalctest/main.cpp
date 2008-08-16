
#include "bigdecimaltest.h"
#include <QTest>

int main()
{
	BigDecimalTest bigDecimalTest;
	QTest::qExec(&bigDecimalTest);
	return 0;
}
