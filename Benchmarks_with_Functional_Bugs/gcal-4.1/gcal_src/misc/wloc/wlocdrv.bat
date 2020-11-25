@echo off
::  $Id: wlocdrv.bat 0.06 2000/03/23 00:00:06 tom Exp $
::
::  wlocdrv.bat:  Uses `wlocdrv1.awk' and `wlocdrv2.awk' for processing
::                  the ZONE file `zone.tab' to generate the Gcal resource
::                  file `wlocdrv.rc' and the `wloc' script text, which
::                  creates location files containing air line distances
::                  and course angles between several geographic locations
::                  around the world.
::
::                 Usage: wlocdrv --help
::                 Usage: wlocdrv [--precise] [ZONE-FILE]
::
::                   `--precise':  Use more precise values.
::
::
::  MS-DOS solution.
::
::  Needs `gawk' for processing!
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
set -=gawk
set oldawkpath=%awkpath%
set awkpath=.;c:\dos
::
:: Generic file names, set these environment variables properly!
::
set myname=%0
set mydefaultname=wlocdrv
::
:: Output/script file names, set these environment variables properly!
::
set runscriptname=wloc
set gcalresource=%mydefaultname%.rc
set outfile=%runscriptname%.bat
::
:: Input file names, set these environment variables properly!
::
set pool=zone.tab
set awkscript1=%mydefaultname%1.awk
set awkscript2=%mydefaultname%2.awk
::
if %1"" == "" goto next1
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
if %1 == --precise goto next0
if %1 == --precis goto next0
if %1 == --preci goto next0
if %1 == --prec goto next0
if %1 == --pre goto next0
if %1 == --pr goto next0
if %1 == -precise goto next0
if %1 == -precis goto next0
if %1 == -preci goto next0
if %1 == -prec goto next0
if %1 == -pre goto next0
if %1 == -pr goto next0
goto next0a
::
:next0
set precise=--precise
shift
if %1"" == "" goto next1
::
:next0a
if exist %1 goto next0b
echo %myname%: error: ZONE-FILE `%1' not found
goto eof
::
:next0b
set pooltmp=%1
shift
if ""%1 == "" goto next0c
goto usage
:next0c
set pool=%pooltmp%
set pooltmp=
goto next2
::
:next1
if exist %pool% goto next2
echo %myname%: error: file `%pool%' is missing
goto eof
::
:next2
if exist %awkscript1% goto next3
echo %myname%: error: file `%awkscript1%' is missing
goto eof
::
:next3
if exist %awkscript2% goto start
echo %myname%: error: file `%awkscript2%' is missing
goto eof
::
:start
echo %myname%: generating the script text `%outfile%', please wait...
%-% -f %awkscript1% -- -a0 -b.\%gcalresource% -c%_% -d%precise% %pool%> %outfile%
if errorlevel 2 goto msg1
echo %myname%: generating the Gcal resource file `%gcalresource%', please wait...
%-% -f %awkscript2% -- -a0 -b%gcalresource% %pool%> %gcalresource%
if errorlevel 2 goto msg2
goto eof
::
:msg1
echo %myname%: error: invalid option to `%-%' given
if exist %outfile% del %outfile%> NUL:
goto eof
::
:msg2
echo %myname%: error: invalid option to `%-%' given
if exist %gcalresource% del %gcalresource%> NUL:
goto eof
::
:usage
echo %myname%: Generates the Gcal resource file `%gcalresource%' and
echo %myname%: the `%outfile%' script text, which creates location
echo %myname%: files containing air line distances and course angles
echo %myname%: between several geographic locations around the world.
echo %myname%:
echo usage: %myname%  [--precise] [ZONE-FILE]
echo usage: %myname%  without ZONE-FILE uses file `%pool%' by default
::
:eof
::
set precise=
set awkscript2=
set awkscript1=
set pool=
set outfile=
set gcalresource=
set runscriptname=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set _=
