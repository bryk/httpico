ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -Wall
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include -I$(GTEST_DIR)/include

bin_PROGRAMS = dist/httpico
dist_httpico_SOURCES = src/Httpico.cpp \
						src/HttpRequest.cpp \
						src/HttpResponse.cpp \
						src/HttpServer.cpp \
						src/HttpRequestProcessor.cpp \
						src/HttpServerConfiguration.cpp \
						src/Utils.cpp
dist_httpico_LDFLAGS =
		
### TEST section
GTEST_DIR = contrib/gtest-1.6.0
GTEST_LIBS = $(GTEST_DIR)/lib/libgtest_main.la \
				$(GTEST_DIR)/lib/libgtest.la	
TESTS=
check_PROGRAMS=

#HttpRequestTest
TESTS += dist/HttpRequestTest
check_PROGRAMS += dist/HttpRequestTest
dist_HttpRequestTest_SOURCES = test/HttpRequestTest.cpp
dist_HttpRequestTest_LDADD = $(GTEST_LIBS)
     
#HttpResponseTest
TESTS += dist/HttpResponseTest
check_PROGRAMS += dist/HttpResponseTest
dist_HttpResponseTest_SOURCES = test/HttpResponseTest.cpp
dist_HttpResponseTest_LDADD = $(GTEST_LIBS)