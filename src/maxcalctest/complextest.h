
#ifndef COMPLEXTEST_H
#define COMPLEXTEST_H

// Qt
#include <QTest>

class ComplexTest : public QObject
{
	Q_OBJECT

private slots:
	// ComplexFormat tests
	void complexFormatDefault();
	void complexFormatCustom();
	void complexFormatAccessors();

};

#endif // COMPLEXTEST_H
