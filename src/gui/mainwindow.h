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

// MaxCalcEngine
#include "parser.h"
#include "parsercontext.h"
// Local
#include "inputbox.h"
#if defined(MAXCALC_MATHML)
#include "miceventhandler.h"
#endif
// Qt
#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QDockWidget>
#include <QActionGroup>

class MainWindow : public QMainWindow
#if defined(MAXCALC_MATHML)
, public MICEventHandler<MainWindow>
#endif
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

#if defined(MAXCALC_MATHML)
    void onMathInput(BSTR mathml);
#endif

signals:
    /// Emitted when expression is calculated by parser.
    void expressionCalculated();

private:
    QWidget mCentralWidget;
    QVBoxLayout mLayout;
    QHBoxLayout mBottomLayout;
    InputBox mInputBox;
    QTextEdit mHistoryBox;
    QPushButton mOkButton;
    QListWidget * mVariablesList;
    QDockWidget mVariablesListWrapper;
    QListWidget * mFunctionsList;
    QDockWidget mFunctionsListWrapper;
    QMenuBar mMainMenu;
    QActionGroup * mAngleUnitActionGroup;

    MaxCalcEngine::Parser mParser;

#if defined(MAXCALC_MATHML)
    // Math Input Control
    CComPtr<IMathInputControl> mic;
#endif

    void initUi();
    void initMainMenu();
    void updateVariablesList();
    void initFunctionsList();
    void outputError(const QString & message);

private slots:
    void onExpressionEntered();
    void onVariableClicked(QListWidgetItem * item);
    void onFunctionClicked(QListWidgetItem * item);
    void onHelpAbout();
    void onHelpWebSite();
    void onHelpReportIssue();
    void onDeleteAllVariables();
    void onUnitConversion(const QString & conversion);
    void onSettingsRadians();
    void onSettingsDegrees();
    void onSettingsGrads();
};

#endif // MAINWINDOW_H
