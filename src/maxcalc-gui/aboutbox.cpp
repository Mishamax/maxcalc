/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2008 Michael Maximov (michael.maximov@gmail.com)
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
#include "version.h"
// Qt
#include <QSpacerItem>
#include <QPalette>


AboutBox::AboutBox(QWidget * parent) : QDialog(parent)
{
	setWindowTitle("MaxCalc");

	MaxCalcEngine::tstring labelText;
	labelText = _T("MaxCalc v");
	labelText += MaxCalcEngine::VERSION;
	labelText += _T(" (");
	if (MaxCalcEngine::VERSION_LABEL[0] != 0)
	{
		labelText += MaxCalcEngine::VERSION_LABEL;
		labelText += _T(", ");
	}
	labelText += _T("built: ");
	MaxCalcEngine::tstring date;
	MaxCalcEngine::stringToWideString(__DATE__, date);
	labelText += date;
	labelText += _T(")<br>");
	labelText += MaxCalcEngine::COPYRIGHT;
	labelText += _T("<br><a href='");
	labelText += MaxCalcEngine::WEBSITE;
	labelText += _T("'>");
	labelText += MaxCalcEngine::WEBSITE;
	labelText += _T("</a>");

	label.setHtml(QString::fromStdWString(labelText));
	label.setOpenExternalLinks(true);
	label.setFrameStyle(QFrame::NoFrame);
	QPalette p;
	p.setColor(QPalette::Base, p.color(QPalette::Background));
	label.setPalette(p);
	label.setLineWrapMode(QTextEdit::NoWrap);
	label.document()->adjustSize();
	label.setMaximumSize(label.document()->size().toSize());

	closeButton.setText(tr("&Close"));

    layout.addWidget(&label, 1, 0, 1, -1);
    layout.addItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed), 2, 1, 1, 1);
    layout.addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding), 3, 0, 1, 1);
    layout.addWidget(&closeButton, 3, 1, 1, 1);
    layout.addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding), 3, 2, 1, 1);    

	setLayout(&layout);

	setMaximumSize(sizeHint());
	setMinimumSize(sizeHint());

	connect(&closeButton, SIGNAL(clicked()), this, SLOT(close()));
}
