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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"
#include <gtkmm.h>

using namespace Gtk;
using namespace Glib;

class MainWindow : public Window
{
public:
    MainWindow();
    ~MainWindow() {};

private:
    void onOkButtonClicked();
    bool onInputBoxEnter(GdkEventKey * key);

    VBox mVBoxLayout;
    HBox mHBoxLayout;

    ScrolledWindow mHistoryScrolledWindow;
    TextView mHistoryBox;
    Entry mInputBox;
    RefPtr<TextBuffer> mHistoryTextBuffer;
    RefPtr<TextBuffer::TagTable> mTagTable;
    RefPtr<TextBuffer::Tag> mBlueTag, mGreenTag, mRedTag;

    Button mOkButton;

    Parser mParser;
};

#endif // MAINWINDOW_H
