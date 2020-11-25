dnl Template file for GNU Autoconf
dnl
dnl Copyright (c) 2010, 2011, 2013 Free Software Foundation, Inc.
dnl Copyright (c) 1995, 96, 97, 2000 Thomas Esken
dnl
dnl This software doesn't claim completeness, correctness or usability.
dnl On principle I will not be liable for ANY damages or losses (implicit
dnl or explicit), which result from using or handling my software.
dnl If you use this software, you agree without any exception to this
dnl agreement, which binds you LEGALLY !!
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the `GNU General Public License' as published by
dnl the `Free Software Foundation'; either version 3, or (at your option)
dnl any later version.
dnl
dnl You should have received a copy of the `GNU General Public License'
dnl along with this program; if not, write to the:

dnl
dnl Process this file with autoconf to produce a configure script for Gcal.
dnl

dnl
dnl Starting operations.
dnl

AC_INIT([gcal],
        m4_esyscmd([build-aux/git-version-gen .tarball-version]),
        [bug-gcal@gnu.org])
AC_PREREQ(2.65)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.9])

AC_CONFIG_SRCDIR([src/gcal.c])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_USE_SYSTEM_EXTENSIONS

dnl
dnl Emit some text.  (I know that some people dislike a talkative `configure'
dnl script, but those persons should really use the `--quiet' option instead
dnl of writing me that I have to remove this behavior, because I like it!)
dnl
AC_MSG_RESULT()
AC_MSG_RESULT(this is the automatical configuration step of $PACKAGE-$VERSION)
AC_MSG_RESULT()
AC_MSG_RESULT(please wait...)
AC_MSG_RESULT()


dnl
dnl Checks for C compiler.
dnl
if test -z "$CFLAGS"; then
  gcal_possibly_adjust_cflags=yes
fi
AC_MSG_RESULT()
AC_CHECKING(for C compiler)
dnl
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

gl_EARLY

AC_FUNC_ALLOCA

dnl
dnl Support transforming of program names.
dnl
AC_ARG_PROGRAM

dnl
dnl Check if the build environment is sane.
dnl
AM_SANITY_CHECK

dnl
dnl Checks for Gcal specific package options.
dnl
AC_MSG_RESULT()
AC_CHECKING(for $PACKAGE specific package options)
dnl
AC_MSG_CHECKING(whether unicode support is required)
AC_ARG_ENABLE(unicode,
  [  --enable-unicode        use unicode],
  gcal_cv_use_unicode=$enableval,
  gcal_cv_use_unicode=no)
AC_MSG_RESULT($gcal_cv_use_unicode)
if test "$gcal_cv_use_unicode" = yes; then
  AC_DEFINE([USE_UNICODE], [1], [Enable unicode.])
fi
dnl
AC_MSG_CHECKING(whether extended ASCII character set is required)
AC_ARG_ENABLE(easc,
  [  --enable-easc           use extended ASCII character set],
  gcal_cv_use_easc=$enableval,
  gcal_cv_use_easc=no)
AC_MSG_RESULT($gcal_cv_use_easc)
if test "$gcal_cv_use_easc" = yes; then
  AC_DEFINE([USE_EASC], [1], [Enable displaying using the extended ASCII character set.])
fi
dnl
AC_MSG_CHECKING(whether all available country specific holidays are required)
AC_ARG_ENABLE(cc-holidays,
  [  --disable-cc-holidays   do not use all available country specific holidays],
  gcal_cv_use_cc_holidays=$enableval,
  gcal_cv_use_cc_holidays=yes)
AC_MSG_RESULT($gcal_cv_use_cc_holidays)
if test "$gcal_cv_use_cc_holidays" = yes; then
  GCAL_HD_OBJS="\$(GCAL_HD_OBJS)"
else
  AC_DEFINE([HD_TOP20CC], [1], [Enable a small selection of country specific holidays only.])
fi
AC_SUBST(GCAL_HD_OBJS)
dnl
AC_MSG_CHECKING(whether highlighting sequences are required)
AC_ARG_ENABLE(hls,
  [  --disable-hls           do not use highlighting sequences],
  gcal_cv_use_hls=$enableval,
  gcal_cv_use_hls=yes)
AC_MSG_RESULT($gcal_cv_use_hls)
if test "$gcal_cv_use_hls" = yes; then
  AC_DEFINE([USE_HLS], [1], [Enable highlighting sequences.])
fi
dnl
AC_MSG_CHECKING(whether pager feature is required)
AC_ARG_ENABLE(pager,
  [  --disable-pager         do not use external or simple internal pager feature],
  gcal_cv_use_pager=$enableval,
  gcal_cv_use_pager=yes)
AC_MSG_RESULT($gcal_cv_use_pager)
if test "$gcal_cv_use_pager" = yes; then
  AC_DEFINE([USE_PAGER], [1], [Enable simple internal pager feature.])
fi
dnl
AC_MSG_CHECKING(whether fixed date feature is required)
AC_ARG_ENABLE(rc,
  [  --disable-rc            do not use fixed date feature],
  gcal_cv_use_rc=$enableval,
  gcal_cv_use_rc=yes)
AC_MSG_RESULT($gcal_cv_use_rc)
if test "$gcal_cv_use_rc" = yes; then
  GCAL_RC_OBJS="\$(GCAL_RC_OBJS)"
  AC_DEFINE([USE_RC], [1], [Enable fixed date feature.])
fi
AC_SUBST(GCAL_RC_OBJS)
dnl
AC_MSG_CHECKING(whether terminal libraries are required)
AC_ARG_ENABLE(term,
  [  --disable-term          do not use libncurses/libtermcap/libtermlib],
  gcal_cv_use_term=$enableval,
  gcal_cv_use_term=yes)
AC_MSG_RESULT($gcal_cv_use_term)

dnl
dnl Checks for programs.
dnl
AC_MSG_RESULT()
AC_CHECKING(for programs)
dnl
dnl Checks for make.
dnl
AC_PROG_MAKE_SET
dnl
dnl Checks for text converters/interpreters (perl, awk).
dnl
AC_CHECK_PROG(PERL, perl, perl)

AC_PROG_AWK
if test -z "$ac_cv_prog_AWK"; then
  dnl
  dnl Previous AC_PROG_AWK has already checked for `gawk'.
  dnl
  AC_CHECK_PROG(AWK, gnuawk, gnuawk)
  if test -z "$ac_cv_prog_AWK"; then
    AC_MSG_RESULT()
    AC_MSG_WARN(no automatic installation of dates files!)
    AC_MSG_WARN(Makefile target *installdates* will never be executed)
    AC_MSG_WARN(and some other targets will create useless script files.)
    AC_MSG_RESULT()
  fi
fi
dnl
dnl Checks for zdump.
dnl
AC_PATH_PROG(ZDUMP, zdump, , $PATH:\
/sbin:\
/usr/sbin:\
/usr/local/sbin:\
/usr/etc:\
/usr/etc/zoneinfo:\
/usr/lib:\
/usr/lib/zoneinfo:\
/usr/share:\
/usr/share/zoneinfo:\
/usr/share/lib:\
/usr/share/lib/zoneinfo:\
/usr/share/etc:\
/usr/share/etc/zoneinfo:\
/usr/local/etc:\
/usr/local/etc/zoneinfo:\
/usr/local/lib:\
/usr/local/lib/zoneinfo:\
/usr/local/share:\
/usr/local/share/zoneinfo:\
/usr/local/share/lib:\
/usr/local/share/lib/zoneinfo:\
/usr/local/share/etc:\
/usr/local/share/etc/zoneinfo:\
/opt/gnu/sbin:\
/opt/gnu/usr/sbin:\
/opt/gnu/usr/local/sbin:\
/opt/gnu/etc:\
/opt/gnu/etc/zoneinfo:\
/opt/gnu/lib:\
/opt/gnu/lib/zoneinfo:\
/opt/gnu/share:\
/opt/gnu/share/zoneinfo:\
/opt/gnu/share/lib:\
/opt/gnu/share/lib/zoneinfo:\
/opt/gnu/share/etc:\
/opt/gnu/share/etc/zoneinfo)
if test -z "$ac_cv_path_ZDUMP"; then
  AC_MSG_RESULT()
  AC_MSG_WARN(Ask your sysadmin for the ZONEINFO time zone dumping program!)
  AC_MSG_WARN(Makefile target *dst* will produce a useless script file.)
  AC_MSG_RESULT()
fi
if test "$gcal_cv_use_pager" = yes; then
  dnl
  dnl Checks for ``external'' pager programs (less, more, pg).
  dnl
  AC_CHECK_PROG(PAGER1, less, less)
  AC_DEFINE_UNQUOTED([PAGER1_PROG], "$PAGER1", [Enable preferred external pager program (`less').])
  AC_CHECK_PROG(PAGER2, more, more)
  AC_DEFINE_UNQUOTED([PAGER2_PROG], "$PAGER2", [Enable alternative external pager program (`more').])
  AC_CHECK_PROG(PAGER3, pg, pg)
  AC_DEFINE_UNQUOTED([PAGER3_PROG], "$PAGER3", [Enable second alternative external pager program (`pg').])
fi
dnl
dnl Checks for cp.
dnl
AC_CHECK_PROGS(CP, cp copy)
if test -n "$ac_cv_prog_CP"; then
  if test "$ac_cv_prog_CP" = cp; then
    CP="cp -p"
  else
    if test "$ac_cv_prog_CP" != "cp -p"; then
      if test "$ac_cv_prog_CP" != "copy -m"; then
        dnl
        dnl Assume `copy -m' on SCO 3.2v4 which does not have `cp -p'.
        dnl
        CP="copy -m"
      fi
    fi
  fi
  AC_SUBST(CP)
fi

dnl
dnl Checks whether `ln -s' works.
dnl
AC_PROG_LN_S


dnl
AC_PROG_GCC_TRADITIONAL
dnl
dnl Set some warning flags in CFLAGS if this environment variable was either
dnl undefined or not set before previous COMPILER test, and `gcc' is found.
dnl
if test "$gcal_possibly_adjust_cflags" = yes && test -n "$GCC"; then
  CFLAGS="$CFLAGS -Wall"
fi

dnl
dnl Checks for operating system variants.
dnl
AC_MSG_RESULT()
AC_CHECKING(for operating system variants)
dnl
AC_AIX
AC_MINIX
AC_ISC_POSIX

dnl
dnl Checks for libraries.
dnl
AC_MSG_RESULT()
AC_CHECKING(for libraries)
dnl
dnl Checks for terminal libraries.
dnl
if test "$gcal_cv_use_term" = yes; then
  gcal_ttylibs_ok=no
  gcal_libtermlib_ok=no
  TTYLIBS=
  AC_CHECK_LIB(ncurses,
    initscr,
    [TTYLIBS="-lncurses"])
  if test -z "$TTYLIBS"; then
    AC_CHECK_LIB(termcap,
      tgetent,
      [TTYLIBS="-ltermcap"])
    AC_CHECK_LIB(termlib,
      tgetent,
      [have_libtermlib=yes],
      [have_libtermlib=no])
  fi
  dnl
  dnl Solaris has termcap, but it don't work without libucb
  dnl which is broken, so we use termlib.
  dnl
  AC_MSG_CHECKING(for working terminal libraries)
  SAVE_LIBS=$LIBS
  LIBS="$LIBS $TTYLIBS"
  AC_TRY_LINK(,
    [tgetent(0); tgetflag(0); tgetnum(0); tgetstr(0);],
    [gcal_ttylibs_ok=yes])
  if test "$gcal_ttylibs_ok" = yes; then
    AC_DEFINE([HAVE_TTYLIBS], [1], [Link -lncurses or -ltermcap or -ltermlib to executable.])
    AC_MSG_RESULT(using $TTYLIBS)
  else
    if test "$TTYLIBS" = "-lncurses"; then
      AC_MSG_RESULT(using $TTYLIBS fails)
      LIBS="$SAVE_LIBS"
      AC_CHECK_LIB(termcap,
        tgetent,
        [TTYLIBS="-ltermcap"])
      AC_MSG_CHECKING(for working terminal libraries again)
      LIBS="$LIBS $TTYLIBS"
      AC_TRY_LINK(,
        [tgetent(0); tgetflag(0); tgetnum(0); tgetstr(0);],
        [gcal_ttylibs_ok=yes])
      if test "$gcal_ttylibs_ok" = yes; then
        AC_DEFINE([HAVE_TTYLIBS], [1], [Link -lncurses or -ltermcap or -ltermlib to executable.])
        AC_MSG_RESULT(using $TTYLIBS)
      else
        AC_MSG_RESULT(using $TTYLIBS fails)
        LIBS="$SAVE_LIBS"
        AC_CHECK_LIB(termlib,
          tgetent,
          [have_libtermlib=yes],
          [have_libtermlib=no])
        TTYLIBS="-ltermlib"
        LIBS="$LIBS $TTYLIBS"
        if test "$have_libtermlib" = yes; then
          AC_MSG_CHECKING(for working terminal libraries again)
          AC_TRY_LINK(,
            [tgetent(0); tgetflag(0); tgetnum(0); tgetstr(0);],
            [gcal_libtermlib_ok=yes])
          if test "$gcal_libtermlib_ok" = yes; then
            AC_DEFINE([HAVE_TTYLIBS], [1], [Link -lncurses or -ltermcap or -ltermlib to executable.])
            AC_MSG_RESULT(using $TTYLIBS)
          fi
        fi
      fi
    else
      if test "$have_libtermlib" = yes; then
        TTYLIBS="-ltermlib"
        LIBS="$SAVE_LIBS $TTYLIBS"
        AC_TRY_LINK(,
          [tgetent(0); tgetflag(0); tgetnum(0); tgetstr(0);],
          [gcal_libtermlib_ok=yes])
        if test "$gcal_libtermlib_ok" = yes; then
            AC_DEFINE([HAVE_TTYLIBS], [1], [Link -lncurses or -ltermcap or -ltermlib to executable.])
          AC_MSG_RESULT(using $TTYLIBS)
        fi
      fi
    fi
  fi
  if test "$gcal_ttylibs_ok" = no && test "$gcal_libtermlib_ok" = no; then
    LIBS="$SAVE_LIBS"
    AC_MSG_RESULT(hmm... whats this??)
    AC_MSG_RESULT()
    AC_MSG_WARN(no terminal libraries available)
    AC_MSG_WARN()
    AC_MSG_WARN(this means $PACKAGE-$VERSION will use its defaults now)
    AC_MSG_WARN(and is unable to support several terminal types!)
    AC_MSG_WARN(note: $PACKAGE-$VERSION would work more flexible if libncurses.a)
    AC_MSG_WARN(libtermcap.a or libtermlib.a are installed.)
    AC_MSG_RESULT()
  fi
fi

dnl
dnl Check for mathematical library.
dnl
AC_CHECK_LIB(m,
  cos,
  [LIBM="-lm"
   AC_DEFINE([HAVE_LIBM], [1], [Define if you have the m library (-lm)])
   have_libm=yes],
  [have_libm=no])
AC_SUBST(LIBM)

dnl
dnl Checks for header files.
dnl
AC_MSG_RESULT()
AC_CHECKING(for header files)
dnl
AC_STDC_HEADERS
AC_CHECK_HEADERS(stdio.h stdlib.h string.h unistd.h ctype.h errno.h limits.h)
AC_CHECK_HEADERS(assert.h signal.h termio.h termios.h sgtty.h langinfo.h)

AC_MSG_CHECKING([for _NL_TIME_FIRST_WEEKDAY])
AC_TRY_LINK([#include <langinfo.h>], [
char c;
c = *((unsigned char *) nl_langinfo(_NL_TIME_FIRST_WEEKDAY));
], nl_res=yes, nl_res=no)
AC_MSG_RESULT($nl_res)
if test "$nl_res" = "yes"; then
AC_DEFINE([HAVE__NL_TIME_FIRST_WEEKDAY], [1],
[Define if _NL_TIME_FIRST_WEEKDAY is available])
fi

if test "$gcal_cv_use_term" = yes; then
  if test "$gcal_ttylibs_ok" = yes || test "$gcal_libtermlib_ok" = yes; then
    AC_CHECK_HEADERS(termcap.h)
  fi
fi
if test "$have_libm" = yes; then
  AC_CHECK_HEADERS(math.h)
fi
if test "$gcal_cv_use_included_regexps" = no; then
  AC_CHECK_HEADERS(regex.h)
fi
if test "$gcal_cv_use_pager" = yes; then
  AC_CHECK_HEADERS(fcntl.h)
  AC_HEADER_SYS_WAIT
fi
AC_CHECK_HEADERS(sys/ioctl.h sys/stream.h sys/ptem.h sys/stat.h sys/types.h sys/time.h)

dnl
dnl Checks for typedefs and structures.
dnl
AC_MSG_RESULT()
AC_CHECKING(for typedefs and structures)
dnl
if test "$gcal_cv_use_pager" = yes; then
  AC_TYPE_PID_T
fi
AC_TYPE_SIGNAL
AC_HEADER_TIME
AC_MSG_CHECKING(for time_t)
AC_CACHE_VAL(gcal_cv_type_time_t,
  AC_TRY_COMPILE([
#    if TIME_WITH_SYS_TIME
#      include <sys/time.h>
#      include <time.h>
#    else
#      if HAVE_SYS_TIME_H
#        include <sys/time.h>
#      else
#        include <time.h>
#      endif
#    endif],
    [time_t t = 0;],
    [gcal_cv_type_time_t=yes],
    [gcal_cv_type_time_t=no]))
if test "$gcal_cv_type_time_t" = yes; then
  AC_DEFINE([HAVE_TIME_T], [1], [Type time_t is predefined.])
fi
AC_MSG_RESULT($gcal_cv_type_time_t)

dnl
dnl Checks for compiler characteristics.
dnl
AC_MSG_RESULT()
AC_CHECKING(for compiler characteristics)
dnl
AC_C_CONST
dnl
AC_MSG_CHECKING(for void)
AC_CACHE_VAL(gcal_cv_c_void,
  AC_TRY_COMPILE(,
    [void *foo = 0;],
    [gcal_cv_c_void=yes],
    [gcal_cv_c_void=no]))
if test "$gcal_cv_c_void" = yes; then
  AC_DEFINE([HAVE_VOID], [1], [Identifier void exists.])
fi
AC_MSG_RESULT($gcal_cv_c_void)

dnl
dnl Checks for library functions.
dnl
AC_MSG_RESULT()
AC_CHECKING(for library functions)
dnl
if test "$gcal_cv_use_pager" = yes; then
  AC_CHECK_FUNCS([dup dup2])
fi
AC_CHECK_FUNCS([signal strtol strstr strchr strrchr strcspn strcasecmp strncasecmp setlocale])
dnl
dnl Some systems have termios.h but not the corresponding functions.
dnl
AC_CHECK_FUNC(tcgetattr, AC_DEFINE([HAVE_TERMIOS_FUNCS], [1], [All termios functions are available.]))
dnl
dnl Check for working system() function.
dnl
AC_MSG_CHECKING(for working system function)
AC_CACHE_VAL(gcal_cv_func_system_ok,
  AC_TRY_RUN([
#    if STDC_HEADERS
#      include <stdlib.h>
#    endif
    int main()
    {
      int i;
      i = system(NULL);
      if (!i)
        exit(1);
      exit(0);
    }],
    gcal_cv_func_system_ok=yes,
    gcal_cv_func_system_ok=no,
    gcal_cv_func_system_ok=no))
AC_MSG_RESULT($gcal_cv_func_system_ok)
if test "$gcal_cv_func_system_ok" = yes; then
  AC_DEFINE([HAVE_SYSTEM], [1], [Define if you have a working system function])
fi
if test "$gcal_cv_use_included_regexps" = no; then
  dnl
  dnl Checks for regular expression functions.
  dnl
  have_regexp=no
  AC_MSG_RESULT()
  AC_CHECKING(for regular expression functions)
  dnl
  dnl Are the preferred GNU regexp functions available?
  dnl
  AC_CHECK_FUNC(re_compile_pattern,
    AC_MSG_RESULT(using GNU re_compile_pattern)
    AC_DEFINE([HAVE_GNU_RE_COMPILE_PATTERN], [1], [GNU re_compile_pattern() and regex.h])
    have_regexp=yes)
  if test "$have_regexp" = no; then
    have_posix_regexp=unknown
    dnl
    dnl Some versions of Solaris have a regcomp() function, but it doesn't work!
    dnl So we run a test program.  If we're cross-compiling, do it the old way.
    dnl
    AC_TRY_RUN([
#      if HAVE_SYS_TYPES_H
#        include <sys/types.h>
#      endif
#      if HAVE_REGEX_H
#        include <regex.h>
#      endif
      int main()
      {
        regex_t r;
        regmatch_t rm;
        if (regcomp(&r, "abc", 0))
          exit(1);
        if (regexec(&r, "xabcy", 1, &rm, 0))
          exit(1);
        exit(0);
      }],
      have_posix_regexp=yes,
      have_posix_regexp=no,
      have_posix_regexp=unknown)
    if test "$have_posix_regexp" = yes; then
      AC_MSG_RESULT(using POSIX regcomp)
      AC_DEFINE([HAVE_POSIX_REGCOMP], [1], [POSIX regcomp() and regex.h])
      have_regexp=yes
    else
      if test "$have_posix_regexp" = unknown; then
        AC_TRY_LINK([
#          if HAVE_SYS_TYPES_H
#            include <sys/types.h>
#          endif
#          if HAVE_REGEX_H
#            include <regex.h>
#          endif],
          [regex_t *r;
           regfree(r);],
          AC_MSG_RESULT(using POSIX regcomp)
          AC_DEFINE([HAVE_POSIX_REGCOMP], [1], [POSIX regcomp() and regex.])
          have_regexp=yes)
      fi
    fi
    if test "$have_regexp" = no; then
      AC_CHECK_FUNC(re_comp,
        AC_MSG_RESULT(using BSD re_comp)
        AC_DEFINE([HAVE_RE_COMP], [1], [BSD re_comp()])
        have_regexp=yes)
    fi
  fi
fi
dnl
dnl Checks for ctype functions.
dnl
AC_MSG_RESULT()
AC_CHECKING(for ctype library functions)
dnl
AC_MSG_CHECKING(for upper/lower)
AC_CACHE_VAL(gcal_cv_func_upper_lower,
  AC_TRY_LINK([
#    if HAVE_CTYPE_H
#      include <ctype.h>
#    endif],
    [static int x; x = tolower(x); x = toupper(x); x = islower(x); x = isupper(x);],
    [gcal_cv_func_upper_lower=yes],
    [gcal_cv_func_upper_lower=no]))
if test "$gcal_cv_func_upper_lower" = yes; then
  AC_DEFINE([HAVE_UPPER_LOWER], [1], [Functions/macros toupper(), tolower(), isupper() and islower() are found.])
fi
AC_MSG_RESULT($gcal_cv_func_upper_lower)
dnl
AC_MSG_CHECKING(for isdigit)
AC_CACHE_VAL(gcal_cv_func_isdigit,
  AC_TRY_LINK([
#    if HAVE_CTYPE_H
#      include <ctype.h>
#    endif],
    [static int x; x = isdigit(x);],
    [gcal_cv_func_isdigit=yes],
    [gcal_cv_func_isdigit=no]))
if test "$gcal_cv_func_isdigit" = yes; then
  AC_DEFINE([HAVE_ISDIGIT], [1], [Function/macro isdigit() is found.])
fi
AC_MSG_RESULT($gcal_cv_func_isdigit)
dnl
AC_MSG_CHECKING(for isalpha)
AC_CACHE_VAL(gcal_cv_func_isalpha,
  AC_TRY_LINK([
#    if HAVE_CTYPE_H
#      include <ctype.h>
#    endif],
    [static int x; x = isalpha(x);],
    [gcal_cv_func_isalpha=yes],
    [gcal_cv_func_isalpha=no]))
if test "$gcal_cv_func_isalpha" = yes; then
  AC_DEFINE([HAVE_ISALPHA], [1], [Have isalpha])
fi
AC_MSG_RESULT($gcal_cv_func_isalpha)
dnl
AC_MSG_CHECKING(for isalnum)
AC_CACHE_VAL(gcal_cv_func_isalnum,
  AC_TRY_LINK([
#    if HAVE_CTYPE_H
#      include <ctype.h>
#    endif],
    [static int x; x = isalnum(x);],
    [gcal_cv_func_isalnum=yes],
    [gcal_cv_func_isalnum=no]))
if test "$gcal_cv_func_isalnum" = yes; then
  AC_DEFINE([HAVE_ISALNUM], [1], [Have isalnum])
fi
AC_MSG_RESULT($gcal_cv_func_isalnum)
dnl
AC_MSG_CHECKING(for isspace)
AC_CACHE_VAL(gcal_cv_func_isspace,
  AC_TRY_LINK([
#    if HAVE_CTYPE_H
#      include <ctype.h>
#    endif],
    [static int x; x = isspace(x);],
    [gcal_cv_func_isspace=yes],
    [gcal_cv_func_isspace=no]))
if test "$gcal_cv_func_isspace" = yes; then
  AC_DEFINE([HAVE_ISSPACE], [1], [Have isspace])
fi
AC_MSG_RESULT($gcal_cv_func_isspace)

dnl
dnl Checks for external variable `ospeed' in the ncurses/termcap/termlib library.
dnl
if test "$gcal_cv_use_term" = yes; then
  if test "$gcal_ttylibs_ok" = yes || test "$gcal_libtermlib_ok" = yes; then
    AC_MSG_RESULT()
    AC_CHECKING(for external variables)
    dnl
    AC_MSG_CHECKING(terminal library for ospeed)
    AC_CACHE_VAL(gcal_cv_var_ospeed_test_1,
      AC_TRY_LINK([
#        if HAVE_SYS_TYPES_H
#          include <sys/types.h>
#        endif
#        if HAVE_TERMIOS_H
#          include <termios.h>
#        endif
#        if HAVE_TERMCAP_H
#          include <termcap.h>
#        endif],
        [ospeed = 0;],
        [gcal_cv_var_ospeed_test_1=yes],
        [gcal_cv_var_ospeed_test_1=no]))
    if test "$gcal_cv_var_ospeed_test_1" = no; then
      AC_CACHE_VAL(gcal_cv_var_ospeed_test_2,
        AC_TRY_LINK(,
          [extern short ospeed; ospeed = 0;],
          [gcal_cv_var_ospeed_test_2=yes],
          [gcal_cv_var_ospeed_test_2=no]))
    fi
    if test "$gcal_cv_var_ospeed_test_1" = yes || test "$gcal_cv_var_ospeed_test_2" = yes; then
      AC_DEFINE([HAVE_OSPEED], [1], [The ospeed variable is defined in -lncurses or -ltermcap or -ltermlib])
      if test "$gcal_cv_var_ospeed_test_2" = yes; then
        AC_DEFINE([MUST_DCL_OSPEED], [1], [We must declare the ospeed variable])
      fi
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
    fi
  fi
fi

dnl
dnl Checks for system services.
dnl
AC_MSG_RESULT()
AC_CHECKING(for system services)
dnl
AC_EXEEXT
AC_OBJEXT
AC_SYS_INTERPRETER
if test "$ac_cv_sys_interpreter" = yes; then
  AC_DEFINE([HAVE_SYS_INTERPRETER], [1], [Shell can interpret `#!' in first line of a shell script.])
fi

AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT([external])

gl_INIT

dnl
dnl Ending operations.
dnl
AC_SUBST(CFLAGS)dnl
dnl
changequote(<<, >>)dnl
dnl
dnl Transform the `$srcdir/doc/mks?sed.in' SED scripts while copying
dnl because some dumb SEDs cannot handle comments.
dnl
if test -s "$srcdir/doc/mks1sed.in"; then
  sed -e '/^#.*[^\\]$/d' -e '/^#$/d' $srcdir/doc/mks1sed.in > $srcdir/doc/mksamp-1.sed
fi
if test -s "$srcdir/doc/mks2sed.in"; then
  sed -e '/^#.*[^\\]$/d' -e '/^#$/d' $srcdir/doc/mks2sed.in > $srcdir/doc/mksamp-2.sed
fi
dnl
changequote([, ])dnl
dnl
AC_MSG_RESULT()

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT([Makefile \
           install-inf \
           data/Makefile \
           doc/Makefile \
           doc/en/Makefile \
           doc/en/examples/Makefile \
           doc/mksamp \
           lib/Makefile \
           misc/Makefile \
           misc/daily/daily \
           misc/ddiff/ddiff \
           misc/ddiff/ddiffdrv \
           misc/dst/dst \
           misc/gcalltx/gcalltx \
           misc/gcalltx/gcalltxp \
           misc/moon/moon \
           misc/mrms/mrms \
           misc/srss/srss \
           misc/wloc/wlocdrv \
           po/Makefile.in \
           src/Makefile])dnl

dnl
dnl Emit some final text.
dnl
AC_MSG_RESULT()
AC_MSG_RESULT(Bored? Fallen asleep??   All checks done!)
AC_MSG_RESULT()
AC_MSG_RESULT(Perhaps you might want to inspect the created)
AC_MSG_RESULT(Makefiles now for tuning some settings...)
AC_MSG_RESULT()
