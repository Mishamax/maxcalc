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

#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

// Local
#include "bigdecimal.h"
#include "unicode.h"


class UnitConversion
{
public:

    /*! Units. */
    enum Unit
    {
        NO_UNIT,
        // Length
        MIL, INCH, FOOT, YARD, MILE, MICRON, MILLIMETER, CENTIMETER, METER, KILOMETER,
        // Weight
        POUND, OUNCE, GRAMM, KILOGRAM,
        // Time
        MICROSECOND, MILLISECOND, SECOND, MINUTE, HOUR, DAY,
        // Speed
        MILE_PER_HOUR, METER_PER_SECOND, FOOT_PER_HOUR, KILOMETER_PER_HOUR, KNOT,
        // Temperature
        CELSIUS, FAHRENHEIT, KELVIN
    };

    /*! Types of unit conversions. */
    enum Type
    {
        NO_TYPE, LENGTH, WEIGHT, TIME, SPEED, TEMPERATURE
    };

    /*! Unit definition for table of units. */
    struct UnitDef
    {
        tstring name;   ///< English name of the unit.
        Unit unit;      ///< Unit
        Type type;      ///< Type of the unit.
        tstring desc;   ///< Description (full name of the unit).
    };

private:
    static const UnitDef mUnits[];

    /*! Represents simple unit conversion (multiply / divide by multiplier). */
    struct SimpleConversion
    {
        const Unit unit1;
        const Unit unit2;
        const BigDecimal multiplier;
    };

    /*! Represents arbitrary unit conversion (call conversion functions). */
    struct ArbitraryConversion
    {
        const Unit unit1;
        const Unit unit2;
        BigDecimal (*convert)(const BigDecimal & arg);
    };

    static const SimpleConversion mSimpleConversions[];
    static const ArbitraryConversion mArbitraryConversions[];

    // Arbitrary conversions functions.
    static BigDecimal ctof(const BigDecimal & arg) { return arg * 1.8 + 32; }
    static BigDecimal ctok(const BigDecimal & arg) { return arg + 273.15; }
    static BigDecimal ftoc(const BigDecimal & arg) { return (arg + 459.67) * 5.0 / 9.0 - 273.15; }
    static BigDecimal ftok(const BigDecimal & arg) { return (arg + 459.67) * 5.0 / 9.0; }
    static BigDecimal ktoc(const BigDecimal & arg) { return arg - 273.15; }
    static BigDecimal ktof(const BigDecimal & arg) { return (arg - 273.15) * 1.8 + 32; }

public:
    static BigDecimal convert(const BigDecimal number,
                              const tstring & unit1,
                              const tstring & unit2);

    static const UnitDef * units();
};


#endif // UNITCONVERSION_H
