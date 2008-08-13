
#ifndef COMPLEX_H
#define COMPLEX_H

#include "bigdecimal.h"
#include <QString>

class Complex
{
public:
	BigDecimal re, im;

	Complex();
	Complex(const QString & real, const QString & imaginary = "0");
	Complex(const char * real, const char * imaginary = "0");
	Complex(const BigDecimal & real, const BigDecimal & imaginary = 0);
	Complex(const int real, const int imaginary = 0);
	Complex(const unsigned real, const unsigned imaginary = 0);

	QString toString(bool engineeringFormat = false, const int digitsAfterDecimalPoint = -1) const;

	Complex operator+() const;
	Complex operator-() const;

	Complex operator+(const Complex & num) const;
	Complex operator-(const Complex & num) const;
	Complex operator*(const Complex & num) const;
	Complex operator/(const Complex & num) const;

	bool operator==(const Complex & num) const;
	bool operator!=(const Complex & num) const;
};

#endif
