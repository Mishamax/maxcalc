
#ifndef COMPLEX_H
#define COMPLEX_H

#include "bigdecimal.h"
#include "exception.h"
#include <QString>



static const ComplexFormat defaultComplexFormat = ComplexFormat();

class Complex
{
public:
	// Real and imaginary parts of complex number
	BigDecimal re, im;

	// Constructors
	Complex();
	Complex(const QString & real, const QString & imaginary = "0");
	Complex(const char * real, const char * imaginary = "0");
	Complex(const BigDecimal & real, const BigDecimal & imaginary = 0);
	Complex(const int real, const int imaginary = 0);
	Complex(const unsigned real, const unsigned imaginary = 0);

	// Conversion to string
	QString toString(const ComplexFormat & format = ComplexFormat::getDefault()) const;

	// Operators
	Complex operator+() const;
	Complex operator-() const;

	Complex operator+(const Complex & num) const;
	Complex operator-(const Complex & num) const;
	Complex operator*(const Complex & num) const;
	Complex operator/(const Complex & num) const;

	bool operator==(const Complex & num) const;
	bool operator!=(const Complex & num) const;

	// Math functions
	static BigDecimal sqr(const Complex & num);
	static BigDecimal abs(const Complex & num);

	// Exception classes
	class ComplexException : public Exception {};
	class DivisionByZeroException : public Exception {};
};

#endif
