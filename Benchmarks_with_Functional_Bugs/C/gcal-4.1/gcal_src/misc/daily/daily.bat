@echo off
::  $Id: daily.bat 0.07 2000/01/12 00:00:07 tom Exp $
::
::  daily.bat:  Uses `daily.awk' for processing Gcal resource file lines used
::                for time management of fixed dates stored in the fixed
::                `DATE-PART HH1:MM1,HH2:MM2,%n,%t  FIXED-DATE-TEXT' format.
::                `HH1:MM1' is the time (HOUR:MINUTE) the fixed date takes place.
::                `HH2:MM2' is the time (HOUR:MINUTE) the fixed date is warned
::                in advance.  The valid range of HOUR is 00...23, and the
::                valid range of MINUTE is 00...59.  For example
::                  `0 08:15,00:30,%n,%t  Dentist'
::                creates a today's fixed date message of 30 minutes
::                starting at 07:45 until 08:15.
::
::              Usage: daily --help
::              Usage: daily [RESOURCE-FILE...] [more Gcal options...]
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
set mydefaultname=daily
::
:: Input file names, set these environment variables properly!
::
set awkscript=%mydefaultname%.awk
set gcalresource=%mydefaultname%.rc
::
if ""%1 == "" goto next0
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
:next0
if exist %awkscript% goto next1
echo %myname%: error: file `%awkscript%' is missing
goto eof
::
:next1
if exist %gcalresource% goto start
echo %myname%: error: file `%gcalresource%' is missing
goto eof
::
:start
if ""%1 == "" goto default
%_% -Hyes -QUax -f%1 %2 %3 %4 %5 %6 %7 %8 %9 >$$$tmp
goto filter
::
:default
%_% -Hyes -QUax -f%gcalresource% %1 %2 %3 %4 %5 %6 %7 %8 %9 >$$$tmp
::
:filter
%-% -f%awkscript% $$$tmp
if errorlevel 1 goto msg
goto eof
::
:msg
echo %myname%: no active fixed dates actually
goto eof
::
:usage
echo %myname%: Time management of fixed dates, which are stored
echo %myname%: in a Gcal resource file by using the fixed format
echo %myname%: `DATE-PART HH1:MM1,HH2:MM2,%%n,%%t  FIXED-DATE-TEXT'.
echo %myname%:
echo usage: %myname%  [RESOURCE-FILE...] [more Gcal options...]
echo usage: %myname%  without RESOURCE-FILE uses file `%gcalresource%' by default
::
:eof
::
if exist $$$tmp del $$$tmp >NUL:
::
set gcalresource=
set awkscript=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set gcalpath=%oldgcalpath%
set oldgcalpath=
set _=
