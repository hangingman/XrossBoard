/* XrossBoard - a text board site viewer for open BBS
 * Copyright (C) 2011-2015 Hiroyuki Nagata
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

#include "xrossboarduiutil.hpp"
#include "xrossboardutil.hpp"
#include "xrossboard.hpp"

/**
 * wxTreeCtrlのインスタンスを受け取って共通の設定を行う
 *
 * @param wxTreeCtrl* treeCtrl 設定対象のツリー
 * @param const wxWindowID id  設定対象のGUIの部位を表すID
 */
void XrossBoardUiUtil::SetTreeCtrlCommonSetting(wxTreeCtrl* treeCtrl, const wxWindowID id) 
{
     // プロパティファイルにフォント設定/背景色があれば使用する
     wxString widgetsName = wxT("ID_TreeFontButton");
     wxString widgetsInfo = wxEmptyString;
     XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxFont font;
	  bool ret = font.SetNativeFontInfoUserDesc(widgetsInfo);
	  if(ret) treeCtrl->SetFont(font);
     }
     widgetsName = wxT("ID_BoardListBGColorButton");
     widgetsInfo.Clear();
     XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxColour bgColor;
	  bool ret = bgColor.Set(widgetsInfo);
	  if(ret) treeCtrl->SetBackgroundColour(bgColor);
     }

     wxTreeItemData treeData;
     wxTreeItemId m_rootId;

     // イメージリストにアイコンを登録する
     wxImageList* treeImage = new wxImageList(16, 16);
     wxBitmap idx1(folderImg, wxBITMAP_TYPE_PNG);
     treeImage->Add(idx1);
     wxBitmap idx2(textHtmlImg, wxBITMAP_TYPE_PNG);
     treeImage->Add(idx2);
     treeCtrl->AssignImageList(treeImage);

     // ツリー部分へのカーソル合わせが起きた場合のイベント通知
     treeCtrl->Connect(id,
		       wxEVT_ENTER_WINDOW,
		       wxMouseEventHandler(XrossBoard::OnEnterWindow),
		       NULL, wxWindow::FindWindowById(ID_WxXrossBoard));

     switch (id) 
     {

     case ID_BoardTreectrl:
     {	  
	  treeCtrl->SetLabel(BOARD_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("2ch板一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     break;

     case ID_FavsTreectrl:
     {
	  treeCtrl->SetLabel(FAVS_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("お気に入り一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     break;
	  
     case ID_NowReadingTreectrl:
     {
	  treeCtrl->SetLabel(NOW_READ_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("閲覧中一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     
     break;
     }
};


/**
 * wxWidgetsのイベント通知関数のラッパー
 *
 * @param const wxWindowID type 
 * @param const wxWindowID id
 * @param const wxString&  m
 * @param wxObject* o
 */
void XrossBoardUiUtil::QueueEventHelper(const wxWindowID type, const wxWindowID id, const wxString& m, wxObject* o)
{
     wxCommandEvent* e = new wxCommandEvent(type, id);

     if (!m.IsEmpty()) {
	  e->SetString(m.c_str());
     }
     
     if (o) {
	  e->SetEventObject(o);
     }

#if wxCHECK_VERSION(2, 9, 0)
     wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(e->Clone());
#else
     wxWindow::FindWindowById(ID_WxXrossBoard)->AddPendingEvent(*e);
#endif    
};


/**
 * XrossBoardのログ処理のラッパー
 *
 * @param const wxString& message
 */
void XrossBoardUiUtil::SendLoggingHelper(const wxString& message)
{
     XrossBoardUiUtil::QueueEventHelper(wxEVT_COMMAND_TEXT_UPDATED, ID_Logging, message);
};
