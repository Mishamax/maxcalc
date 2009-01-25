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

// Local
#include "mainwindow.h"
#include "aboutbox.h"
// MaxCalcEngine
#include "bigdecimal.h"

/// Indentation used for output
static const QString indent = "    ";

/*!
	\class MainWindow
	\brief Main window of calculator.

	\ingroup MaxCalcGui
*/

/*!
	Constructs a new main window.
*/
MainWindow::MainWindow() : QMainWindow()
{
	initUi();
	initMainMenu();
	updateVariablesList();
	initFunctionsList();
}

/*!
	Inits UI elements.
*/
void MainWindow::initUi()
{
	setWindowTitle(tr("MaxCalc"));
	setMinimumSize(500, 350);
	resize(750, 550);

	setCentralWidget(&centralWidget);

	variablesList.setMaximumWidth(150);
	variablesList.setMinimumWidth(150);

	functionsList.setMaximumWidth(100);
	functionsList.setMinimumWidth(100);

	QFont font = historyBox.currentFont();
	font.setPixelSize(12);
	historyBox.setCurrentFont(font);
	historyBox.setReadOnly(true);

	font = inputBox.font();
	font.setPixelSize(16);
	inputBox.setFont(font);

	font = okButton.font();
	font.setPixelSize(14);
	okButton.setFont(font);
	okButton.setText(tr("OK"));
	okButton.setMinimumWidth(30);
	okButton.setMaximumWidth(30);

	bottomLayout.addWidget(&inputBox);
	bottomLayout.addWidget(&okButton);

	layout.setSpacing(3);
	layout.addWidget(&variablesList, 0, 0);
	layout.addWidget(&historyBox, 0, 1);
	layout.addWidget(&functionsList, 0, 2);
	layout.addLayout(&bottomLayout, 1, 0, 1, 3);
	
	centralWidget.setLayout(&layout);

	QObject::connect(this, SIGNAL(expressionCalculated()), &inputBox,
		SLOT(addTextToHistory()));
	QObject::connect(&okButton, SIGNAL(clicked()), this,
		SLOT(onExpressionEntered()));
	QObject::connect(&inputBox, SIGNAL(returnPressed()), this,
		SLOT(onExpressionEntered()));
	QObject::connect(&variablesList, SIGNAL(itemActivated(QListWidgetItem *)),
		this, SLOT(onVariableClicked(QListWidgetItem *)));
	QObject::connect(&functionsList, SIGNAL(itemActivated(QListWidgetItem *)),
		this, SLOT(onFunctionClicked(QListWidgetItem *)));

	inputBox.setFocus();
}

/*!
	Creates main menu.
*/
void MainWindow::initMainMenu()
{
	setMenuBar(&mainMenu);

	QMenu * file = mainMenu.addMenu(tr("&File"));
	file->addAction(tr("&Exit"), this, SLOT(close()));

	QMenu * commands = mainMenu.addMenu(tr("&Commands"));
	commands->addAction(tr("&Clear history"), &historyBox, SLOT(clear()));
	commands->addAction(tr("&Delete all variables"), this, SLOT(onDeleteAllVariables()));

	QMenu * help = mainMenu.addMenu(tr("&Help"));
	help->addAction(tr("&About"), this, SLOT(onHelpAbout()));
}

/*!
	Adds constants to varibables list.
*/
void MainWindow::updateVariablesList()
{
	variablesList.clear();

	variablesList.addItem(tr("e = ") +
		QString::fromStdWString(MaxCalcEngine::BigDecimal::E.toWideString()));
	variablesList.addItem(tr("pi = ") +
		QString::fromStdWString(MaxCalcEngine::BigDecimal::PI.toWideString()));

	if (parser.context().resultExists())
	{
		variablesList.addItem(tr("res = ") +
			QString::fromStdWString(parser.context().result().toWideString()));
	}

	MaxCalcEngine::Variables::const_iterator iter;
	for (iter = parser.context().variables().begin();
		iter != parser.context().variables().end(); ++iter)
	{
		variablesList.addItem(QString::fromStdWString(iter->name) + " = " +
			QString::fromStdWString(iter->value.toWideString()));
	}
}

/*!
	Adds supported functions to functions list.
*/
void MainWindow::initFunctionsList()
{
	functionsList.addItem(tr("abs"));
	functionsList.addItem(tr("sqr"));
	functionsList.addItem(tr("sqrt"));
	functionsList.addItem(tr("pow"));
	functionsList.addItem(tr("fact"));
	functionsList.addItem(tr("sin"));
	functionsList.addItem(tr("cos"));
	functionsList.addItem(tr("tan"));
	functionsList.addItem(tr("cot"));
	functionsList.addItem(tr("asin"));
	functionsList.addItem(tr("acos"));
	functionsList.addItem(tr("atan"));
	functionsList.addItem(tr("acot"));
	functionsList.addItem(tr("sinh"));
	functionsList.addItem(tr("cosh"));
	functionsList.addItem(tr("tanh"));
	functionsList.addItem(tr("coth"));
	functionsList.addItem(tr("asinh"));
	functionsList.addItem(tr("acosh"));
	functionsList.addItem(tr("atanh"));
	functionsList.addItem(tr("acoth"));
	functionsList.addItem(tr("ln"));
	functionsList.addItem(tr("log2"));
	functionsList.addItem(tr("log10"));
	functionsList.addItem(tr("exp"));
}

/*!
	Called when expression is entered.
*/
void MainWindow::onExpressionEntered()
{
	QString expr = inputBox.text();
	expr = expr.trimmed();

	if (expr.isEmpty())
		return;

	parser.setExpression(expr.toStdWString());

	try
	{
		parser.parse();
	}
	catch (std::exception)
	{
		historyBox.setTextColor(Qt::blue);
		historyBox.append(inputBox.text());
		historyBox.setTextColor(Qt::red);
		historyBox.append(indent + tr("Error"));
		inputBox.selectAll();
		inputBox.setFocus();
		return;
	}

	emit expressionCalculated();

	historyBox.setTextColor(Qt::blue);
	historyBox.append(inputBox.text());
	historyBox.setTextColor(Qt::darkGreen);
	historyBox.append(indent +
		QString::fromStdWString(parser.context().result().toWideString()));
	inputBox.clear();
	inputBox.setFocus();
	updateVariablesList();
}

/*!
	Called when variable in the list is clicked.
*/
void MainWindow::onVariableClicked(QListWidgetItem * item)
{
	QString text = item->text();
	int equalIndex = text.indexOf(" = ");
	text.remove(equalIndex, text.length() - equalIndex);
	inputBox.insert(text);
	inputBox.setFocus();
}

/*!
	Called when function in the list is clicked.
*/
void MainWindow::onFunctionClicked(QListWidgetItem * item)
{
	inputBox.insert(item->text());
	inputBox.insert("()");
	inputBox.setCursorPosition(inputBox.cursorPosition() - 1);
	inputBox.setFocus();
}

/*!
	Help -> About command.
*/
void MainWindow::onHelpAbout()
{
	AboutBox aboutBox(this);
	aboutBox.exec();
}

/*!
	Commands -> Delete all variables command.
*/
void MainWindow::onDeleteAllVariables()
{
	parser.context().variables().removeAll();
	updateVariablesList();
}
