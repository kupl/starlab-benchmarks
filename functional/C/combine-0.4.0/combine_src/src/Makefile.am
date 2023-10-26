## Process this file with automake to produce Makefile.in -*-Makefile-*-
AUTOMAKE_OPTIONS = ansi2knr
SUBDIRS = combine_scm

bin_PROGRAMS = combine

noinst_HEADERS = df_global.h dstring.h field.h \
  hash.h iguile.h index.h df_options.h process_files.h \
  read_files.h write_files.h df_common.h
EXTRA_DIST = util.scm

localedir = $(datadir)/locale

INCLUDES = -I$(top_srcdir) -I$(srcdir) -I$(top_srcdir)/intl -I$(top_srcdir)/lib -I$(includedir)
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

# Sometimes, the expansion of @LIBINTL@ includes -lc which may
# include modules defining variables like `optind', so libfetish.a
# must precede @LIBINTL@ in order to ensure we use GNU getopt.
# But libfetish.a must also follow @LIBINTL@, since libintl uses
# replacement functions defined in libfetish.a.
##LDADD = ../lib/libfetish.a @LIBINTL@ $(top_builddir)/lib/libfetish.a

GUILE_CFLAGS = @GUILE_CFLAGS@
GUILE_LDFLAGS = @GUILE_LDFLAGS@
LDADD = -L$(top_srcdir)/lib
combine_SOURCES = df_global.c dstring.c field.c \
  hash.c iguile.c index.c main.c makeindex.c df_options.c process_files.c \
  read_files.c write_files.c
combine_LDADD = $(top_builddir)/lib/libdf.a $(LDADD) -L$(libdir)   

## $(PROGRAMS): $(top_builddir)/lib/libfetish.a

## If necessary, add -lm to resolve use of pow in lib/strtod.c.

