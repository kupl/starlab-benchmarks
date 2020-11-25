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

int hrchy_add_path (HASHTYPE, HASHTYPE, HASHTYPE **, HASHTYPE *, STRINGTYPE *);
int process_reference_files ();
int process_data_file (char *);
int process_data_reference_matches (STRINGTYPE **, int, int *, STRINGTYPE *);
HASHTYPE traverse_hierarchy (STRINGTYPE **, int, int *, STRINGTYPE *, HASHTYPE,
			     HASHTYPE);
int update_counters (int, HASHTYPE, STRINGTYPE **, STRINGTYPE *);
int write_output ();
