/* wx_twitter_notebook - An GUI widgets for Twitter
 *
 * Copyright (C) 2015 Hiroyuki Nagata
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

#include <fstream>
#include <wx/app.h>
#include <wx/dir.h>
#include <wx/textdlg.h>
#include <wx/thread.h>
#include "wx/wx_twitter_notebook.hpp"
#include "wx/wx_twitter_htmlwindow.hpp"

// バージョン
static const std::string THIS_VERSION	   = "PACKAGE_VERSION";

// 設定ファイル保存先
static const wxString DEFAULT_AUTH_FILE	   = wxT(".authkey_");
static const wxString APP_DIR		   = wxT(".ctw");
static const wxString DEFAULT_SETTING_FILE = wxT("ctwrc");

wxString wxTwitterNotebook::AP_COMSUMER_KEY    = wxEmptyString;
wxString wxTwitterNotebook::AP_COMSUMER_SECRET = wxEmptyString;

/**
 * ログ出力用に使用するウィジェットを登録する
 *
 * @param wxTextCtrl* ログ出力ウィジェット
 */
void wxTwitterNotebook::SetLoggingTextCtrl(wxTextCtrl* logging)
{
     this->log = logging;
}

/**
 * cmdline_twitterが出力するファイルを格納するディレクトリを指定する
 * "指定したディレクトリ" / .ctw 以下が対象となる
 *
 * @param const wxString& ファイルを格納するディレクトリ
 */
void wxTwitterNotebook::SetAppDir(const wxString& dir)
{
     // dir = "~/" なので、その内部に ".ctw" というディレクトリを作成する
     this->appDir = dir + wxFILE_SEP_PATH + APP_DIR;
}

void wxTwitterNotebook::SetComsumerPair(const wxString& key,const wxString& sec)
{
     AP_COMSUMER_KEY    = key;
     AP_COMSUMER_SECRET = sec;
}

/**
 * cmdline_twitterが出力するファイルを格納するディレクトリを取得する
 * ディレクトリが存在しなければ作成する
 *
 * @return const wxString ファイルを格納するディレクトリ
 */
const wxString wxTwitterNotebook::GetAppDir()
{
     if ( !this->appDir.IsEmpty() && !wxDir::Exists(this->appDir) )
     {
	  ::wxMkdir(this->appDir);
     }
     
     return this->appDir;
}

/**
 * 初期化処理
 */
void wxTwitterNotebook::Initialize()
{
     // このアプリのコンシューマキーなどを設定
     client.setComsumerPair(std::string(AP_COMSUMER_KEY.mb_str()),
			    std::string(AP_COMSUMER_SECRET.mb_str()));

     // 設定読み込み
     ReadSetting();
	
     // ここから先はユーザのアクセスキーが必要
     if(!ReadAccessKey())
     {
	  // 認証
	  DoAuthentication();
     }

     std::string key, sec;
     client.getUserAccessPair(key,sec);
     if(key.empty() || sec.empty())
     {
	  // アクセスキーがない
	  return;
     }
     
     DoWxWidgetsUIMode();
     return;
}

/**
 * 一連の認証動作を行う
 */
void wxTwitterNotebook::DoAuthentication()
{
     *log << wxT("Twitterにつなぐ準備中...\n");

     std::string rurl, pincode;
	
     *log << wxT("認証手続きをしています...\n");
     if(!client.Authentication_GetURL(rurl))
     {
	  return;
     }

     wxString message = wxT("以下のURLにアクセスして認証後、ダイアログにPINを入力してください\n");
     message << wxT("（※デフォルトのブラウザを開きます）\n");
     message << wxString((const char*)rurl.c_str(), wxConvUTF8);

     // デフォルトのブラウザを開く
     ::wxLaunchDefaultBrowser(wxString((const char*)rurl.c_str(), wxConvUTF8));

     // PIN 入力用ダイアログを出す
     wxTextEntryDialog* dlg = new wxTextEntryDialog(this, message, wxT("Twitter - PINコード認証"));

     // OSX に対応させるため少し奇怪な書き方になっている。嫌ならAppleを批判しろ。
#if wxCHECK_VERSION(3, 0, 0)
     wxTheApp->GetTopWindow()->GetEventHandler()->CallAfter([&, dlg]{
	       dlg->ShowModal();
	       this->ReceivePincode(dlg->GetValue());
	       this->Initialize();
	  });
#else
     // wx-2.8 互換
     dlg->ShowModal();
     this->ReceivePincode(dlg->GetValue());
     this->Initialize();
#endif

     return;
}

void wxTwitterNotebook::ReceivePincode(const wxString& pin)
{
     std::string pincode = std::string(pin.mb_str());

     *log << wxT("認証中です...\n");
     if(pin.IsEmpty() || !client.Authentication_Finish(pincode)){
	  *log << wxT("認証に失敗しました。再度認証しなおしてください\n");
	  return;
     }
     *log << wxT("認証に成功しました\n");
	
     wxString fname;
     std::string key,sec;
     std::ofstream fout;
	
     // ~/.ctw/以下に保存する
     if(GetAppDir().IsEmpty() || !wxDir::Exists(GetAppDir()) )
     {
	  *log << wxT("HOME/.ctw/ ディレクトリを作成できません。\n");
	  return;
     }

     fname << GetAppDir();
     fname << wxFILE_SEP_PATH;
     fname << DEFAULT_AUTH_FILE;
	
     client.getUserAccessPair(key,sec);
     fout.open(fname.mb_str(), std::ios::out);
     if(! fout.is_open()){
	  *log << wxT("設定ファイルを開けませんでした。\n");
	  *log << wxT("再度認証しなおしてください\n");
		
	  return;
     }
     fout << key << std::endl;
     fout << sec << std::endl;
     fout.close();

     return;
}

/**
 * 保存している認証コードを読みこむ
 */
bool wxTwitterNotebook::ReadAccessKey()
{
     wxString fname;
     std::string key,sec;
     std::ifstream fin;
	
     // ~/.ctw/以下から読み込みする
     if(GetAppDir().IsEmpty() || !wxDir::Exists(GetAppDir()) )
     {
	  *log << wxT("HOME/.ctw/ ディレクトリを作成できません。\n");
	  return false;
     }
     
     fname << GetAppDir();
     fname << wxFILE_SEP_PATH;
     fname << DEFAULT_AUTH_FILE;
     
     fin.open(fname.mb_str());
     if(! fin.is_open())
     {
	  *log << wxT("設定ファイルを開けませんでした。\n");
	  *log << wxT("指定されたエイリアス名が間違っているかもしれません。\n");
	  return false;
     }
	
     fin >> key;
     fin >> sec;
     fin.close();
     if(key.empty() || sec.empty())
     {
	  *log << wxT("設定ファイルが壊れています。再度認証してください\n");
	  return false;
     }
     client.setUserAccessPair(key,sec);
     return true;
}

void wxTwitterNotebook::InitSetting()
{
     setting["READHOME_COUNT"]		= minisetting::value(200);
     setting["READHOME_VIEWRT"]		= minisetting::value(true);
     setting["READHOME_VIEWMENTION"]    = minisetting::value(true);
	
     setting["READUSER_COUNT"]		= minisetting::value(200);
     setting["READUSER_VIEWRT"]		= minisetting::value(true);
     setting["READUSER_VIEWMENTION"]    = minisetting::value(true);

     setting["READDM_COUNT"]		= minisetting::value(200);

     setting["READLIST_COUNT"]		= minisetting::value(200);
     setting["READLIST_VIEWRT"]		= minisetting::value(true);
	
     setting["VIEW_SHORT"]		= minisetting::value(false);
     setting["VIEW_SHORT_NAMEONLY"]	= minisetting::value(true);
     setting["VIEW_STATUSID"]		= minisetting::value(true);
}

/**
 * 設定ファイルの読み込み
 */
void wxTwitterNotebook::ReadSetting()
{
     using namespace minisetting;
     wxString fname;
     std::ifstream fin;
	
     InitSetting();
	
     // ~/.ctw/以下から読み込みする
     if(GetAppDir().IsEmpty() || !wxDir::Exists(GetAppDir()) )
     {
	  *log << wxT("HOME/.ctw/ ディレクトリを作成できません。\n");
	  *log << wxT("設定ファイルを読み込めませんでした。	 \n");
	  *log << wxT("デフォルトの設定を使用します		 \n");
	  return;
     }

     fname << GetAppDir();
     fname << wxFILE_SEP_PATH;
     fname << DEFAULT_SETTING_FILE;

     fin.open(fname.mb_str());
     if(! fin.is_open())
     {
	  *log << wxT("設定ファイルを開けませんでした。\n");
	  *log << wxT("デフォルトの設定を使用します   \n"); 
	  WriteSetting();
	  return;
     }
     parse(fin,setting);
     fin.close();
}

/**
 * 設定ファイルが存在しない場合に新規に作成する
 */
void wxTwitterNotebook::WriteSetting()
{
     
     using namespace minisetting;
     wxString fname;
     std::ofstream fout;
	
     // ~/.ctw/以下に保存する
     if(GetAppDir().IsEmpty() || !wxDir::Exists(GetAppDir()) )
     {
	  *log << wxT("HOME/.ctw/ ディレクトリを作成できません。\n");
	  return;
     }

     fname << GetAppDir();
     fname << wxFILE_SEP_PATH;
     fname << DEFAULT_SETTING_FILE;

     fout.open(fname.mb_str(),ios::out);
     if(! fout.is_open())
     {
	  return;
     }
     
     // "ホームタイムライン読み込み設定"
     // "ホームタイムラインの読み込み件数(20-200)"
     putval(fout,setting,"READHOME_COUNT");
     // "ホームタイムラインでRTを表示するかどうか(true false)"
     putval(fout,setting,"READHOME_VIEWRT");
     // "ホームタイムラインで@を表示するかどうか(true false)"
     putval(fout,setting,"READHOME_VIEWMENTION");
     // "ユーザタイムライン読み込み設定"
     // "ユーザタイムラインの読み込み件数(20-200)"
     putval(fout,setting,"READUSER_COUNT");
     // "ユーザタイムラインでRTを表示するかどうか(true false)"
     putval(fout,setting,"READUSER_VIEWRT");
     // "ユーザタイムラインで@を表示するかどうか(true false)"
     putval(fout,setting,"READUSER_VIEWMENTION");
     // "ダイレクトメッセージ読み込み設定"
     // "ダイレクトメッセージの読み込み件数(20-200)"
     putval(fout,setting,"READDM_COUNT");
     // "リスト読み込み設定"
     // "リストタイムラインの読み込み件数(20-200)"
     putval(fout,setting,"READLIST_COUNT");
     // "リストタイムラインでRTを表示するかどうか(true false)"
     putval(fout,setting,"READLIST_VIEWRT");
     // "表示設定"
     // "短縮表示をおこなうかどうか(true false)"
     putval(fout,setting,"VIEW_SHORT");
     // "短縮表示のとき、名前を登録名(スクリーンネーム以外)にする(true false)"
     putval(fout,setting,"VIEW_SHORT_NAMEONLY");
     // "発言IDの表示をおこなうかどうか(true false)"
     putval(fout,setting,"VIEW_STATUSID");
	
     fout.close();
}

/**
 * GUIのウィジェット設定
 */
void wxTwitterNotebook::DoWxWidgetsUIMode()
{
     DeleteAllPages();

     wxPanel* dummy1 = new wxPanel(this);
     wxBoxSizer* dummy1_sizer = new wxBoxSizer(wxHORIZONTAL);
     wxTwitterHtmlWindow* dummy1_txt = new wxTwitterHtmlWindow(dummy1);
     dummy1_sizer->Add(dummy1_txt, 1, wxEXPAND, 0);
     dummy1->SetSizer(dummy1_sizer);

     wxPanel* dummy2 = new wxPanel(this);
     wxBoxSizer* dummy2_sizer = new wxBoxSizer(wxHORIZONTAL);
     wxTextCtrl* dummy2_txt   = new wxTextCtrl(dummy2, 
					       wxID_ANY, 
					       wxEmptyString, 
					       wxDefaultPosition, 
					       wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
     dummy2_sizer->Add(dummy2_txt, 1, wxEXPAND, 0);
     dummy2->SetSizer(dummy2_sizer);

     wxPanel* dummy3 = new wxPanel(this);
     wxBoxSizer* dummy3_sizer = new wxBoxSizer(wxHORIZONTAL);
     wxTextCtrl* dummy3_txt   = new wxTextCtrl(dummy3, 
					       wxID_ANY, 
					       wxEmptyString, 
					       wxDefaultPosition, 
					       wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
     dummy3_sizer->Add(dummy3_txt, 1, wxEXPAND, 0);
     dummy3->SetSizer(dummy3_sizer);

     AddPage(dummy1, wxT("タイムライン"), false);
     dummy1_txt->Initialize(client, wxTwitterHtmlWindow::TwitterViewKind::TWITTER_HOME);

     AddPage(dummy2, wxT("通知"), false);
     AddPage(dummy3, wxT("メッセージ"), false);
}
