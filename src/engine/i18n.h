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

#ifndef I18N_H
#define I18N_H

#include "unicode.h"
#include "exceptions.h"

#if defined(MAXCALC_QT_I18N)
#include <QCoreApplication>
#define i18n_string QString
#else
#define Q_DECLARE_TR_FUNCTIONS(className)
#define i18n_string tstring
#endif

namespace MaxCalcEngine {

class I18n
{
    Q_DECLARE_TR_FUNCTIONS(I18n)

public:
    static i18n_string maxCalcExceptionToString(MaxCalcException & ex);
    static i18n_string parserExceptionToString(ParserException & ex);
    static i18n_string arithmeticExceptionToString(ArithmeticException & ex);
    static i18n_string invalidArgumentExceptionToString(InvalidArgumentException & ex);

private:
    static i18n_string addArg(const i18n_string str, const tstring & arg);
    static i18n_string addArg(const i18n_string str, const tstring & arg1,
                              const i18n_string arg2);
};

}; // namespace MaxCalcEngine

#endif // I18N_H
