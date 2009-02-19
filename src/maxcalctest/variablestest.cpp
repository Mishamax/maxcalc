
// Local
#include "variablestest.h"
#include "utility.h"
// MaxCalcEngine
#include "variables.h"
// STL
#include <string>
#include <cstdlib>
#include <sstream>

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

	for (int i = 0; i < 10000; ++i)
	{
		swprintf(stri, 6, L"%d", i);
		name = L"Variable#";
		name += stri;
		vars.add(Variable(name, Complex(1 / rand(), 1 / rand())));
	}
	COMPARE(vars.count(), size_t(10000));

	for (int i = 2500; i < 7500; ++i)
	{
		swprintf(stri, 6, L"%d", i);
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
		swprintf(stri, 6, L"%d", i);
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
