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
    COMPARE_BIGDECIMAL(UnitConversion::convert(10, _T("m"), _T("m")), 10);
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

// Units: mil in ft yd mi micron mm cm m km
void UnitConversionTest::length()
{
    // mil
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("in")), "0.001");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mil"), _T("ft")), "0.0833333333333333e-3", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mil"), _T("yd")), "0.0277777777777778e-3", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mil"), _T("mile")), "1.578282828282828e-8", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("micron")), "25400e-3");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("mm")), "25.4e-3");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("cm")), "2.54e-3");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("m")), "0.0254e-3");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mil"), _T("km")), "0.0000254e-3");
    // in
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("mil")), "1000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("in"), _T("ft")), "0.0833333333333333", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("in"), _T("yd")), "0.0277777777777778", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("in"), _T("mile")), "1.578282828282828e-5", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("micron")), "25400");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("mm")), "25.4");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("cm")), "2.54");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("m")), "0.0254");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("in"), _T("km")), "0.0000254");
    // ft
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("in")), "12");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("mil")), "12000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ft"), _T("yd")), "0.3333333333333333", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ft"), _T("mile")), "1.893939393939394e-4", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("micron")), "304800");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("mm")), "304.8");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("cm")), "30.48");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("m")), "0.3048");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft"), _T("km")), "0.0003048");
    // yd
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("in")), "36");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("ft")), "3");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("mil")), "36000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("yd"), _T("mile")), "5.681818181818182e-4", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("micron")), "914400");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("mm")), "914.4");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("cm")), "91.44");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("m")), "0.9144");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("yd"), _T("km")), "0.0009144");
    // mi
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("in")), "63360");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("ft")), "5280");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("yd")), "1760");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("mil")), "63360000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("micron")), "1609344000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("mm")), "1609344");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("cm")), "160934.4");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("m")), "1609.344");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile"), _T("km")), "1.609344");
    // micron
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micron"), _T("in")), "3.937007874015748e-5", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micron"), _T("ft")), "3.280839895013123e-6", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micron"), _T("yd")), "1.093613298337708e-6", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micron"), _T("mile")), "6.21371192237334e-10", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micron"), _T("mil")), "3.937007874015748e-2", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micron"), _T("mm")), "0.001");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micron"), _T("cm")), "0.0001");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micron"), _T("m")), "0.000001");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micron"), _T("km")), "0.000000001");
    // mm
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mm"), _T("in")), "0.0393700787401575", 15);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mm"), _T("ft")), "0.0032808398950131", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mm"), _T("yd")), "0.0010936132983377", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mm"), _T("mile")), "6.21371192237334e-7", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mm"), _T("micron")), "1000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mm"), _T("mil")), "0.0393700787401575e+3", 15);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mm"), _T("cm")), "0.1");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mm"), _T("m")), "0.001");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mm"), _T("km")), "0.000001");
    // cm
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("cm"), _T("in")), "0.3937007874015748", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("cm"), _T("ft")), "0.0328083989501312", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("cm"), _T("yd")), "0.0109361329833771", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("cm"), _T("mile")), "6.21371192237334e-6", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("cm"), _T("micron")), "10000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("cm"), _T("mm")), "10");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("cm"), _T("mil")), "0.3937007874015748e+3", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("cm"), _T("m")), "0.01");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("cm"), _T("km")), "0.00001");
    // m
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m"), _T("in")), "39.37007874015748", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m"), _T("ft")), "3.280839895013123", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m"), _T("yd")), "1.093613298337708", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m"), _T("mile")), "6.21371192237334e-4", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("m"), _T("micron")), "1000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("m"), _T("mm")), "1000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("m"), _T("cm")), "100");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m"), _T("mil")), "39.37007874015748e+3", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("m"), _T("km")), "0.001");
    // km
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km"), _T("in")), "39370.07874015748", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km"), _T("ft")), "3280.839895013123", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km"), _T("yd")), "1093.613298337708", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km"), _T("mile")), "0.621371192237334", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("km"), _T("micron")), "1000000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("km"), _T("mm")), "1000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("km"), _T("cm")), "100000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("km"), _T("m")), "1000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km"), _T("mil")), "39370.07874015748e+3", 14);
}

// Units: oz lb g kg
void UnitConversionTest::weight()
{
    // oz
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("oz"), _T("lb")), "0.0625");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("oz"), _T("g")), "28.349523125");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("oz"), _T("kg")), "0.028349523125");
    // lb
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("lb"), _T("oz")), "16");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("lb"), _T("g")), "453.59237");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("lb"), _T("kg")), "0.45359237");
    // g
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("g"), _T("oz")), "0.0352739619495804", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("g"), _T("lb")), "0.0022046226218488", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("g"), _T("kg")), "0.001");
    // kg
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("kg"), _T("oz")), "35.27396194958041", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("kg"), _T("lb")), "2.204622621848776", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("kg"), _T("g")), "1000");
}

// Units: micros ms s min h d
void UnitConversionTest::time()
{
    // micros
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micros"), _T("ms")), "0.001");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("micros"), _T("s")), "0.000001");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micros"), _T("min")), "1.666666666666667e-8", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micros"), _T("h")), "2.777777777777778e-10", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("micros"), _T("d")), "1.157407407407407e-11", 14);
    // ms
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ms"), _T("micros")), "1000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ms"), _T("s")), "0.001");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ms"), _T("min")), "1.666666666666667e-5", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ms"), _T("h")), "2.777777777777778e-7", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ms"), _T("d")), "1.157407407407407e-8", 14);
    // s
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("s"), _T("micros")), "1000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("s"), _T("ms")), "1000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("s"), _T("min")), "0.0166666666666667", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("s"), _T("h")), "2.777777777777778e-4", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("s"), _T("d")), "1.157407407407407e-5", 14);
    // min
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("min"), _T("ms")), "60000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("min"), _T("s")), "60");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("min"), _T("micros")), "60000000");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("min"), _T("h")), "0.0166666666666667", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("min"), _T("d")), "6.944444444444444e-4", 14);
    // h
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("h"), _T("ms")), "3600000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("h"), _T("s")), "3600");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("h"), _T("micros")), "3600000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("h"), _T("min")), "60");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("h"), _T("d")), "0.0416666666666667", 14);
    // d
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("d"), _T("ms")), "86400000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("d"), _T("s")), "86400");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("d"), _T("micros")), "86400000000");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("d"), _T("min")), "1440");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("d"), _T("h")), "24");
}

// Units: mile/h m/s ft/h km/h knot
void UnitConversionTest::speed()
{
    // mile/h
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile/h"), _T("m/s")), "0.44704");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile/h"), _T("ft/h")), "1760");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("mile/h"), _T("km/h")), "1.609344");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("mile/h"), _T("knot")), "0.8689762419006479", 14);
    // m/s
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m/s"), _T("mile/h")), "2.236936292054402", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m/s"), _T("ft/h")), "3937.007874015748", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("m/s"), _T("km/h")), "3.6");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("m/s"), _T("knot")), "1.943844492440605", 14);
    // ft/h
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft/h"), _T("m/s")), "0.000254");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ft/h"), _T("mile/h")), "5.68181818181818e-4", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("ft/h"), _T("km/h")), "0.0009144");
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("ft/h"), _T("knot")), "4.937365010799136e-4", 14);
    // km/h
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km/h"), _T("mile/h")), "0.621371192237334", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km/h"), _T("ft/h")), "1093.6132983377078", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km/h"), _T("m/s")), "0.2777777777777778", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("km/h"), _T("knot")), "0.5399568034557235", 14);
    // knot
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("knot"), _T("mile/h")), "1.150779448023543", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("knot"), _T("ft/h")), "2025.37182852143482", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("knot"), _T("m/s")), "0.5144444444444444", 14);
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("knot"), _T("km/h")), "1.852");
}

// Units: k, c, f
void UnitConversionTest::temperature()
{
    // k
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("k"), _T("c")), "-272.15");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("k"), _T("f")), "-457.87");
    // c
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("c"), _T("k")), "274.15");
    COMPARE_BIGDECIMAL(UnitConversion::convert(1, _T("c"), _T("f")), "33.8");
    // f
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("f"), _T("c")), "-17.22222222222222", 14);
    COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(1, _T("f"), _T("k")), "255.9277777777778", 14);
}
