/* wx_twitter_htmlwindow - An GUI widgets for Twitter
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

#include "wx/wx_twitter_htmlwindow.hpp"
#include "wx/wx_twitter_const.hpp"

/**
BEGIN_EVENT_TABLE(wxTwitterHtmlWindow, wxHtmlWindow)
   EVT_RIGHT_DOWN(ThreadContentWindow::OnRightClickHtmlWindow)
   EVT_HTML_LINK_CLICKED(wxID_ANY, ThreadContentWindow::OnLeftClickHtmlWindow)
   EVT_MENU(ID_CopyTAllToClipBoard, ThreadContentWindow::CopyTAllToClipBoard)   

#ifdef __WXMSW__
   EVT_MOUSEWHEEL(ThreadContentWindow::PageUpDown)
#endif

END_EVENT_TABLE()

*/

void wxTwitterHtmlWindow::Initialize(TwitterClient& client, const wxTwitterHtmlWindow::TwitterViewKind kind)
{
     // 自動スクロールは混乱の元なので使わない
     this->StopAutoScrolling();

     switch(kind)
     {
     case TwitterViewKind::TWITTER_HOME:
	  ReloadHomeTimeline(client);
	  break;
     case TwitterViewKind::TWITTER_MENTION:
	  // FIXME -> Imple later
	  // ReloadTwitterMention 
	  break;
     default:
	  break;
     }
}

/**
 * 自分のタイムライン取得
 * @param TwitterClient& Twitterクライアントの実体
 */
void wxTwitterHtmlWindow::ReloadHomeTimeline(TwitterClient& client)
{
     uint16_t count = 30;
     const std::string since_id;
     const std::string max_id;
     bool include_rts;
     bool include_replies;
     picojson::array rtimeline;

     wxString source = WX_TWITTER_HTML_HEADER;
     source << wxT("<p>Hello, World !!</p>");
     source << WX_TWITTER_HTML_FOOTER;

/**
     if (client.getHomeTimeline(count, since_id, max_id, include_rts, include_replies, rtimeline))
     {
	  for (picojson::array::iterator i = rtimeline.begin(); i != rtimeline.end(); i++) {

	       std::string str = i->serialize(true);
	       *dummy1_txt << wxString((const char*) str.c_str(), wxConvUTF8) << wxT("\n");
	  }
     }
     else
     {
	  // FAIL
	  // *log << wxT("タイムラインの取得に失敗しました\n");
     }
*/

     // メモリに読み込んだHTMLを表示する
     this->AppendToPage(source);
}




