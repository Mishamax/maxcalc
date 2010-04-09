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

#include "myaction.h"

/*!
    \class MyAction
    \brief Custom QAction with additional \a action member.

    MyAction is used for Unit Converison menus. It emits \a executed(action)
    signal when \a triggered() is received.

    \ingroup MaxCalcGui
*/

/*!
    Constructs new MyAction object.
*/
MyAction::MyAction(QObject * parent, const QString & text,
                   const QString & action, QObject * receiver,
                   const char * member) : QAction(text, parent)
{
    Q_ASSERT(parent);
    Q_ASSERT(receiver);
    Q_ASSERT(member);

    mAction = action;
    connect(this, SIGNAL(triggered()), this, SLOT(execute()));
    connect(this, SIGNAL(executed(const QString &)), receiver, member);
}

/*!
    Emits \a executed(mAction) signal.
*/
void MyAction::execute()
{
    emit executed(mAction);
}
