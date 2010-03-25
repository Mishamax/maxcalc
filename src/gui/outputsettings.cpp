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
#include "outputsettings.h"
// Engine
#include "parsercontext.h"
#include "constants.h"
// Qt
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGridLayout>


/*!
    \class OutputSettings
    \brief Output settings dialog.

    \ingroup MaxCalcGui
*/

/*!
    Constructs a new output settings dialog on top of given \a parent window.
*/
OutputSettings::OutputSettings(QWidget * parent, ParserContext & context)
    : QDialog(parent), mContext(context)
{
    setWindowTitle("Output Format");

    QVBoxLayout * vlayout = new QVBoxLayout(this);
    setLayout(vlayout);

    QHBoxLayout * hlayout = new QHBoxLayout();
    vlayout->addLayout(hlayout);

    QLabel * label1 = new QLabel(tr("Precision:"));
    QLabel * label2 = new QLabel(tr("decimal digits (1..50)"));
    mLineEdit = new QLineEdit(QString().setNum(context.numberFormat().precision));
    hlayout->addWidget(label1);
    hlayout->addWidget(mLineEdit);
    hlayout->addWidget(label2);

    QGridLayout * glayout = new QGridLayout();
    vlayout->addLayout(glayout);

    mRadioDot = new QRadioButton(tr("Dot"));
    mRadioComma = new QRadioButton(tr("Comma"));
    QButtonGroup * group1 = new QButtonGroup(this);
    if (context.numberFormat().decimalSeparator == ComplexFormat::DOT_SEPARATOR) {
        mRadioDot->setChecked(true);
    } else {
        mRadioComma->setChecked(true);
    }
    group1->addButton(mRadioDot);
    group1->addButton(mRadioComma);
    glayout->addWidget(new QLabel(tr("Decimal separator:")), 1, 0);
    glayout->addWidget(mRadioDot, 1, 1);
    glayout->addWidget(mRadioComma, 1, 2);

    mRadioI = new QRadioButton(tr("i"));
    mRadioJ = new QRadioButton(tr("j"));
    QButtonGroup * group2 = new QButtonGroup(this);
    if (context.numberFormat().imaginaryOne == ComplexFormat::IMAGINARY_ONE_I) {
        mRadioI->setChecked(true);
    } else {
        mRadioJ->setChecked(true);
    }
    group2->addButton(mRadioI);
    group2->addButton(mRadioJ);
    glayout->addWidget(new QLabel(tr("Imaginary one:")), 2, 0);
    glayout->addWidget(mRadioI, 2, 1);
    glayout->addWidget(mRadioJ, 2, 2);

    vlayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
        QSizePolicy::Fixed));

    hlayout = new QHBoxLayout();
    QPushButton * buttonOk = new QPushButton(tr("&OK"));
    QPushButton * buttonDefaults = new QPushButton(tr("&Defaults"));
    hlayout->addItem(new QSpacerItem(10, 30, QSizePolicy::Fixed,
        QSizePolicy::Minimum));
    hlayout->addWidget(buttonOk);
    hlayout->addWidget(buttonDefaults);
    hlayout->addItem(new QSpacerItem(10, 30, QSizePolicy::Fixed,
        QSizePolicy::Minimum));
    vlayout->addLayout(hlayout);

    mLineEdit->setMaxLength(2);
    mLineEdit->setInputMask("09");
    mLineEdit->setMaximumWidth(glayout->sizeHint().width() -
                              label1->sizeHint().width() -
                              label2->sizeHint().width());

    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(buttonOk, SIGNAL(clicked()), parent, SLOT(updateVariablesList()));
    connect(buttonDefaults, SIGNAL(clicked()), this, SLOT(onDefaultsButtonClicked()));
}

void OutputSettings::onOkButtonClicked()
{
    bool ok;
    int precision = mLineEdit->text().toInt(&ok);
    if (!ok || precision < 1 || precision > Constants::MAX_IO_PRECISION) {
        precision = Constants::DEFAULT_IO_PRECISION;
    }
    mContext.numberFormat().precision = precision;

    mContext.numberFormat().decimalSeparator = mRadioDot->isChecked() ?
                                               ComplexFormat::DOT_SEPARATOR :
                                               ComplexFormat::COMMA_SEPARATOR;

    mContext.numberFormat().imaginaryOne = mRadioI->isChecked() ?
                                           ComplexFormat::IMAGINARY_ONE_I :
                                           ComplexFormat::IMAGINARY_ONE_J;

    close();
}

void OutputSettings::onDefaultsButtonClicked()
{
    mLineEdit->setText(QString().setNum(Constants::DEFAULT_IO_PRECISION));
    mRadioDot->setChecked(true);
    mRadioI->setChecked(true);
}
