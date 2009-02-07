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

#ifndef VARIABLES_H
#define VARIABLES_H

// Local
#include "unicode.h"
#include "complex.h"
//STL
#include <map>
#include <iterator>

namespace MaxCalcEngine {


struct Variable
{
	tstring name;		///< Name of variable.
	Complex value;		///< Value of variable.

	/// Constructs a new variable with specified \a name and \a value.
	Variable(const tstring & name_ = _T(""), const Complex & value_ = 0)
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
	inline void removeAll();
	Complex operator[] (tstring name);
	inline size_t count();
	inline const_iterator begin();
	inline const_iterator end();

private:
	typedef std::map<tstring, Variable> VarsMap;
	VarsMap vars_;

public:
	class const_iterator : public VarsMap::const_iterator
	{
	public:
		const_iterator() : VarsMap::const_iterator()
		{
		}

		const_iterator(const VarsMap::const_iterator & iter) :
			VarsMap::const_iterator(iter)
		{
		}

		const Variable & operator* ()
		{
			return (VarsMap::const_iterator::operator*()).second;
		}

		const Variable * operator-> ()
		{
			return &((VarsMap::const_iterator::operator*()).second);
		}
	};
};


///////////////////////////////////////////////////////////////////////////
// Inline functions

/*!
	Removes all variables.
*/
inline void Variables::removeAll()
{
	vars_.clear();
}

/*!
	Returns number of variables.
*/
inline size_t Variables::count()
{
	return vars_.size();
}

inline Variables::const_iterator Variables::begin()
{
    return const_iterator(vars_.begin());
}

inline Variables::const_iterator Variables::end()
{
    return const_iterator(vars_.end());
}

} // namespace MaxCalcEngine

#endif // VARIABLES_H
