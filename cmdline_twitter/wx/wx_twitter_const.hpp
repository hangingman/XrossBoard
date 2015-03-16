#ifndef WX_TWITTER_CONST_HPP_
#define WX_TWITTER_CONST_HPP_

// ヘッダ部分にあたるHTML
static const wxString WX_TWITTER_HTML_HEADER =
     wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#efefef text=black link=blue alink=red vlink=#660099>");

// ヘッダ部分にあたるHTML
static const wxString WX_TWITTER_CUSTOM_HTML_HEADER =
     wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");

// ポップアップウィンドウのヘッダ部分にあたるHTML
static const wxString WX_TWITTER_HTML_HEADER_POPUP =
     wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#eedcb3 text=black link=blue alink=red vlink=#660099>");

// 読み込みに失敗した場合に表示するページ
static const wxString WX_TWITTER_FAIL_TO_READ_PAGE = 
     wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\"><title></title></head><body><span>ファイルの読み込みに失敗しました・リロードしてみてください</span></body></html>");

// フッター部分にあたるHTML
static const wxString WX_TWITTER_HTML_FOOTER = wxT("</body></html>");

#endif /* WX_TWITTER_CONST_HPP_ */
