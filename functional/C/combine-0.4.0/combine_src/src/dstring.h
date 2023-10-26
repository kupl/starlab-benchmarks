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

#ifndef DSTRING_H_GUARD
#define DSTRING_H_GUARD 1

#define _INTL_REDIRECT_MACROS
/* dstring.h -- Information for processing strings based on descriptors
 *              indicating the position and length rather than null-
 *              delimited strings.  This allows strings to nicely contain
 *              null characters.
 */

/* We want the length value to be as long as is possible.
 * Here, we're shooting for 2^64, since there exist computers
 * with memory too big for 2^32.  (Just planning for the future.)
 * It may be that we should make this the size of the pointer, but
 * I still like it as big as possible.
 */
#define HAVE_LONG_LONGS 1

#ifdef HAVE_LONG_LONGS
#define   DSTR_SIZE_TYPE size_t
#else /* no HAVE_LONG_LONGS */
#define   DSTR_SIZE_TYPE size_t
#endif /* no HAVE_LONG_LONGS */

#define DSTR_INVALID(String) (String) == NULL || ((String)->string == NULL && (String)->length != 0)

#define DSTR_EINVALID -1
#define DSTR_EMEMORY -2

enum DStr_codes
{
  DStr_own = 0,
  DStr_not_own,
  DStr_constant
  };

typedef enum DStr_codes DStr_codes;

struct DStr_string_descriptor
{
  DSTR_SIZE_TYPE length;
  DStr_codes own_string;
  unsigned char *string;
  };

typedef struct DStr_string_descriptor DStr_string_descriptor;

/* Emulate the standard string functions with the descriptor- or
 *   dynamic strings.
 */

/* String concatenation, and limited to a number of characters. */
DStr_string_descriptor *dstrcat (DStr_string_descriptor *,
				 DStr_string_descriptor *);
DStr_string_descriptor *dstrncat (DStr_string_descriptor *,
				  DStr_string_descriptor *, size_t);

/* Search for a character; first and last occurrence */
DSTR_SIZE_TYPE dstrchr (DStr_string_descriptor *, unsigned int);
DSTR_SIZE_TYPE dstrrchr (DStr_string_descriptor *, unsigned int);

/* String comparison, and limited to a number of characters. */
int dstrcmp (DStr_string_descriptor *, DStr_string_descriptor *);
int dstrncmp (DStr_string_descriptor *, DStr_string_descriptor *, size_t);

/* Case-insensitive string comparison, and limited to a number of characters. */
int dstrcasecmp (DStr_string_descriptor *, DStr_string_descriptor *);
int dstrncasecmp (DStr_string_descriptor *, DStr_string_descriptor *, size_t);

/* String copying, and limited to a number of characters. */
DStr_string_descriptor *dstrcpy (DStr_string_descriptor *,
				 DStr_string_descriptor *);
DStr_string_descriptor *dstrncpy (DStr_string_descriptor *,
				  DStr_string_descriptor *, size_t);

/* Get an error string based on an error code */
DStr_string_descriptor *dstrerror (int);

/* String length */
size_t dstrlen (DStr_string_descriptor *);

/* Find any of a set of characters in a string. (or reverse, not standard)*/
DSTR_SIZE_TYPE dstrpbrk (DStr_string_descriptor *, DStr_string_descriptor *);
DSTR_SIZE_TYPE dstrrpbrk (DStr_string_descriptor *, DStr_string_descriptor *);

/* Divide a string based on any delimiter in a set */
DStr_string_descriptor *dstrsep (DStr_string_descriptor *,
				 DStr_string_descriptor *);

/* Count the number of consecutive characters at the beginning of a string 
 * that are memebers of a given set of characters. And the complement of the set
 */
size_t dstrspn (DStr_string_descriptor *, DStr_string_descriptor *);
size_t dstrcspn (DStr_string_descriptor *, DStr_string_descriptor *);

/* Find the first occurrence of substring in a string */
DSTR_SIZE_TYPE dstrstr (DStr_string_descriptor *, DStr_string_descriptor *);

/* We won't do dstrtok as dstrsep does the same
DStr_string_descriptor*
dstrtok      (DStr_string_descriptor*, DStr_string_descriptor*);

Same with dstrindex as dstrchr does the same.
DSTR_SIZE_TYPE
dstrindex    (DStr_string_descriptor*, int);
DSTR_SIZE_TYPE
dstrrindex   (DStr_string_descriptor*, int);*/

/* Duplicate a dynamic string */
DStr_string_descriptor *dstrdup (DStr_string_descriptor *);

/* Duplicate a null-delimited string */
DStr_string_descriptor *dstrdupstr (char *);

/* Duplicate a descriptor string as a null-delimited string */
char *strdupdstr (DStr_string_descriptor *);

/* Connect a new string to an existing descriptor */
int dstrmapstr (DStr_string_descriptor *, char *, DStr_codes);

/* Create and free a dynamic descriptor string */
DStr_string_descriptor *dstrnew ();
void dstrfree (DStr_string_descriptor *);
DStr_string_descriptor *dstrmakeown (DStr_string_descriptor *);

DStr_string_descriptor *dstrsubstr (DStr_string_descriptor *, size_t, size_t,
				    size_t, DStr_codes);
int dstrsplit (DStr_string_descriptor *, DStr_string_descriptor *,
	       DStr_string_descriptor ***, int *);

DStr_string_descriptor *dstrextend (DStr_string_descriptor *, size_t, int);

#endif /* no DSTRING_H_GUARD */
