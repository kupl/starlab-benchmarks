#
# Makefile template for the compat libaray.
# Copyright (c) 1995-1999 Markku Rossi.
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

AUTOMAKE_OPTIONS = no-dependencies

noinst_LIBRARIES = libcompat.a
libcompat_a_SOURCES = getopt.c getopt1.c xalloc.c regex.c
libcompat_a_LIBADD = @LIBOBJS@ @ALLOCA@
libcompat_a_DEPENDENCIES = @LIBOBJS@ @ALLOCA@

libcompat_a_SOURCES_windelta =	\
	+../w32/gethostname.c	\
	+../w32/getpwd.c	\
	+../w32/getuid.c

noinst_HEADERS = getopt.h regex.h xalloc.h

EXTRA_DIST = ChangeLog.old strerror.c memmove.c memcpy.c strtol.c	\
	strtoul.c gettext.h

INCLUDES = -I$(srcdir)/../intl -I../intl

INCLUDES_windelta = +-I../w32
