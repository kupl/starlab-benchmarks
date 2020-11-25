@echo off
::  $Id: ddiffdrv.bat 0.04 2000/01/12 00:00:04 tom Exp $
::
::  ddiffdrv.bat:  First step:
::                   Uses `ddiffdrv.rc' or any other Gcal location pool
::                   resource file for creating all Gcal location response
::                   files which can be generated from that file.
::                   If no Gcal location pool resource file exists, this
::                   script tries to create the `ddiffdrv.rc' file by
::                   processing the ZONE file `zone.tab' by means of GAWK
::                   and the `ddiffdrv.awk' script.
::                   A Gcal location response file is a Gcal response file
::                   consisting of one line, which passes the location name
::                   LOC, its country code CC, and the ISO-6709-co-ordinate
::                   COORD via text variables to Gcal.  The line template of
::                   a Gcal location response file is:
::                     -r$l=CC-LOC:$c=COORD
::                 Second step (only if the `--create' option is given):
::                   Creation of all Gcal location resource files, which
::                   contain the day/night lengths and differences for all
::                   days of the year YYYY for the location.
::                 Third step (only if the `--create' option is given):
::                   Creation of the Gcal include file `ddiff.rc', which
::                   contains include statements of all Gcal location
::                   resource files created.
::
::                 Usage: ddiffdrv --help
::                 Usage: ddiffdrv [--create] [YYYY] [-- LOCATION-POOL-RESOURCE-FILE]
::
::  MS-DOS solution.
::
::  Needs `gcal' and `gawk' for processing!
::
::  Copyright (c) 2000  Thomas Esken      <esken@uni-muenster.de>
::                      Im Hagenfeld 84
::                      D-48147 M"unster
::                      GERMANY
::
::  This software doesn't claim completeness, correctness or usability.
::  On principle I will not be liable for ANY damages or losses (implicit
::  or explicit), which result from using or handling my software.
::  If you use this software, you agree without any exception to this
::  agreement, which binds you LEGALLY !!
::
::  This program is free software; you can redistribute it and/or modify
::  it under the terms of the `GNU General Public License' as published by
::  the `Free Software Foundation'; either version 3, or (at your option)
::  any later version.
::
::  You should have received a copy of the `GNU General Public License'
::  along with this program; if not, write to the:
::
::
::
:: Basically used programs, set these environment variables properly!
::
set _=gcal
set oldgcalpath=%gcalpath%
set gcalpath=.
set -=gawk
set oldawkpath=%awkpath%
set awkpath=.;c:\dos
::
:: Generic file names, set these environment variables properly!
::
set myname=%0
set mydefaultname=ddiffdrv
::
:: Script file name, set this environment variable properly!
::
set runscriptname=ddiff
::
:: Input file names, set these environment variables properly!
::
set pool=zone.tab
set awkscript=%mydefaultname%.awk
::
:: Output file names, set these environment variables properly!
::
set gcalrcsuffix=.rc
set outfile1=%mydefaultname%%gcalrcsuffix%
set outfile2=%runscriptname%%gcalrcsuffix%
::
set create=no
::
if %1"" == "" goto default
if %1 == --help goto usage
if %1 == --HELP goto usage
if %1 == --hel goto usage
if %1 == --HEL goto usage
if %1 == --he goto usage
if %1 == --HE goto usage
if %1 == --h goto usage
if %1 == --H goto usage
if %1 == -h goto usage
if %1 == -H goto usage
if %1 == /h goto usage
if %1 == /H goto usage
if %1 == -? goto usage
if %1 == /? goto usage
if %1 == ? goto usage
::
if %1 == -- goto option2
if ""%2 == "" goto option1
if %2 == -- goto option3
if ""%3 == "" goto option2a
if %3 == -- goto option4
goto usage
::
:option4
if ""%4 == "" goto usage
if %1 == --create set create=yes
if %create% == yes shift
set gcalyear=%1
shift
if %1 == --create set create=yes
if %create% == no goto usage
shift
shift
if exist %1 goto setofn
goto default
::
:option3
if ""%3 == "" goto usage
if %1 == --create set create=yes
if %create% == yes goto option3a
set gcalyear=%1
:option3a
shift
shift
if exist %1 goto setofn
goto default
::
:option2
if ""%2 == "" goto usage
shift
if exist %1 goto setofn
goto default
::
:option2a
if %1 == --create set create=yes
if %create% == yes shift
set gcalyear=%1
shift
if %1 == --create set create=yes
if %create% == no goto usage
goto default
::
:option1
if %1 == --create goto setcreate
set gcalyear=%1
goto default
::
:setcreate
set create=yes
goto default
::
:setofn
set outfile1=%1
shift
if ""%1 == "" goto start
goto usage
::
:default
if exist %outfile1% goto start
if exist %awkscript% goto next2
echo %myname%: error: file `%awkscript%' is missing
goto eof
::
:next2
if exist %pool% goto next3
echo %myname%: error: file `%pool%' is missing
goto eof
::
:next3
shift
if ""%1 == "" goto next3a
goto usage
:next3a
echo %myname%: creating the Gcal location pool resource file `%outfile1%', please wait...
%-% -f %awkscript% -- -a1 -b%outfile1% -c%myname% %pool%> %outfile1%
if errorlevel 2 goto msg2
::
:start
echo %myname%: generating all Gcal location response files, please wait...
%_% --execute -f%outfile1% -QUx --translate-string=\_
if %create% == no goto eof
::
set myoldname=%myname%
for %%f in (??-*.) do call %runscriptname% %gcalyear% %%f
::
echo %myoldname%: creating the Gcal include file `%outfile2%', please wait...
echo ; %outfile2%, includes the locations for Gcal-2.20 or newer> %outfile2%
echo ;>> %outfile2%
for %%f in (??-*.rc) do echo #include "%%f">> %outfile2%
goto eof
::
:msg2
echo %myname%: error: invalid option to `%-%' given
del %outfile1%> NUL:
goto eof
::
:usage
echo %myname%: Creates all Gcal location response files which can be generated
echo %myname%: from the Gcal location pool resource file `%outfile1%'.
echo %myname%: By definition, a Gcal location response file is a text file of
echo %myname%: one line with a   `-r$l=CC-LOCATION:$c=COORDINATE'   contents.
echo %myname%: If no Gcal location pool resource file exists, this script tries
echo %myname%: to create the `%outfile1%' file by processing the ZONE file
echo %myname%: `%pool%'.  Then, all Gcal location resource files are created,
echo %myname%: which contain the day/night lengths and differences for all days
echo %myname%: of the year YYYY for the location.  At last, the Gcal include
echo %myname%: file `%outfile2%' is created, which contains include statements
echo %myname%: of all Gcal location resource files.  These last two steps are
echo %myname%: done only if the `--create' option is given!
echo %myname%:
echo usage: %myname%  [--create] [YYYY] [-- LOCATION-POOL-RESOURCE-FILE]
echo usage: %myname%  without LOCATION-POOL-RESOURCE-FILE uses file `%outfile1%' by default
::
:eof
::
set myoldname=
set create=
set gcalyear=
set outfile2=
set outfile1=
set gcalrcsuffix=
set awkscript=
set pool=
set runscriptname=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set gcalpath=%oldgcalpath%
set oldgcalpath=
set _=
