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

#ifndef MYACTION_H
#define MYACTION_H

#include <QAction>

class MyAction : public QAction
{
    Q_OBJECT

private:
    QString mAction;    ///< Action which will be passed into executed() signal.

public:
    MyAction(QObject * parent, const QString & text, const QString & action,
             QObject * receiver, const char * member);

private slots:
    void execute();

signals:
    void executed(const QString & action); ///< Emited when action is triggered.
};

#endif // MYACTION_H
