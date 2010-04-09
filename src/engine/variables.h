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

#ifndef VARIABLES_H
#define VARIABLES_H

// Local
#include "complex.h"
#include "unicode.h"
//STL
#include <map>
#include <iterator>


struct Variable
{
    tstring name;       ///< Name of variable.
    Complex value;      ///< Value of variable.

    /// Default constructor.
    Variable()
    {
        name = _T("");
        value = 0;
    }

    /// Constructs a new variable with specified \a name and \a value.
    Variable(const tstring & name_, const Complex & value_)
    {
        name = name_;
        value = value_;
    }
};

class Variables
{
public:
    class const_iterator;

    void add(const tstring & name, const Complex & value);
    void add(const Variable & var);
    void remove(tstring name);
    void removeAll();
    Complex operator[] (tstring name);
    size_t count();
    const_iterator begin();
    const_iterator end();

private:
    typedef std::map<tstring, Variable> VarsMap;
    VarsMap mVars;

public:
    class const_iterator : public VarsMap::const_iterator
    {
    public:
        /// Default constructor.
        const_iterator() : VarsMap::const_iterator()
        {
        }

        /// Constructs const_iterator from std::map<tstring, Variable> iterator.
        const_iterator(const VarsMap::const_iterator & iter) :
            VarsMap::const_iterator(iter)
        {
        }

        /// Gets Variable associated with current value.
        const Variable & operator* ()
        {
            return (VarsMap::const_iterator::operator*()).second;
        }

        /// Gets Variable associated with current value.
        const Variable * operator-> ()
        {
            return &((VarsMap::const_iterator::operator*()).second);
        }
    };
};


#endif // VARIABLES_H
