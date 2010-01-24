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

// MaxCalcEngine
#include "parser.h"
#include "parsercontext.h"
#include "unitconversion.h"
// Local
#include "mainwindow.h"
#include "aboutbox.h"
#include "myaction.h"
#include "inputbox.h"
#include "../engine/i18n.h"
// Qt
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QActionGroup>
#include <QListWidget>
#include <QSystemTrayIcon>
#include <QSettings>
#include <QDockWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMessageBox>

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
MainWindow::MainWindow() : QMainWindow(), mTrayIcon(0), mTrayContextMenu(0),
        mSettingFileName(QApplication::applicationDirPath() + "/maxcalc.ini")
{
    // Create Parser first, so that we can read its settings into it
    mParser = new Parser();

    readSettings();
    createUi();
    createMainMenu();
    updateVariablesList();
    createFunctionsList();
}

MainWindow::~MainWindow()
{
    saveSettings();

    // Delete tray icon
    if (mTrayIcon) {
        mTrayIcon->hide();
        delete mTrayContextMenu;
        delete mTrayIcon;
    }

    // Delete main menu
    delete mAngleUnitActionGroup;
    delete mMainMenu;

    // Delete UI
    delete mVariablesList;
    delete mFunctionsList;
    delete mVariablesListWrapper;
    delete mFunctionsListWrapper;
    delete mOkButton;
    delete mInputBox;
    delete mHistoryBox;
    delete mBottomLayout;
    delete mLayout;
    delete mCentralWidget;
}

/*!
    Reads settings.
*/
void MainWindow::readSettings()
{
    QSettings settings(mSettingFileName, QSettings::IniFormat);
    mCloseToTray = QSystemTrayIcon::isSystemTrayAvailable() ?
                   settings.value("CloseToTray", false).toBool() :
                   false;
    addRemoveTrayIcon(mCloseToTray);
    mShowFunctions = settings.value("ShowFunctions", true).toBool();
    mShowVariables = settings.value("ShowVariables", true).toBool();
    mSingleInstanceMode = settings.value("SingleInstanceMode", false).toBool();
    mParser->context().setAngleUnit(
            (ParserContext::AngleUnit)settings.value("AngleUnit", 0).toInt());
}

/*!
    Saves settings.
*/
void MainWindow::saveSettings()
{
    QSettings settings(mSettingFileName, QSettings::IniFormat);
    settings.setValue("CloseToTray", mCloseToTray);
    settings.setValue("ShowFunctions", mShowFunctions);
    settings.setValue("ShowVariables", mShowVariables);
    settings.setValue("SingleInstanceMode", mSingleInstanceMode);
    settings.setValue("AngleUnit", mParser->context().angleUnit());
    settings.sync();
}

/*!
    Inits UI elements.
*/
void MainWindow::createUi()
{
    // Set window parameters
    setWindowTitle("MaxCalc");
    setMinimumSize(500, 350);
    resize(750, 550);

    // Create central widget
    mCentralWidget = new QWidget(this);
    setCentralWidget(mCentralWidget);

    // Create history box
    mHistoryBox = new QTextEdit(this);
    QFont font = mHistoryBox->currentFont();
    font.setPixelSize(12);
    mHistoryBox->setFont(font);
    mHistoryBox->setReadOnly(true);

    // Create input box
    mInputBox = new InputBox(this);
    font = mInputBox->font();
    font.setPixelSize(16);
    mInputBox->setFont(font);

    // Create OK button
    mOkButton = new QPushButton(this);
    font = mOkButton->font();
    font.setPixelSize(14);
    mOkButton->setFont(font);
    mOkButton->setText(tr("OK"));
    mOkButton->setMinimumWidth(30);
    mOkButton->setMaximumWidth(30);

    // Create bottom layout (with input box and OK button)
    mBottomLayout = new QHBoxLayout();
    mBottomLayout->addWidget(mInputBox);
    mBottomLayout->addWidget(mOkButton);

    // Create main layout
    mLayout = new QVBoxLayout();
    mLayout->addWidget(mHistoryBox);
    mLayout->addLayout(mBottomLayout);
    mCentralWidget->setLayout(mLayout);

    // Create functions and variables lists
    mVariablesList = new QListWidget();
    mFunctionsList = new QListWidget();
    mVariablesListWrapper = new QDockWidget(tr("Variables"), this);
    mFunctionsListWrapper = new QDockWidget(tr("Functions"), this);
    mVariablesListWrapper->setWidget(mVariablesList);
    mFunctionsListWrapper->setWidget(mFunctionsList);
    addDockWidget(Qt::RightDockWidgetArea, mVariablesListWrapper);
    tabifyDockWidget(mVariablesListWrapper, mFunctionsListWrapper);
    mVariablesListWrapper->raise();
    mVariablesListWrapper->setVisible(mShowVariables);
    mFunctionsListWrapper->setVisible(mShowFunctions);

    // Connect
    QObject::connect(this, SIGNAL(expressionCalculated()), mInputBox,
        SLOT(addTextToHistory()));
    QObject::connect(mOkButton, SIGNAL(clicked()), this,
        SLOT(onExpressionEntered()));
    QObject::connect(mInputBox, SIGNAL(returnPressed()), this,
        SLOT(onExpressionEntered()));
    QObject::connect(mVariablesList, SIGNAL(itemActivated(QListWidgetItem *)),
        this, SLOT(onVariableClicked(QListWidgetItem *)));
    QObject::connect(mFunctionsList, SIGNAL(itemActivated(QListWidgetItem *)),
        this, SLOT(onFunctionClicked(QListWidgetItem *)));

    // Set focus
    mInputBox->setFocus();
}

/*!
    Creates main menu.
*/
void MainWindow::createMainMenu()
{
    mMainMenu = new QMenuBar(this);
    setMenuBar(mMainMenu);

    // Commands menu

    QMenu * commands = mMainMenu->addMenu(tr("&Commands"));
    commands->addAction(tr("Clear &history"), mHistoryBox, SLOT(clear()),
                        tr("Ctrl+H"));
    commands->addAction(tr("&Delete all variables"), this,
                        SLOT(onDeleteAllVariables()), tr("Ctrl+D"));
    commands->addSeparator();
    commands->addAction(tr("&Quit"), QApplication::instance(), SLOT(quit()), tr("Ctrl+Q"));

    // Settings menu

    QMenu * settings = mMainMenu->addMenu(tr("&Settings"));
    QAction * action;
    mAngleUnitActionGroup = new QActionGroup(this);
    action = settings->addAction(tr("&Radians"), this, SLOT(onSettingsRadians()), tr("F2"));
    action->setCheckable(true);
    action->setChecked(mParser->context().angleUnit() == ParserContext::RADIANS);
    mAngleUnitActionGroup->addAction(action);
    action = settings->addAction(tr("&Degrees"), this, SLOT(onSettingsDegrees()), tr("F3"));
    action->setCheckable(true);
    action->setChecked(mParser->context().angleUnit() == ParserContext::DEGREES);
    mAngleUnitActionGroup->addAction(action);
    action = settings->addAction(tr("&Grads"), this, SLOT(onSettingsGrads()), tr("F4"));
    action->setCheckable(true);
    action->setChecked(mParser->context().angleUnit() == ParserContext::GRADS);
    mAngleUnitActionGroup->addAction(action);

    settings->addSeparator();

    action = settings->addAction(tr("Show &variables"));
    action->setCheckable(true);
    action->setChecked(mShowVariables);
    connect(action, SIGNAL(toggled(bool)), mVariablesListWrapper,
            SLOT(setVisible(bool)));
    connect(action, SIGNAL(toggled(bool)), this,
            SLOT(onDockWidgetToggled(bool)));
    action = settings->addAction(tr("Show &functions"));
    action->setCheckable(true);
    action->setChecked(mShowFunctions);
    connect(action, SIGNAL(toggled(bool)), mFunctionsListWrapper,
            SLOT(setVisible(bool)));
    connect(action, SIGNAL(toggled(bool)), this,
            SLOT(onDockWidgetToggled(bool)));

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        settings->addSeparator();
        action = settings->addAction(tr("&Close to tray"));
        action->setCheckable(true);
        action->setChecked(mCloseToTray);
        connect(action, SIGNAL(toggled(bool)), this,
                SLOT(addRemoveTrayIcon(bool)));
    }
    action = settings->addAction(tr("&Single instance mode"));
    action->setCheckable(true);
    action->setChecked(mSingleInstanceMode);
    connect(action, SIGNAL(toggled(bool)), this,
            SLOT(onSettingsSingleInstanceMode(bool)));

    // Unit conversion menu

    QMenu * unitConversion = mMainMenu->addMenu(tr("&Unit conversion"));
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

    QMenu * help = mMainMenu->addMenu(tr("&Help"));
    help->addAction(tr("MaxCalc &Web Site..."), this, SLOT(onHelpWebSite()));
    help->addAction(tr("&Report issue..."), this, SLOT(onHelpReportIssue()));
    help->addSeparator();
    help->addAction(tr("About &MaxCalc"), this, SLOT(onHelpAbout()), tr("F1"));
    help->addAction(tr("About &Qt"), QApplication::instance(), SLOT(aboutQt()));
}

/*!
    Updates list of varibables from \a mParser.
     Also adds 'e' and 'pi' constants and 'res' variable at the beginning.
*/
void MainWindow::updateVariablesList()
{
    mVariablesList->clear();

    mVariablesList->addItem("e = " +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::E.toWideString().c_str()));
    mVariablesList->addItem("pi = " +
        QString::fromWCharArray(MaxCalcEngine::BigDecimal::PI.toWideString().c_str()));

    if (mParser->context().resultExists()) {
        mVariablesList->addItem("res = " +
            QString::fromWCharArray(mParser->context().result().toWideString().c_str()));
    }

    MaxCalcEngine::Variables::const_iterator iter;
    for (iter = mParser->context().variables().begin();
        iter != mParser->context().variables().end(); ++iter) {
        mVariablesList->addItem(QString::fromWCharArray(iter->name.c_str()) +
            " = " + QString::fromWCharArray(iter->value.toWideString().c_str()));
    }
}

/*!
    Adds supported functions to functions list.
*/
void MainWindow::createFunctionsList()
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
    // Get expression from input box
    QString expr = mInputBox->text();
    expr = expr.trimmed();
    if (expr.isEmpty()) {
        return;
    }
    wchar_t * str = new wchar_t[expr.length() + 1];
    expr.toWCharArray(str);
    str[expr.length()] = L'\0';
    mParser->setExpression(str);
    delete[] str;

    // Add expression to history
    mHistoryBox->moveCursor(QTextCursor::End);
    mHistoryBox->setTextColor(Qt::blue);
    mHistoryBox->append(mInputBox->text());

    try {
        mParser->parse();

        // No error during parsing, output result (otherwise an exception will be caught)

        // Add expression to input box history
        emit expressionCalculated();

        // Add result to history, clear input box, update variables
        mHistoryBox->setTextColor(Qt::darkGreen);
        mHistoryBox->append(indent +
            QString::fromWCharArray(mParser->context().result().toWideString().c_str()));
        mInputBox->clear();
        mInputBox->setFocus();
        updateVariablesList();
    } catch (ParserException & ex) {
        printError(I18n::parserExceptionToString(ex));
    } catch (InvalidArgumentException & ex) {
        printError(I18n::invalidArgumentExceptionToString(ex));
    } catch (ArithmeticException & ex) {
        printError(I18n::arithmeticExceptionToString(ex));
    } catch (MaxCalcException & ex) {
        printError(I18n::maxCalcExceptionToString(ex));
    }
}

/*!
    Prints error message into history box and focuses input box.
*/
void MainWindow::printError(const QString & message)
{
    mHistoryBox->setTextColor(Qt::red);
    mHistoryBox->append(indent + message);
    mInputBox->selectAll();
    mInputBox->setFocus();
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
    mInputBox->insert(text);
    mInputBox->setFocus();
}

/*!
    Called when function in the list is clicked.
*/
void MainWindow::onFunctionClicked(QListWidgetItem * item)
{
    Q_ASSERT(item);
    mInputBox->insert(item->text());
    mInputBox->insert("()");
    mInputBox->setCursorPosition(mInputBox->cursorPosition() - 1);
    mInputBox->setFocus();
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
    mParser->context().variables().removeAll();
    updateVariablesList();
}

/*!
    Unit conversion menu command handler.
*/
void MainWindow::onUnitConversion(const QString & conversion)
{
    mInputBox->insert(conversion);
}

/*!
    Settings -> Radians command.
*/
void MainWindow::onSettingsRadians()
{
    mParser->context().setAngleUnit(ParserContext::RADIANS);
}

/*!
    Settings -> Degrees command.
*/
void MainWindow::onSettingsDegrees()
{
    mParser->context().setAngleUnit(ParserContext::DEGREES);
}

/*!
    Settings -> Grads command.
*/
void MainWindow::onSettingsGrads()
{
    mParser->context().setAngleUnit(ParserContext::GRADS);
}

/*!
    Adds or removes tray icon depending on \a addIcon parameter.
*/
void MainWindow::addRemoveTrayIcon(bool addIcon)
{
    mCloseToTray = addIcon;

    if (addIcon && !mTrayIcon) {
        // Create tray icon
        mTrayIcon = new QSystemTrayIcon(QIcon(":/appicon.ico"));
        connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(onTrayIconClicked(QSystemTrayIcon::ActivationReason)));
        mTrayIcon->setToolTip("MaxCalc");
        mTrayContextMenu = new QMenu();
        mTrayContextMenu->addAction(tr("&Minimize"), this,
                                    SLOT(onTrayMinimizeRestore()));
        mTrayContextMenu->addAction(tr("&Quit"), QApplication::instance(),
                                    SLOT(quit()));
        mTrayIcon->setContextMenu(mTrayContextMenu);
        mTrayIcon->show();
    } else if (mTrayIcon) {
        // Delete tray icon
        mTrayIcon->hide();
        delete mTrayIcon;
        mTrayIcon = 0;
        delete mTrayContextMenu;
        mTrayContextMenu = 0;
    }
}

/*!
    Close window event.
*/
void MainWindow::closeEvent(QCloseEvent * event)
{
    if (mCloseToTray) {
        onTrayIconClicked(QSystemTrayIcon::Trigger);
        event->ignore();
    }
}

/*!
    Tray icon clicked.
*/
void MainWindow::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
        if (isVisible()) {
            activateWindow();
            mTrayContextMenu->actions().at(0)->setText(tr("&Minimize"));
        } else {
            mTrayContextMenu->actions().at(0)->setText(tr("&Restore"));
        }
    }
}

/*!
    Tray -> Minimize / Restore clicked.
*/
void MainWindow::onTrayMinimizeRestore()
{
    onTrayIconClicked(QSystemTrayIcon::Trigger);
}

/*!
    Called when a dock widget (functions or variables list) is toggled.
    Updates mShowFunctions and mShowVariables.
*/
void MainWindow::onDockWidgetToggled(bool /*visible*/)
{
    mShowFunctions = mFunctionsListWrapper->isVisible();
    mShowVariables = mVariablesListWrapper->isVisible();
}

/*!
    Settings -> Single instance mode command.
*/
void MainWindow::onSettingsSingleInstanceMode(bool active)
{
    mSingleInstanceMode = active;
    saveSettings();
}

/*!
    Called when single instance window needs to be activated.
*/
void MainWindow::activate(const QString & /*str*/)
{
    if (!isVisible()) {
        onTrayIconClicked(QSystemTrayIcon::Trigger);
    }
    activateWindow();
    raise();
}
