/* combine - flexible file merging    
   Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine

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

#ifndef FIELD_H_GUARD
#define FIELD_H_GUARD

/* A structure to internally define a field.  It consists of a source,
   a type, and a sequence number in each range.  Among the sources,
   r=reference d=data o=other.  In the types, o=outfield k=constant
   f=flag n=counter s=sum e=extensibility.  */
struct s_field_id
{
  char source;
  int source_num;
  char type;
  int type_num;
  };

typedef struct s_field_id field_id;

/**********
 * Macros to deal with the semi-unstructured flags and counters.
 */
/* How many of the flag-counter fields we don't plan to write. */
#define SKIP_COUNTERS (2 - gi_counter_ind - gi_flag_ind)
/* How to tell if the flag or the counter is first in the first two flag-counter fields. */
#define FLAG_INDEX (gi_counter_ind == 1 || gi_flag_ind == 0 ? 0 : 1)
#define COUNTER_INDEX (gi_counter_ind == 1 || gi_flag_ind == 0 ? 1 : 0)
/* Always have a flag and a counter for statistics if nothing else.  The additional sum fields come after the 2. */
#define NUM_COUNTERS (2 + gi_data_sum_range_count + (gi_data_sum_end_start > 0 ? 1 : 0))
/**********/

/* How many ranges do we have to work with in the reference file? */
#define TOTAL_REF_OUT_RANGES(n) (gra_reference_file_info[(n)].out_range_count + \
                                 (gra_reference_file_info[(n)].out_end_start > 0 ? 1 : 0) + \
                                 (gra_reference_file_info[(n)].constant_string == NULL ? 0 : 1))

#define ADD_RANGE_PAIR(ranges, low, high, precision, comment)         \
  do {                                                                \
    if (n_ranges >= n_ranges_allocated) {                             \
      n_ranges_allocated *= 2;                                        \
      (ranges) = (field_range *) realloc ((char *) (ranges),          \
                                          n_ranges_allocated * sizeof (*(ranges)));     \
      }                                                               \
    ranges[n_ranges].lower_bound = (low);                             \
    ranges[n_ranges].upper_bound = (high);                            \
    ranges[n_ranges].precision = (precision);                         \
    if (comment == NULL) ranges[n_ranges].format = NULL;              \
    else ranges[n_ranges].format = strdup((comment));                 \
    ++n_ranges;                                                       \
    } while (0)



struct s_field_range
{
  long int lower_bound;
  long int upper_bound;
  int precision;
  char *format;
  };

typedef struct s_field_range field_range;

int a_find_input_ranges (STRINGTYPE ***, STRINGTYPE *, field_range *, int,
			 size_t, STRINGTYPE *, int, DStr_codes, STRINGTYPE *,
			 STRINGTYPE *);
STRINGTYPE *find_field (field_range *, int, int, STRINGTYPE *, STRINGTYPE *);
int find_input_ranges (STRINGTYPE *, STRINGTYPE *, field_range *, int, size_t,
		       STRINGTYPE *, int, STRINGTYPE *);
field_id *parse_field_order_string (STRINGTYPE *, int *);
field_id *parse_join_order_string (STRINGTYPE *, int *);
int fill_empty_fields (STRINGTYPE **, size_t, STRINGTYPE *);

#endif /* No FIELD_H_GUARD */
