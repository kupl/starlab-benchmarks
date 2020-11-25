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




/********
 * Handle reading in a buffered manner, to minimize the chance of
 * moving data when it's not necessary.  Arguments are the name of the
 * stream, the address of the buffer, the address of a number pointing
 * out our location in the buffer (which should not be modified
 * externally, the address of the size of the buffer (likewise), and
 * the delimiter that indicates the end of a line, and the fixed
 * length of a line.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "df_common.h"
#include "dstring.h"

#define STRINGTYPE DStr_string_descriptor

STRINGTYPE *
df_get_next_record (stream, buffer, bufferpos, buffersiz, record_delimiter, record_length)
     FILE *stream;
     unsigned char **buffer;
     size_t *bufferpos;
     size_t *buffersiz;
     STRINGTYPE *record_delimiter;
     size_t record_length;
{
  STRINGTYPE buffer_desc;
  int need_more_data;
  int next_record_pos;
  int return_code;
  STRINGTYPE *return_string;

  buffer_desc.string = *buffer + *bufferpos;
  buffer_desc.length = *buffersiz - *bufferpos;

  need_more_data = 0;

  if (*buffersiz == 0)
    need_more_data = 1;
  else if (record_length != 0) {
    if (*buffersiz - *bufferpos < record_length)
      need_more_data = 1;
    }
  else {
    next_record_pos = dstrstr (&buffer_desc, record_delimiter);
    if (next_record_pos == 0)
      need_more_data = 1;
    }

  while (need_more_data && !feof (stream)) {
    if (*bufferpos == 0) {
      /* The buffer is already full without a record end.  We need to
       * expand the buffer. (This is normal on the first read.)
       */
      if (*buffer == NULL)
	*buffer = malloc (DF_BUFFER_INCREMENT);
      else
	*buffer = realloc (*buffer, *buffersiz + DF_BUFFER_INCREMENT);
      if (*buffer == NULL)
	FATAL_ERROR (_("unable to allocate memory"));

      return_code
	= fread (*buffer + *buffersiz, 1, DF_BUFFER_INCREMENT, stream);
      if (return_code < DF_BUFFER_INCREMENT && ferror (stream))
	FATAL_PERROR (_("File"));

      *buffersiz += return_code;
      }
    else {
      /* We need to get more data, but the current buffer size may be OK. */
      memmove (*buffer, *buffer + *bufferpos, *buffersiz - *bufferpos);

      *bufferpos = *buffersiz - *bufferpos;

      return_code
	= fread (*buffer + *bufferpos, 1, *buffersiz - *bufferpos, stream);
      if (return_code < *buffersiz - *bufferpos && ferror (stream))
	FATAL_PERROR (_("File"));
      if (return_code < *buffersiz - *bufferpos)
	*buffersiz = *bufferpos + return_code;

      *bufferpos = 0;
      }

    buffer_desc.string = *buffer + *bufferpos;
    buffer_desc.length = *buffersiz - *bufferpos;

    need_more_data = 0;

    if (record_length != 0) {
      if (*buffersiz - *bufferpos < record_length)
	need_more_data = 1;
      }
    else {
      next_record_pos = dstrstr (&buffer_desc, record_delimiter);
      if (next_record_pos == 0)
	need_more_data = 1;
      }
    }

  if (*bufferpos < *buffersiz) {
    if (record_length != 0) {
      return_string
	= dstrsubstr (&buffer_desc, 1, 0,
		      MIN (record_length, buffer_desc.length), DStr_not_own);
      *bufferpos += return_string->length;
      }
    else {
      if (next_record_pos == 0)
	next_record_pos = buffer_desc.length + 1;
      return_string
	= dstrsubstr (&buffer_desc, 1, next_record_pos - 1, 0, DStr_not_own);
      *bufferpos = *bufferpos + next_record_pos - 1 + record_delimiter->length;
      }
    if (return_string == NULL) {
      FATAL_ERROR (_("unable to allocate memory"));
      }
    return return_string;
    }

  return NULL;
  }
