#
# Automakefile for the library files.
# Copyright (c) 1995-1998 Markku Rossi
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

pkgdata_DATA = ascii.enc asciifise.enc asciidkno.enc hp8.enc ibmpc.enc	\
koi8.enc 88591.enc 88592.enc 88593.enc 88594.enc 88595.enc 88597.enc	\
88599.enc 885910.enc mac.enc ps.enc pslatin1.enc vms.enc a2ps.hdr	\
edd.hdr emacs.hdr enscript.hdr enscript-color.hdr frame.hdr mp.hdr	\
simple.hdr squeeze.hdr enscript.pro

sysconf_DATA = enscript.cfg
CLEANFILES = $(sysconf_DATA)

EXTRA_DIST = ChangeLog.old enscript.cfg.in $(pkgdata_DATA)	\
	Makefile-encodings make-encoding.pl

do_subst = sed -e 's%@DATADIR@%$(datadir)%g' \
               -e 's%@media@%@MEDIA@%g' \
	       -e 's%@BINDIR@%$(bindir)%g' \
	       -e 's%@spooler@%@SPOOLER@%g' \
	       -e 's%@pslevel@%@PSLEVEL@%g'

enscript.cfg: enscript.cfg.in Makefile
	$(do_subst) < $(srcdir)/enscript.cfg.in > enscript.cfg

