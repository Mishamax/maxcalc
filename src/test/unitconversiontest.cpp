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
#include "unitconversiontest.h"
#include "utility.h"
// MaxCalcEngine
#include "unitconversion.h"
#include "exceptions.h"
// STL
#include <string>
#include <cstdlib>

using namespace std;

void UnitConversionTest::convert()
{
    FAIL_TEST(UnitConversion::convert(0, _T(""), _T("")), "Unknown unit", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("")), "Unknown unit", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T(""), _T("cm")), "Unknown unit", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("m/s")), "Unknown conversion", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T("q"), _T("m/s")), "Unknown unit", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("m/sq")), "Unknown unit", ParserException);
    FAIL_TEST(UnitConversion::convert(0, _T("qwe"), _T("rty")), "Unknown unit", ParserException);
    COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("oz"), _T("g")), 0);
    COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("g"), _T("oz")), 0);
    COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("c"), _T("f")), 32);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(0, _T("f"), _T("c")), -17.78, 4);
}

void UnitConversionTest::iterators()
{
    int i = 0;
    for (const UnitConversion::UnitDef * cur = UnitConversion::units();
         cur->unit != UnitConversion::NO_UNIT;
         ++cur, ++i) {
        VERIFY(isUnit(cur->name));
        VERIFY(cur->type != UnitConversion::NO_TYPE);
    }

    VERIFY(i == 28);
}

bool UnitConversionTest::isUnit(const tstring str)
{
    for (tstring::const_iterator i = str.begin(); i != str.end(); ++i) {
        if (!istalpha((const int)*i) && *i != _T('/')) {
            return false;
        }
    }
    return true;
}
