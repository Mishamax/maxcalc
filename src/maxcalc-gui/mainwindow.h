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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QListView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>

class MainWindow : public QWidget
{
public:
	MainWindow();

private:
	QGridLayout layout;
	QHBoxLayout bottomLayout;
	QLineEdit inputBox;
	QTextEdit historyBox;
	QListView variablesList;
	QPushButton okButton;
};

#endif // MAINWINDOW_H
