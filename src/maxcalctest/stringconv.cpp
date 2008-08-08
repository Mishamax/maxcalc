
#include <iostream>

#include "stringconv.h"
#include "test.h"

using namespace std;

void multiByteToWideCharTest1()
{
	char str[] = "Hello";
	wchar_t wstr[6];
	StringConv::multiByteToWideChar(StringConv::LOCAL_8BIT, str, wstr, 6);
	TEST(wcscmp(wstr, L"Hello") == 0);
	StringConv::multiByteToWideChar(StringConv::UTF8, str, wstr, 6);
	TEST(wcscmp(wstr, L"Hello") == 0);
}

void multiByteToWideCharTest2()
{
	string str = "Hello";
	wstring wstr;
	StringConv::multiByteToWideChar(StringConv::LOCAL_8BIT, str, wstr);
	TEST(wstr == L"Hello");
	StringConv::multiByteToWideChar(StringConv::UTF8, str, wstr);
	TEST(wstr == L"Hello");
}

void wideCharToMultiByteTest1()
{
	wchar_t wstr[] = L"Hello";
	char str[6];
	StringConv::wideCharToMultiByte(StringConv::LOCAL_8BIT, wstr, str, 6);
	TEST(strcmp(str, "Hello") == 0);
	StringConv::wideCharToMultiByte(StringConv::UTF8, wstr, str, 6);
	TEST(strcmp(str, "Hello") == 0);
}

void wideCharToMultiByteTest2()
{
	wstring wstr = L"Hello";
	string str;
	StringConv::wideCharToMultiByte(StringConv::LOCAL_8BIT, wstr, str);
	TEST(str == "Hello");
	StringConv::wideCharToMultiByte(StringConv::UTF8, wstr, str);
	TEST(str == "Hello");
}

int main()
{
	multiByteToWideCharTest1();
	multiByteToWideCharTest2();
	wideCharToMultiByteTest1();
	wideCharToMultiByteTest2();
	return failedTestCount;
}
