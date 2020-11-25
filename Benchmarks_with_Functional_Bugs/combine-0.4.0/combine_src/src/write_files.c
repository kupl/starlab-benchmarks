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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "df_common.h"
#include "dstring.h"


/* Write a string into the input buffer and actually write it to the
 * file when the buffer is full.  The arguments end_record and
 * end_file indicate whether to write the record delimiter or to flush
 * the buffer.
 */

int
df_write_output (stream, buffer, bufferpos, buffersiz, string, record_delimiter,
		 end_record, end_file)
     FILE *stream;
     unsigned char **buffer;
     size_t *bufferpos;
     size_t *buffersiz;
     STRINGTYPE *string;
     STRINGTYPE *record_delimiter;
     int end_record;
     int end_file;
{
  STRINGTYPE temp_string;
  STRINGTYPE buffer_desc;
  int return_code;

  if (*buffer == NULL) {
    *buffer = malloc (DF_BUFFER_INCREMENT);
    if (*buffer == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    *buffersiz = DF_BUFFER_INCREMENT;
    }

  if (string != NULL) {
    temp_string.string = string->string;
    temp_string.length = string->length;

    buffer_desc.string = *buffer + *bufferpos;
    buffer_desc.length = *buffersiz - *bufferpos;

    while (*bufferpos + temp_string.length >= *buffersiz) {
      /* Put as much as we can into the buffer */
      memmove (buffer_desc.string, temp_string.string, buffer_desc.length);

      /* Write the buffer and adjust */
      return_code = fwrite (*buffer, 1, *buffersiz, stream);
      if (return_code != *buffersiz)
	FATAL_PERROR (_("File"));

      temp_string.string += buffer_desc.length;
      temp_string.length -= buffer_desc.length;
      *bufferpos = 0;
      buffer_desc.string = *buffer;
      buffer_desc.length = *buffersiz;
      }

    if (temp_string.length > 0) {
      memmove (buffer_desc.string, temp_string.string, temp_string.length);
      *bufferpos += temp_string.length;
      }
    }

  if (end_record && record_delimiter != NULL) {
    temp_string.string = record_delimiter->string;
    temp_string.length = record_delimiter->length;

    buffer_desc.string = *buffer + *bufferpos;
    buffer_desc.length = *buffersiz - *bufferpos;

    while (*bufferpos + temp_string.length > *buffersiz) {
      /* Put as much as we can into the buffer */
      memmove (buffer_desc.string, temp_string.string, buffer_desc.length);

      /* Write the buffer and adjust */
      return_code = fwrite (*buffer, 1, *buffersiz, stream);
      if (return_code != *buffersiz)
	FATAL_PERROR (_("File"));

      temp_string.string += buffer_desc.length;
      temp_string.length -= buffer_desc.length;
      *bufferpos = 0;
      buffer_desc.string = *buffer;
      buffer_desc.length = *buffersiz;
      }

    if (temp_string.length > 0) {
      memmove (buffer_desc.string, temp_string.string, temp_string.length);
      *bufferpos += temp_string.length;
      }
    }

  if (end_file && *bufferpos > 0) {
    return_code = fwrite (*buffer, 1, *bufferpos, stream);
    if (return_code != *bufferpos)
      FATAL_PERROR (_("File"));
    }

  return EXIT_SUCCESS;

  }

/********
 * Write a string to the appropriate output file.  Include a delimiter if
 *   requested, substitute spaces if requested.  Currently limited by my imagination
 *   to fields up to 100 bytes long for space substitution.
 */
int
df_write_field (stream, buffer, bufferpos, buffersiz, string, field_delimiter,
		first_written, substitute_spaces, suppress_empty)
     FILE *stream;
     unsigned char **buffer;
     size_t *bufferpos;
     size_t *buffersiz;
     STRINGTYPE *string;
     STRINGTYPE *field_delimiter;
     int *first_written;
     int substitute_spaces;
     int suppress_empty;
{
  int return_code;
  STRINGTYPE spaces;
  if (*first_written && field_delimiter != NULL
      && (!suppress_empty || string->length > 0 || substitute_spaces > 0)) {
    return_code
      = df_write_output (stream, buffer, bufferpos, buffersiz, field_delimiter,
			 NULL, 0, 0);
    if (return_code != EXIT_SUCCESS)
      return return_code;
    }

  if (substitute_spaces > 100)
    FATAL_ERROR (_
		 ("the programmer was too lazy to substitute spaces for fields longer than 100 bytes"));

  if (substitute_spaces == 0) {
    return_code
      = df_write_output (stream, buffer, bufferpos, buffersiz, string, NULL, 0,
			 0);
    if (return_code != EXIT_SUCCESS)
      return return_code;
    }
  else if (substitute_spaces == -1)
    return EXIT_SUCCESS;
  else {
    spaces.length = substitute_spaces;
    spaces.string
      =
      "                                                                                           ";
    return_code
      = df_write_output (stream, buffer, bufferpos, buffersiz, &spaces, NULL, 0,
			 0);
    if (return_code != EXIT_SUCCESS)
      return return_code;
    }

  *first_written = 1;

  return EXIT_SUCCESS;
  }

