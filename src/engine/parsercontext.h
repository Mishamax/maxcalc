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

#ifndef PARSERCONTEXT_H
#define PARSERCONTEXT_H

// Local
#include "complex.h"
#include "complexformat.h"
#include "exceptions.h"
#include "variables.h"

namespace MaxCalcEngine {


class ParserContext
{
public:

    ///////////////////////////////////////////////////////////////////////////
    // Enums

    /// Determines angle unit
    enum AngleUnit { RADIANS, DEGREES, GRADS };

    ///////////////////////////////////////////////////////////////////////////
    // Constructors

    explicit ParserContext(const ComplexFormat & numberFormat = ComplexFormat());


    ///////////////////////////////////////////////////////////////////////////
    // Accessors

    Complex result() const;
    void setResult(const Complex & result);

    /// Returns true if there is result of previous calculation.
    bool resultExists() const { return mResultExists; }

    /// Gets number format.
    ComplexFormat & numberFormat() { return mNumberFormat; }
    /// Sets number format.
    void setNumberFormat(const ComplexFormat & format) {mNumberFormat = format; }

    /// Gets variables.
    Variables & variables() { return mVars; }
    /// Sets variables.
    void setVariables(const Variables & vars) { mVars = vars; }

    /// Gets angle unit.
    AngleUnit angleUnit() const { return mAngleUnit; }
    /// Sets angle unit.
    void setAngleUnit(const AngleUnit unit) { mAngleUnit = unit; }

private:

    ///////////////////////////////////////////////////////////////////////////
    // Properties

    Complex mResult;                ///< Result of last calculation.
    bool mResultExists;             ///< Determines if result exists.
    ComplexFormat mNumberFormat;    ///< Number format used for conversions.
    Variables mVars;                ///< Variables.
    AngleUnit mAngleUnit;           ///< Angle unit.
};


} // namespace MaxCalcEngine

#endif // PARSERCONTEXT_H
