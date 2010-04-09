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
#include "aboutbox.h"
// MaxCalc Engine
#include "constants.h"
#include "unicode.h"
// Qt
#include <QSpacerItem>
#include <QPalette>
#include <QTextBrowser>
#include <QPushButton>
#include <QGridLayout>

/*!
    \class AboutBox
    \brief About box based on QDialog.

    \ingroup MaxCalcGui
*/


/*!
    Constructs a new about box on top of given \a parent window.
*/
AboutBox::AboutBox(QWidget * parent) : QDialog(parent)
{
    setWindowTitle("About MaxCalc");

    tstring labelText = _T("MaxCalc v");
    labelText += Constants::VERSION;
    labelText += _T(" (");
    labelText += _T("built: ");
    tstring date = stringToWideString(__DATE__);
    labelText += date;
    labelText += _T(")<br>");
    labelText += Constants::COPYRIGHT;
    labelText += _T("<br><a href='");
    labelText += Constants::WEBSITE;
    labelText += _T("'>");
    labelText += Constants::WEBSITE;
    labelText += _T("</a>");

    QTextBrowser * label = new QTextBrowser;
    label->setHtml(QString::fromWCharArray(labelText.c_str()));
    label->setOpenExternalLinks(true);
    label->setFrameStyle(QFrame::NoFrame);
    QPalette p;
    p.setColor(QPalette::Base, p.color(QPalette::Background));
    label->setPalette(p);
    label->setLineWrapMode(QTextEdit::NoWrap);
    label->document()->adjustSize();
    label->setMinimumSize(label->document()->size().toSize());
    label->setMaximumSize(label->document()->size().toSize());

    QPushButton * closeButton = new QPushButton;
    closeButton->setText(tr("&Close"));

    QGridLayout * layout = new QGridLayout;
    layout->addWidget(label, 1, 0, 1, -1);
    layout->addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum,
        QSizePolicy::Fixed), 2, 1, 1, 1);
    layout->addItem(new QSpacerItem(20, 20,
        QSizePolicy::Expanding), 3, 0, 1, 1);
    layout->addWidget(closeButton, 3, 1, 1, 1);
    layout->addItem(new QSpacerItem(20, 20,
        QSizePolicy::Expanding), 3, 2, 1, 1);    

    setLayout(layout);

    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}
