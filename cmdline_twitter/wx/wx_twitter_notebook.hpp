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
     void Initialize();
     void SetLoggingTextCtrl(wxTextCtrl* logging);

private:

     wxTextCtrl* log;
     
/**
     void DoAuthentication();
     bool ReadAccesskey();
     bool InitUserinfo();
     void DoSimpleUImode();
	
     void InitSetting();
     void ReadSetting();
     void WriteSetting();
*/
     //cmdlineOption        opt;
     //minisetting::object  setting;
};

#endif // WX_TWITTER_NOTEBOOK_HPP_
