/* 
 * Replacement for the passwd related functions for micro ports.
 * Copyright (c) 1996 Markku Rossi.
 *
 * Author: Markku Rossi <mtr@iki.fi>
 */

/*
 * This file is part of GNU Enscript.
 * 
 * Enscript is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Enscript is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Enscript.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#include <windows.h>
#endif

#include "dummypwd.h"

/*
 * Static variables
 */

static struct passwd dummy_passwd =
{
  "dos", "Dos User", "/home",
};


/*
 * Global functions
 */

struct passwd *
getpwuid ()
{
#if defined(WIN32)

  static char userName[32];
  DWORD userNameLen = sizeof (userName);
        	
  if (GetUserName (userName, &userNameLen))
    {
      dummy_passwd.pw_name = userName;
      dummy_passwd.pw_gecos = userName;
    }

  return &dummy_passwd;

#else /* not WIN32 */

  return &dummy_passwd;

#endif /* not WIN32 */
}


struct passwd *
getpwnam (name)
     char *name;
{
  return getpwuid ();
}
