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

#ifndef COMPLEX_H
#define COMPLEX_H

// Local
#include "bigdecimal.h"
#include "complexformat.h"
#include "unicode.h"

namespace MaxCalcEngine {


class Complex
{
public:

    /// Real part of the complex number.
    BigDecimal re;
    /// Imaginary part of the complex number.
    BigDecimal im;


    ///////////////////////////////////////////////////////////////////////////
    // Constants
    static const Complex i;


    ///////////////////////////////////////////////////////////////////////////
    // Constructors

    Complex();
    Complex(const char * real, const char * imaginary = "0",
        const ComplexFormat & format = ComplexFormat::getDefault());
    Complex(const std::string & real, const std::string & imaginary = "0",
        const ComplexFormat & format = ComplexFormat::getDefault());
#if defined(MAXCALC_UNICODE)
    Complex(const wchar_t * real, const wchar_t * imaginary = L"0",
        const ComplexFormat & format = ComplexFormat::getDefault());
    Complex(const std::wstring & real, const std::wstring & imaginary = L"0",
        const ComplexFormat & format = ComplexFormat::getDefault());
#endif
    Complex(const BigDecimal & real, const BigDecimal & imaginary = 0);
    Complex(const int real, const int imaginary = 0);
    Complex(const unsigned real, const unsigned imaginary = 0);
    Complex(const Complex & num);


    ///////////////////////////////////////////////////////////////////////////
    // Conversion functions

    std::string toString(const ComplexFormat & format =
        ComplexFormat::getDefault()) const;
#if defined(MAXCALC_UNICODE)
    std::wstring toWideString(const ComplexFormat & format =
        ComplexFormat::getDefault()) const;
#endif
    tstring toTString(const ComplexFormat & format =
        ComplexFormat::getDefault()) const;


    ///////////////////////////////////////////////////////////////////////////
    // Operators

    Complex operator+() const;
    Complex operator-() const;

    Complex operator+(const Complex & num) const;
    Complex operator-(const Complex & num) const;
    Complex operator*(const Complex & num) const;
    Complex operator/(const Complex & num) const;

    Complex operator+=(const Complex & num);
    Complex operator-=(const Complex & num);
    Complex operator*=(const Complex & num);
    Complex operator/=(const Complex & num);

    bool operator==(const Complex & num) const;
    bool operator!=(const Complex & num) const;


    ///////////////////////////////////////////////////////////////////////////
    // Misc functions
    bool isZero() const;


    ///////////////////////////////////////////////////////////////////////////
    // Math functions

    static BigDecimal sqr(const Complex & num);
    static BigDecimal abs(const Complex & num);
    static BigDecimal arg(const Complex & num);
    static BigDecimal factorial(const Complex & num);
    static Complex ln(const Complex & num);
    static Complex log2(const Complex & num);
    static Complex log10(const Complex & num);
    static Complex exp(const Complex & num);
    static Complex pow(const Complex & num, const Complex & power);
    static Complex sqrt(const Complex & num);
    static Complex sin(const Complex & num);
    static Complex cos(const Complex & num);
    static Complex tan(const Complex & num);
    static Complex cot(const Complex & num);
    static Complex arcsin(const Complex & num);
    static Complex arccos(const Complex & num);
    static Complex arctan(const Complex & num);
    static Complex arccot(const Complex & num);
    static Complex sinh(const Complex & num);
    static Complex cosh(const Complex & num);
    static Complex tanh(const Complex & num);
    static Complex coth(const Complex & num);
    static Complex arcsinh(const Complex & num);
    static Complex arccosh(const Complex & num);
    static Complex arctanh(const Complex & num);
    static Complex arccoth(const Complex & num);

};

} // namespace MaxCalcEngine

#endif
