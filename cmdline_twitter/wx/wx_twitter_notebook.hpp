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

#ifndef WX_TWITTER_NOTEBOOK_HPP_
#define WX_TWITTER_NOTEBOOK_HPP_

#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/textctrl.h>
#include <string>
#include "typedef.hpp"
#include "minisetting.hpp"
#include "twitter_client.hpp"

class wxTwitterNotebook : public wxAuiNotebook
{
     
public:
     wxTwitterNotebook(wxWindow* parent,
		       wxWindowID id = wxID_ANY,
		       const wxPoint& pos = wxDefaultPosition,
		       const wxSize& size = wxDefaultSize,
		       long style = wxAUI_NB_TOP    |
		       wxAUI_NB_TAB_SPLIT	    |
		       wxAUI_NB_TAB_MOVE	    |
		       wxAUI_NB_WINDOWLIST_BUTTON   |
		       wxAUI_NB_SCROLL_BUTTONS):
	  wxAuiNotebook(parent, id, pos, size, style){}

     void Initialize();
     void SetLoggingTextCtrl(wxTextCtrl* logging);
     void SetAppDir(const wxString& dir);
     void SetComsumerPair(const wxString& key,const wxString& sec);
     const wxString GetAppDir();

private:

     void DoAuthentication();
     bool ReadAccessKey();
     void InitSetting();
     void ReadSetting();
     void WriteSetting();
     void DoWxWidgetsUIMode();
     
     minisetting::object setting;
     TwitterClient       client;
     wxTextCtrl*         log;
     wxString            appDir;
     static wxString	 AP_COMSUMER_KEY;
     static wxString	 AP_COMSUMER_SECRET;
     
};

#endif // WX_TWITTER_NOTEBOOK_HPP_
