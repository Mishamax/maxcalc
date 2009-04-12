
// Local
#include "unitconversion.h"
#include "exceptions.h"

namespace MaxCalcEngine {

// TODO: improve conversions precision
SimpleConversion UnitConversion::simpleConversions_[] =
{
	//---------------------------------------------------------------------
	// Length
	// Mil
	_T("mil"),		_T("in"),		0.001,
	_T("mil"),		_T("ft"),		0.00008333333333333333333,
	_T("mil"),		_T("yd"),		0.00002777777777777777778,
	_T("mil"),		_T("mile"),		1.57828282828282828283e-8,
	_T("mil"),		_T("micron"),	25.4,
	_T("mil"),		_T("mm"),		0.0254,
	_T("mil"),		_T("cm"),		0.00254,
	_T("mil"),		_T("m"),		0.0000254,
	_T("mil"),		_T("km"),		0.0000000254,
	// Inch
	_T("in"),		_T("mil"),		1000,
	_T("in"),		_T("ft"),		0.08333333333333333333,
	_T("in"),		_T("yd"),		0.02777777777777777778,
	_T("in"),		_T("mile"),		1.57828282828282828283e-5,
	_T("in"),		_T("micron"),	25400,
	_T("in"),		_T("mm"),		25.4,
	_T("in"),		_T("cm"),		2.54,
	_T("in"),		_T("m"),		0.0254,
	_T("in"),		_T("km"),		0.0000254,
	// Foot
	_T("ft"),		_T("mil"),		12000,
	_T("ft"),		_T("in"),		12,
	_T("ft"),		_T("yd"),		0.33333333333333333333,
	_T("ft"),		_T("mile"),		1.89393939393939393939e-4,
	_T("ft"),		_T("micron"),	304800,
	_T("ft"),		_T("mm"),		304.8,
	_T("ft"),		_T("cm"),		30.48,
	_T("ft"),		_T("m"),		0.3048,
	_T("ft"),		_T("km"),		0.0003048,
	// Yard
	_T("yd"),		_T("mil"),		36000,
	_T("yd"),		_T("in"),		36,
	_T("yd"),		_T("ft"),		3,
	_T("yd"),		_T("mile"),		5.68181818181818181818e-4,
	_T("yd"),		_T("micron"),	914400,
	_T("yd"),		_T("mm"),		914.4,
	_T("yd"),		_T("cm"),		91.44,
	_T("yd"),		_T("m"),		0.9144,
	_T("yd"),		_T("km"),		0.0009144,
	// Mile
	_T("mile"),		_T("mil"),		63360000,
	_T("mile"),		_T("in"),		63360,
	_T("mile"),		_T("ft"),		5280,
	_T("mile"),		_T("yd"),		1760,
	_T("mile"),		_T("micron"),	1609344000,
	_T("mile"),		_T("mm"),		1609344,
	_T("mile"),		_T("cm"),		160934.4,
	_T("mile"),		_T("m"),		1609.344,
	_T("mile"),		_T("km"),		1.609344,
	// Micrometer
	_T("micron"),	_T("mil"),		0.03937007874015748031,
	_T("micron"),	_T("in"),		0.00003937007874015748031,
	_T("micron"),	_T("ft"),		0.00000328083989501312336,
	_T("micron"),	_T("yd"),		0.00000109361329833770779,
	_T("micron"),	_T("mile"),		6.21371192237333969617e-10,
	_T("micron"),	_T("mm"),		0.001,
	_T("micron"),	_T("cm"),		0.0001,
	_T("micron"),	_T("m"),		0.000001,
	_T("micron"),	_T("km"),		0.000000001,
	// Millimeter
	_T("mm"),		_T("mil"),		39.37007874015748031,
	_T("mm"),		_T("in"),		0.03937007874015748031,
	_T("mm"),		_T("ft"),		0.00328083989501312336,
	_T("mm"),		_T("yd"),		0.00109361329833770779,
	_T("mm"),		_T("mile"),		6.21371192237333969617e-7,
	_T("mm"),		_T("micron"),	1000,
	_T("mm"),		_T("cm"),		0.1,
	_T("mm"),		_T("m"),		0.001,
	_T("mm"),		_T("km"),		0.000001,
	// Cantimeter
	_T("cm"),		_T("mil"),		393.70078740157480315,
	_T("cm"),		_T("in"),		0.39370078740157480315,
	_T("cm"),		_T("ft"),		0.0328083989501312336,
	_T("cm"),		_T("yd"),		0.01093613298337707787,
	_T("cm"),		_T("mile"),		6.21371192237333969617e-6,
	_T("cm"),		_T("micron"),	10000,
	_T("cm"),		_T("mm"),		10,
	_T("cm"),		_T("m"),		0.01,
	_T("cm"),		_T("km"),		0.00001,
	// Meter
	_T("m"),		_T("mil"),		39370.07874015748031496,
	_T("m"),		_T("in"),		39.37007874015748031496,
	_T("m"),		_T("ft"),		3.28083989501312335958,
	_T("m"),		_T("yd"),		1.09361329833770778653,
	_T("m"),		_T("mile"),		6.21371192237333969617e-4,
	_T("m"),		_T("micron"),	1000000,
	_T("m"),		_T("mm"),		1000,
	_T("m"),		_T("cm"),		100,
	_T("m"),		_T("km"),		0.001,
	// Kilometre
	_T("km"),		_T("mil"),		39370078.74015748031496063,
	_T("km"),		_T("in"),		39370.07874015748031496063,
	_T("km"),		_T("ft"),		3280.83989501312335958005,
	_T("km"),		_T("yd"),		1093.61329833770778652668,
	_T("km"),		_T("mile"),		0.62137119223733396962,
	_T("km"),		_T("micron"),	1000000000,
	_T("km"),		_T("mm"),		1000000,
	_T("km"),		_T("cm"),		100000,
	_T("km"),		_T("m"),		1000,
	//---------------------------------------------------------------------
	// Weight
	// Pound
	_T("lb"),		_T("oz"),		16.00000282191726702957,
	_T("lb"),		_T("g"),		453.5924,
	_T("lb"),		_T("kg"),		0.4535924,
	// Ounce
	_T("oz"),		_T("lb"),		0.06249998897688761981,
	_T("oz"),		_T("g"),		28.34952,
	_T("oz"),		_T("kg"),		0.02834952,
	// Gramme
	_T("g"),		_T("lb"),		0.00220462247603795831,
	_T("g"),		_T("oz"),		0.03527396583786956534,
	_T("g"),		_T("kg"),		0.001,
	// Kilogramme
	_T("kg"),		_T("lb"),		2.20462247603795830794,
	_T("kg"),		_T("oz"),		35.27396583786956534008,
	_T("kg"),		_T("g"),		1000,
	//---------------------------------------------------------------------
	// Time
	// Microsecond
	_T("micros"),	_T("ms"),		0.001,
	_T("micros"),	_T("s"),		0.000001,
	_T("micros"),	_T("min"),		0.00000001666666666666666667,
	_T("micros"),	_T("h"),		2.77777777777777777778e-10,
	_T("micros"),	_T("d"),		1.15740740740740740741e-11,
	// Millisecond
	_T("ms"),		_T("micros"),	1000,
	_T("ms"),		_T("s"),		0.001,
	_T("ms"),		_T("min"),		0.00001666666666666666667,
	_T("ms"),		_T("h"),		2.77777777777777777778e-7,
	_T("ms"),		_T("d"),		1.15740740740740740741e-8,
	// Second
	_T("s"),		_T("micros"),	1000000,
	_T("s"),		_T("ms"),		1000,
	_T("s"),		_T("min"),		0.01666666666666666667,
	_T("s"),		_T("h"),		2.77777777777777777778e-4,
	_T("s"),		_T("d"),		1.15740740740740740741e-5,
	// Minute
	_T("min"),		_T("miros"),	60000000,
	_T("min"),		_T("ms"),		60000,
	_T("min"),		_T("s"),		60,
	_T("min"),		_T("h"),		0.01666666666666666667,
	_T("min"),		_T("d"),		6.94444444444444444444e-4,
	// Hour
	_T("h"),		_T("micros"),	3600000000.0,
	_T("h"),		_T("ms"),		3600000,
	_T("h"),		_T("s"),		3600,
	_T("h"),		_T("min"),		60,
	_T("h"),		_T("d"),		0.04166666666666666667,
	// Day
	_T("d"),		_T("micros"),	86400000000.0,
	_T("d"),		_T("ms"),		86400000,
	_T("d"),		_T("sec"),		86400,
	_T("d"),		_T("min"),		1440,
	_T("d"),		_T("h"),		24,
	//---------------------------------------------------------------------
	// Speed
	// Mile/hour
	_T("mile/h"),	_T("m/s"),		0.44704,
	_T("mile/h"),	_T("ft/h"),		1760,
	_T("mile/h"),	_T("km/h"),		1.609344,
	_T("mile/h"),	_T("knot"),		0.86897624190064794816,
	// Meter/second
	_T("m/s"),		_T("mile/h"),	2.23693629205440229062,
	_T("m/s"),		_T("ft/h"),		3937.00787401574803149606,
	_T("m/s"),		_T("km/h"),		3.6,
	_T("m/s"),		_T("knot"),		1.94384449244060475162,
	// Foot/hour
	_T("ft/h"),		_T("mile/h"),	5.68181818181818181818e-4,
	_T("ft/h"),		_T("m/s"),		0.000254,
	_T("ft/h"),		_T("km/h"),		0.0009144,
	_T("ft/h"),		_T("knot"),		4.93736501079913606911e-4,
	// Kilometre/hour
	_T("km/h"),		_T("mile/h"),	0.62137119223733396962,
	_T("km/h"),		_T("m/s"),		0.27777777777777777778,
	_T("km/h"),		_T("ft/h"),		1093.61329833770778652668,
	_T("km/h"),		_T("knot"),		0.53995680345572354212,
	// Knot
	_T("knot"),		_T("mile/h"),	1.15077944802354251173,
	_T("knot"),		_T("m/s"),		0.51444444444444444444,
	_T("knot"),		_T("ft/h"),		2025.37182852143482064742,
	_T("knot"),		_T("km/h"),		1.852,
	{ _T(""), _T(""), 0 }
};

ArbitraryConversion UnitConversion::arbitraryConversions_[] =
{
	{ _T("c"), _T("f"), UnitConversion::ctof },
	{ _T("c"), _T("k"), UnitConversion::ctok },
	{ _T("f"), _T("c"), UnitConversion::ftoc },
	{ _T("f"), _T("k"), UnitConversion::ftok },
	{ _T("k"), _T("c"), UnitConversion::ktoc },
	{ _T("k"), _T("f"), UnitConversion::ktof },
	{ _T(""), _T(""), 0 }
};

BigDecimal UnitConversion::convert(const BigDecimal arg, const tstring & unit1, const tstring & unit2)
{
	// Look up simple conversions table
	for (SimpleConversion * cur = simpleConversions_; cur->unit1.length() != 0; cur++)
	{
		if (unit1 == cur->unit1 && unit2 == cur->unit2)
			return arg * cur->multiplier;
	}

	// Look up arbitrary conversions table
	for (ArbitraryConversion * cur = arbitraryConversions_; cur->unit1.length() != 0; cur++)
	{
		if (unit1 == cur->unit1 && unit2 == cur->unit2)
			return cur->convert(arg);
	}

	throw UnknownUnitConversionException();
}

} // namespace MaxCalcEngine
