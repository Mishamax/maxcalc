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
#include "parsercontext.h"

namespace MaxCalcEngine {


/*!
    \class ParserContext
    \brief Represents context used by Parser.

    Context is used to store Parser state between calculations and defines
    behavior of Parser.
    It includes result of last calculation and number format used to
    conversions.

    \sa Parser
    \ingroup MaxCalcEngine
*/

/*!
    Constucts new ParserContext with given \a numberFormat which will be used
    for number conversions in Parser.
*/
ParserContext::ParserContext(const ComplexFormat & numberFormat)
{
    mResult = Complex();
    mResultExists = false;
    mNumberFormat = numberFormat;
}

/*!
    Gets result of last calculation.
    \exception ResultDoesNotExistException Result does not exist.
*/
Complex ParserContext::result() const
{
    if (mResultExists) return mResult;
    else throw ResultDoesNotExistException();
}

/*!
    Sets result of last calculation.
*/
void ParserContext::setResult(const Complex & result)
{
    mResult = result;
    mResultExists = true;
}

/*!
    Determines if result of last calculation exists.
*/
bool ParserContext::resultExists() const
{
    return mResultExists;
}

/*!
    Gets number format.
*/
ComplexFormat & ParserContext::numberFormat()
{
    return mNumberFormat;
}

/*!
    Sets number format.
*/
void ParserContext::setNumberFormat(const ComplexFormat & numberFormat)
{
    mNumberFormat = numberFormat;
}

/*!
    Gets variables.
*/
Variables & ParserContext::variables()
{
    return mVars;
}

/*!
    Sets variables.
*/
void ParserContext::setVariables(const Variables & vars)
{
    mVars = vars;
}

} // namespace MaxCalcEngine
