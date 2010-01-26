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

#ifndef BIGDECIMALTEST_H
#define BIGDECIMALTEST_H

// Qt
#include <QTest>

class BigDecimalTest : public QObject
{
    Q_OBJECT

private slots:
    // BigDecimalFormat tests
    void bigDecimalFormatDefault();

    // Constructors
    void fromString();
    void fromCharStr();
    void fromWideString();
    void fromWideCharStr();
    void fromBigDecimal();
    void fromInt();
    void fromUInt();
    void fromDouble();

    // Conversions
    void toString();
    void toWideString();
    void toInt();
    void toUInt();

    // Operators
    void unaryOperators();
    void binaryArithmeticOperators();
    void binaryLogicalOperators();
    void comparisonOperators();

    // Functions
    void round();
    void integer();
    void fractional();
    void floor();
    void ceil();
    void abs();
    void exp();
    void ln();
    void log10();
    void sqr();
    void sqrt();
    void pow();
    void div();
    void max();
    void min();
    void factorial();
    void sin();
    void cos();
    void tan();
    void cot();
    void arcsin();
    void arccos();
    void arctan();
    void arccot();

    // Misc
    void consts();
};

#endif // BIGDECIMALTEST_H
