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
#include "mainwindow.h"
// MaxCalcEngine
#include "bigdecimal.h"

MainWindow::MainWindow() : QWidget()
{
	initUi();
	initVariablesList();
	initFunctionsList();
}

void MainWindow::initUi()
{
	setWindowTitle(tr("MaxCalc"));
	setMinimumSize(400, 300);
	resize(600, 400);

	variablesList.setMaximumWidth(150);
	variablesList.setMinimumWidth(150);

	functionsList.setMaximumWidth(100);
	functionsList.setMinimumWidth(100);

	QFont font = historyBox.currentFont();
	font.setPixelSize(12);
	historyBox.setCurrentFont(font);
	historyBox.setReadOnly(true);

	font = inputBox.font();
	font.setPixelSize(14);
	inputBox.setFont(font);

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
	
	setLayout(&layout);

	QObject::connect(&okButton, SIGNAL(clicked()), this, SLOT(onExpressionEntered()));
	QObject::connect(&inputBox, SIGNAL(returnPressed()), this, SLOT(onExpressionEntered()));
	QObject::connect(&variablesList, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(onVariableClicked(QListWidgetItem *)));
	QObject::connect(&functionsList, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(onFunctionClicked(QListWidgetItem *)));

	inputBox.setFocus();
}

void MainWindow::initVariablesList()
{
	variablesList.addItem(tr("e = ") + QString::fromStdWString(MaxCalcEngine::BigDecimal::E.toWideString()));
	variablesList.addItem(tr("pi = ") + QString::fromStdWString(MaxCalcEngine::BigDecimal::PI.toWideString()));
}

void MainWindow::initFunctionsList()
{
	functionsList.addItem(tr("abs"));
	functionsList.addItem(tr("sqr"));
	functionsList.addItem(tr("sqrt"));
	functionsList.addItem(tr("pow"));
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

void MainWindow::onExpressionEntered()
{
	parser.setExpression(inputBox.text().toStdWString());

	try
	{
		parser.parse();
	}
	catch (std::exception)
	{
		historyBox.setTextColor(Qt::blue);
		historyBox.append(inputBox.text());
		historyBox.setTextColor(Qt::red);
		historyBox.append(tr("    Error"));
		inputBox.selectAll();
		inputBox.setFocus();
		return;
	}

	historyBox.setTextColor(Qt::blue);
	historyBox.append(inputBox.text());
	historyBox.setTextColor(Qt::darkGreen);
	historyBox.append(QString("    ") + QString::fromStdWString(parser.context().result().toWideString()));
	if (variablesList.count() > 2)
		variablesList.takeItem(2);
	variablesList.addItem(tr("res = ") + QString::fromStdWString(parser.context().result().toWideString()));
	inputBox.clear();
}

void MainWindow::onVariableClicked(QListWidgetItem * item)
{
	QString text = item->text();
	int equalIndex = text.indexOf(" = ");
	text.remove(equalIndex, text.length() - equalIndex);
	inputBox.insert(text);
	inputBox.setFocus();
}

void MainWindow::onFunctionClicked(QListWidgetItem * item)
{
	inputBox.insert(item->text());
	inputBox.insert("()");
	inputBox.setCursorPosition(inputBox.cursorPosition() - 1);
	inputBox.setFocus();
}
