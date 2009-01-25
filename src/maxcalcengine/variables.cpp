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
#include "variables.h"
#include "exceptions.h"

namespace MaxCalcEngine {

using namespace std;

/*!
	\class Variable
	\brief Represents a variable with name and value.
	\ingroup MaxCalcEngine
*/

/*!
	\class Variables
	\brief Represents a list of variables with functions to manage them.

	Variables are case-insensitive ('x' and 'X' is the same variable).
	Names are stores in the case they were added, but their retrieve is
	case-independent (e.g. you can add variable 'HelloWorld' and it will
	be stored as 'HelloWorld', and can be retrieved in this way through
	iterator, but you can get or change its value, or remove it by
	name 'helloworld' or 'HELLOWORLD').

	To retrieve value of a variable you can use operator[](name).
	To retrieve all variables there is const_iterator class and begin()
	and end() functions which return const_iterator.

	\ingroup MaxCalcEngine
*/


/*!
	Adds new variable with specified \a name and \a value.
	If the variable already exists its value is replaced.
*/
void Variables::add(const tstring & name, const Complex & value)
{
	tstring lowerName = name;
	strToLower(lowerName);
	vars_[lowerName] = Variable(name, value);
}

/*!
	Adds new variable with specified \a name and \a value.
	If the variable already exists its value is replaced.
*/
void Variables::add(const Variable & var)
{
	tstring lowerName = var.name;
	strToLower(lowerName);
	vars_[lowerName] = var;
}

/*!
	Removes variable with specified \a name.

	\exception UnknownVariableException Specified variable doesn't exist.
*/
void Variables::remove(tstring name)
{
	if (!vars_.erase(strToLower(name)))
		throw UnknownVariableException();
}

/*!
	Returns value of variables with specified \a name.

	\exception UnknownVariableException Specified variable doesn't exist.
*/
Complex Variables::operator[] (tstring name)
{
	VarsMap::const_iterator iter = vars_.find(strToLower(name));

	if (iter == vars_.end())
		throw UnknownVariableException();
	else
		return iter->second.value;
}

} // namespace MaxCalcEngine
