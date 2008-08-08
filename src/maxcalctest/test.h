
#ifndef TEST_H
#define TEST_H

#include <iostream>

using namespace std;

extern int testCount;
extern int failedTestCount;

#define TEST(expr) Test(__FILE__, __LINE__, expr, #expr)

void Test(const char * fileName, int lineNumber, bool isSucceded, const char * expr);

#endif // TEST_H
