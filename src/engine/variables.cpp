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

// Local
#include "variables.h"
#include "exceptions.h"


using namespace std;

/*!
    \struct Variable
    \brief Represents a variable as a (name, value) pair.
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
    To retrieve all variables there is const_iterator class and begin(),
    and end() functions which return const_iterator.

    \ingroup MaxCalcEngine
*/

/*!
    \class Variables::const_iterator
    \brief Constant iterator for Variables class.
    
    This iterator is used to retrieve variables. It can be done by using
    begin() and end() functions in Variables class.
*/

/*!
    Adds new variable with specified \a name and \a value.
    If the variable already exists its value is replaced.
*/
void Variables::add(const tstring & name, const Complex & value)
{
    tstring lowerName = name;
    strToLower(lowerName);
    mVars[lowerName] = Variable(name, value);
}

/*!
    Adds new variable with specified \a name and \a value.
    If the variable already exists its value is replaced.
*/
void Variables::add(const Variable & var)
{
    tstring lowerName = var.name;
    strToLower(lowerName);
    mVars[lowerName] = var;
}

/*!
    Removes variable with specified \a name.

    \exception UnknownVariableException Specified variable doesn't exist.
*/
void Variables::remove(tstring name)
{
    if (!mVars.erase(strToLower(name))) {
        throw ParserException(ParserException::UNKNOWN_VARIABLE, name);
    }
}

/*!
    Returns value of variables with specified \a name.

    \exception UnknownVariableException Specified variable doesn't exist.
*/
Complex Variables::operator[] (tstring name)
{
    VarsMap::const_iterator iter = mVars.find(strToLower(name));

    if (iter == mVars.end()) throw ParserException(ParserException::UNKNOWN_VARIABLE, name);
    else return iter->second.value;
}

/*!
    Removes all variables.
*/
void Variables::removeAll()
{
    mVars.clear();
}

/*!
    Returns number of variables.
*/
size_t Variables::count()
{
    return mVars.size();
}

/*!
    Returns const_iterator pointing to the beginning of variables' map.
*/
Variables::const_iterator Variables::begin()
{
    return const_iterator(mVars.begin());
}

/*!
    Returns const_iterator pointing to the next element after the end of
    variables' map.
*/
Variables::const_iterator Variables::end()
{
    return const_iterator(mVars.end());
}

