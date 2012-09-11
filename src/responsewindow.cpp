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

#include "responsewindow.hpp"

#ifndef __WXMSW__
#include"../rc/janeclone.xpm"
#endif

// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Sep 11 22:14:45 2012

// begin wxGlade: ::extracode
// end wxGlade

ResponseWindow::ResponseWindow(wxWindow* parent, const wxString& title):
     wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{

     // アイコンの設定を行う
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(janeclone));
#endif

     // begin wxGlade: ResponseWindow::ResponseWindow
     resNoteBook = new wxNotebook(this, ID_ResponseWindow, wxDefaultPosition, wxDefaultSize, 0);
     settingPane = new wxPanel(resNoteBook, wxID_ANY);
     localRulePane = new wxPanel(resNoteBook, wxID_ANY);
     previewPane = new wxPanel(resNoteBook, wxID_ANY);
     resPane = new wxPanel(resNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
     boardNameTitle = new wxStaticText(resPane, wxID_ANY, wxT("【板名】 - スレッドタイトル"));
     name = new wxStaticText(resPane, wxID_ANY, wxT("名前："));
     const wxString *nameCombo_choices = NULL;
     nameCombo = new wxComboBox(resPane, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, nameCombo_choices, wxCB_DROPDOWN);
     mail = new wxStaticText(resPane, wxID_ANY, wxT("メール"));
     const wxString *mailCombo_choices = NULL;
     mailCombo = new wxComboBox(resPane, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, mailCombo_choices, wxCB_DROPDOWN);
     sageCheck = new wxCheckBox(resPane, wxID_ANY, wxT("sage"));
     text_ctrl_1 = new wxTextCtrl(resPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxHSCROLL);
     previewWindow = new wxHtmlWindow(previewPane, wxID_ANY);
     localRuleWindow = new wxHtmlWindow(localRulePane, wxID_ANY);
     settingText = new wxTextCtrl(settingPane, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
     handleCheck = new wxCheckBox(this, wxID_ANY, wxT("コテハン記憶"));
     frontCheck = new wxCheckBox(this, wxID_ANY, wxT("最前面"));
     formatCheck = new wxCheckBox(this, wxID_ANY, wxT("末尾整形"));
     beCheck = new wxCheckBox(this, wxID_ANY, wxT("BE"));
     const wxString *beMailCombo_choices = NULL;
     beMailCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, beMailCombo_choices, wxCB_DROPDOWN);
     proxyCheck = new wxCheckBox(this, wxID_ANY, wxT("Proxy"));
     const wxString *proxyCombo_choices = NULL;
     proxyCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, proxyCombo_choices, wxCB_DROPDOWN);
     space = new wxPanel(this, wxID_ANY);
     postButton = new wxButton(this, wxID_ANY, wxT("書き込み"));
     quitButton = new wxButton(this, wxID_ANY, wxT("やめる"));

     set_properties(title);
     do_layout();
     // end wxGlade
}


void ResponseWindow::set_properties(const wxString& title)
{
    // begin wxGlade: ResponseWindow::set_properties
    SetTitle(wxT("『") + title + wxT("』にレス"));
    SetSize(wxSize(640, 480));
    // end wxGlade
}


void ResponseWindow::do_layout()
{
    // begin wxGlade: ResponseWindow::do_layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hboxOther2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hboxOther1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* settingVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* localRuleVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* previewVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* resVbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    resVbox->Add(boardNameTitle, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(name, 0, wxALL|wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(nameCombo, 1, 0, 0);
    hbox1->Add(mail, 0, wxALIGN_CENTER_VERTICAL, 0);
    hbox1->Add(mailCombo, 0, 0, 0);
    hbox1->Add(sageCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    resVbox->Add(hbox1, 0, wxEXPAND, 0);
    resVbox->Add(text_ctrl_1, 1, wxEXPAND, 0);
    resPane->SetSizer(resVbox);
    previewVbox->Add(previewWindow, 1, wxEXPAND, 0);
    previewPane->SetSizer(previewVbox);
    localRuleVbox->Add(localRuleWindow, 1, wxEXPAND, 0);
    localRulePane->SetSizer(localRuleVbox);
    settingVbox->Add(settingText, 1, wxEXPAND, 0);
    settingPane->SetSizer(settingVbox);
    resNoteBook->AddPage(resPane, wxT("書き込み"));
    resNoteBook->AddPage(previewPane, wxT("プレビュー"));
    resNoteBook->AddPage(localRulePane, wxT("ローカルルール"));
    resNoteBook->AddPage(settingPane, wxT("SETTING.TXT"));
    vbox->Add(resNoteBook, 1, wxEXPAND, 0);
    hboxOther1->Add(handleCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(frontCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(formatCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(beCheck, 0, wxALIGN_CENTER_VERTICAL, 0);
    hboxOther1->Add(beMailCombo, 0, wxALIGN_CENTER_VERTICAL, 0);
    vbox->Add(hboxOther1, 0, wxEXPAND, 0);
    hboxOther2->Add(proxyCheck, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    hboxOther2->Add(proxyCombo, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 0);
    hboxOther2->Add(space, 1, wxEXPAND, 0);
    hboxOther2->Add(postButton, 0, wxALIGN_RIGHT, 0);
    hboxOther2->Add(quitButton, 0, wxALIGN_RIGHT, 0);
    vbox->Add(hboxOther2, 0, wxEXPAND, 0);
    SetSizer(vbox);
    Layout();
    // end wxGlade
}
