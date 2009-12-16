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
#include "mathmlparser.h"
#include "exceptions.h"
#include "Markup.h"

namespace MaxCalcEngine {


/*!
    \class MathMLParser
    \brief Parses MathML code to arithmetic expressions.
    
    Based on CMarkup XML parser.

    \ingroup MaxCalcEngine
*/


//****************************************************************************
// Constructors
//****************************************************************************


/*!
    Constructs a new instance of MathMLParser with specified \a expr.
*/
MathMLParser::MathMLParser(const tstring & expr)
{
    xml.SetDoc(expr);
}


//****************************************************************************
// Public functions
//****************************************************************************

/*!
    Performs parsing of given expression.
*/
tstring MathMLParser::parse()
{
    tstring result = _T("");

    // Root <math> element
    if (!xml.FindElem(_T("math")) || !xml.IntoElem()) {
        throw MaxCalcException();
    }
    if (!parseRow(result)) {
        throw MaxCalcException();
    }

    return result;
}


//****************************************************************************
// Private functions
//****************************************************************************

/*!
    Parses <mrow> tag.
    Looks for <mn> tag within <mrow> and parses it.
*/
bool MathMLParser::parseRow(tstring & result)
{
    if (!xml.FindElem(_T("mrow")) || !xml.IntoElem()) return false;
    bool found = true;
    while (found) {
        if (parseI(result)) { found = true; continue; }
        if (parseN(result)) { found = true; continue; }
        if (parseO(result)) { found = true; continue; }
        found = false;
    }
    if (!xml.OutOfElem()) return false;
    return true;
}

/*!
    Parses <mi> tag (identifier).
    Adds the content of the tag to \a result.
*/
bool MathMLParser::parseI(tstring & result)
{
    if (!xml.FindElem(_T("mi"))) return false;
    tstring data = xml.GetData();
    if (data == _T("")) return false;
    result += data;
    return true;
}


/*!
    Parses <mn> tag (number).
    Adds the content of the tag to \a result.
*/
bool MathMLParser::parseN(tstring & result)
{
    if (!xml.FindElem(_T("mn"))) return false;
    tstring data = xml.GetData();
    if (data == _T("")) return false;
    result += data;
    return true;
}

/*!
    Parses <mo> tag (operator).
    Adds the content of the tag to \a result.
*/
bool MathMLParser::parseO(tstring & result)
{
    if (!xml.FindElem(_T("mo"))) return false;
    tstring data = xml.GetData();
    if (data == _T("")) return false;
    result += data;
    return true;
}

} // namespace MaxCalcEngine
