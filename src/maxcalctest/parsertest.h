
#ifndef PARSERTEST_H
#define PARSERTEST_H

// Qt
#include <QTest>

class ParserTest : public QObject
{
	Q_OBJECT

private slots:
	void addSub();
	void mulDiv();
};

#endif // PARSERTEST_H
