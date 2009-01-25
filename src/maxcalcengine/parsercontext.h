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

#ifndef PARSERCONTEXT_H
#define PARSERCONTEXT_H

// Local
#include "complex.h"
#include "complexformat.h"
#include "exceptions.h"
#include "variables.h"

namespace MaxCalcEngine {


class ParserContext
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Constructors

	explicit ParserContext(const ComplexFormat & numberFormat =
		ComplexFormat::getDefault());


	///////////////////////////////////////////////////////////////////////////
	// Accessors

	inline Complex result() const;
	inline void setResult(const Complex & result);
	inline bool resultExists() const;

	inline ComplexFormat & numberFormat();
	inline void setNumberFormat(const ComplexFormat & numberFormat);

	inline Variables & variables();
	inline void setVariables(const Variables & vars);

private:

	///////////////////////////////////////////////////////////////////////////
	// Properties

	Complex result_;				///< Result of last calculation.
	bool resultExists_;				///< Determines if result exists.
	ComplexFormat numberFormat_;	///< Number format used for conversions.
	Variables vars_;				///< Variables.
};



///////////////////////////////////////////////////////////////////////////
// Inline functions

/*!
	Gets result of last calculation.
	\exception ResultDoesNotExistException Result does not exist.
*/
inline Complex ParserContext::result() const
{
	if (resultExists_)
		return result_;
	else
		throw ResultDoesNotExistException();
}

/*!
	Sets result of last calculation.
*/
inline void ParserContext::setResult(const Complex & result)
{
	result_ = result;
	resultExists_ = true;
}

/*!
	Determines if result of last calculation exists.
*/
inline bool ParserContext::resultExists() const
{
	return resultExists_;
}

/*!
	Gets number format.
*/
inline ComplexFormat & ParserContext::numberFormat()
{
	return numberFormat_;
}

/*!
	Sets number format.
*/
inline void ParserContext::setNumberFormat(const ComplexFormat & numberFormat)
{
	numberFormat_ = numberFormat;
}

/*!
	Gets variables.
*/
inline Variables & ParserContext::variables()
{
	return vars_;
}

/*!
	Sets variables.
*/
inline void ParserContext::setVariables(const Variables & vars)
{
	vars_ = vars;
}

} // namespace MaxCalcEngine

#endif // PARSERCONTEXT_H
