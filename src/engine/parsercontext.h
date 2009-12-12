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
    // Constructors

    explicit ParserContext(const ComplexFormat & numberFormat =
        ComplexFormat::getDefault());


    ///////////////////////////////////////////////////////////////////////////
    // Accessors

    Complex result() const;
    void setResult(const Complex & result);
    bool resultExists() const;

    ComplexFormat & numberFormat();
    void setNumberFormat(const ComplexFormat & numberFormat);

    Variables & variables();
    void setVariables(const Variables & vars);

private:

    ///////////////////////////////////////////////////////////////////////////
    // Properties

    Complex mResult;                ///< Result of last calculation.
    bool mResultExists;             ///< Determines if result exists.
    ComplexFormat mNumberFormat;    ///< Number format used for conversions.
    Variables mVars;                ///< Variables.
};


} // namespace MaxCalcEngine

#endif // PARSERCONTEXT_H
