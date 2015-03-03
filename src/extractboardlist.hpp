/**XrossBoard - a text board site viewer for open BBS
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

#ifndef EXTRACTBOARDLIST_HPP_
#define EXTRACTBOARDLIST_HPP_

#include <utility>
#include <tuple>
#include <vector>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <wx/wx.h>
#include "datatype.hpp"

// 除外するカテゴリ
#define EXCLUDECATEGORY1 wxT("特別企画")
#define EXCLUDECATEGORY2 wxT("他のサイト")

class ExtractBoardList 
{
public:
     // 板一覧抽出
     static void ExtractBoardInfo(const char* file);
     // HTML整形
     static const wxString HtmlFormat(const wxString& html);

private:
     // 内部の処理関数
     static void FindBoardInfo(std::vector<BoardRowTuple>& array, xmlNode*& element);
};

#endif // EXTRACTBOARDLIST_HPP
