
#ifndef UNITCONVERSIONTEST_H
#define UNITCONVERSIONTEST_H

// MaxCalcEngine
#include "unicode.h"
// Qt
#include <QTest>

class UnitConversionTest : public QObject
{
	Q_OBJECT

private:
	bool isUnit(const MaxCalcEngine::tstring str);

private slots:
	void convert();
	void iterators();
};

#endif // UNITCONVERSIONTEST_H
