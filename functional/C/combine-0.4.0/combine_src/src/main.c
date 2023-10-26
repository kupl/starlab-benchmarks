/* combine - flexible file merging  
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

#include "df_common.h"
#ifdef HAVE_LIBGUILE_H
#  include <libguile.h>
#else
#  ifdef HAVE_LIBGUILE
#  include <guile/gh.h>
#    endif /* HAVE_LIBGUILE */
#  endif /* HAVE_LIBGUILE_H */
#include "df_global.h"
#include "write_files.h"
#include "df_options.h"
#include "process_files.h"
#include "iguile.h"

/********
 * Start guile and then run our program.
 */
#  ifdef HAVE_LIBGUILE_H
void df_main ();
#  else	/* no HAVE_LIBGUILE_H */
void df_main (int, char **);
#    endif /* HAVE_LIBGUILE_H */

void
close_stdout ()
{
  int return_code;
  return_code = fclose (stdout);
  if (return_code != 0) {
    perror (program_name);
    _exit (-1);
    }
  }
int
main (argc, argv)
     int argc;
     char **argv;
{
  int return_code;
  return_code = atexit (close_stdout);
  if (return_code != 0)
    FATAL_ERROR (_("unable to register exit routine"));
#ifdef HAVE_GETTEXT
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif

#ifdef HAVE_LIBGUILE
#  ifdef HAVE_LIBGUILE_H
  scm_boot_guile (argc, argv, df_main, NULL);
#  else	/* no HAVE_LIBGUILE_H */
  gh_enter (argc, argv, df_main);
#    endif /* HAVE_LIBGUILE_H */
#else /* no HAVE_LIBGUILE */
  df_main (argc, argv);
#  endif /* HAVE_LIBGUILE */

  return EXIT_SUCCESS;
  }

/************
 * Control the overall program: handle parameters, handle reference files,
 *                              handle data files, write output,
 *                              write statistics
 * Returns the return values of process_data_file and write_output
 * SHOULD determine exactly what they are.
 */
#  ifdef HAVE_LIBGUILE_H
void
df_main (closure, argc, argv)
#  else	/* no HAVE_LIBGUILE_H */
void
df_main (argc, argv)
#    endif			/* HAVE_LIBGUILE_H */
#  ifdef HAVE_LIBGUILE_H
     void *closure;
#    endif /* HAVE_LIBGUILE_H */
     int argc;
     char **argv;
{
  int i;
  int exit_status;
  int return_code;

  exit_status = 0;

  initialize_variables (argc, argv);

  handle_arguments (argc, argv);

  if (validate_options () != EXIT_SUCCESS)
    FATAL_ERROR (_("invalid option set"));

  if (gi_verbose_ind != 0)
    write_user_request ();

#ifdef HAVE_LIBGUILE
  if (gs_ext_init_file != NULL) {
    SCM return_val;
    char *filename;

#  ifdef HAVE_LIBGUILE_H
    return_val
      =
      scm_primitive_load (scm_mem2string
			  (gs_ext_init_file->string, gs_ext_init_file->length));
#  else
    filename = strdupdstr (gs_ext_init_file);
    if (filename == NULL)
      FATAL_ERROR (_("unable to allocate memory"));

    return_val = gh_eval_file (filename);
    free (filename);
#    endif /* HAVE_LIBGUILE_H */
    /* Should check that return value */
    }

  if (gs_ext_begin_command != NULL) {
    SCM return_val;

    return_val = eval_scheme_command (gs_ext_begin_command);
    /* Should check that return value */
    }
#  endif /* HAVE_LIBGUILE */

  process_reference_files ();

  if (gi_data_is_reference == 0) {
    if (gs_data_output_file_name == NULL)
      output_file = stdout;
    else {
      output_file = fopen (gs_data_output_file_name, "w");
      if (output_file == NULL)
	FATAL_PERROR (gs_data_output_file_name);
      }

    if (filecount == 0) {
      exit_status = process_data_file ("-");
      }
    else {
      for (i = 0; i < filecount; i++) {
	exit_status |= process_data_file (filenames[i]);
	}
      }
    }

  return_code
    = df_write_output (output_file, &(gs_data_output_buffer),
		       &(gi_data_output_buffer_pos),
		       &(gi_data_output_buffer_size), NULL,
		       gs_output_record_delimiter, 0, 1);

  if (return_code != EXIT_SUCCESS)
    FATAL_PERROR (gra_reference_file_info[i].output_file_name);

  if (output_file != stdout && gi_data_is_reference == 0) {
    int return_code;
    return_code = fclose (output_file);
    if (return_code != 0)
      FATAL_PERROR (gs_data_output_file_name);
    }

  if (gi_statistics_ind != 0 && filecount > 1) {
    fprintf (stderr, _("Statistics for all data files\n"));
    fprintf (stderr, _("  Number of records read:              %15d\n"),
	     gi_data_records_read);
    fprintf (stderr, _("  Number of records dropped by filter: %15d\n"),
	     gi_data_records_dropped_filter);
    fprintf (stderr, _("  Number of records matched on key:    %15d\n"),
	     gi_data_records_matched);
    fprintf (stderr, _("  Number of records written:           %15d\n"),
	     gi_output_records_written);
    }

  exit_status |= write_output ();


#ifdef HAVE_LIBGUILE
  if (gs_ext_end_command != NULL) {
    SCM return_val;
    char *temp_command;

    return_val = eval_scheme_command (gs_ext_end_command);
    /* Should check that return value */
    }
#  endif /* HAVE_LIBGUILE */

  exit (exit_status);
  }

