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
    if (!xml.FindElem(_T("m:math")) || !xml.IntoElem()) {
        throw MaxCalcException();
    }

    return parseOneLevel();
}


//****************************************************************************
// Private functions
//****************************************************************************

/*!
    Parses MathML tags that on single level (like within one <mrow> or within
    main <math> tag).
*/
tstring MathMLParser::parseOneLevel()
{
    tstring result;
    while (xml.FindElem()) {
        tstring tag = xml.GetTagName();
        if (tag == _T("m:mi")) {
            result += parseI();
        } else if (tag == _T("m:mn")) {
            result += parseN();
        } else if (tag == _T("m:mo")) {
            result += parseO();
        } else if (tag == _T("m:mrow")) {
            result += parseRow();
        } else if (tag == _T("m:mfrac")) {
            result += parseFrac();
        }
    }
    return result;
}

/*!
    Parses <mrow> tag.
*/
tstring MathMLParser::parseRow()
{
    xml.IntoElem();
    tstring result = parseOneLevel();
    xml.OutOfElem();
    return result;
}

/*!
    Parses <mfrac> tag.
    Assumes that <mfrac> contains two <mrow> tags.
*/
tstring MathMLParser::parseFrac()
{
    tstring result;
    xml.IntoElem();
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("((") + parseRow() + _T(")");
    result += _T("/");
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("(") + parseRow() + _T("))");
    xml.OutOfElem();
    return result;
}

/*!
    Parses <mi> tag.
*/
tstring MathMLParser::parseI()
{
    return xml.GetData();
}

/*!
    Parses <mn> tag.
*/
tstring MathMLParser::parseN()
{
    return xml.GetData();
}

/*!
    Parses <mo> tag.
*/
tstring MathMLParser::parseO()
{
    tstring data = xml.GetData();
    if (data == _T("\x22C5")) return _T("*");
    else return data;
}

} // namespace MaxCalcEngine
