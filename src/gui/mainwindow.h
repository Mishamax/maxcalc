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

// Qt
#include <QMainWindow>
#include <QSystemTrayIcon>

// Forward declarations
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class QDockWidget;
class QMenuBar;
class QActionGroup;
class InputBox;
namespace MaxCalcEngine {
    class Parser;
    class CommandParser;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

signals:
    /// Emitted when expression is calculated by parser.
    void expressionCalculated();
    /// Emitted when the window needs to be minimized to tray.
    void minimizeToTray();

private:

    // Window elements
    QWidget * mCentralWidget;
    QVBoxLayout * mLayout;
    QHBoxLayout * mBottomLayout;
    InputBox * mInputBox;
    QTextEdit * mHistoryBox;
    QPushButton * mOkButton;
    QListWidget * mVariablesList;
    QDockWidget * mVariablesListDock;
    QMenuBar * mMainMenu;
    QAction * mMenuSettingsRadians;
    QAction * mMenuSettingsDegrees;
    QAction * mMenuSettingsGrads;

    // Tray icon
    QSystemTrayIcon * mTrayIcon;
    QMenu * mTrayContextMenu;

    // Settings
    bool mMinimizeToTray;
    bool mCloseToTray;
    bool mShowVariables;
#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    bool mSingleInstanceMode;
#endif

    // Parser
    MaxCalcEngine::CommandParser * mCmdParser;
    MaxCalcEngine::Parser * mParser;
    std::wostringstream * mOut;

    // Private functions
    void readSettings();
    void saveSettings();
    void createUi();
    void createMainMenu();
    QAction * newRadioAction(const QString & title, const char * slot,
        const QString & shortcut, bool checked, QActionGroup * actionGroup);
    QAction * newFunctionAction(QObject * parent, const QString & title);
    void printResult(const QString & message);
    void printError(const QString & message);

protected:
    // Overriden events
    void changeEvent(QEvent * event);
    void closeEvent(QCloseEvent * event);

private slots:
    void updateVariablesList();
    void onExpressionEntered();
    void onVariableClicked(QListWidgetItem * item);
    void onHelpReadme();
    void onHelpAbout();
    void onHelpWebSite();
    void onHelpReportIssue();
    void onDeleteAllVariables();
    void onFunction(const QString & function);
    void onUnitConversion(const QString & conversion);
    void onSettingsRadians();
    void onSettingsDegrees();
    void onSettingsGrads();
    void onSettingsOutput();
    void onSettingsVariables(bool active);
    void onSettingsMinimizeToTray(bool active);
    void onSettingsCloseToTray(bool active);
#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    void onSettingsSingleInstanceMode(bool active);
#endif
    void onAddRemoveTrayIcon(bool addIcon);
    void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void onTrayMinimizeRestore();

public slots:
    void activate(const QString & str);
};

#endif // MAINWINDOW_H
