#
# Automakefile for the enscript documentation.
# Copyright (c) 1998-1999 Markku Rossi
#
# Author: Markku Rossi <mtr@iki.fi>
#

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
#
# along with Enscript.  If not, see <http://www.gnu.org/licenses/>.
#

info_TEXINFOS = enscript.texi
enscript_TEXINFOS = fdl-1.3.texi

man_MANS = enscript.1 states.1
CLEANFILES = $(man_MANS)

EXTRA_DIST = ChangeLog.old enscript.man states.man FAQ.html	\
	clean-nroff.pl texinfo.tex

enscript.man.html: enscript.1
	nroff -man $(srcdir)/enscript.1 | ./clean-nroff.pl > enscript.tmp
	enscript -E --color -q -Whtml -penscript.man.html enscript.tmp
	rm -f enscript.tmp

SUFFIXES = .1 .man

.man.1:
	sed 's%@DATADIR@%$(datadir)%g; s%@media@%@MEDIA@%g; s%@SYSCONFDIR@%$(sysconfdir)%g' \
	$< > $@
