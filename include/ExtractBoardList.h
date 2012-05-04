#ifndef EXTRACTBOARDLIST_H
#define EXTRACTBOARDLIST_H

#include <wx/wx.h>
#include <libxml/HTMLparser.h>
#include "SQLiteAccessor.h"

using namespace std;

class ExtractBoardList
{
	public:
		// コンストラクタ
		ExtractBoardList(const char* file);

	private:
		// 内部の処理関数
		void FindBoardInfo(xmlNode*& element);
		// 構造体へのポインタ
		htmlDocPtr m_doc;

		// SQLiteAccessorのインスタンス
		SQLiteAccessor* accessor;
};

#endif // EXTRACTBOARDLIST_H
