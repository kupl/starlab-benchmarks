#  $Id: moon1.awk 0.03 2000/01/12 00:00:03 tom Exp $
#
#  moon1.awk:  Used by `moon.bat' for displaying the dates at which the next
#                Full Moon and New Moon phases take place, plus some
#                additionally information about today's date.
#
#  Copyright (c) 2000  Thomas Esken      <esken@uni-muenster.de>
#                      Im Hagenfeld 84
#                      D-48147 M"unster
#                      GERMANY
#
#  This software doesn't claim completeness, correctness or usability.
#  On principle I will not be liable for ANY damages or losses (implicit
#  or explicit), which result from using or handling my software.
#  If you use this software, you agree without any exception to this
#  agreement, which binds you LEGALLY !!
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the `GNU General Public License' as published by
#  the `Free Software Foundation'; either version 3, or (at your option)
#  any later version.
#
#  You should have received a copy of the `GNU General Public License'
#  along with this program; if not, write to the:
#
#
#
BEGIN {
  found = 0
  moon_match = 0
}

$0 ~ /@/ {
  if (moon == "full")
   {
     if (found == 0)
      {
        moon_match = FNR
        found = 1
      }
   }
}

$0 ~ /!/ {
  if (moon == "new")
   {
     if (found == 0)
      {
        moon_match = FNR
        found = 1
      }
   }
}

END {
  if (moon_match == 1)
    printf "-r$f=%d:$t=day\n", moon_match
  else
    printf "-r$f=%d:$t=days\n", moon_match
}
