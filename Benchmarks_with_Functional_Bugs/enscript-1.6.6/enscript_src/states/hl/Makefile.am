#
# Automakefile for the highlighting rules.
# Copyright (c) 1998-2003 Markku Rossi
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

misc = enscript.st describe_languages.st default_faces.st

styles = style_emacs.st style_emacs_verbose.st style_ifh.st	\
style_a2ps.st style_msvc.st

languages = lang_ansi.st lang_enscript.st lang_html.st	\
lang_overstrike.st lang_rtf.st lang_texinfo.st

highlightings = ada.st asm.st awk.st bash.st c.st c_comment.st		\
c_ppline.st c_string.st changelog.st cpp.st csh.st delphi.st diff.st	\
diffs.st diffu.st elisp.st fortran.st fortran_pp.st f90.st haskell.st html.st	\
idl.st inf.st java.st javascript.st ksh.st m4.st mail.st makefile.st	\
matlab.st nroff.st objc.st outline.st pascal.st passthrough.st perl.st	\
postscript.st python.st pyrex.st rfc.st scheme.st sh.st skill.st	\
sql.st states.st synopsys.st tcl.st tcsh.st tex.st vba.st verilog.st	\
vhdl.st vrml.st wmlscript.st zsh.st

hldir = $(pkgdatadir)/hl
dist_hl_DATA = $(misc) $(styles) $(languages) $(highlightings)

EXTRA_DIST = ChangeLog.old