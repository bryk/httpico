AM_CFLAGS = -Wall -g -O0
AM_CXXFLAGS = -Wall -g -O0
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include

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
						src/Utils.cpp
dist_httpico_LDFLAGS =
		
