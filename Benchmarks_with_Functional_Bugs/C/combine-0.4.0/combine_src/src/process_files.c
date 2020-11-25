/* combine - flexible file merging  
   Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine

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

#include <string.h>
#include <errno.h>
#include "df_common.h"
#include "df_global.h"
#include "dstring.h"
#include "process_files.h"
#include "write_files.h"
#include "hash.h"
#include "iguile.h"
#include "read_files.h"

#define REFINFO gra_reference_file_info
/************
 * Read in the relevant data from all the reference files and store
 *   the key, the output fields, and relevant flags and counters
 *   in a hash table.  
 * Return value is meaningless.
 * Should force a successful value return.
 */
int
process_reference_files ()
{
  STRINGTYPE *key_value;
  STRINGTYPE *hrchy_key_value;
  STRINGTYPE **ref_output_fields;
  STRINGTYPE *input_record_desc;
  HASHTYPE hash_index;
  long int i;
  long int j;
  COUNTTYPE temp_number;
  int return_code;
  FILE *reference_file;

  for (i = 0; i < gi_reference_file_count; i++) {

    /* Allocate the hash table(s). */
    REFINFO[i].hash_table
      = calloc (REFINFO[i].hash_size, sizeof (*REFINFO[i].hash_table));
    if (REFINFO[i].hash_table == NULL) {
      FATAL_ERROR3 (_
		    ("unable to allocate hash table for %s -- wanted %ld things of size %ld bytes\n"),
		    REFINFO[i].file_name, REFINFO[i].hash_size,
		    sizeof (*REFINFO[i].hash_table));
      }

    /* We'll have a second hash for the hierarchy only if we are headed down.
     * That means we are looking for the present key in a different field on 
     * the next record.
     */
    if ((REFINFO[i].hrchy_key_range_count != 0
	 || REFINFO[i].hrchy_key_end_start != 0)
	&& REFINFO[i].hrchy_direction == 0) {
      REFINFO[i].hrchy_hash_table
	= calloc (REFINFO[i].hash_size, sizeof (*REFINFO[i].hash_table));
      if (REFINFO[i].hrchy_hash_table == NULL) {
	FATAL_ERROR3 (_
		      ("unable to allocate hash table for %s -- wanted %ld things of size %ld bytes\n"),
		      REFINFO[i].file_name, REFINFO[i].hash_size,
		      sizeof (*REFINFO[i].hash_table));
	}
      }
    else
      REFINFO[i].hrchy_hash_table = NULL;

    /* Open the reference file. */
    if (strcmp (REFINFO[i].file_name, "-") == 0)
      reference_file = stdin;
    else {
      reference_file = fopen (REFINFO[i].file_name, "r");
      if (reference_file == NULL)
	FATAL_PERROR (REFINFO[i].file_name);
      }

    /* Read all the records and put data in the hash table. */
    input_record_desc
      = df_get_next_record (reference_file, &(REFINFO[i].input_buffer),
			    &(REFINFO[i].input_buffer_pos),
			    &(REFINFO[i].input_buffer_size),
			    REFINFO[i].input_record_delimiter,
			    REFINFO[i].input_record_length);
    while (input_record_desc != NULL) {
      int keep_record;

      REFINFO[i].records_read++;
      keep_record = 1;

      if (gi_verbose_ind != 0 && (REFINFO[i].records_read % 1000) == 0)
	fprintf (stderr,
		 _("\rReading from reference file (%ld) %s: %12d records"),
		 i + 1, REFINFO[i].file_name, REFINFO[i].records_read);

      /* If we need to do anything to the entire record, do it */
#ifdef HAVE_LIBGUILE
      if (REFINFO[i].ext_record_read_command != NULL) {
	char *command_string;
	STRINGTYPE *result_string;
	SCM return_val;
	size_t return_length;

	command_string
	  = malloc (REFINFO[i].ext_record_read_command->length
		    + input_record_desc->length + 1);
	if (command_string == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	return_length
	  = sprintf (command_string, REFINFO[i].ext_record_read_command->string,
		     input_record_desc->length, input_record_desc->string);
	if (return_length
	    > REFINFO[i].ext_record_read_command->length
	    + input_record_desc->length)
	  FATAL_ERROR1 (_("sprintf overflow in %s"),
			"reference record command");
	result_string = eval_scheme_string_function (command_string);
	if (input_record_desc->own_string == DStr_own)
	  free (input_record_desc->string);
	input_record_desc->string = result_string->string;
	input_record_desc->length = result_string->length;
	input_record_desc->own_string = DStr_not_own;
        result_string->own_string = DStr_not_own;
	dstrfree (result_string);
	free (command_string);
	}
#  endif /* HAVE_LIBGUILE */

      /* Collect the key for matching the data file. */
      key_value = dstrnew ();
      find_input_ranges (key_value, input_record_desc, REFINFO[i].key_ranges,
			 REFINFO[i].key_range_count, REFINFO[i].key_end_start,
			 REFINFO[i].input_field_delimiter,
			 REFINFO[i].input_field_delimiter_repeat,
			 gs_output_field_delimiter);

      /* Collect the hierarcy key for matching within the reference file. */
      if (REFINFO[i].hrchy_key_range_count != 0
	  || REFINFO[i].hrchy_key_end_start != 0) {
	hrchy_key_value = dstrnew ();
	find_input_ranges (hrchy_key_value, input_record_desc,
			   REFINFO[i].hrchy_key_ranges,
			   REFINFO[i].hrchy_key_range_count,
			   REFINFO[i].hrchy_key_end_start,
			   REFINFO[i].input_field_delimiter,
			   REFINFO[i].input_field_delimiter_repeat,
			   gs_output_field_delimiter);
	}
      else
	hrchy_key_value = NULL;

      /* The new way, storing them in the output_fields array. */

      return_code
	= a_find_input_ranges (&(ref_output_fields), input_record_desc,
			       REFINFO[i].out_ranges,
			       REFINFO[i].out_range_count,
			       REFINFO[i].out_end_start,
			       REFINFO[i].input_field_delimiter,
			       REFINFO[i].input_field_delimiter_repeat,
			       DStr_own, gs_output_field_delimiter,
			       REFINFO[i].empty_string);
      if (return_code != EXIT_SUCCESS)
	FATAL_ERROR (_("unable to allocate memory"));
      if (REFINFO[i].empty_string != NULL) {
	return_code
	  = fill_empty_fields (ref_output_fields, REFINFO[i].out_range_count,
			       REFINFO[i].empty_string);
	if (return_code != EXIT_SUCCESS)
	  FATAL_ERROR (_("unable to allocate memory"));
	}

      /* Before we do anything for the record, check for filtering request */
#ifdef HAVE_LIBGUILE
      if (REFINFO[i].ext_filter_command != NULL) {
	char *command_string;
	SCM return_val;
	int still_keep_record;
	int ifield;

	/* Make the output fields available to Guile */

	for (ifield = 0;
	     ifield < REFINFO[i].out_range_count + (REFINFO[i].out_end_start
						    != 0 ? 1 : 0); ifield++) {
	  create_scheme_string_variable (&df_name_reference_field, ifield + 1,
					 ref_output_fields[ifield]);
	  }

	command_string = strdupdstr (REFINFO[i].ext_filter_command);
	if (command_string == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	still_keep_record = eval_scheme_boolean_function (command_string);
	free (command_string);
	if (!still_keep_record)
	  keep_record = 0;
	}
#  endif /* HAVE_LIBGUILE */

      if (keep_record) {
	/* Look for existing hash entry.  If there is one, update it.
	 * Otherwise, create a new one and update that.
	 */
	if (REFINFO[i].unique_ind == 0
	    || find_table_entry (key_value, REFINFO[i].hash_table,
				 REFINFO[i].hash_size) == -1) {
	  hash_index
	    = create_table_entry (key_value, REFINFO[i].hash_table,
				  REFINFO[i].hash_size);
	  if (hash_index == -1)
	    FATAL_ERROR (_("unable to insert new hash entry"));
	  else {
	    /* Now allocate room for stored information (counts and sums). */
	    REFINFO[i].hash_table[hash_index].info
	      = malloc (NUM_COUNTERS * sizeof (temp_number)
			+ sizeof (&temp_number));
	    if (REFINFO[i].hash_table[hash_index].info == NULL)
	      FATAL_ERROR (_("unable to allocate memory"));

	    /* Initialize counters to 0 */
	    for (j = 0; j < NUM_COUNTERS; j++)
	      ((COUNTTYPE *) (REFINFO[i].hash_table[hash_index].info))[j] = 0;
	    REFINFO[i].hash_table[hash_index].output_fields = ref_output_fields;
	    REFINFO[i].hash_table[hash_index].chain_key = hrchy_key_value;
	    }

	  if (REFINFO[i].hrchy_hash_table != NULL) {
	    hash_index
	      = create_table_entry (hrchy_key_value,
				    REFINFO[i].hrchy_hash_table,
				    REFINFO[i].hash_size);
	    if (hash_index == -1)
	      FATAL_ERROR (_("unable to insert new hash entry"));
	    else {
	      REFINFO[i].hrchy_hash_table[hash_index].info = NULL;
	      REFINFO[i].hrchy_hash_table[hash_index].output_fields
		= ref_output_fields;
	      REFINFO[i].hrchy_hash_table[hash_index].chain_key = key_value;
	      }
	    }
	  key_value = NULL;	/* We gave this away to the hash table. */
	  hrchy_key_value = NULL;	/* We gave this away to the hash table. */
	  ref_output_fields = NULL;
	  }
	else {
	  /* Not using these, so don't waste memory. */
	  dstrfree (key_value);
	  key_value = NULL;
	  dstrfree (hrchy_key_value);
	  hrchy_key_value = NULL;
	  for (j = 0; j < REFINFO[i].out_range_count; j++)
	    dstrfree (ref_output_fields[j]);
	  free (ref_output_fields);
	  ref_output_fields = NULL;
	  }

	if (gi_data_is_reference != 0) {
	  int write_dummy;
	  /* If we are making the df_check program, we want to process any flags and counters now.  
	   */
	  process_data_reference_matches (NULL, 0, &write_dummy,
					  input_record_desc);
	  }
	}
      else {
	REFINFO[i].records_dropped_filter++;
	}

      dstrfree (input_record_desc);
      input_record_desc
	= df_get_next_record (reference_file, &(REFINFO[i].input_buffer),
			      &(REFINFO[i].input_buffer_pos),
			      &(REFINFO[i].input_buffer_size),
			      REFINFO[i].input_record_delimiter,
			      REFINFO[i].input_record_length);
      }
    if (gi_verbose_ind != 0) {
      fprintf (stderr,
	       _("\rReading from reference file (%ld) %s: %12d records"), i + 1,
	       REFINFO[i].file_name, REFINFO[i].records_read);
      fprintf (stderr, "\n");
      }
    {
      int return_code;
      return_code = fclose (reference_file);
      if (return_code != 0)
	FATAL_PERROR (REFINFO[i].file_name);
      }
    }
  return EXIT_SUCCESS;
  }


/********
 * Read the records from a file and write the portion thereof that make
 * sense to write, based on field selection and key matches.
 *
 * The input_filename is the name of the file that should be opened and
 * read.  Assumes global veriables for counting and for maintaining 
 * options.
 */
int
process_data_file (input_filename)
     char *input_filename;
{
  STRINGTYPE *value;
  int write_record;
  STRINGTYPE *new_value;
  STRINGTYPE *key_value;
  STRINGTYPE *tmp_buf_ptr;
  STRINGTYPE *input_record_desc;

  STRINGTYPE **output_fields;
  int i;
  int return_code;


  value = NULL;
  new_value = NULL;
  key_value = NULL;
  tmp_buf_ptr = NULL;


  /* This is a new file.  Reset the file counters. */
  gi_file_records_read = 0;
  gi_file_records_dropped_filter = 0;
  gi_file_records_matched = 0;
  gi_file_output_records_written = 0;

  if (strcmp (input_filename, "-"))
    input_file = fopen (input_filename, "r");
  else
    input_file = stdin;

  if (input_file == NULL)
    FATAL_PERROR (input_filename);

  input_record_desc
    = df_get_next_record (input_file, &(gs_data_input_buffer),
			  &(gi_data_input_buffer_pos),
			  &(gi_data_input_buffer_size),
			  gs_data_input_record_delimiter,
			  gi_data_input_record_length);

  while (input_record_desc != NULL) {
    int keep_record;
    /* For each record, assemble an output buffer from the output fields for
     * the data file and any matching reference records from the reference 
     * files.  Update the counters in the hash tables for any required
     * information.  If there is no match to a required reference record,
     * then drop the record from output processing.  It will still be counted
     * as a match to any reference table that is matched.
     */
    gi_data_records_read++;
    gi_file_records_read++;
    keep_record = 1;
    write_record = 1;

    if (gi_verbose_ind != 0 && (gi_file_records_read % 1000) == 0)
      fprintf (stderr, _("\rReading from data file %s: %12d records"),
	       (input_file == stdin ? "stdin" : input_filename),
	       gi_file_records_read);

    /* If we need to do anything to the entire record, do it */
#ifdef HAVE_LIBGUILE
    if (gs_data_ext_record_read_command != NULL) {
      char *command_string;
      STRINGTYPE *result_string;
      SCM return_val;
      size_t return_length;

      command_string
	= malloc (gs_data_ext_record_read_command->length
		  + input_record_desc->length + 1);
      if (command_string == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      sprintf (command_string, gs_data_ext_record_read_command->string,
	       input_record_desc->length, input_record_desc->string);
      result_string = eval_scheme_string_function (command_string);
      if (input_record_desc->own_string == DStr_own)
	free (input_record_desc->string);
      input_record_desc->string = result_string->string;
      input_record_desc->length = result_string->length;
      input_record_desc->own_string = DStr_not_own;
      result_string->own_string = DStr_not_own;
      dstrfree (result_string);
      free (command_string);
      }
#  endif /* HAVE_LIBGUILE */


    /* Assemble data file output fields. */
    return_code
      = a_find_input_ranges (&output_fields, input_record_desc,
			     gra_data_out_ranges, gi_data_out_range_count,
			     gi_data_out_end_start,
			     gs_data_input_field_delimiter,
			     gi_data_input_field_delimiter_repeat, DStr_not_own,
			     gs_output_field_delimiter, gs_data_empty_string);
    if (return_code != EXIT_SUCCESS)
      FATAL_ERROR (_("unable to allocate memory"));
    if (gs_data_empty_string != NULL) {
      return_code
	= fill_empty_fields (output_fields, gi_data_out_range_count,
			     gs_data_empty_string);
      if (return_code != EXIT_SUCCESS)
	FATAL_ERROR (_("unable to allocate memory"));
      }

#ifdef HAVE_LIBGUILE
    /* Before we do anything for the record, check for filtering request */
    if (gs_data_ext_filter_command != NULL) {
      char *command_string;
      SCM return_val;
      int still_keep_record;
      int ifield;

      /* Make the output fields available to Guile */
      for (ifield = 0;
	   ifield < gi_data_out_range_count + (gi_data_out_end_start != 0 ? 1
					       : 0); ifield++) {
	create_scheme_string_variable (&df_name_data_field, ifield + 1,
				       output_fields[ifield]);
	}

      command_string = strdupdstr (gs_data_ext_filter_command);
      if (command_string == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      still_keep_record = eval_scheme_boolean_function (command_string);
      free (command_string);
      if (!still_keep_record)
	keep_record = 0;
      }
#  endif /* HAVE_LIBGUILE */


    if (keep_record) {
      /* Now loop through reference files to find matches. */
      process_data_reference_matches (output_fields, 0, &write_record,
				      input_record_desc);
      if (write_record != 0) {
	gi_data_records_matched++;
	gi_file_records_matched++;
	}
      }
    else {
      gi_file_records_dropped_filter++;
      gi_data_records_dropped_filter++;
      }

    for (i = 0; i < gi_data_out_range_count; i++)
      dstrfree (output_fields[i]);
    free (output_fields);

    dstrfree (input_record_desc);
    input_record_desc
      = df_get_next_record (input_file, &(gs_data_input_buffer),
			    &(gi_data_input_buffer_pos),
			    &(gi_data_input_buffer_size),
			    gs_data_input_record_delimiter,
			    gi_data_input_record_length);
    }
  if (gi_verbose_ind != 0) {
    fprintf (stderr, _("\rReading from data file %s: %12d records"),
	     (input_file == stdin ? "stdin" : input_filename),
	     gi_file_records_read);
    fprintf (stderr, "\n");
    }

  if (ferror (input_file))
    FATAL_PERROR (input_filename);
  if (input_file != stdin) {
    int return_code;
    return_code = fclose (input_file);
    if (return_code != 0)
      FATAL_PERROR (input_filename);
    }

  /* Loop through the hash table writing reference records to the data output file
   * if a match to the data record is optional.  Given that there is not a clear
   * way to link records from multiple reference files together, we'll write each
   * of them separately. */
  /* FIXME This does not handle hierarchy non-matches right, but I don't know what right is. */
  if (gi_data_optional_ind != 0) {
    for (i = 0; i < gi_reference_file_count; i++) {
      int j;
      for (j = 0; j < gi_reference_file_count; j++) {
	REFINFO[j].current_hash_selection = -1;
	}
      for (j = 0; j < REFINFO[i].hash_size; j++) {
	if (REFINFO[i].hash_table[j].value != NULL
	    && *(COUNTTYPE *) (REFINFO[i].hash_table[j].info) == 0) {
	  REFINFO[i].current_hash_selection = j;
	  write_match_record (NULL);
	  }
	}
      REFINFO[i].current_hash_selection = -1;
      }
    }

  free (gs_data_input_buffer);
  gs_data_input_buffer = NULL;
  gi_data_input_buffer_size = 0;
  gi_data_input_buffer_pos = 0;

  if (gi_statistics_ind != 0) {
    fprintf (stderr, _("Statistics for data file %s\n"), input_filename);
    fprintf (stderr, _("  Number of records read:              %15d\n"),
	     gi_file_records_read);
    fprintf (stderr, _("  Number of records dropped by filter: %15d\n"),
	     gi_file_records_dropped_filter);
    fprintf (stderr, _("  Number of records matched on key:    %15d\n"),
	     gi_file_records_matched);
    fprintf (stderr, _("  Number of records written:           %15d\n"),
	     gi_file_output_records_written);
    }
  return EXIT_SUCCESS;
  }


/* Recursively look for matches in the various reference files,
   indicated by REFERENCE_FILE_NUMBER.  Carry along the output fields
   and record-writing indicator for the data file record for use in
   writing at the bottom. */
int
process_data_reference_matches (data_output_fields, reference_file_number,
				write_record, input_record_desc)
     STRINGTYPE **data_output_fields;
     int reference_file_number;
     int *write_record;
     STRINGTYPE *input_record_desc;
{
  STRINGTYPE *key_value;
  int j;
  int k;
  HASHTYPE hash_index;
  int first_written;

  if (reference_file_number < gi_reference_file_count) {
    /* Assemble key value. */
    key_value = dstrnew ();
    if (key_value == NULL)
      FATAL_ERROR (_("unable to allocate memory"));

    find_input_ranges (key_value, input_record_desc,
		       REFINFO[reference_file_number].data_key_ranges,
		       REFINFO[reference_file_number].data_key_range_count,
		       REFINFO[reference_file_number].data_key_end_start,
		       gs_data_input_field_delimiter,
		       gi_data_input_field_delimiter_repeat,
		       gs_output_field_delimiter);

    hash_index
      = find_table_entry (key_value, REFINFO[reference_file_number].hash_table,
			  REFINFO[reference_file_number].hash_size);
    }

  do {
    int keep_match;

    keep_match = 1;

#ifdef HAVE_LIBGUILE
    if (gi_reference_file_count != 0 && hash_index != -1
	&& REFINFO[reference_file_number].ext_match_command != NULL) {
      int ifield;
      SCM return_val;
      char *command_string;
      int still_keep_match;

      /* Given the values available from the reference and data records, send a guile command */
      /* First make the fields available to guile */
      for (ifield = 0;
	   ifield < gi_data_out_range_count + (gi_data_out_end_start != 0 ? 1
					       : 0); ifield++) {
	create_scheme_string_variable (&df_name_data_field, ifield + 1,
				       data_output_fields[ifield]);
	}

      for (ifield = 0;
	   ifield
	   < REFINFO[reference_file_number].out_range_count
	   + (REFINFO[reference_file_number].out_end_start != 0 ? 1 : 0);
	   ifield++) {
	create_scheme_string_variable (&df_name_reference_field, ifield + 1,
				       REFINFO[reference_file_number]
				       .hash_table[hash_index]
				       .output_fields[ifield]);
	}


      command_string
	= strdupdstr (REFINFO[reference_file_number].ext_match_command);
      if (command_string == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      still_keep_match = eval_scheme_boolean_function (command_string);
      free (command_string);
      if (!still_keep_match)
	keep_match = 0;
      }
#  endif /* HAVE_LIBGUILE */

    if ((hash_index == -1 || keep_match != 1) && gi_reference_file_count != 0) {
      /* If necessary, invalidate writing. */
      if (gi_reference_file_count != 0
	  && REFINFO[reference_file_number].optional_ind == 0)
	*write_record = 0;
      }
    else if (gi_reference_file_count != 0 && keep_match == 1) {
      /* We need to traverse the hierarchy if requested.  Otherwise, treat this match. */
      if (REFINFO[reference_file_number].hrchy_key_range_count != 0
	  || REFINFO[reference_file_number].hrchy_key_end_start != 0) {
	traverse_hierarchy (data_output_fields, reference_file_number,
			    write_record, input_record_desc, hash_index, 0);
	}
      /* Update flags, counters, and sums. */
      else
	update_counters (reference_file_number, hash_index, data_output_fields,
			 input_record_desc);
      }

    if (keep_match != 1 || gi_reference_file_count == 0
	|| (REFINFO[reference_file_number].hrchy_key_range_count == 0
	    && REFINFO[reference_file_number].hrchy_key_end_start == 0)) {
      /* Either continue with next reference file or write. */
      if (keep_match == 1 && gi_reference_file_count != 0)
	REFINFO[reference_file_number].current_hash_selection = hash_index;
      else if (gi_reference_file_count != 0)
	REFINFO[reference_file_number].current_hash_selection = -1;
      if (*write_record == 0 && gi_checkallref_ind == 0) {
	dstrfree (key_value);
	return EXIT_SUCCESS;
	}
      if (reference_file_number < gi_reference_file_count - 1) {
	process_data_reference_matches (data_output_fields,
					reference_file_number + 1, write_record,
					input_record_desc);
	}
      else {			/* We're at the end of the line, so write the record */
	if (*write_record != 0 && gi_data_write_ind == 1) {
	  int return_code;
	  write_match_record (data_output_fields);
	  }
	}
      }

    /* Find the next hash key if any TEST */
    if (gi_reference_file_count > 0 && hash_index != -1
	&& REFINFO[reference_file_number].hash_table[hash_index].another_for_key
	!= 0) {
      do {
	hash_index
	  = (hash_index + 1) % REFINFO[reference_file_number].hash_size;
	}
      while (dstrcmp
	     (REFINFO[reference_file_number].hash_table[hash_index].value,
	      key_value) != 0);
      }
    else
      hash_index = REFINFO[reference_file_number].hash_size + 1;
  } while (gi_reference_file_count > 0
	     && hash_index != REFINFO[reference_file_number].hash_size + 1);
/*
  dstrfree (key_value);
 */
  return EXIT_SUCCESS;
  }


/********
 * For each reference file we are to write output from, write a record for each record
 *   stored in our hash table adding on flags, counters, and sums as appropriate.
 */
int
write_output ()
{
  int i;
  int j;
  int k;
  char counterformat[10];	// Should be a parameter for maximum counter format size (log10(max counter size) + 5)
  FILE *outfile;
  int return_code;
  int first_written;

  sprintf (counterformat, "%%%dd", gi_output_counter_size);
  for (i = 0; i < gi_reference_file_count; i++) {
    if (REFINFO[i].output_file_name != NULL) {
      outfile = fopen (REFINFO[i].output_file_name, "w");
      if (outfile == NULL)
	FATAL_PERROR (REFINFO[i].output_file_name);
      }
    else
      outfile = stdout;
    for (j = 0; j < REFINFO[i].hash_size; j++) {
      if (REFINFO[i].hash_table[j].value != NULL) {
	REFINFO[i].records_stored++;	/* It's in the table, so we must have stored it. */
	first_written = 0;	/* We have not yet written anything for this record */
	if (((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[FLAG_INDEX] != 0)
	  REFINFO[i].records_matched_key++;
	if (((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[FLAG_INDEX] == 1)
	  REFINFO[i].records_matched++;
	REFINFO[i].data_records_matched
	  += ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[COUNTER_INDEX];
	/* If we have output fields specified, use them.  Otherwise, put the yes out at least. */
	if (REFINFO[i].write_ind == 1) {
	  if (REFINFO[i].output_field_order == NULL) {
	    if (TOTAL_REF_OUT_RANGES (i) > 0) {
	      if (REFINFO[i].constant_string != NULL) {
		return_code
		  = df_write_field (outfile, &(REFINFO[i].output_buffer),
				    &(REFINFO[i].output_buffer_pos),
				    &(REFINFO[i].output_buffer_size),
				    REFINFO[i].constant_string,
				    gs_output_field_delimiter, &first_written,
				    0, REFINFO[i].suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (REFINFO[i].output_file_name);
		}

	      for (k = 0; k < REFINFO[i].out_range_count; k++) {
		return_code
		  = df_write_field (outfile, &(REFINFO[i].output_buffer),
				    &(REFINFO[i].output_buffer_pos),
				    &(REFINFO[i].output_buffer_size),
				    REFINFO[i].hash_table[j].output_fields[k],
				    gs_output_field_delimiter, &first_written,
				    0, REFINFO[i].suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (REFINFO[i].output_file_name);
		}

	      if (REFINFO[i].out_end_start != 0) {
		return_code
		  = df_write_field (outfile, &(REFINFO[i].output_buffer),
				    &(REFINFO[i].output_buffer_pos),
				    &(REFINFO[i].output_buffer_size),
				    REFINFO[i].hash_table[j]
				    .output_fields[REFINFO[i].out_range_count],
				    gs_output_field_delimiter, &first_written,
				    0, REFINFO[i].suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (REFINFO[i].output_file_name);
		}
	      }
	    else {
	      return_code
		= df_write_output (outfile, &(REFINFO[i].output_buffer),
				   &(REFINFO[i].output_buffer_pos),
				   &(REFINFO[i].output_buffer_size),
				   REFINFO[i].hash_table[j].value,
				   gs_output_record_delimiter, 0, 0);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (REFINFO[i].output_file_name);
	      first_written = 1;
	      }

	    for (k = SKIP_COUNTERS; k < NUM_COUNTERS; k++) {
	      char temp_value[100];	/* Assuming that a counter will never need 100 digits */
	      STRINGTYPE *temp_dstring;

	      if (k < 2 || gra_data_sum_ranges[k - 2].precision == 0) {
		sprintf (temp_value, COUNTFORMAT, gi_output_counter_size,
			 ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k]);
		temp_dstring = dstrdupstr (temp_value);
		if (temp_dstring == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		}
	      else {
		STRINGTYPE *temp_period;
		COUNTTYPE temp_scale;
		COUNTTYPE temp_remainder;

		temp_scale = pow (10, gra_data_sum_ranges[k - 2].precision);
		if (errno == ERANGE)
		  FATAL_ERROR1 (_
				("precision causes fixed decimal overflow in sum field number %d"),
				k - 1);
		if (((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k] < 0
		    && ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k]
		    > -1 * temp_scale) {
		  /* Small negative numbers are cool. */
		  temp_dstring = dstrdupstr ("-");
		  if (temp_dstring == NULL)
		    FATAL_ERROR (_("unable to allocate memory"));
		  sprintf (temp_value, COUNTFORMAT,
			   MIN (gi_output_counter_size
				- gra_data_sum_ranges[k - 2].precision - 2, 0),
			   ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k]
			   / temp_scale);
		  temp_period = dstrdupstr (temp_value);
		  if (temp_period == NULL)
		    FATAL_ERROR (_("unable to allocate memory"));
		  temp_dstring = dstrcat (temp_dstring, temp_period);
		  if (temp_dstring == NULL)
		    FATAL_ERROR (_("unable to allocate memory"));
		  dstrfree (temp_period);
		  }
		else {
		  sprintf (temp_value, COUNTFORMAT,
			   MIN (gi_output_counter_size
				- gra_data_sum_ranges[k - 2].precision - 1, 0),
			   ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k]
			   / temp_scale);
		  temp_dstring = dstrdupstr (temp_value);
		  }
		if (temp_dstring == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		temp_period = dstrdupstr (".");
		if (temp_period == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		temp_dstring = dstrcat (temp_dstring, temp_period);
		if (temp_dstring == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		dstrfree (temp_period);
		temp_remainder
		  = ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[k]
		  % temp_scale;
		if (temp_remainder < 0)
		  temp_remainder *= -1;
		sprintf (temp_value, COUNTFRACTFORMAT,
			 gra_data_sum_ranges[k - 2].precision, temp_remainder);
		temp_period = dstrdupstr (temp_value);
		if (temp_period == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		temp_dstring = dstrcat (temp_dstring, temp_period);
		if (temp_dstring == NULL)
		  FATAL_ERROR (_("unable to allocate memory"));
		dstrfree (temp_period);
		}
	      return_code
		= df_write_field (outfile, &(REFINFO[i].output_buffer),
				  &(REFINFO[i].output_buffer_pos),
				  &(REFINFO[i].output_buffer_size),
				  temp_dstring, gs_output_field_delimiter,
				  &first_written, 0,
				  REFINFO[i].suppress_empty_ind);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (REFINFO[i].output_file_name);
	      dstrfree (temp_dstring);
	      }
	    }
	  else {		/* Write out fields according to order */
	    char temp_value[100];	/* Assuming that a counter will never need 100 digits */
	    STRINGTYPE *temp_dstring;

	    for (k = 0; k < REFINFO[i].output_field_order_count; k++) {
	      switch (REFINFO[i].output_field_order[k].source) {
	      case 'r':
		if (REFINFO[i].output_field_order[k].source_num != i + 1
		    && REFINFO[i].output_field_order[k].source_num != 0)
		  FATAL_ERROR (_
			       ("reference file order requests another reference file"));

		switch (REFINFO[i].output_field_order[k].type) {
		case 'o':
		  return_code
		    = df_write_field (outfile, &(REFINFO[i].output_buffer),
				      &(REFINFO[i].output_buffer_pos),
				      &(REFINFO[i].output_buffer_size),
				      REFINFO[i].hash_table[j]
				      .output_fields[REFINFO[i]
						     .output_field_order[k]
						     .type_num - 1],
				      gs_output_field_delimiter, &first_written,
				      0, REFINFO[i].suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (REFINFO[i].output_file_name);
		  break;
		case 'k':
		  return_code
		    = df_write_field (outfile, &(REFINFO[i].output_buffer),
				      &(REFINFO[i].output_buffer_pos),
				      &(REFINFO[i].output_buffer_size),
				      REFINFO[i].constant_string,
				      gs_output_field_delimiter, &first_written,
				      0, REFINFO[i].suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (REFINFO[i].output_file_name);
		  break;
		case 'f':
		  sprintf (temp_value, COUNTFORMAT, gi_output_counter_size,
			   ((COUNTTYPE *) (REFINFO[i].hash_table[j]
					   .info))[FLAG_INDEX]);
		  temp_dstring = dstrdupstr (temp_value);
		  if (temp_dstring == NULL)
		    FATAL_ERROR (_("unable to allocate memory"));
		  return_code
		    = df_write_field (outfile, &(REFINFO[i].output_buffer),
				      &(REFINFO[i].output_buffer_pos),
				      &(REFINFO[i].output_buffer_size),
				      temp_dstring, gs_output_field_delimiter,
				      &first_written, 0,
				      REFINFO[i].suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (REFINFO[i].output_file_name);
		  dstrfree (temp_dstring);
		  break;
		case 'n':
		  sprintf (temp_value, COUNTFORMAT, gi_output_counter_size,
			   ((COUNTTYPE *) (REFINFO[i].hash_table[j]
					   .info))[COUNTER_INDEX]);
		  temp_dstring = dstrdupstr (temp_value);
		  if (temp_dstring == NULL)
		    FATAL_ERROR (_("unable to allocate memory"));
		  return_code
		    = df_write_field (outfile, &(REFINFO[i].output_buffer),
				      &(REFINFO[i].output_buffer_pos),
				      &(REFINFO[i].output_buffer_size),
				      temp_dstring, gs_output_field_delimiter,
				      &first_written, 0,
				      REFINFO[i].suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (REFINFO[i].output_file_name);
		  dstrfree (temp_dstring);
		  break;
		case 's':
		  if (gra_data_sum_ranges
		      [REFINFO[i].output_field_order[k].type_num - 1].precision
		      == 0) {
		    sprintf (temp_value, COUNTFORMAT, gi_output_counter_size,
			     ((COUNTTYPE *) (REFINFO[i].hash_table[j]
					     .info))[REFINFO[i]
						     .output_field_order[k]
						     .type_num - 1]);
		    temp_dstring = dstrdupstr (temp_value);
		    if (temp_dstring == NULL)
		      FATAL_ERROR (_("unable to allocate memory"));
		    }
		  else {
		    STRINGTYPE *temp_period;
		    COUNTTYPE temp_scale;
		    COUNTTYPE temp_remainder;

		    temp_scale
		      = pow (10,
			     gra_data_sum_ranges[REFINFO[i]
						 .output_field_order[k].type_num
						 - 1].precision);
		    if (errno == ERANGE)
		      FATAL_ERROR1 (_
				    ("precision causes fixed decimal overflow in sum field number %d"),
				    k - 1);
		    if (((COUNTTYPE *) (REFINFO[i].hash_table[j]
					.info))[REFINFO[i].output_field_order[k]
						.type_num - 1] < 0
			&&
			((COUNTTYPE *) (REFINFO[i].hash_table[j]
					.info))[REFINFO[i].output_field_order[k]
						.type_num - 1]
			> -1 * temp_scale) {
		      /* Small negative numbers are cool. */
		      temp_dstring = dstrdupstr ("-");
		      if (temp_dstring == NULL)
			FATAL_ERROR (_("unable to allocate memory"));
		      sprintf (temp_value, COUNTFORMAT,
			       MIN (gi_output_counter_size
				    - gra_data_sum_ranges[REFINFO[i]
							  .output_field_order[k]
							  .type_num
							  - 1].precision - 2,
				    0),
			       ((COUNTTYPE *) (REFINFO[i].hash_table[j]
					       .info))[REFINFO[i]
						       .output_field_order[k]
						       .type_num
						       + 1] / temp_scale);
		      temp_period = dstrdupstr (temp_value);
		      if (temp_period == NULL)
			FATAL_ERROR (_("unable to allocate memory"));
		      temp_dstring = dstrcat (temp_dstring, temp_period);
		      if (temp_dstring == NULL)
			FATAL_ERROR (_("unable to allocate memory"));
		      dstrfree (temp_period);
		      }
		    else {
		      sprintf (temp_value, COUNTFORMAT,
			       MIN (gi_output_counter_size
				    - gra_data_sum_ranges[REFINFO[i]
							  .output_field_order[k]
							  .type_num
							  - 1].precision - 1,
				    0),
			       ((COUNTTYPE *) (REFINFO[i].hash_table[j]
					       .info))[REFINFO[i]
						       .output_field_order[k]
						       .type_num
						       + 1] / temp_scale);
		      temp_dstring = dstrdupstr (temp_value);
		      if (temp_dstring == NULL)
			FATAL_ERROR (_("unable to allocate memory"));
		      }
		    temp_period = dstrdupstr (".");
		    if (temp_period == NULL)
		      FATAL_ERROR (_("unable to allocate memory"));
		    temp_dstring = dstrcat (temp_dstring, temp_period);
		    if (temp_dstring == NULL)
		      FATAL_ERROR (_("unable to allocate memory"));
		    dstrfree (temp_period);
		    temp_remainder
		      =
		      ((COUNTTYPE *) (REFINFO[i].hash_table[j].info))[REFINFO[i]
								      .output_field_order
								      [k]
								      .type_num
								      + 1]
		      % temp_scale;
		    if (temp_remainder < 0)
		      temp_remainder *= -1;
		    sprintf (temp_value, COUNTFRACTFORMAT,
			     gra_data_sum_ranges[REFINFO[i]
						 .output_field_order[k].type_num
						 - 1].precision,
			     temp_remainder);
		    temp_period = dstrdupstr (temp_value);
		    if (temp_period == NULL)
		      FATAL_ERROR (_("unable to allocate memory"));
		    temp_dstring = dstrcat (temp_dstring, temp_period);
		    if (temp_dstring == NULL)
		      FATAL_ERROR (_("unable to allocate memory"));
		    dstrfree (temp_period);
		    }
		  return_code
		    = df_write_field (outfile, &(REFINFO[i].output_buffer),
				      &(REFINFO[i].output_buffer_pos),
				      &(REFINFO[i].output_buffer_size),
				      temp_dstring, gs_output_field_delimiter,
				      &first_written, 0,
				      REFINFO[i].suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (REFINFO[i].output_file_name);
		  dstrfree (temp_dstring);
		  break;
		default:
		  FATAL_ERROR2 (_("unknown reference order field type: %c.%c"),
				REFINFO[i].output_field_order[k].source,
				REFINFO[i].output_field_order[k].type);
		  }
		break;
	      default:
		FATAL_ERROR1 (_("unknown reference order field source: %c"),
			      REFINFO[i].output_field_order[k].source);
		}
	      }
	    }
	  return_code
	    = df_write_output (outfile, &(REFINFO[i].output_buffer),
			       &(REFINFO[i].output_buffer_pos),
			       &(REFINFO[i].output_buffer_size), NULL,
			       gs_output_record_delimiter, 1, 0);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (REFINFO[i].output_file_name);
	  }
	}
      }
    return_code
      = df_write_output (outfile, &(REFINFO[i].output_buffer),
			 &(REFINFO[i].output_buffer_pos),
			 &(REFINFO[i].output_buffer_size), NULL,
			 gs_output_record_delimiter, 0, 1);
    if (return_code != EXIT_SUCCESS)
      FATAL_PERROR (REFINFO[i].output_file_name);
    if (REFINFO[i].output_file_name != NULL) {
      return_code = fclose (outfile);
      if (return_code != 0)
	FATAL_PERROR (REFINFO[i].output_file_name);
      }
    if (gi_statistics_ind != 0) {
      fprintf (stderr, _("Statistics for reference file %s\n"),
	       REFINFO[i].file_name);
      fprintf (stderr, _("  Number of records read:              %15d\n"),
	       REFINFO[i].records_read);
      fprintf (stderr, _("  Number of records dropped by filter: %15d\n"),
	       REFINFO[i].records_dropped_filter);
      fprintf (stderr, _("  Number of records stored:            %15d\n"),
	       REFINFO[i].records_stored);
      fprintf (stderr, _("  Number of records matched on key:    %15d\n"),
	       REFINFO[i].records_matched_key);
      fprintf (stderr, _("  Number of records matched fully:     %15d\n"),
	       REFINFO[i].records_matched);
      fprintf (stderr, _("  Number of reference-data matches:    %15d\n"),
	       REFINFO[i].data_records_matched);
      }
    }
  return EXIT_SUCCESS;
  }


HASHTYPE
traverse_hierarchy (data_output_fields, reference_file_number, write_record,
		    input_record_desc, start, depth)
     STRINGTYPE **data_output_fields;
     int reference_file_number;
     int *write_record;
     STRINGTYPE *input_record_desc;
     HASHTYPE start;
     HASHTYPE depth;
{
  HASHTYPE hash_entry;
  HASHTYPE next_hash_entry;
  HASHTYPE other_hash_entry;

  hash_entry = start;
  if (REFINFO[reference_file_number].hrchy_hash_table == NULL) {
    /* Going up */
    while (REFINFO[reference_file_number].hash_table[hash_entry].chain_key
	   != NULL
	   && REFINFO[reference_file_number].hash_table[hash_entry].chain_key
	   ->length != 0) {
      /* If writing every node, either continue with next reference file or write. */
      if (REFINFO[reference_file_number].hrchy_flatten_levels == 0
	  && REFINFO[reference_file_number].hrchy_leaf_only == 0) {
	REFINFO[reference_file_number].current_hash_selection = hash_entry;
	update_counters (reference_file_number, hash_entry, data_output_fields,
			 input_record_desc);
	if (reference_file_number < gi_reference_file_count - 1) {
	  process_data_reference_matches (data_output_fields,
					  reference_file_number + 1,
					  write_record, input_record_desc);
	  }
	else {			/* We're at the end of the line, so write the record */
	  if (*write_record != 0 && gi_data_write_ind == 1) {
	    int return_code;
	    write_match_record (data_output_fields);
	    }
	  }
	}

      next_hash_entry
	= find_table_entry (REFINFO[reference_file_number]
			    .hash_table[hash_entry].chain_key,
			    REFINFO[reference_file_number].hash_table,
			    REFINFO[reference_file_number].hash_size);
      if (next_hash_entry == -1)
	FATAL_ERROR2 (_("referential integrity problem: cannot find key %.*s"),
		      REFINFO[reference_file_number].hash_table[hash_entry]
		      .chain_key->length,
		      REFINFO[reference_file_number].hash_table[hash_entry]
		      .chain_key->string);
#ifdef HAVE_LIBGUILE
      if (REFINFO[reference_file_number].ext_hrchy_match_command != NULL) {
	int ifield;
	SCM return_val;
	char *command_string;
	int still_keep_match;

	/* Given the values available from the previous and current hierarchy records, send a guile command */
	/* First make the fields available to guile */
	for (ifield = 0;
	     ifield
	     < REFINFO[reference_file_number].out_range_count
	     + (REFINFO[reference_file_number].out_end_start != 0 ? 1 : 0);
	     ifield++) {
	  create_scheme_string_variable (&df_name_hierarchy_field, ifield + 1,
					 REFINFO[reference_file_number]
					 .hash_table[hash_entry]
					 .output_fields[ifield]);
	  create_scheme_string_variable (&df_name_reference_field, ifield + 1,
					 REFINFO[reference_file_number]
					 .hash_table[next_hash_entry]
					 .output_fields[ifield]);
	  }

	command_string
	  = strdupdstr (REFINFO[reference_file_number].ext_hrchy_match_command);
	if (command_string == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	still_keep_match = eval_scheme_boolean_function (command_string);
	free (command_string);
	if (!still_keep_match)
	  break;
	}
#  endif /* HAVE_LIBGUILE */
      hash_entry = next_hash_entry;
      }
    /* At the top: either continue with next reference file or write. */
    REFINFO[reference_file_number].current_hash_selection = hash_entry;
    update_counters (reference_file_number, hash_entry, data_output_fields,
		     input_record_desc);
    if (reference_file_number < gi_reference_file_count - 1) {
      process_data_reference_matches (data_output_fields,
				      reference_file_number + 1, write_record,
				      input_record_desc);
      }
    else {			/* We're at the end of the line, so write the record */
      if (*write_record != 0 && gi_data_write_ind == 1) {
	int return_code;
	write_match_record (data_output_fields);
	}
      }
    return hash_entry;
    }
  else {
    REFINFO[reference_file_number].hrchy_curr_depth = depth + 1;
    hrchy_add_path (depth, hash_entry,
		    &(REFINFO[reference_file_number].hrchy_path),
		    &(REFINFO[reference_file_number].hrchy_path_size),
		    REFINFO[reference_file_number].hash_table[hash_entry]
		    .value);

    /* Going down: need to deal with this node and then loop through its children. */
    if (REFINFO[reference_file_number].hrchy_flatten_levels == 0
	&& REFINFO[reference_file_number].hrchy_leaf_only == 0) {
      REFINFO[reference_file_number].current_hash_selection = hash_entry;
      update_counters (reference_file_number, hash_entry, data_output_fields,
		       input_record_desc);
      if (reference_file_number < gi_reference_file_count - 1) {
	process_data_reference_matches (data_output_fields,
					reference_file_number + 1, write_record,
					input_record_desc);
	}
      else {			/* We're at the end of the line, so write the record */
	if (*write_record != 0 && gi_data_write_ind == 1) {
	  int return_code;
	  write_match_record (data_output_fields);
	  }
	}
      }

    next_hash_entry
      = find_table_entry (REFINFO[reference_file_number].hash_table[hash_entry]
			  .value,
			  REFINFO[reference_file_number].hrchy_hash_table,
			  REFINFO[reference_file_number].hash_size);
    if (next_hash_entry == -1) {
      /* If working with every node, go to it. */
      if (REFINFO[reference_file_number].hrchy_leaf_only != 0
	  || REFINFO[reference_file_number].hrchy_flatten_levels != 0) {
	update_counters (reference_file_number, hash_entry, data_output_fields,
			 input_record_desc);
	if (reference_file_number < gi_reference_file_count - 1) {
	  process_data_reference_matches (data_output_fields,
					  reference_file_number + 1,
					  write_record, input_record_desc);
	  }
	else {			/* We're at the end of the line, so write the record */
	  if (*write_record != 0 && gi_data_write_ind == 1) {
	    int return_code;
	    REFINFO[reference_file_number].current_hash_selection = hash_entry;
	    write_match_record (data_output_fields);
	    }
	  }
	}
      }
    else
      do {
	other_hash_entry
	  = find_table_entry (REFINFO[reference_file_number]
			      .hrchy_hash_table[next_hash_entry].chain_key,
			      REFINFO[reference_file_number].hash_table,
			      REFINFO[reference_file_number].hash_size);
	if (other_hash_entry == -1)
	  FATAL_ERROR2 (_
			("referential integrity problem: cannot find key %.*s"),
			REFINFO[reference_file_number]
			.hrchy_hash_table[next_hash_entry].chain_key->length,
			REFINFO[reference_file_number]
			.hrchy_hash_table[next_hash_entry].chain_key->string);
#ifdef HAVE_LIBGUILE
	if (REFINFO[reference_file_number].ext_hrchy_match_command != NULL) {
	  int ifield;
	  SCM return_val;
	  char *command_string;
	  int still_keep_match;

	  /* Given the values available from the previous and current hierarchy records, send a guile command */
	  /* First make the fields available to guile */
	  for (ifield = 0;
	       ifield
	       < REFINFO[reference_file_number].out_range_count
	       + (REFINFO[reference_file_number].out_end_start != 0 ? 1 : 0);
	       ifield++) {
	    create_scheme_string_variable (&df_name_hierarchy_field, ifield + 1,
					   REFINFO[reference_file_number]
					   .hash_table[hash_entry]
					   .output_fields[ifield]);
	    create_scheme_string_variable (&df_name_reference_field, ifield + 1,
					   REFINFO[reference_file_number]
					   .hash_table[other_hash_entry]
					   .output_fields[ifield]);
	    }

	  command_string
	    = strdupdstr (REFINFO[reference_file_number]
			  .ext_hrchy_match_command);
	  if (command_string == NULL)
	    FATAL_ERROR (_("unable to allocate memory"));
	  still_keep_match = eval_scheme_boolean_function (command_string);
	  free (command_string);
	  if (!still_keep_match)
	    other_hash_entry = -1;
	  }
#  endif /* HAVE_LIBGUILE */
	if (other_hash_entry != -1) {
	  REFINFO[reference_file_number].current_hash_selection
	    = other_hash_entry;
	  traverse_hierarchy (data_output_fields, reference_file_number,
			      write_record, input_record_desc, other_hash_entry,
			      depth + 1);
	  }
	/* Find the next hash key if any TEST */
	if (next_hash_entry != -1
	    && REFINFO[reference_file_number].hrchy_hash_table[next_hash_entry]
	    .another_for_key != 0) {
	  do {
	    next_hash_entry
	      = (next_hash_entry
		 + 1) % REFINFO[reference_file_number].hash_size;
	    }
	  while (dstrcmp
		 (REFINFO[reference_file_number]
		  .hrchy_hash_table[next_hash_entry].value,
		  REFINFO[reference_file_number].hash_table[hash_entry].value)
		 != 0);
	  }
	else
	  next_hash_entry = REFINFO[reference_file_number].hash_size + 1;
      } while (next_hash_entry != REFINFO[reference_file_number].hash_size + 1);
    }
  return start;
  }


int
write_match_record (data_output_fields)
     STRINGTYPE **data_output_fields;
{
  int first_written;
  int return_code;
  int i;
  int j;
  int k;

  /* The only way data_output_fields will be NULL is if the data file
   * was optional, and we are writing with the reference file only.
   */

  /* In the event that we want to write only records that did not match
   * then we will want to suppress writing if there is a full match.
   */
  if (data_output_fields != NULL && gi_data_exclude_match_ind)
    return EXIT_SUCCESS;
  for (i = 0; i < gi_reference_file_count; i++) {
    if (REFINFO[i].current_hash_selection != -1 && REFINFO[i].exclude_match_ind)
      return EXIT_SUCCESS;
    }

  gi_output_records_written++;
  gi_file_output_records_written++;

  first_written = 0;
  if (gra_data_output_field_order == NULL) {
    if (data_output_fields != NULL) {
      for (j = 0; j < gi_data_out_range_count; j++) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size),
			    data_output_fields[j], gs_output_field_delimiter,
			    &first_written, 0, gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}

      if (gi_data_out_end_start != 0) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size),
			    data_output_fields[gi_data_out_range_count],
			    gs_output_field_delimiter, &first_written, 0,
			    gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}

      if (gs_data_constant_string != NULL) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size),
			    gs_data_constant_string, gs_output_field_delimiter,
			    &first_written, 0, gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}
      }
    else if (gs_data_missing_string != NULL) {
      for (j = 0; j < gi_data_out_range_count; j++) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size),
			    gs_data_missing_string, gs_output_field_delimiter,
			    &first_written, 0, gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}

      if (gs_data_constant_string != NULL) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size),
			    gs_data_missing_string, gs_output_field_delimiter,
			    &first_written, 0, gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}
      }
    else {
      for (j = 0; j < gi_data_out_range_count; j++) {
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size), NULL,
			    gs_output_field_delimiter, &first_written,
			    (gra_data_out_ranges[j].upper_bound
			     - gra_data_out_ranges[j].lower_bound + 1),
			    gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}

      if (gs_data_constant_string != NULL) {
	int temp_len;

	temp_len = gs_data_constant_string->length;
	return_code
	  = df_write_field (output_file, &(gs_data_output_buffer),
			    &(gi_data_output_buffer_pos),
			    &(gi_data_output_buffer_size), NULL,
			    gs_output_field_delimiter, &first_written, temp_len,
			    gi_data_suppress_empty_ind);
	if (return_code != EXIT_SUCCESS)
	  FATAL_PERROR (gs_data_output_file_name);
	}
      }

    for (k = 0; k < gi_reference_file_count; k++) {
      if (REFINFO[k].hrchy_flatten_levels != 0) {
	if (REFINFO[k].current_hash_selection != -1) {
	  if (REFINFO[k].constant_string != NULL) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[k].constant_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }

	  for (i = 0; i < REFINFO[k].hrchy_flatten_levels; i++) {
	    if (i < REFINFO[k].hrchy_curr_depth) {
	      for (j = 0; j < REFINFO[k].out_range_count; j++) {
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    REFINFO[k].hash_table[REFINFO[k]
							  .hrchy_path[i]]
				    .output_fields[j],
				    gs_output_field_delimiter, &first_written,
				    0, gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		}

	      if (REFINFO[k].out_end_start != 0) {
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    REFINFO[k].hash_table[REFINFO[k]
							  .hrchy_path[i]]
				    .output_fields[REFINFO[k].out_range_count],
				    gs_output_field_delimiter, &first_written,
				    0, gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		}
	      }
	    else {
	      for (j = 0; j < REFINFO[k].out_range_count; j++) {
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size), NULL,
				    gs_output_field_delimiter, &first_written,
				    (REFINFO[k].out_ranges[j].upper_bound
				     - REFINFO[k].out_ranges[j].lower_bound
				     + 1), gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		}
	      }
	    }
	  }
	else if (REFINFO[k].missing_string != NULL) {
	  if (REFINFO[k].constant_string != NULL) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[k].missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }

	  for (j = 0; j < REFINFO[k].out_range_count; j++) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[k].missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  }
	else {
	  if (REFINFO[k].constant_string != NULL) {
	    int temp_len;

	    temp_len = REFINFO[k].constant_string->length;
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				temp_len, gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }

	  for (j = 0; j < REFINFO[k].out_range_count; j++) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				(REFINFO[k].out_ranges[j].upper_bound
				 - REFINFO[k].out_ranges[j].lower_bound + 1),
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  }
	}
      else if (REFINFO[k].current_hash_selection != -1) {
	if (REFINFO[k].constant_string != NULL) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size),
			      REFINFO[k].constant_string,
			      gs_output_field_delimiter, &first_written, 0,
			      gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }

	for (j = 0; j < REFINFO[k].out_range_count; j++) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size),
			      REFINFO[k].hash_table[REFINFO[k]
						    .current_hash_selection]
			      .output_fields[j], gs_output_field_delimiter,
			      &first_written, 0, gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }

	if (REFINFO[k].out_end_start != 0) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size),
			      REFINFO[k].hash_table[REFINFO[k]
						    .current_hash_selection]
			      .output_fields[REFINFO[k].out_range_count],
			      gs_output_field_delimiter, &first_written, 0,
			      gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }

	}
      else if (REFINFO[k].missing_string != NULL) {
	if (REFINFO[k].constant_string != NULL) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size),
			      REFINFO[k].missing_string,
			      gs_output_field_delimiter, &first_written, 0,
			      gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }

	for (j = 0; j < REFINFO[k].out_range_count; j++) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size),
			      REFINFO[k].missing_string,
			      gs_output_field_delimiter, &first_written, 0,
			      gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }
	}
      else {
	if (REFINFO[k].constant_string != NULL) {
	  int temp_len;

	  temp_len = REFINFO[k].constant_string->length;
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size), NULL,
			      gs_output_field_delimiter, &first_written,
			      temp_len, gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }

	for (j = 0; j < REFINFO[k].out_range_count; j++) {
	  return_code
	    = df_write_field (output_file, &(gs_data_output_buffer),
			      &(gi_data_output_buffer_pos),
			      &(gi_data_output_buffer_size), NULL,
			      gs_output_field_delimiter, &first_written,
			      (REFINFO[k].out_ranges[j].upper_bound
			       - REFINFO[k].out_ranges[j].lower_bound + 1),
			      gi_data_suppress_empty_ind);
	  if (return_code != EXIT_SUCCESS)
	    FATAL_PERROR (gs_data_output_file_name);
	  }
	}
      }
    }
  else {
    /* Write in the requested order */
    for (k = 0; k < gi_data_output_field_order_count; k++) {
      switch (gra_data_output_field_order[k].source) {
      case 's':
	/* Ignore substitutions outside the context of a missed reference entry. */
	break;
      case 'r':
	switch (gra_data_output_field_order[k].type) {
	case 'o':
	  if (REFINFO[gra_data_output_field_order[k].source_num - 1]
	      .current_hash_selection != -1) {
	    int hash_pos
	      = REFINFO[gra_data_output_field_order[k].source_num
			- 1].current_hash_selection;
	    if (gra_data_output_field_order[k].type_num > 0) {
	      return_code
		= df_write_field (output_file, &(gs_data_output_buffer),
				  &(gi_data_output_buffer_pos),
				  &(gi_data_output_buffer_size),
				  REFINFO[gra_data_output_field_order[k]
					  .source_num
					  - 1].hash_table[hash_pos]
				  .output_fields[gra_data_output_field_order[k]
						 .type_num - 1],
				  gs_output_field_delimiter, &first_written, 0,
				  gi_data_suppress_empty_ind);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (gs_data_output_file_name);
	      }
	    else {
	      int i;
	      for (i = -1 * gra_data_output_field_order[k].type_num - 1;
		   i
		   < REFINFO[gra_data_output_field_order[k].source_num
			     - 1].out_range_count
		   + (REFINFO[gra_data_output_field_order[k].source_num - 1]
		      .out_end_start != 0 ? 1 : 0); i++) {
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    REFINFO[gra_data_output_field_order[k]
					    .source_num
					    - 1].hash_table[hash_pos]
				    .output_fields[i],
				    gs_output_field_delimiter, &first_written,
				    0, gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		}
	      }
	    }
	  else if (k + 1 < gi_data_output_field_order_count
		   && gra_data_output_field_order[k + 1].source == 's') {
	    int wrote_something;
	    wrote_something = 0;
	    while (k + 1 < gi_data_output_field_order_count
		   && gra_data_output_field_order[k + 1].source == 's') {
	      int hash_pos;
	      k++;
	      if (gra_data_output_field_order[k].source_num == 0 && data_output_fields != NULL) {	/* This is the data file */
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    data_output_fields
				    [gra_data_output_field_order[k].type_num
				     - 1], gs_output_field_delimiter,
				    &first_written, 0,
				    gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		wrote_something = 1;
		break;
		}
	      else if (gra_data_output_field_order[k].source_num != 0) {
		hash_pos
		  = REFINFO[gra_data_output_field_order[k].source_num
			    - 1].current_hash_selection;
		if (hash_pos != -1) {	/* We have a record */
		  return_code
		    = df_write_field (output_file, &(gs_data_output_buffer),
				      &(gi_data_output_buffer_pos),
				      &(gi_data_output_buffer_size),
				      REFINFO[gra_data_output_field_order[k]
					      .source_num
					      - 1].hash_table[hash_pos]
				      .output_fields[gra_data_output_field_order
						     [k].type_num - 1],
				      gs_output_field_delimiter, &first_written,
				      0, gi_data_suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (gs_data_output_file_name);
		  wrote_something = 1;
		  break;
		  }
		}
	      }
	    if (wrote_something == 0) {	/* We need some emptiness if all the substitutions fell through. */
	      return_code
		= df_write_field (output_file, &(gs_data_output_buffer),
				  &(gi_data_output_buffer_pos),
				  &(gi_data_output_buffer_size), NULL,
				  gs_output_field_delimiter, &first_written,
				  (REFINFO
				   [gra_data_output_field_order[k].source_num
				    - 1]
				   .out_ranges[gra_data_output_field_order[k]
					       .type_num - 1].upper_bound
				   - REFINFO[gra_data_output_field_order[k]
					     .source_num
					     - 1]
				   .out_ranges[gra_data_output_field_order[k]
					       .type_num - 1].lower_bound + 1),
				  gi_data_suppress_empty_ind);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (gs_data_output_file_name);
	      }
	    }
	  else if (REFINFO[gra_data_output_field_order[k].source_num - 1].missing_string != NULL && gra_data_output_field_order[k].type_num != REFINFO[gra_data_output_field_order[k].source_num - 1].out_range_count + 1) {	/* We don't have a field so we need spaces or an empty field */
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[gra_data_output_field_order[k]
					.source_num - 1].missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else if (gra_data_output_field_order[k].type_num != REFINFO[gra_data_output_field_order[k].source_num - 1].out_range_count + 1) {	/* We don't have a field so we need spaces or an empty field */
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				(REFINFO
				 [gra_data_output_field_order[k].source_num
				  - 1].out_ranges[gra_data_output_field_order[k]
						  .type_num - 1].upper_bound
				 - REFINFO[gra_data_output_field_order[k]
					   .source_num
					   - 1]
				 .out_ranges[gra_data_output_field_order[k]
					     .type_num - 1].lower_bound + 1),
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  break;
	case 'k':
	  if (REFINFO[gra_data_output_field_order[k].source_num - 1]
	      .current_hash_selection != -1) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[gra_data_output_field_order[k]
					.source_num - 1].constant_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else if (REFINFO[gra_data_output_field_order[k].source_num - 1]
		   .missing_string != NULL) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				REFINFO[gra_data_output_field_order[k]
					.source_num - 1].missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				REFINFO[gra_data_output_field_order[k]
					.source_num
					- 1].constant_string->length,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  break;
	default:
	  FATAL_ERROR2 (_("unknown data output order field type: %c.%c"),
			gra_data_output_field_order[k].source,
			gra_data_output_field_order[k].type);
	  }
	break;
      case 'd':
	switch (gra_data_output_field_order[k].type) {
	case 'o':
	  if (data_output_fields != NULL) {
	    if (gra_data_output_field_order[k].type_num > 0) {
	      return_code
		= df_write_field (output_file, &(gs_data_output_buffer),
				  &(gi_data_output_buffer_pos),
				  &(gi_data_output_buffer_size),
				  data_output_fields[gra_data_output_field_order
						     [k].type_num - 1],
				  gs_output_field_delimiter, &first_written, 0,
				  gi_data_suppress_empty_ind);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (gs_data_output_file_name);
	      }
	    else {
	      int i;
	      for (i = -1 * gra_data_output_field_order[k].type_num - 1;
		   i < gi_data_out_range_count + (gi_data_out_end_start != 0 ? 1
						  : 0); i++) {
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    data_output_fields[i],
				    gs_output_field_delimiter, &first_written,
				    0, gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		}
	      }
	    }
	  else if (k + 1 < gi_data_output_field_order_count
		   && gra_data_output_field_order[k + 1].source == 's') {
	    int wrote_something;
	    wrote_something = 0;
	    while (k + 1 < gi_data_output_field_order_count
		   && gra_data_output_field_order[k + 1].source == 's') {
	      int hash_pos;
	      k++;
	      if (gra_data_output_field_order[k].source_num == 0 && data_output_fields != NULL) {	/* This is the data file */
		return_code
		  = df_write_field (output_file, &(gs_data_output_buffer),
				    &(gi_data_output_buffer_pos),
				    &(gi_data_output_buffer_size),
				    data_output_fields
				    [gra_data_output_field_order[k].type_num
				     - 1], gs_output_field_delimiter,
				    &first_written, 0,
				    gi_data_suppress_empty_ind);
		if (return_code != EXIT_SUCCESS)
		  FATAL_PERROR (gs_data_output_file_name);
		wrote_something = 1;
		break;
		}
	      else if (gra_data_output_field_order[k].source_num != 0) {
		hash_pos
		  = REFINFO[gra_data_output_field_order[k].source_num
			    - 1].current_hash_selection;
		if (hash_pos != -1) {	/* We have a record */
		  return_code
		    = df_write_field (output_file, &(gs_data_output_buffer),
				      &(gi_data_output_buffer_pos),
				      &(gi_data_output_buffer_size),
				      REFINFO[gra_data_output_field_order[k]
					      .source_num
					      - 1].hash_table[hash_pos]
				      .output_fields[gra_data_output_field_order
						     [k].type_num - 1],
				      gs_output_field_delimiter, &first_written,
				      0, gi_data_suppress_empty_ind);
		  if (return_code != EXIT_SUCCESS)
		    FATAL_PERROR (gs_data_output_file_name);
		  wrote_something = 1;
		  break;
		  }
		}
	      }
	    if (wrote_something == 0) {	/* We need some emptiness if all the substitutions fell through. */
	      return_code
		= df_write_field (output_file, &(gs_data_output_buffer),
				  &(gi_data_output_buffer_pos),
				  &(gi_data_output_buffer_size), NULL,
				  gs_output_field_delimiter, &first_written,
				  gra_data_out_ranges
				  [gra_data_output_field_order[k].type_num
				   - 1].upper_bound
				  -
				  gra_data_out_ranges
				  [gra_data_output_field_order[k].type_num
				   - 1].lower_bound + 1,
				  gi_data_suppress_empty_ind);
	      if (return_code != EXIT_SUCCESS)
		FATAL_PERROR (gs_data_output_file_name);
	      }
	    }
	  else if (gs_data_missing_string != NULL
		   && gra_data_output_field_order[k].type_num
		   != gi_data_out_range_count + 1) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				gs_data_missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else if (gra_data_output_field_order[k].type_num
		   != gi_data_out_range_count + 1) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				gra_data_out_ranges[gra_data_output_field_order
						    [k].type_num
						    - 1].upper_bound
				-
				gra_data_out_ranges[gra_data_output_field_order
						    [k].type_num
						    - 1].lower_bound + 1,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  break;
	case 'k':
	  if (data_output_fields != NULL) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				gs_data_constant_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else if (gs_data_missing_string != NULL) {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size),
				gs_data_missing_string,
				gs_output_field_delimiter, &first_written, 0,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  else {
	    return_code
	      = df_write_field (output_file, &(gs_data_output_buffer),
				&(gi_data_output_buffer_pos),
				&(gi_data_output_buffer_size), NULL,
				gs_output_field_delimiter, &first_written,
				gs_data_constant_string->length,
				gi_data_suppress_empty_ind);
	    if (return_code != EXIT_SUCCESS)
	      FATAL_PERROR (gs_data_output_file_name);
	    }
	  break;
	default:
	  FATAL_ERROR2 (_("unknown data output order field type: %c.%c"),
			gra_data_output_field_order[k].source,
			gra_data_output_field_order[k].type);
	  }
	break;
      default:
	FATAL_ERROR1 (_("unknown output order field source: %c"),
		      gra_data_output_field_order[k].source);
	}
      }
    }
  return_code
    = df_write_output (output_file, &(gs_data_output_buffer),
		       &(gi_data_output_buffer_pos),
		       &(gi_data_output_buffer_size), NULL,
		       gs_output_record_delimiter, 1, 0);
  if (return_code != EXIT_SUCCESS)
    FATAL_PERROR (gs_data_output_file_name);
  }

int
update_counters (reference_file_number, hash_index, data_output_fields, input_record_desc)
     int reference_file_number;
     HASHTYPE hash_index;
     STRINGTYPE **data_output_fields;
     STRINGTYPE *input_record_desc;
{
  int j;
  int k;

  ((COUNTTYPE *) (REFINFO[reference_file_number].hash_table[hash_index]
		  .info))[FLAG_INDEX] |= 1;
  ((COUNTTYPE *) (REFINFO[reference_file_number].hash_table[hash_index]
		  .info))[COUNTER_INDEX] += 1;
  j = 2;

  for (k = 0; k < gi_data_sum_range_count; k++) {
    DStr_string_descriptor *temp_string;
    char *temp_value;

    if (gra_data_sum_ranges[k].format != NULL) {
      int ifield;

      /* Given the values available from the reference and data records, send a guile command */
      /* First make the fields available to guile */
      for (ifield = 0;
	   ifield < gi_data_out_range_count + (gi_data_out_end_start != 0 ? 1
					       : 0); ifield++) {
	create_scheme_string_variable (&df_name_data_field, ifield + 1,
				       data_output_fields[ifield]);
	}

      for (ifield = 0;
	   ifield
	   < REFINFO[reference_file_number].out_range_count
	   + (REFINFO[reference_file_number].out_end_start != 0 ? 1 : 0);
	   ifield++) {
	create_scheme_string_variable (&df_name_reference_field, ifield + 1,
				       REFINFO[reference_file_number]
				       .hash_table[hash_index]
				       .output_fields[ifield]);
	}

      }

    temp_string
      = find_field (gra_data_sum_ranges, gi_data_sum_range_count, k,
		    input_record_desc, gs_data_input_field_delimiter);
    if (temp_string == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    temp_value = strdupdstr (temp_string);
    if (temp_value == NULL)
      FATAL_ERROR (_("unable to allocate memory"));

    /* In a normal case, we want to sum the value in the field as is;
     * however, if there is a precision listed, we further want to look 
     * for anything after the decimal point and make an integer which is
     * the number times 10^precision.
     */
    if (gra_data_sum_ranges[k].precision == 0)
      ((COUNTTYPE *) (REFINFO[reference_file_number].hash_table[hash_index]
		      .info))[j + k] += CONVERT_SUM (temp_value);
    else
      CONVERT_SUM_PRECISION (temp_value, gra_data_sum_ranges[k].precision,
			     ((COUNTTYPE *) (REFINFO[reference_file_number]
					     .hash_table[hash_index].info))[j
									    +
									    k]);
    free (temp_value);
    dstrfree (temp_string);
    }
  }

int
hrchy_add_path (depth, new_entry, path, path_size, key)
     HASHTYPE depth;
     HASHTYPE new_entry;
     HASHTYPE **path;
     HASHTYPE *path_size;
     STRINGTYPE *key;
{
  int i;

  if (*path == NULL) {
    *path = malloc (DF_HRCHY_INCREMENT * sizeof (HASHTYPE));
    if (*path == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    *path_size = DF_HRCHY_INCREMENT;
    }
  else if (*path_size <= depth) {
    *path
      = realloc (*path, (*path_size + DF_HRCHY_INCREMENT) * sizeof (HASHTYPE));
    if (*path == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    *path_size += DF_HRCHY_INCREMENT;
    }

  for (i = 0; i < depth; i++) {
    /* FIXME This error message needs more context, so it needs to be added to the arguments */
    if ((*path)[i] == new_entry)
      FATAL_ERROR2 (_("loop in hierarchy: %.*s"), key->length, key->string);
    }
  (*path)[depth] = new_entry;
  }
