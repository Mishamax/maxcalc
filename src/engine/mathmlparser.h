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

#ifndef MATHMLPARSER_H
#define MATHMLPARSER_H

// Local
#include "unicode.h"
#include "Markup.h"

namespace MaxCalcEngine {


class MathMLParser
{
public:

    MathMLParser(const tstring & expr);

    tstring parse();

private:
    CMarkup xml;                            ///< CMarkup XML parser

    tstring parseOneLevel();
    tstring parseRow();
    tstring parseFrac();
    tstring parseSup();
    tstring parseFenced();
    tstring parseSqrt();
    tstring parseRoot();
    tstring parseI();
    tstring parseN();
    tstring parseO();

    void findAndReplace(tstring & str, const tchar what, const tstring & replacement);
};


} // namespace MaxCalcEngine

#endif // MATHMLPARSER_H
