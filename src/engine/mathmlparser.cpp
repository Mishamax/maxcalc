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

        if (tag == _T("m:mi")) result += parseI();
        else if (tag == _T("m:mn")) result += parseN();
        else if (tag == _T("m:mo")) result += parseO();
        else if (tag == _T("m:mrow")) result += parseRow();
        else if (tag == _T("m:mfrac")) result += parseFrac();
        else if (tag == _T("m:msup")) result += parseSup();
        else if (tag == _T("m:mfenced")) result += parseFenced();
        else if (tag == _T("m:msqrt")) result += parseSqrt();
        else if (tag == _T("m:mroot")) result += parseRoot();
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
    Parses <mfrac> tag (fraction).
    Assumes that <mfrac> contains two <mrow> tags.
*/
tstring MathMLParser::parseFrac()
{
    tstring result;
    xml.IntoElem();
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("((") + parseRow() + _T(") / ");
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("(") + parseRow() + _T("))");
    xml.OutOfElem();
    return result;
}

/*!
    Parses <msup> tag (power).
    Assumes that <msup> contains two <mrow> tags.
*/
tstring MathMLParser::parseSup()
{
    tstring result;
    xml.IntoElem();
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("pow(") + parseRow() + _T("; ");
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += parseRow() + _T(")");
    xml.OutOfElem();
    return result;
}

/*!
    Parses <mfenced> tag (brackets).
*/
tstring MathMLParser::parseFenced()
{
    tstring result;
    tstring open = xml.GetAttrib(_T("open"));
    tstring close = xml.GetAttrib(_T("close"));
    xml.IntoElem();
    result += open + parseOneLevel() + close;
    xml.OutOfElem();
    return result;
}

/*!
    Parses <msqrt> tag (square root).
*/
tstring MathMLParser::parseSqrt()
{
    tstring result;
    xml.IntoElem();
    result += _T("sqrt(") + parseOneLevel() + _T(")");
    xml.OutOfElem();
    return result;
}

/*!
    Parses <mroot> tag (root).
    Assumes that <msup> contains two <mrow> tags.
*/
tstring MathMLParser::parseRoot()
{
    tstring result;
    xml.IntoElem();
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += _T("pow(") + parseRow() + _T("; 1/(");
    xml.FindElem();
    if (xml.GetTagName() != _T("m:mrow")) throw MaxCalcException();
    result += parseRow() + _T("))");
    xml.OutOfElem();
    return result;
}

/*!
    Parses <mi> tag (identifier).
*/
tstring MathMLParser::parseI()
{
    tstring data = xml.GetData();
    xml.SavePos(_T("mi"));
    if (xml.FindElem()) {
        if (xml.GetTagName() == _T("m:mi")) {
            xml.SetData(data + xml.GetData());
            data = _T("");
        } else if (xml.GetTagName() == _T("m:msup")) {
            if (xml.IntoElem() && xml.FindElem() && xml.GetTagName() == _T("m:mrow")) {
                if (xml.IntoElem() && xml.FindElem() && xml.GetTagName() == _T("m:mi")) {
                    xml.SetData(data + xml.GetData());
                    data = _T("");
                }
            }
        }
    }
    xml.RestorePos(_T("mi"));
    if (data == _T("")) xml.RemoveElem();
    findAndReplace(data, _T('\x2148'), _T("i"));    // imaginary one
    return data;
}

/*!
    Parses <mn> tag (number).
*/
tstring MathMLParser::parseN()
{
    return xml.GetData();
}

/*!
    Parses <mo> tag (operator).
*/
tstring MathMLParser::parseO()
{
    tstring data = xml.GetData();
    findAndReplace(data, _T('\x22C5'), _T("*"));    // multiply
    findAndReplace(data, _T('\x2215'), _T("/"));    // divide
    findAndReplace(data, _T('\x2061'), _T(""));     // invisible multiplication
    findAndReplace(data, _T('\x2192'), _T("->"));   // arrow (unit conversion)
    return data;
}

void MathMLParser::findAndReplace(tstring & str, const tchar what, const tstring & replacement)
{
    size_t pos = 0;
    while ((pos = str.find(what)) != tstring::npos) {
        str.replace(pos, 1, replacement);
    }
}

} // namespace MaxCalcEngine
