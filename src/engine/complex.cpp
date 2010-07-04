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
#include "complex.h"
#include "exceptions.h"


/*!
    \class Complex
    \brief Represents a complex number.

    BigDecimal class is used to represent real and imaginary parts of
    the complex number.

    When converting to string, number format is specified by ComplexFormat class.

    \sa BigDecimal, ComplexFormat
    \ingroup MaxCalcEngine
*/


/*!
    Imaginary one.
*/
const Complex Complex::i = Complex(0, 1);


//****************************************************************************
// Constructors
//****************************************************************************

/*!
    Default constructor (initializes the number with zero).
*/
Complex::Complex()
{
    re = 0;
    im = 0;
}

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const char * real, const char * imaginary) :
        re(real), im(imaginary)
{
}

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const string & real, const string & imaginary) :
        re(real), im(imaginary)
{
}

#if defined(MAXCALC_UNICODE)

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const wchar_t * real, const wchar_t * imaginary) :
        re(real), im(imaginary)
{
}

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const wstring & real, const wstring & imaginary) :
        re(real), im(imaginary)
{
}

#endif // #if defined(MAXCALC_UNICODE)

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const BigDecimal & real, const BigDecimal & imaginary)
{
    re = real;
    im = imaginary;
}

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const int real, const int imaginary)
{
    re = real;
    im = imaginary;
}

/*!
    Constructs a new instance of Complex class from given \a real and
    \a imaginary parts.
*/
Complex::Complex(const unsigned real, const unsigned imaginary)
{
    re = real;
    im = imaginary;
}

/*!
    Constructs a copy of \a num.
*/
Complex::Complex(const Complex & num)
{
    re = num.re;
    im = num.im;
}


//****************************************************************************
// Conversion functions
//****************************************************************************

/*!
    Converts this number to std::string using given ComplexFormat.

    If \a format is not specified, the default ComplexFormat is used.

    \sa ComplexFormat
*/
string Complex::toString(const ComplexFormat & format) const
{
    string rePart = re.toString(format);
    string imPart = BigDecimal::abs(im).setBase(im.base()).toString(format);
    bool isReZero = re.isZero();
    bool isImZero = im.isZero();
    bool isImNegative = im.isNegative();

    string result = "";

    if (isReZero && isImZero) {
        return "0";
    }

    result = isReZero ? "0" : rePart;

    if (!isImZero) {
        if (isImNegative) result += " - ";
        else result += " + ";

        result += (isImZero ? "0" : imPart) + format.imaginaryOneChar();
    }

    return result;
}

#if defined(MAXCALC_UNICODE)

/*!
    Converts this number to std::wstring using given ComplexFormat.

    If \a format is not specified, the default ComplexFormat is used.

    \sa ComplexFormat
*/
wstring Complex::toWideString(const ComplexFormat & format) const
{
    return stringToWideString(toString(format));
}

#endif // #if defined(MAXCALC_UNICODE)

/*!
    Converts this number to tstring using given ComplexFormat.

    If \a format is not specified, the default ComplexFormat is used.

    \sa ComplexFormat, tstring
*/
tstring Complex::toTString(const ComplexFormat & format) const
{
#if defined(MAXCALC_UNICODE)
        return toWideString(format);
#else
        return toString(format);
#endif
}

//****************************************************************************
// Misc functions
//****************************************************************************

/*!
    Sets number base.
    This setting overrides base set in BigDecimalFormat during conversion to string.
    Set base = 0 to disable override.

    \throws ArithmeticException Invalid base is specified.
    \returns this number.
    \see BigDecimal.setBase
*/
Complex & Complex::setBase(int base)
{
    re.setBase(base);
    im.setBase(base);
    return *this;
}

/*!
    Returns number base.

    \see BigDecimal.base
*/
int Complex::base() const
{
    return re.base();
}

//****************************************************************************
// Operators
//****************************************************************************

/*!
    Returns the same Complex number as this number.
*/
Complex Complex::operator+() const
{
    return *this;
}

/*!
    Returns negated number.
*/
Complex Complex::operator-() const
{
    return Complex(-re, -im);
}

/*!
    Adds two numbers.
*/
Complex Complex::operator+(const Complex & num) const
{
    return Complex(re + num.re, im + num.im);
}

/*!
    Subtracts two numbers.
*/
Complex Complex::operator-(const Complex & num) const
{
    return Complex(re - num.re, im - num.im);
}

/*!
    Multiplies two numbers.
*/
Complex Complex::operator*(const Complex & num) const
{
    return Complex(re * num.re - im * num.im, re * num.im + im * num.re);
}

/*!
    Divides two numbers.

    \exception ArithmeticException(DIVISION_BY_ZERO) \a num == (0, 0) is given.
*/
Complex Complex::operator/(const Complex & num) const
{
    BigDecimal sqrt = num.re * num.re + num.im * num.im;
    if (sqrt.isZero()) {
        throw ArithmeticException(ArithmeticException::DIVISION_BY_ZERO);
    }
    return Complex((re * num.re + im * num.im) / sqrt,
        (im * num.re - re * num.im) / sqrt);
}

/*!
    Adds \a num to this.
*/
Complex Complex::operator+=(const Complex & num)
{
    return Complex(re += num.re, im += num.im);
}

/*!
    Subtracts \a num from this.
*/
Complex Complex::operator-=(const Complex & num)
{
    return Complex(re -= num.re, im -= num.im);
}

/*!
    Multiplies this by \a num.
*/
Complex Complex::operator*=(const Complex & num)
{
    Complex result = Complex(re * num.re - im * num.im,
        re * num.im + im * num.re);
    re = result.re;
    im = result.im;
    return result;
}

/*!
    Divides this by \a num.

    \exception ArithmeticException(DIVISION_BY_ZERO) \a num == (0, 0) is given.
*/
Complex Complex::operator/=(const Complex & num)
{
    BigDecimal sqrt = num.re * num.re + num.im * num.im;
    if (sqrt.isZero()) {
        throw ArithmeticException(ArithmeticException::DIVISION_BY_ZERO);
    }
    Complex result = Complex((re * num.re + im * num.im) / sqrt,
        (im * num.re - re * num.im) / sqrt);
    re = result.re;
    im = result.im;
    return result;
}

/*!
    Returns \a true if two numbers are equal.
*/
bool Complex::operator==(const Complex & num) const
{
    return (num.re == re) && (num.im == im);
}

/*!
    Returns \a true if two numbers are not equal.
*/
bool Complex::operator!=(const Complex & num) const
{
    return (re != num.re) || (im != num.im);
}


//****************************************************************************
// Misc functions
//****************************************************************************

/*!
    Returns true if this is zero.
*/
bool Complex::isZero() const
{
    return re.isZero() && im.isZero();
}


//****************************************************************************
// Math functions
//****************************************************************************

/*!
    Calculates square of \a num.

    sqr(num) = Re(num)^2 + Im(num)^2.
*/
BigDecimal Complex::sqr(const Complex & num)
{
    // z^2 = |z|^2 = Re^2(z) + Im^2(z)
    return (num.re * num.re + num.im * num.im);
}

/*!
    Calculates absolute value of \a num.

    abs(num) = sqrt(sqr(num)).
*/
BigDecimal Complex::abs(const Complex & num)
{
    return BigDecimal::sqrt(sqr(num));
}

/*!
    Calculates argument of \a num.

    arg(num) = arccos(re(z) / |z|).
*/
BigDecimal Complex::arg(const Complex & num)
{
    if (num.im.isZero()) {
        return (num.re >= 0) ? 0 : BigDecimal::PI;
    }

    BigDecimal arg = BigDecimal::arccos(num.re / abs(num));
    return (num.im >= 0) ? arg : -arg;
}

/*!
    Calculates factorial of \a num.

    \a num must be integer and >= 0.

    \exception InvalidArgumentException Fractional or negative \a num is given.
*/
BigDecimal Complex::factorial(const Complex & num)
{
    if (!num.im.isZero()) {
        throw InvalidArgumentException(_T("fact"),
            InvalidArgumentException::FACTORIAL_FUNCTION);
    }

    return BigDecimal::factorial(num.re);
}

/*!
    Calculates natural logarithm of \a num.

    Ln(num) = ln(|num|) + i*arg(num) + 2*Pi*k*i (k = 0).
*/
Complex Complex::ln(const Complex & num)
{
    if (num.im.isZero() && num.re >= 0) {
        return BigDecimal::ln(num.re);
    }
    return Complex(0, arg(num)) + BigDecimal::ln(abs(num));
}

/*!
    Calculates base-2 logarithm of \a num.

    log2(num) = ln(num) / ln(2)
*/
Complex Complex::log2(const Complex & num)
{
    try {
        return ln(num) / BigDecimal::ln(2);
    } catch (InvalidArgumentException & ex) {
        throw InvalidArgumentException(_T("log2"), ex.reason());
    }
}

/*!
    Calculates base-10 logarithm of \a num.

    log10(num) = ln(num) / ln(10)
*/
Complex Complex::log10(const Complex & num)
{
    try {
        return ln(num) / BigDecimal::ln(10);
    } catch (InvalidArgumentException & ex) {
        throw InvalidArgumentException(_T("log10"), ex.reason());
    }
}

/*!
    Calculates exponent of \a num.

    exp(num) = exp(re(num) + i*im(num)) = exp(re(num)) * exp(i*im(num)) =
    exp(re(z)) * (cos(im(z)) + i*sin(im(z))).
*/
Complex Complex::exp(const Complex & num)
{
    if (num.im.isZero()) {
        return BigDecimal::exp(num.re);
    }
    return Complex(BigDecimal::cos(num.im),
        BigDecimal::sin(num.im)) * BigDecimal::exp(num.re);
}

/*!
    Calculates \a num raised in \a power (num^power).

    pow(num, power) = exp(power * ln(num))
*/
Complex Complex::pow(const Complex & num, const Complex & power)
{
    if (num.im.isZero() && power.im.isZero() && num.re >= 0 && power.re >= 0) {
        return BigDecimal::pow(num.re, power.re);
    }
    // for pow(0, x) - ln(0) will return negative infinity
    if (num.isZero()) {
        return 0;
    }
    return exp(power * ln(num));    
}

/*!
    Calculates square root of \a num.

    sqrt(num) = pow(num, 0.5)
*/
Complex Complex::sqrt(const Complex & num)
{
    return pow(num, Complex("0.5"));
}

/*!
    Calculates sine of \a num.

    sin(num) = (exp(i*num) - exp(-i*num)) / 2
*/
Complex Complex::sin(const Complex & num)
{
    if (num.im.isZero()) {
        return BigDecimal::sin(num.re);
    }
    return (exp(i * num) - exp(-i * num)) / (i * 2);
}

/*!
    Calculates cosine of \a num.

    cos(num) = (exp(i*num) + exp(-i*num)) / 2
*/
Complex Complex::cos(const Complex & num)
{
    if (num.im.isZero()) {
        return BigDecimal::cos(num.re);
    }
    return (exp(i * num) + exp(-i * num)) / 2;
}

/*!
    Calculates tangent of \a num.

    tan(num) = sin(num) / cos(num)

    \exception InvalidArgumentException cos(num) == 0
*/
Complex Complex::tan(const Complex & num)
{
    Complex cosine = cos(num);
    if (cosine.isZero()) {
        throw InvalidArgumentException(_T("tan"),
            InvalidArgumentException::TANGENT_FUNCTION);
    }
    return sin(num) / cosine;
}

/*!
    Calculates cotangent of \a num.

    cot(num) = cos(num) / sin(num)

    \exception InvalidArgumentException sin(num) == 0
*/
Complex Complex::cot(const Complex & num)
{
    Complex sine = sin(num);
    if (sine.isZero()) {
        throw InvalidArgumentException(_T("cot"),
            InvalidArgumentException::COTANGENT_FUNCTION);
    }
    return cos(num) / sine;
}

/*!
    Calculates arcsine of \a num.

    arcsin(num) = -i * ln(i * num + sqrt(1 - num^2))
*/
Complex Complex::arcsin(const Complex & num)
{
    if (num.im.isZero() && num.re >= -1 && num.re <= 1) {
        return BigDecimal::arcsin(num.re);
    }
    return -i * ln(i * num + sqrt(Complex(1) - num * num));
}

/*!
    Calculates arccosine of \a num.

    arccos(num) = -i * ln(num + i * sqrt(1 - num^2))
*/
Complex Complex::arccos(const Complex & num)
{
    if (num.im.isZero() && num.re >= -1 && num.re <= 1) {
        return BigDecimal::arccos(num.re);
    }
    return -i * ln(num + i * sqrt(Complex(1) - num * num));
}

/*!
    Calculates arctangent of \a num.

    arctan(num) = (i / 2) * ln((i + num) / (i - num))
*/
Complex Complex::arctan(const Complex & num)
{
    if (num.im.isZero()) {
        return BigDecimal::arctan(num.re);
    }
    return (i / 2) * ln((i + num) / (i - num));
}

/*!
    Calculates arccotangent of \a num.

    arccot(a) = (i / 2) * ln((num - i) / (num + i))
*/
Complex Complex::arccot(const Complex & num)
{
    if (num.im.isZero()) {
        return BigDecimal::arccot(num.re);
    }
    return (i / 2) * ln((num - i) / (num + i));
}

/*!
    Calculates hyperbolical sine of \a num.

    sinh(num) = (exp(num) - exp(-num)) / 2
*/
Complex Complex::sinh(const Complex & num)
{
    return (exp(num) - exp(-num)) / 2;
}

/*!
    Calculates hyperbolical cosine of \a num.

    cosh(num) = (exp(num) + exp(-num)) / 2
*/
Complex Complex::cosh(const Complex & num)
{
    return (exp(num) + exp(-num)) / 2;
}

/*!
    Calculates hyperbolical tangent of \a num.

    tanh(num) = sinh(num) / cosh(num)

    \exception InvalidArgumentException cosh(num) == 0
*/
Complex Complex::tanh(const Complex & num)
{
    Complex cosineh = cosh(num);
    if (cosineh.isZero()) {
        throw InvalidArgumentException(_T("tanh"),
            InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION);
    }
    return sinh(num) / cosineh;
}

/*!
    Calculates hyperbolical cotangent of \a num.

    coth(num) = cosh(num) / sinh(num)

    \exception InvalidArgumentException sinh(num) == 0
*/
Complex Complex::coth(const Complex & num)
{
    Complex sineh = sinh(num);
    if (sineh.isZero()) {
        throw InvalidArgumentException(_T("coth"),
            InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION);
    }
    return cosh(num) / sineh;
}

/*!
    Calculates hyperbolical arcsine of \a num.

    arcsinh(num) = ln(num + sqrt(num^2 + 1))
*/
Complex Complex::arcsinh(const Complex & num)
{
    return ln(num + sqrt(num * num + 1));
}

/*!
    Calculates hyperbolical arccosine of \a num.

    arccosh(num) = ln(num + sqrt(num^2 - 1))
*/
Complex Complex::arccosh(const Complex & num)
{
    return ln(num + sqrt(num * num - 1));
}

/*!
    Calculates hyperbolical arctangent of \a num.

    arctanh(num) = ln((1 + num) / (1 - num)) / 2
*/
Complex Complex::arctanh(const Complex & num)
{
    return ln((num + 1) / (-num + 1)) / 2;
}

/*!
    Calculates hyperbolical arccotangent of \a num.

    arccoth(num) = ln((num + 1) / (num - 1)) / 2
*/
Complex Complex::arccoth(const Complex & num)
{
    return ln((num + 1) / (num - 1)) / 2;
}


