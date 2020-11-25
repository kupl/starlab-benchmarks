dnl Process this file with autoconf to produce a configure script.
AC_INIT([GNU Enscript],[1.6.6],[bug-enscript@gnu.org])
AC_PREREQ([2.61])
AC_CONFIG_SRCDIR([src/gsint.h])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

AC_PROG_INSTALL

AC_PROG_CC

AC_USE_SYSTEM_EXTENSIONS
AM_C_PROTOTYPES

AC_C_CONST
AC_FUNC_ALLOCA

AC_HEADER_STDC
AC_CHECK_HEADERS([string.h stdlib.h unistd.h stdarg.h math.h pwd.h])
AC_CHECK_HEADERS([sys/types.h sys/stat.h])

dnl Check some functions.
AC_CHECK_FUNCS(strchr getcwd)
AC_REPLACE_FUNCS(strerror memmove memcpy strtoul)
AC_FUNC_VPRINTF

dnl -lsun is needed on IRIX-4.0 to get the user passwd entry through
dnl the yellow pages / NIS.
AC_CHECK_LIB(sun, main)

dnl Find out where the gethostname() is.
AC_CHECK_FUNC(gethostname, , [
  AC_CHECK_LIB(nsl, gethostname, LIBS="$LIBS -lnsl", [
    AC_CHECK_LIB(socket, gethostname)
  ])
])

AC_PROG_RANLIB
AC_PROG_YACC
AM_PROG_LEX

AC_ARG_ENABLE(debug,
[  --enable-debug	  enable debugging], [
  echo enabling debugging
  if test -n "$GCC"; then
    CFLAGS="$CFLAGS -Wall"
  fi
])

dnl Define the default output media.

AC_ARG_WITH(media,
[  --with-media(=MEDIA)	  use output media Letter (MEDIA), default is A4],
  if test "X$withval" != "Xno"; then
    if test "X$withval" = "Xyes"; then
      ac_cv_value_media='Letter'
    else
      ac_cv_value_media=$withval
    fi
  else
    ac_cv_value_media=${MEDIA-A4}
  fi,
  ac_cv_value_media=${MEDIA-A4}
)
MEDIA=$ac_cv_value_media
AC_SUBST(MEDIA)

dnl Find the printer spooler command.

AC_CHECK_PROG(SPOOLER, lpr, lpr)
AC_CHECK_PROG(SPOOLER, lp, lp)

dnl user can overwrite this
AC_ARG_WITH(spooler,
[  --with-spooler=SPOOLER  set the printer spooler command],
  if test "X$withval" != "Xno"; then
    SPOOLER=$withval
  fi
)

dnl If SPOOLER is still empty, set it to `lpr'.  After this, we should
dnl be able to compile enscript on HURD.
if test "X$SPOOLER" = "X"; then
  SPOOLER=lpr
fi

dnl The PostScript language level.
PSLEVEL="2"
AC_ARG_WITH(ps-level,
[  --with-ps-level=LEVEL   set the PostScript language level to LEVEL],
  if test "X$withval" != "Xno"; then
    PSLEVEL=$withval
  fi
)
AC_SUBST(PSLEVEL)

dnl Internationalization.
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.17])

dnl Path separator character.
AC_DEFINE_UNQUOTED([PATH_SEPARATOR], [':'],
  [Character used to separate path components.])
AC_DEFINE_UNQUOTED([PATH_SEPARATOR_STR], [":"],
  [Character used to separate path components, as string.])

dnl
dnl Scripts
dnl

dnl diffpp

AC_PATH_PROG(PERL, perl)

dnl substitutions

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile  po/Makefile.in intl/Makefile po/Makefile \
	compat/Makefile \
	afm/Makefile \
	afmlib/Makefile \
	src/Makefile \
	src/tests/Makefile \
	lib/Makefile \
	scripts/Makefile \
	states/Makefile \
	states/hl/Makefile \
	states/tests/Makefile \
	docs/Makefile \
	w32/Makefile])

AC_OUTPUT

echo ""

echo "Enscript is now configured to your system with the following"
echo "user-definable options.  Please, check that they are correct and"
echo "match to your system's properties."
echo ""
echo "Option     Change with configure's option   Current value"
echo "---------------------------------------------------------"
echo "Media      --with-media=MEDIA               $MEDIA"
echo "Spooler    --with-spooler=SPOOLER           $SPOOLER"
echo "PS level   --with-ps-level=LEVEL            $PSLEVEL"
echo ""
