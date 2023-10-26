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
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include "dstring.h"
#include "df_common.h"
#include "field.h"
#include "iguile.h"


/********
 * Create a string from a set of ranges and an optional delimiter.
 * The OUT_STRING is a pointer to the dynamic result string.  The
 * INPUT_RECORD is a pointer to a string descriptor for the source of
 * the fields.  The RANGES is a pointer to an array of RANGE_COUNT
 * range specifier structures where we expect to find the fields.  If
 * END_RANGE_START is other than 0, it signifies a final field that
 * extends to the end of the record.  If the string descriptor pointer
 * INPUT_DELIMITER is not null, it indicates that character to use to
 * separate the string into fields.  In that case, the ranges are
 * field numbers, rather than bytes as otherwise.  The result will
 * have its fields delimited by the string pointed to by
 * OUTPUT_DELIMITER if it is not null.  */
int
find_input_ranges (out_string, input_record, ranges, range_count,
		   end_range_start, input_delimiter, input_delimiter_repeat,
		   output_delimiter)
     STRINGTYPE *out_string;
     STRINGTYPE *input_record;
     field_range *ranges;
     int range_count;
     size_t end_range_start;
     STRINGTYPE *input_delimiter;
     int input_delimiter_repeat;
     STRINGTYPE *output_delimiter;
{
  int i;
  int j;
  STRINGTYPE **field_list;
  int field_list_length;
  int return_code;

  if (input_delimiter != NULL) {
    return_code
      = dstrsplit (input_record, input_delimiter, &field_list,
		   &field_list_length);
    if (return_code != EXIT_SUCCESS) {
      if (return_code == DSTR_EMEMORY)
	FATAL_ERROR (_("unable to allocate memory"));
      else
	FATAL_ERROR (_("internal error: invalid string descriptor"));
      }
    if (input_delimiter_repeat) {
      i = 0;
      while (i < field_list_length) {
	if (field_list[i]->length == 0) {
	  for (j = i + 1; j < field_list_length; j++)
	    field_list[j - 1] = field_list[j];
	  field_list_length--;
	  }
	else
	  i++;
	}
      field_list
	= realloc (field_list, field_list_length * sizeof (*field_list));
      if (field_list == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    }

  for (i = 0; i < range_count; i++) {
    STRINGTYPE *temp_string;
    STRINGTYPE *other_string;

    if (out_string->length > 0 && output_delimiter != NULL)
      dstrcat (out_string, output_delimiter);
    if (input_delimiter != NULL) {
      if (ranges[i].lower_bound < 0 || ranges[i].lower_bound > field_list_length)
	FATAL_ERROR2 (_
		      ("bad field substring -- wanted field %ld from %d fields"),
		      ranges[i].lower_bound, field_list_length);
      temp_string = field_list[ranges[i].lower_bound - 1];
      }
    else {
      temp_string
	= dstrsubstr (input_record, ranges[i].lower_bound,
		      ranges[i].upper_bound, 0, DStr_not_own);
      if (temp_string == NULL) {
	if (ranges[i].lower_bound > 0
	    || ranges[i].lower_bound > ranges[i].upper_bound
	    || ranges[i].upper_bound > input_record->length)
	  FATAL_ERROR3 (_
			("bad field substring -- wanted bytes %ld-%ld in %ld-byte string"),
			ranges[i].lower_bound, ranges[i].upper_bound,
			input_record->length);
	else
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      }
#ifdef HAVE_LIBGUILE
    if (ranges[i].format != NULL) {	/* We have some processing to do. */
      SCM return_value;
      STRINGTYPE *result_string;
      size_t return_length;
      /* Assuming scheme for now */
      /* Assemble scheme code */
      create_scheme_string_variable (&df_name_input_field, 0, temp_string);

      /* Call Guile */
      result_string = eval_scheme_string_function (ranges[i].format);
      if (input_delimiter == NULL)
	dstrfree (temp_string);
      temp_string = result_string;
      }
#  endif /* HAVE_LIBGUILE */
    other_string = dstrcat (out_string, temp_string);
    if (other_string == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    if (input_delimiter == NULL || ranges[i].format != NULL)
      dstrfree (temp_string);
    }

  /* Note: we have not given the possibility of changing a record-ending
   *  key string with scheme extensibility.
   */
  if (end_range_start != 0) {
    STRINGTYPE *temp_string;
    STRINGTYPE *other_string;
    if (out_string->length > 0 && output_delimiter != NULL)
      dstrcat (out_string, output_delimiter);
    temp_string
      = dstrsubstr (input_record, end_range_start, input_record->length, 0,
		    DStr_not_own);
    if (temp_string == NULL) {
      if (end_range_start > input_record->length || end_range_start < 0)
	FATAL_ERROR3 (_
		      ("bad field substring -- wanted bytes %ld-%ld in %ld-byte string"),
		      end_range_start, input_record->length,
		      input_record->length);
      else
	FATAL_ERROR (_("unable to allocate memory"));
      }
    other_string = dstrcat (out_string, temp_string);
    if (other_string == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    dstrfree (temp_string);
    }

  if (input_delimiter != NULL) {
    int i;
    for (i = 0; i < field_list_length; i++)
      dstrfree (field_list[i]);
    free (field_list);
    }

  return EXIT_SUCCESS;
  }

/********
 * Create an arrax of field strings from a set of ranges and an
 * optional delimiter.  Generally the same as find_input_ranges above,
 * with a different target.  The additional parameter MAKE_COPY is
 * non-zero to indicate that we need to use the results after
 * INPUT_RECORD has been over-written.  The processing assumes that in
 * delimited files, each field will be referred to only once.  The
 * problem here is that we are handing the pointer off to the array,
 * so we can't just free it in the future.  Perhaps we need to give up
 * that efficiency for flexibility, or mark when the field is used and
 * then copy it in the future.  
 */
int
a_find_input_ranges (out_ranges, input_record, ranges, range_count,
		     end_range_start, input_delimiter, input_delimiter_repeat,
		     make_copy, output_delimiter, empty_replacement)
     STRINGTYPE ***out_ranges;
     STRINGTYPE *input_record;
     field_range *ranges;
     int range_count;
     size_t end_range_start;
     STRINGTYPE *input_delimiter;
     int input_delimiter_repeat;
     DStr_codes make_copy;
     STRINGTYPE *output_delimiter;
     STRINGTYPE *empty_replacement;
{
  int i;
  int j;
  STRINGTYPE **field_list;
  int field_list_length;
  int return_code;

  if (input_delimiter != NULL) {
    return_code
      = dstrsplit (input_record, input_delimiter, &field_list,
		   &field_list_length);
    if (return_code != EXIT_SUCCESS) {
      if (return_code == DSTR_EMEMORY)
	FATAL_ERROR (_("unable to allocate memory"));
      else
	FATAL_ERROR (_("internal error: invalid string descriptor"));
      }
    if (input_delimiter_repeat) {
      i = 0;
      while (i < field_list_length) {
	if (field_list[i]->length == 0) {
	  for (j = i + 1; j < field_list_length; j++)
	    field_list[j - 1] = field_list[j];
	  if (i < field_list_length - 1)
	    field_list_length--;
	  else
	    i++;
	  }
	else
	  i++;
	}
      field_list
	= realloc (field_list, field_list_length * sizeof (*field_list));
      if (field_list == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    }

  *out_ranges
    = malloc ((range_count + (end_range_start == 0 ? 0 : 1))
	      * sizeof (STRINGTYPE));
  if (*out_ranges == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < range_count; i++) {
    if (input_delimiter != NULL) {
      if (ranges[i].lower_bound <= field_list_length) {
	if (empty_replacement != NULL
	    && field_list[ranges[i].lower_bound - 1]->length == 0)
	  (*out_ranges)[i] = dstrdup (empty_replacement);
	else
	  (*out_ranges)[i] = dstrdup (field_list[ranges[i].lower_bound - 1]);
	}
      else if (empty_replacement != NULL)
	(*out_ranges)[i] = dstrdup (empty_replacement);
      else
	(*out_ranges)[i] = dstrdupstr ("");
      if ((*out_ranges)[i] == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    else {
      (*out_ranges)[i]
	= dstrsubstr (input_record, ranges[i].lower_bound,
		      ranges[i].upper_bound, 0, make_copy);
      if ((*out_ranges)[i] == NULL) {
	if (ranges[i].lower_bound < 1
	    || ranges[i].lower_bound > ranges[i].upper_bound
	    || ranges[i].upper_bound > input_record->length)
	  FATAL_ERROR3 (_
			("bad field substring -- wanted bytes %ld-%ld in %ld-byte string"),
			ranges[i].lower_bound, ranges[i].upper_bound,
			input_record->length);
	else
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      }
#ifdef HAVE_LIBGUILE
    if (ranges[i].format != NULL) {	/* We have some processing to do. */
      SCM return_value;
      STRINGTYPE *temp_string;
      size_t return_length;

      create_scheme_string_variable (&df_name_input_field, 0,
				     ((*out_ranges)[i]));

      /* Call Guile */
      temp_string = eval_scheme_string_function (ranges[i].format);
      dstrfree ((*out_ranges)[i]);
      (*out_ranges)[i] = temp_string;
      }
#  endif /* HAVE_LIBGUILE */
    }

  if (end_range_start != 0 && input_delimiter == NULL) {
    (*out_ranges)[i]
      = dstrsubstr (input_record, end_range_start, input_record->length, 0,
		    make_copy);
    if ((*out_ranges)[i] == NULL) {
      if (end_range_start < 1 || end_range_start > input_record->length)
	FATAL_ERROR3 (_
		      ("bad field substring -- wanted bytes %ld-%ld in %ld-byte string"),
		      end_range_start, input_record->length,
		      input_record->length);
      else
	FATAL_ERROR (_("unable to allocate memory"));
      }
    }
  else if (end_range_start != 0 && input_delimiter != NULL) {
    if (end_range_start <= field_list_length)
      (*out_ranges)[i] = dstrdup (field_list[end_range_start - 1]);
    else
      (*out_ranges)[i] = dstrdupstr ("");
    j = end_range_start;
    while (j < field_list_length) {
      /* If there are more fields, tack them on with the output delimiter if appropriate. */
      if (output_delimiter != NULL)
	dstrcat ((*out_ranges)[i], output_delimiter);
      if (empty_replacement == NULL || field_list[j]->length != 0)
	dstrcat ((*out_ranges)[i], field_list[j]);
      else
	dstrcat ((*out_ranges)[i], empty_replacement);
      j++;
      }
    }

  if (input_delimiter != NULL) {
    int i;
    for (i = 0; i < field_list_length; i++)
      dstrfree (field_list[i]);
    free (field_list);
    }

  return EXIT_SUCCESS;
  }



/********
 * Parse a STRING that contains a list of fields that are to be written in a particular order
 *   Return an array of indicators and set FIELD_COUNT to the length of the array.
 *   The string should be of the format fn.tn,...
 *   where f is r, d, or o for reference, data, other
 *   and t is o, k, f, n, s, e for outfield, costant, flag, counter, sum, or extensibility.
 */

field_id *
parse_field_order_string (string, field_count)
     STRINGTYPE *string;
     int *field_count;
{
  field_id *return_array;
  char *curr_pos;
  int i;
  STRINGTYPE **field_list;
  int field_list_length;
  int field_list_extra;
  int return_code;
  STRINGTYPE delimiter;

  delimiter.length = 1;
  delimiter.string = ",";
  field_list_extra = 0;

  return_code = dstrsplit (string, &delimiter, &field_list, &field_list_length);
  if (return_code != EXIT_SUCCESS) {
    if (return_code == DSTR_EMEMORY)
      FATAL_ERROR (_("unable to allocate memory"));
    else
      FATAL_ERROR (_("internal error: invalid string descriptor"));
    }

  if (field_list_length == 0)
    return NULL;

  return_array = malloc (field_list_length * sizeof (field_id));
  if (return_array == NULL)
    return NULL;

  for (i = 0; i < field_list_length; i++) {
    curr_pos = field_list[i - field_list_extra]->string;
    switch (*curr_pos) {
    case 'r':
    case 'd':
    case 'o':
    case 's':
      return_array[i].source = *curr_pos;
      break;
    default:
      FATAL_ERROR2 (_
		    ("field order specification starts with %c:  only %s are supported"),
		    *curr_pos, "r, d, o, s");
      }

    curr_pos++;
    return_array[i].source_num = strtol (curr_pos, &curr_pos, 0);
    if (errno == ERANGE
	&& (return_array[i].source_num == LONG_MIN
	    || return_array[i].source_num == LONG_MAX))
      FATAL_ERROR (_("number out of range in field order specification"));

    if (*curr_pos == '.')
      curr_pos++;
    else
      FATAL_ERROR2 (_("badly formed field order specification: %.*s"),
		    field_list[i - field_list_extra]->length,
		    field_list[i - field_list_extra]->string);

    switch (*curr_pos) {
    case 'o':
    case 'k':
    case 'f':
    case 'n':
    case 's':
    case 'e':
      return_array[i].type = *curr_pos;
      break;
    default:
      FATAL_ERROR3 (_
		    ("badly formed field order specification: type %c unknown: %.*s"),
		    *curr_pos, field_list[i - field_list_extra]->length,
		    field_list[i - field_list_extra]->string);
      }

    curr_pos++;
    if (*curr_pos == '-')
      FATAL_ERROR (_
		   ("field number started with -; use a positive number or range 1-n"));
    return_array[i].type_num = strtol (curr_pos, &curr_pos, 0);
    if (errno == ERANGE
	&& (return_array[i].type_num == LONG_MIN
	    || return_array[i].type_num == LONG_MAX))
      FATAL_ERROR (_("number out of range in field order specification"));

    if (*curr_pos == '-') {	/* Using a range of fields */
      int upper_bound;
      int lower_bound;
      int j;

      curr_pos++;
      if (*curr_pos == ',' || *curr_pos == '\0') {
	/* No range.  Go from whatever there was to the end. */
	return_array[i].type_num *= -1;
	/* A negative number will indicate that field and all after it. */
	}
      else {
	lower_bound = return_array[i].type_num + 1;
	upper_bound = strtol (curr_pos, &curr_pos, 0);
	if (errno == ERANGE && (upper_bound == LONG_MIN || upper_bound == LONG_MAX))
	  FATAL_ERROR (_("number out of range in field order specification"));

	field_list_length += upper_bound - lower_bound + 1;

	return_array
	  = realloc (return_array, field_list_length * sizeof (field_id));
	if (return_array == NULL)
	  return NULL;

	for (j = lower_bound; j <= upper_bound; j++) {
	  i++;
	  field_list_extra++;
	  return_array[i].source = return_array[i - 1].source;
	  return_array[i].source_num = return_array[i - 1].source_num;
	  return_array[i].type = return_array[i - 1].type;
	  return_array[i].type_num = j;
	  }
	}
      }
    }
  *field_count = field_list_length;
  return return_array;
  }


/********
 * Parse a STRING that contains a list of fields that are to be written in a particular order
 *   Return an array of indicators and set FIELD_COUNT to the length of the array.
 *   The string should be of the format m.n,...
 *   where m is 1 or 2 for file 1 or 2 in the join command, 
 *   and n is the field number.
 */

field_id *
parse_join_order_string (string, field_count)
     STRINGTYPE *string;
     int *field_count;
{
  field_id *return_array;
  char *curr_pos;
  int i;
  int field_list_length;
  int have_file;

  return_array = malloc (sizeof (field_id));
  if (return_array == NULL)
    FATAL_ERROR (_("unable to allocate memory"));

  curr_pos = string->string;
  i = 0;
  field_list_length = 0;
  have_file = 0;

  while (i < string->length) {
    if (*curr_pos == ',' || *curr_pos == ' ' || *curr_pos == '\t') {
      i++;
      curr_pos++;
      have_file = 0;
      field_list_length++;
      return_array
	= realloc (return_array, (field_list_length + 1) * sizeof (field_id));
      if (return_array == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    else if (have_file == 0) {
      switch (*curr_pos) {
      case '0':
	/* The key, wherever it came from. */
	i++;
	curr_pos++;
	if (i < string->length && *curr_pos != ',' && *curr_pos != ' ')
	  FATAL_ERROR (_("the join key (file 0) is misspecified"));
	return_array[field_list_length].source = 'r';
	return_array[field_list_length].source_num = 1;
	return_array[field_list_length].type = 'o';
	return_array[field_list_length].type_num = 1;
	field_list_length++;
	return_array
	  = realloc (return_array, (field_list_length + 1) * sizeof (field_id));
	if (return_array == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	return_array[field_list_length].source = 's';
	return_array[field_list_length].source_num = 0;
	return_array[field_list_length].type = 'o';
	return_array[field_list_length].type_num = 1;
	break;
	/* FIXME: This assumes the key field is field 1 only */
      case '1':
	return_array[field_list_length].source = 'r';
	return_array[field_list_length].source_num = 1;
	return_array[field_list_length].type = 'o';
	return_array[field_list_length].type_num = 0;
	have_file = -1;
	i++;
	curr_pos++;
	if (i >= string->length || *curr_pos != '.')
	  FATAL_ERROR (_("did not find '.' where expected in order"));
	i++;
	curr_pos++;
	break;
      case '2':
	return_array[field_list_length].source = 'd';
	return_array[field_list_length].source_num = 0;
	return_array[field_list_length].type = 'o';
	return_array[field_list_length].type_num = 0;
	have_file = -1;
	i++;
	curr_pos++;
	if (i >= string->length || *curr_pos != '.')
	  FATAL_ERROR (_("did not find '.' where expected in order"));
	i++;
	curr_pos++;
	break;
      default:
	FATAL_ERROR1 (_("file number must be 1 or 2 -- got %c"), *curr_pos);
	break;
	}
      }
    else {
      if (*curr_pos < '0' || *curr_pos > '9')
	FATAL_ERROR1 (_("non-digit in field number: '%c'"), *curr_pos);
      return_array[field_list_length].type_num *= 10;
      return_array[field_list_length].type_num += *curr_pos - '0';
      i++;
      curr_pos++;
      }
    }
  *field_count = field_list_length + 1;
  return return_array;
  }

/********
 * Identify a data field from a given record, INPUT_RECORD.  RANGES is
 *   an array of RANGE_COUNT field specifications.  WHICH_RANGE is the
 *   number of the field we want within the ranges.  If
 *   INPUT_DELIMITER is not null, use it's string as the separator
 *   between fields.  */
STRINGTYPE *
find_field (ranges, range_count, which_range, input_record, input_delimiter)
     field_range *ranges;
     int range_count;
     int which_range;
     STRINGTYPE *input_record;
     STRINGTYPE *input_delimiter;
{
  STRINGTYPE *return_string;
  STRINGTYPE **field_list;
  int field_list_length;
  int return_code;

  if (input_delimiter != NULL) {
    int i;

    return_code
      = dstrsplit (input_record, input_delimiter, &field_list,
		   &field_list_length);
    if (return_code != EXIT_SUCCESS) {
      if (return_code == DSTR_EMEMORY)
	FATAL_ERROR (_("unable to allocate memory"));
      else
	FATAL_ERROR (_("internal error: invalid string descriptor"));
      }

    return_string = dstrdup (field_list[ranges[which_range].lower_bound - 1]);
    for (i = 0; i < field_list_length; i++)
      dstrfree (field_list[i]);
    free (field_list);
    }
  else {
    if (ranges[which_range].lower_bound > input_record->length)
      return NULL;
    return_string
      = dstrsubstr (input_record, ranges[which_range].lower_bound,
		    ranges[which_range].upper_bound, 0, DStr_not_own);
    if (return_string == NULL) {
      if (ranges[which_range].lower_bound > 0
	  || ranges[which_range].lower_bound > ranges[which_range].upper_bound
	  || ranges[which_range].upper_bound > input_record->length)
	FATAL_ERROR3 (_
		      ("bad field substring -- wanted bytes %ld-%ld in %ld-byte string"),
		      ranges[which_range].lower_bound,
		      ranges[which_range].upper_bound, input_record->length);
      else
	FATAL_ERROR (_("unable to allocate memory"));
      }
    }

#ifdef HAVE_LIBGUILE
  if (ranges[which_range].format != NULL) {	/* We have some processing to do. */
    SCM return_value;
    STRINGTYPE *temp_string;
    size_t return_length;
    /* Assuming scheme for now */
    /* Put the string into a scheme variable */
    create_scheme_string_variable (&df_name_sum_field, which_range,
				   return_string);
    /* Call Guile */
    temp_string = eval_scheme_string_function (ranges[which_range].format);
    dstrfree (return_string);
    return_string = temp_string;
    }
#  endif /* HAVE_LIBGUILE */

  return return_string;
  }

int
fill_empty_fields (fields, field_count, empty_replacement)
     STRINGTYPE **fields;
     size_t field_count;
     STRINGTYPE *empty_replacement;
{
  size_t i;
  for (i = 0; i < field_count; i++) {
    if (fields[i]->length == 0) {
      dstrfree (fields[i]);
      fields[i] = dstrdup (empty_replacement);
      if (fields[i] == NULL)
	return EXIT_FAILURE;
      }
    }
  return EXIT_SUCCESS;
  }
