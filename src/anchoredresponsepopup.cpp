
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

#include "anchoredresponsepopup.hpp"


IMPLEMENT_CLASS(AnchoredResponsePopup,wxPopupTransientWindow)

BEGIN_EVENT_TABLE(AnchoredResponsePopup,wxPopupTransientWindow)
   EVT_ENTER_WINDOW( AnchoredResponsePopup::EnterWindow )
   EVT_LEAVE_WINDOW( AnchoredResponsePopup::LeaveWindow )
   EVT_SIZE( AnchoredResponsePopup::OnSize )
   EVT_SET_FOCUS( AnchoredResponsePopup::OnSetFocus )
   EVT_KILL_FOCUS( AnchoredResponsePopup::OnKillFocus )
END_EVENT_TABLE()

/**
 * ポップアップ用ウィンドウのコンストラクタ
 *
 * @param wxWindow* parent     親ウィンドウのインスタンス
 * @param wxPoint&  point      ウィンドウの出現位置
 * @param wxSize    size       ウィンドウのサイズ
 * @raram wxString& htmlSource 表示させるHTMLソース
 */
AnchoredResponsePopup::AnchoredResponsePopup( wxWindow *parent, wxPoint& point, wxSize size, wxString& htmlSource )
:wxPopupTransientWindow( parent ) {

     // HTMLのソース
     wxString html = HTML_HEADER;
     html+=htmlSource;
     html+=HTML_FOOTER;
     wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);
     // wxHtmlWindowにHTMLソースを設定する
     htmlWin = new wxHtmlWindow(this, wxID_ANY, point, size, wxHW_SCROLLBAR_AUTO);
     this->SetInternalFonts(htmlWin);
     htmlWin->SetBorders(0);
     htmlWin->SetPage(html);
     // wxHtmlWindow内部での実質サイズを取得する
     int hx = htmlWin->GetInternalRepresentation()->GetWidth();
     int hy = htmlWin->GetInternalRepresentation()->GetHeight();
     htmlWin->SetSize(hx, hy);
     // このウィンドウ(popup)にサイザーをセットして最小の大きさに設定する
     topsizer->Add(htmlWin, 1, wxALL, 10);
     this->SetSize(hx, hy);
}
/**
 * ポップアップ内部のフォントを設定する
 * @param wxHtmlWindow
 */
void AnchoredResponsePopup::SetInternalFonts(wxHtmlWindow* htmlWin) {

     // ユーザーのホームディレクトリを取得
     wxDir workDir(::wxGetHomeDir());
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!workDir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }
     if (!jcDir.HasSubDirs(wxT("prop"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("prop"));
     }

     wxString configFile = jcDir.GetName() + wxFileSeparator + wxT("prop") + wxFileSeparator + APP_CONFIG_FILE;
     wxFileConfig* config = new wxFileConfig(wxT("JaneClone"), wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

     // フォント設定を読み出し
     wxString fontName;
     config->Read(wxT("HTML.Font"), &fontName, wxEmptyString);
     int p, realSize;
     config->Read(wxT("HTML.PointSize"), &p, 0);
     delete config;
     // HTMLソース中のテキストの種類とサイズを決定する
     static int f_size[] = {p - 2, p - 1, p, p + 1, p + 2, p + 3, p + 4 };

     // 設定ファイルにフォントが設定されていなければSetFontsは実行しない
     if (fontName != wxEmptyString && p != 0) {
	  htmlWin->SetFonts(fontName, wxEmptyString, f_size);
     }
}

wxSize AnchoredResponsePopup::GetPopupWindowSize() {
     return wxSize(htmlWin->GetInternalRepresentation()->GetWidth(), htmlWin->GetInternalRepresentation()->GetHeight());
}

void AnchoredResponsePopup::Popup(wxWindow* WXUNUSED(focus)) {
     wxPopupTransientWindow::Popup();
}

void AnchoredResponsePopup::OnDismiss() {
     wxPopupTransientWindow::OnDismiss();
}

bool AnchoredResponsePopup::ProcessLeftDown(wxMouseEvent& event) {
     return wxPopupTransientWindow::ProcessLeftDown(event);
}
bool AnchoredResponsePopup::Show(bool show) {
     return wxPopupTransientWindow::Show(show);
}

void AnchoredResponsePopup::OnSize(wxSizeEvent &event) {
     event.Skip();
}

void AnchoredResponsePopup::OnSetFocus(wxFocusEvent &event) {
     event.Skip();
}

void AnchoredResponsePopup::OnKillFocus(wxFocusEvent &event) {
     event.Skip();
}

void AnchoredResponsePopup::EnterWindow(wxMouseEvent &event) {
}

void AnchoredResponsePopup::LeaveWindow(wxMouseEvent &event) {
}
