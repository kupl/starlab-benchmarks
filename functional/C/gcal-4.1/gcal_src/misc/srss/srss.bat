@echo off
::  $Id: srss.bat 0.06 2000/01/12 00:00:06 tom Exp $
::
::  srss.bat:  Uses `srss.awk' for processing the Gcal `srss.rc' resource
::               file to display the times at which sunrise/sunset has happened
::               in the past respectively will happen in the future, based
::               on the actual local time for several geographic locations
::               around the world.
::
::             Usage: srss --help
::             Usage: srss [--no-pager] [rise|set [abs|rel]] [-- more Gcal options...]
::
::             `rise':        Display the sunrise local times of the locations.
::             `set':         Display the sunset local times of the locations.
::                              If this option is not given, `rise' is used
::                              by default.
::             `abs':         The sort order of the past times (indicated
::                              by a `-' sign) respectively future times
::                              (indicated by a `+' sign) is absolute.
::             `rel':         The sort order of the past times (indicated
::                              by a `-' sign) respectively future times
::                              (indicated by a `+' sign) is relative.
::                              If this option is not given, `abs' is used
::                              by default.
::             `--no-pager':  Disable the use of a pager program.
::
::  MS-DOS solution.
::
::  Needs `gcal', `gawk' and `sort' for processing!
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
set mydefaultname=srss
::
:: Script/Input file names, set these environment variables properly!
::
set gcalresource=%mydefaultname%.rc
set awkscript=%mydefaultname%.awk
::
:: Command line argument names, set these environment variables properly!
::
set param0=--no-pager
set param1=rise
set param2=set
set param3=abs
set param4=rel
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
if exist %gcalresource% goto next2
echo %myname%: error: file `%gcalresource%' is missing
goto eof
::
:next2
if ""%1 == "" goto default1
if %1 == -- goto default1
if %1 == %param1% goto check1
if %1 == %param2% goto check1
if %1 == %param0% goto next3
goto usage
::
:next3
set no_pager=yes
shift
goto next2
::
:default1
set script_args=-a%param1%
goto default2
::
:check1
set script_args=-a%1
shift
if ""%1 == "" goto default2
if %1 == %param3% goto check2
if %1 == %param4% goto check2
goto usage
::
:default2
set script_args=%script_args% -b%param3%
if ""%1 == "" goto eval
if %1 == -- goto remove_arg
goto eval
::
:check2
set script_args=%script_args% -b%1
:remove_arg
shift
::
:eval
%_% -Hno -QUx -f %gcalresource% %1 %2 %3 %4 %5 %6 %7 %8 %9> $$$tmp0
%-% -f %awkscript% -- %script_args% $$$tmp0> $$$tmp1
if errorlevel 2 goto msg2
if errorlevel 1 goto msg1
type $$$tmp1 | sort> $$$tmp2
%-% -F@ "{print $2}" $$$tmp2> $$$tmp3
if ""%no_pager% == "" goto output1
type $$$tmp3
goto delete
::
:output1
type $$$tmp3 | more
goto delete
::
:msg2
echo %myname%: error: invalid option `%script_args%' to `%-%' given
goto delete
::
:msg1
echo %myname%: error: no sun%param1%/sun%param2% times found
::
:delete
del $$$tmp? >NUL:
goto eof
::
:usage
echo %myname%: Displays the times at which sun%param1%/sun%param2% has happened in
echo %myname%: the past respectively will happen in the future, based on the
echo %myname%: actual local time for several geographic locations around the
echo %myname%: world, by using the Gcal resource file `%gcalresource%'.
echo %myname%:
echo usage: %myname%  [--no-pager] [%param1% or %param2% [%param3% or %param4%]] [-- more Gcal options...]
::
:eof
::
set no_pager=
set script_args=
set param4=
set param3=
set param2=
set param1=
set param0=
set awkscript=
set gcalresource=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set gcalpath=%oldgcalpath%
set oldgcalpath=
set _=
