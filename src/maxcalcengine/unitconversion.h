
#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

// STL
#include <map>

// Local
#include "unicode.h"
#include "bigdecimal.h"

namespace MaxCalcEngine {

class UnitConversion
{
	enum Unit {
		END = 0,
		// Length
		MIL, IN, FT, YD, MI, MICRON, MM, CM, M, KM,
		// Weight
		LB, OZ, G, KG,
		// Time
		MICROS, MS, S, MIN, H, D,
		// Speed
		MIpH, MpS, FTpH, KMpH, KNOT,
		// Temperature
		C, F, K };

	typedef std::map<tstring, Unit> UnitsMap;
	static UnitsMap units_;
	static bool unitsInitialized_;
	static bool initUnits();

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

	class const_iterator : public UnitsMap::const_iterator
	{
	public:
		/// Default constructor.
		const_iterator() : UnitsMap::const_iterator()
		{
		}

		/// Constructs const_iterator from std::map<tstring, Unit> iterator.
		const_iterator(const UnitsMap::const_iterator & iter) :
			UnitsMap::const_iterator(iter)
		{
		}

		/// Gets Variable associated with current value.
		const tstring & operator* ()
		{
			return (UnitsMap::const_iterator::operator*()).first;
		}

		/// Gets Variable associated with current value.
		const tstring * operator-> ()
		{
			return &((UnitsMap::const_iterator::operator*()).first);
		}
	};

	inline static const_iterator begin();
	inline static const_iterator end();
};

/*!
	Returns const_iterator pointing to the beginning of variables' map.
*/
inline UnitConversion::const_iterator UnitConversion::begin()
{
    return const_iterator(units_.begin());
}

/*!
	Returns const_iterator pointing to the next element after the end of
	variables' map.
*/
inline UnitConversion::const_iterator UnitConversion::end()
{
    return const_iterator(units_.end());
}

} // namespace MaxCalcEngine

#endif // UNITCONVERSION_H
