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

#include "mainwindow.h"

using namespace Gdk;
using namespace Gtk;
using namespace Pango;
using namespace std;

/**
    Constructs main window.
*/
MainWindow::MainWindow()
{
    set_title("MaxCalc");
    set_border_width(5);
    set_default_size(750, 550);

    add(mVBoxLayout);

    mHistoryScrolledWindow.add(mHistoryBox);
    mHistoryScrolledWindow.set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);

    mVBoxLayout.pack_start(mHistoryScrolledWindow);
    mVBoxLayout.pack_start(mHBoxLayout, PACK_SHRINK);
    mHBoxLayout.pack_start(mInputBox);
    mHBoxLayout.pack_start(mOkButton, PACK_SHRINK);

    mOkButton.set_label("OK");
    mOkButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onOkButtonClicked));
    mInputBox.signal_key_release_event().connect(sigc::mem_fun(*this, &MainWindow::onInputBoxEnter));

    // Create tag table
    mTagTable = TextBuffer::TagTable::create();
    mBlueTag = TextBuffer::Tag::create();
    mBlueTag->property_foreground() = "blue";
    mTagTable->add(mBlueTag);
    mGreenTag = TextBuffer::Tag::create();
    mGreenTag->property_foreground() = "darkgreen";
    mTagTable->add(mGreenTag);
    mRedTag = TextBuffer::Tag::create();
    mRedTag->property_foreground() = "red";
    mTagTable->add(mRedTag);

    // Create history box with text buffer
    mHistoryTextBuffer = TextBuffer::create(mTagTable);
    mHistoryBox.set_buffer(mHistoryTextBuffer);
    mHistoryBox.set_editable(false);
    mInputBox.modify_font(FontDescription("14"));

    mOkButton.grab_default();
    mInputBox.grab_focus();

    show_all_children();
}

/**
    OK button clicked.
*/
void MainWindow::onOkButtonClicked()
{
    ustring ustr = mInputBox.get_text();
    if (ustr == "") return;
    mHistoryTextBuffer->insert_with_tag(mHistoryTextBuffer->end(), ustr + "\n",
        mBlueTag);
    wstring wstr = stringToWideString(ustr);
    mParser.setExpression(wstr);
    try {
        wstr = mParser.parse().result().toWideString();
        mHistoryTextBuffer->insert_with_tag(mHistoryTextBuffer->end(),
            "    " + wideStringToString(wstr) + "\n", mGreenTag);
        mInputBox.set_text("");
    } catch (MaxCalcException & ex) {
        wstr = ex.toString();
        mHistoryTextBuffer->insert_with_tag(mHistoryTextBuffer->end(),
            "    " + wideStringToString(wstr) + "\n", mRedTag);
    }
    mInputBox.grab_focus();
}

/**
    <Enter> pressed in input box.
*/
bool MainWindow::onInputBoxEnter(GdkEventKey * key)
{
    if (key->keyval == GDK_Return || key->keyval == GDK_KP_Enter) {
        onOkButtonClicked();
    }
}
