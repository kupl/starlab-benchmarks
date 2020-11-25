@echo off
::  $Id: ddiff.bat 0.04 2000/01/12 00:00:04 tom Exp $
::
::  ddiff.bat:  Uses `ddiff1.awk', `ddiff2.awk' and `ddiff.@' for creating
::                a Gcal location resource file which contains the day/night
::                lengths and differences for all days of the year YYYY for
::                a definite location.
::                A Gcal location response file is a Gcal response file
::                consisting of one line, which passes the location name
::                LOC, its country code CC, and the ISO-6709-co-ordinate
::                COORD via text variables to Gcal.  The line template of
::                a Gcal location response file is: -r$l=CC-LOC:$c=COORD
::
::                Usage: ddiff --help
::                Usage: ddiff [YYYY] LOCATION-RESPONSE-FILE
::                Usage: ddiff [YYYY] CC LOCATION-NAME ISO-COORDINATE
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
set mydefaultname=ddiff
::
:: Input file names, set these environment variables properly!
::
set gcalresponse=%mydefaultname%.@
set awkscript1=%mydefaultname%1.awk
set awkscript2=%mydefaultname%2.awk
::
if ""%1 == "" goto usage
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
if exist %gcalresponse% goto next0
echo %myname%: error: file `%gcalresponse%' is missing
goto eof
::
:next0
if exist %awkscript1% goto next1
echo %myname%: error: file `%awkscript1%' is missing
goto eof
::
:next1
if exist %awkscript2% goto next2
echo %myname%: error: file `%awkscript2%' is missing
goto eof
::
:next2
if ""%2 == "" goto option1
if ""%3 == "" goto option2
if ""%4 == "" goto option3
if ""%5 == "" goto option4
goto usage
::
:option4
set outfile=%2-%3.rc
echo %myname%: creating the Gcal location resource file `%outfile%', please wait...
%_% -fNUL: -QUx -#0_-r$y=%%:04*Y> $$$tmp0
%_% -r$l=%2-%3:$c=%4 @$$$tmp0 @%gcalresponse% %1+%1> $$$tmp1
goto start
::
:option3
set outfile=%1-%2.rc
echo %myname%: creating the Gcal location resource file `%outfile%', please wait...
%_% -fNUL: -QUx -#0_-r$y=%%:04*Y> $$$tmp0
%_% -r$l=%1-%2:$c=%3 @$$$tmp0 @%gcalresponse%> $$$tmp1
goto start
::
:option2
if exist %2 goto cont2
echo %myname%: error: LOCATION-RESPONSE-FILE `%2' is missing
goto eof
::
:cont2
set outfile=%2.rc
echo %myname%: creating the Gcal location resource file `%outfile%', please wait...
%_% -fNUL: -QUx -#0_-r$y=%%:04*Y> $$$tmp0
%_% @%2 @$$$tmp0 @%gcalresponse% %1+%1> $$$tmp1
goto start
::
:option1
if exist %1 goto cont1
echo %myname%: error: LOCATION-RESPONSE-FILE `%1' is missing
goto eof
::
:cont1
set outfile=%1.rc
echo %myname%: creating the Gcal location resource file `%outfile%', please wait...
%_% -fNUL: -QUx -#0_-r$y=%%:04*Y> $$$tmp0
%_% @%1 @$$$tmp0 @%gcalresponse%> $$$tmp1
goto start
::
:start
%-% -f %awkscript1% $$$tmp1> $$$tmp2
type %awkscript2%>> $$$tmp2
%-% -f $$$tmp2 -- -a%outfile% $$$tmp1> %outfile%
if errorlevel 2 goto msg2
goto delete
::
:msg2
echo %myname%: error: invalid option to `%-%' given
del %outfile%> NUL:
::
:delete
del $$$tmp?> NUL:
goto eof
::
:usage
echo %myname%: Creates a Gcal location resource file which contains the
echo %myname%: day/night lengths and differences for all days of the year
echo %myname%: YYYY for a definite location.
echo %myname%: By definition, a Gcal location response file is a text file of
echo %myname%: one line with a   `-r$l=CC-LOCATION:$c=COORDINATE'   contents.
echo %myname%:
echo usage: %myname%  [YYYY] LOCATION-RESPONSE-FILE
echo usage: %myname%  [YYYY] CC LOCATION-NAME ISO-COORDINATE
::
:eof
::
set outfile=
set awkscript2=
set awkscript1=
set gcalresponse=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set gcalpath=%oldgcalpath%
set oldgcalpath=
set _=
