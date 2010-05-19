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
#include "commandparser.h"
#include "constants.h"
// Local
#include "mainwindow.h"
#include "aboutbox.h"
#include "myaction.h"
#include "inputbox.h"
#include "outputsettings.h"
// STL
#include <sstream>
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
#include <QFile>

/// Indentation used for output
static const QString indent = "    ";
/// Version for saveState() / restoreState()
static const int version = 201;

/*!
    \class MainWindow
    \brief Main window of calculator.

    \ingroup MaxCalcGui
*/


/*!
    Constructs a new main window.
*/
MainWindow::MainWindow() : QMainWindow(), mTrayIcon(0), mTrayContextMenu(0)
{
    // Create Parser first, so that we can read its settings into it
    mParser = new Parser();
    mOut = new std::wostringstream;
    mCmdParser = new CommandParser(*mOut, mParser->context());

    readSettings();
    createUi();
    createMainMenu();
    updateVariablesList();

    // Restore window settings
    QSettings * settings = getSettings();
    restoreGeometry(settings->value("WindowGeometry").toByteArray());
    restoreState(settings->value("WindowState").toByteArray(), version);

    delete settings;
}

/*!
    Saves settings and destructs the main window.
*/
MainWindow::~MainWindow()
{
    saveSettings();

    // Delete parser
    delete mCmdParser;
    delete mOut;
    delete mParser;

    // Delete tray icon
    if (mTrayIcon) {
        mTrayIcon->hide();
        delete mTrayContextMenu;
        delete mTrayIcon;
    }
}

/*!
    Reads settings.
*/
void MainWindow::readSettings()
{
    QSettings * settings = getSettings();

    // UI settings
    mMinimizeToTray = QSystemTrayIcon::isSystemTrayAvailable() ?
                      settings->value("MinimizeToTray", false).toBool() : false;
    mCloseToTray = QSystemTrayIcon::isSystemTrayAvailable() ?
                   settings->value("CloseToTray", false).toBool() : false;
    onAddRemoveTrayIcon(mCloseToTray || mMinimizeToTray);
    mShowVariables = settings->value("ShowVariables", true).toBool();
#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    mSingleInstanceMode = settings->value("SingleInstanceMode", false).toBool();
#endif

    // Parser state
    mParser->context().setAngleUnit(
            (ParserContext::AngleUnit)settings->value("AngleUnit", 0).toInt());
    mParser->context().numberFormat().precision = settings->value("Precision",
        Constants::DEFAULT_IO_PRECISION).toInt();
    mParser->context().numberFormat().decimalSeparator =
        (ComplexFormat::DecimalSeparator)settings->value("DecimalSeparator",
        ComplexFormat::DOT_SEPARATOR).toInt();
    mParser->context().numberFormat().imaginaryOne =
        (ComplexFormat::ImaginaryOne)settings->value("ImaginaryOne",
        ComplexFormat::IMAGINARY_ONE_I).toInt();

    delete settings;
}

/*!
    Saves settings.
*/
void MainWindow::saveSettings()
{
    QSettings * settings = getSettings();

    // UI settings
    settings->setValue("MinimizeToTray", mMinimizeToTray);
    settings->setValue("CloseToTray", mCloseToTray);
    settings->setValue("ShowVariables", mShowVariables);
    settings->setValue("WindowGeometry", saveGeometry());
    settings->setValue("WindowState", saveState(version));
#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    settings->setValue("SingleInstanceMode", mSingleInstanceMode);
#endif

    // Parser state
    settings->setValue("AngleUnit", mParser->context().angleUnit());
    settings->setValue("Precision", mParser->context().numberFormat().precision);
    settings->setValue("DecimalSeparator", mParser->context().numberFormat().decimalSeparator);
    settings->setValue("ImaginaryOne", mParser->context().numberFormat().imaginaryOne);

    settings->sync();
    delete settings;
}

/*!
    Inits UI elements.
*/
void MainWindow::createUi()
{
    // Set window parameters
    setWindowTitle("MaxCalc");
#if !defined(_WIN32)
    setWindowIcon(QIcon(":/appicon.png"));
#endif
    setMinimumSize(500, 350);
    resize(750, 550);

    // Create central widget
    mCentralWidget = new QWidget();
    setCentralWidget(mCentralWidget);

    // Create history box
    mHistoryBox = new QTextEdit();
    QFont font = mHistoryBox->currentFont();
    font.setPixelSize(12);
    mHistoryBox->setFont(font);
    mHistoryBox->setReadOnly(true);

    // Create input box
    mInputBox = new InputBox();
    font = mInputBox->font();
    font.setPixelSize(16);
    mInputBox->setFont(font);

    // Create OK button
    mOkButton = new QPushButton();
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
    mVariablesList = new QListWidget(this);
    mVariablesListDock = new QDockWidget(tr("Variables"), this);
    mVariablesListDock->setObjectName("VariablesDockWidget");
    mVariablesListDock->setWidget(mVariablesList);
    addDockWidget(Qt::RightDockWidgetArea, mVariablesListDock);
    mVariablesListDock->setVisible(mShowVariables);

    // Connect
    connect(this, SIGNAL(expressionCalculated()), mInputBox,
        SLOT(addTextToHistory()));
    connect(mOkButton, SIGNAL(clicked()), this,
        SLOT(onExpressionEntered()));
    connect(mInputBox, SIGNAL(returnPressed()), this,
        SLOT(onExpressionEntered()));
    connect(mVariablesList, SIGNAL(itemActivated(QListWidgetItem *)),
        this, SLOT(onVariableClicked(QListWidgetItem *)));

    // Set focus
    mInputBox->setFocus();
}

/*!
    Creates main menu.
*/
void MainWindow::createMainMenu()
{
    QAction * action;

    mMainMenu = new QMenuBar();
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

    QActionGroup * actionGroup = new QActionGroup(this);
    mMenuSettingsRadians = newRadioAction(tr("&Radians"),
        SLOT(onSettingsRadians()), tr("F2"),
        mParser->context().angleUnit() == ParserContext::RADIANS, actionGroup);
    mMenuSettingsDegrees = newRadioAction(tr("&Degrees"),
        SLOT(onSettingsDegrees()), tr("F3"),
        mParser->context().angleUnit() == ParserContext::DEGREES, actionGroup);
    mMenuSettingsGrads = newRadioAction(tr("&Gradians"),
        SLOT(onSettingsGradians()), tr("F4"),
        mParser->context().angleUnit() == ParserContext::GRADIANS, actionGroup);
    settings->addAction(mMenuSettingsRadians);
    settings->addAction(mMenuSettingsDegrees);
    settings->addAction(mMenuSettingsGrads);

    settings->addSeparator();

    settings->addAction(tr("&Output format..."), this, SLOT(onSettingsOutput()),
                        tr("F5"));

    settings->addSeparator();

    action = mVariablesListDock->toggleViewAction();
    settings->addAction(action);
    action->setText(tr("Show &variables"));
    connect(action, SIGNAL(toggled(bool)), this, SLOT(onSettingsVariables(bool)));

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        settings->addSeparator();
        action = settings->addAction(tr("&Minimize to tray"));
        action->setCheckable(true);
        action->setChecked(mMinimizeToTray);
        connect(action, SIGNAL(toggled(bool)), this,
                SLOT(onSettingsMinimizeToTray(bool)));
        action = settings->addAction(tr("&Close to tray"));
        action->setCheckable(true);
        action->setChecked(mCloseToTray);
        connect(action, SIGNAL(toggled(bool)), this,
                SLOT(onSettingsCloseToTray(bool)));
        connect(this, SIGNAL(minimizeToTray()), this,
                SLOT(onTrayMinimizeRestore()), Qt::QueuedConnection);
    }

#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    action = settings->addAction(tr("&Single instance mode"));
    action->setCheckable(true);
    action->setChecked(mSingleInstanceMode);
    connect(action, SIGNAL(toggled(bool)), this,
            SLOT(onSettingsSingleInstanceMode(bool)));
#endif

    // Functions menu

    QMenu * functions = mMainMenu->addMenu(tr("&Functions"));

    QMenu * subMenu = functions->addMenu(tr("&Common"));
    subMenu->addAction(newFunctionAction(subMenu, "abs (Absolute value)"));
    subMenu->addAction(newFunctionAction(subMenu, "sqr (Square)"));
    subMenu->addAction(newFunctionAction(subMenu, "sqrt (Square root)"));
    subMenu->addAction(newFunctionAction(subMenu, "pow (Power)"));
    subMenu->addAction(newFunctionAction(subMenu, "fact (Factorial)"));

    subMenu = functions->addMenu(tr("&Trigonometric"));
    subMenu->addAction(newFunctionAction(subMenu, "sin (Sine)"));
    subMenu->addAction(newFunctionAction(subMenu, "cos (Cosine)"));
    subMenu->addAction(newFunctionAction(subMenu, "tan (Tangent)"));
    subMenu->addAction(newFunctionAction(subMenu, "cot (Cotangent)"));
    subMenu->addSeparator();
    subMenu->addAction(newFunctionAction(subMenu, "asin (Arc Sine)"));
    subMenu->addAction(newFunctionAction(subMenu, "acos (Arc Cosine)"));
    subMenu->addAction(newFunctionAction(subMenu, "atan (Arc Tangent)"));
    subMenu->addAction(newFunctionAction(subMenu, "acot (Arc Cotangent)"));
    subMenu->addSeparator();
    subMenu->addAction(newFunctionAction(subMenu, "sinh (Hyperbolic Sine)"));
    subMenu->addAction(newFunctionAction(subMenu, "cosh (Hyperbolic Cosine)"));
    subMenu->addAction(newFunctionAction(subMenu, "tanh (Hyperbolic Tangent)"));
    subMenu->addAction(newFunctionAction(subMenu, "coth (Hyperbolic Cotangent)"));
    subMenu->addSeparator();
    subMenu->addAction(newFunctionAction(subMenu, "asinh (Hyperbolic Arc Sine)"));
    subMenu->addAction(newFunctionAction(subMenu, "acosh (Hyperbolic Arc Cosine)"));
    subMenu->addAction(newFunctionAction(subMenu, "atanh (Hyperbolic Arc Tangent)"));
    subMenu->addAction(newFunctionAction(subMenu, "acoth (Hyperbolic Arc Cotangent)"));

    subMenu = functions->addMenu(tr("&Logarithmic"));
    subMenu->addAction(newFunctionAction(subMenu, "ln (Natural Logarithm)"));
    subMenu->addAction(newFunctionAction(subMenu, "log2 (Base-2 Logarithm)"));
    subMenu->addAction(newFunctionAction(subMenu, "log10 (Base-10 Logarithm)"));
    subMenu->addAction(newFunctionAction(subMenu, "exp (Exponent)"));

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
            case UnitConversion::ANGLE:
                currentUnits = unitConversion->addMenu(tr("&Angle"));
                break;
            case UnitConversion::LENGTH:
                currentUnits = unitConversion->addMenu(tr("&Length"));
                break;
            case UnitConversion::MASS:
                currentUnits = unitConversion->addMenu(tr("&Mass"));
                break;
            case UnitConversion::TEMPERATURE:
                currentUnits = unitConversion->addMenu(tr("&Temperature"));
                break;
            case UnitConversion::TIME:
                currentUnits = unitConversion->addMenu(tr("T&ime"));
                break;
            case UnitConversion::VELOCITY:
                currentUnits = unitConversion->addMenu(tr("&Velocity"));
                break;
            default:
                currentUnits = unitConversion->addMenu(tr("&Unknown units"));
                break;
            }
        }

        QString firstName = QString::fromWCharArray(cur->name.c_str());
        QString firstDesc = QString::fromWCharArray(cur->desc.c_str());
        QString menu = QString("%1 (%2)").arg(firstDesc, firstName);
        firstLevelMenu = currentUnits->addMenu(menu);
        const UnitConversion::UnitDef * cur2;
        for (cur2 = firstLevelCur; cur2->type == type; ++cur2) {
            if (cur2 == cur) continue;
            QString secondName = QString::fromWCharArray(cur2->name.c_str());
            QString secondDesc = QString::fromWCharArray(cur2->desc.c_str());
            QString conversion = QString("[%1->%2]").arg(firstName, secondName);
            menu = QString("%1 (%2)").arg(secondDesc, secondName);
            firstLevelMenu->addAction(new MyAction(firstLevelMenu, menu,
                conversion, this, SLOT(onUnitConversion(const QString &))));
        }
    }

    // Help menu

    QMenu * help = mMainMenu->addMenu(tr("&Help"));
    help->addAction(tr("&Readme..."), this, SLOT(onHelpReadme()), tr("F1"));
    help->addAction(tr("MaxCalc &Web Site..."), this, SLOT(onHelpWebSite()));
    help->addAction(tr("Report &issue..."), this, SLOT(onHelpReportIssue()));
    help->addSeparator();
    help->addAction(tr("About &MaxCalc"), this, SLOT(onHelpAbout()));
    help->addAction(tr("About &Qt"), QApplication::instance(), SLOT(aboutQt()));
}

/*!
    Adds radio button-like action to menu.
*/
QAction * MainWindow::newRadioAction(const QString & title, const char * slot,
    const QString & shortcut, bool checked, QActionGroup * actionGroup)
{
    QAction * action = new QAction(title, actionGroup);
    connect(action, SIGNAL(triggered()), this, slot);
    action->setShortcut(shortcut);
    action->setCheckable(true);
    action->setChecked(checked);
    return action;
}

/*!
    Adds radio button-like action to menu.
*/
QAction * MainWindow::newFunctionAction(QObject * parent, const QString & title)
{
    return new MyAction(parent, title, title.section(' ', 0, 0), this,
                        SLOT(onFunction(QString)));
}

/*!
    Updates list of varibables from \a mParser.
     Also adds 'e' and 'pi' constants and 'res' variable at the beginning.
*/
void MainWindow::updateVariablesList()
{
    mVariablesList->clear();

    ComplexFormat & format = mParser->context().numberFormat();

    mVariablesList->addItem("e = " +
        QString::fromWCharArray(BigDecimal::E.toWideString(format).c_str()));
    mVariablesList->addItem("pi = " +
        QString::fromWCharArray(BigDecimal::PI.toWideString(format).c_str()));

    if (mParser->context().resultExists()) {
        mVariablesList->addItem("res = " +
            QString::fromWCharArray(mParser->context().result().toWideString(format).c_str()));
    }

    Variables::const_iterator iter;
    for (iter = mParser->context().variables().begin();
        iter != mParser->context().variables().end(); ++iter) {
        mVariablesList->addItem(QString::fromWCharArray(iter->name.c_str()) +
            " = " + QString::fromWCharArray(iter->value.toWideString(format).c_str()));
    }
}

/*!
    Called when expression is entered.
*/
void MainWindow::onExpressionEntered()
{
    // Get expression from input box
    QString expr = mInputBox->text();
    expr = expr.trimmed();
    if (expr.isEmpty()) return;

    // Add expression to history
    mHistoryBox->moveCursor(QTextCursor::End);
    mHistoryBox->setTextColor(Qt::blue);
    mHistoryBox->insertPlainText(mInputBox->text() + "\n");

    wchar_t * str = new wchar_t[expr.length() + 1];
    expr.toWCharArray(str);
    str[expr.length()] = L'\0';

    CommandParser::Result res = mCmdParser->parse(str);

    if (res == CommandParser::EXIT_COMMAND) close();
    if (res == CommandParser::COMMAND_PARSED) {
        // Update settings menu
        ParserContext::AngleUnit angles = mParser->context().angleUnit();
        if (angles == ParserContext::RADIANS) mMenuSettingsRadians->setChecked(true);
        else if (angles == ParserContext::DEGREES) mMenuSettingsDegrees->setChecked(true);
        else mMenuSettingsGrads->setChecked(true);
        // Add expression to input box history
        emit expressionCalculated();
        // Output result
        printResult(QString::fromWCharArray(mOut->str().c_str()));
        mOut->str(_T(""));
        return;
    }

    mParser->setExpression(str);
    delete[] str;

    try {
        ParserContext & context = mParser->parse();
        // Add expression to input box history
        emit expressionCalculated();
        // No error during parsing, output result (otherwise an exception will be caught)
        printResult(QString::fromWCharArray(context.result().toWideString(context.numberFormat()).c_str()));
    } catch (MaxCalcException & ex) {
        printError(QString::fromStdWString(ex.toString()));
    }
}

/*!
    Prints result into history box, clears and focuses input box,
    and updates variables list.
*/
void MainWindow::printResult(const QString & message)
{
    mHistoryBox->setTextColor(Qt::darkGreen);
    QStringList strList = message.split("\n");
    foreach (QString str, strList) {
        if (str != "")
            mHistoryBox->insertPlainText(indent + str + "\n");
    }
    mHistoryBox->ensureCursorVisible();
    mInputBox->clear();
    mInputBox->setFocus();
    updateVariablesList();
}

/*!
    Prints error message into history box and focuses input box.
*/
void MainWindow::printError(const QString & message)
{
    mHistoryBox->setTextColor(Qt::red);
    QStringList strList = message.split("\n");
    foreach (QString str, strList) {
        if (str != "")
            mHistoryBox->insertPlainText(indent + str + "\n");
    }
    mHistoryBox->ensureCursorVisible();
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
    Help -> Readme command.
*/
void MainWindow::onHelpReadme()
{
    QString readme = QApplication::applicationDirPath() + "/Readme.txt";
    if (!QFile::exists(readme)) {
#if !defined(_WIN32)
        readme = QApplication::applicationDirPath() + "/../share/doc/maxcalc/README";
        if (!QFile::exists(readme)) {
#endif
            qDebug("Readme file not found");
            return;
#if !defined(_WIN32)
        }
#endif
    }
    QDesktopServices::openUrl(QUrl("file:///" + readme, QUrl::TolerantMode));
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
void MainWindow::onFunction(const QString & function)
{
    mInputBox->insert(function);
    if (function == "pow") {
        mInputBox->insert("(;)");
        mInputBox->setCursorPosition(mInputBox->cursorPosition() - 2);
    } else {
        mInputBox->insert("()");
        mInputBox->setCursorPosition(mInputBox->cursorPosition() - 1);
    }
    mInputBox->setFocus();
}

/*!
    Unit conversion menu command handler.
*/
void MainWindow::onUnitConversion(const QString & conversion)
{
    mInputBox->insert(conversion);
    mInputBox->setFocus();
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
void MainWindow::onSettingsGradians()
{
    mParser->context().setAngleUnit(ParserContext::GRADIANS);
}

/*!
    Settings -> Output format command.
*/
void MainWindow::onSettingsOutput()
{
    OutputSettings outputSettings(this, mParser->context());
    outputSettings.exec();
}

/*!
    Settings -> Variables command.
*/
void MainWindow::onSettingsVariables(bool active)
{
    if (isVisible() && !isMinimized()) mShowVariables = active;
}

/*!
    Settings -> Minimize to tray command.
*/
void MainWindow::onSettingsMinimizeToTray(bool active)
{
    mMinimizeToTray = active;
    onAddRemoveTrayIcon(mMinimizeToTray || mCloseToTray);
}

/*!
    Settings -> Close to tray command.
*/
void MainWindow::onSettingsCloseToTray(bool active)
{
    mCloseToTray = active;
    onAddRemoveTrayIcon(mMinimizeToTray || mCloseToTray);
}

/*!
    Adds or removes tray icon depending on \a addIcon parameter.
*/
void MainWindow::onAddRemoveTrayIcon(bool addIcon)
{
    if (addIcon && !mTrayIcon) {
        // Create tray icon
        mTrayIcon = new QSystemTrayIcon(QIcon(":/appicon.png"));
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
    } else if (!addIcon && mTrayIcon) {
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
    } else {
        QMainWindow::closeEvent(event);
    }
}

/*!
    Overrides QWidget::changeEvent() to catch window state changes.
*/
void MainWindow::changeEvent(QEvent * event)
{
    if (mMinimizeToTray && event->type() == QEvent::WindowStateChange &&
        isMinimized()) {
        event->ignore();
        emit minimizeToTray();
    } else {
        QMainWindow::changeEvent(event);
    }
}

/*!
    Tray icon clicked.
*/
void MainWindow::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (!isVisible()) {
            showNormal();
            activateWindow();
            if (mTrayContextMenu) {
                mTrayContextMenu->actions().at(0)->setText(tr("&Minimize"));
            }
        } else {
            hide();
            if (mTrayContextMenu) {
                mTrayContextMenu->actions().at(0)->setText(tr("&Restore"));
            }
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

#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
/*!
    Settings -> Single instance mode command.
*/
void MainWindow::onSettingsSingleInstanceMode(bool active)
{
    mSingleInstanceMode = active;
    saveSettings();
    QMessageBox msgBox;
    msgBox.setWindowTitle("MaxCalc");
    msgBox.setText(tr("Please restart the program (File -> Quit) for this setting to take effect."));
    msgBox.exec();
}
#endif

/*!
    Called when single instance window needs to be activated.
*/
void MainWindow::activate(const QString & /*str*/)
{
    if (!isVisible()) {
        onTrayIconClicked(QSystemTrayIcon::Trigger);
    }
    // Hack to force window activation
    bool tmp = mMinimizeToTray;
    mMinimizeToTray = false;
    if (!isMinimized()) showMinimized();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    mMinimizeToTray = tmp;
}

/*!
    Returns QSettings object to access MaxCalc's .ini file with settings.
*/
QSettings * MainWindow::getSettings()
{
#if defined(MAXCALC_PORTABLE)
    return new QSettings(QApplication::applicationDirPath() + "/maxcalc.ini",
        QSettings::IniFormat);
#else
    return new QSettings(QSettings::IniFormat, QSettings::UserScope, "maxcalc",
        "maxcalc");
#endif
}
