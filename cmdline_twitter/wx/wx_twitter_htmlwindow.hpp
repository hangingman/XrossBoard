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

#ifndef WX_TWITTER_HTMLWINDOW_HPP_
#define WX_TWITTER_HTMLWINDOW_HPP_

#include <wx/html/htmlwin.h>
#include <wx/html/htmldefs.h>
#include <wx/event.h>

class wxTwitterHtmlWindow : public wxHtmlWindow 
{

public:




#ifdef __WXMSW__ /** Windows Only */
     void PageUpDown(wxMouseEvent& event);
#endif

//DECLARE_EVENT_TABLE()
};

#endif // WX_TWITTER_HTMLWINDOW_HPP_
