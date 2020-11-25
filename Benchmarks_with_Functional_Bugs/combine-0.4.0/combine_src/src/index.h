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

/* index.h -- Definitions for indexing
 * 
 * Copyright (C) 2002 Daniel P. Valentine
 *
 * Author: Daniel P. Valentine
 */

/*  Ruminations on indexing.
 *
 *  An index is an external representation of a lookup structure, 
 *  prepared so that one needn't spend processing time later to 
 *  either build the lookup structure or perform procesing without.
 *
 *  One form of index would hold a hash table and a set of keys, with
 *  pointers to records in the original file that are found by hashing 
 *  on the key.
 *  
 *  The structure of such a file could be a header of (say) 512 bytes
 *  followed by a hash table with a number of entries specified in the
 *  header followed by strings pointed to by the hash entries.
 */
/* The size of an entry indicating an index file entry */
#define INDEXNUM long long int
/* The size of individual index elements (2^bits + 1 for NULL + 1 for Field Delimiter) */
#define INDEX_ATOM 258

/* Start of the header */
struct comb_index_hdr_s
{
  long int magic_number;
  long int version;
  long int hash_method;
  long int hash_number_size;
  };
typedef struct comb_index_hdr_s comb_index_hdr;

struct comb_index_hdr_ll_s
{
  long long int hash_size;
  long long int filler[61];
  };
typedef struct comb_index_hdr_ll_s comb_index_hdr_ll;

struct comb_index_hdr_l_s
{
  long int hash_size;
  long int filler[123];
  };
typedef struct comb_index_hdr_l_s comb_index_hdr_l;

struct comb_index_hdr_s_s
{
  short int hash_size;
  short int filler[247];
  };
typedef struct comb_index_hdr_s_s comb_index_hdr_s;

struct comb_hash_entry_ll_s
{
  long long int file_position;
  long long int record_length;
  long long int key_position;
  long long int key_length;
  long long int next_same_key;
  long long int prev_same_key;
  };
typedef struct comb_hash_entry_ll_s comb_hash_entry_ll;

struct comb_hash_entry_l_s
{
  long int file_position;
  long int record_length;
  long int key_position;
  long int key_length;
  long int next_same_key;
  long int prev_same_key;
  };
typedef struct comb_hash_entry_l_s comb_hash_entry_l;

struct comb_hash_entry_s_s
{
  short int file_position;
  short int record_length;
  short int key_position;
  short int key_length;
  short int next_same_key;
  short int prev_same_key;
  };
typedef struct comb_hash_entry_s_s comb_hash_entry_s;

struct df_index_base_s
{
  INDEXNUM key_continuation;
  INDEXNUM key_previous;
  INDEXNUM first_record;
  unsigned char byte_value;
  };

struct df_index_record_entry_s
{
  INDEXNUM file_position;
  INDEXNUM record_length;
  INDEXNUM next_same_key;
  INDEXNUM prev_same_key;
  };

typedef struct df_index_base_s df_index_base;
typedef struct df_index_record_entry_s df_index_record;

int add_index_record_entry (DStr_string_descriptor **, int, INDEXNUM, INDEXNUM,
			    FILE **);
int open_index_file (char *, FILE **, int);
int initialize_index_file (FILE **);
int close_index_file (FILE **);
