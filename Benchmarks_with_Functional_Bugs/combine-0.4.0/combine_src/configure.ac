#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(combine, 0.4.0, bug-combine@gnu.org)
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h])
AM_C_PROTOTYPES

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_RANLIB
AC_PROG_YACC
AC_PROG_CPP

# Checks for libraries.
AC_CHECK_LIB([guile], [gh_scm2bool])
AC_CHECK_LIB([intl], [gettext])


# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([argz.h fcntl.h float.h inttypes.h langinfo.h libintl.h limits.h locale.h malloc.h stddef.h stdint.h stdio_ext.h stdlib.h string.h strings.h sys/param.h unistd.h wchar.h libguile.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t [long long]])

# Checks for library functions.
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION([0.18.1])
AC_FUNC_MALLOC
AC_FUNC_MBRTOWC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify atexit bzero getcwd localeconv mblen mbrlen memchr memmove mempcpy memset munmap nl_langinfo pow setlocale stpcpy strcasecmp strchr strcspn strdup strerror strncasecmp strsep strstr strtol strtoul])


# Guile!
# Checking on the variable set by looking for the guile header check.
if test "x$ac_cv_header_libguile_h" = "xyes"; then :
  GUILE_FLAGS
  GUILE_SITE_DIR
  fi # have guile

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 intl/Makefile
                 po/Makefile.in
                 lib/Makefile
                 m4/Makefile
                 src/Makefile
                 src/combine_scm/Makefile
                 tests/Makefile])
AC_OUTPUT
