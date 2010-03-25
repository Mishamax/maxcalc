/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2010 Michael Maximov (michael.maximov@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *****************************************************************************/

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
    wstring name;
    int rand1, rand2;
    tstringstream ss;

    srand((unsigned)time(0));
    for (int i = 0; i < 10000; ++i) {
        tstringstream ss;
        ss << i;
        name = L"Variable#";
        name += ss.str();
        while ((rand1 = rand()) == 0) {
        }
        while ((rand2 = rand()) == 0) {
        }
        vars.add(Variable(name, Complex(BigDecimal(1.0 / rand1), BigDecimal(1.0 / rand2))));
    }
    COMPARE(vars.count(), size_t(10000));

    for (int i = 2500; i < 7500; ++i) {
        tstringstream ss;
        ss << i;
        name = L"Variable#";
        name += ss.str();
        vars.remove(name);
    }
    COMPARE(vars.count(), size_t(5000));

    vars.removeAll();
    COMPARE(vars.count(), size_t(0));
}

void VariablesTest::iterators()
{
    Variables vars;
    tstringstream ss;

    for (int i = 0; i < 1000; ++i) {
        tstringstream ss;
        ss << i;
        vars.add(ss.str(), Complex(i, 1000-i));
    }

    Variables::const_iterator iter;
    int i;
    for (iter = vars.begin(); iter != vars.end(); ++iter) {
        wstringstream ss(iter->name.c_str());
        ss >> i;
        COMPARE_COMPLEX((*iter).value, Complex(i, 1000-i));
        COMPARE_COMPLEX(iter->value, Complex(i, 1000-i));
    }
}
