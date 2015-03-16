AUTOMAKE_OPTIONS 	= foreign subdir-objects
ACLOCAL_AMFLAGS		= -I m4

HASH_SRCS 		= hashcodes/crypto_hash.cpp hashcodes/sha1.cpp hashcodes/md5.cpp
HTTP_SRCS 		= http/httpclient.cpp http/httpcurl.cpp
AUTH_SRCS 		= oauth/base64.cpp oauth/oauth.cpp
TWCR_SRCS 		= twitter/twitter_client.cpp
UI_SRCS   		= ui/simple_ui.cpp ui/simple_terminal.cpp
SRCS	  		= main.cpp
KEYS	  		= keys/apikeys.hpp

### Lib Target
noinst_LIBRARIES 	= libctw.a
libctw_a_SOURCES	= $(HASH_SRCS) $(HTTP_SRCS) $(AUTH_SRCS) $(TWCR_SRCS)
libctw_a_CXXFLAGS	= @CURL_CFLAGS@ \
			  -I$(top_builddir)/cmdline_twitter/hashcodes \
			  -I$(top_builddir)/cmdline_twitter/http      \
			  -I$(top_builddir)/cmdline_twitter/oauth     \
			  -I$(top_builddir)/cmdline_twitter/twitter   \
			  -I$(top_builddir)/cmdline_twitter/include

# for package build
noinst_HEADERS =                      \
	oauth/base64.hpp              \
	oauth/oauth.hpp		      \
	main.hpp		      \
	keys/apikeys.hpp	      \
	hashcodes/portable_endian.hpp \
	hashcodes/byteorder.hpp	      \
	hashcodes/crypto_hash.hpp     \
	hashcodes/sha1.hpp	      \
	hashcodes/hmac.hpp	      \
	hashcodes/md5.hpp	      \
	include/picojson.h	      \
	include/typedef.hpp	      \
	include/minisetting.hpp	      \
	twitter/twitter_client.hpp    \
	twitter/twitter_json.hpp      \
	http/httpcurl.hpp	      \
	http/httpclient.hpp

# wxWidgets related source ( if you would not to build it, comment out below )
WX_UI_SRCS   		= wx/wx_twitter_notebook.cpp   \
			  wx/wx_twitter_notebook.hpp   \
			  wx/wx_twitter_htmlwindow.cpp \
			  wx/wx_twitter_htmlwindow.hpp

libctw_a_SOURCES	+= $(WX_UI_SRCS)
libctw_a_CXXFLAGS	+= -I$(top_builddir)/cmdline_twitter
# Header file directory is wx/*.hpp according to wx tradition. It plays a role of namespace.
noinst_HEADERS          += wx/wx_twitter_notebook.hpp  \
			   wx/wx_twitter_htmlwindow.hpp

$(KEYS):
	-@ ./write_apikeys.sh
