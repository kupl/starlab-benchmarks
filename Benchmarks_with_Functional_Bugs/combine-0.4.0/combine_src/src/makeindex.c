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

#include <stdio.h>
#include <stdlib.h>
#include "df_common.h"
#include "dstring.h"
#include "index.h"
#include "field.h"
#include "read_files.h"

int
create_file_index (filename, indexname, record_delimiter, record_length,
		   field_delimiter, field_delimiter_repeat, keys, key_count,
		   unique)
     char *filename;
     char *indexname;
     DStr_string_descriptor *record_delimiter;
     size_t record_length;
     DStr_string_descriptor *field_delimiter;
     int field_delimiter_repeat;
     field_range **keys;
     int key_count;
     int unique;
{
  FILE *input_file;
  FILE *index_files[2];
  DStr_string_descriptor *input_record;
  DStr_string_descriptor **input_fields;
  unsigned char *input_buffer;
  size_t input_buffer_pos;
  size_t input_buffer_size;
  int record_count;
  int return_code;
  int i;
  INDEXNUM data_file_position;
  INDEXNUM data_record_length;

  input_buffer_size = 0;
  input_buffer_pos = 0;
  input_buffer = NULL;

  input_file = fopen (filename, "r");
  if (input_file == NULL)
    return EXIT_FAILURE;

  return_code = open_index_file (indexname, index_files, 1);
  if (return_code != EXIT_SUCCESS)
    return EXIT_FAILURE;

  return_code = initialize_index_file (index_files);	/* Truncate if necessary and write first chunk. */
  if (return_code != EXIT_SUCCESS)
    return EXIT_FAILURE;

  record_count = 0;
  data_file_position = 0;

  input_record
    = df_get_next_record (input_file, &input_buffer, &input_buffer_pos,
			  &input_buffer_size, record_delimiter, record_length);

  while (input_record != NULL) {
    record_count++;
    data_record_length = input_record->length;
    /* TODO Add verbose status updates. */
    /* TODO Add Guile hook for transformation of record. */

    return_code
      = a_find_input_ranges (&(input_fields), input_record, *keys, key_count, 0,
			     field_delimiter, field_delimiter_repeat,
			     DStr_not_own, NULL, NULL);
    if (return_code != EXIT_SUCCESS)
      FATAL_ERROR (_("unable to allocate memory"));

    return_code
      = add_index_record_entry (input_fields, key_count, data_file_position,
				data_record_length, index_files);
    if (return_code != EXIT_SUCCESS)
      return EXIT_FAILURE;

    data_file_position += data_record_length;

    /* Free the record and field array */
    dstrfree (input_record);
    for (i = 0; i < key_count; i++)
      dstrfree (input_fields[i]);
    free (input_fields);

    input_record
      = df_get_next_record (input_file, &input_buffer, &input_buffer_pos,
			    &input_buffer_size, record_delimiter,
			    record_length);
    }

  fclose (input_file);
  close_index_file (index_files);

  return EXIT_SUCCESS;
  }

#ifdef TEST_INDEX
int
main (argc, argv)
     int argc;
     char **argv;
{
  DStr_string_descriptor *record_delimiter;
  DStr_string_descriptor *field_delimiter;
  field_range *keys;
  field_range my_key;

  record_delimiter = dstrdupstr ("\n");
  /* field_delimiter = dstrdupstr (","); */
  keys = &my_key;
  keys->lower_bound = 1;
  keys->upper_bound = 1;
  keys->format = NULL;

  return create_file_index (argv[1], argv[2], record_delimiter, 0, NULL, 0,
			    &keys, 1, 0);
  }

#endif
