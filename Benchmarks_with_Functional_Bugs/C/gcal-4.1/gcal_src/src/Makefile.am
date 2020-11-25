# GNU gcal
#
# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2013 Free
# Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
MISC_LIBS = @LIBM@ @LIBS@
LIBS = $(MISC_LIBS)

AM_CFLAGS = -I$(top_srcdir)/lib -I$(srcdir) -I.. $(CPPFLAGS)
LDADD = ../lib/libgnu.a $(LIBINTL)

SRCS = \
  file-io.c \
  gcal.c \
  hd-astro.c \
  hd-data.c \
  hd-data1.c \
  hd-data2.c \
  hd-data3.c \
  hd-data4.c \
  hd-data5.c \
  hd-use.c \
  help.c \
  print.c \
  rc-astro.c \
  rc-check.c \
  rc-insert.c \
  rc-use.c \
  rc-utils.c \
  tty.c \
  utils.c \
  tcal.c \
  txt2gcal.c \
  gcal2txt.c

COMMON_HDRS = \
  common.h \
  defines.h \
  tailor.h

GCAL_BASE_HDRS = \
  file-io.h \
  gcal.h \
  globals.h \
  hd-astro.h \
  hd-data.h \
  hd-data1.h \
  hd-defs.h \
  hd-use.h \
  help.h \
  print.h \
  tty.h \
  utils.h

GCAL_HD_HDRS = \
  hd-data2.h \
  hd-data3.h \
  hd-data4.h \
  hd-data5.h

GCAL_RC_HDRS = \
  rc-astro.h \
  rc-check.h \
  rc-defs.h \
  rc-insert.h \
  rc-use.h \
  rc-utils.h

GCAL2TXT_HDRS = \
  gcal2txt.h

TCAL_HDRS = \
  tcal.h

TXT2GCAL_HDRS = \
  txt2gcal.h

HDRS = \
  $(COMMON_HDRS) \
  $(GCAL_BASE_HDRS) \
  $(GCAL_HD_HDRS) \
  $(GCAL_RC_HDRS) \
  $(GCAL2TXT_HDRS) \
  $(TCAL_HDRS) \
  $(TXT2GCAL_HDRS)

GCAL_BASE_SRCS = \
  gcal.c \
  file-io.c \
  hd-astro.c \
  hd-data.c \
  hd-data1.c \
  hd-use.c \
  help.c \
  print.c \
  tty.c \
  utils.c

GCAL_HD_SRCS = \
  hd-data2.c \
  hd-data3.c \
  hd-data4.c \
  hd-data5.c

GCAL_RC_SRCS = \
  rc-astro.c \
  rc-check.c \
  rc-insert.c \
  rc-use.c \
  rc-utils.c

GCAL2TXT_SRCS = \
  gcal2txt.c

TCAL_SRCS = \
  tcal.c

TXT2GCAL_SRCS = \
  txt2gcal.c

bin_PROGRAMS = gcal gcal2txt tcal txt2gcal

gcal_SOURCES = $(GCAL_BASE_SRCS) $(GCAL_RC_SRCS) $(GCAL_HD_SRCS)

gcal2txt_SOURCES = $(GCAL2TXT_SRCS)

tcal_SOURCES =  $(TCAL_SRCS)

txt2gcal_SOURCES = $(TXT2GCAL_SRCS)

EXTRA_DIST = common.h \
	gcal2txt.h \
	hd-data1.h \
	hd-data5.h \
	print.h \
	rc-insert.h \
	tailor.h \
	utils.h \
	defines.h \
	globals.h \
	hd-data2.h \
	hd-defs.h \
	rc-astro.h \
	rc-use.h \
	tcal.h \
	file-io.h \
	hd-astro.h \
	hd-data3.h \
	hd-use.h \
	rc-check.h \
	rc-utils.h \
	tty.h \
	gcal.h \
	hd-data.h \
	hd-data4.h \
	help.h \
	rc-defs.h \
	txt2gcal.h
