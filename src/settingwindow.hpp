/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#ifndef SETTINGWINDOW_HPP
#define SETTINGWINDOW_HPP

// -*- C++ -*- generated by wxGlade 0.6.5 on Tue May 21 00:18:19 2013

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/splitter.h>
#include <wx/treectrl.h>
// end wxGlade
#include "enums.hpp"
#include "otherpanels.hpp"

// begin wxGlade: ::extracode
// end wxGlade

class SettingDialog: public wxDialog {

public:
     // begin wxGlade: SettingDialog::ids
     // end wxGlade

     /**
      * 設定画面のコンストラクタ
      */
     SettingDialog(wxWindow* parent, 
		   int id, 
		   const wxString& title, 
		   const wxPoint& pos=wxDefaultPosition, 
		   const wxSize& size=wxDefaultSize, 
		   long style=wxDEFAULT_DIALOG_STYLE);

private:
     // begin wxGlade: SettingDialog::methods
     void SetProperties();
     void DoLayout();
     // end wxGlade

     void OnQuit(wxCommandEvent& event);
     void OnChangeSettingPanel(wxTreeEvent& event);

protected:
     // begin wxGlade: SettingDialog::attributes
     wxTreeCtrl* settingTreeCtrl;
     wxPanel* treePanel;
     wxPanel* settingPanel;
     wxSplitterWindow* splitterWindow;
     wxPanel* spacePanel;
     wxButton* okButton;
     wxButton* cancelButton;
     wxPanel* bottomPanel;
     // end wxGlade
     DECLARE_EVENT_TABLE()
}; // wxGlade: end class


#endif // SETTINGWINDOW_H