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


// MaxCalcEngine
#include "bigdecimal.h"
#include "unitconversion.h"
// Local
#include "mainwindow.h"
#include "aboutbox.h"
#include "myaction.h"
#include "miceventhandler.h"
// Qt
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

/// Indentation used for output
static const QString indent = "    ";

/*!
    \class MainWindow
    \brief Main window of calculator.

    \ingroup MaxCalcGui
*/

const _ATL_FUNC_INFO MICEventHandler<MainWindow>::mOnMICInsertInfo = {CC_STDCALL, VT_I4, 1, {VT_BSTR}};
const _ATL_FUNC_INFO MICEventHandler<MainWindow>::mOnMICCloseInfo = {CC_STDCALL, VT_I4, 0, {VT_EMPTY}};

using namespace MaxCalcEngine;

/*!
    Constructs a new main window.
*/
MainWindow::MainWindow() : QMainWindow(),
        mVariablesListWrapper(tr("Variables"), this),
        mFunctionsListWrapper(tr("Functions"), this)
{
    initUi();
    initMainMenu();
    updateVariablesList();
    initFunctionsList();

    // Show Math Input Control
    CoInitialize(NULL);
    mic.CoCreateInstance(CLSID_MathInputControl);
    Initialize(mic);
    DispEventAdvise(mic);
    mic->Show();
}

/*!
    Inits UI elements.
*/
void MainWindow::initUi()
{
    mVariablesList = new QListWidget();
    mFunctionsList = new QListWidget();

    setWindowTitle(tr("MaxCalc"));
    setMinimumSize(500, 350);
    resize(750, 550);

    setCentralWidget(&mCentralWidget);

    QFont font = mHistoryBox.currentFont();
    font.setPixelSize(12);
    mHistoryBox.setFont(font);
    mHistoryBox.setReadOnly(true);

    font = mInputBox.font();
    font.setPixelSize(16);
    mInputBox.setFont(font);

    font = mOkButton.font();
    font.setPixelSize(14);
    mOkButton.setFont(font);
    mOkButton.setText(tr("OK"));
    mOkButton.setMinimumWidth(30);
    mOkButton.setMaximumWidth(30);

    mBottomLayout.addWidget(&mInputBox);
    mBottomLayout.addWidget(&mOkButton);

    mVariablesListWrapper.setWidget(mVariablesList);
    mFunctionsListWrapper.setWidget(mFunctionsList);

    mLayout.addWidget(&mHistoryBox);
    mLayout.addLayout(&mBottomLayout);
    
    addDockWidget(Qt::RightDockWidgetArea, &mVariablesListWrapper);
    tabifyDockWidget(&mVariablesListWrapper, &mFunctionsListWrapper);
    mVariablesListWrapper.raise();

    mCentralWidget.setLayout(&mLayout);

    QObject::connect(this, SIGNAL(expressionCalculated()), &mInputBox,
        SLOT(addTextToHistory()));
    QObject::connect(&mOkButton, SIGNAL(clicked()), this,
        SLOT(onExpressionEntered()));
    QObject::connect(&mInputBox, SIGNAL(returnPressed()), this,
        SLOT(onExpressionEntered()));
    QObject::connect(mVariablesList, SIGNAL(itemActivated(QListWidgetItem *)),
        this, SLOT(onVariableClicked(QListWidgetItem *)));
    QObject::connect(mFunctionsList, SIGNAL(itemActivated(QListWidgetItem *)),
        this, SLOT(onFunctionClicked(QListWidgetItem *)));

    mInputBox.setFocus();
}

MainWindow::~MainWindow()
{
    delete mVariablesList;
    delete mFunctionsList;
}

/*!
    Creates main menu.
*/
void MainWindow::initMainMenu()
{
    setMenuBar(&mMainMenu);

    // File menu

    QMenu * file = mMainMenu.addMenu(tr("&File"));
    file->addAction(tr("&Exit"), this, SLOT(close()));

    // Commands menu

    QMenu * commands = mMainMenu.addMenu(tr("&Commands"));
    commands->addAction(tr("&Clear history"), &mHistoryBox, SLOT(clear()));
    commands->addAction(tr("&Delete all variables"), this,
                        SLOT(onDeleteAllVariables()));

    // Settings menu

    QMenu * settings = mMainMenu.addMenu(tr("&Settings"));
    QAction * action = new QAction(tr("&Variables"), settings);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(toggled(bool)), &mVariablesListWrapper,
            SLOT(setVisible(bool)));
    settings->addAction(action);
    action = new QAction(tr("&Functions"), settings);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(toggled(bool)), &mFunctionsListWrapper,
            SLOT(setVisible(bool)));
    settings->addAction(action);
//    action = new QAction(tr("&"))

    // Unit conversion menu

    QMenu * unitConversion = mMainMenu.addMenu(tr("&Unit conversion"));
    QMenu * currentUnits = 0;
    QMenu * firstLevelMenu = 0;
    const UnitConversion::UnitDef * firstLevelCur = 0;

    UnitConversion::Type type = UnitConversion::NO_TYPE;
    const UnitConversion::UnitDef * cur;
    for (cur = UnitConversion::units(); cur->unit != UnitConversion::NO_UNIT; ++cur) {
        if (type != cur->type) {
            type = cur->type;
            firstLevelCur = cur;
            switch (type) {
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
        for (cur2 = firstLevelCur; cur2->type == type; ++cur2) {
            if (cur2 == cur) {
                continue;
            }
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

    QMenu * help = mMainMenu.addMenu(tr("&Help"));
    help->addAction(tr("MaxCalc &Web Site..."), this, SLOT(onHelpWebSite()));
    help->addAction(tr("&Report issue..."), this, SLOT(onHelpReportIssue()));
    help->addSeparator();
    help->addAction(tr("About &MaxCalc"), this, SLOT(onHelpAbout()));
    help->addAction(tr("About &Qt"), QApplication::instance(), SLOT(aboutQt()));
}

/*!
    Adds constants to varibables list.
*/
void MainWindow::updateVariablesList()
{
    mVariablesList->clear();

    mVariablesList->addItem("e = " +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::E.toWideString().c_str()));
    mVariablesList->addItem("pi = " +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::PI.toWideString().c_str()));

    if (mParser.context().resultExists()) {
        mVariablesList->addItem("res = " +
            QString::fromWCharArray(mParser.context().result().toWideString().c_str()));
    }

    MaxCalcEngine::Variables::const_iterator iter;
    for (iter = mParser.context().variables().begin();
        iter != mParser.context().variables().end(); ++iter) {
        mVariablesList->addItem(QString::fromWCharArray(iter->name.c_str()) +
            " = " + QString::fromWCharArray(iter->value.toWideString().c_str()));
    }
}

/*!
    Adds supported functions to functions list.
*/
void MainWindow::initFunctionsList()
{
    mFunctionsList->addItem("abs");
    mFunctionsList->addItem("sqr");
    mFunctionsList->addItem("sqrt");
    mFunctionsList->addItem("pow");
    mFunctionsList->addItem("fact");
    mFunctionsList->addItem("sin");
    mFunctionsList->addItem("cos");
    mFunctionsList->addItem("tan");
    mFunctionsList->addItem("cot");
    mFunctionsList->addItem("asin");
    mFunctionsList->addItem("acos");
    mFunctionsList->addItem("atan");
    mFunctionsList->addItem("acot");
    mFunctionsList->addItem("sinh");
    mFunctionsList->addItem("cosh");
    mFunctionsList->addItem("tanh");
    mFunctionsList->addItem("coth");
    mFunctionsList->addItem("asinh");
    mFunctionsList->addItem("acosh");
    mFunctionsList->addItem("atanh");
    mFunctionsList->addItem("acoth");
    mFunctionsList->addItem("ln");
    mFunctionsList->addItem("log2");
    mFunctionsList->addItem("log10");
    mFunctionsList->addItem("exp");
}

/*!
    Called when expression is entered.
*/
void MainWindow::onExpressionEntered()
{
    QString expr = mInputBox.text();
    expr = expr.trimmed();

    if (expr.isEmpty()) {
        return;
    }

    wchar_t * str = new wchar_t[expr.length() + 1];
    expr.toWCharArray(str);
    str[expr.length()] = L'\0';
    mParser.setExpression(str);

    mHistoryBox.moveCursor(QTextCursor::End);
    mHistoryBox.setTextColor(Qt::blue);
    mHistoryBox.append(mInputBox.text());

    try {
        mParser.parse();

        // No error during parsing, output result (otherwise an exception will be caught)

        emit expressionCalculated();

        mHistoryBox.setTextColor(Qt::darkGreen);
        mHistoryBox.append(indent +
            QString::fromWCharArray(mParser.context().result().toWideString().c_str()));
        mInputBox.clear();
        mInputBox.setFocus();
        updateVariablesList();
    }
    // Parser exceptions
    catch (ResultDoesNotExistException) {
        outputError(tr("No result of previous calculations"));
    } catch (UnknownTokenException & ex) {
        outputError(tr("Unknown token '%1' in expression")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    } catch (IncorrectNumberException & ex) {
        if (ex.what() == _T("")) {
            outputError(tr("Incorrect number"));
        } else {
            outputError(tr("Incorrect number '%1'")
                        .arg(QString::fromWCharArray(ex.what().c_str())));
        }
    } catch (IncorrectExpressionException) {
        outputError(tr("Incorrect expression"));
    } catch (NoClosingBracketException) {
        outputError(tr("No closing bracket"));
    } catch (TooManyClosingBracketsException) {
        outputError(tr("Too many closing brackets"));
    } catch (UnknownFunctionException & ex) {
        outputError(tr("Unknown function '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    } catch (UnknownVariableException & ex) {
        outputError(tr("Unknown variable '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    } catch (IncorrectVariableNameException) {
        outputError(tr("Incorrect name of variable"));
    } catch (IncorrectUnitConversionSyntaxException) {
        outputError(tr("Incorrect unit conversion syntax"));
    } catch (UnknownUnitException & ex) {
        outputError(tr("Unknown unit '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    } catch (UnknownUnitConversionException & ex) {
        outputError(tr("There is no unit conversion '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    }
    // Invalid argument exceptions
    catch (InvalidArgumentException & ex) {
        QString msg = tr("Invalid argument of function '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str()));
        switch (ex.reason()) {
        case InvalidArgumentException::ZERO:
            msg += tr(" (zero)");
            break;
        case InvalidArgumentException::NEGATIVE:
            msg += tr(" (negative number)");
            break;
        case InvalidArgumentException::ZERO_OR_NEGATIVE:
            msg += tr(" (zero or negative number)");
            break;
        case InvalidArgumentException::POWER_FUNCTION:
            msg += tr(" (zero or negative number in negative degree)");
            break;
        case InvalidArgumentException::FACTORIAL_FUNCTION:
            msg += tr(" (negative, fractional or complex number)");
            break;
        case InvalidArgumentException::TANGENT_FUNCTION:
            msg += tr(" (cos(arg) = 0)");
            break;
        case InvalidArgumentException::COTANGENT_FUNCTION:
            msg += tr(" (sin(arg) = 0)");
            break;
        case InvalidArgumentException::ARCSINE_FUNCTION:
        case InvalidArgumentException::ARCCOSINE_FUNCTION:
            msg += tr(" (abs(arg) > 1)");
            break;
        case InvalidArgumentException::HYPERBOLIC_TANGENT_FUNCTION:
            msg += tr(" (cosh(arg) = 0)");
            break;
        case InvalidArgumentException::HYPERBOLIC_COTANGENT_FUNCTION:
            msg += tr(" (sinh(arg) = 0)");
            break;
        case InvalidArgumentException::UNKNOWN:
        default:
            // Add nothing
            break;
        }
        outputError(msg);
    } catch (InvalidUnitConversionArgumentException & ex) {
        outputError(tr("Complex argument in unit conversion '%1'")
                    .arg(QString::fromWCharArray(ex.what().c_str())));
    }
    // Arithmetic exception
    catch (ArithmeticException & ex) {
        QString reason;
        switch (ex.reason()) {
        case ArithmeticException::DIVISION_BY_ZERO:
            reason = tr("Division by zero");
            break;
        case ArithmeticException::DIVISION_IMPOSSIBLE:
            reason = tr("Division impossible");
            break;
        case ArithmeticException::ARITHMETIC_OVERFLOW:
            reason = tr("Arithmetic overflow");
            break;
        case ArithmeticException::ARITHMETIC_UNDERFLOW:
            reason = tr("Arithmetic underflow");
            break;
        case ArithmeticException::CONVERSION_IMPOSSIBLE:
            reason = tr("Arithmetic conversion impossible");
            break;
        case ArithmeticException::INVALID_OPERATION_ON_FRACTIONAL_NUMBER:
            reason = tr("Invalid operation on fractional number");
            break;
        default: // This includes UNKNOWN_REASON
            reason = tr("Unknown arithmetic error");
            break;
        }
        outputError(reason);
    }
    // Generic parser exception
    catch (ParserException) {
        outputError(tr("Unknown error"));
    }
    // Generic MaxCalc exception
    catch (MaxCalcException) {
        outputError(tr("Unknown error"));
    }
}

void MainWindow::outputError(const QString & message)
{
    mHistoryBox.setTextColor(Qt::red);
    mHistoryBox.append(indent + message);
    mInputBox.selectAll();
    mInputBox.setFocus();
}

/*!
    Called when variable in the list is clicked.
*/
void MainWindow::onVariableClicked(QListWidgetItem * item)
{
    Q_ASSERT(item);
    QString text = item->text();
    int equalIndex = text.indexOf(" = ");
    text.remove(equalIndex, text.length() - equalIndex);
    mInputBox.insert(text);
    mInputBox.setFocus();
}

/*!
    Called when function in the list is clicked.
*/
void MainWindow::onFunctionClicked(QListWidgetItem * item)
{
    Q_ASSERT(item);
    mInputBox.insert(item->text());
    mInputBox.insert("()");
    mInputBox.setCursorPosition(mInputBox.cursorPosition() - 1);
    mInputBox.setFocus();
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
    Help -> MaxCalc Web site command.
*/
void MainWindow::onHelpWebSite()
{
    QDesktopServices::openUrl(QUrl("http://code.google.com/p/maxcalc/"));
}

/*!
    Help -> Report issue site command.
*/
void MainWindow::onHelpReportIssue()
{
    QDesktopServices::openUrl(QUrl("http://code.google.com/p/maxcalc/issues/entry"));
}

/*!
    Commands -> Delete all variables command.
*/
void MainWindow::onDeleteAllVariables()
{
    mParser.context().variables().removeAll();
    updateVariablesList();
}

/*!
    Unit conversion menu command handler.
*/
void MainWindow::onUnitConversion(const QString & conversion)
{
    mInputBox.insert(conversion);
}

/*!
    Handles MathML input from Math Input Control.
*/
void MainWindow::onMathInput(BSTR mathml)
{
    mInputBox.insert(QString::fromWCharArray(mathml));
}
