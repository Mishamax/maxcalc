
#ifndef BIGDECIMALBENCH_H
#define BIGDECIMALBENCH_H

// Qt
#include <QTest>

class BigDecimalBench : public QObject
{
	Q_OBJECT

private slots:
	// Math functions
	void fact5();
	void fact10();
	void fact15();
	void fact20();
	void fact50();
	void fact100();
	void fact500();
	void fact200000();
};

#endif // BIGDECIMALBENCH_H
