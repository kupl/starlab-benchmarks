@echo off
::  $Id: moon.bat 0.07 2000/01/12 00:00:07 tom Exp $
::
::  moon.bat:  Uses `moon1.awk' and `moon2.awk' for displaying the dates
::               at which the next Full Moon and New Moon phases take place,
::               plus some additionally information about today's date.
::
::             Usage: moon --help
::             Usage: moon [more Gcal options...]
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
set mydefaultname=moon
::
:: Input file names, set these environment variables properly!
::
set awkscript1=%mydefaultname%1.awk
set awkscript2=%mydefaultname%2.awk
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
if exist %awkscript1% goto next1
echo %0: error: file `%awkscript1%' is missing
goto eof
::
:next1
if exist %awkscript2% goto start
echo %0: error: file `%awkscript2%' is missing
goto eof
::
:start
::
:: Prints some information about today's day.
::
echo 0@t %%-_ : Today is %%K , %%1%%n%%2 (J.D. %%3%%j%%4, DOY %%3%%N %%4, CW %%1%%k %%2).>$$$mtxt
%_% %1 %2 %3 -f$$$mtxt -QUx
del $$$*>NUL:
::
:: Prints today's Moon phase.
::
%_% %1 %2 %3 -fNUL: -QUx -# "0@t %%-\_ : The Moon phase is %%1%%O %%2."
::
:: Prints date of next Full Moon phase.
::
set moon_marker=@
%_% %1 %2 %3 -fNUL: -QUx -# "0 %%O" -I%moon_marker%>NUL:
if errorlevel 1 goto fm1:
%_% %1 %2 %3 -fNUL: -QUx -# "0@t %%-\_ : Today (%%n) is Full Moon."
:fm1
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O" -I%moon_marker%>NUL:
if errorlevel 1 goto fm2:
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O">$$$moon
%-% -f%awkscript1% -vmoon=full $$$moon>$$$mres
goto fm-result
:fm2
%_% %1 %2 %3 -fNUL: -QUx -# "0 %%M  %%>04*Y">$$$date
%-% -f%awkscript2% $$$date>$$$@date
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O">$$$moon
%_% %1 %2 %3 -fNUL: -QUxm @$$$@date -# "0*d1#999 %%O">>$$$moon
%-% -f%awkscript1% -vmoon=full $$$moon>$$$mres
:fm-result
echo 0@t %%-_ : Next Full Moon is on %%1%%n+$f%%2 (%%3in $f $t%%4).>$$$mtxt
%_% %1 %2 %3 -f$$$mtxt @$$$mres -QUx
del $$$moon>NUL:
::
:: Prints date of next New Moon phase.
::
set moon_marker=!
%_% %1 %2 %3 -fNUL: -QUx -# "0 %%O" -I%moon_marker%>NUL:
if errorlevel 1 goto nm1:
%_% %1 %2 %3 -fNUL: -QUx -# "0@t %%-\_ : Today (%%n) is New Moon."
:nm1
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O" -I%moon_marker%>NUL:
if errorlevel 1 goto nm2:
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O">$$$moon
%-% -f%awkscript1% -vmoon=new $$$moon>$$$mres
goto nm-result
:nm2
if exist $$$date goto nm3
%_% %1 %2 %3 -fNUL: -QUx -# "0 %%M  %%>04*Y">$$$date
%-% -f%awkscript2% $$$date>$$$@date
:nm3
%_% %1 %2 %3 -fNUL: -QUxm+ -# "0*d1#999 %%O">$$$moon
%_% %1 %2 %3 -fNUL: -QUxm @$$$@date -# "0*d1#999 %%O">>$$$moon
%-% -f%awkscript1% -vmoon=new $$$moon>$$$mres
:nm-result
echo 0@t %%-_ : Next New Moon is on %%1%%n+$f%%2 (%%3in $f $t%%4).>$$$mtxt
%_% %1 %2 %3 -f$$$mtxt @$$$mres -QUx
del $$$*>NUL:
set moon_marker=
::
:: Prints the number of days until New Year's Day of next year.
::
%_% %1 %2 %3 -fNUL: -QUxy -# "00001231 $d=%%d">$$$mdays
echo $d++>>$$$mdays
echo 0 %%-_ : Only %%1$d%%2 days remaining until %%3%%n+$d%%4 (J.D. %%3%%j+$d%%4).>>$$$mdays
%_% %1 %2 %3 -f$$$mdays -QUx
del $$$*>NUL:
::
:: Prints the number of days until New Year's Day 3000.
::
echo 30000101 %%-_ : Only %%1%%d%%2 days remaining until %%3%%n%%4 (J.D. %%3%%j%%4).>$$$mtxt
%_% %1 %2 %3 -f$$$mtxt -QUx -u 1 3000
del $$$*>NUL:
goto eof
::
:usage
echo %myname%: Displays the dates at which the next Full Moon and
echo %myname%: New Moon phases take place, plus some additionally
echo %myname%: information about today's date.
echo %myname%:
echo usage: %myname%  [more Gcal options...]
::
:eof
::
set awkscript2=
set awkscript1=
set mydefaultname=
set myname=
set awkpath=%oldawkpath%
set oldawkpath=
set -=
set gcalpath=%oldgcalpath%
set oldgcalpath=
set _=
