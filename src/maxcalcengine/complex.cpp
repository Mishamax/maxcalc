
#include "complex.h"
#include <iostream>

using namespace std;

Complex::Complex()
{
	re = 0;
	im = 0;
}

Complex::Complex(const QString & real, const QString & imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const char * real, const char * imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const BigDecimal & real, const BigDecimal & imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const int real, const int imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const unsigned real, const unsigned imaginary)
{
	re = real;
	im = imaginary;
}

QString Complex::toString(bool engineeringFormat, const int digitsAfterDecimalPoint) const
{
	QString result = "(" + re.toString(engineeringFormat, digitsAfterDecimalPoint) +	")";
	if (im.isNegative())
		result += "- i*(" + (-im).toString(engineeringFormat, digitsAfterDecimalPoint) + ")";
	else
		result += "+ i*(" + im.toString(engineeringFormat, digitsAfterDecimalPoint) + ")";
	return result;
}

Complex Complex::operator+() const
{
	return *this;
}

Complex Complex::operator-() const
{
	return Complex(-re, -im);
}

Complex Complex::operator+(const Complex & num) const
{
	return Complex(re + num.re, im + num.im);
}

Complex Complex::operator-(const Complex & num) const
{
	return Complex(re - num.re, im - num.im);
}

Complex Complex::operator*(const Complex & num) const
{
	return Complex(re * num.re - im * num.im, re * num.im + im * num.re);
}

Complex Complex::operator/(const Complex & num) const
{
	BigDecimal sqrt = re * num.re + im * num.im;
	return Complex((re * num.re + im * num.im) / sqrt, (im * num.re - re * num.im) / sqrt);
}

bool Complex::operator==(const Complex & num) const
{
	return (re == num.re) && (im == num.im);
}

bool Complex::operator!=(const Complex & num) const
{
	return (re != num.re) || (im != num.im);
}
