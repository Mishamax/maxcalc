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
#include "myaction.h"
// MaxCalcEngine
#include "bigdecimal.h"
#include "unitconversion.h"

/// Indentation used for output
static const QString indent = "    ";

/*!
	\class MainWindow
	\brief Main window of calculator.

	\ingroup MaxCalcGui
*/

using namespace MaxCalcEngine;

/*!
	Constructs a new main window.
*/
MainWindow::MainWindow() : QMainWindow(),
		variablesListWrapper(tr("Variables"), this),
		functionsListWrapper(tr("Functions"), this)
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
	variablesList = new QListWidget();
	functionsList = new QListWidget();

	setWindowTitle(tr("MaxCalc"));
	setMinimumSize(500, 350);
	resize(750, 550);

	setCentralWidget(&centralWidget);

	QFont font = historyBox.currentFont();
	font.setPixelSize(12);
    historyBox.setFont(font);
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

	variablesListWrapper.setWidget(variablesList);
	functionsListWrapper.setWidget(functionsList);

	layout.addWidget(&historyBox);
	layout.addLayout(&bottomLayout);
	
	addDockWidget(Qt::RightDockWidgetArea, &variablesListWrapper);
	tabifyDockWidget(&variablesListWrapper, &functionsListWrapper);
	variablesListWrapper.raise();

	centralWidget.setLayout(&layout);

	QObject::connect(this, SIGNAL(expressionCalculated()), &inputBox,
		SLOT(addTextToHistory()));
	QObject::connect(&okButton, SIGNAL(clicked()), this,
		SLOT(onExpressionEntered()));
	QObject::connect(&inputBox, SIGNAL(returnPressed()), this,
		SLOT(onExpressionEntered()));
	QObject::connect(variablesList, SIGNAL(itemActivated(QListWidgetItem *)),
		this, SLOT(onVariableClicked(QListWidgetItem *)));
	QObject::connect(functionsList, SIGNAL(itemActivated(QListWidgetItem *)),
		this, SLOT(onFunctionClicked(QListWidgetItem *)));

	inputBox.setFocus();
}

MainWindow::~MainWindow()
{
	delete variablesList;
	delete functionsList;
}

/*!
	Creates main menu.
*/
void MainWindow::initMainMenu()
{
	setMenuBar(&mainMenu);

	// File menu

	QMenu * file = mainMenu.addMenu(tr("&File"));
	file->addAction(tr("&Exit"), this, SLOT(close()));

	// Commands menu

	QMenu * commands = mainMenu.addMenu(tr("&Commands"));
	commands->addAction(tr("&Clear history"), &historyBox, SLOT(clear()));
	commands->addAction(tr("&Delete all variables"), this,
						SLOT(onDeleteAllVariables()));

	// View menu

	QMenu * view = mainMenu.addMenu(tr("&View"));
	QAction * action = new QAction(tr("&Variables"), view);
	action->setCheckable(true);
	action->setChecked(true);
	connect(action, SIGNAL(toggled(bool)), &variablesListWrapper,
			SLOT(setVisible(bool)));
	view->addAction(action);
	action = new QAction(tr("&Functions"), view);
	action->setCheckable(true);
	action->setChecked(true);
	connect(action, SIGNAL(toggled(bool)), &functionsListWrapper,
			SLOT(setVisible(bool)));
	view->addAction(action);

	// Unit conversion menu

	QMenu * unitConversion = mainMenu.addMenu(tr("&Unit conversion"));
	QMenu * currentUnits = 0;
	QMenu * firstLevelMenu = 0;
    const UnitConversion::UnitDef * firstLevelCur = 0;

	UnitConversion::Type type = UnitConversion::NO_TYPE;
	const UnitConversion::UnitDef * cur;
	for (cur = UnitConversion::units(); cur->unit != UnitConversion::NO_UNIT; ++cur)
	{
		if (type != cur->type)
		{
			type = cur->type;
			firstLevelCur = cur;
			switch (type)
			{
			case UnitConversion::LENGTH:
				currentUnits = unitConversion->addMenu(tr("&Length"));
				break;
			case UnitConversion::WEIGHT:
				currentUnits = unitConversion->addMenu(tr("&Weight"));
				break;
			case UnitConversion::TIME:
				currentUnits = unitConversion->addMenu(tr("&Time"));
				break;
			case UnitConversion::SPEED:
				currentUnits = unitConversion->addMenu(tr("&Speed"));
				break;
			case UnitConversion::TEMPERATURE:
				currentUnits = unitConversion->addMenu(tr("T&emperature"));
				break;
			default:
				currentUnits = unitConversion->addMenu(tr("&Unknown units"));
				break;
			}
		}

        firstLevelMenu = currentUnits->addMenu(QString::fromWCharArray(
                cur->name.c_str()));
		const UnitConversion::UnitDef * cur2;
		for (cur2 = firstLevelCur; cur2->type == type; ++cur2)
		{
			if (cur2 == cur)
				continue;
			QString conversion = QString("[") +
								 firstLevelMenu->title() +
								 QString("->") +
                                 QString::fromWCharArray(cur2->name.c_str()) +
								 QString("]");
			MyAction * action = new MyAction(firstLevelMenu,
                                             QString::fromWCharArray(cur2->name.c_str()),
											 conversion, this,
											 SLOT(onUnitConversion(const QString &)));
			firstLevelMenu->addAction(action);
		}
	}

	// Help menu

	QMenu * help = mainMenu.addMenu(tr("&Help"));
	help->addAction(tr("&About"), this, SLOT(onHelpAbout()));
}

/*!
	Adds constants to varibables list.
*/
void MainWindow::updateVariablesList()
{
	variablesList->clear();

	variablesList->addItem(tr("e = ") +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::E.toWideString().c_str()));
	variablesList->addItem(tr("pi = ") +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::PI.toWideString().c_str()));

	if (parser.context().resultExists())
	{
		variablesList->addItem(tr("res = ") +
            QString::fromWCharArray(parser.context().result().toWideString().c_str()));
	}

	MaxCalcEngine::Variables::const_iterator iter;
	for (iter = parser.context().variables().begin();
		iter != parser.context().variables().end(); ++iter)
	{
        variablesList->addItem(QString::fromWCharArray(iter->name.c_str()) +
            " = " + QString::fromWCharArray(iter->value.toWideString().c_str()));
	}
}

/*!
	Adds supported functions to functions list.
*/
void MainWindow::initFunctionsList()
{
	functionsList->addItem(tr("abs"));
	functionsList->addItem(tr("sqr"));
	functionsList->addItem(tr("sqrt"));
	functionsList->addItem(tr("pow"));
	functionsList->addItem(tr("fact"));
	functionsList->addItem(tr("sin"));
	functionsList->addItem(tr("cos"));
	functionsList->addItem(tr("tan"));
	functionsList->addItem(tr("cot"));
	functionsList->addItem(tr("asin"));
	functionsList->addItem(tr("acos"));
	functionsList->addItem(tr("atan"));
	functionsList->addItem(tr("acot"));
	functionsList->addItem(tr("sinh"));
	functionsList->addItem(tr("cosh"));
	functionsList->addItem(tr("tanh"));
	functionsList->addItem(tr("coth"));
	functionsList->addItem(tr("asinh"));
	functionsList->addItem(tr("acosh"));
	functionsList->addItem(tr("atanh"));
	functionsList->addItem(tr("acoth"));
	functionsList->addItem(tr("ln"));
	functionsList->addItem(tr("log2"));
	functionsList->addItem(tr("log10"));
	functionsList->addItem(tr("exp"));
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

	wchar_t * str = new wchar_t[expr.length() + 1];
	expr.toWCharArray(str);
    str[expr.length()] = L'\0';
	parser.setExpression(str);

    historyBox.moveCursor(QTextCursor::End);
    historyBox.setTextColor(Qt::blue);
    historyBox.append(inputBox.text());

	try
	{
		parser.parse();

		// No error during parsing, output result (otherwise an exception will be caught)

		emit expressionCalculated();

		historyBox.setTextColor(Qt::darkGreen);
		historyBox.append(indent +
            QString::fromWCharArray(parser.context().result().toWideString().c_str()));
		inputBox.clear();
		inputBox.setFocus();
		updateVariablesList();
	}
	// Parser exceptions
	catch (ResultDoesNotExistException)
	{
		outputError(tr("No result of previous calculations"));
	}
	catch (UnknownTokenException & ex)
	{
		outputError(QString("Unknown token '%1' in expression")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
	}
    catch (IncorrectNumberException & ex)
	{
        QString msg = "Incorrect number";
        if (ex.what() != _T(""))
            msg += QString(" '%1'").arg(QString::fromWCharArray(ex.what().c_str()));
        outputError(msg);
	}
	catch (IncorrectExpressionException)
	{
		outputError(tr("Incorrect expression"));
	}
	catch (NoClosingBracketException)
	{
		outputError(tr("No closing bracket"));
	}
    catch (UnknownFunctionException & ex)
	{
        outputError(QString("Unknown function '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
	}
    catch (UnknownVariableException & ex)
	{
        outputError(QString("Unknown variable '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
	}
	catch (IncorrectVariableNameException)
	{
		outputError(tr("Incorrect name of variable"));
	}
	catch (IncorrectUnitConversionSyntaxException)
	{
		outputError(tr("Incorrect unit conversion syntax"));
	}
	catch (UnknownUnitConversionException)
	{
		outputError(tr("Unknown unit conversion"));
	}
	// Invalid argument exceptions
	catch (InvalidArgumentException & ex)
	{
        QString msg = QString("Invalid argument of function '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str()));
        switch (ex.reason())
        {
        case InvalidArgumentException::ZERO:
            msg += " (zero)";
            break;
        case InvalidArgumentException::NEGATIVE:
            msg += " (negative number)";
            break;
        case InvalidArgumentException::ZERO_OR_NEGATIVE:
            msg += " (zero or negative number)";
            break;
        case InvalidArgumentException::POWER_FUNCTION:
            msg += " (zero or negative number in negative degree)";
            break;
        case InvalidArgumentException::FACTORIAL_FUNCTION:
            msg += " (negative, fractional or complex number)";
            break;
        case InvalidArgumentException::TANGENT_FUNCTION:
            msg += " (cos(arg) = 0)";
            break;
        case InvalidArgumentException::COTANGENT_FUNCTION:
            msg += " (sin(arg) = 0)";
            break;
        case InvalidArgumentException::ARCSINE_FUNCTION:
        case InvalidArgumentException::ARCCOSINE_FUNCTION:
            msg += " (abs(arg) > 1)";
            break;
        case InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION:
            msg += " (cosh(arg) = 0)";
            break;
        case InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION:
            msg += " (sinh(arg) = 0)";
            break;
        case InvalidArgumentException::UNKNOWN:
        default:
            // Add nothing
            break;
        }
        outputError(msg);
    }
	catch (InvalidUnitConversionArgumentException & ex)
	{
		outputError(QString("Complex argument in unit conversion '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
	}
	// Arithmetic exception
	catch (ArithmeticException & ex)
	{
		QString reason;
		switch (ex.reason())
		{
		case ArithmeticException::DIVISION_BY_ZERO:
			reason = "Division by zero";
			break;
		case ArithmeticException::DIVISION_IMPOSSIBLE:
			reason = "Division impossible";
			break;
		case ArithmeticException::OVERFLOW:
			reason = "Arithmetic overflow";
			break;
		case ArithmeticException::UNDERFLOW:
			reason = "Arithmetic underflow";
			break;
		case ArithmeticException::CONVERSION_IMPOSSIBLE:
			reason = "Arithmetic conversion impossible";
			break;
		case ArithmeticException::INVALID_OPERATION_ON_FRACTIONAL_NUMBER:
			reason = "Invalid operation on fractional number";
			break;
		default: // This includes UNKNOWN_REASON
			reason = "Unknown arithmetic error";
			break;
		}
		outputError(reason);
	}
	// Generic parser exception
	catch (ParserException)
	{
		outputError(tr("Unknown error"));
	}
	// Generic MaxCalc exception
	catch (MaxCalcException)
	{
		outputError(tr("Unknown error"));
	}
}

void MainWindow::outputError(QString message)
{
	historyBox.setTextColor(Qt::red);
	historyBox.append(indent + message);
	inputBox.selectAll();
	inputBox.setFocus();
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

/*!
	Unit conversion menu command handler.
*/
void MainWindow::onUnitConversion(const QString & conversion)
{
	inputBox.insert(conversion);
}
