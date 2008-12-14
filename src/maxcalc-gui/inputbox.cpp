/******************************************************************************
 *  MaxCalc - a powerful scientific calculator.
 *  Copyright (C) 2005, 2008 Michael Maximov (michael.maximov@gmail.com)
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

InputBox::InputBox() : QLineEdit()
{
	history.append("");
	currentHistoryEntry = history.begin();

	QObject::connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(onTextEdited(const QString &)));
}

void InputBox::onExpressionCalculated()
{
	*history.begin() = text();
	history.push_front("");
	currentHistoryEntry = history.begin();
}

void InputBox::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Up)
		onKeyUpPressed();
	else if (event->key() == Qt::Key_Down)
		onKeyDownPressed();
	else if (event->key() == Qt::Key_Escape)
		onKeyEscapePressed();
	else
		QLineEdit::keyPressEvent(event);
}

void InputBox::onKeyUpPressed()
{
	currentHistoryEntry++;
	if (currentHistoryEntry == history.end())
	{
		currentHistoryEntry--;
	}
	else
	{
		setText(*currentHistoryEntry);
		selectAll();
	}
}

void InputBox::onKeyDownPressed()
{
	if (currentHistoryEntry != history.begin())
	{
		currentHistoryEntry--;
		setText(*currentHistoryEntry);
		selectAll();
	}
}

void InputBox::onKeyEscapePressed()
{
	clear();
}

void InputBox::onTextEdited(const QString & text)
{
	*history.begin() = text;
}
