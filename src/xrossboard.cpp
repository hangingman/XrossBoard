﻿/* XrossBoard - a text board site viewer for open BBS
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

#include <algorithm>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/tokenzr.h>
#include <wx/filesys.h>
#include <wx/filename.h>
#include <wx/mstream.h>
#include <wx/datstrm.h>
#include <wx/aboutdlg.h>
#include <wx/textfile.h>
#include <wx/dialog.h>
#include <wx/txtstrm.h>
#include <wx/file.h>
#include <wx/clipbrd.h>

#include <curl/curl.h>
#include <curlpp/cURLpp.hpp>
#include <babel.h>

#include "xrossboard.hpp"
#include "enums.hpp"
#include "extractboardlist.hpp"
#include "socketcommunication.hpp"
#include "virtualboardlistctrl.hpp"
#include "sqliteaccessor.hpp"
#include "xrossboardutil.hpp"
#include "threadcontentbar.hpp"
#include "anchoredresponsepopup.hpp"
#include "responsewindow.hpp"
#include "settingwindow.hpp"
#include "otherpanels.hpp"

#include "wx/wx_twitter_notebook.hpp"

#ifndef __WXMSW__
   #include"../rc/xrossboard.xpm"
#endif

#ifdef USE_WX_WEBVIEW
   #include "threadcontentwebview.hpp"
#else
   #include "threadcontentwindow.hpp"
#endif

// event table
BEGIN_EVENT_TABLE(XrossBoard, wxFrame)
   // メニューバー・ポップアップメニューにあるコマンド入力で起動するメソッドのイベントテーブル
   EVT_MENU(wxID_EXIT, XrossBoard::OnQuit)
   EVT_MENU(ID_Quit, XrossBoard::OnQuit)
   EVT_MENU(ID_Restart, XrossBoard::OnRestart)
   EVT_MENU(ID_WindowMinimize, XrossBoard::WindowMinimize)
   EVT_MENU(ID_GetBoardList, XrossBoard::OnGetBoardList)
   EVT_MENU(ID_CheckLogDirectory, XrossBoard::CheckLogDirectory)
   EVT_MENU(ID_GetVersionInfo, XrossBoard::OnVersionInfo)
   EVT_MENU(ID_OneBoardTabClose, XrossBoard::OneBoardTabClose)
   EVT_MENU(ID_ExcepSelTabClose, XrossBoard::ExcepSelTabClose)
   EVT_MENU(ID_AllBoardTabClose, XrossBoard::AllBoardTabClose)
   EVT_MENU(ID_AllLeftBoardTabClose, XrossBoard::AllLeftBoardTabClose)
   EVT_MENU(ID_AllRightBoardTabClose, XrossBoard::AllRightBoardTabClose)
   EVT_MENU(ID_OnOpenBoardByBrowser, XrossBoard::OnOpenBoardByBrowser)
   EVT_MENU(ID_ReloadOneBoard, XrossBoard::ReloadOneBoard)
   EVT_MENU(ID_ReloadAllBoard, XrossBoard::ReloadAllBoard)
   EVT_MENU(ID_CopyBURLToClipBoard, XrossBoard::CopyBURLToClipBoard)
   EVT_MENU(ID_CopyBTitleToClipBoard, XrossBoard::CopyBTitleToClipBoard)
   EVT_MENU(ID_CopyBBothDataToClipBoard, XrossBoard::CopyBBothDataToClipBoard)
   EVT_MENU(ID_DeleteBSelectedDatFile,       XrossBoard::DeleteBSelectedDatFile)     
   EVT_MENU(ID_DeleteBAllDatFile,            XrossBoard::DeleteBAllDatFile)        
   EVT_MENU(ID_DeleteBAllDatFileWithoutFav,  XrossBoard::DeleteBAllDatFileWithoutFav)
   EVT_MENU(ID_CopyTURLToClipBoard, XrossBoard::CopyTURLToClipBoard)
   EVT_MENU(ID_CopyTTitleToClipBoard, XrossBoard::CopyTTitleToClipBoard)
   EVT_MENU(ID_CopyTBothDataToClipBoard, XrossBoard::CopyTBothDataToClipBoard)
   EVT_MENU(ID_OneThreadTabClose, XrossBoard::OneThreadTabClose)
   EVT_MENU(ID_ExcepSelThreadTabClose, XrossBoard::ExcepSelThreadTabClose)
   EVT_MENU(ID_AllThreadTabClose, XrossBoard::AllThreadTabClose)
   EVT_MENU(ID_AllLeftThreadTabClose, XrossBoard::AllLeftThreadTabClose)
   EVT_MENU(ID_AllRightThreadTabClose, XrossBoard::AllRightThreadTabClose)
   EVT_MENU(ID_OnOpenThreadByBrowser, XrossBoard::OnOpenThreadByBrowser)
   EVT_MENU(ID_SaveDatFile, XrossBoard::SaveDatFile)
   EVT_MENU(ID_SaveDatFileToClipBoard, XrossBoard::SaveDatFileToClipBoard)
   EVT_MENU(ID_DeleteDatFile, XrossBoard::DeleteDatFile)
   EVT_MENU(ID_ReloadThisThread, XrossBoard::ReloadThisThread)
   EVT_MENU(ID_CallResponseWindow, XrossBoard::CallResponseWindow)
   EVT_MENU(ID_OnOpenXrossBoardOfficial, XrossBoard::OnOpenXrossBoardOfficial)
   EVT_MENU(ID_AddThreadFavorite, XrossBoard::AddThreadFavorite)
   EVT_MENU(ID_DelThreadFavorite, XrossBoard::DelThreadFavorite)
   EVT_MENU(ID_AddAllThreadFavorite, XrossBoard::AddAllThreadFavorite)
   EVT_MENU(ID_AddBoardFavorite,  XrossBoard::AddBoardFavorite) 	
   EVT_MENU(ID_DelBoardFavorite,  XrossBoard::DelBoardFavorite)
   EVT_MENU(ID_MoveNextTab,       XrossBoard::MoveNextTab)   
   EVT_MENU(ID_MovePrevTab,       XrossBoard::MovePrevTab)
   EVT_MENU(ID_CallNewBoardAddDialog, XrossBoard::CallNewBoardAddDialog)
   EVT_MENU_RANGE(ID_UserLastClosedThreadClick, ID_UserLastClosedThreadClick + 99, XrossBoard::OnUserLastClosedThreadClick)
   EVT_MENU_RANGE(ID_UserLastClosedBoardClick,  ID_UserLastClosedBoardClick  + 99, XrossBoard::OnUserLastClosedBoardClick)
   EVT_MENU_RANGE(ID_UserFavoriteThreadClick,   ID_UserFavoriteThreadClick   + 99, XrossBoard::OnUserFavoriteThreadClick)

   // メニューバーからスレッド一覧リストをソート
   EVT_MENU(ID_OnClickMenuCOL_CHK,      XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_NUM,      XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_TITLE,    XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_RESP,     XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_CACHEDRES,XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_NEWRESP,  XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_INCRESP,  XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_MOMENTUM, XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_LASTUP,   XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_SINCE,    XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_OID,      XrossBoard::OnThreadListSort)
   EVT_MENU(ID_OnClickMenuCOL_BOARDNAME,XrossBoard::OnThreadListSort)

   // ツールバーからの命令
   EVT_MENU(ID_ShowBoardListTree, XrossBoard::ShowBoardListTree)
   EVT_MENU(ID_SwitchSeparateXY, XrossBoard::SwitchSeparateXY)
   EVT_MENU(ID_SwitchTwoThreePane, XrossBoard::SwitchTwoThreePane)
   EVT_MENU(ID_SwitchRightPane, XrossBoard::SwitchRightPane)
   EVT_MENU(ID_CallSettingWindow, XrossBoard::CallSettingWindow)
   EVT_MENU(ID_CallViewerSettingWindow, XrossBoard::CallViewerSettingWindow)

   // 検索バー系の命令
   EVT_MENU(ID_ThreadSearchBoxDoSearch, XrossBoard::ThreadSearchBoxDoSearch)
   EVT_MENU(ID_BoardSearchBoxDoSearch, XrossBoard::BoardSearchBoxDoSearch)
   EVT_MENU(ID_SearchBarHide, XrossBoard::HideSearchBar)
   EVT_MENU(ID_SearchBoxUp, XrossBoard::SearchBoxUp)
   EVT_MENU(ID_SearchBoxDown, XrossBoard::SearchBoxDown)
   EVT_MENU(ID_SearchBoxCopy, XrossBoard::SearchBoxCopy)
   EVT_MENU(ID_SearchBoxCut, XrossBoard::SearchBoxCut)
   EVT_MENU(ID_SearchBoxSelectAll, XrossBoard::SearchBoxSelectAll)
   EVT_MENU(ID_SearchBoxClear, XrossBoard::SearchBoxClear)
    
   // 動的に項目を追加するメニューでのイベント
   EVT_MENU_OPEN(XrossBoard::OnMenuOpen)
   EVT_UPDATE_UI(ID_UserLastClosedThreadMenuUp, XrossBoard::UserLastClosedThreadMenuUp)
   EVT_UPDATE_UI(ID_UserLastClosedBoardMenuUp, XrossBoard::UserLastClosedBoardMenuUp)
   EVT_UPDATE_UI(ID_UserLookingTabsMenuUp, XrossBoard::UserLookingTabsMenuUp)
   EVT_UPDATE_UI(ID_NowReadingTreectrlUpdate, XrossBoard::NowReadingTreectrlUpdate)
    
   // 2ch板一覧ツリーコントロールのイベント
   EVT_TREE_SEL_CHANGED(ID_BoardTreectrl, XrossBoard::OnGetBoardInfo)

#ifdef USE_SHINGETSU
   // 新月公開ノードツリーコントロールのイベント
   EVT_TREE_SEL_CHANGED(ID_ShingetsuBoardTreectrl, XrossBoard::OnGetShingetsuNodeInfo)
   EVT_TREE_ITEM_RIGHT_CLICK(ID_ShingetsuBoardTreectrl, XrossBoard::OnRightClickShingetsuNodeTree)
#endif /** USE_SHINGETSU */

   // 板一覧ノートブックで右クリックされた時の処理
   EVT_AUINOTEBOOK_TAB_RIGHT_DOWN(ID_BoardNoteBook, XrossBoard::OnRightClickBoardNoteBook)
   // スレッド一覧ノートブックで右クリックされた時の処理
   EVT_AUINOTEBOOK_TAB_RIGHT_DOWN(ID_ThreadNoteBook, XrossBoard::OnRightClickThreadNoteBook)
    
   // 板一覧ノートブックで、タブが消される前の処理
   EVT_AUINOTEBOOK_PAGE_CLOSE(ID_BoardNoteBook, XrossBoard::OnAboutCloseBoardNoteBook)
   // スレッド一覧ノートブックで、タブが消される前の処理
   EVT_AUINOTEBOOK_PAGE_CLOSE(ID_ThreadNoteBook, XrossBoard::OnAboutCloseThreadNoteBook)
    
   // AuiNotebookのタブを変更中の処理
   EVT_AUINOTEBOOK_PAGE_CHANGING(ID_BoardNoteBook,  XrossBoard::OnChangeBoardTab)
   EVT_AUINOTEBOOK_PAGE_CHANGING(ID_ThreadNoteBook, XrossBoard::OnChangeThreadTab)
    
   // AuiNotebookのタブを変更し終わった時の処理
   EVT_AUINOTEBOOK_PAGE_CHANGED(ID_BoardNoteBook,  XrossBoard::OnChangedBoardTab)
   EVT_AUINOTEBOOK_PAGE_CHANGED(ID_ThreadNoteBook, XrossBoard::OnChangedThreadTab)
    
   // フォーカスの監視
   EVT_SET_FOCUS(XrossBoard::OnSetFocus)
    
   // スレッド一覧リストでのクリック
   EVT_LIST_ITEM_SELECTED(ID_BoardListCtrl,          XrossBoard::OnLeftClickAtListCtrl)
   EVT_LIST_ITEM_SELECTED(ID_ShingetsuBoardListCtrl, XrossBoard::OnLeftClickAtListCtrl)
   EVT_LIST_COL_CLICK(ID_BoardListCtrl,          XrossBoard::OnLeftClickAtListCtrlCol)
   EVT_LIST_COL_CLICK(ID_ShingetsuBoardListCtrl, XrossBoard::OnLeftClickAtListCtrlCol)
   // 終了前処理
   EVT_CLOSE(XrossBoard::OnCloseWindow)
   // wxHtmlWindow上でのイベント処理
   EVT_HTML_CELL_HOVER(wxID_ANY, XrossBoard::OnCellHover)
   // URL入力ウィンドウのボタンイベント処理
   EVT_BUTTON(ID_URLWindowButton, XrossBoard::OnClickURLWindowButton)
   // ログ出力制御用イベント
   EVT_TEXT(ID_Logging, XrossBoard::Logging)
   EVT_TEXT(ID_ChangeUserLastAttached, XrossBoard::ChangeUserLastAttached)
   EVT_TEXT(ID_ReloadThreadByName, XrossBoard::ReloadThreadByName)
   EVT_TEXT(ID_ResetBoardListTree, XrossBoard::SetBoardList)

#ifdef __WXMAC__
   // UIの更新通 OS X(Cocoa)はGUIの更新をワーカースレッドから行えない 
   EVT_UPDATE_UI(ID_ThreadContentBarUpdate, XrossBoard::UpdateXrossBoardUI)
   EVT_UPDATE_UI(ID_SettingPanelUpdate, XrossBoard::UpdateXrossBoardUI)
   EVT_UPDATE_UI(ID_NetworkPanelUpdate, XrossBoard::UpdateXrossBoardUI)
   EVT_UPDATE_UI(ID_BoardTreectrl, XrossBoard::UpdateXrossBoardUI)
   EVT_UPDATE_UI(ID_FavsTreectrl, XrossBoard::UpdateXrossBoardUI)
   EVT_UPDATE_UI(ID_NowReadingTreectrl, XrossBoard::UpdateXrossBoardUI)
#endif

END_EVENT_TABLE()

// 画像ビューアのインスタンスを初期化
XrossBoardImageViewer* XrossBoard::imageViewer = NULL;
// ユーザーが最後に触ったノートブック情報を初期化
wxString XrossBoard::userLastAttachedNotebook = wxEmptyString;
bool XrossBoard::restartAppFlag = false;

// インスタンスを渡す処理
XrossBoardImageViewer* XrossBoard::GetXrossBoardImageViewer () 
{
     if (imageViewer != NULL) 
     {
	  return imageViewer;
     }
     // !! ここ以下のコードは多分動かない !!
     // 画像ビューアのインスタンスを作る
     imageViewer = new XrossBoardImageViewer(NULL, wxID_ANY, wxT("画像ビューア"));
     // 通常は隠しておく
     imageViewer->Show(false);     
     return imageViewer;
}

/**
 * 現在XrossBoardが保持しているスレッド情報を取得する
 */
void XrossBoard::GetThreadInfoHash(ThreadInfoHash& threadInfoHash) 
{
     if (!this->tiHash.empty()) 
     {
	  threadInfoHash = ThreadInfoHash(this->tiHash); // copy
     }
}
/**
 * 現在XrossBoardが保持しているスレッド情報を設定する
 */
void XrossBoard::SetThreadInfoHash(ThreadInfoHash& threadInfoHash) 
{
     if (!threadInfoHash.empty()) 
     {
	  this->tiHash.clear();		 // delete
	  this->tiHash = threadInfoHash; // copy
     }
}

/**
 * 現在XrossBoardが保持している画像情報を取得する
 */
void XrossBoard::GetHashedImageFileSet(std::set<wxString>& hashedImageFileSet)
{
     if (!this->hashedImageFileSet.empty()) 
     {
	  hashedImageFileSet = std::set<wxString>(this->hashedImageFileSet); // copy
     }
}

/**
 * 現在XrossBoardが保持している画像情報を設定する
 */
void XrossBoard::SetHashedImageFileSet(std::set<wxString>& hashedImageFileSet)
{
     if (!hashedImageFileSet.empty()) 
     {
	  this->hashedImageFileSet.clear();              // delete
	  this->hashedImageFileSet = hashedImageFileSet; // copy
     }
}

/**
 * XrossBoardのコンストラクタ：すべての起点
 */
XrossBoard::XrossBoard(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
     wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
     // アイコンの設定
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(xrossboard));
#endif

     /**
      * 必要なwxWindowを宣言する
      */
     // 検索バー
     m_search_ctrl = new wxSearchCtrl((wxWindow*)this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
     // アイコン付きのツールバー
     m_floatToolBar = new wxAuiToolBar(this, ID_FloatToolBar, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE);
     // URL入力欄
     m_url_input_panel = new wxPanel(this, wxID_ANY);
     m_url_input = new wxTextCtrl(m_url_input_panel, wxID_ANY, m_url_text, wxDefaultPosition, wxDefaultSize);
     m_url_input_button = new wxBitmapButton(m_url_input_panel, ID_URLWindowButton, wxBitmap(goNextImg, wxBITMAP_TYPE_ANY));

     // ログ出力ウィンドウ
     m_logCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

     *m_logCtrl << wxT("(ヽ´ん`)…デバッグ用画面…\n");
     // ステータスバー設置
     this->CreateStatusBar(2);

     // 板一覧ツリーを載せるノートブック
     boardTreeNoteBook = new wxAuiNotebook(this, 
					   ID_BoardTreeNoteBook, 
					   wxDefaultPosition, 
					   wxDefaultSize, 
					   wxAUI_NB_TAB_FIXED_WIDTH|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TOP);
     boardTreeNoteBook->Connect(wxID_ANY,
				wxEVT_ENTER_WINDOW,
				wxMouseEventHandler(XrossBoard::OnEnterWindow),
				NULL, this);

     // 2ch板一覧用ページ
     m_boardTreePanel = new wxPanel(boardTreeNoteBook);
     boardTreeNoteBook->AddPage(m_boardTreePanel, wxT("2ch板一覧"), false);

     // お気に入り用ページ
     m_favoriteTreePanel = new wxPanel(boardTreeNoteBook);
     boardTreeNoteBook->AddPage(m_favoriteTreePanel, wxT("お気に入り"), false);

     // 閲覧中
     m_nowReadingTreePanel = new wxPanel(boardTreeNoteBook);
     boardTreeNoteBook->AddPage(m_nowReadingTreePanel, wxT("閲覧中"), false);

#ifdef USE_SHINGETSU
     // 新月の公開ノード用ページ
     m_shingetsuTreePanel = new wxPanel(boardTreeNoteBook);
     boardTreeNoteBook->AddPage(m_shingetsuTreePanel, wxT("新月公開ノード一覧"), false);
#endif /** USE_SHINGETSU */

     // 各種GUI設定を行う
     SetProperties();       // 前回までの設定を読み出す
     DoLayout();            // 実際にレイアウトに展開する
     SetXrossBoardMenuBar(); // メニューバーを設定する

     // ユーザーが前回までに見ていた板一覧タブとスレッド一覧タブをセットする
     SetPreviousUserLookedTab();
     boardNoteBook->Update();
     threadNoteBook->Update();

     // 閲覧中ツリーの情報を更新する
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, ID_NowReadingTreectrlUpdate);

     // フォーカスイベントをバインドする
     boardNoteBook->Connect(wxID_ANY,
			    wxEVT_ENTER_WINDOW,
			    wxMouseEventHandler(XrossBoard::OnEnterWindow),
			    NULL, this);
     threadNoteBook->Connect(wxID_ANY,
			     wxEVT_ENTER_WINDOW,
			     wxMouseEventHandler(XrossBoard::OnEnterWindow),
			     NULL, this);

     SetStatusText(wxT(" 完了"));
     *m_logCtrl << wxT("(ヽ´ん`)…レイアウト設定終わりです…\n");
}
/**
 * SetXrossBoardMenuBar
 * メニューバーの設定を行う
 */
void XrossBoard::SetXrossBoardMenuBar() 
{
     /*
      * メニューバーの設置
      */
     wxMenuBar *menuBar = new wxMenuBar;

     /**
      * 必要なメニューのインスタンスは先に確保する
      */
     wxMenu* menu1  = new wxMenu;
     wxMenu* menu2  = new wxMenu;
     wxMenu* menu3  = new wxMenu;
     wxMenu* menu4  = new wxMenu;
     wxMenu* menu5  = new wxMenu;
     wxMenu* menu6  = new wxMenu;
     wxMenu* menu7  = new wxMenu;
     wxMenu* menu8  = new wxMenu;
     wxMenu* menu9  = new wxMenu;
     wxMenu* menu10 = new wxMenu;

     /** メニューバーにメニューをセットしてタイトルを付けておく */
     menuBar->Append(menu1, wxT("ファイル"));
     menu1->SetTitle(wxT("ファイル"));
     menuBar->Append(menu2, wxT("表示"));
     menu2->SetTitle(wxT("表示"));
     menuBar->Append(menu3, wxT("板覧"));
     menu3->SetTitle(wxT("板覧"));
     menuBar->Append(menu4, wxT("スレ覧"));
     menu4->SetTitle(wxT("スレ覧"));
     menuBar->Append(menu5, wxT("スレッド"));
     menu5->SetTitle(wxT("スレッド"));
     menuBar->Append(menu6, wxT("お気に入り"));
     menu6->SetTitle(wxT("お気に入り"));
     menuBar->Append(menu7, wxT("検索"));
     menu7->SetTitle(wxT("検索"));
     menuBar->Append(menu8, wxT("ウィンドウ"));
     menu8->SetTitle(wxT("ウィンドウ"));
     menuBar->Append(menu9, wxT("ツール"));
     menu9->SetTitle(wxT("ツール"));
     menuBar->Append(menu10, wxT("ヘルプ"));
     menu10->SetTitle(wxT("ヘルプ"));

     /**
      * ファイル部分
      */
     menu1->AppendSeparator();
     this->closeT = new wxMenu;
     // 動的に項目を補充する
     closeT->Append(ID_UserLastClosedThreadMenuUp, wxT("最後に閉じたスレを開く"), wxT("最後に閉じたスレを開きます"));
     closeT->AppendSeparator();

     menu1->AppendSubMenu(closeT, wxT("最近閉じたスレ"), wxT("最近閉じたスレを開きます"));
     this->closeB = new wxMenu;
     // 動的に項目を補充する
     closeB->Append(ID_UserLastClosedBoardMenuUp, wxT("最後に閉じた板を開く"), wxT("最後に閉じた板を開きます"));
     closeB->AppendSeparator();

     menu1->AppendSubMenu(closeB, wxT("最近閉じた板"));
     menu1->AppendSeparator();
     menu1->Append(ID_Restart, wxT("再起動"), wxT("XrossBoardを再起動します"));
     menu1->Append(ID_Quit, wxT("終了"), wxT("XrossBoardを終了します"));
     /**
      * 表示部分
      */
     wxMenu *toolBar   = new wxMenu;
     toolBar->Append(wxID_ANY, wxT("メニュー"));
     toolBar->Append(wxID_ANY, wxT("メインツールバー"));
     toolBar->Append(wxID_ANY, wxT("リンク"));
     toolBar->Append(wxID_ANY, wxT("アドレスバー"));
     toolBar->Append(wxID_ANY, wxT("移動ボタン"));
     toolBar->Append(wxID_ANY, wxT("メイン検索バー"));
     toolBar->AppendSeparator();
     toolBar->Append(wxID_ANY, wxT("ツールバーを固定する"));
     menu2->AppendSubMenu(toolBar, wxT("ツールバー"));
     menu2->AppendCheckItem(wxID_ANY, wxT("ステータスバー"));
     wxMenu *boardTree = new wxMenu;
     boardTree->AppendCheckItem(wxID_ANY, wxT("ステータスバー"));
     boardTree->AppendCheckItem(wxID_ANY, wxT("板ツリー全体"));
     boardTree->AppendSeparator();
     boardTree->AppendCheckItem(wxID_ANY, wxT("トップバー"));
     boardTree->AppendSeparator();
     boardTree->AppendRadioItem(wxID_ANY, wxT("板一覧"));
     boardTree->AppendRadioItem(wxID_ANY, wxT("お気に入り"));
     boardTree->AppendRadioItem(wxID_ANY, wxT("閲覧中"));
     menu2->AppendSubMenu(boardTree, wxT("板ツリー"));
     wxMenu *memoWin   = new wxMenu;
     memoWin->AppendCheckItem(wxID_ANY, wxT("メモ欄全体"));
     memoWin->AppendSeparator();
     memoWin->AppendCheckItem(wxID_ANY, wxT("書き込みバー"));
     memoWin->AppendCheckItem(wxID_ANY, wxT("メモツールバー"));
     memoWin->AppendCheckItem(wxID_ANY, wxT("設定バー"));
     memoWin->AppendSeparator();
     memoWin->AppendCheckItem(wxID_ANY, wxT("メモ欄ツールバーの固定"));
     menu2->AppendSubMenu(memoWin, wxT("メモ欄"));
     wxMenu *tab       = new wxMenu;
     tab->AppendCheckItem(wxID_ANY, wxT("ツリータブ"));
     tab->AppendCheckItem(wxID_ANY, wxT("スレ覧タブ"));
     tab->AppendCheckItem(wxID_ANY, wxT("スレタブ"));
     tab->AppendCheckItem(wxID_ANY, wxT("メモ欄タブ"));
     menu2->AppendSubMenu(tab, wxT("タブ"));
     wxMenu *searchBar = new wxMenu;
     searchBar->AppendCheckItem(wxID_ANY, wxT("スレ一覧検索バー"));
     searchBar->AppendCheckItem(wxID_ANY, wxT("スレ検索バー"));
     searchBar->AppendCheckItem(wxID_ANY, wxT("板ツリー検索バー"));
     menu2->AppendSubMenu(searchBar, wxT("検索バー"));
     menu2->Append(wxID_ANY, wxT("ビューア"));
     menu2->AppendCheckItem(wxID_ANY, wxT("Twitter"));
     menu2->AppendSeparator();
     menu2->Append(wxID_ANY, wxT("更新"));
     menu2->AppendSeparator();
     wxMenu *focus     = new wxMenu;
     focus->Append(wxID_ANY, wxT("次のペイン"));
     focus->Append(wxID_ANY, wxT("前のペイン"));
     focus->AppendSeparator();
     focus->Append(wxID_ANY, wxT("板"));
     focus->Append(wxID_ANY, wxT("お気に入り"));
     focus->Append(wxID_ANY, wxT("スレ一覧"));
     focus->Append(wxID_ANY, wxT("スレ"));
     focus->Append(wxID_ANY, wxT("メモ欄"));
     menu2->AppendSubMenu(focus, wxT("フォーカス"));
     wxMenu *fontSize  = new wxMenu;
     fontSize->AppendRadioItem(wxID_ANY, wxT("最大"));
     fontSize->AppendRadioItem(wxID_ANY, wxT("大"));
     fontSize->AppendRadioItem(wxID_ANY, wxT("中"));
     fontSize->AppendRadioItem(wxID_ANY, wxT("小"));
     fontSize->AppendRadioItem(wxID_ANY, wxT("最小"));
     menu2->AppendSubMenu(fontSize, wxT("文字のサイズ"));
     menu2->AppendSeparator();
     menu2->Append(ID_SwitchSeparateXY, wxT("縦⇔横分割切り替え"));
     menu2->Append(ID_SwitchTwoThreePane, wxT("２⇔３ペイン切り替え"));
     menu2->Append(wxID_ANY, wxT("右側切替"));

     /**
      * 板覧部分
      */
     menu3->Append(ID_GetBoardList, wxT("板一覧の更新"));
     menu3->AppendSeparator();
     menu3->Append(wxID_ANY, wxT("お気に入りを板として開く"));
     menu3->Append(wxID_ANY, wxT("ログ一覧を開く"));
     menu3->Append(wxID_ANY, wxT("開いているスレッドを板として開く"));
     menu3->Append(wxID_ANY, wxT("最近読み込んだスレッド"));
     menu3->Append(wxID_ANY, wxT("最近書き込んだスレッド"));
     menu3->AppendSeparator();
     menu3->Append(ID_CheckLogDirectory, wxT("ログフォルダのチェック"));
     menu3->AppendSeparator();
     menu3->Append(wxID_ANY, wxT("すべての板のインデックスを再構築"));
     menu3->Append(wxID_ANY, wxT("ログの再構築"));
     menu3->AppendSeparator();
     menu3->Append(wxID_ANY, wxT("ログ一覧制限変更"));
     menu3->Append(wxID_ANY, wxT("ログ一覧から取り込む"));
     menu3->AppendSeparator();
     menu3->Append(ID_CallNewBoardAddDialog, wxT("新規板登録"));
     /**
      * スレ覧部分
      */
     wxMenu *closeB = new wxMenu;
     closeB->Append(ID_OneBoardTabClose, wxT("現在の板を閉じる"));
     closeB->AppendSeparator();
     closeB->Append(ID_ExcepSelTabClose, wxT("選択されていない板を閉じる"));
     closeB->Append(ID_AllBoardTabClose, wxT("すべてのタブを閉じる"));
     closeB->Append(ID_AllLeftBoardTabClose, wxT("これより左を閉じる"));
     closeB->Append(ID_AllRightBoardTabClose, wxT("これより右を閉じる"));
     menu4->AppendSubMenu(closeB, wxT("閉じる"));
     menu4->AppendSeparator();
     wxMenu *open = new wxMenu;
     open->Append(wxID_ANY, wxT("新着をすべて開く"));
     open->Append(wxID_ANY, wxT("お気に入りの新着をすべて開く"));
     open->Append(wxID_ANY, wxT("新着スレッドをすべて開く"));
     open->AppendSeparator();
     open->Append(wxID_ANY, wxT("新着のあるスレッドを一件開く"));
     open->AppendSeparator();
     open->Append(wxID_ANY, wxT("新しいタブで開く"));
     open->Append(wxID_ANY, wxT("今のタブで開く"));
     open->Append(wxID_ANY, wxT("バックグラウンドで開く"));
     menu4->AppendSubMenu(open, wxT("開く"));
     menu4->AppendSeparator();
     menu4->Append(wxID_ANY, wxT("選択中のスレを閉じる"));
     menu4->AppendSeparator();
     menu4->Append(wxID_ANY, wxT("選択中のスレをタブロック"));
     menu4->Append(wxID_ANY, wxT("既読にする"));
     menu4->Append(wxID_ANY, wxT("印を付ける"));
     menu4->Append(ID_AddBoardFavorite, wxT("お気に入りに追加"));
     menu4->Append(ID_DelBoardFavorite, wxT("お気に入りを削除"));
     menu4->AppendSeparator();
     menu4->Append(ID_ReloadOneBoard, wxT("スレ一覧更新"));
     menu4->Append(ID_ReloadAllBoard, wxT("すべてのタブのスレ一覧更新"));
     menu4->Append(wxID_ANY, wxT("板移転の追尾"));
     menu4->AppendSeparator();
     menu4->Append(wxID_ANY, wxT("スレッド新規作成"));
     menu4->AppendSeparator();
     menu4->Append(ID_OnOpenBoardByBrowser, wxT("ブラウザで開く"));
     menu4->Append(wxID_ANY, wxT("看板を見る"));
     menu4->AppendSeparator();
     wxMenu *selectCopy = new wxMenu;
     selectCopy->Append(wxID_ANY, wxT("datをクリップボードにコピー"));
     menu4->AppendSubMenu(selectCopy, wxT("選択中のログをコピー"));
     wxMenu *copy = new wxMenu;
     copy->Append(ID_CopyBURLToClipBoard, wxT("URLをコピー"));
     copy->Append(ID_CopyBTitleToClipBoard, wxT("タイトルをコピー"));
     copy->Append(ID_CopyBBothDataToClipBoard, wxT("タイトルとURLをコピー"));
     menu4->AppendSubMenu(copy, wxT("コピー"));
     menu4->AppendSeparator();
     wxMenu *deleteLog = new wxMenu;
     deleteLog->Append(ID_DeleteBSelectedDatFile,      wxT("選択中のログを削除"));
     deleteLog->Append(ID_DeleteBAllDatFile,           wxT("すべてのログを削除"));
     deleteLog->Append(ID_DeleteBAllDatFileWithoutFav, wxT("お気に入り以外のログを削除"));
     menu4->AppendSubMenu(deleteLog, wxT("ログ削除"));
     menu4->AppendSeparator();
     menu4->Append(wxID_ANY, wxT("このスレをチェック"));
     menu4->AppendSeparator();
     wxMenu *abone = new wxMenu;
     abone->Append(wxID_ANY, wxT("スレッドあぼ～ん"));
     abone->Append(wxID_ANY, wxT("透明スレッドあぼ～ん"));
     abone->Append(wxID_ANY, wxT("あぼ～ん・チェックを解除"));
     menu4->AppendSubMenu(abone, wxT("あぼ～ん"));
     menu4->AppendSeparator();
     wxMenu *sort = new wxMenu;
     sort->Append(ID_OnClickMenuCOL_CHK,       wxT("1 !"));
     sort->Append(ID_OnClickMenuCOL_NUM,       wxT("2 番号"));
     sort->Append(ID_OnClickMenuCOL_TITLE,     wxT("3 タイトル"));
     sort->Append(ID_OnClickMenuCOL_RESP,      wxT("4 レス"));
     sort->Append(ID_OnClickMenuCOL_CACHEDRES, wxT("5 取得"));
     sort->Append(ID_OnClickMenuCOL_NEWRESP,   wxT("6 新着"));
     sort->Append(ID_OnClickMenuCOL_INCRESP,   wxT("7 増レス"));
     sort->Append(ID_OnClickMenuCOL_MOMENTUM,  wxT("8 勢い"));
     sort->Append(ID_OnClickMenuCOL_LASTUP,    wxT("9 最終取得"));
     sort->Append(ID_OnClickMenuCOL_SINCE,     wxT("0 SINCE"));
     sort->Append(ID_OnClickMenuCOL_OID,       wxT("a 固有番号"));
     sort->Append(ID_OnClickMenuCOL_BOARDNAME, wxT("b 板"));
     sort->AppendSeparator();
     sort->AppendCheckItem(wxID_ANY, wxT("開いているスレを上へソート"));
     sort->AppendCheckItem(wxID_ANY, wxT("重要スレを上へソート"));
     menu4->AppendSubMenu(sort, wxT("ソート"));
     wxMenu *aboneB = new wxMenu;
     aboneB->AppendRadioItem(wxID_ANY, wxT("透明"));
     aboneB->AppendRadioItem(wxID_ANY, wxT("標準"));
     aboneB->AppendRadioItem(wxID_ANY, wxT("あぼ～ん無効"));
     aboneB->AppendRadioItem(wxID_ANY, wxT("あぼ～んのみ"));
     aboneB->AppendRadioItem(wxID_ANY, wxT("重要スレのみ"));
     menu4->AppendSubMenu(aboneB, wxT("スレッドあぼ～んの表示変更"));
     menu4->AppendSeparator();
     menu4->Append(wxID_ANY, wxT("過去ログ非表示"));
     /**
      * スレッド部分
      */
     wxMenu *closeTh = new wxMenu;
     closeTh->Append(ID_OneThreadTabClose, wxT("選択中のタブを閉じる"));
     closeTh->Append(wxID_ANY, wxT("未読として閉じる"));
     closeTh->AppendSeparator();
     closeTh->Append(ID_ExcepSelThreadTabClose, wxT("選択されていないタブを閉じる"));
     closeTh->Append(ID_AllThreadTabClose, wxT("すべてのタブを閉じる"));
     closeTh->Append(ID_AllLeftThreadTabClose, wxT("これより左を閉じる"));
     closeTh->Append(ID_AllRightThreadTabClose, wxT("これより右を閉じる"));
     closeTh->AppendSeparator();
     closeTh->Append(wxID_ANY, wxT("新着なしのタブを閉じる"));
     closeTh->Append(wxID_ANY, wxT("dat落ちのタブを閉じる"));
     menu5->AppendSubMenu(closeTh, wxT("閉じる"));
     menu5->AppendSeparator();
     wxMenu *tabLock = new wxMenu;
     tabLock->Append(wxID_ANY, wxT("このタブをロックする"));
     tabLock->Append(wxID_ANY, wxT("すべてのタブをロックする"));
     tabLock->Append(wxID_ANY, wxT("これより左をロックする"));
     tabLock->Append(wxID_ANY, wxT("これより右をロックする"));
     menu5->AppendSubMenu(tabLock, wxT("タブロック"));
     menu5->AppendSeparator();
     menu5->Append(wxID_ANY, wxT("次スレ候補検索"));
     menu5->Append(wxID_ANY, wxT("次スレ候補を開く"));
     menu5->AppendSeparator();
     menu5->Append(wxID_ANY, wxT("ヒストリー検索"));
     menu5->AppendSeparator();
     menu5->Append(wxID_ANY, wxT("印を付ける"));
     wxMenu *addFav = new wxMenu;
     addFav->Append(ID_AddThreadFavorite, wxT("「お気に入り」に追加"));
     addFav->AppendSeparator();
     addFav->Append(wxID_ANY, wxT("「リンク」に追加"));
     menu5->AppendSubMenu(addFav, wxT("お気に入りに追加"));
     menu5->Append(wxID_ANY, wxT("お気に入りを削除"));
     menu5->AppendSeparator();
     menu5->Append(wxID_ANY, wxT("強制過去ログ化"));
     menu5->AppendSeparator();
     wxMenu *move = new wxMenu;
     move->Append(wxID_ANY, wxT("戻る"));
     move->Append(wxID_ANY, wxT("進む"));
     move->AppendSeparator();
     move->Append(wxID_ANY, wxT("閉じた付近に戻る"));
     move->Append(wxID_ANY, wxT("新着までスクロール"));
     move->Append(wxID_ANY, wxT("次のレス↓"));
     move->Append(wxID_ANY, wxT("前のレス↑"));
     move->AppendSeparator();
     move->Append(wxID_ANY, wxT("指定レス番号にジャンプ"));
     menu5->AppendSubMenu(move, wxT("移動"));
     menu5->AppendSeparator();
     menu5->Append(ID_ReloadThisThread, wxT("新着チェック"));
     menu5->Append(wxID_ANY, wxT("すべてのタブの新着チェック"));
     menu5->Append(wxID_ANY, wxT("すべてのタブの更新チェック"));
     menu5->Append(wxID_ANY, wxT("中止"));
     menu5->Append(ID_CallResponseWindow, wxT("レス"));
     menu5->AppendSeparator();
     menu5->Append(ID_OnOpenThreadByBrowser, wxT("ブラウザで開く"));
     menu5->AppendSeparator();
     wxMenu *broadcast = new wxMenu;
     broadcast->Append(wxID_ANY, wxT("オートリロード"));
     broadcast->Append(wxID_ANY, wxT("オートスクロール"));
     broadcast->Append(wxID_ANY, wxT("オートリロード・スクロール"));
     menu5->AppendSubMenu(broadcast, wxT("実況支援"));
     menu5->AppendSeparator();
     wxMenu *copyMenu5 = new wxMenu;
     copyMenu5->Append(ID_CopyTURLToClipBoard, wxT("URLをコピー"));
     copyMenu5->Append(ID_CopyTTitleToClipBoard, wxT("タイトルをコピー"));
     copyMenu5->Append(ID_CopyTBothDataToClipBoard, wxT("タイトルとURLをコピー"));
     menu5->AppendSubMenu(copyMenu5, wxT("コピー"));
     menu5->AppendSeparator();
     menu5->Append(wxID_ANY, wxT("この板を開く"));
     menu5->Append(wxID_ANY, wxT("この板の看板を見る"));
     menu5->AppendSeparator();
     wxMenu *saveLog = new wxMenu;
     saveLog->Append(ID_SaveDatFile, wxT("datを名前を付けて保存"));
     saveLog->Append(ID_SaveDatFileToClipBoard, wxT("datをクリップボードにコピー"));
     menu5->AppendSubMenu(saveLog, wxT("このログを保存"));
     menu5->Append(ID_DeleteDatFile, wxT("このログを削除"));
     menu5->Append(ID_ReloadThisThread, wxT("再読み込み"));
     menu5->AppendSeparator();
     wxMenu *iReadHere = new wxMenu;
     iReadHere->Append(wxID_ANY, wxT("この辺まで読んだ"));
     iReadHere->Append(wxID_ANY, wxT("最後まで読んだ"));
     iReadHere->Append(wxID_ANY, wxT("「ここまで読んだ」にジャンプ"));
     iReadHere->Append(wxID_ANY, wxT("「ここまで読んだ」を解除"));
     menu5->AppendSubMenu(iReadHere, wxT("ここまで読んだ"));
     wxMenu *chkResponse = new wxMenu;
     chkResponse->Append(wxID_ANY, wxT("レスのチェックをすべて解除"));
     menu5->AppendSubMenu(chkResponse, wxT("レスのチェック"));
     menu5->AppendSeparator();
     wxMenu *changeLocalAbone = new wxMenu;
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("透明"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("標準"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("ポップアップ"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("あぼ～ん無効"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("あぼ～んのみ"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("自分書込のみ"));
     changeLocalAbone->AppendRadioItem(wxID_ANY, wxT("自分返信のみ"));
     menu5->AppendSubMenu(changeLocalAbone, wxT("ローカルあぼ～ん表示の変更"));
     wxMenu *extractRedRes = new wxMenu;
     extractRedRes->Append(wxID_ANY, wxT("赤レス抽出"));
     extractRedRes->Append(wxID_ANY, wxT("被参照レス抽出"));
     extractRedRes->AppendSeparator();
     extractRedRes->Append(wxID_ANY, wxT("赤レスフィルタ"));
     extractRedRes->Append(wxID_ANY, wxT("被参照レス抽出フィルタ"));
     menu5->AppendSubMenu(extractRedRes, wxT("赤レス抽出"));

     /**
      * お気に入り部分
      */
     menu6->Append(wxID_ANY, wxT("このフォルダを開く"));
     menu6->AppendSeparator();

     std::vector<std::tuple<wxString, wxString, wxString>> favoriteList;
     SQLiteAccessor::GetUserFavoriteThreadList(favoriteList);

     if ( favoriteList.size() == 0 ) 
     {
          menu6->Append(wxID_ANY, wxT("お気に入りのスレッドがないよ"));
     } 
     else 
     {
	  for (unsigned int i = 0; i < favoriteList.size(); i++ ) 
	  {
	       if ( std::get<1>(favoriteList.at(i)) != wxT("0")) 
	       {
		    // スレッドの場合
		    // ex) http://engawa.2ch.net/test/read.cgi/linux/1044149677/
		    wxString helperUrl = wxT("http://hostname/test/read.cgi/") 
			 + std::get<2>(favoriteList.at(i)) 
			 + wxT("/") 
			 + std::get<1>(favoriteList.at(i));

		    menu6->Append(ID_UserFavoriteThreadClick + i, std::get<0>(favoriteList.at(i)), helperUrl);
	       } 
	       else 
	       {
		    // 板の場合
		    // ex) http://engawa.2ch.net/linux/
		    wxString helperUrl = wxT("http://hostname/")
			 + std::get<2>(favoriteList.at(i))
		         + wxT("/");
		
		    menu6->Append(ID_UserFavoriteThreadClick + i, std::get<0>(favoriteList.at(i)), helperUrl);
	       }
	  }
     }  

     /**
      * 検索部分
      */
     menu7->Append(wxID_ANY, wxT("検索"));
     menu7->Append(wxID_ANY, wxT("↓検索"));
     menu7->Append(wxID_ANY, wxT("↑検索"));
     menu7->AppendSeparator();
     menu7->Append(wxID_ANY, wxT("レス抽出"));
     menu7->AppendSeparator();
     menu7->Append(wxID_ANY, wxT("スレのツリー形式表示"));
     menu7->Append(wxID_ANY, wxT("スレのアウトライン表示"));
     menu7->AppendSeparator();
     menu7->Append(wxID_ANY, wxT("スレ絞込み"));
     menu7->Append(wxID_ANY, wxT("スレ絞込み結果クリア"));
     menu7->Append(wxID_ANY, wxT("ログから検索"));
     menu7->AppendSeparator();
     menu7->Append(wxID_ANY, wxT("スレッドタイトル検索"));
     menu7->Append(wxID_ANY, wxT("2chのURLにジャンプ"));
     menu7->AppendSeparator();
     menu7->Append(wxID_ANY, wxT("検索履歴削除"));
     /**
      * ウィンドウ部分
      */
     menu8->Append(ID_UserLookingTabsControl, wxT("閉じる"));
     menu8->AppendSeparator();
     menu8->Append(ID_MoveNextTab, wxT("次のタブ"));
     menu8->Append(ID_MovePrevTab, wxT("前のタブ"));
     menu8->AppendSeparator();
     menu8->Append(wxID_ANY, wxT("重ねて表示"));
     menu8->Append(wxID_ANY, wxT("左右に並べて表示"));
     menu8->Append(wxID_ANY, wxT("上下に並べて表示"));
     menu8->Append(wxID_ANY, wxT("すべて元のサイズに戻す"));
     menu8->Append(wxID_ANY, wxT("すべて最大化"));
     menu8->AppendSeparator();
     menu8->Append(ID_WindowMinimize, wxT("最小化")); // 最小化メソッドがわからない
     menu8->Enable(ID_WindowMinimize, false);
     menu8->AppendSeparator();
     this->lookingTB = new wxMenu();
     lookingTB->Append(ID_UserLookingTabsMenuUp, wxT("現在開いている板とスレッド"), wxT("現在開いている板とスレッドを開きます"));
     lookingTB->AppendSeparator();
     menu8->AppendSubMenu(lookingTB, wxT("現在開いている板とスレッド"), wxT("現在開いている板とスレッドを開きます"));
     /**
      * ツール部分
      */
     menu9->Append(wxID_ANY, wxT("更新チェック"));
     menu9->AppendSeparator();
     wxMenu *image = new wxMenu;
     image->Append(wxID_ANY, wxT("マウスオーバーで画像を開く"));
     image->Append(wxID_ANY, wxT("クリップボード監視"));
     image->AppendSeparator();
     image->Append(wxID_ANY, wxT("キャッシュ一覧"));
     menu9->AppendSubMenu(image, wxT("画像"));
     wxMenu *memo = new wxMenu;
     memo->Append(wxID_ANY, wxT("名前履歴消去"));
     memo->Append(wxID_ANY, wxT("メール履歴消去"));
     memo->AppendSeparator();
     memo->Append(wxID_ANY, wxT("次のメモ"));
     memo->Append(wxID_ANY, wxT("前のメモ"));
     memo->Append(wxID_ANY, wxT("名前をつけて保存"));
     memo->Append(wxID_ANY, wxT("ファイルを開く"));
     memo->Append(wxID_ANY, wxT("メモをクリア"));
     memo->Append(wxID_ANY, wxT("AA入力支援"));
     memo->Append(wxID_ANY, wxT("一つ前のレスを貼り付け"));
     memo->AppendSeparator();
     memo->Append(wxID_ANY, wxT("メモ欄の内容で書き込む"));
     menu9->AppendSubMenu(memo, wxT("メモ"));
     menu9->AppendSeparator();
     menu9->AppendCheckItem(wxID_ANY, wxT("サウンド"));
     menu9->AppendSeparator();
     menu9->Append(ID_CallViewerSettingWindow, wxT("ビューア設定"));
     menu9->Append(ID_CallSettingWindow, wxT("設定"));
     /**
      * ヘルプ部分
      */
     menu10->Append(wxID_ANY, wxT("ヘルプ"));
     menu10->AppendSeparator();
     menu10->Append(ID_OnOpenXrossBoardOfficial, wxT("XrossBoard公式サイトをブラウザで開く"));
     menu10->AppendSeparator();
     menu10->Append(wxID_ANY, wxT("アップデートチェック"));
     menu10->AppendSeparator();
     menu10->Append(wxID_ANY, wxT("バグレポート"));
     menu10->Append(ID_GetVersionInfo, wxT("バージョン情報を開く"));

     // Linuxではファイルごとクリップボードにコピーすることができない
#ifndef __WXMSW__
     menuBar->Enable(ID_SaveDatFileToClipBoard, false);
#endif

     SetMenuBar(menuBar);
     /**
      * メニューバー設置終わり
      */
}
/**
 * SetProperties
 * 前回からのデータ引継ぎ等の処理を行う。
 */
void XrossBoard::SetProperties() 
{
     // wxGladeによる自動生成
     SetTitle(_("XrossBoard"));
     SetSize(wxSize(960, 640));
     isDragging = false;
     isClicking = false;

     // ユーザーのホームディレクトリを取得
     wxDir workDir(::wxGetHomeDir());
     wxString xb = ::wxGetHomeDir() + wxFILE_SEP_PATH + XROSSBOARD_DIR;
     wxDir xbDir(xb);

     /**
      * XrossBoardの各種初期化処理
      */
     
     // sqliteの初期化を行う
     std::unique_ptr<SQLiteAccessor> sqliteAccessor(new SQLiteAccessor());
     // Curlの初期化を行う
     cURLpp::initialize();
     // babelの初期化を行う
     babel::init_babel();
     // XRCの初期化を行う
     wxXmlResource::Get()->InitAllHandlers();
     bool bRet = wxXmlResource::Get()->Load(wxT(RESOURCE_PATH L"*.xrc"));

     if( !bRet )
     {
	  wxMessageBox(wxT("XRCファイルの読み込みに失敗しました。"));
     }

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!workDir.HasSubDirs(XROSSBOARD_DIR)) 
     {	  
	  ::wxMkdir(xb);
 
	  // 存在しない場合は初期化処理を実施する
	  InitializeXrossBoard(xb);
	  // ソケット通信を行う
	  std::unique_ptr<SocketCommunication> sock(new SocketCommunication());

	  // 実行コード別のダイアログを出す
	  if (0 != sock->DownloadBoardList(BOARD_LIST_PATH, BOARD_LIST_HEADER_PATH)) 
	  {
	       wxMessageBox(wxT("板一覧情報取得に失敗しました。ネットワークの接続状況を確認してください。"));
	  } 
	  else 
	  {
	       // 板一覧情報取得
	       GetBoardListImpl();
	  }
     } 

     // 初回起動以外の際、確認のためディレクトリをチェックする
     wxDir chkDir(xb);
     // dat, shingetsu, prop, cacheフォルダが存在するか確認。無ければ確認＆フォルダを作成
     wxArrayString xbNeedDir;
     xbNeedDir.Add(wxT("dat"));
     xbNeedDir.Add(wxT("shingetsu"));
     xbNeedDir.Add(wxT("prop"));
     xbNeedDir.Add(wxT("cache"));
     for (size_t i = 0; i < xbNeedDir.GetCount(); i++ ) 
     {
	  XrossBoardUtil::CreateSpecifyDirectory(chkDir, xbNeedDir[i]);
     }

     // アプリ上部URL入力欄の画像つきボタンのサイズ調整
     m_url_input_button->SetSize(m_url_input_button->GetBestSize());
     // アプリ上部URL入力欄のフォント調整
     m_url_input->SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));

     // ノートブックのサイズ調整
     wxSize client_size = GetClientSize();

     // 板名のツリーコントロールをクリックした場合表示されるwxAuiNoteBook
     boardNoteBook = new wxAuiNotebook(this, ID_BoardNoteBook, wxPoint(client_size.x, client_size.y), 
				       wxDefaultSize, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_WINDOWLIST_BUTTON);
     
     // 板名のツリーコントロールをクリックした場合表示されるwxAuiNoteBook
     threadNoteBook = new wxAuiNotebook(this, ID_ThreadNoteBook, wxPoint(client_size.x, client_size.y), 
					wxDefaultSize, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_WINDOWLIST_BUTTON);

     // 画像ビューアのインスタンスを作る
     imageViewer = new XrossBoardImageViewer(this, wxID_ANY, wxT("画像ビューア"));
     // 通常は隠しておく
     imageViewer->Show(false);
}
/**
 * XrossBoardを初回起動した場合に実行するメソッド
 */
void XrossBoard::InitializeXrossBoard(const wxString& xb) 
{
     wxDir xbDir(xb);
     ::wxMkdir(xbDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));
     ::wxMkdir(xbDir.GetName() + wxFILE_SEP_PATH + wxT("prop"));
     ::wxMkdir(xbDir.GetName() + wxFILE_SEP_PATH + wxT("cache"));
}
/**
 * DoLayout
 * ユーザーが触る前のアプリのレイアウトを設定する
 * 前回の起動時にレイアウトに変更があった場合はそれを反映する
 */
void XrossBoard::DoLayout() 
{
     // アプリ上部、URL入力欄のレイアウトを設定する
     wxBoxSizer* url_sizer = new wxBoxSizer(wxHORIZONTAL);
     url_sizer->Add(m_url_input, 1, wxEXPAND, 0);
     url_sizer->Add(m_url_input_button, 0, 0, 0);
     m_url_input_panel->SetSizer(url_sizer);

     // アイコン付きのツールバーのレイアウトを設定する
     m_floatToolBar->SetToolBitmapSize(wxSize(16,16));
     m_floatToolBar->AddTool(ID_SwitchSeparateXY,
			     wxT("縦⇔横分割切り替え"),
			     wxBitmap(thrPaneWinImg, wxBITMAP_TYPE_ANY),
			     wxT("縦⇔横分割切り替え"));
     m_floatToolBar->SetToolLongHelp(ID_SwitchSeparateXY, wxT("ウィンドウを３分割する形式を切り替えます."));
     m_floatToolBar->AddTool(ID_SwitchTwoThreePane,
			     wxT("２⇔３ペイン切り替え"),
			     wxBitmap(twoPaneWinImg, wxBITMAP_TYPE_ANY),
			     wxT("２⇔３ペイン切り替え"));
     m_floatToolBar->AddTool(ID_ShowBoardListTree,
			     wxT("板ツリー表示"),
			     wxBitmap(sideTreeImg, wxBITMAP_TYPE_ANY),
			     wxBitmap(sideTreeImg, wxBITMAP_TYPE_ANY),
			     wxITEM_CHECK,
			     wxT("板ツリー表示"),
			     wxT("板一覧ツリーを表示させるかどうか設定します"),
			     NULL);
     m_floatToolBar->AddTool(ID_SwitchRightPane,
			     wxT("右側切り替え"),
			     wxBitmap(twoPaneWinImg, wxBITMAP_TYPE_ANY),
			     wxT("右側切り替え"));
     m_floatToolBar->AddSeparator();
     m_floatToolBar->AddTool(ID_CallResponseWindow,
			     wxT("スレッド新規作成"),
			     wxBitmap(responseImg, wxBITMAP_TYPE_ANY),
			     wxT("スレッド新規作成"));
     m_floatToolBar->AddTool(wxID_ANY,
			     wxT("更新/すべてのタブの更新"),
			     wxBitmap(refreshImg, wxBITMAP_TYPE_ANY),
			     wxT("更新/すべてのタブの更新"));
     m_floatToolBar->AddTool(wxID_ANY,
			     wxT("お気に入りの更新チェック"),
			     wxBitmap(bookMarkImg, wxBITMAP_TYPE_ANY),
			     wxT("お気に入りの更新チェック"));
     m_floatToolBar->AddSeparator();
     m_floatToolBar->AddTool(wxID_ANY,
			     wxT("スレ絞り込み検索/検索結果のクリア"),
			     wxBitmap(redResExtractImg, wxBITMAP_TYPE_ANY),
			     wxT("スレ絞り込み検索/検索結果のクリア"));
     m_floatToolBar->AddTool(wxID_ANY,
			     wxT("ログから検索"),
			     wxBitmap(logSearchImg, wxBITMAP_TYPE_ANY),
			     wxT("ログから検索"));
     m_floatToolBar->AddSeparator();
     m_floatToolBar->AddTool(ID_CallSettingWindow,
			     wxT("設定/ビューア設定"),
			     wxBitmap(configImg, wxBITMAP_TYPE_ANY),
			     wxT("設定/ビューア設定"));
     m_floatToolBar->AddTool(wxID_ANY,
			     wxT("ログから検索"),
			     wxBitmap(helpImg, wxBITMAP_TYPE_ANY),
			     wxT("ログから検索"));

     // Auiマネージャーがどのフレームを管理するか示す
     m_mgr.SetManagedWindow(this);

     // XrossBoardのバージョン
     wxString xbVer = wxEmptyString;
     XrossBoardUtil::GetXrossBoardProperties(wxT("XBVersion"), &xbVer);
     if ( xbVer == wxEmptyString )
     {
	  // TODO: 初回起動 or コンフィグを消している時の処理を追加する
	  //       コンフィグファイルの初期化やSQLの実行など
     }
     else
     {
	  if ( xbVer.Contains(wxT("."))   && 
	       xbVer != xrossboardVersion && 
	       xbVer.Replace(wxT("."), wxEmptyString) == 2 )
	  {
	       wxString currentVer = xrossboardVersion;
	       currentVer.Replace(wxT("."), wxEmptyString);
	       // TODO: 比較して処理する
	  }
     }
     // 現在のバージョンをコンフィグファイルに書き込む
     XrossBoardUtil::SetXrossBoardProperties(wxT("XBVersion"), xrossboardVersion);

     long x = 640, y = 480;
     XrossBoardUtil::GetXrossBoardProperties(wxT("FrameX"), &x);
     XrossBoardUtil::GetXrossBoardProperties(wxT("FrameY"), &y);
     this->SetSize(static_cast<int>(x), static_cast<int>(y));
     // 初回起動時にここが0,0だと困ったことになる
     long px = 300, py = 300;
     XrossBoardUtil::GetXrossBoardProperties(wxT("FramePx"), &px);
     XrossBoardUtil::GetXrossBoardProperties(wxT("FramePy"), &py);
     this->Move(static_cast<int>(px), static_cast<int>(py));

#ifndef __WXMAC__ // MaximizeはMacOSXではサポートされない
     // ウィンドウの最大化情報
     bool isMaximized = false;
     XrossBoardUtil::GetXrossBoardProperties(wxT("IsMaximized"), &isMaximized);
     this->Maximize(isMaximized);
#endif

     // 板一覧ツリーを表示するかどうか
     bool toggled = true;
     XrossBoardUtil::GetXrossBoardProperties(wxT("ShowBoardListTree"), &toggled);
     m_floatToolBar->ToggleTool(ID_ShowBoardListTree, toggled);

     // 画面の分割を縦横どちらにするか
     // separateX = 日, separateY = 而
     separateIsX = true;
     XrossBoardUtil::GetXrossBoardProperties(wxT("SeparateXY"), &separateIsX);

     // 画面のペイン数を何個にするか
     paneIsThree = true; // デフォルト値=3
     XrossBoardUtil::GetXrossBoardProperties(wxT("PaneIsThree"), &paneIsThree);
     // GUI右側に表示するウィジェットは何か
     rightIsThreadList = true; // デフォルト値=スレッド一覧
     XrossBoardUtil::GetXrossBoardProperties(wxT("RightIsThreadList"), &rightIsThreadList);

     // それぞれのペインの情報を設定する
     SetAuiPaneInfo();

     // 設定ファイルからレイアウト情報を読み取る
     wxString perspective = wxEmptyString;
     XrossBoardUtil::GetXrossBoardProperties(wxT("Perspective"), &perspective);
     m_mgr.LoadPerspective(perspective, false);

     // プロパティファイルにフォント設定/背景色があれば使用する
     wxString widgetsName = wxT("ID_LogWindowFontButton");
     wxString widgetsInfo = wxEmptyString;
     XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxFont font;
	  bool ret = font.SetNativeFontInfoUserDesc(widgetsInfo);
	  if(ret) m_logCtrl->SetFont(font);
     }
     widgetsName = wxT("ID_LogWindowBGColorButton");
     widgetsInfo.Clear();
     XrossBoardUtil::GetXrossBoardProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxColour bgColor;
	  bool ret = bgColor.Set(widgetsInfo);
	  if(ret) m_logCtrl->SetBackgroundColour(bgColor);
     }

     // Auiマネージャーの設定を反映する
     m_mgr.Update();
     // 初期設定はこのLayout()が呼ばれる前に行わなくてはいけない
     Layout();
     // end wxGlade
}
/**
 * SetAuiPaneInfo
 * AuiManagerのPaneInfoを設定する
 */
void XrossBoard::SetAuiPaneInfo() 
{     
     // 上部・検索バーを設定する
     wxAuiPaneInfo search;
     search.Name(wxT("searchbar")).ToolbarPane().Layer(1).Top().Position(0)
	  .MinSize(m_search_ctrl->GetBestSize())
	  .BestSize((GetClientSize().GetWidth() -
		     m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRIPPER_SIZE) * 3) / 3,
		    m_floatToolBar->GetClientSize().GetHeight());

     // 上部・URL入力欄を設定する
     wxAuiPaneInfo url;
     url.Name(wxT("urlbar")).ToolbarPane().Layer(1).Top().MinSize(wxSize(0, 16)).Position(2);

     // アイコン付きのツールバーを設定する
     wxAuiPaneInfo toolBar;
     toolBar.Name(wxT("toolBar")).ToolbarPane().Layer(1).Top().Position(1);

     // 板一覧の幅初期値をウィンドウ幅の1/5とする
     const int bestWidth = GetClientSize().GetWidth() / 5;
     // 左側・板一覧のツリーコントロールを設定する
     wxAuiPaneInfo boardTree;
     boardTree.Name(wxT("boardTree")).Layer(1).Left().CaptionVisible(false).Floatable(false)
	  .BestSize(bestWidth, 100);

     // 左側下部・ログ出力ウィンドウを設定する
     wxAuiPaneInfo logWindow;
     logWindow.Name(wxT("logWindow")).Layer(1).Left().CaptionVisible(false).Floatable(false)
	  .BestSize(bestWidth, 100);;

     // 右側上部・板一覧のノートブックとスレッド一覧リストが載ったウィンドウ
     wxAuiPaneInfo boardListThreadListInfo;
     boardListThreadListInfo.Name(wxT("boardListThreadListInfo")).CenterPane()
	  .Floatable(false).BestSize(400, 400);

     // 右側下部・スレッド一覧タブとスレ表示画面が載ったウィンドウ
     wxAuiPaneInfo threadTabThreadContentInfo;
     threadTabThreadContentInfo.Name(wxT("threadTabThreadContentInfo")).CenterPane()
	  .Floatable(false).BestSize(400, 400);

     m_mgr.AddPane(m_search_ctrl, search);
     m_mgr.AddPane(m_floatToolBar, toolBar);
     m_mgr.AddPane(m_url_input_panel, url);
     m_mgr.AddPane(boardTreeNoteBook, boardTree);
     m_mgr.AddPane(m_logCtrl, logWindow);

     m_mgr.AddPane(boardNoteBook, boardListThreadListInfo);
     m_mgr.AddPane(threadNoteBook, threadTabThreadContentInfo);

     // 各ウィンドウで識別用のラベルを設定する
     this->SetLabel(XROSSBOARD_WINDOW);
     m_search_ctrl->SetLabel(SEARCH_BAR);
     m_search_ctrl->Clear(); // ウィンドウに文字列が入るので削除
     m_url_input_panel->SetLabel(URL_BAR);
     m_logCtrl->SetLabel(LOG_WINDOW);
     boardNoteBook->SetLabel(BOARD_NOTEBOOK);
     threadNoteBook->SetLabel(THREAD_NOTEBOOK);
     boardTreeNoteBook->SetLabel(BOARD_TREE_NOTEBOOK);

     // 板一覧更ツリーの初期化
     InitializeBoardList();
     // ツールバーの明示化
     m_floatToolBar->Realize();
     /** 
      * 前回設定されたフォント情報があれば設定する
      * 板一覧ツリーのフォント設定は XrossBoard::InitializeBoardListにて実施
      */

     // もし初回起動であればフォント設定は行わない
     // すればセグメンテーション違反が起こりやすい
     const wxString prop = ::wxGetHomeDir() \
	  + wxFILE_SEP_PATH  \
	  + XROSSBOARD_DIR    \
	  + wxFILE_SEP_PATH  \
	  + APP_CONFIG_FILE;

     if ( wxFile::Exists(prop) ) 
     {
	  *m_logCtrl << wxT("前回使用したフォント情報の読み出し…\n");
	  m_search_ctrl->SetFont(ReadFontInfo(SEARCH_BAR));
	  m_url_input_panel->SetFont(ReadFontInfo(URL_BAR));
	  m_logCtrl->SetFont(ReadFontInfo(LOG_WINDOW));
	  boardNoteBook->SetFont(ReadFontInfo(BOARD_NOTEBOOK));
	  threadNoteBook->SetFont(ReadFontInfo(THREAD_NOTEBOOK));
	  *m_logCtrl << wxT("フォント情報の読み出し終了…\n");
     }

     // FIXME: テスト用の実装
     const wxString ctwDir = \
	  ::wxGetHomeDir()   \
	  + wxFILE_SEP_PATH  \
	  + XROSSBOARD_DIR;

     wxTwitterNotebook twitter;
     twitter.SetLoggingTextCtrl(m_logCtrl);
     twitter.SetAppDir(ctwDir);
     twitter.Initialize();
     // FIXME

     UpdatePanes(false);
}
/**
 * SetPreviousUserLookedTab
 * ユーザーが前回までに見ていた板一覧タブとスレッド一覧タブをセットする
 */
void XrossBoard::SetPreviousUserLookedTab() 
{
     wxArrayString userLookedBoardList = SQLiteAccessor::GetUserLookedBoardList();

     for (unsigned int i = 0; i < userLookedBoardList.GetCount(); i++) 
     {
	  // 板名
	  wxString boardName = userLookedBoardList[i];

	  // 板名に対応したURLを取ってくる
	  URLvsBoardName hash = retainHash[boardName];
	  wxString boardNameAscii = hash.boardNameAscii;

	  if (!boardNameAscii.IsEmpty()) 
	  {
	       /** 2chのdatファイルが入っていた場合の処理 */
	       wxString outputPath = ::wxGetHomeDir() 
		    + wxFILE_SEP_PATH 
		    + XROSSBOARD_DIR
		    + wxFILE_SEP_PATH
		    + wxT("dat")
		    + wxFILE_SEP_PATH
		    + boardNameAscii
		    + wxFILE_SEP_PATH
		    + boardNameAscii
		    + wxT(".dat");

	       // 板一覧タブをセットする
	       std::map<wxString, ThreadList> stub;
	       // 要素を追加する
	       wxString stubStr = wxT("NO_NEED_TO_CHK_THREAD_STATE");
	       ThreadList thList;
	       stub.insert( std::map<wxString, ThreadList>::value_type( stubStr, thList ) );

	       SetThreadListItemNew(boardName, outputPath, i, stub);

#ifdef USE_SHINGETSU
	  } 
	  else 
	  {
	       // ホスト名
	       const wxString nodeHostname = boardName;
	       // ファイルパス
	       wxString filePath = XrossBoardUtil::CreateShingetsuThreadListFilePath(nodeHostname);

	       /** 新月のcsvファイルを参照する */
	       SetShingetsuThreadListToNoteBook(nodeHostname, filePath);
#endif /** USE_SHINGETSU */
	  }
     }

     wxArrayString userLookedThreadList = SQLiteAccessor::GetUserLookedThreadList();

     for (unsigned int i = 0; i < userLookedThreadList.GetCount(); i+= 3) 
     {
	  wxString title = userLookedThreadList[i];
	  wxString origNumber = userLookedThreadList[i+1];
	  wxString boardNameAscii = userLookedThreadList[i+2];

	  // ファイルパスの組み立てとファイルの有無確認
	  wxString threadContentPath = XrossBoardUtil::AssembleFilePath(boardNameAscii, origNumber, KIND_THREAD_DAT);

	  // ファイルの有無確認
	  if (!wxFile::Exists(threadContentPath)) 
	  {
	       // 無ければ警告を出して次へ
	       wxMessageBox(wxT("前回読み込んでいたdatファイルの読み出しに失敗しました\n\
                                 datファイルを削除しているか、datファイルの保存先を変更していませんか？"), wxT("読み込んでいるスレッド"), wxICON_ERROR);
	       continue;
	  }

	  // スレッドの内容をノートブックに反映する
	  SetThreadContentToNoteBook(threadContentPath, origNumber, title);
	  // ノートブックに登録されたスレッド情報をハッシュに登録する
	  ThreadInfo info;
	  info.origNumber = origNumber;
	  info.boardNameAscii = boardNameAscii;
	  tiHash[title] = info;
     }
}
/**
 * デストラクタ
 */
XrossBoard::~XrossBoard() 
{
     // Auiマネージャーを削除する
     m_mgr.UnInit();
     // Curlの終了処理を行う
     cURLpp::terminate();
}
/**
 * XrossBoardを終了させる
 */
void XrossBoard::OnQuit(wxCommandEvent& e) 
{
     restartAppFlag = false;
     XrossBoardUiUtil::QueueEventHelper(wxEVT_CLOSE_WINDOW, wxID_ANY);
     e.Skip(false);
}
/**
 * XrossBoardを再起動する
 */
void XrossBoard::OnRestart(wxCommandEvent& event) 
{
#ifdef __WXMSW__
     restartAppFlag = true;
     Close(true);
#else
     // ここはどうしたものやら
     this->Hide();
     wxSleep(1);
     this->Show();
#endif
}
/**
 * XrossBoardのウィンドウを最小化する
 */
void XrossBoard::WindowMinimize(wxCommandEvent& event) 
{
     
}
/**
 * 板一覧のツリーがクリックされたときに起きるイベント
 */
void XrossBoard::OnGetBoardInfo(wxTreeEvent& event) 
{
     // 選択されたTreeItemIdのインスタンス
     wxTreeItemId pushedTree = event.GetItem();

     // もし選択されたツリーが板名だったら(※TreeItemに子要素が無かったら)
     if (!m_tree_ctrl->ItemHasChildren(pushedTree)) 
     {
	  // 板名をwxStringで取得する
	  wxString boardName(m_tree_ctrl->GetItemText(pushedTree));

	  // 取得した板名が取得不可なものであればリターン
	  if (boardName == wxEmptyString || boardName == wxT("2ch板一覧"))
	       return;

	  // URLを保持する文字列
	  wxString boardURL;
	  // サーバー名を保持する文字列
	  wxString boardNameAscii;

	  // 板名に対応したURLを取ってくる
	  URLvsBoardName hash = retainHash[boardName];
	  boardNameAscii = hash.boardNameAscii;
	  boardURL = hash.boardURL;

	  // 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
	  SetBoardNameToNoteBook(boardName, boardURL, boardNameAscii);
     }
}

#ifdef USE_SHINGETSU
/**
 * 新月公開ノードツリーコントロールでクリックした時のイベント
 */
void XrossBoard::OnGetShingetsuNodeInfo(wxTreeEvent& event) 
{
     // 選択されたTreeItemIdのインスタンス
     wxTreeItemId pushedTree = event.GetItem();

     // もし選択されたツリーが板名だったら(※TreeItemに子要素が無かったら)
     if (!m_tree_ctrl->ItemHasChildren(pushedTree)) 
     {
	  // 板名をwxStringで取得する
	  const wxString nodeHostname(m_tree_ctrl->GetItemText(pushedTree));

	  // 取得した板名が取得不可なものであればリターン
	  if (nodeHostname == wxEmptyString || nodeHostname == wxT("新月公開ノード一覧"))
	       return;
	  // 新月公開ノードをクリックして、それをノートブックに反映するメソッド
	  SetShingetsuNodeToNoteBook(nodeHostname);
     }
}
/**
 * 新月公開ノードツリー上で右クリックした時のイベント
 * 動的に公開ノードのURLを登録する
 */
void XrossBoard::OnRightClickShingetsuNodeTree(wxTreeEvent& event) 
{
     wxTextEntryDialog* askDialog = new wxTextEntryDialog(this,
							  wxT("新月公開ノードURLの追加を行います.URLを入力してください."),
							  wxT("新月公開ノードURLの追加"));
     askDialog->SetTitle(wxT("新月公開ノードURLの追加"));

#ifdef __WXMSW__
     askDialog->SetIcon(wxICON(wxicon));
#else
     askDialog->SetIcon(wxICON(xrossboard));
#endif
     
     if ( askDialog->ShowModal() == wxID_OK ) 
     {
	  const wxString nodeURL = askDialog->GetValue();

	  // URIから各パラメーターを抜き取る
	  PartOfURI* uri = new PartOfURI;
	  bool urlIsSane = XrossBoardUtil::SubstringURI(nodeURL, uri);
	  const wxString protocol = uri->protocol;            // http
	  const wxString hostname = uri->hostname;            // localhost
	  const wxString port = uri->port;                    // :8080
	  const wxString path = uri->path;                    // /path/to/content
	  delete uri;

	  if (!urlIsSane) 
	  {
	       wxMessageBox(wxT("エラー, 不正なURLです.入力したURLが正しいか確認してください."), 
			    wxT("新月公開ノードの登録"), wxICON_ERROR);
	       askDialog->Destroy();
	       return;
	  }

	  if (!nodeURL.IsEmpty()) 
	  {
	       if (!port.IsEmpty()) 
	       {
		    const wxString url = protocol + wxT("://") + hostname + port + wxT("/");
		    SQLiteAccessor::SetShingetsuNode(url);
		    m_shingetsu_tree_ctrl->AppendItem(m_shingetsu_tree_ctrl->GetRootItem(), url, 1, 1);
	       } 
	       else 
	       {
		    const wxString url = protocol + wxT("://") + hostname + wxT("/");
		    SQLiteAccessor::SetShingetsuNode(url);
		    m_shingetsu_tree_ctrl->AppendItem(m_shingetsu_tree_ctrl->GetRootItem(), url, 1, 1);	       
	       }
	  }
     }

     askDialog->Destroy();
}
#endif /** USE_SHINGETSU */

/**
 * 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
 */
void XrossBoard::SetBoardNameToNoteBook(wxString& boardName, wxString& boardURL, wxString& boardNameAscii) 
{
     // 以前ダウンロードしていたスレッドの情報をmap化する
     std::map<wxString, ThreadList> oldThreadMap;
     URLvsBoardName boardInfo;
     boardInfo.boardName      = boardName;
     boardInfo.boardURL	      = boardURL;
     boardInfo.boardNameAscii = boardNameAscii;
     // map化の実行
     XrossBoardUtil::GenerateOldThreadMap(oldThreadMap, boardInfo);

     // スレ一覧をダウンロードする
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());
     wxString outputPath = sock->DownloadThreadList(boardName, boardURL, boardNameAscii);
     

     // 新規にセットされる板名かどうかのフラグを用意する
     bool itIsNewBoardName = true;
     // 次に選択されるべきタブのページ数を格納する変数
     size_t selectedPage = 0;

     // ユーザーが開いているタブの板名を調べる
     for (unsigned int i = 0; i < boardNoteBook->GetPageCount(); i++) 
     {
	  if (boardName.Cmp(boardNoteBook->GetPageText(i)) == 0) 
	  {
	       itIsNewBoardName = false;
	       selectedPage = i;
	       break;
	  }
     }

     if (itIsNewBoardName) 
     {
	  // もし新規のダウンロードだった場合、選択されるべきページを指定
	  selectedPage = boardNoteBook->GetPageCount();
	  SetThreadListItemNew((const wxString) boardName, (const wxString) outputPath, 
			       (const size_t) selectedPage, oldThreadMap);
     } 
     else 
     {
	  // 更新処理の場合、選択されるべきページはi
	  SetThreadListItemUpdate((const wxString) boardName, (const wxString) outputPath, 
				  (const size_t) selectedPage, oldThreadMap);
     }

     // アイコンの更新を実施する
     UpdateThreadTabIcons();
}
/**
 * ノートブックに、新規にダウンロードされたスレッド一覧情報を反映するメソッド
 */
void XrossBoard::SetThreadListItemNew(const wxString boardName, 
				     const wxString outputPath, 
				     const size_t selectedPage, 
				     const std::map<wxString,ThreadList>& oldThreadMap) 
{
     // wxAuiToolBarを宣言する
     wxPanel* panel = CreateAuiToolBar(boardNoteBook, boardName, outputPath, oldThreadMap);
     // スレッドリストを表示させる
     boardNoteBook->AddPage(panel, boardName, true);
}
/**
 * ノートブックに、スレッド一覧情報の更新を反映するメソッド
 */
void XrossBoard::SetThreadListItemUpdate(const wxString boardName, 
					const wxString outputPath, 
					const size_t selectedPage, 
					const std::map<wxString,ThreadList>& oldThreadMap) 
{
     // wxAuiToolBarを宣言する
     wxPanel* panel = CreateAuiToolBar(boardNoteBook, boardName, outputPath, oldThreadMap);     
     boardNoteBook->DeletePage(selectedPage);
     boardNoteBook->InsertPage(selectedPage, panel, boardName, false, wxNullBitmap);
     // ノートブックの選択処理
     boardNoteBook->SetSelection(selectedPage);
}
/**
 * アイコンの更新を実施する
 */
void XrossBoard::UpdateThreadTabIcons() 
{
     // 現在アクティブになっているタブの板名を取得する
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     URLvsBoardName hash = retainHash[boardName];
     wxString tabBoardNameAscii = hash.boardNameAscii;

     // スレッド一覧情報が入ったコンテナ
     VirtualBoardList vBoardList;

     // リストコントロールを引き出してくる
     if ( VirtualBoardListCtrl* vbListCtrl = 
	  dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName)) ) 
     {

	  // vectorをコピーしてソートしておく
	  vbListCtrl->CopyVectorItems(vBoardList);
	  std::sort(vBoardList.begin(), 
		    vBoardList.end(),
		    [] (const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem) -> bool {
			 return wxAtoi(lItem.getOid()) > wxAtoi(rItem.getOid());
		    });

     } 
     else 
     {
	  wxMessageBox(wxT("内部エラー, アイコンの更新処理に失敗しました."), wxT("スレタブ"), wxICON_ERROR);
	  return;
     }

     ThreadInfoHash::iterator it;
     for (it = tiHash.begin(); it != tiHash.end(); ++it) 
     {
	  wxString key = it->first;
	  ThreadInfo value = it->second;
	  wxString origNumber     = value.origNumber;
	  wxString boardNameAscii = value.boardNameAscii;

	  // 更新した板でなければアイコンは変えない
	  if (boardNameAscii != tabBoardNameAscii) continue;

	  // 固有番号を見つける
	  std::vector<VirtualBoardListItem>::iterator itV 
	       = std::find_if(vBoardList.begin(),
			      vBoardList.end(),
			      [&origNumber] (const VirtualBoardListItem& item) -> bool {
				   return item.getOid().Contains(origNumber);		
			      });

	  /**
	   * まずスレタブを全部見る
	   */
	  for (size_t i = 0; i < threadNoteBook->GetPageCount(); i++) 
	  {
	       if (key.Cmp(threadNoteBook->GetPageText(i)) == 0) 
	       {
		    if ( itV != vBoardList.end() ) 
		    {
			 // スレッド一覧情報内にスレッドがあった => 更新確認
			 int newResNumber = wxAtoi(itV->getResponse());
			 if (ThreadContentBar* oldThreadBar = 
			     dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(i))) 
			 {
			      // スレッド情報をもってくる
			      int oldResNumber = oldThreadBar->GetThreadContentThreadResponseNum();

			      // もし取得数のほうが多かったら更新 "+" !
			      if ( newResNumber > oldResNumber ) 
			      {
				   threadNoteBook->SetPageBitmap(i, wxBitmap(threadTabAddImg, wxBITMAP_TYPE_ANY));
			      }
			 }

		    } 
		    else 
		    {
			 // スレッド一覧情報を内にスレッドがない => dat落ち
			 threadNoteBook->SetPageBitmap(i, wxBitmap(threadTabDrpImg, wxBITMAP_TYPE_ANY));
			 break;
		    }
	       }
	  }
     }
}
/**
 * 板一覧更新処理
 */
void XrossBoard::OnGetBoardList(wxCommandEvent&) {

     XrossBoardUiUtil::SendLoggingHelper(wxT("三┏（ ；´ん｀）┛…板一覧更新\n"));

     // ソケット通信を行う
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());

     // 実行コード別のダイアログを出す
     if (0 != sock->DownloadBoardList(BOARD_LIST_PATH, BOARD_LIST_HEADER_PATH)) 
     {
	  wxMessageBox(wxT("板一覧情報取得に失敗しました。ネットワークの接続状況を確認してください。"));
     } 
     else 
     {
	  // 板一覧情報取得
	  GetBoardListImpl();
	  // 板一覧更新
	  SetBoardList();
	  
	  XrossBoardUiUtil::SendLoggingHelper(wxT("　　　(ヽ´ん`) 完了\n"));
     }
}

/**
 * 板一覧情報取得処理の実体
 * 板一覧情報を展開しSQLiteに設定する
 */
void XrossBoard::GetBoardListImpl()
{
     // 板一覧情報を展開し、SQLiteに設定する
     SQLiteAccessor::DeleteTableData(wxT("BOARD_INFO"));
     wxString boardListPath = BOARD_LIST_PATH;
     ExtractBoardList::ExtractBoardInfo(boardListPath.mb_str());
     XrossBoardUiUtil::SendLoggingHelper(wxT("(ヽ´ん`) 板一覧更新完了\n"));
}

/**
 * HtmlWindow上でマウスホバーが起きた場合の処理
 */
void XrossBoard::OnCellHover(wxHtmlCellEvent& event) 
{
     wxHtmlCell* cell = event.GetCell();
     wxHtmlLinkInfo* linkInfo = cell->GetLink(cell->GetPosX(), cell->GetPosY());
     
     if (linkInfo && linkInfo->GetHref() != wxEmptyString) 
     {
	  // FIXME: ログの出力レベル切り替えが必要か
	  //XrossBoardUiUtil::SendLoggingHelper(linkInfo->GetHref());
	  wxString rest;

	  if ( linkInfo->GetTarget() == _T("_blank") ) 
	  {
	       // レスアンカーを察知した場合の処理
	       // <a>タグ内サンプル　<a href="../test/read.cgi/poverty/1345636335/20" target="_blank">
	       wxString href = linkInfo->GetHref();
	       wxStringTokenizer tkz(href, wxT("//"));
	       wxString boardNameAscii, origNumber, resNumber;

	       while (tkz.HasMoreTokens()) 
	       {
		    wxString tmp = tkz.GetNextToken();
		    if (tmp == _T("read.cgi")) 
		    {
			 boardNameAscii = tkz.GetNextToken();
			 origNumber = tkz.GetNextToken();
			 resNumber = tkz.GetNextToken();
			 break;
		    }
	       }

	       if (origNumber == wxEmptyString || resNumber == wxEmptyString || boardNameAscii == wxEmptyString) 
	       {
		    return;
	       }
	       // アンカーの出現位置
	       wxPoint anchorPoint(cell->GetPosX(), cell->GetPosY());

	       // FIXME: ログの出力レベル切り替えが必要か
	       // XrossBoardUiUtil::SendLoggingHelper(wxString::Format(wxT("Anchor X:%d, Y:%d\n"), anchorPoint.x, anchorPoint.y));
	       // 取得した情報を元に新しいポップアップウィンドウを出現させる
	       SetPopUpWindow(event, boardNameAscii, origNumber, resNumber, anchorPoint);

	  } 
	  else if ( linkInfo->GetHref().StartsWith(wxT("#_"), &rest) ) 
	  {
	       // ID:xxxxxxxxx の形式であればポップアップさせる
	       if (regexID.IsValid() && regexID.Matches(wxT("ID:") + rest)) 
	       {
		    // アンカーの出現位置
		    wxPoint anchorPoint(cell->GetPosX(), cell->GetPosY());
		    // FIXME: ログの出力レベル切り替えが必要か
		    //XrossBoardUiUtil::SendLoggingHelper(wxString::Format(wxT("Anchor X:%d, Y:%d\n"), anchorPoint.x, anchorPoint.y));

		    const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
		    wxString boardNameAscii = tiHash[title].boardNameAscii;
		    wxString origNumber = tiHash[title].origNumber;

		    // 取得した情報を元に新しいポップアップウィンドウを出現させる
		    SetPopUpWindowForID(event, boardNameAscii, origNumber, rest, anchorPoint);		    
	       }
	  }
	  else if ( linkInfo->GetHref().StartsWith(wxT("#"), &rest) )
	  {
	       // アンカーの出現位置
	       wxPoint anchorPoint(cell->GetPosX(), cell->GetPosY());
	       // FIXME: ログの出力レベル切り替えが必要か
	       // XrossBoardUiUtil::SendLoggingHelper(wxString::Format(wxT("Anchor X:%d, Y:%d\n"), anchorPoint.x, anchorPoint.y));

	       const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
	       wxString boardNameAscii = tiHash[title].boardNameAscii;
	       wxString origNumber = tiHash[title].origNumber;

	       // 取得した情報を元に新しいポップアップウィンドウを出現させる
	       SetPopUpWindowByIndex(event, rest, anchorPoint);	       
	  }
     }
}
/**
 * スレッド一覧ノートブックで、タブが消される前の処理
 */
void XrossBoard::OnAboutCloseThreadNoteBook(wxAuiNotebookEvent& event) 
{
     // 消されようとしているタブのタイトルを取得
     wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     // 固有番号を取得
     wxString origNumber = tiHash[title].origNumber;
     // スレッドの情報をSQLiteに格納する
     ThreadInfo t;
     t.title = title;
     t.origNumber = origNumber;
     t.boardNameAscii = tiHash[title].boardNameAscii;
     SQLiteAccessor::SetThreadInfo(&t);
     // ハッシュからタイトルのキーを持つデータを削除
     tiHash.erase(title);
     // 閲覧中ツリーの情報を更新する
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, ID_NowReadingTreectrlUpdate);
}
/**
 * 板一覧ノートブックで、タブが消される前の処理
 */
void XrossBoard::OnAboutCloseBoardNoteBook(wxAuiNotebookEvent& event) 
{
     // 消されようとしているタブのタイトルを取得
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     URLvsBoardName hash = retainHash[boardName];
     SQLiteAccessor::SetClosedBoardInfo(&hash);
}
/**
 * アクティブな板タブをひとつ閉じる
 */
void XrossBoard::OneBoardTabClose(wxCommandEvent & event) 
{
     // アクティブなタブを選択して閉じる
     boardNoteBook->DeletePage(boardNoteBook->GetSelection());
}
/**
 * 現在選択されていないスレッド一覧タブを閉じる
 */
void XrossBoard::ExcepSelTabClose(wxCommandEvent & event) 
{
     // タブの数を数える
     size_t pages = boardNoteBook->GetPageCount();
     size_t select = boardNoteBook->GetSelection();
     bool delete_f = false;

     for (unsigned int i=0;i<pages;i++) 
     {
	  if (i != select && !delete_f) 
	  {
	       boardNoteBook->DeletePage(0);
	  } 
	  else if (i == select && !delete_f) 
	  {
	       boardNoteBook->DeletePage(1);
	       delete_f = true;
	  } 
	  else if (i != select && delete_f) 
	  {
	       boardNoteBook->DeletePage(1);
	  }
     }
}
/**
 * すべてのスレッド一覧タブを閉じる
 */
void XrossBoard::AllBoardTabClose(wxCommandEvent& event) 
{
     int pages = boardNoteBook->GetPageCount();
     for (int i=0;i<pages;i++) 
     {
	  boardNoteBook->DeletePage(0);
     }
}
/**
 *　これより左のスレッド一覧タブをを閉じる
 */
void XrossBoard::AllLeftBoardTabClose(wxCommandEvent& event) {

     // タブの数を数える
     size_t select = boardNoteBook->GetSelection();

     for (unsigned int i=0;i<select;i++) 
     {
	  boardNoteBook->DeletePage(0);
     }
}
/**
 *　これより右のスレッド一覧タブを閉じる
 */
void XrossBoard::AllRightBoardTabClose(wxCommandEvent& event) {

     // タブの数を数える
     size_t pages = boardNoteBook->GetPageCount();
     size_t select = boardNoteBook->GetSelection();
     for (unsigned int i=0;i<pages;i++) 
     {
	  if (i>select) 
	  {
	       boardNoteBook->DeletePage(select+1);
	  }
     }
}
/**
 * スレッド一覧をブラウザで開く
 */
void XrossBoard::OnOpenBoardByBrowser(wxCommandEvent& event) {

     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     URLvsBoardName hash = retainHash[boardName];
     wxLaunchDefaultBrowser(hash.boardURL);
}
/**
 * アクティブなスレッド一覧をひとつ更新する
 */
void XrossBoard::ReloadOneBoard(wxCommandEvent& event) {

     size_t page = boardNoteBook->GetSelection();
     wxString boardName = boardNoteBook->GetPageText(page);
     URLvsBoardName hash = retainHash[boardName];
     SetBoardNameToNoteBook(boardName, hash.boardURL, hash.boardNameAscii);
}
/**
 * すべてのタブのスレッド一覧を更新する
 */
void XrossBoard::ReloadAllBoard(wxCommandEvent& event) {

     for ( size_t page = 0; page < boardNoteBook->GetPageCount(); page++ ) 
     {
	  wxString boardName = boardNoteBook->GetPageText(page);
	  URLvsBoardName hash = retainHash[boardName];
	  SetBoardNameToNoteBook(boardName, hash.boardURL, hash.boardNameAscii);
     }     
}
/**
 * 板のURLをクリップボードにコピーする
 */
void XrossBoard::CopyBURLToClipBoard(wxCommandEvent& event) {

     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     URLvsBoardName hash = retainHash[boardName];

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(hash.boardURL));
	  wxTheClipboard->Close();
     }
}
/**
 * 板のタイトルをクリップボードにコピーする
 */
void XrossBoard::CopyBTitleToClipBoard(wxCommandEvent& event) {

     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(boardName));
	  wxTheClipboard->Close();
     }
}
/**
 * 板のURLとタイトルをクリップボードにコピーする
 */
void XrossBoard::CopyBBothDataToClipBoard(wxCommandEvent& event) {

     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     URLvsBoardName hash = retainHash[boardName];

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(boardName + wxT("\n") + hash.boardURL));
	  wxTheClipboard->Close();
     }
}
/**
 * 選択されている板のスレッド一覧情報を削除する
 */
void XrossBoard::DeleteBSelectedDatFile(wxCommandEvent& event) {

     // datファイル名の組み立て
     wxString title, boardNameAscii, origNumber, boardURL;
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // ハッシュから英字の板名を探す
     NameURLHash::iterator it;
     URLvsBoardName boardInfoHash;
     for (it = retainHash.begin(); it != retainHash.end(); ++it) 
     {
	  wxString key = it->first;
	  boardInfoHash = it->second;

	  if (boardInfoHash.boardName == boardName) 
	  {
	       boardNameAscii = boardInfoHash.boardNameAscii;
	       break;
	  }
     }

     // ファイルパスの組み立てとファイルの有無確認
     wxString filePath = XrossBoardUtil::AssembleFilePath(boardNameAscii, origNumber, KIND_BOARD__DAT);

     if (!wxFile::Exists(filePath)) 
     {
	  // 無ければエラーメッセージ表示
	  wxMessageBox(wxT("削除するためのdatファイルが見つかりませんでした"));
	  return;
     }

     wxString message = wxT("ファイル") + filePath + wxT("を削除してよろしいですか？");
     wxMessageDialog* dlg = new wxMessageDialog(this, message, wxT("ログファイル削除"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
     const int result = dlg->ShowModal();

     if (result == wxID_YES) 
     {
	  wxRemoveFile(filePath);
     }
}
/**
 * すべてのスレッド一覧情報を削除する
 */
void XrossBoard::DeleteBAllDatFile(wxCommandEvent& event) {

     wxString message = wxT("すべてのスレッド一覧ログを削除してよろしいですか？");
     wxMessageDialog* dlg = new wxMessageDialog(this, message, wxT("ログファイル削除"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
     const int result = dlg->ShowModal();

     if (result == wxID_YES) 
     {
	  // TODO
	  //wxRemoveFile(filePath);
     }
}
/**
 * お気に入り以外のすべてのスレッド一覧情報を削除する
 */
void XrossBoard::DeleteBAllDatFileWithoutFav(wxCommandEvent& event) 
{

}
/**
 * スレッドのURLをクリップボードにコピーする
 */
void XrossBoard::CopyTURLToClipBoard(wxCommandEvent& event) 
{
     const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     const wxString boardNameAscii = tiHash[title].boardNameAscii;
     const wxString origNumber = tiHash[title].origNumber;

     wxString threadURL = GetThreadURL(title, boardNameAscii, origNumber);

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(threadURL));
	  wxTheClipboard->Close();
     }
}
/**
 * スレッドのタイトルをクリップボードにコピーする
 */
void XrossBoard::CopyTTitleToClipBoard(wxCommandEvent& event) {

     wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(title));
	  wxTheClipboard->Close();
     }
}
/**
 * スレッドのURLとタイトルをクリップボードにコピーする
 */
void XrossBoard::CopyTBothDataToClipBoard(wxCommandEvent& event) 
{
     wxString title, boardNameAscii, origNumber, boardURL;

     title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     boardNameAscii = tiHash[title].boardNameAscii;
     origNumber = tiHash[title].origNumber;

     // 仕方がないので総当りでハッシュからURLを探す
     NameURLHash::iterator it;
     for (it = retainHash.begin(); it != retainHash.end(); ++it) 
     {
	  wxString key = it->first;
	  URLvsBoardName value = it->second;

	  if (value.boardNameAscii == boardNameAscii) 
	  {
	       boardURL = value.boardURL;
	       break;
	  }
     }

     wxString threadURL = boardURL;

     // ホスト名の後の板名を除く
     int begin = threadURL.Find(boardNameAscii);
     if (begin == wxNOT_FOUND) 
     {
	  return;
     }
     threadURL = threadURL.Mid(0, begin);
     threadURL += wxT("test/read.cgi/");
     threadURL += boardNameAscii;
     threadURL += wxT("/");
     threadURL += origNumber;
     threadURL += wxT("/");

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->SetData(new wxTextDataObject(title + wxT("\n") + threadURL));
	  wxTheClipboard->Close();
     }
}
/**
 * 保存されているログをスレッド一覧に表示する
 */
void XrossBoard::CheckLogDirectory(wxCommandEvent& event) 
{
     // ファイルパスの組み立て
     wxDir xbDir(::wxGetHomeDir() + wxFILE_SEP_PATH + XROSSBOARD_DIR);
     wxString filePath = xbDir.GetName() + wxFILE_SEP_PATH + wxT("dat");     

     // datファイルパスを開く
     wxDir datDir(filePath);
     if ( !datDir.IsOpened() )
     {
	  return;
     }

     wxString dirName = datDir.GetName();
     wxArrayString allFileList;
     datDir.GetAllFiles(dirName, &allFileList, wxEmptyString, wxDIR_DIRS | wxDIR_FILES);
     allFileList.Shrink();

     // datファイルのみの配列に置き換える
     wxArrayString datList;

     for (unsigned int i=0;i < allFileList.GetCount();i++) 
     {
	  wxFileName* filename = new wxFileName(allFileList[i], wxPATH_NATIVE);
	  if(filename->GetName().IsNumber() && allFileList[i].Contains(wxT(".dat"))) 
	  {
	       // ファイル名が数字10桁で拡張子が「.dat」のものを取得する
	       datList.Add(allFileList[i]);
	  }
     }
     datList.Shrink();

     // ノートブックの変更中はノートブックに触れないようにする
#ifndef __WXMSW__
     boardNoteBook->Freeze();
#endif

     // 新規にセットされる板名かどうかのフラグを用意する
     bool itIsNewBoardName = true;
     // 次に選択されるべきタブのページ数を格納する変数
     size_t selectedPage = 0;
     // 板名は「ログ一覧」で固定する
     wxString boardName = wxT("ログ一覧");

     // ユーザーが開いているタブの板名を調べる
     for (unsigned int i = 0; i < boardNoteBook->GetPageCount(); i++) 
     {
	  if (boardName.Cmp(boardNoteBook->GetPageText(i)) == 0) 
	  {
	       itIsNewBoardName = false;
	       selectedPage = i;
	       break;
	  }
     }

     // 仮想リストのインスタンス（スコープの外で宣言したい）
     VirtualBoardListCtrl* vbListCtrl;

     if (itIsNewBoardName) 
     {
	  /*
	   * 新規にログ一覧を作成する場合
	   */

	  // ログ一覧用のインスタンスを準備する
	  vbListCtrl = new VirtualBoardListCtrl(boardNoteBook, boardName, datList);
	  // スレッドリストを表示させる
	  boardNoteBook->AddPage(vbListCtrl, boardName, false);

     } 
     else 
     {
	  /*
	   * 既にログ一覧がタブにある場合
	   */

	  // ログ一覧用のインスタンスを準備する
	  vbListCtrl = new VirtualBoardListCtrl(boardNoteBook, boardName, datList);
	  boardNoteBook->DeletePage(selectedPage);
	  boardNoteBook->InsertPage(selectedPage, vbListCtrl, boardName, false, wxNullBitmap);
     }

     // カラムの幅を最大化
     wxFont font = GetCurrentFont();
     int pointSize = font.GetPointSize();
     // 2chのスレタイの文字数制限は全角24文字
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_CHK      , 20);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_TITLE    , pointSize * 52);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_SINCE    , pointSize * 12);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_OID      , pointSize * 10);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_BOARDNAME, pointSize * 12);

     // ノートブックの選択処理
     boardNoteBook->SetSelection(selectedPage);
#ifndef __WXMSW__
     boardNoteBook->Thaw();
#endif
     boardNoteBook->Update();
}
/**
 * スレタブをひとつ閉じる
 */
void XrossBoard::OneThreadTabClose(wxCommandEvent& event) 
{
     // アクティブなタブを選択して閉じる
#if wxCHECK_VERSION(3, 0, 0)
     wxAuiNotebookEvent e(wxEVT_AUINOTEBOOK_PAGE_CLOSE);
#else
     wxAuiNotebookEvent e(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE);
#endif
     e.SetSelection(threadNoteBook->GetSelection());
     e.SetId(ID_ThreadNoteBook);
     OnAboutCloseThreadNoteBook(e);
     threadNoteBook->DeletePage(threadNoteBook->GetSelection());
}
/**
 * 現在選択されていないスレタブを閉じる
 */
void XrossBoard::ExcepSelThreadTabClose(wxCommandEvent& event) 
{
     // タブの数を数える
     const size_t pages = threadNoteBook->GetPageCount();
     const size_t select = threadNoteBook->GetSelection();
     bool delete_f = false;

     for (unsigned int i = 0; i < pages; i++) 
     {
#if wxCHECK_VERSION(3, 0, 0)
	  wxAuiNotebookEvent e(wxEVT_AUINOTEBOOK_PAGE_CLOSE);
#else
	  wxAuiNotebookEvent e(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE);
#endif
	  e.SetSelection(i);
	  e.SetId(ID_ThreadNoteBook);

	  if (i != select && !delete_f) 
	  {
	       OnAboutCloseThreadNoteBook(e);
	       threadNoteBook->DeletePage(0);
	  } 
	  else if (i == select && !delete_f) 
	  {
	       OnAboutCloseThreadNoteBook(e);
	       threadNoteBook->DeletePage(1);
	       delete_f = true;
	  } 
	  else if (i != select && delete_f) 
	  {
	       OnAboutCloseThreadNoteBook(e);
	       threadNoteBook->DeletePage(1);
	  }
     }
}
/**
 * すべてのスレタブを閉じる
 */
void XrossBoard::AllThreadTabClose(wxCommandEvent& event) 
{
     const int pages = threadNoteBook->GetPageCount();

     for (int i = 0; i < pages; i++) 
     {
#if wxCHECK_VERSION(3, 0, 0)
	  wxAuiNotebookEvent e(wxEVT_AUINOTEBOOK_PAGE_CLOSE);
#else
	  wxAuiNotebookEvent e(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE);
#endif
	  e.SetSelection(i);
	  e.SetId(ID_ThreadNoteBook);
	  OnAboutCloseThreadNoteBook(e);
	  threadNoteBook->DeletePage(0);
     }
}
/**
 * これより左のスレタブをを閉じる
 */
void XrossBoard::AllLeftThreadTabClose(wxCommandEvent& event) 
{
     // タブの数を数える
     const size_t select = threadNoteBook->GetSelection();

     for (unsigned int i = 0; i < select; i++) 
     {
	  // wxAuiNotebookEvent e(wxEVT_AUINOTEBOOK_PAGE_CLOSE);
	  // e.SetSelection(select - 2);
	  // e.SetId(ID_ThreadNoteBook);
	  // OnAboutCloseThreadNoteBook(e);
	  threadNoteBook->DeletePage(0);
     }
}
/**
 * これより右のスレタブを閉じる
 */
void XrossBoard::AllRightThreadTabClose(wxCommandEvent& event) 
{
     // タブの数を数える
     const size_t pages  = threadNoteBook->GetPageCount();
     const size_t select = threadNoteBook->GetSelection();

     for (unsigned int i = 0; i < pages; i++) 
     {
	  if ( i > select ) 
	  {
	       // wxAuiNotebookEvent e(wxEVT_AUINOTEBOOK_PAGE_CLOSE);
	       // e.SetSelection(select + 2);
	       // e.SetId(ID_ThreadNoteBook);
	       // OnAboutCloseThreadNoteBook(e);
	       threadNoteBook->DeletePage(select+1);
	  }
     }
}
/**
 * スレッドをブラウザで開く
 */
void XrossBoard::OnOpenThreadByBrowser(wxCommandEvent& event) 
{
     CopyTURLToClipBoard(event);

     if (wxTheClipboard->Open()) 
     {
	  wxTextDataObject data;
	  wxTheClipboard->GetData(data);
	  wxString url = data.GetText();
	  wxTheClipboard->Close();
	  wxLaunchDefaultBrowser(url);
     }
}
/**
 * datファイルに名前を付けて保存
 */
void XrossBoard::SaveDatFile(wxCommandEvent& event) 
{
     wxString caption = wxT("datファイルに名前を付けて保存");
     wxString defaultDir = wxEmptyString; // OSのデフォルトに合わせる
     wxString wildCard = wxT("dat files (*.dat) |*.dat");

     // datファイル名の組み立て
     wxString title, boardNameAscii, origNumber, boardURL;

     title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     boardNameAscii = tiHash[title].boardNameAscii;
     origNumber = tiHash[title].origNumber;

     // ファイルパスの組み立てとファイルの有無確認
     wxString filePath = XrossBoardUtil::AssembleFilePath(boardNameAscii, origNumber, KIND_THREAD_DAT);

     if (!wxFile::Exists(filePath)) 
     {
	  // 無ければエラーメッセージ表示
	  wxMessageBox(wxT("保存するためのdatファイルが見つかりませんでした"));
	  return;
     }

     wxFileDialog dialog(this, caption, defaultDir, wxEmptyString, wildCard, wxFD_SAVE);
     dialog.SetPath(filePath);

     if (dialog.ShowModal() == wxID_OK) 
     {
	  bool ret = wxCopyFile(filePath, dialog.GetPath(), true);
	  if (!ret) 
	  {
	       wxMessageBox(wxT("datファイルの保存に失敗しました"));
	  }
     }
}
/**
 * datをクリップボードにコピー
 */
void XrossBoard::SaveDatFileToClipBoard(wxCommandEvent& event) 
{
     // datファイル名の組み立て
     wxString title, boardNameAscii, origNumber, boardURL;

     title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     boardNameAscii = tiHash[title].boardNameAscii;
     origNumber = tiHash[title].origNumber;

     // ファイルパスの組み立てとファイルの有無確認
     wxString filePath = XrossBoardUtil::AssembleFilePath(boardNameAscii, origNumber, KIND_THREAD_DAT);

     if (!wxFile::Exists(filePath)) 
     {
	  // 無ければエラーメッセージ表示
	  wxMessageBox(wxT("保存するためのdatファイルが見つかりませんでした"));
	  return;
     }

     if (wxTheClipboard->Open()) 
     {
	  wxFileDataObject* file = new wxFileDataObject();
	  file->AddFile(filePath);

	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(file);
	  wxTheClipboard->Close();
     }
}
/**
 * このログを削除
 */
void XrossBoard::DeleteDatFile(wxCommandEvent& event) 
{
     // datファイル名の組み立て
     wxString title, boardNameAscii, origNumber, boardURL;

     title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     boardNameAscii = tiHash[title].boardNameAscii;
     origNumber = tiHash[title].origNumber;

     // ファイルパスの組み立てとファイルの有無確認
     wxString filePath = XrossBoardUtil::AssembleFilePath(boardNameAscii, origNumber, KIND_THREAD_DAT);

     if (!wxFile::Exists(filePath)) 
     {
	  // 無ければエラーメッセージ表示
	  wxMessageBox(wxT("削除するためのdatファイルが見つかりませんでした"));
	  return;
     }

     wxString message = wxT("ファイル") + filePath + wxT("を削除してよろしいですか？");
     wxMessageDialog* dlg = new wxMessageDialog(this, message, wxT("ログファイル削除"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
     const int result = dlg->ShowModal();

     if (result == wxID_YES) 
     {
	  wxRemoveFile(filePath);
     }
}
/**
 * スレッドの再読み込み
 */
void XrossBoard::ReloadThisThread(wxCommandEvent& event) 
{
     // 選択されたスレタブの情報を集める
     wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     ReloadThread(title);
}
/**
 * 指定された名前のスレッドを更新する
 */
void XrossBoard::ReloadThreadByName(wxCommandEvent& event) 
{     
     wxString title = event.GetString();
     ReloadThread(title);
}
/**
 * 指定されたタイトルのスレッドを更新する
 */
void XrossBoard::ReloadThread(wxString& title) 
{
     const size_t page = threadNoteBook->GetSelection();
     wxString boardName,boardURL, origNumber, boardNameAscii;
     ThreadInfo info = tiHash[title];
     origNumber = info.origNumber;
     boardNameAscii = info.boardNameAscii;

     // 仕方がないので総当りでハッシュからURLを探す
     NameURLHash::iterator it;
     for (it = retainHash.begin(); it != retainHash.end(); ++it) 
     {
	  wxString key = it->first;
	  URLvsBoardName value = it->second;

	  if (value.boardNameAscii == boardNameAscii) 
	  {
	       boardName = value.boardName;
	       boardURL = value.boardURL;
	       break;
	  }
     }

     // ソケット通信を行う
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());
     const wxString threadContentPath = sock->DownloadThread(boardName, boardURL, boardNameAscii, origNumber);
     
     // 無事に通信が終了したならばステータスバーに表示
     this->SetStatusText(wxT(" スレッドのダウンロード終了"));

     // 既存のページから情報を取得して削除する
     ThreadContentBar* oldThreadBar = dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(page));

     if (oldThreadBar) 
     {
	  wxPoint p;
	  oldThreadBar->GetThreadContentWindowScrollPos(&p);
	  oldThreadBar->ReloadThreadContentWindow(threadContentPath);
	  oldThreadBar->SetThreadContentWindowScroll(&p);
     }
     
     // ノートブックに登録されたスレッド情報をハッシュに登録する
     info.origNumber = origNumber;
     info.boardNameAscii = boardNameAscii;
     tiHash[title] = info;

     wxString message = wxT("完了…　(´ん｀/)三\n");
     SendLogging(message);
}
/**
 *  書き込み用のウィンドウを呼び出す
 */
void XrossBoard::CallResponseWindow(wxCommandEvent& event) 
{
     // 必要な構造体を宣言する
     ThreadInfo threadInfoHash;
     URLvsBoardName boardInfoHash;

     // 選択されたスレタブの情報を集める
     wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     threadInfoHash = tiHash[title];
     threadInfoHash.title = title; // タイトル情報を設定する

     // ハッシュからURLを探す
     NameURLHash::iterator it;
     for (it = retainHash.begin(); it != retainHash.end(); ++it) 
     {
	  wxString key = it->first;
	  boardInfoHash = it->second;

	  if (boardInfoHash.boardNameAscii == threadInfoHash.boardNameAscii) break;
     }

     // ウィンドウの大きさを取得する
     int wScreenPx, hScreenPx;
     ::wxDisplaySize(&wScreenPx, &hScreenPx);
     // レス用のウィンドウは 640:480なので、ちょうど中央にくるように調整する
     wxPoint point(wScreenPx/2 - 320, hScreenPx/2 - 240);
     ResponseWindow* response = new ResponseWindow(this, title, boardInfoHash, threadInfoHash, point, m_logCtrl);
     // ログ出力ウィンドウのインスタンスのポインタを渡す
     response->Show(true);
}
/**
 * 板一覧更ツリーの初期化
 */
void XrossBoard::InitializeBoardList() 
{     
     // 2ch板一覧ツリーの初期化
     this->Initialize2chBoardList();

     // お気に入り一覧ツリーの初期化
     this->InitializeFavsList();
     
     // 閲覧中の一覧ツリーの初期化
     this->InitializeNowReadingList();

#ifdef USE_SHINGETSU
     // 新月公開ノードの初期化
     this->InitializeShingetsuNodeList();
#endif /** USE_SHINGETSU */
}
/**
 * 2ch板一覧ツリーの初期化
 */
void XrossBoard::Initialize2chBoardList() {

     // ArrayStringの形で板一覧情報を取得する
     wxArrayString boardInfoArray = SQLiteAccessor::GetBoardInfo();
     // カテゴリ名一時格納用
     wxString categoryName;
     // 板名一時格納用
     wxString boardName;
     // URL一時格納用
     wxString url;
     // Sizer
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

     // ツリー用ウィジェットのインスタンスを用意する
     m_tree_ctrl = new wxTreeCtrl(m_boardTreePanel, ID_BoardTreectrl, wxDefaultPosition, wxDefaultSize, 
				  wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);

     // 検索用ツールバーを設定する
#ifndef __WXMAC__ /** Windows & Linux */
     CreateCommonAuiToolBar(m_boardTreePanel, vbox, ID_BoardSearchBar);
     vbox->Add(m_tree_ctrl, 1, wxEXPAND, 0);

#else
     /** Mac OS X */
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, 
				       ID_BoardTreectrl, 
				       wxT("CommonAuiToolBarUpdate"), 
				       this);
#endif

     // ツリーコントロールの共通設定を実行
     XrossBoardUiUtil::SetTreeCtrlCommonSetting(m_tree_ctrl, ID_BoardTreectrl);

     // カテゴリ名を保持するためのID
     wxTreeItemId category;
     // Hashのカウント用Integer
     int hashID = 0;

     // 板一覧情報をツリーに渡す
     for (unsigned int i = 0; i < boardInfoArray.GetCount(); i += 3) {

	  // カテゴリをツリーに登録
	  if (categoryName != boardInfoArray[i + 2]) {
	       category = m_tree_ctrl->AppendItem(m_tree_ctrl->GetRootItem(), boardInfoArray[i + 2]);
	       m_tree_ctrl->SetItemImage(category, 0, wxTreeItemIcon_Normal);
	  }
	  // それぞれの要素を一時格納
	  boardName = boardInfoArray[i];
	  url = boardInfoArray[i + 1];
	  categoryName = boardInfoArray[i + 2];

	  // ツリーに板名を追加する
	  wxTreeItemId itemIdTemp = m_tree_ctrl->AppendItem(category, boardName);
	  m_tree_ctrl->SetItemImage(itemIdTemp, 1, wxTreeItemIcon_Normal);

	  // 板名の配列に板名とURLを入れておく
	  URLvsBoardName urlVsName;
	  urlVsName.boardName = boardName;
	  urlVsName.boardURL = url;

	  // 正規表現を使ってサーバ名と板名(ascii)を取得する
	  // そこまで難しい正規表現を使う必要はないようです
	  wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);

	  // 正規表現のコンパイルにエラーがなければ
	  if (reThreadList.IsValid()) {
	       // マッチさせる
	       if (reThreadList.Matches(url)) {
		    // マッチした文字列の３番目をいただく
		    urlVsName.boardNameAscii = reThreadList.GetMatch(url, 3);
	       }
	  }
	  // Hashに板情報を入れる
	  if (!boardName.IsEmpty())
	       retainHash[(const wxString) boardName]
		    = (const URLvsBoardName&) urlVsName;
	  // Hashのキー値をインクリメントしておく
	  hashID++;
     }
     // 展開
     m_tree_ctrl->Expand(m_tree_ctrl->GetRootItem());

     // パネルにSizerを設定する
     m_boardTreePanel->SetSizer(vbox);
     m_boardTreePanel->Layout();
}

/**
 * お気に入り一覧ツリーの初期化
 */
void XrossBoard::InitializeFavsList() {

     // Sizer
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

     // ツリー用ウィジェットのインスタンスを用意する
     m_fav_tree_ctrl = new wxTreeCtrl(m_favoriteTreePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
				      wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);
     vbox->Add(m_fav_tree_ctrl, 1, wxEXPAND, 0);

     // 検索用ツールバーを設定する
#ifndef __WXMAC__ /** Windows & Linux */
     CreateCommonAuiToolBar(m_favoriteTreePanel, vbox, ID_FavsSearchBar);
#else
     /** Mac OS X */
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, 
				       ID_FavsTreectrl, 
				       wxT("CommonAuiToolBarUpdate"), 
				       this);
#endif

     // ツリーコントロールの共通設定を実行
     XrossBoardUiUtil::SetTreeCtrlCommonSetting(m_fav_tree_ctrl, ID_FavsTreectrl);

     // パネルにSizerを設定する
     m_favoriteTreePanel->SetSizer(vbox);
     m_favoriteTreePanel->Layout();
}
     
/**
 * 閲覧中の一覧ツリーの初期化
 */
void XrossBoard::InitializeNowReadingList() {

     // Sizer
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

     // ツリー用ウィジェットのインスタンスを用意する
     m_now_reading_tree_ctrl = new wxTreeCtrl(m_nowReadingTreePanel, 
					      wxID_ANY, 
					      wxDefaultPosition, 
					      wxDefaultSize, 
					      wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);

     // 検索用ツールバーを設定する
#ifndef __WXMAC__ /** Windows & Linux */
     CreateCommonAuiToolBar(m_nowReadingTreePanel, vbox, ID_NowReadingSearchBar);
     vbox->Add(m_now_reading_tree_ctrl, 1, wxEXPAND, 0);
#else
     /** Mac OS X */
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, 
				       ID_NowReadingTreectrl, 
				       wxT("CommonAuiToolBarUpdate"), 
				       this);
#endif

     // ツリーコントロールの共通設定を実行
     XrossBoardUiUtil::SetTreeCtrlCommonSetting(m_now_reading_tree_ctrl, ID_NowReadingTreectrl);

     // パネルにSizerを設定する
     m_nowReadingTreePanel->SetSizer(vbox);
     m_nowReadingTreePanel->Layout();
}

#ifdef USE_SHINGETSU
/**
 *  新月公開ノードの初期化
 */
void XrossBoard::InitializeShingetsuNodeList() {

     // ArrayStringの形で板一覧情報を取得する
     wxArrayString shingetsuInfoArray = SQLiteAccessor::GetShingetsuNodeList();
     
     // URL一時格納用
     wxString url;
     // Sizer
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
     // 検索用ツールバーを設定する
     CreateCommonAuiToolBar(m_shingetsuTreePanel, vbox, ID_ShingetsuBoardSearchBar);

     // ツリー用ウィジェットのインスタンスを用意する
     m_shingetsu_tree_ctrl = new wxTreeCtrl(m_shingetsuTreePanel, ID_ShingetsuBoardTreectrl, wxDefaultPosition, wxDefaultSize, 
					    wxTR_HAS_BUTTONS|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);
     vbox->Add(m_shingetsu_tree_ctrl, 1, wxEXPAND, 0);

     wxTreeItemData treeData;
     wxTreeItemId m_rootId;

     // イメージリストにアイコンを登録する
     wxImageList* treeImage = new wxImageList(16, 16);
     wxBitmap idx1(folderImg, wxBITMAP_TYPE_PNG);
     treeImage->Add(idx1);
     wxBitmap idx2(textHtmlImg), wxBITMAP_TYPE_PNG);
     treeImage->Add(idx2);
     m_shingetsu_tree_ctrl->AssignImageList(treeImage);
     m_shingetsu_tree_ctrl->SetLabel(SHINGETU_NODE_TREE);
     wxTreeItemId rootTemp = m_shingetsu_tree_ctrl->AddRoot(wxT("新月公開ノード一覧"));
     m_tree_ctrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);

     for (unsigned int i = 0; i < shingetsuInfoArray.GetCount(); i++ ) {
	  m_shingetsu_tree_ctrl->AppendItem(m_shingetsu_tree_ctrl->GetRootItem(), shingetsuInfoArray[i], 1, 1);
     }

     // 展開
     m_tree_ctrl->Expand(m_shingetsu_tree_ctrl->GetRootItem());

     // パネルにSizerを設定する
     m_shingetsuTreePanel->SetSizer(vbox);
     m_shingetsuTreePanel->Layout();
}
#endif /** USE_SHINGETSU */

/**
 * 板一覧リストのツリーのウィジェットをDBからリロードするイベント
 */
void XrossBoard::SetBoardList(wxCommandEvent& event)
{
     this->SetBoardList(true);
}
/**
 * SQLiteから板一覧情報を抽出してレイアウトに反映するメソッド
 * @param bool updateHash true:内部の板一覧情報ハッシュを更新する, false:更新しない
 */
void XrossBoard::SetBoardList(const bool updateHash)
{

     // ArrayStringの形で板一覧情報を取得する
     wxArrayString boardInfoArray = SQLiteAccessor::GetBoardInfo();
     // カテゴリ名一時格納用
     wxString categoryName;
     // 板名一時格納用
     wxString boardName;
     // URL一時格納用
     wxString url;
     // カテゴリ名を保持するためのID
     wxTreeItemId category;
     // Hashのカウント用Integer
     int hashID = 0;
     // 一度中身を削除する
     m_tree_ctrl->CollapseAndReset(m_tree_ctrl->GetRootItem());
     // 板一覧情報ハッシュは初期化する
     if ( updateHash ) {
	  retainHash.clear();
     }

     // 板一覧情報をツリーに渡す
     for (unsigned int i = 0; i < boardInfoArray.GetCount(); i += 3) {
	  // カテゴリをツリーに登録
	  if (categoryName != boardInfoArray[i + 2]) {
	       category = m_tree_ctrl->AppendItem(m_tree_ctrl->GetRootItem(), boardInfoArray[i + 2]);
	       m_tree_ctrl->SetItemImage(category, 0, wxTreeItemIcon_Normal);
	  }
	  // それぞれの要素を一時格納
	  boardName = boardInfoArray[i];
	  url = boardInfoArray[i + 1];
	  categoryName = boardInfoArray[i + 2];

	  // ツリーに板名を追加する
	  wxTreeItemId tmp = m_tree_ctrl->AppendItem(category, boardName);
	  m_tree_ctrl->SetItemImage(tmp, 1, wxTreeItemIcon_Normal);

	  // 板名の配列に板名とURLを入れておく
	  URLvsBoardName urlVsName;
	  urlVsName.boardName = boardName;
	  urlVsName.boardURL = url;

	  // 正規表現を使ってサーバ名と板名(ascii)を取得する
	  // そこまで難しい正規表現を使う必要はないようです
	  wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);

	  // 正規表現のコンパイルにエラーがなければ
	  if (reThreadList.IsValid()) {
	       // マッチさせる
	       if (reThreadList.Matches(url)) {
		    // マッチした文字列の３番目をいただく
		    urlVsName.boardNameAscii = reThreadList.GetMatch(url, 3);
	       }
	  }
	  // Hashに板情報を入れる
	  if (!boardName.IsEmpty() && updateHash) {
	       // 板情報ハッシュを更新する
	       retainHash[boardName] = urlVsName;
	  }
	  // Hashのキー値をインクリメントしておく
	  hashID++;
     }

#if !wxCHECK_VERSION(2, 9, 0)
     m_tree_ctrl->Expand(m_tree_ctrl->GetRootItem());
#endif

}
/**
 * バージョン情報が書かれたダイアログを表示する
 */
void XrossBoard::OnVersionInfo(wxCommandEvent&) {

     wxAboutDialogInfo info;
     info.SetName(wxT("XrossBoard - 汎用掲示板クライアント"));
     info.AddDeveloper(wxT("Hiroyuki Nagata    newserver002@gmail.com"));
     info.AddDeveloper(wxT("K.Watanabe         kwtnb@outlook.com"));
     info.AddDeveloper(wxT("Vabock             vabock@gmail.com"));
     info.SetVersion(xrossboardVersion);
     info.SetCopyright(wxT("Copyright(C) 2015 Hiroyuki Nagata, All Rights Reserved. "));
     info.SetWebSite(wxT("http://nantonaku-shiawase.hatenablog.com/"));

     // 説明を追加
     wxString description = wxT("wxWidgetsのバージョン:");
     description << wxVERSION_STRING;
     description << wxT("\n");
     description << wxT("Curlのバージョン:");

     /** curlをwxWidgetsから呼ぶテスト */
     curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
     description << wxString::From8BitData(data->version);
     description << wxT("\n");
     description << wxT("対応プロトコル:");

     for (int i = 0; i < sizeof data->protocols; i++) {
	  description << wxString::From8BitData(data->protocols[i]);
	  description << wxT(", ");	  
     }

     description << wxT("OpenSSLのバージョン:");
     description << wxString::From8BitData(data->ssl_version);
     description << wxT("\n");

     description << wxT("zlibのバージョン");
     description << wxString::From8BitData(data->libz_version);
     description << wxT("\n");

     info.SetDescription(description);

     // ライセンスを読み込む
     wxTextFile licenceFile;
     licenceFile.Open(licencePath, wxConvUTF8);
     wxString licence;

     // ファイルがオープンされているならば
     if (licenceFile.IsOpened()) 
     {
	  for (licence << licenceFile.GetFirstLine(); !licenceFile.Eof(); licence << licenceFile.GetNextLine()) 
	  {
	       licence << wxT("\n");
	  }
     }
     // ここまで来てしまった場合空文字を返す
     licenceFile.Close();
     info.SetLicence(licence);

     wxAboutBox(info);
}
/**
 * 終了前処理では、保存しておきたいユーザー設定をSqliteに登録しておく
 */
void XrossBoard::OnCloseWindow(wxCloseEvent& event) {

     // 終了処理中と表示する
     XrossBoardUiUtil::SendLoggingHelper(wxT("終了前処理を実行中..."));

     /**
      * 開いていた板の名前をsqliteに登録する
      */
     wxArrayString userLookingBoardName;
     size_t bpages = boardNoteBook->GetPageCount();

     for (unsigned int i = 0; i < bpages; i++) {
	  wxString pageText = boardNoteBook->GetPageText((size_t) i);
	  // 空文字でなければ追加する
	  if (!pageText.IsEmpty()) {
	       userLookingBoardName.Add(pageText);
	  }
     }

     // 開いていた板の一覧をsqliteに送る
     SQLiteAccessor::SetUserLookingBoardList(userLookingBoardName);

     /**
      * 開いていたスレッドの情報をsqliteに登録する
      */
     wxArrayString userLookingThreadName;
     size_t tpages = threadNoteBook->GetPageCount();

     for (unsigned int i = 0; i < tpages; i++) {
	  wxString pageText = threadNoteBook->GetPageText((size_t) i);
	  // 空文字でなければ追加する
	  if (!pageText.IsEmpty()) {
	       userLookingThreadName.Add(pageText);
	       userLookingThreadName.Add(tiHash[pageText].origNumber);
	       userLookingThreadName.Add(tiHash[pageText].boardNameAscii);
	  }
     }
     // 開いていたスレッドの一覧をsqliteに送る
     SQLiteAccessor::SetUserLookingThreadList(userLookingThreadName);

     // wxAuiManagerのレイアウトの情報を保存する
     const wxString perspective = m_mgr.SavePerspective();
     XrossBoardUtil::SetXrossBoardProperties(wxT("Perspective"), perspective);
     // フレームのレイアウト情報を保存する
     int x, y;
     this->GetSize(&x, &y);
     XrossBoardUtil::SetXrossBoardProperties(wxT("FrameX"), static_cast<long>(x));
     XrossBoardUtil::SetXrossBoardProperties(wxT("FrameY"), static_cast<long>(y));

     int px, py;
     this->GetPosition(&px, &py);
     XrossBoardUtil::SetXrossBoardProperties(wxT("FramePx"), static_cast<long>(px));
     XrossBoardUtil::SetXrossBoardProperties(wxT("FramePy"), static_cast<long>(py));

#ifndef __WXMAC__ // MaximizeはMacOSXではサポートされない
     // ウィンドウの最大化情報
     bool isMaximized = this->IsMaximized();
     XrossBoardUtil::SetXrossBoardProperties(wxT("IsMaximaized"), isMaximized);     
#endif

     // 板一覧ツリーの情報
     const bool toggled = m_floatToolBar->GetToolToggled(ID_ShowBoardListTree);
     XrossBoardUtil::SetXrossBoardProperties(wxT("ShowBoardListTree"), toggled);

     // 画面の分割を縦横どちらにするか
     XrossBoardUtil::SetXrossBoardProperties(wxT("SeparateXY"), separateIsX);
     // 画面のペイン数を何個にするか
     XrossBoardUtil::SetXrossBoardProperties(wxT("PaneIsThree"), paneIsThree);
     // GUI右側に表示するウィジェットは何か
     XrossBoardUtil::SetXrossBoardProperties(wxT("RightIsThreadList"), rightIsThreadList);

     SetStatusText(wxT("終了前処理が終わりました！"));

     Destroy();
}
/**
 * お気に入りに追加(スレッド)
 */
void XrossBoard::AddThreadFavorite(wxCommandEvent& event) {

     // お気に入りに追加するタブのタイトルを取得
     wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
     // 固有番号を取得
     wxString origNumber = tiHash[title].origNumber;
     // スレッドの情報をSQLiteに格納する
     ThreadInfo t;
     t.title = title;
     t.origNumber = origNumber;
     t.boardNameAscii = tiHash[title].boardNameAscii;
     SQLiteAccessor::SetThreadInfo(&t, event.GetId());

     wxString message = wxT("「") + title + wxT("」をお気に入りに追加") + wxT("（  ´ん｀）");
     SendLogging(message);
}
/**
 * お気に入りを削除(スレッド)
 */
void XrossBoard::DelThreadFavorite(wxCommandEvent& event) {

}
/**
 * お気に入りに追加(板)
 */
void XrossBoard::AddBoardFavorite(wxCommandEvent& event) {

     // お気に入りに追加するタブのタイトルを取得
     wxString title = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
     // 固有番号は「0」で登録
     wxString origNumber = wxT("0");
     // 板名を取得
     URLvsBoardName hash = retainHash[title];
     wxString boardNameAscii = hash.boardNameAscii;

     // 板の情報スレッドとしてをSQLiteに格納する
     ThreadInfo t;
     t.title = title;
     t.origNumber = origNumber;
     t.boardNameAscii = boardNameAscii;
     SQLiteAccessor::SetThreadInfo(&t, event.GetId());

     wxString message = wxT("「") + title + wxT("」をお気に入りに追加") + wxT("（  ´ん｀）");
     SendLogging(message);
}
/**
 * お気に入りを削除(板)
 */
void XrossBoard::DelBoardFavorite(wxCommandEvent& event) {

}
/**
 * すべてのタブをお気に入りに追加
 */
void XrossBoard::AddAllThreadFavorite(wxCommandEvent& event) {

     for ( size_t i = 0; i < threadNoteBook->GetPageCount(); i++ ) {

	  wxAuiNotebook* page = dynamic_cast<wxAuiNotebook*>(threadNoteBook->GetPage(i));
	  if (page == NULL) continue;
	  
	  wxString title = page->GetPageText(i);
	  // 固有番号を取得
	  wxString origNumber = tiHash[title].origNumber;
	  // スレッドの情報をSQLiteに格納する
	  ThreadInfo t;
	  t.title = title;
	  t.origNumber = origNumber;
	  t.boardNameAscii = tiHash[title].boardNameAscii;
	  SQLiteAccessor::SetThreadInfo(&t, ID_AddThreadFavorite);

	  wxString message = wxT("「") + title + wxT("」をお気に入りに追加") + wxT("（  ´ん｀）");
	  SendLogging(message);
     }
}
/**
 * 表示しているタブを移動する(次)
 */
void XrossBoard::MoveNextTab(wxCommandEvent& event) {

     if (this->userLastAttachedNotebook == BOARD_NOTEBOOK) {
	  boardNoteBook->AdvanceSelection(true);
     } else if (this->userLastAttachedNotebook == THREAD_NOTEBOOK) {
	  threadNoteBook->AdvanceSelection(true);
     }
}
/**
 * 表示しているタブを移動する(前)
 */
void XrossBoard::MovePrevTab(wxCommandEvent& event) {

     if (this->userLastAttachedNotebook == BOARD_NOTEBOOK) {
	  boardNoteBook->AdvanceSelection(false);
     } else if (this->userLastAttachedNotebook == THREAD_NOTEBOOK) {
	  threadNoteBook->AdvanceSelection(false);
     }
}
/**
 * 板一覧リストでのクリック時のイベント
 */
void XrossBoard::OnLeftClickAtListCtrl(wxListEvent& event) {

     wxString message1 = wxT("スレッド取得　三　(　＾ν）\n");
     SendLogging(message1);

     switch (event.GetId()) {

#ifdef USE_SHINGETSU
     case ID_ShingetsuBoardListCtrl:
	  this->OnLeftClickAtListCtrlShingetsu(event);
	  break;
#endif /** USE_SHINGETSU */
     case ID_BoardListCtrl:
	  this->OnLeftClickAtListCtrl2ch(event);
	  break;
     default:
	  wxMessageBox(wxT("内部エラー, スレッドダウンロード処理に失敗しました."), wxT("スレッド一覧リスト"), wxICON_ERROR);
	  break;
     }

     wxString message2 = wxT("完了…　(´ん｀/)三\n");
     SendLogging(message2);
}
/**
 * スレッドのダウンロード2ch向け
 */
void XrossBoard::OnLeftClickAtListCtrl2ch(wxListEvent& event) {

     // 現在アクティブになっているタブの板名を取得する
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // リストコントロールを引き出してくる
     VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName));
     if (vbListCtrl == NULL) {
	  wxMessageBox(wxT("内部エラー, スレッドダウンロード処理に失敗しました."), wxT("スレッド一覧リスト"), wxICON_ERROR);
	  return;
     }

     // Hashから情報を引き出す
     URLvsBoardName hash = retainHash[boardName];
     wxString boardURL = hash.boardURL;
     wxString boardNameAscii = hash.boardNameAscii;

     // スレの固有番号とタイトルをリストから取り出す
     const long index = event.GetIndex();
     const wxString origNumber(vbListCtrl->OnGetItemText(index, static_cast<long>(VirtualBoardListCtrl::Columns::COL_OID)));
     const wxString title(vbListCtrl->OnGetItemText(index, static_cast<long>(VirtualBoardListCtrl::Columns::COL_TITLE)));

     // ソケット通信を行う
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());
     const wxString threadContentPath = sock->DownloadThread(boardName, boardURL, boardNameAscii, origNumber);
     
     // 無事に通信が終了したならばステータスバーに表示
     this->SetStatusText(wxT(" スレッドのダウンロード終了"));

     // スレッドの内容をノートブックに反映する
     SetThreadContentToNoteBook(threadContentPath, origNumber, title);
     // ノートブックに登録されたスレッド情報をハッシュに登録する
     ThreadInfo info;
     info.origNumber = origNumber;
     info.boardNameAscii = boardNameAscii;
     tiHash[title] = info;
}

#ifdef USE_SHINGETSU
/**
 * スレッドのダウンロード新月向け
 */
void XrossBoard::OnLeftClickAtListCtrlShingetsu(wxListEvent& event) {

     // 現在アクティブになっているタブの板名を取得する
     wxString nodeHostname = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // リストコントロールを引き出してくる
     VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(nodeHostname));
     if (vbListCtrl == NULL) {
	  wxMessageBox(wxT("内部エラー, スレッドダウンロード処理に失敗しました."), wxT("スレッド一覧リスト"), wxICON_ERROR);
	  return;
     }

     // スレッドのファイル名を取り出す
     const long index = event.GetIndex();
     const wxString filename(vbListCtrl->m_vBoardList[index].getFilename());
     const wxString title(vbListCtrl->m_vBoardList[index].getTitle());

     // ソケット通信を行う
     this->SetStatusText(wxT(" 取得 - ") + filename);
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());
     const wxString threadContentPath = sock->DownloadShingetsuThread(nodeHostname, title, filename);
     
     // 無事に通信が終了したならばステータスバーに表示
     this->SetStatusText(wxT(" スレッドのダウンロード終了"));

}
#endif /** USE_SHINGETSU */
/**
 * スレッド一覧リストでリストのヘッダーにクリックした場合の処理
 */
void XrossBoard::OnLeftClickAtListCtrlCol(wxListEvent& event) 
{     
     // 現在アクティブになっているタブの板名を取得する
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // リストコントロールを引き出してくる
     VirtualBoardListCtrl* vbListCtrl 
	  = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName));
     if (vbListCtrl == NULL) 
     {
	  wxMessageBox(wxT("内部エラー, スレッドソート処理に失敗しました."), 
		       wxT("スレッド一覧リスト"), 
		       wxICON_ERROR);
	  return;
     }

     // 板名が一致するwindowクラスでソートをかける
     vbListCtrl->SortVectorItems(event.GetColumn());
}
/**
 * スレッドをノートブックに反映するメソッド
 */
void XrossBoard::SetThreadContentToNoteBook(const wxString& threadContentPath,
					   const wxString& origNumber, const wxString& title) {

     // スレッド用の検索バー等のインスタンスを用意する
     size_t page = threadNoteBook->GetPageCount();
     ThreadContentBar* threadBar = new ThreadContentBar(threadNoteBook, ID_ThreadContentBar);
     threadBar->SetTitle(title);

     // スレッドの内容はThreadContentBarの中で設定する
     threadBar->SetThreadContentWindow(threadContentPath, origNumber);
     threadNoteBook->AddPage(threadBar, title, true, wxBitmap(threadTabNewImg, wxBITMAP_TYPE_ANY));
     // 閲覧中ツリーの情報を更新する
     XrossBoardUiUtil::QueueEventHelper(wxEVT_UPDATE_UI, ID_NowReadingTreectrlUpdate);
}
/**
 * 板一覧ノートブックで右クリックされた時の処理
 */
void XrossBoard::OnRightClickBoardNoteBook(wxAuiNotebookEvent& event) {

     wxString selectedBoardName = boardNoteBook->GetPageText(event.GetSelection());

     wxMenu* boardTabUtil = new wxMenu();
     boardTabUtil->Append(ID_OneBoardTabClose, wxT("このタブを閉じる"));
     boardTabUtil->AppendSeparator();
     boardTabUtil->Append(ID_ExcepSelTabClose, wxT("このタブ以外を閉じる"));
     boardTabUtil->Append(ID_AllBoardTabClose, wxT("すべてのタブを閉じる"));
     boardTabUtil->Append(ID_AllLeftBoardTabClose, wxT("これより左を閉じる"));
     boardTabUtil->Append(ID_AllRightBoardTabClose, wxT("これより右を閉じる"));
     boardTabUtil->AppendSeparator();
     boardTabUtil->Append(ID_ReloadOneBoard, wxT("スレ一覧更新"));
     boardTabUtil->Append(wxID_ANY, wxT("新着をすべて開く"));
     boardTabUtil->Append(wxID_ANY, wxT("お気に入りの新着をすべて開く"));
     boardTabUtil->Append(wxID_ANY, wxT("新着スレッドをすべて開く"));
     boardTabUtil->AppendSeparator();

     wxMenu* addFav = new wxMenu();
     addFav->Append(ID_AddBoardFavorite, wxT("「お気に入り」に追加"));
     addFav->AppendSeparator();
     addFav->Append(wxID_ANY, wxT("「リンク」に追加"));
     boardTabUtil->AppendSubMenu(addFav, wxT("お気に入りに追加"));
     boardTabUtil->AppendSeparator();

     boardTabUtil->Append(wxID_ANY, wxT("スレッド新規作成"));
     boardTabUtil->AppendSeparator();
     boardTabUtil->Append(ID_OnOpenBoardByBrowser, wxT("ブラウザで開く"));
     boardTabUtil->Append(wxID_ANY, wxT("index表示"));
     boardTabUtil->Append(wxID_ANY, wxT("看板を見る"));
     boardTabUtil->AppendSeparator();

     boardTabUtil->AppendSeparator();
     wxMenu* copy = new wxMenu();
     copy->Append(ID_CopyBURLToClipBoard, wxT("URLをコピー"));
     copy->Append(ID_CopyBTitleToClipBoard, wxT("タイトルをコピー"));
     copy->Append(ID_CopyBBothDataToClipBoard, wxT("タイトルとURLをコピー"));
     boardTabUtil->AppendSubMenu(copy, wxT("コピー"));
     boardTabUtil->AppendSeparator();

     wxMenu* deleteLog = new wxMenu();
     deleteLog->Append(ID_DeleteBSelectedDatFile,      wxT("選択中のログを削除"));
     deleteLog->Append(ID_DeleteBAllDatFile,           wxT("すべてのログを削除"));
     deleteLog->Append(ID_DeleteBAllDatFileWithoutFav, wxT("お気に入り以外のログを削除"));
     boardTabUtil->AppendSubMenu(deleteLog, wxT("ログ削除"));
     boardTabUtil->AppendSeparator();

     boardTabUtil->Append(wxID_ANY, wxT("板移転の追尾"));
     boardTabUtil->AppendSeparator();
     boardTabUtil->Append(wxID_ANY, wxT("板のインデックスを再構築"));
     boardTabUtil->Append(wxID_ANY, wxT("過去ログ非表示"));

     // ポップアップメニューを表示させる
     PopupMenu(boardTabUtil);
}
/**
 * スレッド一覧ノートブックで右クリックされた時の処理
 */
void XrossBoard::OnRightClickThreadNoteBook(wxAuiNotebookEvent& event) {
     wxString selectedThreadName = threadNoteBook->GetPageText(
	  event.GetSelection());

     wxMenu* threadTabUtil = new wxMenu();
     threadTabUtil->Append(ID_OneThreadTabClose, wxT("このタブを閉じる"));
     threadTabUtil->Append(wxID_ANY, wxT("未読として閉じる"));
     threadTabUtil->AppendSeparator();
     threadTabUtil->Append(ID_ExcepSelThreadTabClose, wxT("このタブ以外を閉じる"));
     threadTabUtil->Append(wxID_ANY, wxT("新着なしのタブを閉じる"));
     threadTabUtil->Append(ID_AllThreadTabClose, wxT("すべてのタブを閉じる"));
     threadTabUtil->Append(ID_AllLeftThreadTabClose, wxT("これより左を閉じる"));
     threadTabUtil->Append(ID_AllRightThreadTabClose, wxT("これより右を閉じる"));
     threadTabUtil->AppendSeparator();

     wxMenu *tabLock = new wxMenu;
     tabLock->Append(wxID_ANY, wxT("このタブをロックする"));
     tabLock->Append(wxID_ANY, wxT("すべてのタブをロックする"));
     tabLock->Append(wxID_ANY, wxT("これより左をロックする"));
     tabLock->Append(wxID_ANY, wxT("これより右をロックする"));
     threadTabUtil->AppendSubMenu(tabLock, wxT("タブロック"));
     threadTabUtil->AppendSeparator();

     threadTabUtil->Append(wxID_ANY, wxT("次スレ候補検索"));
     threadTabUtil->Append(wxID_ANY, wxT("次スレ候補を開く"));
     threadTabUtil->Append(wxID_ANY, wxT("ヒストリー検索"));
     threadTabUtil->AppendSeparator();

     threadTabUtil->Append(wxID_ANY, wxT("印を付ける"));

     wxMenu* addFav = new wxMenu();
     addFav->Append(ID_AddThreadFavorite, wxT("「お気に入り」に追加"));
     addFav->AppendSeparator();
     addFav->Append(wxID_ANY, wxT("「リンク」に追加"));
     threadTabUtil->AppendSubMenu(addFav, wxT("お気に入りに追加"));
     threadTabUtil->AppendSeparator();

     wxMenu* addFavAll = new wxMenu();
     addFavAll->Append(ID_AddAllThreadFavorite, wxT("「お気に入り」に追加"));
     addFavAll->AppendSeparator();
     addFavAll->Append(wxID_ANY, wxT("「リンク」に追加"));
     threadTabUtil->AppendSubMenu(addFavAll, wxT("すべてのタブをお気に入りに追加"));
     threadTabUtil->AppendSeparator();

     threadTabUtil->Append(ID_ReloadThisThread, wxT("新着チェック"));
     threadTabUtil->Append(wxID_ANY, wxT("すべてのタブの新着チェック"));
     threadTabUtil->Append(wxID_ANY, wxT("中止"));
     threadTabUtil->Append(ID_CallResponseWindow, wxT("レス"));
     threadTabUtil->AppendSeparator();
     threadTabUtil->Append(ID_OnOpenThreadByBrowser, wxT("ブラウザで開く"));
     threadTabUtil->AppendSeparator();

     wxMenu* copy = new wxMenu();
     copy->Append(ID_CopyTURLToClipBoard, wxT("URLをコピー"));
     copy->Append(ID_CopyTTitleToClipBoard, wxT("タイトルをコピー"));
     copy->Append(ID_CopyTBothDataToClipBoard, wxT("タイトルとURLをコピー"));
     threadTabUtil->AppendSubMenu(copy, wxT("コピー"));
     threadTabUtil->AppendSeparator();

     threadTabUtil->Append(wxID_ANY, wxT("この板を開く"));
     threadTabUtil->AppendSeparator();

     wxMenu *broadcast = new wxMenu;
     broadcast->Append(wxID_ANY, wxT("オートリロード"));
     broadcast->Append(wxID_ANY, wxT("オートスクロール"));
     broadcast->Append(wxID_ANY, wxT("オートリロード・スクロール"));
     threadTabUtil->AppendSubMenu(broadcast, wxT("実況支援"));
     threadTabUtil->AppendSeparator();

     wxMenu *iReadHere = new wxMenu;
     iReadHere->Append(wxID_ANY, wxT("この辺まで読んだ"));
     iReadHere->Append(wxID_ANY, wxT("最後まで読んだ"));
     iReadHere->Append(wxID_ANY, wxT("「ここまで読んだ」にジャンプ"));
     iReadHere->Append(wxID_ANY, wxT("「ここまで読んだ」を解除"));
     threadTabUtil->AppendSubMenu(iReadHere, wxT("ここまで読んだ"));

     wxMenu *chkResponse = new wxMenu;
     chkResponse->Append(wxID_ANY, wxT("レスのチェックをすべて解除"));
     threadTabUtil->AppendSubMenu(chkResponse, wxT("レスのチェック"));
     threadTabUtil->AppendSeparator();

     wxMenu *saveLog = new wxMenu;
     saveLog->Append(ID_SaveDatFile, wxT("datを名前を付けて保存"));
     saveLog->Append(ID_SaveDatFileToClipBoard, wxT("datをクリップボードにコピー"));
     threadTabUtil->AppendSubMenu(saveLog, wxT("このログを保存"));
     threadTabUtil->Append(ID_DeleteDatFile, wxT("このログを削除"));
     threadTabUtil->Append(ID_ReloadThisThread, wxT("再読み込み"));

     // Linuxではファイルごとクリップボードにコピーすることができない
#ifndef __WXMSW__
     saveLog->Enable(ID_SaveDatFileToClipBoard, false);
#endif

     // ポップアップメニューを表示させる
     PopupMenu(threadTabUtil);
}
/**
 * レスアンカーに対応するレスを表示するポップアップウィンドウを出現させる
 */
void XrossBoard::SetPopUpWindow(wxHtmlCellEvent& event, wxString& boardNameAscii, 
			       wxString& origNumber, wxString& resNumber, wxPoint& anchorPoint) {

     // ポップアップさせるHTMLソース
     wxString htmlDOM;

     // '-'が含まれている場合、複数レスをポップアップする
     if (resNumber.Contains(wxT("-"))) {
	  wxStringTokenizer tkz(resNumber, wxT("-"));
	  if (tkz.HasMoreTokens()) {
	       const wxString resNumberStart = tkz.GetNextToken();
	       const wxString resNumberEnd   = tkz.GetNextToken();
	       htmlDOM                       = XrossBoardUtil::FindAnchoredResponse(boardNameAscii, origNumber, resNumberStart, resNumberEnd);
	  }
     } else {
	 htmlDOM = XrossBoardUtil::FindAnchoredResponse(boardNameAscii, origNumber, resNumber);
     }
     
     if (wxEmptyString == htmlDOM) {
	  // 空文字で帰ってきたらリターン
	  return;
     }

     // 取得したレスをポップアップさせる
     AnchoredResponsePopup* popup = new AnchoredResponsePopup(threadNoteBook, anchorPoint, wxSize(640, 300), htmlDOM);
     // FIXME: ログの出力レベル切り替えが必要か
     //XrossBoardUiUtil::SendLoggingHelper(wxString::Format(wxT("anchor point: %d, %d\n"), anchorPoint.x, anchorPoint.y));

     // マウスカーソルの位置に調整する
     wxPoint p = ClientToScreen(wxGetMousePosition());
     // FIXME: ログの出力レベル切り替えが必要か
     //XrossBoardUiUtil::SendLoggingHelper(wxString::Format(wxT("mouse point: %d, %d\n"), p.x, p.y));
     popup->Position(anchorPoint, wxSize(p.x + 50, p.y - 150));
     popup->Popup();

     event.Skip();
}
/**
 * 取得した情報を元に新しいポップアップウィンドウを出現させる
 */
void XrossBoard::SetPopUpWindowForID(wxHtmlCellEvent& event, wxString& boardNameAscii,
				    wxString& origNumber, wxString& extractId, wxPoint& anchorPoint) {

     // ポップアップさせるHTMLソース
     wxString htmlDOM = XrossBoardUtil::FindResponseById(boardNameAscii, origNumber, extractId);

     if (wxEmptyString == htmlDOM) {
	  // 空文字で帰ってきたらリターン
	  return;
     }

     // 取得したレスをポップアップさせる
     AnchoredResponsePopup* popup = new AnchoredResponsePopup(threadNoteBook, anchorPoint, wxSize(640, 300), htmlDOM);
     // マウスカーソルの位置に調整する
     wxPoint p = ClientToScreen(wxGetMousePosition());
     popup->Position(p, popup->GetSize());
     popup->Popup();

     event.Skip();
}
/**
 * 被レス状態を元に新しいポップアップウィンドウを出現させる
 */
void XrossBoard::SetPopUpWindowByIndex(wxHtmlCellEvent& event, wxString& extractIndex, wxPoint& anchorPoint) {

     // スレッド内容ウィンドウの処理
     ThreadContentBar* contentBar = 
	  dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(threadNoteBook->GetSelection()));

     // ポップアップさせるHTMLソース
     wxString htmlDOM = XrossBoardUtil::FindResponseByIndex(contentBar->GetThreadRawHTML(), extractIndex);

     if (wxEmptyString == htmlDOM) {
	  // 空文字で帰ってきたらリターン
	  return;
     }

     // 取得したレスをポップアップさせる
     AnchoredResponsePopup* popup = new AnchoredResponsePopup(threadNoteBook, anchorPoint, wxSize(640, 300), htmlDOM);
     // マウスカーソルの位置に調整する
     wxPoint p = ClientToScreen(wxGetMousePosition());
     popup->Position(p, popup->GetSize());
     popup->Popup();

     event.Skip();
}

/**
 * 現在使用しているフォントの情報を取得する
 */
wxFont XrossBoard::GetCurrentFont() {

     // wxFontのサンプルコードを参照
#ifndef __WXMSW__
     wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT,
		 wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false /* !underlined */,
		 wxEmptyString /* facename */, wxFONTENCODING_UTF8);
#else
     wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT,
		 wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false /* !underlined */,
		 wxEmptyString /* facename */, wxFONTENCODING_CP932);
#endif

     if (font.IsOk()) {
	  return font;
     }
     // フォントが取得できなかった場合
#ifndef __WXMSW__
     return *wxFont::New(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
			 wxFONTWEIGHT_NORMAL, false, wxEmptyString, wxFONTENCODING_UTF8);
#else
     return *wxFont::New(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
			 wxFONTWEIGHT_NORMAL, false, wxEmptyString, wxFONTENCODING_CP932);
#endif
}
/**
 * フォント情報をコンフィグファイルから読み出す
 */
wxFont XrossBoard::ReadFontInfo(const wxString& widgetName) 
{
     wxString nativeFontInfo = wxEmptyString;
     XrossBoardUtil::GetXrossBoardProperties(widgetName, &nativeFontInfo);
     wxFont f;

     if ( nativeFontInfo != wxEmptyString ) 
     {
	  const bool ret = f.SetNativeFontInfo(nativeFontInfo);

	  if (ret)
	  {
	       return f;
	  } else
	  {
	       return wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT(""));
	  }
     }

     return f;
}
/**
 * スレタブ上に存在するスレッドのURLを返す
 */
wxString XrossBoard::GetThreadURL(const wxString title,const wxString boardNameAscii,
				 const wxString origNumber) {
     NameURLHash::iterator it;
     wxString threadURL;

     for (it = retainHash.begin(); it != retainHash.end(); ++it) {
	  wxString key = it->first;
	  const URLvsBoardName value = it->second;

	  if (value.boardNameAscii == boardNameAscii) {
	       threadURL = value.boardURL;
	       break;
	  }
     }

     // ホスト名の後の板名を除く
     int begin = threadURL.Find(boardNameAscii);
     if (begin == wxNOT_FOUND) {
	  return wxEmptyString;
     }
     threadURL = threadURL.Mid(0, begin);
     threadURL += wxT("test/read.cgi/");
     threadURL += boardNameAscii;
     threadURL += wxT("/");
     threadURL += origNumber;
     threadURL += wxT("/");

     return threadURL;
}
/**
 * 板一覧リストタブを変更した時のイベント
 */
void XrossBoard::OnChangeBoardTab(wxAuiNotebookEvent& event) {
     return;
}
/**
 * スレッド一覧タブを変更した時のイベント
 */
void XrossBoard::OnChangeThreadTab(wxAuiNotebookEvent& event) {
     return;
}
/**
 * 板一覧リストタブを変更した後のイベント
 */
void XrossBoard::OnChangedBoardTab(wxAuiNotebookEvent& event) {
     
     // 選択したタブの板名を取得する
     wxString selectedBoardName = boardNoteBook->GetPageText(event.GetSelection());
     // タイトルを設定する
     SetTitle(selectedBoardName + wxT(" - XrossBoard"));

     // 選択したタブのURLをURL画面に設定する
     if (wxEmptyString != boardNoteBook->GetPageText(event.GetSelection())) {
	  // 板一覧タブの場合
	  wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());
	  URLvsBoardName hash = retainHash[boardName];

	  if (hash.boardURL != wxT("test/read.cgi///"))
	       m_url_input->SetValue(hash.boardURL);

     } else {
	  // その他などないのでエラー
	  wxString message = wxT("(ヽ´ん`)…板タブ変更時にエラーあったみたい…\n");
	  SendLogging(message);
	  boardNoteBook->Thaw();

	  return;
     }

#if !wxCHECK_VERSION(2, 9, 0)
     // リストコントロールを引き出してくる
     for (  wxWindowList::const_iterator i = boardNoteBook->GetChildren().begin(); 
	    i != GetChildren().end(); ++i  ) {
	  if ( wxDynamicCast(*i, wxWindow) && (*i)->GetLabel() == selectedBoardName ) {
	       // 板名が一致するwindowクラスを再描画させる
	       VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(*i);
	       vbListCtrl->GetItemCount();
	       break;
	  }	       
     }
#endif

     // タイトルを設定する
     SetTitle(selectedBoardName + wxT(" - XrossBoard"));
     // 最後に選択したノートブックの記録
     //this->userLastAttachedNotebook = BOARD_NOTEBOOK;
}
/**
 * スレッド一覧タブを変更した後のイベント
 */
void XrossBoard::OnChangedThreadTab(wxAuiNotebookEvent& event) {

     // 選択したタブのスレッド名を取得する
     wxString selectedThreadName = threadNoteBook->GetPageText(event.GetSelection());
     // タイトルを設定する
     SetTitle(selectedThreadName + wxT(" - XrossBoard"));

     if (wxEmptyString != threadNoteBook->GetPageText(event.GetSelection())) {
	  // スレタブの場合
	  const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
	  const wxString boardNameAscii = tiHash[title].boardNameAscii;
	  const wxString origNumber = tiHash[title].origNumber;

	  // URL表示部分にスレッドのURLを設定する
	  wxString targetURL = GetThreadURL(title, boardNameAscii, origNumber);

	  if (targetURL != wxT("test/read.cgi///"))
	       m_url_input->SetValue(targetURL);

     } else {
	  // その他などないのでエラー
	  wxString message = wxT("(ヽ´ん`)…スレタブ変更時にエラーあったみたい…\n");
	  SendLogging(message);
	  threadNoteBook->Thaw();

	  return;
     }

     // 最後に選択したノートブックの記録
     //this->userLastAttachedNotebook = THREAD_NOTEBOOK;
}

void XrossBoard::OnClickURLWindowButton(wxCommandEvent& event) {

     const wxString inputURL = m_url_input->GetValue();

     if (inputURL.Contains(wxT("2ch.net"))) {
	  // 2chのURLである可能性が微粒子レベルで存在する？
	  if (inputURL.Contains(wxT("/test/read.cgi/"))) {
	       // スレッドを表すURLの可能性がある場合
	  } else {
	       // 板名を表すURLの可能性がある場合
	  }
     } else {
	  wxLaunchDefaultBrowser(inputURL);
     }
}
/**
 * メニューアイテムが開かれた場合呼ばれるイベント
 */
void XrossBoard::OnMenuOpen(wxMenuEvent& event) {

     wxMenu* menu = event.GetMenu();
     wxString menuTitle = menu->GetTitle();

     if (menuTitle == wxT("お気に入り")) {

	  wxMenuItemList itemList = menu->GetMenuItems();
	  std::vector<std::tuple<wxString, wxString, wxString>> favoriteList;
	  SQLiteAccessor::GetUserFavoriteThreadList(favoriteList);

#if wxCHECK_VERSION(2, 9, 0)
	  wxMenuItemList::compatibility_iterator current_menuitem_node;
#else
	  wxMenuItemList::Node* current_menuitem_node;
#endif
	  wxMenuItem* current_menuitem;
	  if ( itemList.GetLast() ) {
	       // そもそも要素があるかどうかチェック
	       while ( current_menuitem_node = menu->GetMenuItems().GetLast() ) {
		    
#if wxCHECK_VERSION(2, 9, 0)
		    current_menuitem = current_menuitem_node->GetData();
#else
		    current_menuitem = current_menuitem_node->GetData();
#endif
		    if (!current_menuitem->IsSeparator()) {
		    	 // menuの区切りでなければ削除する
			 menu->Delete( current_menuitem );
		    } else {
		    	 // そうでなければ削除は終わりなので脱出
		    	 break;
		    }
	       }

	       // 取得した値をメニューに設定する
	       if ( favoriteList.size() == 0 ) {
		    menu->Append(wxID_ANY, wxT("お気に入りのスレッドがないよ"));
	       } else {
		    for (unsigned int i = 0; i < favoriteList.size(); i++ ) {
			 if ( std::get<1>(favoriteList.at(i)) != wxT("0")) {
			      // スレッドの場合
			      // ex) http://engawa.2ch.net/test/read.cgi/linux/1044149677/
			      wxString helperUrl = wxT("http://hostname/test/read.cgi/") 
				   + std::get<2>(favoriteList.at(i)) 
				   + wxT("/") 
				   + std::get<1>(favoriteList.at(i));

			      menu->Append(ID_UserFavoriteThreadClick + i, std::get<0>(favoriteList.at(i)), helperUrl);
			 } else {
			      // 板の場合
			      // ex) http://engawa.2ch.net/linux/
			      wxString helperUrl = wxT("http://hostname/")
				   + std::get<2>(favoriteList.at(i))
				   + wxT("/");
		
			      menu->Append(ID_UserFavoriteThreadClick + i, std::get<0>(favoriteList.at(i)), helperUrl);
			 }
		    }
	       }
	  }
     }

     event.Skip();
}
/**
 * ユーザーが最近閉じた板タブの情報をSQLiteから取得して設定する
 */
void XrossBoard::UserLastClosedBoardMenuUp(wxUpdateUIEvent& event) {

     // メニューアイテムを順次消していく
#if wxCHECK_VERSION(2, 9, 0)
     wxMenuItemList::compatibility_iterator current_menuitem_node;
#else
     wxMenuItemList::Node* current_menuitem_node;
#endif
     wxMenuItem* current_menuitem;
     if ( closeB->GetMenuItems().GetLast() ) {
	  // そもそも要素があるかどうかチェック
	  while ( current_menuitem_node = closeB->GetMenuItems().GetLast() ) {
#if wxCHECK_VERSION(2, 9, 0)
	       current_menuitem = current_menuitem_node->GetData();
#else
	       current_menuitem = current_menuitem_node->GetData();
#endif
	       if (!current_menuitem->IsSeparator()) {
		    // menuの区切りでなければ削除する
		    closeB->Delete( current_menuitem );
	       } else {
		    // そうでなければ削除は終わりなので脱出
		    break;
	       }
	  }
	  // ユーザが閉じたスレッドのうち、データベースに保存されている数
	  wxArrayString array = SQLiteAccessor::GetClosedBoardInfo();
	  if ( array.GetCount() == 0 ) {
	       closeB->Append(wxID_ANY, wxT("過去に閉じた板がないよ"));
	       return;
	  }

	  for (unsigned int i = 0; i < array.GetCount(); i++ ) {
	       closeB->Append(ID_UserLastClosedBoardClick + i, array[i]);
	  }
     }     
}
/**
 * ユーザーが最後に閉じた板を開く
 */
void XrossBoard::OnUserLastClosedBoardClick(wxCommandEvent& event) 
{
     // メニューアイテムの項目番号を取得する
     wxString boardName = closeB->GetLabelText(event.GetId());
     // 板名に対応したURLを取ってくる
     URLvsBoardName hash = retainHash[boardName];
     wxString boardNameAscii = hash.boardNameAscii;

     // ファイルのパスを設定する
     wxString outputPath = ::wxGetHomeDir() 
	  + wxFILE_SEP_PATH 
	  + XROSSBOARD_DIR
	  + wxFILE_SEP_PATH
	  + wxT("dat")
	  + wxFILE_SEP_PATH
	  + boardNameAscii
	  + wxFILE_SEP_PATH
	  + boardNameAscii
	  + wxT(".dat");

     // 板一覧タブをセットする
     std::map<wxString, ThreadList> stub;
     // 要素を追加する
     wxString stubStr = wxT("NO_NEED_TO_CHK_THREAD_STATE");
     ThreadList thList;
     stub.insert( std::map<wxString, ThreadList>::value_type( stubStr, thList ) );

     // 新規にセットされる板名かどうかのフラグを用意する
     bool itIsNewBoardName = true;
     // 次に選択されるべきタブのページ数を格納する変数
     size_t selectedPage = 0;

     // ユーザーが開いているタブの板名を調べる
     for (unsigned int i = 0; i < boardNoteBook->GetPageCount(); i++) 
     {
	  if (boardName.Cmp(boardNoteBook->GetPageText(i)) == 0) 
	  {
	       itIsNewBoardName = false;
	       selectedPage = i;
	       // もうスレッド一覧を開いていれば開く
	       boardNoteBook->SetSelection(selectedPage);
	       return;
	  }
     }
     // スレッド一覧リストを配置
     SetThreadListItemNew(boardName, outputPath, boardNoteBook->GetPageCount() + 1, stub);
}
/**
 * ユーザーが最近閉じたスレタブの情報をSQLiteから取得して設定する
 */
void XrossBoard::UserLastClosedThreadMenuUp(wxUpdateUIEvent& event) 
{
     // メニューアイテムを順次消していく
#if wxCHECK_VERSION(2, 9, 0)
     wxMenuItemList::compatibility_iterator current_menuitem_node;
#else
     wxMenuItemList::Node* current_menuitem_node;
#endif
     wxMenuItem* current_menuitem;
     if ( closeT->GetMenuItems().GetLast() ) {
	  // そもそも要素があるかどうかチェック
	  while ( current_menuitem_node = closeT->GetMenuItems().GetLast() ) {
#if wxCHECK_VERSION(2, 9, 0)
	       current_menuitem = current_menuitem_node->GetData();
#else
	       current_menuitem = current_menuitem_node->GetData();
#endif
	       if (!current_menuitem->IsSeparator()) {
		    // menuの区切りでなければ削除する
		    closeT->Delete( current_menuitem );
	       } else {
		    // そうでなければ削除は終わりなので脱出
		    break;
	       }
	  }
	  // ユーザが閉じたスレッドのうち、データベースに保存されている数
	  wxArrayString array = SQLiteAccessor::GetThreadInfo();
	  if ( array.GetCount() == 0 ) {
	       closeT->Append(wxID_ANY, wxT("過去に閉じたスレッドがないよ"));
	       return;
	  }

	  for (unsigned int i = 0; i < array.GetCount(); i++ ) {
	       closeT->Append(ID_UserLastClosedThreadClick + i, array[i]);
	  }
     }
}
/**
 * ユーザーが最後に閉じたスレッドを開く
 */
void XrossBoard::OnUserLastClosedThreadClick(wxCommandEvent& event) {

     // メニューアイテムの項目番号を取得する
     const int number = event.GetId() - ID_UserLastClosedThreadClick;
     std::unique_ptr<ThreadInfo> threadInfo(new ThreadInfo());

     SQLiteAccessor::GetThreadFullInfo(number, threadInfo, event.GetId());

     if (!threadInfo) {
	  // 無ければ警告を出して終了
	  wxMessageBox(wxT("前回読み込んでいたdatファイルの読み出しに失敗しました\n\
                            datファイルを削除しているか、datファイルの保存先を変更していませんか？"), wxT("読み込んでいるスレッド"), wxICON_ERROR);
	  return;
     }

     // ファイルパスの組み立てとファイルの有無確認
     wxString threadContentPath = XrossBoardUtil::AssembleFilePath(threadInfo->boardNameAscii, threadInfo->origNumber, KIND_THREAD_DAT);

     // ファイルの有無確認
     if (!wxFile::Exists(threadContentPath)) {
	  // 無ければ警告を出して次へ
	  wxMessageBox(wxT("前回読み込んでいたdatファイルの読み出しに失敗しました\n\
                            datファイルを削除しているか、datファイルの保存先を変更していませんか？"), wxT("読み込んでいるスレッド"), wxICON_ERROR);
	  return;
     }

     // スレッドの内容をノートブックに反映する
     SetThreadContentToNoteBook(threadContentPath, threadInfo->origNumber, threadInfo->title);
     // ノートブックに登録されたスレッド情報をハッシュに登録する
     ThreadInfo info;
     info.origNumber = threadInfo->origNumber;
     info.boardNameAscii = threadInfo->boardNameAscii;
     tiHash[threadInfo->title] = info;
}
/**
 * ユーザーがお気に入り登録したスレッド or 板を開く
 */
void XrossBoard::OnUserFavoriteThreadClick(wxCommandEvent& event) {

     // メニューアイテムの項目番号を取得する
     const int number = event.GetId() - ID_UserFavoriteThreadClick;
     std::unique_ptr<ThreadInfo> threadInfo(new ThreadInfo());
     SQLiteAccessor::GetThreadFullInfo(number, threadInfo, event.GetId());

     if ( threadInfo->origNumber == wxT("0") ) {
	  // 板情報なので板を表示する
	  URLvsBoardName hash = retainHash[threadInfo->title];
	  wxString boardNameAscii = hash.boardNameAscii;
	  wxString boardURL = hash.boardURL;

	  SetBoardNameToNoteBook(threadInfo->title, boardURL, boardNameAscii);
	  return;
     }

     if (!threadInfo) {
	  // 無ければ警告を出して終了
	  wxMessageBox(wxT("お気に入り登録されたdatファイルの読み出しに失敗しました\n\
                            datファイルを削除しているか、datファイルの保存先を変更していませんか？"), 
		       wxT("読み込んでいるスレッド"), wxICON_ERROR);
	  return;
     }

     // ファイルパスの組み立てとファイルの有無確認
     wxString threadContentPath = XrossBoardUtil::AssembleFilePath(threadInfo->boardNameAscii, threadInfo->origNumber, KIND_THREAD_DAT);

     // ファイルの有無確認
     if (!wxFile::Exists(threadContentPath)) {
	  // 無ければ警告を出して次へ
	  wxMessageBox(wxT("お気に入り登録されたdatファイルの読み出しに失敗しました\n\
                            datファイルを削除しているか、datファイルの保存先を変更していませんか？"), 
		       wxT("読み込んでいるスレッド"), wxICON_ERROR);
	  return;
     }

     // スレッドの内容をノートブックに反映する
     SetThreadContentToNoteBook(threadContentPath, threadInfo->origNumber, threadInfo->title);
     // ノートブックに登録されたスレッド情報をハッシュに登録する
     ThreadInfo info;
     info.origNumber = threadInfo->origNumber;
     info.boardNameAscii = threadInfo->boardNameAscii;
     tiHash[threadInfo->title] = info;
}
/**
 * ユーザーが現在開いているスレタブ、板タブの一覧を作成する
 */
void XrossBoard::UserLookingTabsMenuUp(wxUpdateUIEvent& event) {
     
     // メニューアイテムを順次消していく
     wxMenuItemList::compatibility_iterator current_menuitem_node;
     wxMenuItem* current_menuitem;

     while ( current_menuitem_node = lookingTB->GetMenuItems().GetLast() ) {
	  current_menuitem = current_menuitem_node->GetData();
	  if (!current_menuitem->IsSeparator()) {
	       // menuの区切りでなければ削除する
	       lookingTB->Delete( current_menuitem );
	  } else {
	       // そうでなければ削除は終わりなので脱出
	       break;
	  }
     }

     // menu8に現在ユーザーが開いているタブの名前を追加する
     wxArrayString array;

     for (unsigned int i = 0; i < boardNoteBook->GetPageCount(); i++) {
	  array.Add(boardNoteBook->GetPageText(i));
     }
     for (unsigned int i = 0; i < threadNoteBook->GetPageCount(); i++) {
	  array.Add(threadNoteBook->GetPageText(i));
     }
     for (unsigned int i = 0; i < array.GetCount(); i++ ) {
	  lookingTB->Append(ID_UserLookingTabsMenuClick, array[i]);
     }
}

/**
 * 閲覧中ツリーのデータ更新を行う
 */
void XrossBoard::NowReadingTreectrlUpdate(wxUpdateUIEvent& event) {

     if (m_now_reading_tree_ctrl) {
	  // リセット
	  m_now_reading_tree_ctrl
	       ->CollapseAndReset(m_now_reading_tree_ctrl->GetRootItem());
     }

     // 閲覧中データを集める
     ThreadInfoHash::iterator it;
     wxTreeItemId category;
     // ツリーに追加したカテゴリ名
     std::vector<wxString> added;

     for (it = tiHash.begin(); it != tiHash.end(); ++it) {
	  wxString key = it->first;
	  ThreadInfo value = it->second;

	  wxString title          = value.title;
	  wxString origNumber     = value.origNumber;
	  wxString boardNameAscii = value.boardNameAscii;

	  // Hashから板名を探す
	  NameURLHash::iterator itr;
	  for (itr = retainHash.begin(); itr != retainHash.end(); ++itr) {
	       wxString k = itr->first;
	       const URLvsBoardName v = itr->second;

	       // カテゴリ名検索用
	       std::vector<wxString>::iterator memory = added.begin();
	       memory = std::find_if(added.begin(), 
				     added.end(), 
				     [&v] (const wxString& item) -> bool {
					  return item == v.boardName;
				     });

	       if (v.boardNameAscii == boardNameAscii) {

		    if (memory == added.end()) {
			 // カテゴリ名を覚えておく
			 added.push_back(v.boardName);

			 // カテゴリ名を登録する
			 category = m_now_reading_tree_ctrl
			      ->AppendItem(m_now_reading_tree_ctrl->GetRootItem(), v.boardName);
			 m_now_reading_tree_ctrl->SetItemImage(category, 0, wxTreeItemIcon_Normal);
		    }
		    
		    // 閲覧中のスレッドタイトルを登録する
		    wxTreeItemId tmp = m_tree_ctrl->AppendItem(category, key);
		    m_now_reading_tree_ctrl->SetItemImage(tmp, 1, wxTreeItemIcon_Normal);

		    continue;
	       }
	  }
     }

     // 展開
     m_now_reading_tree_ctrl->Expand(m_now_reading_tree_ctrl->GetRootItem());
}

/**
 * ユーザーが現在フォーカスしているウィンドウの操作を行う
 */
void XrossBoard::UserLookingTabsControl(wxUpdateUIEvent& event) {
     event.Skip();
}
/**
 * フォーカスを変更した際に反応するイベント
 */
void XrossBoard::OnSetFocus(wxFocusEvent& event) {
     event.Skip();
}
/**
 * スレッド一覧画面にツールバーを設定する
 */
wxPanel* XrossBoard::CreateAuiToolBar(wxWindow* parent, const wxString& boardName, const wxString& outputPath,
				     const std::map<wxString,ThreadList>& oldThreadMap) {

     // スレッド検索ボックスとスレッド一覧リストを格納するサイザーを宣言する
     wxPanel* panel = new wxPanel(parent, -1);
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
     // 検索ツールバーをpanelとsizerに載せる
     CreateCommonAuiToolBar(panel, vbox, ID_ThreadSearchBar, boardName);

     // Hashに格納する板名タブのオブジェクトのインスタンスを準備する
     VirtualBoardListCtrl* vbListCtrl = new VirtualBoardListCtrl(
	  (wxWindow*) panel,
	  ID_BoardListCtrl,
	  (const wxString) boardName,
	  (const wxString) outputPath,
	  oldThreadMap);
     vbListCtrl->SetName(boardName);
     vbox->Add(vbListCtrl, 1, wxEXPAND, 0);

     // カラムの幅を最大化
     const wxFont font = GetCurrentFont();
     int pointSize = font.GetPointSize();
     // 2chのスレタイの文字数制限は全角24文字
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_CHK      , 20);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_TITLE    , pointSize * 52);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_SINCE    , pointSize * 12);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_OID      , pointSize * 10);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_BOARDNAME, pointSize * 12);

     // パネルにSizerを設定する
     panel->SetSizer(vbox);

     return panel;
}
/**
 * 検索用ツールバー設定の共通部分
 */
void XrossBoard::CreateCommonAuiToolBar(wxPanel* panel, wxBoxSizer* vbox, wxWindowID id, const wxString& boardName) {

     // wxAuiToolBarを宣言する
     wxAuiToolBar* searchBox = new wxAuiToolBar(panel, 
						id,
						wxDefaultPosition, 
						wxDefaultSize, 
						wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
     int targetId = 0;
     
     switch (id) {
     case ID_ThreadSearchBar:
	  targetId = ID_ThreadSearchBoxDoSearch;
	  break;
     case ID_BoardSearchBar:
	  targetId = ID_BoardSearchBoxDoSearch;
	  break;
     case ID_FavsSearchBar:
	  targetId = ID_FavsSearchBoxDoSeach;
	  break;
     case ID_NowReadingSearchBar:
	  targetId = ID_NowReadingBoxDoSeach;
	  break;
     }

     // 検索ボックスのID設定
     searchBox->SetToolBitmapSize(wxSize(32,32));
     searchBox->AddTool(targetId, SEARCH_BOX, wxBitmap(redResExtractImg, wxBITMAP_TYPE_ANY), wxT("検索"));

     // メニューの設定
     wxAuiToolBarItemArray prepend_items1;
     wxAuiToolBarItemArray append_items1;
     wxAuiToolBarItem item;
     // 後からどのウィンドウか判別するために、itemとwindowを関連付ける
     item.SetLabel(wxT("search_box_item"));
     item.SetWindow(searchBox);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxUp);
     item.SetLabel(wxT("↑検索"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxDown);
     item.SetLabel(wxT("↓検索"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxCopy);
     item.SetLabel(wxT("コピー"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxCut);
     item.SetLabel(wxT("切り取り"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(wxID_ANY);
     item.SetLabel(wxT("貼り付け"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxSelectAll);
     item.SetLabel(wxT("全て選択"));
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBoxClear);
     item.SetLabel(wxT("クリア"));
     append_items1.Add(item);

     item.SetKind(wxITEM_SEPARATOR);
     append_items1.Add(item);

     item.SetKind(wxITEM_NORMAL);
     item.SetId(ID_SearchBarHide);
     item.SetLabel(wxT("閉じる"));
     append_items1.Add(item);

     searchBox->SetCustomOverflowItems(prepend_items1, append_items1);
     searchBox->AddTool(ID_SearchBoxRegexSearch, 
			wxT("正規表現"), 
			wxBitmap(regexImg, wxBITMAP_TYPE_ANY), 
			wxT("正規表現を使います"), 
			wxITEM_CHECK);

     /**
      * 1. ラベルの設定
      * 2. 検索ボックスの設定
      *
      */

     int TARGET_COMBO = 0;
     wxArrayString choices;
     wxString label = wxEmptyString;

     switch (id) {

     case ID_ThreadSearchBar:
	  label = THREADLIST_SEARCH;
	  label += wxT("_");
	  label += boardName;
	  searchBox->SetLabel(label);
	  TARGET_COMBO = ID_ThreadSearchBarCombo;	  
	  break;
     case ID_BoardSearchBar:
	  searchBox->SetLabel(BOARD_TREE_SEARCH);
	  TARGET_COMBO = ID_BoardSearchBarCombo;
	  break;
     case ID_FavsSearchBar:
	  searchBox->SetLabel(FAVS_TREE_SEARCH);
	  TARGET_COMBO = ID_FavsSearchBarCombo;
	  break;
     case ID_NowReadingSearchBar:
	  searchBox->SetLabel(NOW_READ_TREE_SEARCH);
	  TARGET_COMBO = ID_NowReadingSearchBarCombo;
	  break;
	  
#ifdef USE_SHINGETSU /** USE_SHINGETSU */
     case ID_ShingetsuBoardSearchBar:
	  searchBox->SetLabel(SHINGETU_NODE_SEARCH);
	  TARGET_COMBO = NULL; // FIXME
	  break;
#endif /** USE_SHINGETSU */
     }

     // 検索ボックスを設定する
     wxComboBox* searchWordCombo = new wxComboBox(searchBox,
						  TARGET_COMBO,
						  wxEmptyString, 
						  wxDefaultPosition, 
						  wxDefaultSize,  
						  choices, 
						  wxCB_DROPDOWN);

     /**
      * wxComboBoxのインスタンス化後のラベルの設定
      */

     // 前回検索語の補填
     SupplySearchWords(searchWordCombo, id);

     switch (id) {

     case ID_ThreadSearchBar:
	  searchWordCombo->SetLabel(boardName + wxT("_combo"));
	  searchBox->AddControl(searchWordCombo, boardName + wxT("_combo"));
	  
	  break;
     case ID_BoardSearchBar:
	  searchBox->AddControl(searchWordCombo, wxT("board_tree_combo"));
	  
	  break;
     case ID_FavsSearchBar:
	  searchBox->AddControl(searchWordCombo, wxT("favs_search_combo"));
	  break;
     case ID_NowReadingSearchBar:
	  searchBox->AddControl(searchWordCombo, wxT("now_reading_combo"));
	  break;
	  
#ifdef USE_SHINGETSU /** USE_SHINGETSU */
     case ID_ShingetsuBoardSearchBar:
	  searchBox->AddControl(searchWordCombo, wxT("shingetsu_tree_combo"));
	  break;
#endif /** USE_SHINGETSU */
     }

     // 検索ボックスは空にする
     searchWordCombo->SetValue(wxEmptyString);

     // 閉じるボタンを設定する
     searchBox->AddTool(ID_SearchBarHide, 
			wxEmptyString, 
			wxBitmap(closeImg, wxBITMAP_TYPE_ANY), 
			wxT("検索ボックスを隠す"));

     searchBox->Realize();
     vbox->Add(searchBox, 0, wxEXPAND, 0);
}
/**
 * 以前検索したキーワードをコンボボックスに補填する
 */
void XrossBoard::SupplySearchWords(wxComboBox* combo, const wxWindowID id) {

}

/** 
 * 検索実行(スレッド一覧)
 */
void XrossBoard::ThreadSearchBoxDoSearch(wxCommandEvent& event) {

     wxWindow* target = boardNoteBook->GetPage(boardNoteBook->GetSelection());
     if ( wxComboBox* combo = 
	  dynamic_cast<wxComboBox*>(wxWindow::FindWindowById(ID_ThreadSearchBarCombo, target))) {
	  const wxString keyword = combo->GetValue();

	  if (keyword.IsEmpty()) {
	       return;
	  } else {
	       SearchThreadList(keyword);
	       // コンボボックスに検索したキーワードをつめる
	       combo->Append(keyword);
	       SQLiteAccessor::SetUserSearchedKeyword(keyword,ID_ThreadSearchBarCombo);
	  }
     }
}
/** 
 * 検索実行(板一覧)
 */
void XrossBoard::BoardSearchBoxDoSearch(wxCommandEvent& event) {

     wxWindow* target = boardTreeNoteBook->GetPage(boardTreeNoteBook->GetSelection());
     if ( wxComboBox* combo = 
	  dynamic_cast<wxComboBox*>(wxWindow::FindWindowById(ID_BoardSearchBarCombo, target))) {
	  const wxString keyword = combo->GetValue();

	  if (keyword.IsEmpty()) { 
	       return;
	  } else {
	       SearchBoardTree(keyword);
	       // コンボボックスに検索したキーワードをつめる
	       combo->Append(keyword);
	       SQLiteAccessor::SetUserSearchedKeyword(keyword,ID_BoardSearchBarCombo);
	  }	  
     }     
}
/**
 * 板一覧ツリーを検索する
 */
void XrossBoard::SearchBoardTree(const wxString& keyword) {

     // ArrayStringの形で板一覧情報を取得する
     wxArrayString boardInfoArray = SQLiteAccessor::GetBoardInfo();
     // カテゴリ名一時格納用
     wxString categoryName;
     // 板名一時格納用
     wxString boardName;
     // URL一時格納用
     wxString url;
     // カテゴリ名を保持するためのID
     wxTreeItemId category;

     // 一度中身を削除する
     m_tree_ctrl->CollapseAndReset(m_tree_ctrl->GetRootItem());

     // 板一覧情報をツリーに渡す
     for (unsigned int i = 0; i < boardInfoArray.GetCount(); i += 3) {
	  // 板名がキーワードと一致する場合のみカテゴリをツリーに登録
	  if (categoryName != boardInfoArray[i + 2] && boardInfoArray[i].Contains(keyword)) {
	       category = m_tree_ctrl->AppendItem(m_tree_ctrl->GetRootItem(), boardInfoArray[i + 2]);
	       m_tree_ctrl->SetItemImage(category, 0, wxTreeItemIcon_Normal);
	  } else {
	       continue;
	  }
	  // それぞれの要素を一時格納
	  boardName = boardInfoArray[i];
	  url = boardInfoArray[i + 1];
	  categoryName = boardInfoArray[i + 2];

	  wxTreeItemId tmp = m_tree_ctrl->AppendItem(category, boardName);
	  m_tree_ctrl->SetItemImage(tmp, 1, wxTreeItemIcon_Normal);

#if !wxCHECK_VERSION(2, 9, 0)
	  m_tree_ctrl->Expand(category);
#endif
	  
     }
#if !wxCHECK_VERSION(2, 9, 0)
     m_tree_ctrl->Expand(m_tree_ctrl->GetRootItem());
#endif
}
/** 
 * スレッド一覧リストを検索する
 */
void XrossBoard::SearchThreadList(const wxString& keyword) {

     // 現在アクティブになっているタブの板名を取得する
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // リストコントロールを引き出してくる
     if ( VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName)) ) {
	  // スレッドタイトル検索を実施する
	  vbListCtrl->SearchAndSortItems(keyword);
     } else {
	  wxMessageBox(wxT("内部エラー, スレッド検索処理に失敗しました."), wxT("スレッド一覧リスト"), wxICON_ERROR);
	  return;
     }
}
/**
 * スレッド検索ボックスを隠す
 */
void XrossBoard::HideSearchBar(wxCommandEvent& event) {

     wxWindow* window = dynamic_cast<wxWindow*>(event.GetEventObject());
     if (window != NULL && (window->GetLabel() == BOARD_TREE_SEARCH || window->GetLabel() == THREADLIST_SEARCH)) {
	  window->Hide();
	  window->GetParent()->Layout();
     }
}
/** 
 * 検索ボックスで上に移動
 */
void XrossBoard::SearchBoxUp(wxCommandEvent& event) {
}
/** 
 * 検索ボックスで下に移動
 */
void XrossBoard::SearchBoxDown(wxCommandEvent& event) {
}
/** 
 * 検索ボックスでコピー実行
 */
void XrossBoard::SearchBoxCopy(wxCommandEvent& event) {

     wxWindow* window = dynamic_cast<wxWindow*>(event.GetEventObject());
     if (window == NULL) return;
     // 取得対象の検索ボックスを決める
     wxComboBox* combo = FindUserAttachedCombo(event, window);
     if (combo == NULL) return;
     // クリップボードに文字列をコピーする
     combo->Copy();
}
/** 
 * 検索ボックスで切り取り実行
 */
void XrossBoard::SearchBoxCut(wxCommandEvent& event) {

     wxWindow* window = dynamic_cast<wxWindow*>(event.GetEventObject());
     if (window == NULL) return;
     // 取得対象の検索ボックスを決める
     wxComboBox* combo = FindUserAttachedCombo(event, window);
     if (combo == NULL) return;
     // コンボボックスから文字列を削除
     combo->Cut();
}
/**
 * 検索ボックスで全て選択
 */
void XrossBoard::SearchBoxSelectAll(wxCommandEvent& event) {

     wxWindow* window = dynamic_cast<wxWindow*>(event.GetEventObject());
     if (window == NULL) return;
     // 取得対象の検索ボックスを決める
     wxComboBox* combo = FindUserAttachedCombo(event, window);
     if (combo == NULL) return;
     // コンボボックスの文字列を選択
     combo->SetSelection(0, combo->GetLastPosition() + 1);
}
/** 
 * 検索ボックスをクリア
 */
void XrossBoard::SearchBoxClear(wxCommandEvent& event) {

     wxWindow* window = dynamic_cast<wxWindow*>(event.GetEventObject());
     if (window == NULL) return;
     // 取得対象の検索ボックスを決める
     wxComboBox* combo = FindUserAttachedCombo(event, window);
     if (combo == NULL) return;
     // コンボボックスの文字列をクリア
     combo->SetValue(wxEmptyString);

     if (window->GetLabel() == BOARD_TREE_SEARCH) {
	  // 板一覧ツリーも初期状態にもどす
	  SetBoardList(false);
     }
}
/**
 * アクティブなスレッド一覧リストをソートする
 */
void XrossBoard::OnThreadListSort(wxCommandEvent& event) {

     // 現在アクティブになっているタブの板名を取得する
     wxString boardName = boardNoteBook->GetPageText(boardNoteBook->GetSelection());

     // リストコントロールを引き出してくる
     VirtualBoardListCtrl* vbListCtrl = dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardName));
     if (vbListCtrl == NULL) {
	  wxMessageBox(wxT("内部エラー, スレッドソート処理に失敗しました."), wxT("スレッド一覧リスト"), wxICON_ERROR);
	  return;
     }

     switch(event.GetId()) {

     case ID_OnClickMenuCOL_CHK:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_CHK);
	  break;
     case ID_OnClickMenuCOL_NUM:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_NUM);
	  break;
     case ID_OnClickMenuCOL_TITLE:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_TITLE);
	  break;
     case ID_OnClickMenuCOL_RESP:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_RESP);
	  break;
     case ID_OnClickMenuCOL_CACHEDRES:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_CACHEDRES);
	  break;
     case ID_OnClickMenuCOL_NEWRESP:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_NEWRESP);
	  break;
     case ID_OnClickMenuCOL_INCRESP:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_INCRESP);
	  break;
     case ID_OnClickMenuCOL_MOMENTUM:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_MOMENTUM);
	  break;
     case ID_OnClickMenuCOL_LASTUP:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_LASTUP);
	  break;
     case ID_OnClickMenuCOL_SINCE:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_SINCE);
	  break;
     case ID_OnClickMenuCOL_OID:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_OID);
	  break;
     case ID_OnClickMenuCOL_BOARDNAME:
	  vbListCtrl->SortVectorItems(VirtualBoardListCtrl::Columns::COL_BOARDNAME);
	  break;
     default:
	  break;
     }
}
/**
 * ツリーコントロールの表示・非表示切り替え
 */
void XrossBoard::ShowBoardListTree(wxCommandEvent& event) {
     UpdatePanes();
}

/**
 * ペイン位置更新
 */
void XrossBoard::UpdatePanes(bool immediate)
{
     wxAuiPaneInfo& boardTree		       = m_mgr.GetPane(boardTreeNoteBook);
     wxAuiPaneInfo& logWindow		       = m_mgr.GetPane(m_logCtrl);
     wxAuiPaneInfo& boardListThreadListInfo    = m_mgr.GetPane(boardNoteBook);
     wxAuiPaneInfo& threadTabThreadContentInfo = m_mgr.GetPane(threadNoteBook);

     const bool flag = m_floatToolBar->GetToolToggled(ID_ShowBoardListTree);
     boardTree.Position(0).Show(flag);
     logWindow.Position(1).Show(flag);

     m_floatToolBar->EnableTool(ID_SwitchSeparateXY, paneIsThree);
     m_floatToolBar->EnableTool(ID_SwitchRightPane, !paneIsThree);

     if (paneIsThree)
     {
	  if (separateIsX)
	  {
	       boardListThreadListInfo.Center().Position(0).Show();
	       threadTabThreadContentInfo.Center().Position(1).Show();
	  } else
	  {
	       boardListThreadListInfo.Left().Position(0).Show();
	       threadTabThreadContentInfo.Center().Position(0).Show();
	  }
     } else
     {
	  boardListThreadListInfo.Center().Position(0).Show(!rightIsThreadList);
	  threadTabThreadContentInfo.Center().Position(0).Show(rightIsThreadList);
     }

     // 画像の切り替え
     m_floatToolBar->SetToolBitmap (ID_SwitchSeparateXY,
				    wxBitmap(separateIsX ? thrPaneWinImg : thrColumnWinImg,
					     wxBITMAP_TYPE_ANY));

     if (immediate)
	  m_mgr.Update();
}

/**
 * 縦⇔横分割切り替え
 */
void XrossBoard::SwitchSeparateXY(wxCommandEvent& event) 
{
     separateIsX = !separateIsX;
     UpdatePanes();
}

/**
 * ２⇔３ペイン切り替え
 */
void XrossBoard::SwitchTwoThreePane(wxCommandEvent& event)
{
     paneIsThree = !paneIsThree;
     UpdatePanes();
}

/**
 * 右側切り替え
 */
void XrossBoard::SwitchRightPane(wxCommandEvent& event)
{
     rightIsThreadList = !rightIsThreadList;
     UpdatePanes();
}

/**
 * 設定画面を呼び出す
 */
void XrossBoard::CallSettingWindow(wxCommandEvent& event) {

     SettingDialog dialog(this, wxID_ANY, wxT("設定 - "));
     dialog.ShowModal();
}

/**
 * 設定画面を呼び出す
 */
void XrossBoard::CallViewerSettingWindow(wxCommandEvent& event) {

     ViewerSettingDialog dialog(this, wxID_ANY, wxEmptyString);
     dialog.ShowModal();
}

/**
 * クリップボードに指定した文字列をコピーする
 */
void XrossBoard::SetStringToClipBoard(const wxString copyString) 
{
     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(copyString));
	  wxTheClipboard->Close();
     }
}

#ifdef USE_SHINGETSU
/**
 * 新月公開ノードをクリックして、それをノートブックに反映するメソッド
 */
void XrossBoard::SetShingetsuNodeToNoteBook(const wxString& nodeHostname) {

     // CSVファイルの出力先を渡す
     wxString outputFilePath;

     // 通信用クラスの呼び出し
     std::unique_ptr<SocketCommunication> sock(new SocketCommunication());
     bool success = sock->DownloadShingetsuThreadList(nodeHostname, outputFilePath);
     

     if (success) {
	  SetShingetsuThreadListToNoteBook(nodeHostname, outputFilePath);
     } else {
	  wxMessageBox(wxT("内部エラー, 新月公開ノードからスレッドをダウンロードする処理に失敗しました."), 
			 wxT("スレッド一覧リスト"), wxICON_ERROR);
     }
}
/**
 * 新月公開ノード上のスレッド一覧をUIに反映するメソッド
 */
void XrossBoard::SetShingetsuThreadListToNoteBook(const wxString& nodeHostname, wxString& outputFilePath) {

     // 新規にセットされる板名かどうかのフラグを用意する
     bool itIsNewBoardName = true;
     // 次に選択されるべきタブのページ数を格納する変数
     size_t selectedPage = 0;

     // ユーザーが開いているタブの板名を調べる
     for (unsigned int i = 0; i < boardNoteBook->GetPageCount(); i++) {
	  if (nodeHostname.Cmp(boardNoteBook->GetPageText(i)) == 0) {
	       itIsNewBoardName = false;
	       selectedPage = i;
	       break;
	  }
     }

     if (itIsNewBoardName) {
	  // もし新規のダウンロードだった場合、選択されるべきページを指定
	  selectedPage = boardNoteBook->GetPageCount();
	  SetShingetsuThreadListItemNew(nodeHostname, 
					selectedPage, 
					outputFilePath, 
					std::map<wxString,ThreadList>());
     } else {
	  // 更新処理の場合、選択されるべきページはi
	  SetShingetsuThreadListItemUpdate(nodeHostname, 
					   selectedPage, 
					   outputFilePath, 
					   std::map<wxString,ThreadList>());
     }
}
/**
 * 新月のスレッド一覧を新たに取得する
 */
void XrossBoard::SetShingetsuThreadListItemNew(const wxString& nodeHostname, const size_t selectedPage
					      ,wxString& outputFilePath
					      ,const std::map<wxString,ThreadList>& oldThreadMap) {

     // スレッド検索ボックスとスレッド一覧リストを格納するサイザーを宣言する
     wxPanel* panel = new wxPanel(boardNoteBook, -1);
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
     // 検索ツールバーをpanelとsizerに載せる
     //CreateCommonAuiToolBar(panel, vbox, ID_ThreadSearchBar, boardName);

     // Hashに格納する板名タブのオブジェクトのインスタンスを準備する
     VirtualBoardListCtrl* vbListCtrl = new VirtualBoardListCtrl(
	  (wxWindow*) panel,
	  ID_ShingetsuBoardListCtrl,
	  nodeHostname,
	  (const wxString) outputFilePath,
	  std::map<wxString, ThreadList>(),
	  true);
     vbListCtrl->SetName(nodeHostname);
     vbox->Add(vbListCtrl, 1, wxEXPAND, 0);

     // カラムの幅を最大化
     wxFont font = GetCurrentFont();
     int pointSize = font.GetPointSize();
     // 2chのスレタイの文字数制限は全角24文字
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_CHK      , 20);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_TITLE    , pointSize * 52);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_SINCE    , pointSize * 12);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_OID      , pointSize * 10);
     vbListCtrl->SetColumnWidth(VirtualBoardListCtrl::Columns::COL_BOARDNAME, pointSize * 12);

     // パネルにSizerを設定する
     panel->SetSizer(vbox);

     // wxAuiToolBarを宣言する
     // スレッドリストを表示させる
     boardNoteBook->AddPage(panel, nodeHostname, false);
     // ノートブックの選択処理
     boardNoteBook->SetSelection(boardNoteBook->GetPageCount());
}
/**
 * 新月のスレッド一覧を更新する
 */
void XrossBoard::SetShingetsuThreadListItemUpdate(const wxString& nodeHostname, const size_t selectedPage
						 ,wxString& outputFilePath
						 ,const std::map<wxString,ThreadList>& oldThreadMap) {

     // wxAuiToolBarを宣言する
     wxPanel* panel;// = CreateAuiToolBar(boardNoteBook, nodeHostname, outputPath, oldThreadMap);     
     boardNoteBook->DeletePage(selectedPage);
     boardNoteBook->InsertPage(selectedPage, panel, nodeHostname, false, wxNullBitmap);
     // ノートブックの選択処理
     boardNoteBook->SetSelection(selectedPage);
}

#endif /** USE_SHINGETSU */

/**
 * ショートカットキー(Ctrl+F)のイベント
 */
void XrossBoard::CtrlF(wxKeyEvent& event) 
{
     wxWindow* searchBar = nullptr;
     
     if (this->userLastAttachedNotebook == BOARD_NOTEBOOK) 
     {
	  // スレッド一覧ウィンドウの処理
	  wxWindow* target = boardNoteBook->GetPage(boardNoteBook->GetSelection());
	  searchBar = wxWindow::FindWindowById(ID_ThreadSearchBar, target);
     } 
     else if (this->userLastAttachedNotebook == THREAD_NOTEBOOK) 
     {
	  // スレッド内容ウィンドウの処理
	  ThreadContentBar* contentBar = 
	       dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(threadNoteBook->GetSelection()));
	  searchBar = wxWindow::FindWindowById(ID_ThreadContentSearchBar, contentBar);
     } 
     else if (this->userLastAttachedNotebook == BOARD_TREE_NOTEBOOK) 
     {
	  // 板一覧ウィンドウの処理
	  wxWindow* target = boardTreeNoteBook->GetPage(boardTreeNoteBook->GetSelection());
	  searchBar = wxWindow::FindWindowById(ID_BoardSearchBar, target);
     }

     if (searchBar)
     {
	  if (searchBar->IsShown()) 
	  {
	       searchBar->GetNextSibling()->SetFocus();
	       searchBar->Hide();
	  } 
	  else 
	  {
	       searchBar->Show();
	  }
	  searchBar->GetParent()->Layout();
     }
}

/**
 * ショートカットキー(Enter)のイベント
 * @param  wxKeyEvent& event
 * @return 検索実行した:true, 検索実行しなかった:false
 */
bool XrossBoard::Enter(wxKeyEvent& event) 
{
     if (this->userLastAttachedNotebook == BOARD_NOTEBOOK) 
     {
	  // スレッド一覧ウィンドウの処理
	  wxWindow* target = boardNoteBook->GetPage(boardNoteBook->GetSelection());

	  if ( wxAuiToolBar* toolBar 
	       = dynamic_cast<wxAuiToolBar*>(wxWindow::FindWindowById(ID_ThreadSearchBar, target))) 
	  {
	       if (toolBar->IsShown()) 
	       {
		    // 検索実行
		    wxCommandEvent e(wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_ThreadSearchBoxDoSearch));
		    ThreadSearchBoxDoSearch(e);
		    return true;
	       } 
	  }
     } 
     else if (this->userLastAttachedNotebook == THREAD_NOTEBOOK) 
     {
	  // スレッド内容ウィンドウの処理
	  ThreadContentBar* contentBar = 
	       dynamic_cast<ThreadContentBar*>(threadNoteBook->GetPage(threadNoteBook->GetSelection()));
	  
	  if ( wxPanel* searchBarPanel 
	       = dynamic_cast<wxPanel*>(wxWindow::FindWindowById(ID_ThreadContentSearchBar, contentBar))) 
	  {
	       // スレッド内容バーの子ウィンドウを取り出して命令する
	       if (searchBarPanel->IsShown()) 
	       {
		    // 検索実行
		    // TODO: スレッド内容の検索イベント実装
	       } 
	  }
     } 
     else if (this->userLastAttachedNotebook == BOARD_TREE_NOTEBOOK) 
     {
	  // 板一覧ウィンドウの処理
	  wxWindow* target = boardTreeNoteBook->GetPage(boardTreeNoteBook->GetSelection());

	  if ( wxAuiToolBar* toolBar 
	       = dynamic_cast<wxAuiToolBar*>(wxWindow::FindWindowById(ID_BoardSearchBar, target))) 
	  {
	       if (toolBar->IsShown()) 
	       {
		    // 検索実行
		    wxCommandEvent e(wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BoardSearchBoxDoSearch));
		    BoardSearchBoxDoSearch(e);
		    return true;
	       } 
	  }
     }

     return false;
}

/**
 * マウスのカーソルが特定のウィンドウに入った場合の処理
 */
void XrossBoard::OnEnterWindow(wxMouseEvent& event) {


     wxString widgetName = wxEmptyString;

     if ( event.GetId() == ID_BoardNoteBook || event.GetId() == ID_BoardListCtrl ) {
	  // スレッド一覧ウィンドウ
	  widgetName = BOARD_NOTEBOOK;
	  ChangeUserLastAttachedEvent(widgetName);

     } else if ( event.GetId() == ID_ThreadNoteBook || event.GetId() == ID_ThreadContentWindow ) {
	  // スレ内容ウィンドウ
	  widgetName = THREAD_NOTEBOOK;
	  ChangeUserLastAttachedEvent(widgetName);

     } else if ( event.GetId() == ID_BoardTreeNoteBook || event.GetId() == ID_BoardTreectrl ) {
	  // 板一覧リストウィンドウ
	  widgetName = BOARD_TREE_NOTEBOOK;
	  ChangeUserLastAttachedEvent(widgetName);
     }

     event.Skip();
}
/**
 * 外部板登録用のダイアログを呼び出す
 */
void XrossBoard::CallNewBoardAddDialog(wxCommandEvent& event)
{
     NewBoardAddDialog newBoardAddDlg(this, ID_NewBoardAddDialog, wxT("外部板登録"));
     newBoardAddDlg.ShowModal();

     // 板一覧リスト更新のイベントを実行
     SetBoardList(true);
}
