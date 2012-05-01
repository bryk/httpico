GTEST_DIR = contrib/gtest-1.6.0
ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -Wall
AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include -I$(GTEST_DIR)/include
AM_LIBS = -lpthread

bin_PROGRAMS = dist/httpico
dist_httpico_SOURCES = src/httpico.cpp \
						src/HttpRequest.cpp
						
			
TESTS=
check_PROGRAMS=

# A simple sample on using gtest.
TESTS += dist/test
check_PROGRAMS += dist/test
dist_test_SOURCES = test/HttpRequest_test.cpp
dist_test_LDADD = $(GTEST_DIR)/lib/libgtest_main.la \
                                 $(GTEST_DIR)/lib/libgtest.la



