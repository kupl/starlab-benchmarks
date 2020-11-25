/* combine - flexible file merging 
   iguile.c - interface with Guile for extensibility. 
   Copyright (C) 2002, 2003, 2013 Daniel P. Valentine

   This file was written by Daniel P. Valentine <dpv@world.std.com>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3, or (at your option) any later
   version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "df_common.h"
#include "field.h"
#include "iguile.h"

/* Constant name for scheme variables based on data fields */
STRINGTYPE df_name_data_field = { 10,
  DStr_constant,
  "data-field"
  };

/* Constant name for scheme variables based on reference fields */
STRINGTYPE df_name_reference_field = { 15,
  DStr_constant,
  "reference-field"
  };

/* Constant name for scheme variables based on reference fields 
   from the prior record in traversing a hierarchy */
STRINGTYPE df_name_hierarchy_field = { 21,
  DStr_constant,
  "prior-reference-field"
  };

/* Constant name for scheme variables based on sum fields */
STRINGTYPE df_name_sum_field = { 9,
  DStr_constant,
  "sum-field"
  };

/* Constant name for scheme variables based on input fields */
STRINGTYPE df_name_input_field = { 11,
  DStr_constant,
  "input-field"
  };

/* Constant name for scheme variables based on input records */
STRINGTYPE df_name_input_record = { 12,
  DStr_constant,
  "input-record"
  };


/* This will not work for strings containing null characters.
 * I need to figure out how to use a define command with SCM 
 * objects.
 */
/* Create a scheme top-level variable with a name consisting of NAME
   with a hyphen followed by NUMBER.  Its value is the string
   VALUE. */
int
create_scheme_string_variable (name, number, value)
     STRINGTYPE *name;
     int number;
     STRINGTYPE *value;
{
#ifdef HAVE_LIBGUILE
  SCM return_val;
#endif
  char *command;
  char *var_name;

#ifdef HAVE_LIBGUILE
#  ifdef HAVE_LIBGUILE_H
  var_name = malloc (50 + name->length);
  strncpy (var_name, name->string, name->length);
  var_name[name->length] = '\0';
  if (number != 0) {
    strcat (var_name, "-");
    sprintf (&(var_name[strlen (var_name)]), "%d", number);
    }
  return_val = scm_mem2string (value->string, value->length);
  return_val = scm_c_define (var_name, return_val);
  /* I thight this was helpful, but I guess we give the name away to guile.
   * pending confirmation that this is the case, I'll try commenting it out
   * again.  Given that it will fall under the garbage collection there, we
   * ought not be concerned if it appears to be a leak at times.
   * MAINTAIN
   free (var_name);
   */
#  else
  command = malloc (50 + name->length + value->length);
  strcpy (command, "(define ");
  strncat (command, name->string, name->length);
  if (number != 0) {
    strcat (command, "-");
    sprintf (&(command[strlen (command)]), "%d \"", number);
    }
  else
    strcat (command, " \"");
  if (value->length != 0)
    strncat (command, value->string, value->length);
  strcat (command, "\")");
  return_val = gh_eval_str (command);
  /*Should check the return val somehow */

  free (command);
#    endif /* HAVE_LIBGUILE_H */
#  endif /* HAVE_LIBGUILE */

  return EXIT_SUCCESS;
  }


int
eval_scheme_boolean_function (command)
     char *command;
{
#ifdef HAVE_LIBGUILE
  SCM return_val;
  int response;

  return_val = eval_scheme0command (command);
#  ifdef HAVE_LIBGUILE_H
  if (!SCM_BOOLP (return_val))
    FATAL_ERROR (_("scheme command returned a non-string"));
  response = SCM_EQ_P (return_val, SCM_BOOL_T);
#  else
  response = gh_scm2bool (return_val);
#    endif /* HAVE_LIBGUILE_H */
  return response;

#  endif /* HAVE_LIBGUILE */
  }

STRINGTYPE *
eval_scheme_string_function (command)
     char *command;
{
#ifdef HAVE_LIBGUILE
  SCM return_val;
  STRINGTYPE temp_response;
  STRINGTYPE *response;

  return_val = eval_scheme0command (command);
#  ifdef HAVE_LIBGUILE_H
  if (!scm_is_string (return_val))
    FATAL_ERROR (_("scheme command returned a non-string"));
  temp_response.string = scm_to_locale_stringn (return_val, &(temp_response.length) );
  if (temp_response.string ==NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  response = dstrdup (&temp_response);
  if (response == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  free (temp_response.string);
#  else
  response = dstrnew ();
  if (response == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  response->string = gh_scm2newstr (return_val, &return_length);
  response->length = return_length;
#    endif /* HAVE_LIBGUILE_H */
  return response;

#  endif /* HAVE_LIBGUILE */
  }

#ifdef HAVE_LIBGUILE
SCM
eval_scheme0command (command)
     char *command;
{
  SCM return_val;

#  ifdef HAVE_LIBGUILE_H
  return_val = scm_eval_string (scm_makfrom0str (command));
#  else
  return_val = gh_eval_str (command);
#    endif /* HAVE_LIBGUILE_H */
  return return_val;
  }

SCM
eval_scheme_command (command)
     STRINGTYPE *command;
{
  SCM return_val;
  char *command_0str;

#  ifdef HAVE_LIBGUILE_H
  return_val
    = scm_eval_string (scm_mem2string (command->string, command->length));
#  else
  command_0str = malloc (command->length + 1);
  if (command_0str == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  strncpy (command_0str, command->string, command->length);
  command_0str[command->length] = '\0';
  return_val = gh_eval_str (command_0str);
  free (command_0str);
#    endif /* HAVE_LIBGUILE_H */
  return return_val;
  }
#  endif /* HAVE_LIBGUILE */
