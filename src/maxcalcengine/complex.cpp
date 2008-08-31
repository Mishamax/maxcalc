
#include "complex.h"

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

QString Complex::toString(const ComplexFormat & format) const
{
	QString rePart = re.toString(format);
	QString imPart = BigDecimal::abs(im).toString(format);
	bool isReZero = re.isZero();
	bool isImZero = im.isZero();
	bool isImNegative = im.isNegative();

	QString result;

	if (!isReZero)
	{
		if (rePart.contains('e', Qt::CaseInsensitive))
			result = '(' + rePart + ')';
		else
			result = rePart;
	}

	if (!isImZero)
	{
		if (!isReZero)
		{
			if (format.spacesAroundSignes)
				result += ' ';

			if (isImNegative)
				result += '-';
			else
				result += '+';

			if (format.spacesAroundSignes)
				result += ' ';
		}

		if (format.iBeforeImaginaryPart && format.multiplyBetweenIAndImaginaryPart)
			result += "i*";
		else if (format.iBeforeImaginaryPart)
			result += 'i';

		if (imPart.contains('e', Qt::CaseInsensitive))
			result += '(' + imPart + ')';
		else
			result += imPart;

		if (!format.iBeforeImaginaryPart && format.multiplyBetweenIAndImaginaryPart)
			result += "*i";
		else if (!format.iBeforeImaginaryPart)
			result += 'i';
	}

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
