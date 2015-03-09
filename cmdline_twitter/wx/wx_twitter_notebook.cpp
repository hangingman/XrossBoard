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
#include <wx/dir.h>
#include "wx/wx_twitter_notebook.hpp"

// バージョン
static const std::string THIS_VERSION	   = "PACKAGE_VERSION";

// 設定ファイル保存先
static const wxString DEFAULT_AUTH_FILE	   = wxT(".authkey_");
static const wxString APP_DIR		   = wxT(".ctw");
static const wxString DEFAULT_SETTING_FILE = wxT("ctwrc");

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

/**
 * cmdline_twitterが出力するファイルを格納するディレクトリを取得する
 *
 * @return const wxString ファイルを格納するディレクトリ
 */
const wxString wxTwitterNotebook::GetAppDir()
{
     return this->appDir;
}

/**
 * 初期化処理
 */
void wxTwitterNotebook::Initialize()
{
}

/**
 * 一連の認証動作を行う
 */
void wxTwitterNotebook::DoAuthentication()
{
     *log << wxT("Twitterにつなぐ準備中...\n");

     std::string rurl, pincode;
	
     *log << wxT("認証手続きをしています...\n");
     if(!client.Authentication_GetURL(rurl)){
	  return;
     }
/**
     *log << wxT("以下のURLにアクセスして認証後、PINを入力してください");
     *log << rurl;
     cin >> pincode;
*/
     *log << wxT("認証中です...\n");
     if(!client.Authentication_Finish(pincode)){
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
     fname << DEFAULT_AUTH_FILE;
     //if(! opt.getAries().empty()) fname += opt.getAries();
	
     client.getUserAccessPair(key,sec);
     fout.open(fname.c_str(), std::ios::out);
     if(! fout.is_open()){
	  *log << wxT("設定ファイルを開けませんでした。\n");
	  *log << wxT("再度認証しなおしてください\n");
		
	  return;
     }
     fout << key;
     fout << sec;
     fout.close();

     return;
}
