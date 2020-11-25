/* 
 * Replacement for the pwd.h header file for micro ports.
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

#ifndef DUMMYPWD_H
#define DUMMYPWD_H

/* Passwd structure. */
struct passwd
{
  char *pw_name;
  char *pw_gecos;
  char *pw_dir;
};

/*
 * Prototypes for passwd related functions.
 */

struct passwd *getpwuid ();

struct passwd *getpwnam ();

#endif /* DUMMYPWD_H */
