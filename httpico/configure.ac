dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([httpico], [0.1], [piotrek.bryk@gmail.com], [httpico])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_CONFIG_HEADERS([include/config.hpp:config.hpp.in])

AC_PROG_CXX
AC_LANG([C++])
AC_SUBST(AM_CXXFLAGS, "-Wall")
AC_SUBST(CXXFLAGS, "-Wall")

AC_MSG_CHECKING([embedded compilation options])
AC_ARG_ENABLE(emdedded, AS_HELP_STRING(--enable-emdedded=yes, [enable emdedded compilation]),
[
	AC_DEFINE(EMBEDDED, [], [enable emdedded compilation])
    AC_MSG_RESULT(${enableval})
    ],[
    AC_MSG_RESULT("no")
])

AC_PROG_INSTALL
AC_ARG_PROGRAM
PKG_PROG_PKG_CONFIG

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

