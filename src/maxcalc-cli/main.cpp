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

#include "bigdecimal.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
//		BigDecimal res = BigDecimal(L"12234.234") * BigDecimal(L"2434.23443") / BigDecimal(L"3.1");
		BigDecimal res = BigDecimal(QString("100000000000000000")) / BigDecimal(QString("2"));
		wcout << res.toString().toAscii().data() << endl;
		BigDecimal dec = 1;
	}
	catch (...)
	{
		cout << "Exception" << endl;
	}
	return 0;
}
