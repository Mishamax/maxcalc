
#ifndef PARSERTEST_H
#define PARSERTEST_H

// Qt
#include <QTest>

class ParserTest : public QObject
{
	Q_OBJECT

private slots:
	void basic();
	void numbers();
	void complexNumbers();
	void addSub();
	void mulDiv();
	void fails();
};

#endif // PARSERTEST_H
