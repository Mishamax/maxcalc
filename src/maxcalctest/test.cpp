
#include "test.h"

int testCount = 0;
int failedTestCount = 0;

void Test(const char * fileName, int lineNumber, bool isSucceded, const char * expr)
{
	testCount++;
	if (!isSucceded)
	{
		failedTestCount++;
		cerr << endl << fileName << "\nLine " << lineNumber << ":\t" << expr << endl << endl;
	}
}

void main()
{
}
