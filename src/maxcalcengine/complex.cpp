
#include "complex.h"

Complex::Complex()
{
	re = 0;
	im = 0;
}

Complex::Complex(const char * real, const char * imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const std::string & real, const std::string & imaginary)
{
	re = real;
	im = imaginary;
}

#if defined(UNICODE)

Complex::Complex(const wchar_t * real, const wchar_t * imaginary)
{
	re = real;
	im = imaginary;
}

Complex::Complex(const std::wstring & real, const std::wstring & imaginary)
{
	re = real;
	im = imaginary;
}

#endif // #if defined(UNICODE)

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

std::string Complex::toString(const ComplexFormat & format) const
{
	std::string rePart = re.toString(format);
	std::string imPart = BigDecimal::abs(im).toString(format);
	bool isReZero = re.isZero();
	bool isImZero = im.isZero();
	bool isImNegative = im.isNegative();

	std::string result = "0";

	if (!isReZero)
		result = rePart;

	if (!isImZero)
	{
		if (!isReZero)
		{
			if (isImNegative)
				result += '-';
			else
				result += '+';
		}

		result += imPart + format.imaginaryOne();
	}

	return result;
}

#if defined(UNICODE)

std::wstring Complex::toWideString(const ComplexFormat & format) const
{
	const std::string str = toString(format);
	std::wstring wstr;
	stringToWideString(str, wstr);
	return wstr;
}

#endif // #if defined(UNICODE)

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
	if (sqrt.isZero())
		throw DivisionByZeroException();
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

BigDecimal Complex::sqr(const Complex & num)
{
	// z^2 = |z|^2 = Re^2(z) + Im^2(z)
	return (num.re * num.re + num.im * num.im);
}

BigDecimal Complex::abs(const Complex & num)
{
	return BigDecimal::sqrt(sqr(num));
}
