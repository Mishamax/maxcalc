/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2008 Michael Maximov (michael.maximov@gmail.com)
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

namespace MaxCalcEngine {

//****************************************************************************
// ParserContext definition
//****************************************************************************

class ParserContext
{
public:
	ParserContext();

	inline void setResult(const Complex & result) { m_result = result; m_resultExists = true; }
	// TODO: throw right exception
	inline Complex getResult() const { if (m_resultExists) return m_result; else throw std::exception(); }
	inline void unsetResult() { m_resultExists = false; }

private:
	Complex m_result;
	bool m_resultExists;
};

} // namespace MaxCalcEngine

#endif // PARSERCONTEXT_H
