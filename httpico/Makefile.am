AM_CFLAGS = -Wall
AM_CXXFLAGS = -Wall
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include
prefix=/usr
datadir=/usr/share/httpico

bin_PROGRAMS = dist/httpico
dist_httpico_SOURCES = src/Httpico.cpp \
						src/HttpRequest.cpp \
						src/HttpResponse.cpp \
						src/HttpResponseState.cpp \
						src/HttpResponseProcessor.cpp \
						src/DirectoryResponseProcessor.cpp \
						src/FileResponseProcessor.cpp \
						src/HttpServer.cpp \
						src/HttpRequestProcessor.cpp \
						src/HttpServerConfiguration.cpp \
						src/Logger.cpp \
						src/Utils.cpp
					

dist_httpico_LDADD = -lpthread

srvdir = $(datadir)/srv
imgdir = $(srvdir)/img
dist_data_DATA = dist/template.html
dist_img_DATA = dist/img/contentTop.png \
				dist/img/file.png \
				dist/img/folder.png \
				dist/img/footerHr.png \
				dist/img/headerGradient.png 


