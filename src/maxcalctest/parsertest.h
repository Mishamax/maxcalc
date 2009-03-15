
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
	void unaryPlusMinus();
	void power();
	void powerAndUnaryPlusMinus();
	void brackets();
	void constsAndVars();
	void userVars();
	void functions();
	void functionsBasic();
	void functionsTrig();
	void functionsLog();
	void fails();
	void realWorld();
	void unitConversions();
};

#endif // PARSERTEST_H
