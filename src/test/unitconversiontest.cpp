
// Local
#include "unitconversiontest.h"
#include "utility.h"
// MaxCalcEngine
#include "unitconversion.h"
#include "exceptions.h"
// STL
#include <string>
#include <cstdlib>
#include <sstream>

using namespace MaxCalcEngine;
using namespace std;

void UnitConversionTest::convert()
{
	FAIL_TEST(UnitConversion::convert(0, _T(""), _T("")), "Unknown unit", UnknownUnitException);
	FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("")), "Unknown unit", UnknownUnitException);
	FAIL_TEST(UnitConversion::convert(0, _T(""), _T("cm")), "Unknown unit", UnknownUnitException);
	FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("m/s")), "Unknown conversion", UnknownUnitConversionException);
	FAIL_TEST(UnitConversion::convert(0, _T("q"), _T("m/s")), "Unknown unit", UnknownUnitException);
	FAIL_TEST(UnitConversion::convert(0, _T("m"), _T("m/sq")), "Unknown unit", UnknownUnitException);
	FAIL_TEST(UnitConversion::convert(0, _T("qwe"), _T("rty")), "Unknown unit", UnknownUnitException);
	COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("oz"), _T("g")), 0);
	COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("g"), _T("oz")), 0);
	COMPARE_BIGDECIMAL(UnitConversion::convert(0, _T("c"), _T("f")), 32);
	COMPARE_BIGDECIMAL_PRECISION(UnitConversion::convert(0, _T("f"), _T("c")), -17.78, 4);
}

void UnitConversionTest::iterators()
{
	int i = 0;
	for (const UnitConversion::UnitDef * cur = UnitConversion::units(); cur->unit != UnitConversion::NO_UNIT; ++cur, ++i)
	{
		VERIFY(isUnit(cur->name));
		VERIFY(cur->type != UnitConversion::NO_TYPE);
	}

	VERIFY(i == 28);
}

bool UnitConversionTest::isUnit(const tstring str)
{
	for (tstring::const_iterator i = str.begin(); i != str.end(); ++i)
	{
		if (!istalpha((const int)*i) && *i != _T('/'))
			return false;
	}
	return true;
}
