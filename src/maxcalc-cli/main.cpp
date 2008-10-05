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

// Local
#include "bigdecimal.h"
#include "complex.h"

// STL
#include <iostream>
#include <string>

using namespace std;
using namespace MaxCalcEngine;

int main()
{
	wcout << BigDecimal::sin(BigDecimal::PI).toString().c_str() << endl;
	wcout << BigDecimal::sin(BigDecimal::PI * BigDecimal(L"99999999999999999999999999999999999999999999999999999999999999999999999999")).toString().c_str() << endl;

	return 0;
}
