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

/* dstring.c -- a library of routines for working with dynamic described strings */
#define _INTL_REDIRECT_MACROS

#include "../config.h"
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_LIBINTL_H
#  include <libintl.h>
#  endif
#include "dstring.h"

#ifdef HAVE_GETTEXT
#  define _(String) gettext (String)
#else
#  define _(String) (String)
#  endif

#ifdef HAVE_LONG_LONG
#  define BIGNUMBER long long
#else
#  define BIGNUMBER long
#  endif

#ifdef HAVE_STRTOLL
#  define STRTOBIGNUMBER strtoll
#else
#  define STRTOBIGNUMBER strtol
#  endif

#define min(x,y) (x>y?y:x)
#define max(x,y) (x>y?x:y)

/* Concatenate APPEND to the end of TARGET */
DStr_string_descriptor *
dstrcat (target, append)
     DStr_string_descriptor *target;
     DStr_string_descriptor *append;
{
  char *string_pointer;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (target))
    return NULL;
  if (DSTR_INVALID (append))
    return NULL;

  if (append->length == 0)
    return target;

  if (target->own_string == DStr_not_own) {
    DStr_string_descriptor *temp_string;
    temp_string = dstrmakeown (target);
    if (temp_string == NULL)
      return NULL;
    }

  if (target->length == 0)
    string_pointer = malloc (append->length);
  else
    string_pointer = realloc (target->string, target->length + append->length);
  if (string_pointer == NULL)
    return NULL;

  target->string = string_pointer;
  memmove (target->string + target->length, append->string, append->length);

  target->length = target->length + append->length;

  return target;
  }

/* Concatenate up to LEN bytes from APPEND to the end of TARGET */
DStr_string_descriptor *
dstrncat (target, append, len)
     DStr_string_descriptor *target;
     DStr_string_descriptor *append;
     size_t len;
{
  char *string_pointer;
  size_t move_length;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (target))
    return NULL;
  if (DSTR_INVALID (append))
    return NULL;

  move_length = min (append->length, len);

  if (move_length == 0)
    return target;

  if (target->own_string == DStr_not_own) {
    DStr_string_descriptor *temp_string;
    temp_string = dstrmakeown (target);
    if (temp_string == NULL)
      return NULL;
    }

  if (target->length == 0)
    string_pointer = malloc (move_length);
  else
    string_pointer = realloc (target->string, target->length + move_length);
  if (string_pointer == NULL)
    return NULL;

  target->string = string_pointer;
  memmove (target->string + target->length, append->string, move_length);

  target->length = target->length + move_length;

  return target;
  }


/* Find the fist occurrence of the character C in STRING. */
/* LATER consider using memchr rather than repeating the search
 * myself.  
 */
DSTR_SIZE_TYPE
dstrchr (string, c)
     DStr_string_descriptor *string;
     unsigned int c;
{
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (c > 255)
    return DSTR_EINVALID;

  /* Now review */
  for (i = 0; i < string->length; i++)
    if (string->string[i] == c)
      return i + 1;

  return 0;

  }

/* Find the last occurrence of character C in STRING */
DSTR_SIZE_TYPE
dstrrchr (string, c)
     DStr_string_descriptor *string;
     unsigned int c;
{
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (c > 255)
    return DSTR_EINVALID;

  /* Now review */
  for (i = string->length - 1; i >= 0; i--)
    if (string->string[i] == c)
      return i + 1;

  return 0;

  }


/* Compare STRING1 and STRING2.  Return positive is STRING1 is
   greater, negative is STRING2 is greater, zero otherwise.*/
int
dstrcmp (string1, string2)
     DStr_string_descriptor *string1;
     DStr_string_descriptor *string2;
{
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string1))
    return DSTR_EINVALID;
  if (DSTR_INVALID (string2))
    return DSTR_EINVALID;

  if (string1->length == 0) {
    if (string2->length == 0)
      return 0;
    else
      return -1;
    }
  else if (string2->length == 0)
    return 1;

  /* Now review */
  for (i = 0; i < min (string1->length, string2->length); i++)
    if (string1->string[i] != string2->string[i])
      return string1->string[i] - string2->string[i];

  /* If we got this far, the common length is equal, so the longer will win. */
  return string1->length - string2->length;
  }

/* Compare up to LEN bytes from STRING1 and STRING2.  Return positive
   is STRING1 is greater, negative is STRING2 is greater, zero
   otherwise.*/
int
dstrncmp (string1, string2, len)
     DStr_string_descriptor *string1;
     DStr_string_descriptor *string2;
     size_t len;
{
  DSTR_SIZE_TYPE i;
  DSTR_SIZE_TYPE upper_common_length;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string1))
    return DSTR_EINVALID;
  if (DSTR_INVALID (string2))
    return DSTR_EINVALID;
  if (len == 0)
    return 0;

  if (string1->length == 0) {
    if (string2->length == 0)
      return 0;
    else
      return -1;
    }
  else if (string2->length == 0)
    return 1;

  /* Find the length of time that we can compare them all in common. */
  upper_common_length = min (string1->length, string2->length);
  if (len < upper_common_length)
    upper_common_length = len;

  /* Now review */
  for (i = 0; i < upper_common_length; i++)
    if (string1->string[i] != string2->string[i])
      return string1->string[i] - string2->string[i];

  /* If we got this far, the common length is equal, so the longer will win. */
  if (string1->length >= len && string2->length >= len)
    return 0;
  return string1->length - string2->length;
  }

/* Compare STRING1 and STRING2.  Return positive is STRING1 is
   greater, negative is STRING2 is greater, zero otherwise.  Ignore
   case.  Currently uses strncasecmp, so will not work properly for
   strings with null characters in them.*/
int
dstrcasecmp (string1, string2)
     DStr_string_descriptor *string1;
     DStr_string_descriptor *string2;
{
  int return_value;
  DSTR_SIZE_TYPE upper_common_length;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string1))
    return DSTR_EINVALID;
  if (DSTR_INVALID (string2))
    return DSTR_EINVALID;

  if (string1->length == 0) {
    if (string2->length == 0)
      return 0;
    else
      return -1;
    }
  else if (string2->length == 0)
    return 1;

  /* Find the length of time that we can compare them all in common. */
  upper_common_length = min (string1->length, string2->length);

  /* Now review */
  return_value
    = strncasecmp (string1->string, string2->string, upper_common_length);

  if (return_value == 0)
    return_value = string1->length - string2->length;

  return return_value;
  }

/* Compare up to LEN bytes from STRING1 and STRING2.  Return positive
   is STRING1 is greater, negative is STRING2 is greater, zero
   otherwise.  Ignore case.  Currently uses strncasecmp, so will not
   work properly for strings with null characters in them.*/
int
dstrncasecmp (string1, string2, len)
     DStr_string_descriptor *string1;
     DStr_string_descriptor *string2;
     size_t len;
{
  int return_value;
  DSTR_SIZE_TYPE upper_common_length;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string1))
    return DSTR_EINVALID;
  if (DSTR_INVALID (string2))
    return DSTR_EINVALID;
  if (len == 0)
    return 0;

  if (string1->length == 0) {
    if (string2->length == 0)
      return 0;
    else
      return -1;
    }
  else if (string2->length == 0)
    return 1;

  /* Find the length of time that we can compare them all in common. */
  upper_common_length = min (string1->length, string2->length);
  if (len < upper_common_length)
    upper_common_length = len;

  /* Now review */
  return_value
    = strncasecmp (string1->string, string2->string, upper_common_length);

  if (return_value == 0) {
    if (string1->length >= len && string2->length >= len)
      return_value = 0;
    else
      return_value = string1->length - string2->length;
    }

  return return_value;
  }


/* Copy the value of SOURCE to TARGET, allocating a new string, but
   using an existing descriptor. */
DStr_string_descriptor *
dstrcpy (target, source)
     DStr_string_descriptor *target;
     DStr_string_descriptor *source;
{
  char *string_pointer;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (target))
    return NULL;
  if (DSTR_INVALID (source))
    return NULL;

  if (target->own_string == DStr_not_own) {
    DStr_string_descriptor *temp_string;
    temp_string = dstrmakeown (target);
    if (temp_string == NULL)
      return NULL;
    }

  if (source->length == 0) {
    if (target->string != NULL) {
      free (target->string);
      target->string = NULL;
      }
    target->length = 0;
    target->own_string = DStr_not_own;
    }

  if (target->length == 0)
    string_pointer = malloc (source->length);
  else
    string_pointer = realloc (target->string, source->length);
  if (string_pointer == NULL)
    return NULL;

  target->string = string_pointer;
  memmove (target->string, source->string, source->length);

  target->length = source->length;

  return target;
  }

/* Copy the up to LEN bytes of SOURCE to TARGET, allocating a new
   string, but using an existing descriptor. */
DStr_string_descriptor *
dstrncpy (target, source, len)
     DStr_string_descriptor *target;
     DStr_string_descriptor *source;
     size_t len;
{
  char *string_pointer;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (target))
    return NULL;
  if (DSTR_INVALID (source))
    return NULL;

  len = min (source->length, len);

  if (target->own_string == DStr_not_own) {
    DStr_string_descriptor *temp_string;
    temp_string = dstrmakeown (target);
    if (temp_string == NULL)
      return NULL;
    }

  if (len == 0) {
    if (target->string != NULL) {
      free (target->string);
      target->string = NULL;
      }
    target->length = 0;
    target->own_string = DStr_not_own;
    }

  if (target->length == 0)
    string_pointer = malloc (len);
  else
    string_pointer = realloc (target->string, len);
  if (string_pointer == NULL)
    return NULL;

  target->string = string_pointer;
  memmove (target->string, source->string, len);

  target->length = len;

  return target;
  }


/* Get the error for ERRCODE in a new string descriptor */
DStr_string_descriptor *
dstrerror (errcode)
     int errcode;
{
  return dstrdupstr (strerror (errcode));
  }


/* Return the length of STRING */
size_t
dstrlen (string)
     DStr_string_descriptor *string;
{
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  return string->length;
  }


/* Return the position within STRING of the first character in CHARSET
   that appears there.  Return one more than the length of STRING if
   none are found. */
DSTR_SIZE_TYPE
dstrpbrk (string, charset)
     DStr_string_descriptor *string;
     DStr_string_descriptor *charset;
{
  char charlist[256];
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (DSTR_INVALID (charset))
    return DSTR_EINVALID;

  if (charset->length == 0)
    return string->length + 1;
  if (string->length == 0)
    return string->length + 1;

  /* Set up the bitmap for the review. */
  bzero (charlist, 256);
  for (i = 0; i < charset->length; i++)
    charlist[charset->string[i]] = -1;

  /* Now review */
  for (i = 0; i < string->length; i++)
    if (charlist[string->string[i]] != 0)
      return i + 1;

  return string->length + 1;

  }

/* Return the position within STRING of the lase character in CHARSET
   that appears there.  Return one more than the length of STRING if
   none are found. */
DSTR_SIZE_TYPE
dstrrpbrk (string, charset)
     DStr_string_descriptor *string;
     DStr_string_descriptor *charset;
{
  char charlist[256];
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (DSTR_INVALID (charset))
    return DSTR_EINVALID;

  if (charset->length == 0)
    return 0;
  if (string->length == 0)
    return 0;

  /* Set up the bitmap for the review. */
  bzero (charlist, 256);
  for (i = 0; i < charset->length; i++)
    charlist[charset->string[i]] = -1;

  /* Now review */
  for (i = string->length - 1; i >= 0; i--)
    if (charlist[string->string[i]] != 0)
      return i + 1;

  return 0;

  }

/* Return a newly-allocated substring from the beginning of STRING
   prior to the first occurrence of a character from CHARSET
   therein. */
DStr_string_descriptor *
dstrsep (string, charset)
     DStr_string_descriptor *string;
     DStr_string_descriptor *charset;
{
  DStr_string_descriptor *return_value;
  char *temp_string;

  if (DSTR_INVALID (string))
    return NULL;
  if (DSTR_INVALID (charset))
    return NULL;

  return_value = dstrnew ();
  if (return_value == NULL)
    return NULL;

  /* Find a delimiter character and make the return value. */
  return_value->length = dstrpbrk (string, charset) - 1;
  return_value->own_string = DStr_own;
  return_value->string = malloc (return_value->length);
  if (return_value->string == NULL) {
    dstrfree (return_value);
    return NULL;
    }
  memmove (return_value->string, string->string, return_value->length);

  /* Reset the original string. */
  string->length = string->length - return_value->length - 1;
  if (string->length == 0) {
    if (string->own_string == DStr_own)
      free (string->string);
    string->string = NULL;
    string->own_string = DStr_not_own;
    }
  else {
    temp_string = malloc (string->length);
    if (temp_string == NULL) {
      dstrfree (return_value);
      return NULL;
      }
    memmove (temp_string, string->string + return_value->length + 1,
	     string->length);
    if (string->own_string == DStr_own)
      free (string->string);
    string->string = temp_string;
    string->own_string = DStr_own;
    }

  return return_value;

  }

/* Return number of characters at the beginning of STRING consisting
   exclusively of characters in CHARSET. */
size_t
dstrspn (string, charset)
     DStr_string_descriptor *string;
     DStr_string_descriptor *charset;
{
  char charlist[256];
  DSTR_SIZE_TYPE i;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (DSTR_INVALID (charset))
    return DSTR_EINVALID;

  /* Set up the bitmap for the review. */
  bzero (charlist, 256);
  for (i = 0; i < charset->length; i++)
    charlist[charset->string[i]] = -1;

  /* Now review */
  for (i = 0; i < string->length; i++)
    if (charlist[string->string[i]] == 0)
      return i;

  return string->length;
  }

/* Return the number of strings at the beginning of STRING that are
   not members of CHARSET. */
size_t
dstrcspn (string, charset)
     DStr_string_descriptor *string;
     DStr_string_descriptor *charset;
{
  return dstrpbrk (string, charset) - 1;
  }


/* Return the position of the first occurrence of SUBSTRING in STRING */
DSTR_SIZE_TYPE
dstrstr (string, substring)
     DStr_string_descriptor *string;
     DStr_string_descriptor *substring;
{
  unsigned char *curr_string_pos;

  /* Check the consistency of the arguments. */
  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (DSTR_INVALID (substring))
    return DSTR_EINVALID;

  /* If the substring won't fit, we're out. */
  if (string->length < substring->length || string->length == 0)
    return 0;

  /* If the substring has length zero, we will say by convention that it 
     appears after the first byte in the string.  Otherwise, we could 
     have a million of them stacked up at the front. */
  if (substring->length == 0)
    return 2;

  curr_string_pos = string->string;

  while (curr_string_pos + (substring->length) - 1
	 < string->string + string->length) {
    unsigned char *curr_substring_pos;

    curr_substring_pos = substring->string;
    while (*curr_substring_pos == *curr_string_pos
	   && curr_substring_pos < substring->string + substring->length) {
      curr_substring_pos++;
      curr_string_pos++;
      }
    if ((curr_substring_pos - substring->string) == substring->length)
      return (curr_string_pos - string->string) - (curr_substring_pos
						   - substring->string) + 1;
    else
      curr_string_pos
	= curr_string_pos - (curr_substring_pos - substring->string);
    curr_string_pos++;
    }
  return 0;
  }


/* Return a newly allocated descriptor and and string copying
   STRING. */
DStr_string_descriptor *
dstrdup (string)
     DStr_string_descriptor *string;
{
  DStr_string_descriptor *return_value;

  if (DSTR_INVALID (string))
    return NULL;

  return_value = dstrnew ();
  if (return_value == NULL)
    return NULL;

  return_value->length = string->length;
  if (string->length == 0) {
    return_value->string = NULL;
    }
  else {
    return_value->own_string = DStr_own;
    return_value->string = malloc (return_value->length);
    if (return_value->string == NULL) {
      free (return_value);
      return NULL;
      }

    memmove (return_value->string, string->string, return_value->length);
    }

  return return_value;
  }

/* Return an newly allocated descriptor pointing to a newly allocated
   copy of the 0-delimited string CSTRING. */
DStr_string_descriptor *
dstrdupstr (cstring)
     char *cstring;
{
  DStr_string_descriptor *return_value;

  return_value = dstrnew ();
  if (return_value == NULL)
    return NULL;

  return_value->length = strlen (cstring);
  if (return_value->length == 0) {
    return_value->string = NULL;
    }
  else {
    return_value->own_string = DStr_own;
    return_value->string = malloc (return_value->length);
    if (return_value->string == NULL) {
      free (return_value);
      return NULL;
      }

    memmove (return_value->string, cstring, return_value->length);
    }

  return return_value;
  }

/* Return a newly allocated 0-delimited string consisting of the
   string pointed to by STRING. */
/* LATER consider checking for a null character in the string. */
char *
strdupdstr (string)
     DStr_string_descriptor *string;
{
  char *return_value;

  if (DSTR_INVALID (string))
    return NULL;

  return_value = malloc (string->length + 1);
  if (return_value == NULL)
    return NULL;

  strncpy (return_value, string->string, string->length);
  return_value[string->length] = '\0';

  return return_value;
  }

/* Point an existing DESCRIPTOR at an existing CSTRING.  OWN_STRING
   indicates whether the descriptor will be responsible for freeing
   the string. */
int
dstrmapstr (descriptor, cstring, own_string)
     DStr_string_descriptor *descriptor;
     char *cstring;
     DStr_codes own_string;
{
  if (DSTR_INVALID (descriptor))
    return EXIT_FAILURE;
  if (descriptor->own_string == DStr_own)
    free (descriptor->string);
  descriptor->length = strlen (cstring);
  descriptor->own_string = own_string;
  descriptor->string = cstring;
  return EXIT_SUCCESS;
  }


/* Return a newly allocated string descriptor with zero length and
   (currently) a 1-byte allocated string. */
DStr_string_descriptor *
dstrnew ()
{
  DStr_string_descriptor *return_value;

  return_value = malloc (sizeof (DStr_string_descriptor));
  if (return_value == NULL)
    return NULL;

  return_value->length = 0;
  return_value->own_string = DStr_not_own;
  return_value->string = NULL;
  return return_value;
  }

/* Free the string descriptor STRING and possibly its string if we own
   it. */
void
dstrfree (string)
     DStr_string_descriptor *string;
{
  if (string == NULL)
    return;
  if (string->own_string == DStr_own && string->string != NULL)
    free (string->string);
  string->string = NULL;
  free (string);
  }

/* Make a the string pointed to by STRING a copy of the one that it
   currently points to, which we don't own. */
DStr_string_descriptor *
dstrmakeown (string)
     DStr_string_descriptor *string;
{
  char *new_string;

  if (string->length != 0) {
    new_string = malloc (string->length);
    if (new_string == NULL)
      return NULL;

    memmove (new_string, string->string, string->length);
    string->string = new_string;
    }
  else if (string->string != NULL) {
    /* Assume a 1-byte null string */
    new_string = malloc (1);
    if (new_string == NULL)
      return NULL;
    new_string[0] = '\0';
    string->string = new_string;
    }
  string->own_string = DStr_own;

  return string;
  }


/* Return a newly allocated substring of STRING starting at START and
   either ending at END or with length LENGTH, depending on which is
   non-zero.  OWN_STRING indicates whether to copy the value from
   STRING, or just point into it. */
DStr_string_descriptor *
dstrsubstr (string, start, end, length, own_string)
     DStr_string_descriptor *string;
     size_t start;
     size_t end;
     size_t length;
     DStr_codes own_string;
{
  DStr_string_descriptor *return_val;

  if (DSTR_INVALID (string))
    return NULL;


  if ((end == 0 && length == 0) || string->length == 0) {
    return_val = dstrnew ();
    if (return_val == NULL)
      return NULL;
    return_val->string = NULL;
    return_val->length = 0;
    return_val->own_string = DStr_not_own;
    return return_val;
    }

  if (length == 0)
    length = end - start + 1;
  else if (end == 0)
    end = start + length - 1;
  else if (start == 0)
    start = end - length + 1;

  if (end < start || start + length - 1 != end)
    return NULL;
  if (end > string->length)
    return NULL;

  return_val = dstrnew ();
  if (return_val == NULL)
    return NULL;

  return_val->own_string = own_string;
  return_val->length = length;
  if (return_val->own_string == DStr_own) {
    return_val->string = malloc (return_val->length);
    if (return_val->string == NULL) {
      dstrfree (return_val);
      return NULL;
      }
    memmove (return_val->string, string->string + (start - 1),
	     return_val->length);
    }
  else {
    return_val->string = (string->string + (start - 1));
    }

  return return_val;
  }

/* Extend STRING by LEN bytes, filling them with character C. */
DStr_string_descriptor *
dstrextend (string, len, c)
     DStr_string_descriptor *string;
     size_t len;
     int c;
{
  if (DSTR_INVALID (string))
    return NULL;

  if (string->length == 0)
    string->string = malloc (len);
  else
    string->string = realloc (string->string, string->length + len);
  if (string->string == NULL)
    return NULL;
  memset (string->string + string->length, c, len);
  string->length += len;

  return string;
  }

/* Create RETURN_ARRAY as a newly allocated array of all the
   substrings of STRING formed by starting and stopping them on
   occurrences of DELIMITER.  Put the length of the array into
   RETURN_LENGTH.  The values in the array are not owned by the
   descriptors, and depend on the continuing existence of STRING.  */
int
dstrsplit (string, delimiter, return_array, return_length)
     DStr_string_descriptor *string;
     DStr_string_descriptor *delimiter;
     DStr_string_descriptor ***return_array;
     int *return_length;
{
  DStr_string_descriptor *remaining_string;
  int next_pos;

  if (DSTR_INVALID (string))
    return DSTR_EINVALID;
  if (DSTR_INVALID (delimiter))
    return DSTR_EINVALID;

  *return_length = 0;
  *return_array = malloc (sizeof (*return_array));
  if (*return_array == NULL)
    return DSTR_EMEMORY;

  remaining_string = dstrsubstr (string, 1, 0, string->length, DStr_not_own);
  next_pos = dstrstr (remaining_string, delimiter);
  while (next_pos > 0) {

    (*return_length)++;
    *return_array
      = realloc (*return_array, (*return_length) * sizeof (*return_array));
    if (*return_array == NULL)
      return DSTR_EMEMORY;

    (*return_array)[*return_length - 1]
      = dstrsubstr (remaining_string, 1, next_pos - 1, 0, DStr_not_own);
    if ((*return_array)[*return_length - 1] == NULL)
      return DSTR_EMEMORY;

    remaining_string->string += (next_pos - 1) + delimiter->length;
    remaining_string->length -= (next_pos - 1) + delimiter->length;

    next_pos = dstrstr (remaining_string, delimiter);
    }

  (*return_length)++;
  *return_array
    = realloc (*return_array, (*return_length) * sizeof (*return_array));
  if (*return_array == NULL)
    return DSTR_EMEMORY;

  if (remaining_string->length > 0)
    (*return_array)[*return_length - 1]
      = dstrsubstr (remaining_string, 1, 0, remaining_string->length,
		    DStr_not_own);
  else
    (*return_array)[*return_length - 1] = dstrdupstr ("");
  if ((*return_array)[*return_length - 1] == NULL)
    return DSTR_EMEMORY;

  dstrfree (remaining_string);
  return EXIT_SUCCESS;

  }

/* Convert the contents of a dynamic string to a number.  Assume that
 * there are not null characters in the string.  Return the number.
 * If REMAINDER is non-null, set it to a dynamic string pointing to the
 * string after the number, if any.
 */
BIGNUMBER
dstrtonum (string, remainder, base)
     DStr_string_descriptor *string;
     DStr_string_descriptor **remainder;
     int base;
{
  BIGNUMBER return_val;
  char *temp_string;
  char *temp_remainder;

  if (remainder != NULL)
    *remainder = string;

  temp_string = malloc (string->length + 1);
  if (temp_string == NULL)
    return 0;

  memcpy (temp_string, string->string, string->length);
  temp_string[string->length] = '\0';

  return_val = STRTOBIGNUMBER (temp_string, &temp_remainder, base);
  if (*temp_remainder == '\0') {
    /* Either we used the wholde string, or there was an embedded null. */
    if (temp_remainder - temp_string == string->length) {
      if (remainder != NULL)
	*remainder = NULL;
      free (temp_string);
      return return_val;
      }
    }

  if (remainder != NULL) {
    *remainder = dstrnew ();
    if (*remainder == NULL) {
      *remainder = string;
      free (temp_string);
      return return_val;
      }
    (*remainder)->length = string->length - (temp_remainder - temp_string);
    (*remainder)->string = string->string + (temp_remainder - temp_string);
    }

  return return_val;

  }

#ifdef DSTR_TEST_MAIN
#include <stdio.h>
/* An old test main program to validate some functions.  It is left
   here for debugging only. */
int
main (argc, argv)
     int argc;
     char **argv;
{
  DStr_string_descriptor *string1;
  DStr_string_descriptor *string2;
  DStr_string_descriptor *string3;

  string1 = dstrdupstr ("This is a test.");
  string2 = dstrdup (string1);
  dstrcat (string2, string1);
  string3 = dstrdupstr ("st");

  fprintf (stderr, "%.*s\n", string2->length, string2->string);
  fprintf (stderr, "The first 't' is at position %d.\n",
	   dstrchr (string2, 't'));
  fprintf (stderr, "The last 't' is at position %d.\n",
	   dstrrchr (string2, 't'));
  fprintf (stderr, "The first member of [st] is at position %d.\n",
	   dstrpbrk (string2, string3));
  fprintf (stderr, "The last member of [st] is at position %d.\n",
	   dstrrpbrk (string2, string3));
  fprintf (stderr, "The length of the string is %d.\n", dstrlen (string2));

  dstrfree (string3);
  string3 = dstrdupstr (" 	.,()[]{};:!?\n\r");
  fprintf (stderr,
	   "The string starts with %d whitespace or punctuation characters.\n",
	   dstrspn (string2, string3));
  fprintf (stderr,
	   "The string starts with %d non-whitespace or punctuation characters.\n",
	   dstrcspn (string2, string3));

  dstrfree (string3);
  string3 = dstrdupstr ("test");
  fprintf (stderr, "The first occurrence of 'test' is at %d.\n",
	   dstrstr (string2, string3));

  dstrfree (string3);
  string3 = dstrdupstr ("tesg");
  fprintf (stderr, "The first occurrence of 'tesg' is at %d.\n",
	   dstrstr (string2, string3));


  dstrfree (string1);
  dstrfree (string3);
  string3 = dstrdupstr (". ");
  string1 = dstrsep (string2, string3);
  fprintf (stderr, "The first word is '%.*s'.\n", string1->length,
	   string1->string);
  fprintf (stderr, "The remainder is '%.*s'.\n", string2->length,
	   string2->string);

  dstrfree (string1);
  dstrfree (string2);
  dstrfree (string3);
  return EXIT_SUCCESS;
  }

#endif
