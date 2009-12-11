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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Local
#include "inputbox.h"
// MaxCalcEngine
#include "parser.h"
#include "parsercontext.h"
// Qt
#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMenuBar>
#include <QMenu>
#include <QDockWidget>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

signals:
	/// Emitted when expression is calculated by parser.
	void expressionCalculated();

private:
	QWidget centralWidget;
	QVBoxLayout layout;
	QHBoxLayout bottomLayout;
	InputBox inputBox;
	QTextEdit historyBox;
	QPushButton okButton;
	QListWidget * variablesList;
	QDockWidget variablesListWrapper;
	QListWidget * functionsList;
	QDockWidget functionsListWrapper;
	QMenuBar mainMenu;

	MaxCalcEngine::Parser parser;

	void initUi();
	void initMainMenu();
	void updateVariablesList();
	void initFunctionsList();
	void outputError(QString message);

private slots:
	void onExpressionEntered();
	void onVariableClicked(QListWidgetItem * item);
	void onFunctionClicked(QListWidgetItem * item);
	void onHelpAbout();
    void onHelpWebSite();
    void onHelpReportIssue();
    void onDeleteAllVariables();
	void onUnitConversion(const QString & conversion);
};

#endif // MAINWINDOW_H
