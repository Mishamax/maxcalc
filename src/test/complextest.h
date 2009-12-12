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

#ifndef COMPLEXTEST_H
#define COMPLEXTEST_H

// Qt
#include <QTest>

class ComplexTest : public QObject
{
    Q_OBJECT

private slots:
    // ComplexFormat tests
    void complexFormatDefault();
    void complexFormatCustom();
    void complexFormatAccessors();

    // Constructors
    void fromString();
    void fromCharStr();
    void fromWideString();
    void fromWideCharStr();
    void fromBigDecimal();
    void fromInt();
    void fromUInt();

    // Conversions
    void toString();
    void toWideString();

    // Operators
    void unaryOperators();
    void binaryOperators();
    void comparisonOperators();

    // Misc functions
    void isZero();

    // Math functions
    void sqr();
    void abs();
    void arg();
    void ln();
};

#endif // COMPLEXTEST_H
