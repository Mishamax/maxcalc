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

#include "mainwindow.h"

MainWindow::MainWindow() : QWidget()
{
	setWindowTitle(tr("MaxCalc"));
	setMinimumSize(400, 300);
	resize(600, 400);

	variablesList.setMaximumWidth(150);
	variablesList.setMinimumWidth(150);
	layout.setSpacing(3);

	layout.addWidget(&variablesList, 0, 0);
	layout.addWidget(&historyBox, 0, 1);

	okButton.setText(tr("OK"));
	okButton.setMinimumWidth(30);
	okButton.setMaximumWidth(30);

	bottomLayout.addWidget(&inputBox);
	bottomLayout.addWidget(&okButton);

	layout.addLayout(&bottomLayout, 1, 0, 1, 2);
	
	setLayout(&layout);
}
