#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

AC_INIT(lv, 4.51)
AC_CONFIG_SRCDIR([src/lv.c])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

#AX_ENABLE_BUILDDIR
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_LN_S
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CPP
AC_PROG_MAKE_SET
AC_PATH_PROGS(PERL, perl perl4 perl5)

AC_MSG_CHECKING(if --enable-purify is specified)
AC_SUBST(PURIFY)
PURIFY=
AC_ARG_ENABLE(purify,
	[AS_HELP_STRING([--enable-purify], [enables checks by purify])],
	[AC_MSG_RESULT(yes)
	AC_PATH_PROGS(PURIFY, purify, "")
	if test "$PURIFY" != ""; then
		PURIFY="$PURIFY purecov"
	fi],
	[AC_MSG_RESULT(no)])

# Checks host type
case "$host" in
  i?86-win32* | i?86-*-mingw* | x86_64-*-mingw*)
	;;
  *)
	AC_DEFINE(UNIX)
	AC_DEFINE(TERMCAP)
	;;
esac

# Checks for libraries.
AC_CHECK_LIB(terminfo, tgetstr,, [AC_CHECK_LIB(termcap, tgetstr,, [AC_CHECK_LIB(ncurses, tgetstr,, [AC_CHECK_LIB(curses, tgetstr)])])])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h sys/time.h termio.h unistd.h termios.h locale.h)

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS(sigaction tgetnum setlocale)
AC_FUNC_GETPGRP
AC_PROG_GCC_TRADITIONAL
AC_TYPE_SIGNAL

# From Bruno Haible.

AC_DEFUN([AM_LANGINFO_CODESET],
[
  AC_CACHE_CHECK([for nl_langinfo and CODESET], am_cv_langinfo_codeset,
    [AC_TRY_LINK([#include <langinfo.h>],
      [char* cs = nl_langinfo(CODESET);],
      am_cv_langinfo_codeset=yes,
      am_cv_langinfo_codeset=no)
    ])
  if test $am_cv_langinfo_codeset = yes; then
    AC_DEFINE(HAVE_LANGINFO_CODESET, 1,
      [Define if you have <langinfo.h> and nl_langinfo(CODESET).])
  fi
])

AM_LANGINFO_CODESET

# Checks for largefile support
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

AC_MSG_CHECKING(whether fastio is used)
AC_ARG_ENABLE(fastio,
	[AS_HELP_STRING([--enable-fastio], [tries to reduce stdio overhead])],
	[if ! test "$enableval" = no; then
	 	AC_DEFINE(USE_INTERNAL_IOBUF, 1)
		AC_MSG_RESULT(yes)
	 else
		AC_MSG_RESULT(no)
	 fi],
	[AC_MSG_RESULT(no)])

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
