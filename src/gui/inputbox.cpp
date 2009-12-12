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
#include "inputbox.h"

/*!
    \class InputBox
    \brief Input box used to input expression.

    InputBox is based on QLineEdit and has history support.

    \ingroup MaxCalcGui
*/

/*!
    Constructs new instance of InputBox.
*/
InputBox::InputBox() : QLineEdit()
{
    mHistory.append("");
    mCurrentHistoryEntry = mHistory.begin();

    QObject::connect(this, SIGNAL(textEdited(const QString &)), this,
        SLOT(onTextEdited(const QString &)));
}

/*!
    Adds current text to history.
*/
void InputBox::addTextToHistory()
{
    *mHistory.begin() = text();
    mHistory.push_front("");
    mCurrentHistoryEntry = mHistory.begin();
}

/*!
    Processes key events used to work with history.
*/
void InputBox::keyPressEvent(QKeyEvent * event)
{
    Q_ASSERT(event);
    if (event->key() == Qt::Key_Up) onKeyUpPressed();
    else if (event->key() == Qt::Key_Down) onKeyDownPressed();
    else if (event->key() == Qt::Key_Escape) onKeyEscapePressed();
    else QLineEdit::keyPressEvent(event);
}

/*!
    Displays previous history entry.
*/
void InputBox::onKeyUpPressed()
{
    mCurrentHistoryEntry++;
    if (mCurrentHistoryEntry == mHistory.end()) {
        mCurrentHistoryEntry--;
    } else {
        setText(*mCurrentHistoryEntry);
        setCursorPosition(text().length());
    }
}

/*!
    Displays next history entry.
*/
void InputBox::onKeyDownPressed()
{
    if (mCurrentHistoryEntry != mHistory.begin()) {
        mCurrentHistoryEntry--;
        setText(*mCurrentHistoryEntry);
        setCursorPosition(text().length());
    }
}

/*!
    Clears input box by Escape key.
*/
void InputBox::onKeyEscapePressed()
{
    clear();
}

/*!
    Updates current history entry when text is edited.
*/
void InputBox::onTextEdited(const QString & text)
{
    *mHistory.begin() = text;
}
