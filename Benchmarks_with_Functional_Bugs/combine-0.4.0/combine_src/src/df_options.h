/* combine - flexible file merging    
   Copyright (C) 2002, 2003, 2013 Daniel P. Valentine

   This file was written by Daniel P. Valentine <dpv@world.std.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


#ifndef DF_OPTIONS_H_GUARD
#define DF_OPTIONS_H_GUARD

int validate_options ();
int write_user_request ();
int initialize_variables (int, char **);
int calculate_ranges (char *, field_range **, int *, int *);


#endif /* No DF_OPTIONS_H_GUARD */
