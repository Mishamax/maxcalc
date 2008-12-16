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

// Local
#include "inputbox.h"
// MaxCalcEngine
#include "parser.h"
// Qt
#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMenuBar>
#include <QMenu>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

signals:
	void expressionCalculated();

private:
	QWidget centralWidget;
	QGridLayout layout;
	QHBoxLayout bottomLayout;
	InputBox inputBox;
	QTextEdit historyBox;
	QPushButton okButton;
	QListWidget variablesList;
	QListWidget functionsList;
	QMenuBar mainMenu;

	MaxCalcEngine::Parser parser;

	void initUi();
	void initMainMenu();
	void initVariablesList();
	void initFunctionsList();

private slots:
	void onExpressionEntered();
	void onVariableClicked(QListWidgetItem * item);
	void onFunctionClicked(QListWidgetItem * item);
	void onHelpAbout();
};

#endif // MAINWINDOW_H
