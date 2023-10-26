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


/* index.c --- Functions for dealing with indexes
 *
 * Copyright (C) 2002 Daniel P. Valentine
 *
 * Author: Daniel P. Valentine
 */
#define _INTL_REDIRECT_MACROS

#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_LIBINTL_H
#  include <libintl.h>
#  endif
#include "df_common.h"
#include "dstring.h"
#include "index.h"

#ifdef HAVE_GETTEXT
#  define _(String) gettext (String)
#else
#  define _(String) (String)
#  endif

int
initialize_index (entries, number_size, hash_method, filename, index_file)
     INDEXNUM entries;
     INDEXNUM number_size;
     INDEXNUM hash_method;
     char *filename;
     FILE **index_file;
{
  comb_index_hdr index_hdr;
  comb_index_hdr_ll index_hdr_more;
  comb_hash_entry_ll index_entry;
  long long int i;
  int return_val;

  *index_file = fopen (filename, "w");
  if (*index_file == NULL)
    FATAL_PERROR (filename);

  index_hdr.magic_number = 'DpV!';
  index_hdr.version = 1;
  index_hdr.hash_method = hash_method;
  index_hdr.hash_number_size = number_size;

  return_val = fwrite (&index_hdr, sizeof (index_hdr), 1, *index_file);
  if (return_val != 1)
    FATAL_ERROR (_("error writing to index file"));

  /* Assuming the long long version for now */
  index_hdr_more.hash_size = entries;
  return_val
    = fwrite (&index_hdr_more, sizeof (index_hdr_more), 1, *index_file);
  if (return_val != 1)
    FATAL_ERROR (_("error writing to index file"));

  index_entry.file_position = 0;
  index_entry.record_length = 0;
  index_entry.key_position = 0;
  index_entry.key_length = 0;
  index_entry.next_same_key = 0;
  index_entry.prev_same_key = 0;

  for (i = 0; i < entries; i++) {
    return_val = fwrite (&index_entry, sizeof (index_entry), 1, *index_file);
    if (return_val != 1)
      FATAL_ERROR (_("error writing to index file"));
    }

  return 0;
  }

int
open_index (entries, number_size, hash_method, filename, index_file)
     INDEXNUM *entries;
     INDEXNUM *number_size;
     INDEXNUM *hash_method;
     char *filename;
     FILE **index_file;
{
  int return_val;
  comb_index_hdr index_hdr;
  comb_index_hdr_ll index_hdr_more;

  *index_file = fopen (filename, "r");
  if (*index_file == NULL)
    FATAL_PERROR (filename);

  return_val = fread (&index_hdr, sizeof (index_hdr), 1, *index_file);
  if (return_val != 1)
    FATAL_ERROR (_("error reading index file"));
  if (index_hdr.magic_number != 'DpV!' || index_hdr.version != 1)
    FATAL_ERROR (_("invalid index file or unsupported version"));
  *hash_method = index_hdr.hash_method;
  *number_size = index_hdr.hash_number_size;
  if (*number_size != 8)
    FATAL_ERROR (_("only long long numbers are supported now"));

  return_val = fread (&index_hdr_more, sizeof (index_hdr_more), 1, *index_file);
  if (return_val != 1)
    FATAL_ERROR (_("error reading index file"));
  *entries = index_hdr_more.hash_size;

  return 0;
  }


/* Take in information about the index file and a key, returning 0 for success and
 * -1 for failure.  NEXT set to next value for same key if any.  POSITION set to
 * location of indexed record in file.  LENGTH set to length of record in indexed
 * file.
 */
INDEXNUM
find_index_key (entries, number_size, hash_method, index_file, key, next,
		position, length)
     INDEXNUM entries;
     INDEXNUM number_size;
     INDEXNUM hash_method;
     FILE *index_file;
     STRINGTYPE *key;
     INDEXNUM *next;
     INDEXNUM *position;
     INDEXNUM *length;
{
  HASHTYPE hash_key;
  INDEXNUM start_pos;
  INDEXNUM curr_pos;
  comb_hash_entry_ll index_entry;
  int return_val;

  if (*next == -1) {
    INDEXNUM orig_pos;
    start_pos = -1;
    hash_key = calc_hash_key (key);
    curr_pos = hash_key % entries;
    orig_pos = curr_pos;
    do {
      char *test_key;
      /* Get the index entry */
      return_val
	= fseek (index_file,
		 sizeof (comb_index_hdr) + sizeof (comb_index_hdr_ll)
		 + curr_pos * sizeof (comb_hash_entry_ll), SEEK_SET);
      if (return_val != 0)
	FATAL_ERROR (_("unable to reposition within index"));
      return_val = fread (&index_entry, sizeof (index_entry), 1, index_file);
      if (return_val != 1)
	FATAL_ERROR (_("error reading index file"));

      /* Get the index key */
      test_key = malloc (index_entry.key_length);
      if (test_key == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      return_val
	= fseek (index_file,
		 sizeof (comb_index_hdr) + sizeof (comb_index_hdr_ll)
		 + entries * sizeof (comb_hash_entry_ll)
		 + index_entry.key_position, SEEK_SET);
      if (return_val != 0)
	FATAL_ERROR (_("unable to reposition within index"));
      return_val = fread (test_key, index_entry.key_length, 1, index_file);
      if (return_val != 1)
	FATAL_ERROR (_("error reading index file"));

      if (key->length == index_entry.key_length
	  && memcmp (key->string, test_key, key->length) == 0) {
	start_pos = curr_pos;
	orig_pos = curr_pos;
	}
      else
	curr_pos = (curr_pos + 1) % entries;
      free (test_key);
      }
    while (curr_pos != orig_pos);

    }
  else {
    start_pos = *next;
    /* Get the index entry */
    return_val
      = fseek (index_file,
	       sizeof (comb_index_hdr) + sizeof (comb_index_hdr_ll)
	       + start_pos * sizeof (comb_hash_entry_ll), SEEK_SET);
    if (return_val != 0)
      FATAL_ERROR (_("unable to reposition within index"));
    return_val = fread (&index_entry, sizeof (index_entry), 1, index_file);
    if (return_val != 1)
      FATAL_ERROR (_("error reading index file"));
    }

  if (start_pos == -1)
    return -1;

  if (next != NULL)
    *next = index_entry.next_same_key;
  if (position != NULL)
    *position = index_entry.file_position;
  if (length != NULL)
    *length = index_entry.record_length;
  return 0;

  }

INDEXNUM
insert_index_where_open (entries, number_size, hash_method, index_file, key,
			 position, length, hash_key)
     INDEXNUM entries;
     INDEXNUM number_size;
     INDEXNUM hash_method;
     FILE *index_file;
     STRINGTYPE *key;
     INDEXNUM position;
     INDEXNUM length;
     INDEXNUM hash_key;
{
  INDEXNUM start_pos;
  INDEXNUM curr_pos;
  start_pos = position;
  curr_pos = start_pos + 1;

  while (curr_pos != start_pos) {
    if (1) {			/* FIXME if this node is not used */
      /* Fill in the structure, write.  Fill in the key value, write. */
      /* If this is a continuation, update the prior to point to this. */
      return 0;			/* FIXME return the final entry node. */
      }
    curr_pos++;
    curr_pos %= entries;
    }
  return -1;
  }

int
insert_index_key (entries, number_size, hash_method, index_file, key, position,
		  length, hash_key)
     INDEXNUM entries;
     INDEXNUM number_size;
     INDEXNUM hash_method;
     FILE *index_file;
     STRINGTYPE *key;
     INDEXNUM *position;
     INDEXNUM *length;
     INDEXNUM *hash_key;
{
  INDEXNUM return_val;
  INDEXNUM next;
  return_val
    = find_index_key (entries, number_size, hash_method, index_file, key, &next,
		      NULL, NULL);
  while (return_val != -1 && next != -1)
    return_val
      = find_index_key (entries, number_size, hash_method, index_file, key,
			&next, NULL, NULL);
  if (return_val == -1) {
    /* Insert at entry point or shortly thereafter. */
    }
  else {
    /* Insert where open, update return_val record */
    }
  }

/* Declarations of internal functions (not for header file) */
int find_or_create_index_chunk (df_index_base **, INDEXNUM *, df_index_base **,
				INDEXNUM *, int, FILE **);
int create_index_record_entry (df_index_base *, INDEXNUM, int, INDEXNUM,
			       INDEXNUM, FILE **);
df_index_base *create_index_chunk ();
df_index_record *create_index_record ();
df_index_base *read_index_chunk (FILE **, INDEXNUM);
df_index_record *read_index_record (FILE **, INDEXNUM);
int write_index_chunk (FILE **, INDEXNUM, df_index_base *);
int write_index_record (FILE **, INDEXNUM, df_index_record *);


/* Functions for creating or changing index entries */

/* Add a new record entry. For the FIELD_COUNT FIELDS, make the
   appropriate entries in the base index and make an entry in the
   record file indicating the position of the record as
   DATA_FILE_POSITION.  The file descriptors for the index files are
   in STREAMS*/
int
add_index_record_entry (fields, field_count, data_file_position,
			data_record_length, streams)
     DStr_string_descriptor **fields;
     int field_count;
     INDEXNUM data_file_position;
     INDEXNUM data_record_length;
     FILE **streams;
{
  int i;
  int j;
  int return_code;
  int string_loop_limit;
  df_index_base *prev_index_node;
  df_index_base *curr_index_node;
  INDEXNUM prev_position;
  INDEXNUM curr_position;

  curr_index_node = read_index_chunk (streams, 0);
  if (curr_index_node == NULL)
    return EXIT_FAILURE;

  prev_index_node = NULL;
  prev_position = 0;
  curr_position = 0;

  for (i = 0; i < field_count; i++) {	/* Process all fields */
    if (fields[i]->length == 0 && i < field_count - 1) {	/* We need to make a null entry for this field */
      return_code
	= find_or_create_index_chunk (&prev_index_node, &prev_position,
				      &curr_index_node, &curr_position,
				      INDEX_ATOM - 2, streams);
      if (return_code == EXIT_FAILURE) {
	if (prev_index_node != NULL)
	  free (prev_index_node);
	if (curr_index_node != NULL && curr_index_node != prev_index_node)
	  free (curr_index_node);
	return EXIT_FAILURE;
	}
      }

    string_loop_limit = fields[i]->length;
    if (i == field_count - 1)
      string_loop_limit--;
    for (j = 0; j < string_loop_limit; j++) {	/* Process all bytes in
						   the field.  This works
						   for any field, not just
						   strings */
      return_code
	= find_or_create_index_chunk (&prev_index_node, &prev_position,
				      &curr_index_node, &curr_position,
				      fields[i]->string[j], streams);
      if (return_code == EXIT_FAILURE) {
	if (prev_index_node != NULL)
	  free (prev_index_node);
	if (curr_index_node != NULL && curr_index_node != prev_index_node)
	  free (curr_index_node);
	return EXIT_FAILURE;
	}
      }
    /* Here we have to make an entry for the field end , but only if
       this is not the last field */
    if (i < field_count - 1) {
      return_code
	= find_or_create_index_chunk (&prev_index_node, &prev_position,
				      &curr_index_node, &curr_position,
				      INDEX_ATOM - 1, streams);
      if (return_code == EXIT_FAILURE) {
	if (prev_index_node != NULL)
	  free (prev_index_node);
	if (curr_index_node != NULL && curr_index_node != prev_index_node)
	  free (curr_index_node);
	return EXIT_FAILURE;
	}
      }
    }
  /* Now the current node is where we need to hang the record
     entry. */
  if (fields[field_count - 1]->length == 0)
    return_code
      = create_index_record_entry (curr_index_node, curr_position,
				   INDEX_ATOM - 2, data_file_position,
				   data_record_length, streams);
  else
    return_code
      = create_index_record_entry (curr_index_node, curr_position,
				   fields[field_count
					  - 1]->string[fields[field_count
							      - 1]->length - 1],
				   data_file_position, data_record_length,
				   streams);

  if (return_code == EXIT_FAILURE)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
  }

/* Find the given entry or create a new one in the base index table */
int
find_or_create_index_chunk (prev_index_node, prev_position, curr_index_node,
			    curr_position, index_value, streams)
     df_index_base **prev_index_node;
     INDEXNUM *prev_position;
     df_index_base **curr_index_node;
     INDEXNUM *curr_position;
     int index_value;
     FILE **streams;
{
  INDEXNUM next_position;

  if (*prev_index_node != NULL)
    free (*prev_index_node);
  *prev_index_node = *curr_index_node;
  *prev_position = *curr_position;
  *curr_position = (*curr_index_node)[index_value].key_continuation;
  if (*curr_position == 0) {	/* We have to create an entry before moving to it */
    /* Create the node and point it to its parent */
    *curr_index_node = create_index_chunk ();
    if (*curr_index_node == NULL) {
      return EXIT_FAILURE;
      }
    (*curr_index_node)->key_previous = *prev_position;	/* This is just for the first element in the array. */
    *curr_position = write_index_chunk (streams, -1, *curr_index_node);
    if (*curr_position == -1)
      return EXIT_FAILURE;

    /* Point the parent to the new node */
    (*prev_index_node)[index_value].key_continuation = *curr_position;
    next_position
      = write_index_chunk (streams, *prev_position, *prev_index_node);
    if (next_position == -1)
      return EXIT_FAILURE;
    }
  else {
    *curr_index_node = read_index_chunk (streams, *curr_position);
    if (*curr_index_node == NULL)
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
  }

int
create_index_record_entry (chunk, chunk_position, key_position,
			   data_file_position, data_record_length, streams)
     df_index_base *chunk;
     INDEXNUM chunk_position;
     int key_position;
     INDEXNUM data_file_position;
     INDEXNUM data_record_length;
     FILE **streams;
{
  long prev_record_position;
  long curr_record_position;
  long new_record_position;
  long result_position;
  df_index_record *curr_record_entry;
  df_index_record *new_record_entry;

  new_record_entry = create_index_record ();
  if (new_record_entry == NULL)
    return EXIT_FAILURE;

  new_record_entry->file_position = data_file_position;
  new_record_entry->record_length = data_record_length;

  new_record_position = write_index_record (streams, -1, new_record_entry);
  if (new_record_position == -1) {
    free (new_record_entry);
    return EXIT_FAILURE;
    }

  if (chunk[key_position].first_record == 0) {	/* This is the first record for
						   this key */
    chunk[key_position].first_record = new_record_position;
    write_index_chunk (streams, chunk_position, chunk);
    }
  else {			/* There are already records for this key, so we need to run
				   down the chain. */
    prev_record_position = 0;
    curr_record_position = chunk[key_position].first_record;
    curr_record_entry = NULL;
    while (curr_record_position != 0) {
      prev_record_position = curr_record_position;
      if (curr_record_entry != NULL)
	free (curr_record_entry);
      curr_record_entry = read_index_record (streams, curr_record_position);
      if (curr_record_entry == NULL)
	return EXIT_FAILURE;
      curr_record_position = curr_record_entry->next_same_key;
      }

    new_record_entry->prev_same_key = prev_record_position;
    curr_record_entry->next_same_key = new_record_position;
    result_position
      = write_index_record (streams, new_record_position, new_record_entry);
    if (result_position != new_record_position)
      return EXIT_FAILURE;
    result_position
      = write_index_record (streams, prev_record_position, curr_record_entry);
    if (result_position != prev_record_position)
      return EXIT_FAILURE;

    free (curr_record_entry);
    free (new_record_entry);
    }
  return EXIT_SUCCESS;
  }


/* Functions for manipulating the data atructures */

/* Create a chunk of base index records and initialize them */
df_index_base *
create_index_chunk ()
{
  df_index_base *return_array;
  int i;

  return_array = malloc (INDEX_ATOM * sizeof (df_index_base));
  if (return_array == NULL)
    return NULL;

  bzero (return_array, INDEX_ATOM * sizeof (df_index_base));
  for (i = 0; i < 256; i++)
    return_array[i].byte_value = i;
  return return_array;
  }

/* Create a record descriptor and initialize it */
df_index_record *
create_index_record ()
{
  df_index_record *return_record;

  return_record = malloc (sizeof (df_index_record));
  if (return_record == NULL)
    return NULL;

  bzero (return_record, sizeof (df_index_record));
  return return_record;
  }



/* Functions for manipulating the index files. */

/* Open or create 2 index files based on FILENAME and return the
   stream descriptors in the 2-element array STREAMS */
int
open_index_file (filename, streams, new)
     char *filename;
     FILE **streams;
     int new;
{
  char *index_base_file_name;
  char *index_record_file_name;
  int return_code;

  index_base_file_name = malloc (strlen (filename) + 4);
  if (index_base_file_name == NULL)
    return EXIT_FAILURE;

  sprintf (index_base_file_name, "%s.ib", filename);
  streams[0] = fopen (index_base_file_name, "a+");
  if (streams[0] == NULL) {
    perror (index_base_file_name);
    return EXIT_FAILURE;
    }

  if (new != 0) {
    return_code = truncate (index_base_file_name, 0);
    if (return_code != 0) {
      perror (index_base_file_name);
      return EXIT_FAILURE;
      }
    }

  free (index_base_file_name);

  index_record_file_name = malloc (strlen (filename) + 4);
  if (index_record_file_name == NULL)
    return EXIT_FAILURE;

  sprintf (index_record_file_name, "%s.ir", filename);
  streams[1] = fopen (index_record_file_name, "a+");
  if (streams[1] == NULL) {
    perror (index_record_file_name);
    return EXIT_FAILURE;
    }

  if (new != 0) {
    return_code = truncate (index_record_file_name, 0);
    if (return_code != 0) {
      perror (index_record_file_name);
      return EXIT_FAILURE;
      }
    }

  free (index_record_file_name);

  return EXIT_SUCCESS;
  }

/* Initialize a pair of index files. */
int
initialize_index_file (streams)
     FILE **streams;
{
  df_index_base *initial_chunk;
  int return_code;

  initial_chunk = create_index_chunk ();
  if (initial_chunk == NULL)
    return EXIT_FAILURE;

  return_code = write_index_chunk (streams, 0, initial_chunk);
  if (return_code != 0)
    return EXIT_FAILURE;

  free (initial_chunk);
  return EXIT_SUCCESS;
  }

/* Close the 2 index files pointed to by STREAMS */
int
close_index_file (streams)
     FILE **streams;
{
  int return_code;

  return_code = fclose (streams[0]);
  if (return_code != 0) {
    perror (NULL);
    return EXIT_FAILURE;
    }
  return_code = fclose (streams[1]);
  if (return_code != 0) {
    perror (NULL);
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
  }

/* Get a chunk of index from the base index file pointed to by STREAMS
   at POSITION and return a pointer to a newly allocated array
   containing the result. */
df_index_base *
read_index_chunk (streams, position)
     FILE **streams;
     INDEXNUM position;
{
  int return_code;
  long long_position;		/* To fit the offset position size for fseek */
  df_index_base *return_array;

  long_position = position;
  return_code = fseek (streams[0], long_position, SEEK_SET);
  if (return_code != 0) {
    perror (_("index base file"));
    return NULL;
    }

  return_array = malloc (INDEX_ATOM * sizeof (df_index_base));
  if (return_array == NULL)
    return NULL;

  return_code
    = fread (return_array, sizeof (df_index_base), INDEX_ATOM, streams[0]);
  if (return_code != INDEX_ATOM) {
    perror (_("index base file"));
    free (return_array);
    return NULL;
    }

  return return_array;
  }

/* Get a record specifier from the record index file pointed to by
   STREAMS at POSITION and return a pointer to a newly allocated
   record containing the result. */
df_index_record *
read_index_record (streams, position)
     FILE **streams;
     INDEXNUM position;
{
  int return_code;
  long long_position;		/* To fit the offset position size for fseek */
  df_index_record *return_record;

  long_position = position;
  return_code = fseek (streams[1], long_position, SEEK_SET);
  if (return_code != 0) {
    perror (_("index record file"));
    return NULL;
    }

  return_record = malloc (sizeof (df_index_record));
  if (return_record == NULL)
    return NULL;

  return_code = fread (return_record, sizeof (df_index_record), 1, streams[1]);
  if (return_code != 1) {
    perror (_("index record file"));
    free (return_record);
    return NULL;
    }

  return return_record;
  }


/* Write a CHUNK of index to the base index file pointed to by
   STREAMS at POSITION.  A POSITION of -1 means EOF */
int
write_index_chunk (streams, position, chunk)
     FILE **streams;
     INDEXNUM position;
     df_index_base *chunk;
{
  int return_code;
  long long_position;		/* To fit the offset position size for fseek */
  long new_position;

  long_position = position;
  if (long_position >= 0)
    return_code = fseek (streams[0], long_position, SEEK_SET);
  else
    return_code = fseek (streams[0], 0L, SEEK_END);
  if (return_code != 0) {
    perror (_("index base file"));
    return 0;
    }

  new_position = ftell (streams[0]);

  return_code = fwrite (chunk, sizeof (df_index_base), INDEX_ATOM, streams[0]);
  if (return_code != INDEX_ATOM) {
    perror (_("index base file"));
    return 0;
    }

  return new_position;
  }

/* Write a record specifier to the record index file pointed to by
   STREAMS at POSITION and return the position where it was written. */
int
write_index_record (streams, position, record)
     FILE **streams;
     INDEXNUM position;
     df_index_record *record;
{
  int return_code;
  long long_position;		/* To fit the offset position size for fseek */
  long new_position;

  long_position = position;
  if (long_position > 0)
    return_code = fseek (streams[1], long_position, SEEK_SET);
  else
    return_code = fseek (streams[1], 0L, SEEK_END);
  if (return_code != 0) {
    perror (_("index record file"));
    return 0;
    }

  new_position = ftell (streams[1]);

  return_code = fwrite (record, sizeof (df_index_record), 1, streams[1]);
  if (return_code != 1) {
    perror (_("index record file"));
    return 0;
    }

  return new_position;
  }
