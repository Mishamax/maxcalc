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
#include "unitconversion.h"
#include "exceptions.h"


/*!
    \class UnitConversion
    \brief Performs unit conversions.

    Unit conversion of a number from unit1 to unit2 is performed using
    convert(number, unit1, unit2) function.

    This class also stores all unit conversion tables.
    List of units can be retrieved using units() function.

    \ingroup MaxCalcEngine
*/

/*!
    Simple unit conversion which can be performed by multiplying / dividing
    of number by a constant.
    Here are only conversions with constant > 1. Conversions in forward
    direction are performed by multiplying by this constant and in backward
    direction - by dividing by this constant.
    All constants have 150 digits precision.
*/
const UnitConversion::SimpleConversion UnitConversion::mSimpleConversions[] =
{
    //---------------------------------------------------------------------
    // Angle
    // Radian
    { RADIAN,       DEGREE,         "57.2957795130823208767981548141051703324054724665643215491602438612028471483215526324409689958511109441862233816328648932814482646012483150360682678" },
    { RADIAN,       GRADIAN,        "63.6619772367581343075535053490057448137838582961825794990669376235587190536906140360455211065012343824291370907031832147571647384458314611511869642" },
    // Gradian
    { GRADIAN,      DEGREE,         "0.9" },

    //---------------------------------------------------------------------
    // Length
    // Mil
    { MIL,          MICRON,         "25.4" },
    // Inch
    { INCH,         MIL,            "1000" },
    { INCH,         MICRON,         "25400" },
    { INCH,         MILLIMETER,     "25.4" },
    { INCH,         CENTIMETER,     "2.54" },
    // Foot
    { FOOT,         MIL,            "12000" },
    { FOOT,         INCH,           "12" },
    { FOOT,         MICRON,         "304800" },
    { FOOT,         MILLIMETER,     "304.8" },
    { FOOT,         CENTIMETER,     "30.48" },
    // Yard
    { YARD,         MIL,            "36000" },
    { YARD,         INCH,           "36" },
    { YARD,         FOOT,           "3" },
    { YARD,         MICRON,         "914400" },
    { YARD,         MILLIMETER,     "914.4" },
    { YARD,         CENTIMETER,     "91.44" },
    // Mile
    { MILE,         MIL,            "63360000" },
    { MILE,         INCH,           "63360" },
    { MILE,         FOOT,           "5280" },
    { MILE,         YARD,           "1760" },
    { MILE,         MICRON,         "1609344000" },
    { MILE,         MILLIMETER,     "1609344" },
    { MILE,         CENTIMETER,     "160934.4" },
    { MILE,         METER,          "1609.344" },
    { MILE,         KILOMETER,      "1.609344" },
    // Millimeter
    { MILLIMETER,   MIL,            "39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e-3" },
    { MILLIMETER,   MICRON,         "1000" },
    // Cantimeter
    { CENTIMETER,   MIL,            "39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e-2" },
    { CENTIMETER,   MICRON,         "10000" },
    { CENTIMETER,   MILLIMETER,     "10" },
    // Meter
    { METER,        MIL,            "39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
    { METER,        INCH,           "39.370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
    { METER,        FOOT,           "3.2808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934" },
    { METER,        YARD,           "1.0936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645" },
    { METER,        MICRON,         "1000000" },
    { METER,        MILLIMETER,     "1000" },
    { METER,        CENTIMETER,     "100" },
    // Kilometer
    { KILOMETER,    MIL,            "39370.078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e+3" },
    { KILOMETER,    INCH,           "39.370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921e+3" },
    { KILOMETER,    FOOT,           "3.2808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934383202099737532808398950131233595800524934e+3" },
    { KILOMETER,    YARD,           "1.0936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645e+3" },
    { KILOMETER,    MICRON,         "1000000000" },
    { KILOMETER,    MILLIMETER,     "1000000" },
    { KILOMETER,    CENTIMETER,     "100000" },
    { KILOMETER,    METER,          "1000" },

    //---------------------------------------------------------------------
    // Mass
    // Pound
    { POUND,        OUNCE,          "16" },
    { POUND,        GRAM,           "453.59237" },
    // Ounce
    { OUNCE,        GRAM,           "28.349523125" },
    // Kilogramme
    { KILOGRAM,     POUND,          "2.2046226218487758072297380134502703385420702733601978357792923192248582135541653842193156820517064694011497591989918172565380674282506118874971375731033" },
    { KILOGRAM,     OUNCE,          "35.2739619495804129156758082152043254166731243737631653724686771075977314168666461475090509128273035104183961471838690761046090788520097901999542011696" },
    { KILOGRAM,     GRAM,           "1000" },

    //---------------------------------------------------------------------
    // Time
    // Millisecond
    { MILLISECOND,  MICROSECOND,    "1000" },
    // Second
    { SECOND,       MICROSECOND,    "1000000" },
    { SECOND,       MILLISECOND,    "1000" },
    // Minute
    { MINUTE,       MICROSECOND,    "60000000" },
    { MINUTE,       MILLISECOND,    "60000" },
    { MINUTE,       SECOND,         "60" },
    // Hour
    { HOUR,         MICROSECOND,    "3600000000" },
    { HOUR,         MILLISECOND,    "3600000" },
    { HOUR,         SECOND,         "3600" },
    { HOUR,         MINUTE,         "60" },
    // Day
    { DAY,          MICROSECOND,    "86400000000" },
    { DAY,          MILLISECOND,    "86400000" },
    { DAY,          SECOND,         "86400" },
    { DAY,          MINUTE,         "1440" },
    { DAY,          HOUR,           "24" },
    // Week
    { WEEK,         MICROSECOND,    "604800000000" },
    { WEEK,         MILLISECOND,    "604800000" },
    { WEEK,         SECOND,         "604800" },
    { WEEK,         MINUTE,         "10080" },
    { WEEK,         HOUR,           "168" },
    { WEEK,         DAY,            "7" },

    //---------------------------------------------------------------------
    // Velocity
    // mi/hour
    { MILE_PER_HOUR,        FOOT_PER_HOUR,      "1760" },
    { MILE_PER_HOUR,        KILOMETER_PER_HOUR, "1.609344" },
    // Meter/second
    { METER_PER_SECOND,     MILE_PER_HOUR,      "2.2369362920544022906227630637079455977093772369362920544022906227630637079455977093772369362920544022906227630637079455977093772369362920544022906227630637" },
    { METER_PER_SECOND,     FOOT_PER_HOUR,      "3937.0078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921259842519685039370078740157480314960629921" },
    { METER_PER_SECOND,     KILOMETER_PER_HOUR, "3.6" },
    { METER_PER_SECOND,     KNOT,               "1.9438444924406047516198704103671706263498920086393088552915766738660907127429805615550755939524838012958963282937365010799136069114470842332613390928725702" },
    // Kilometre/hour
    { KILOMETER_PER_HOUR,   FOOT_PER_HOUR,      "1093.6132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841644794400699912510936132983377077865266841645" },
    // Knot
    { KNOT,                 MILE_PER_HOUR,      "1.1507794480235425117314881094408653463771574007794480235425117314881094408653463771574007794480235425117314881094408653463771574007794480235425117314881094" },
    { KNOT,                 FOOT_PER_HOUR,      "2025.3718285214348206474190726159230096237970253718285214348206474190726159230096237970253718285214348206474190726159230096237970253718285214348206474190726" },
    { KNOT,                 KILOMETER_PER_HOUR, "1.852" },
    { NO_UNIT,              NO_UNIT,            0 }
};

/*!
    Arbitrary unit conversions which are performed by calling a conversion
    function.

    Conversion functions have one argument - number to be converted.
*/
const UnitConversion::ArbitraryConversion UnitConversion::mArbitraryConversions[] =
{
    //---------------------------------------------------------------------
    // Termperature
    { CELSIUS,              FAHRENHEIT,         UnitConversion::ctof },
    { CELSIUS,              KELVIN,             UnitConversion::ctok },
    { FAHRENHEIT,           CELSIUS,            UnitConversion::ftoc },
    { FAHRENHEIT,           KELVIN,             UnitConversion::ftok },
    { KELVIN,               CELSIUS,            UnitConversion::ktoc },
    { KELVIN,               FAHRENHEIT,         UnitConversion::ktof },
    { NO_UNIT,              NO_UNIT,            0 }
};

/*!
    Units with their names and category.
*/
const UnitConversion::UnitDef UnitConversion::mUnits[] =
{
    // Angle
    { _T("rad"),         RADIAN,             ANGLE,   _T("Radian") },
    { _T("deg"),         DEGREE,             ANGLE,   _T("Degree") },
    { _T("grad"),        GRADIAN,            ANGLE,   _T("Gradian") },

    // Length
    { _T("mil"),        MIL,                LENGTH, _T("Mil") },
    { _T("in"),         INCH,               LENGTH, _T("Inch") },
    { _T("ft"),         FOOT,               LENGTH, _T("Foot") },
    { _T("yd"),         YARD,               LENGTH, _T("Yard") },
    { _T("mile"),       MILE,               LENGTH, _T("Mile") },
    { _T("micron"),     MICRON,             LENGTH, _T("Micron") },
    { _T("mm"),         MILLIMETER,         LENGTH, _T("Millimeter") },
    { _T("cm"),         CENTIMETER,         LENGTH, _T("Centimeter") },
    { _T("m"),          METER,              LENGTH, _T("Meter") },
    { _T("km"),         KILOMETER,          LENGTH, _T("Kilometer") },

    // Mass
    { _T("lb"),         POUND,              MASS,   _T("Pound") },
    { _T("oz"),         OUNCE,              MASS,   _T("Ounce") },
    { _T("g"),          GRAM,               MASS,   _T("Gram") },
    { _T("kg"),         KILOGRAM,           MASS,   _T("Kilogram") },

    // Temperature
    { _T("k"),          KELVIN,             TEMPERATURE, _T("Kelvin") },
    { _T("c"),          CELSIUS,            TEMPERATURE, _T("Celsius") },
    { _T("f"),          FAHRENHEIT,         TEMPERATURE, _T("Fahrenheit") },

    // Time
    { _T("micros"),     MICROSECOND,        TIME,   _T("Microsecond") },
    { _T("ms"),         MILLISECOND,        TIME,   _T("Millisecond") },
    { _T("s"),          SECOND,             TIME,   _T("Second") },
    { _T("min"),        MINUTE,             TIME,   _T("Minute") },
    { _T("h"),          HOUR,               TIME,   _T("Hour") },
    { _T("d"),          DAY,                TIME,   _T("Day") },
    { _T("w"),          WEEK,               TIME,   _T("Week") },

    // Velocity
    { _T("mile/h"),     MILE_PER_HOUR,      VELOCITY,_T("Mile per Hour") },
    { _T("m/s"),        METER_PER_SECOND,   VELOCITY,_T("Meter per Second") },
    { _T("ft/h"),       FOOT_PER_HOUR,      VELOCITY,_T("Foot per Hour") },
    { _T("km/h"),       KILOMETER_PER_HOUR, VELOCITY,_T("Kilometer per Hour") },
    { _T("knot"),       KNOT,               VELOCITY,_T("Knot") },

    { _T(""),           NO_UNIT,            NO_TYPE, _T("") }
};


/*!
    Converts \a number from \a unit1 to \a unit2.

    \return Converted number.

    \throw UnknownUnitException \a unit1 or \a unit2 is an unknown unit.
    \throw UnknownUnitConversionException There's no conversion \a unit1->unit2.
*/
BigDecimal UnitConversion::convert(const BigDecimal number,
                                   const tstring & unit1,
                                   const tstring & unit2)
{
    Unit u1 = NO_UNIT;
    Unit u2 = NO_UNIT;

    // Find unit1 and unit2 in mUnits array and assign u1 and u2
    for (const UnitDef * ud = mUnits; ud->unit != NO_UNIT && (u1 == NO_UNIT || u2 == NO_UNIT); ++ud) {
        if (unit1 == ud->name) u1 = ud->unit;
        if (unit2 == ud->name) u2 = ud->unit;
    }

    // Check that units are found
    if (u1 == NO_UNIT) throw ParserException(ParserException::UNKNOWN_UNIT, unit1);
    if (u2 == NO_UNIT) throw ParserException(ParserException::UNKNOWN_UNIT, unit2);

    // No conversion
    if (u1 == u2) return number;

    // Look up simple conversions table
    for (const SimpleConversion * sc = mSimpleConversions; sc->unit1 != NO_UNIT; ++sc) {
        if (u1 == sc->unit1 && u2 == sc->unit2) return number * sc->multiplier;
        if (u1 == sc->unit2 && u2 == sc->unit1) return number / sc->multiplier;
    }

    // Look up arbitrary conversions table
    for (const ArbitraryConversion * ac = mArbitraryConversions; ac->unit1 != NO_UNIT; ++ac) {
        if (u1 == ac->unit1 && u2 == ac->unit2) return ac->convert(number);
    }

    // There is no such conversion
    throw ParserException(ParserException::UNKNOWN_UNIT_CONVERSION,
                          unit1 + _T(" -> ") + unit2);
}

/*!
    Returns array of units.
*/
const UnitConversion::UnitDef * UnitConversion::units()
{
    return mUnits;
}

