/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2009 Michael Maximov (michael.maximov@gmail.com)
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

#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

// Local
#include "unicode.h"
#include "bigdecimal.h"

// Workaround for Windows CE, where IN is defined in windef.h
#ifdef IN
#undef IN
#endif

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
	static BigDecimal convert(const BigDecimal arg, const tstring & unit1,
							  const tstring & unit2);
	inline static const UnitDef * units();
};

inline const UnitConversion::UnitDef * UnitConversion::units()
{
	return units_;
}

} // namespace MaxCalcEngine

#endif // UNITCONVERSION_H
