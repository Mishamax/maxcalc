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

// Local
#include "unitconversion.h"
#include "exceptions.h"

namespace MaxCalcEngine {

// All constants have 150 digits precision
const UnitConversion::SimpleConversion UnitConversion::simpleConversions_[] =
{
	//---------------------------------------------------------------------
	// Length
	// Mil
    { MIL,		MICRON,	"25.4" },
	// Inch
	{ IN,		MIL,	"1000" },
	{ IN,		MICRON,	"25400" },
	{ IN,		MM,		"25.4" },
	{ IN,		CM,		"2.54" },
	// Foot
	{ FT,		MIL,	"12000" },
	{ FT,		IN,		"12" },
	{ FT,		MICRON,	"304800" },
	{ FT,		MM,		"304.8" },
	{ FT,		CM,		"30.48" },
	// Yard
	{ YD,		MIL,	"36000" },
	{ YD,		IN,		"36" },
	{ YD,		FT,		"3" },
	{ YD,		MICRON,	"914400" },
	{ YD,		MM,		"914.4" },
	{ YD,		CM,		"91.44" },
	// Mile
	{ MI,		MIL,	"63360000" },
	{ MI,		IN,		"63360" },
	{ MI,		FT,		"5280" },
	{ MI,		YD,		"1760" },
	{ MI,		MICRON,	"1609344000" },
	{ MI,		MM,		"1609344" },
	{ MI,		CM,		"160934.4" },
	{ MI,		M,		"1609.344" },
	{ MI,		KM,		"1.609344" },
	// Millimeter
	{ MM,		MIL,	"39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e-3" },
	{ MM,		MICRON,	"1000" },
	// Cantimeter
	{ CM,		MIL,	"39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e-2" },
	{ CM,		MICRON,	"10000" },
	{ CM,		MM,		"10" },
	// Meter
	{ M,		MIL,	"39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
	{ M,		IN,		"39.370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
	{ M,		FT,		"3.2808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934" },
	{ M,		YD,		"1.0936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645" },
	{ M,		MICRON,	"1000000" },
	{ M,		MM,		"1000" },
	{ M,		CM,		"100" },
	// Kilometer
	{ KM,		MIL,	"39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e+3" },
	{ KM,		IN,		"39.370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e+3" },
	{ KM,		FT,		"3.2808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934e+3" },
	{ KM,		YD,		"1.0936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645e+3" },
	{ KM,		MICRON,	"1000000000" },
	{ KM,		MM,		"1000000" },
	{ KM,		CM,		"100000" },
	{ KM,		M,		"1000" },
	//---------------------------------------------------------------------
	// Weight
	// Pound
	{ LB,		OZ,		"16.000002821917267029565227206668754885444268544934799601545281895425389918418371810175269281455206296261806196365934943519325900403251977458524870967832965" },
	{ LB,		G,		"453.5924" },
	// Ounce
	{ OZ,		G,		"28.34952" },
	// Kilogramme
	{ KG,		LB,		"2.2046224760379583079434311509628468201848179114112141208715137202475173746297336551494249021809007381957898765499598317784865883996292706844294569309362326" },
	{ KG,		OZ,		"35.273965837869565340083359436068053356811684995019316023692817373980229647627190866018190078703272577454574186793991573755040649718231560887097912063414125" },
	{ KG,		G,		"1000" },
	//---------------------------------------------------------------------
	// Time
	// Millisecond
	{ MS,		MICROS,	"1000" },
	// Second
	{ S,		MICROS,	"1000000" },
	{ S,		MS,		"1000" },
	// Minute
	{ MIN,		MICROS,	"60000000" },
	{ MIN,		MS,		"60000" },
	{ MIN,		S,		"60" },
	// Hour
	{ H,		MICROS,	"3600000000" },
	{ H,		MS,		"3600000" },
	{ H,		S,		"3600" },
	{ H,		MIN,	"60" },
	// Day
	{ D,		MICROS,	"86400000000" },
	{ D,		MS,		"86400000" },
	{ D,		S,		"86400" },
	{ D,		MIN,	"1440" },
	{ D,		H,		"24" },
	//---------------------------------------------------------------------
	// Speed
	// mi/hour
	{ MIpH,		FTpH,	"1760" },
	{ MIpH,		KMpH,	"1.609344" },
	// Meter/second
	{ MpS,		MIpH,	"2.2369362920544022906227630637079455977093772369362920544022906227630637079455977093772369362920544022906227630637079455977093772369362920544022906227630637" },
	{ MpS,		FTpH,	"3937.0078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
	{ MpS,		KMpH,	"3.6" },
	{ MpS,		KNOT,	"1.9438444924406047516198704103671706263498920086393088552915766738660907127429805615550755939524838012958963282937365010799136069114470842332613390928725702" },
	// Kilometre/hour
	{ KMpH,		FTpH,	"1093.6132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645" },
	// Knot
	{ KNOT,		MIpH,	"1.1507794480235425117314881094408653463771574007794480235425117314881094408653463771574007794480235425117314881094408653463771574007794480235425117314881094" },
	{ KNOT,		FTpH,	"2025.3718285214348206474190726159230096237970253718285214348206474190726159230096237970253718285214348206474190726159230096237970253718285214348206474190726" },
	{ KNOT,		KMpH,	"1.852" },
	{ NO_UNIT,	NO_UNIT,	0 }
};

const UnitConversion::ArbitraryConversion UnitConversion::arbitraryConversions_[] =
{
	{ C,		F,		UnitConversion::ctof },
	{ C,		K,		UnitConversion::ctok },
	{ F,		C,		UnitConversion::ftoc },
	{ F,		K,		UnitConversion::ftok },
	{ K,		C,		UnitConversion::ktoc },
	{ K,		F,		UnitConversion::ktof },
	{ NO_UNIT,	NO_UNIT,	0 }
};

const UnitConversion::UnitDef UnitConversion::units_[] =
{
	// Length
	{ _T("mil"),	MIL,		LENGTH },
	{ _T("in"),		IN,			LENGTH },
	{ _T("ft"),		FT,			LENGTH },
	{ _T("yd"),		YD,			LENGTH },
	{ _T("mi"),		MI,			LENGTH },
	{ _T("micron"),	MICRON,		LENGTH },
	{ _T("mm"),		MM,			LENGTH },
	{ _T("cm"),		CM,			LENGTH },
	{ _T("m"),		M,			LENGTH },
	{ _T("km"),		KM,			LENGTH },

	// Weight
	{ _T("lb"),		LB,			WEIGHT },
	{ _T("oz"),		OZ,			WEIGHT },
	{ _T("g"),		G,			WEIGHT },
	{ _T("kg"),		KG,			WEIGHT },

	// Time
	{ _T("micros"),	MICROS,		TIME },
	{ _T("ms"),		MS,			TIME },
	{ _T("s"),		S,			TIME },
	{ _T("min"),	MIN,		TIME },
	{ _T("h"),		H,			TIME },
	{ _T("d"),		D,			TIME },

	// Speed
	{ _T("mi/h"),	MIpH,		SPEED },
	{ _T("m/s"),	MpS,		SPEED },
	{ _T("ft/h"),	FTpH,		SPEED },
	{ _T("km/h"),	KMpH,		SPEED },
	{ _T("knot"),	KNOT,		SPEED },

	// Temperature
	{ _T("k"),		K,			TEMPERATURE },
	{ _T("c"),		C,			TEMPERATURE },
	{ _T("f"),		F,			TEMPERATURE },

	{ _T(""),		NO_UNIT,	NO_TYPE }
};

/*!
	Converts \a arg from \a unit1 to \a unit2.
*/
BigDecimal UnitConversion::convert(const BigDecimal arg, const tstring & unit1,
								   const tstring & unit2)
{
	Unit u1 = NO_UNIT;
	Unit u2 = NO_UNIT;

	for (const UnitDef * cur = units_;
		 cur->unit != NO_UNIT && (u1 == NO_UNIT || u2 == NO_UNIT);
		 ++cur)
	{
		if (unit1 == cur->name)
			u1 = cur->unit;
		if (unit2 == cur->name)
			u2 = cur->unit;
	}

	if (u1 == NO_UNIT || u2 == NO_UNIT)
		throw UnknownUnitConversionException();

	// Look up simple conversions table
	for (const SimpleConversion * cur = simpleConversions_;
		 cur->unit1 != NO_UNIT;
		 ++cur)
	{
		if (u1 == cur->unit1 && u2 == cur->unit2)
			return arg * cur->multiplier;
		if (u1 == cur->unit2 && u2 == cur->unit1)
			return arg / cur->multiplier;
	}

	// Look up arbitrary conversions table
	for (const ArbitraryConversion * cur = arbitraryConversions_;
		 cur->unit1 != NO_UNIT;
		 cur++)
	{
		if (u1 == cur->unit1 && u2 == cur->unit2)
			return cur->convert(arg);
	}

	throw UnknownUnitConversionException();
}

} // namespace MaxCalcEngine