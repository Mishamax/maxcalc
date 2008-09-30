
#ifndef COMPLEX_H
#define COMPLEX_H

#include "bigdecimal.h"
#include "complexformat.h"

#include <exception>
#include <string>

class Complex
{
public:
	// Real and imaginary parts of complex number
	BigDecimal re, im;

	// Constructors
	Complex();
	Complex(const char * real, const char * imaginary = "0");
	Complex(const std::string & real, const std::string & imaginary = "0");
#if defined(UNICODE)
	Complex(const wchar_t * real, const wchar_t * imaginary = L"0");
	Complex(const std::wstring & real, const std::wstring & imaginary = L"0");
#endif
	Complex(const BigDecimal & real, const BigDecimal & imaginary = 0);
	Complex(const int real, const int imaginary = 0);
	Complex(const unsigned real, const unsigned imaginary = 0);

	// Conversion to string
	std::string toString(const ComplexFormat & format = ComplexFormat::getDefault()) const;
#if defined(UNICODE)
	std::wstring toWideString(const ComplexFormat & format = ComplexFormat::getDefault()) const;
#endif

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
	class ComplexException : public std::exception {};
	class DivisionByZeroException : public ComplexException {};
};

#endif
