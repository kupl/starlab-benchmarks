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

#ifndef IGUILE_H_GUARD
#define IGUILE_H_GUARD

#ifdef HAVE_LIBGUILE_H
#  include <libguile.h>
#else
#  ifdef HAVE_LIBGUILE
#  include <guile/gh.h>
#    endif /* HAVE_LIBGUILE */
#  endif /* HAVE_LIBGUILE_H */
#include "dstring.h"

/* Constant name for scheme variables based on data fields */
extern STRINGTYPE df_name_data_field;
/* Constant name for scheme variables based on reference fields */
extern STRINGTYPE df_name_reference_field;
/* Constant name for scheme variables based on reference fields 
   from prior records in traversing a hierarchy */
extern STRINGTYPE df_name_hierarchy_field;
/* Constant name for scheme variables based on sum fields */
extern STRINGTYPE df_name_sum_field;
/* Constant name for scheme variables based on input fields */
extern STRINGTYPE df_name_input_field;
/* Constant name for scheme variables based on input records */
extern STRINGTYPE df_name_input_record;

int create_scheme_string_variable (STRINGTYPE *, int, STRINGTYPE *);
int eval_scheme_boolean_function (char *);
STRINGTYPE *eval_scheme_string_function (char *);
#ifdef HAVE_LIBGUILE
SCM eval_scheme0command (char *);
SCM eval_scheme_command (STRINGTYPE *);
#  endif /* HAVE_LIBGUILE */
#endif /* no IGUILE_H_GUARD */
