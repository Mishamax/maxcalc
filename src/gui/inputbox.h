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

#ifndef INPUTBOX_H
#define INPUTBOX_H

// Qt
#include <QLineEdit>
#include <QKeyEvent>
#include <QList>

class InputBox : public QLineEdit
{
    Q_OBJECT

public:
    InputBox(QWidget * parent = 0);

public slots:
    void addTextToHistory();

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    QList<QString> mHistory;
    QList<QString>::iterator mCurrentHistoryEntry;

    void onKeyUpPressed();
    void onKeyDownPressed();
    void onKeyEscapePressed();

private slots:
    void onTextEdited(const QString & text);
};


#endif // INPUTBOX_H
