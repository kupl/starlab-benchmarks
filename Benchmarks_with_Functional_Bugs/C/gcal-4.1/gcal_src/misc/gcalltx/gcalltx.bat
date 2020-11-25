@echo off
:: gcalltx.bat -- (C) Claus-Peter R"uckemann, 1996, 1998, 1999
::
:: version 1.5
::
:: This file may be distributed with Gcal under the terms
:: of the GNU public license.
::
:: This script calls gcalltx.sed to convert 'gcal -n -u %0001 year' output to
:: LaTeX tabular output.
::
:: DOS solution.
::

::
:: Save GCAL environment variable into TMP_GCAL, then clear it.
::
set tmp_gcal=
if %gcal% . == . goto case
set tmp_gcal=%gcal%
set gcal=

:case
if %1 . == . goto help
if %2 . == . goto christstd
if %3 . == . goto country
::else do this:
goto countrychrist

:christstd
gcal -n -u %%0001 --christian-holidays %1 >$$$tmp
goto postlude

:country
gcal -n -u %%0001 --cc-holidays=%1 %2>$$$tmp
goto postlude

:countrychrist
gcal -n -u %%0001 --christian-holidays --cc-holidays=%1 %2>$$$tmp
goto postlude

:postlude
::
:: added the two lines for 4-digits line numbers (BREAKPOINT & DEL)
::
sed "s/^/XYZ/g" $$$tmp >$$$tmp.tm1
sed "=" $$$tmp.tm1 >$$$tmp.tm2
:: v--- breakpoint part
echo s/^\([5][0]\)/BREAKPOINT \1/g>$$$tmp.tm3
echo s/^\([0-9][50][0]\)/BREAKPOINT \1/g>>$$$tmp.tm3
echo s/^\([0-9][0-9][50][0]\)/BREAKPOINT \1/g>>$$$tmp.tm3
echo /^[0-9]/d>>$$$tmp.tm3
echo /^[0-9][0-9]/d>>$$$tmp.tm3
echo /^[0-9][0-9][0-9]/d>>$$$tmp.tm3
echo /^[0-9][0-9][0-9][0-9]/d>>$$$tmp.tm3
echo s/^XYZ//>>$$$tmp.tm3
sed -f $$$tmp.tm3 $$$tmp.tm2 >$$$tmp
::     ^--- breakpoint part
::
sed -e "/^#.*[^\\]$/d" -e "/^#$/d" gcalltx.sed >$$$tmp.sed
sed -f $$$tmp.sed $$$tmp >gcalltx.tex
echo Eternal holiday list written to file gcalltx.tex
del $$$tmp.* >NUL:
goto end

:help
echo usage: %0 year
echo        %0 country_code year
echo        %0 country_code year --christian-holidays
echo"
echo (1st usage provides only Christian holidays)
echo (2nd usage provides country_code holidays)
echo (3rd usage provides Christian and country_code holidays)
echo"
echo country_codes is a country code or a list of country codes
echo as supported by gcal.
echo"
echo Country codes are for example: BE ES FR
echo"
echo A list of country codes can be created this way: BE+FR
echo (s. gcal documentation)
goto end

:end
::
:: Restore GCAL environment variable from TMP_GCAL again.
::
if %tmp_gcal% . == . goto realexit
set gcal=%tmp_gcal%
set tmp_gcal=

:realexit
::EOF:
