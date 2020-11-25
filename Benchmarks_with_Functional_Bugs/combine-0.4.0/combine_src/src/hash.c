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

#include <stdlib.h>
#include "df_common.h"
/* #include "df_opts.h"*/
#include "df_global.h"
#include "dstring.h"


/*
 * calc_hash_key
 *
 * Calculates a hash key based on the string passed in.  Takes the three least significant bits
 * from each byte in the string up to a maximum that will fit in the hash key.
 *
 * The option --hashmovement (-H) can be specified as beginning or end to
 *   tell the algorithm which end of the key is likely to have the most
 *   variability.
 *
 * The argument string is the one that needs to have a hash key calculated
 *   for it.
 *
 */
HASHTYPE
calc_hash_key (string)
     STRINGTYPE *string;
{
  int i;
  int j;
  HASHTYPE hash_key;
  HASHTYPE temp_key;
  unsigned char *string_tmp;

  j = 0;
  hash_key = 0;

  switch (gi_hashmovement_ind) {
  case hm_binary:
    /* Use all bits, as many at a time as will fit into HASHTYPE. */
    string_tmp = string->string;
    while (string_tmp < string->string + string->length) {
      i = 0;
      temp_key = 0;
      while (i < sizeof (temp_key)
	     && string_tmp < string->string + string->length) {
	temp_key <<= 8;
	temp_key |= *string_tmp;
	i++;
	string_tmp++;
	}
      temp_key <<= j;
      hash_key = (hash_key ^ temp_key);
      j++;
      }
    break;
  case hm_binary_long:
    /* Not really binary.  This is more a generic string hash. */
    string_tmp = string->string;
    while (string_tmp < string->string + string->length) {
      i = 0;
      temp_key = 0;
      while (i < sizeof (temp_key) * 2
	     && string_tmp < string->string + string->length) {
	temp_key <<= 4;
	temp_key |= (*string_tmp & 15);
	i++;
	string_tmp++;
	}
      temp_key <<= j;
      hash_key = (hash_key ^ temp_key);
      j++;
      }
    break;
  case hm_number:
    hash_key = dstrtonum (string, NULL, 10);
    string_tmp = string->string;
    while (string_tmp < string->string + string->length) {
      hash_key *= 10;
      hash_key += *string_tmp - '0';
      string_tmp++;
      }
    break;
  case hm_beginning:
  case hm_end:

    switch (gi_hashmovement_ind) {
    case hm_beginning:
      string_tmp = string->string;
      while ((i < (sizeof hash_key) * 8 / 3)
	     && string_tmp < string->string + string->length) {
	hash_key <<= 3;
	hash_key |= (*string_tmp & 7);
	i++;
	string_tmp++;
	}
      break;
    case hm_end:
      string_tmp = string->string;
      if (string->length > (sizeof hash_key) * 8 / 3) {
	string_tmp += string->length - ((sizeof hash_key) * 8 / 3);
	}

      while ((i < (sizeof hash_key) * 8 / 3)
	     && string_tmp < string->string + string->length) {
	hash_key <<= 3;
	hash_key |= (*string_tmp & 7);
	i++;
	string_tmp++;
	}
      break;
      }
    break;

  default:
    break;
    }

  return hash_key;
  }

/*
 * find_table_entry
 *
 * Find a hash table entry for a string.  The STRING is a character string
 *   identified as a key.  The HASH_TABLE is a pre-allocated array of
 *   pointers to hash structures that should already have been filled.
 *   The HASH_TABLE_SIZE prevents us from going out of bounds.
 *
 */
HASHTYPE
find_table_entry (string, hash_table, hash_table_size)
     STRINGTYPE *string;
     val_entry *hash_table;
     long int hash_table_size;
{
  HASHTYPE temp_key;
  HASHTYPE orig_hash_key;

  orig_hash_key = calc_hash_key (string);
  temp_key = orig_hash_key % hash_table_size;

  if (hash_table[temp_key].hash_key == orig_hash_key
      && hash_table[temp_key].value != NULL
      && dstrcmp (hash_table[temp_key].value, string) == 0)
    return temp_key;

  temp_key = (temp_key + 1) % hash_table_size;

  while (temp_key != orig_hash_key % hash_table_size
	 && hash_table[temp_key].value != NULL) {
    if (hash_table[temp_key].hash_key == orig_hash_key
	&& hash_table[temp_key].value != NULL
	&& dstrcmp (hash_table[temp_key].value, string) == 0)
      return temp_key;

    temp_key = (temp_key + 1) % hash_table_size;
    }

  return -1;
  }


/*
 * create_table_entry
 *
 * Insert a value in a hash table based on a string key.  The STRING is
 *   the key of the data to enter in the table, the HASH_TABLE is a 
 *   pre-allocated array of pointers to data into which we want to make
 *   a new entry, the HASH_TABLE_SIZE lets us know what the limits are.
 *
 */
HASHTYPE
create_table_entry (string, hash_table, hash_table_size)
     STRINGTYPE *string;
     val_entry *hash_table;
     long int hash_table_size;
{
  HASHTYPE temp_key;
  HASHTYPE orig_hash_key;

  orig_hash_key = calc_hash_key (string);
  temp_key = orig_hash_key % hash_table_size;

  /* For a purely simple entry, put it in. */
  if (hash_table[temp_key].value == NULL) {
    hash_table[temp_key].hash_key = orig_hash_key;
    hash_table[temp_key].another_for_key = 0;
    hash_table[temp_key].value = string;
    return temp_key;
    }


  /* Otherwise, find another spot for the entry. */
  if (dstrcmp (hash_table[temp_key].value, string) == 0)
    hash_table[temp_key].another_for_key++;
  temp_key = (temp_key + 1) % hash_table_size;

  while (temp_key != orig_hash_key % hash_table_size
	 && hash_table[temp_key].value != NULL) {
    if (dstrcmp (hash_table[temp_key].value, string) == 0)
      hash_table[temp_key].another_for_key++;
    temp_key = (temp_key + 1) % hash_table_size;
    }

  if (hash_table[temp_key].value == NULL) {
    hash_table[temp_key].hash_key = orig_hash_key;
    hash_table[temp_key].another_for_key = 0;
    hash_table[temp_key].value = string;
    return temp_key;
    }

  /* LATER add a reallocation/reorganization of hash table on the fly. */
  dstrfree (string);
  return -1;
  }
