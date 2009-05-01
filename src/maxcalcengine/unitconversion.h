
#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

// Local
#include "unicode.h"
#include "bigdecimal.h"

namespace MaxCalcEngine {

class UnitConversion
{
public:
	enum Unit
	{
		NO_UNIT,
		// Length
		MIL, IN, FT, YD, MI, MICRON, MM, CM, M, KM,
		// Weight
		LB, OZ, G, KG,
		// Time
		MICROS, MS, S, MIN, H, D,
		// Speed
		MIpH, MpS, FTpH, KMpH, KNOT,
		// Temperature
		C, F, K
	};

	enum Type
	{
		NO_TYPE, LENGTH, WEIGHT, TIME, SPEED, TEMPERATURE
	};

	struct UnitDef
	{
		tstring name;
		Unit unit;
		Type type;
	};

private:
	static const UnitDef units_[];

	struct SimpleConversion
	{
		const Unit unit1;
		const Unit unit2;
		const BigDecimal multiplier;
	};

	struct ArbitraryConversion
	{
		const Unit unit1;
		const Unit unit2;
		BigDecimal (*convert)(BigDecimal arg);
	};

	static const SimpleConversion simpleConversions_[];
	static const ArbitraryConversion arbitraryConversions_[];

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

public:
	static BigDecimal convert(const BigDecimal arg, const tstring & unit1, const tstring & unit2);
	inline static const UnitDef * units();
};

inline const UnitConversion::UnitDef * UnitConversion::units()
{
	return units_;
}

} // namespace MaxCalcEngine

#endif // UNITCONVERSION_H
