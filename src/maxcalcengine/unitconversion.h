
#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

// Local
#include "unicode.h"
#include "bigdecimal.h"

namespace MaxCalcEngine {

struct SimpleConversion
{
	tstring unit1;
	tstring unit2;
	BigDecimal multiplier;
};

struct ArbitraryConversion
{
	tstring unit1;
	tstring unit2;
	BigDecimal (*convert)(BigDecimal arg);
};

class UnitConversion
{
	static SimpleConversion simpleConversions_[];
	static ArbitraryConversion arbitraryConversions_[];

public:
	static BigDecimal convert(const BigDecimal arg, const tstring & unit1, const tstring & unit2);

	static BigDecimal ctof(BigDecimal arg)
	{
		return arg * 1.8 + 32;
	}
	static BigDecimal ctok(BigDecimal arg)
	{
		return arg + 273.15;
	}
	static BigDecimal ftoc(BigDecimal arg)
	{
		return (arg + 459.67) * 5.0 / 9.0 - 273.15;
	}
	static BigDecimal ftok(BigDecimal arg)
	{
		return (arg + 459.67) * 5.0 / 9.0;
	}
	static BigDecimal ktoc(BigDecimal arg)
	{
		return arg - 273.15;
	}
	static BigDecimal ktof(BigDecimal arg)
	{
		return (arg - 273.15) * 1.8 + 32;
	}
};

} // namespace MaxCalcEngine

#endif // UNITCONVERSION_H
