
// Local
#include "variablestest.h"
#include "utility.h"
// MaxCalcEngine
#include "variables.h"
// STL
#include <string>
#include <cstdlib>
#include <sstream>
#include <ctime>

using namespace MaxCalcEngine;
using namespace std;

void VariablesTest::basic()
{
	Variables vars;
	COMPARE(vars.count(), size_t(0));
	vars.add(_T("x"), 1);
	COMPARE(vars.count(), size_t(1));
	COMPARE_COMPLEX(vars[_T("x")], 1);
	vars.remove(_T("x"));
	COMPARE(vars.count(), size_t(0));
	vars.removeAll();
	COMPARE(vars.count(), size_t(0));
}

void VariablesTest::stress()
{
	Variables vars;
	wchar_t stri[6];
	wstring name;
	int rand1, rand2;

	srand((unsigned)time(0));
	for (int i = 0; i < 10000; ++i)
	{
#if !defined(_MSC_VER) && defined(WIN32)
        swprintf(stri, L"%d", i);
#else
        swprintf(stri, 6, L"%d", i);
#endif
		name = L"Variable#";
		name += stri;
		while ((rand1 = rand()) == 0) {}
		while ((rand2 = rand()) == 0) {}
		vars.add(Variable(name, Complex(BigDecimal(1.0 / rand1), BigDecimal(1.0 / rand2))));
	}
	COMPARE(vars.count(), size_t(10000));

	for (int i = 2500; i < 7500; ++i)
	{
#if !defined(_MSC_VER) && defined(WIN32)
        swprintf(stri, L"%d", i);
#else
        swprintf(stri, 6, L"%d", i);
#endif
        name = L"Variable#";
		name += stri;
		vars.remove(name);
	}
	COMPARE(vars.count(), size_t(5000));

	vars.removeAll();
	COMPARE(vars.count(), size_t(0));
}

void VariablesTest::iterators()
{
	Variables vars;
	wchar_t stri[6];

	for (int i = 0; i < 1000; ++i)
	{
#if !defined(_MSC_VER) && defined(WIN32)
        swprintf(stri, L"%d", i);
#else
        swprintf(stri, 6, L"%d", i);
#endif
        vars.add(stri, Complex(i, 1000-i));
	}

	Variables::const_iterator iter;
	int i;
	for (iter = vars.begin(); iter != vars.end(); ++iter)
	{
		wstringstream ss(iter->name.c_str());
		ss >> i;
		COMPARE_COMPLEX((*iter).value, Complex(i, 1000-i));
		COMPARE_COMPLEX(iter->value, Complex(i, 1000-i));
	}
}
