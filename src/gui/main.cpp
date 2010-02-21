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

// Local
#include "mainwindow.h"
// Qt
#include <QSettings>
#include <QTranslator>
#include <QLocale>

#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
#include "qtsingleapplication/qtsingleapplication.h"
#else
#include <QApplication>
#endif

/*!
    \defgroup MaxCalcGui MaxCalc GUI
*/

int main(int argc, char ** argv)
{
#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    QtSingleApplication app(argc, argv);

    QString settingsFile = QApplication::applicationDirPath() + "/maxcalc.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    bool singleInstanceMode = settings.value("SingleInstanceMode", false).toBool();

    if (singleInstanceMode && app.isRunning()) {
        app.sendMessage("");
        return 0;
    }
#else
    QApplication app(argc, argv);
#endif

    QTranslator translator;
    translator.load(QString("maxcalcgui_") + QLocale::system().name());
    app.installTranslator(&translator);

    MainWindow mainWindow;

#if defined(MAXCALC_SINGLE_INSTANCE_MODE)
    app.setActivationWindow(&mainWindow);
#endif

    QObject::connect(&app, SIGNAL(messageReceived(const QString &)),
                     &mainWindow, SLOT(activate(const QString &)));
    mainWindow.show();

    return app.exec();
}
