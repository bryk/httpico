AM_CFLAGS = -Wall
AM_CXXFLAGS = -Wall
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include
prefix=/usr
datadir=/usr/share/httpico
logdir=$(datadir)/log
cfgdir=/etc

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
						src/ConfigurationReader.cpp \
						src/Logger.cpp \
						src/Utils.cpp
					

dist_httpico_LDADD = -lpthread

srvdir = $(datadir)/srv
imgdir = $(srvdir)/img
dist_cfg_DATA = dist/httpico.conf
dist_log_DATA = dist/empty.log
dist_data_DATA = dist/template.html
dist_img_DATA = dist/img/contentTop.png \
				dist/img/file.png \
				dist/img/folder.png \
				dist/img/footerHr.png \
				dist/img/headerGradient.png 
				

install-exec-hook:
	ls -l $(DESTDIR)$(bindir)/httpico
	chmod 4755 $(DESTDIR)$(bindir)/httpico
	ls -l $(DESTDIR)$(bindir)/httpico


