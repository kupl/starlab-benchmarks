#
# Makefile for states.
# Copyright (c) 1997-2003 Markku Rossi
#
# Author: Markku Rossi <mtr@iki.fi>
#

#
# This file is part of GNU Enscript.
#
# Enscript is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Enscript is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Enscript.  If not, see <http://www.gnu.org/licenses/>.
#

SUBDIRS = hl tests

AUTOMAKE_OPTIONS =

bin_PROGRAMS = states

states_SOURCES = main.c utils.c prims.c process.c gram.y lex.l
states_SOURCES_windelta = \
	-gram.y	\
	+gram.c	\
	-lex.l	\
	+lex.c

LDADD = ../afmlib/libafm.a @LIBINTL@ ../compat/libcompat.a @LEXLIB@
states_DEPENDENCIES = ../compat/libcompat.a

noinst_HEADERS = defs.h gram.h

EXTRA_DIST = ChangeLog.old TODO over.in enscript.el

localedir = $(datadir)/locale

bin_SCRIPTS = over
CLEANFILES = $(bin_SCRIPTS)

over: $(srcdir)/over.in
	sed 's%@LIBRARYDIR@%$(datadir)/enscript%g' \
	$(srcdir)/over.in > over

AM_YFLAGS = -d

INCLUDES = -I$(srcdir)/../compat -I$(srcdir)/../afmlib	\
-I$(srcdir)/../intl -I../intl -DLOCALEDIR=\"$(localedir)\"

INCLUDES_windelta = 	\
	+-I../w32	\
	--DLOCALEDIR=\"$(localedir)\"
