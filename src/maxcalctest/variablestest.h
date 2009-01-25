
#ifndef VARIABLESTEST_H
#define VARIABLESTEST_H

// Qt
#include <QTest>

class VariablesTest : public QObject
{
	Q_OBJECT

private slots:
	void basic();
	void stress();
	void iterators();
};

#endif // VARIABLESTEST_H
