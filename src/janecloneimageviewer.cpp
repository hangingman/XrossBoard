/**
 * janecloneimageviewer.cpp - janeclone's plain imageviewer
 *
 * Copyright (c) 2013 Hiroyuki Nagata <newserver002@gmail.com>
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

#include "janecloneimageviewer.hpp"

#ifndef __WXMSW__
#include"../rc/janeclone.xpm"
#endif

BEGIN_EVENT_TABLE(JaneCloneImageViewer, wxFrame)
   EVT_CLOSE(JaneCloneImageViewer::OnClose)
   EVT_RIGHT_DOWN(JaneCloneImageViewer::OnRightClickImageViewer)
   EVT_MOUSEWHEEL(JaneCloneImageViewer::OnMouseWheel)

   EVT_MENU(ID_OneThumbnailTabClose,JaneCloneImageViewer::OneThumbnailTabClose)
   EVT_MENU(ID_AllThumbnailTabClose,JaneCloneImageViewer::AllThumbnailTabClose)
   EVT_MENU(ID_AllLeftThumbnailTabClose,JaneCloneImageViewer::AllLeftThumbnailTabClose)
   EVT_MENU(ID_AllRightThumbnailTabClose,JaneCloneImageViewer::AllRightThumbnailTabClose)
   EVT_MENU(ID_SelectLeftThumbnailTab,JaneCloneImageViewer::SelectLeftThumbnailTab)
   EVT_MENU(ID_SelectRightThumbnailTab,JaneCloneImageViewer::SelectRightThumbnailTab)
   EVT_MENU(ID_OnOpenImageByBrowser,JaneCloneImageViewer::OnOpenImageByBrowser)
   EVT_MENU(ID_HideThumbnailTab,JaneCloneImageViewer::HideThumbnailTab)
END_EVENT_TABLE()

/**
 * Default constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer() {
}
/**
 * Default constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer(wxWindow* parent, int id, const wxString& title, 
					   const wxPoint& pos, const wxSize& size, long style):
//     wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {
     wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE) {

     // アイコンの設定を行う
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(janeclone));
#endif

     // wxAuiManagerの初期化
     m_mgr.SetManagedWindow(this);
     // ノートブックのサイズ調整
     wxSize client_size = GetClientSize();
     // 画像を表示させるパネルの宣言
     thumbnailNoteBook = new wxAuiNotebook(this, ID_ThumbnailNoteBook, wxPoint(client_size.x, client_size.y), 
					   wxDefaultSize, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_WINDOWLIST_BUTTON);
     // ステータスバー設置
     CreateStatusBar(3);

     set_properties(title);
     do_layout();
}
/**
 * フレームクラスのデストラクタ
 */
JaneCloneImageViewer::~JaneCloneImageViewer() {

     // wxAuiManagerはデストラクタで破棄しなければいけない
     m_mgr.UnInit();
}
/**
 * タイトルとサイズを設定
 */
void JaneCloneImageViewer::set_properties(const wxString& title) {
    SetTitle(title);
    SetSize(wxSize(640, 480));
}
/**
 * レイアウトを設定する
 */
void JaneCloneImageViewer::do_layout() {

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(thumbnailNoteBook, 1, wxEXPAND, 0);
    SetSizer(vbox);
    Layout();
}
/**
 * Copy constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer(const JaneCloneImageViewer& rhs) {}
/**
 * Assignment operator
 */
JaneCloneImageViewer& JaneCloneImageViewer::operator=(const JaneCloneImageViewer& rhs) {
    if (this != &rhs) {
        // TODO: implement copy
    }
    return *this;
}
/**
 *  画像ファイルの情報を設定する
 */
void JaneCloneImageViewer::SetImageFile(DownloadImageResult* result) {

     thumbnailNoteBook->Freeze();

     // resultの結果を元に画像のサムネイルと画像を配置する
     wxImage image;
     wxBitmap bitmap;
     
     // load wxImage
     if (!image.LoadFile(result->imagePath)) {
	  wxMessageBox(wxT("画像ファイルの読み出しに失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  return;
     }
     // wxImage to wxBitmap
     bitmap = wxBitmap(image);

     if (!bitmap.Ok()) {
	  wxMessageBox(wxT("画像データの内部変換に失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  return;
     }
     // wxBitmapTypeの判別
     wxBitmapType type;
     const wxString ext = result->ext;

     if (!ext.CmpNoCase(wxT("png"))) {
     	  type = wxBITMAP_TYPE_PNG;
     } else if (!ext.CmpNoCase(wxT("jpg"))) {
     	  type = wxBITMAP_TYPE_JPEG;
     } else if (!ext.CmpNoCase(wxT("jpeg"))) {
     	  type = wxBITMAP_TYPE_JPEG;
     } else if (!ext.CmpNoCase(wxT("gif"))) {
     	  type = wxBITMAP_TYPE_GIF;
     } else if (!ext.CmpNoCase(wxT("bmp"))) {
     	  type = wxBITMAP_TYPE_BMP;
     } else if (!ext.CmpNoCase(wxT("ico"))) {
     	  type = wxBITMAP_TYPE_ICO;
     } else if (!ext.CmpNoCase(wxT("xpm"))) {
     	  type = wxBITMAP_TYPE_XPM;
     } else if (!ext.CmpNoCase(wxT("tiff"))) {
     	  type = wxBITMAP_TYPE_TIF;
     } else {
     	  type = wxBITMAP_TYPE_ANY;
     }
     // 読み取った画像をパネルに載せる
     wxImagePanel* panel = new wxImagePanel(thumbnailNoteBook, result->imagePath, type);
     thumbnailNoteBook->AddPage(panel, wxT("画像パネル"), true);
     result->result = true;

     thumbnailNoteBook->Thaw();
     m_mgr.Update();
}
/**
 * ウィンドウを閉じるイベント
 */
void JaneCloneImageViewer::OnClose(wxCloseEvent& event) {

     // wxAuiNotebookがもっていたデータは落とす
     size_t pages = thumbnailNoteBook->GetPageCount();
     for (size_t i = 0; i < pages; i++) {
	  thumbnailNoteBook->DeletePage(0);
     }
     
     // 画像ビューアはJaneClone本体が落ちるまで閉じない
     this->Hide();
     event.Skip(false);
}
/**
 * 画像ビューアを開いた状態でマウスホイールを動かした場合のイベント
 */
void JaneCloneImageViewer::OnMouseWheel(wxMouseEvent& event) {
     
     if (event.m_controlDown) {
	  // コントロールキーが押されている => 画像の拡大縮小

     } else {
	  const int current = thumbnailNoteBook->GetSelection();		    
	  const int max     = thumbnailNoteBook->GetPageCount();

	  // 画像の切り替え
	  if (event.m_wheelRotation > 0) {
	       // プラス値 ←
	       if (current != 0) thumbnailNoteBook->SetSelection(current - 1);
	  } else {
	       // マイナス値 →
	       if (current != max) thumbnailNoteBook->SetSelection(current + 1);
	  }	  
     }

     event.Skip();
}
/**
 * 画像ビューアで右クリックした場合の処理
 */
void JaneCloneImageViewer::OnRightClickImageViewer(wxMouseEvent& event) {

     wxMenu* tabs = new wxMenu();
     tabs->Append(ID_OneThumbnailTabClose, wxT("このタブを閉じる"));
     tabs->Append(wxID_ANY, wxT("エラーのタブを閉じる"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("このタブ以外を閉じる"));
     tabs->Append(ID_AllThumbnailTabClose, wxT("すべてのタブを閉じる"));
     tabs->Append(ID_AllLeftThumbnailTabClose, wxT("これより左を閉じる"));
     tabs->Append(ID_AllRightThumbnailTabClose, wxT("これより右を閉じる"));
     tabs->AppendSeparator();
     //tabs->Append(wxID_ANY, wxT(""));
     tabs->Append(wxID_ANY, wxT("マーク/解除"));
     tabs->Append(wxID_ANY, wxT("すべてマーク"));
     tabs->Append(wxID_ANY, wxT("すべてマーク解除"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("名前を付けて保存"));
     tabs->Append(wxID_ANY, wxT("全て保存"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("再読み込み"));
     tabs->Append(ID_OnOpenImageByBrowser, wxT("ブラウザで開く"));
     tabs->Append(wxID_ANY, wxT("外部ビューアで開く"));
     tabs->Append(wxID_ANY, wxT("Windowsの関連付けで開く"));
     tabs->Append(wxID_ANY, wxT("参照元スレッドを開く"));
     tabs->Append(wxID_ANY, wxT("URLをコピー"));
     tabs->AppendSeparator();
     //tabs->Append(wxID_ANY, wxT("外部ビューアで開く"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("左回転"));
     tabs->Append(wxID_ANY, wxT("右回転"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("ズームイン"));
     tabs->Append(wxID_ANY, wxT("ズームアウト"));
     tabs->Append(wxID_ANY, wxT("ズーム変更"));
     tabs->Append(wxID_ANY, wxT("元のサイズに戻す"));
     tabs->AppendSeparator();
     tabs->Append(wxID_ANY, wxT("ウィンドウに合わせて表示"));
     //tabs->Append(wxID_ANY, wxT("ズームアウト"));
     tabs->Append(wxID_ANY, wxT("ビューア設定"));
     tabs->AppendSeparator();
     tabs->Append(ID_SelectRightThumbnailTab, wxT("次のタブ"));
     tabs->Append(ID_SelectLeftThumbnailTab, wxT("前のタブ"));
     tabs->AppendSeparator();
     tabs->Append(ID_HideThumbnailTab, wxT("ビューアを隠す"));

     // ポップアップメニューを表示させる
     PopupMenu(tabs);
}
/**
 * 画像タブをひとつ閉じる
 */
void JaneCloneImageViewer::OneThumbnailTabClose(wxCommandEvent& event) {

     // アクティブなタブを選択して閉じる
     thumbnailNoteBook->DeletePage(thumbnailNoteBook->GetSelection());
}
/**
 * すべての画像タブを閉じる
 */
void JaneCloneImageViewer::AllThumbnailTabClose(wxCommandEvent& event) {

     int pages = thumbnailNoteBook->GetPageCount();
     for (int i=0;i<pages;i++) {
	  thumbnailNoteBook->DeletePage(0);
     }
}
/**
 * これより左の画像タブをを閉じる
 */
void JaneCloneImageViewer::AllLeftThumbnailTabClose(wxCommandEvent& event) {

     // タブの数を数える
     size_t select = thumbnailNoteBook->GetSelection();

     for (unsigned int i=0;i<select;i++) {
	  thumbnailNoteBook->DeletePage(0);
     }
}
/**
 * これより右の画像タブを閉じる
 */
void JaneCloneImageViewer::AllRightThumbnailTabClose(wxCommandEvent& event) {

     // タブの数を数える
     size_t pages = thumbnailNoteBook->GetPageCount();
     size_t select = thumbnailNoteBook->GetSelection();
     for (unsigned int i=0;i<pages;i++) {
	  if (i>select) {
	       thumbnailNoteBook->DeletePage(select+1);
	  }
     }
}
/**
 * 左の画像タブに移動
 */
void JaneCloneImageViewer::SelectLeftThumbnailTab(wxCommandEvent& event) {

     const int current = thumbnailNoteBook->GetSelection();
     // プラス値 ←
     if (current != 0) thumbnailNoteBook->SetSelection(current - 1);
}
/**
 * 右の画像タブに移動
 */
void JaneCloneImageViewer::SelectRightThumbnailTab(wxCommandEvent& event) {

     const int current = thumbnailNoteBook->GetSelection();		    
     const int max     = thumbnailNoteBook->GetPageCount();
     if (current != max) thumbnailNoteBook->SetSelection(current + 1);
}
/**
 * 画像をブラウザで開く
 */
void JaneCloneImageViewer::OnOpenImageByBrowser(wxCommandEvent& event) {
}
/**
 * 画像ビューアを隠す
 */
void JaneCloneImageViewer::HideThumbnailTab(wxCommandEvent& event) {

     this->Hide();
}
