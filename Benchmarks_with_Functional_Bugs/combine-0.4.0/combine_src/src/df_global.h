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

#ifndef DF_GLOBAL_H_GUARD
#define DF_GLOBAL_H_GUARD

#include <stdlib.h>
#include <stdio.h>
#include "df_common.h"
#include "dstring.h"
#include "field.h"

enum field_type
{
  undefined_mode,

  /* Output characters that are in the given bytes. */
  byte_mode,

  /* Output the given delimeter-separated fields. */
  field_mode
  };

extern enum field_type field_type;

enum hm_t
{
  hm_beginning,
  hm_end,
  hm_number,
  hm_binary,
  hm_binary_long,
  };

typedef enum hm_t hm_t;

/* The flag to indicate a record count. */
extern int count_records;

/* The delimeter character for input fields. */
extern int input_delimiter;

/* The output field separator string.   */
extern STRINGTYPE *output_field_delimiter;

extern char **filenames;
extern int filecount;
extern char *input_filename;
extern char *output_filename;

extern FILE *input_file;
extern FILE *output_file;


struct s_ref_file_info
{
  char *file_name;
  unsigned char *input_buffer;
  size_t input_buffer_size;
  size_t input_buffer_pos;
  unsigned char *output_buffer;
  size_t output_buffer_size;
  size_t output_buffer_pos;
  field_range *out_ranges;
  int out_end_start;
  int out_range_count;
  field_range *key_ranges;
  int key_end_start;
  int key_range_count;
  field_range *data_key_ranges;
  int data_key_end_start;
  int data_key_range_count;
  field_range *hrchy_key_ranges;
  int hrchy_key_end_start;
  int hrchy_key_range_count;
  int hrchy_flatten_levels;
  int hrchy_leaf_only;
  int hrchy_direction;
  int unique_ind;
  int optional_ind;
  int exclude_match_ind;
  int write_ind;
  int suppress_empty_ind;
  STRINGTYPE *constant_string;
  STRINGTYPE *missing_string;
  STRINGTYPE *empty_string;
  STRINGTYPE *input_field_delimiter;
  int no_input_field_delimiter;
  int input_field_delimiter_repeat;
  STRINGTYPE *input_record_delimiter;
  int input_record_length;
  int total_output_length;
  char *output_file_name;
  int output_field_count;
  field_id *output_field_order;
  int output_field_order_count;
  long int hash_size;
  int current_hash_selection;
  HASHTYPE *hrchy_path;
  HASHTYPE hrchy_path_size;
  HASHTYPE hrchy_curr_depth;
  long int hrchy_hash_size;
  int hrchy_hash_selection;
  int records_read;
  int records_dropped_filter;
  int records_stored;
  int records_matched_key;
  int records_matched;
  int data_records_matched;
  STRINGTYPE *ext_file_open_command;
  STRINGTYPE *ext_record_read_command;
  STRINGTYPE *ext_file_close_command;
  STRINGTYPE *ext_start_writing_command;
  STRINGTYPE *ext_record_write_command;
  STRINGTYPE *ext_finished_writing_command;
  STRINGTYPE *ext_filter_command;
  STRINGTYPE *ext_match_command;
  STRINGTYPE *ext_hrchy_match_command;
  val_entry *hash_table;
  val_entry *hrchy_hash_table;
  };

typedef struct s_ref_file_info reference_file_struct;

extern int gi_reference_file_count;
extern int gi_data_is_reference;
extern unsigned char *gs_data_input_buffer;
extern size_t gi_data_input_buffer_size;
extern size_t gi_data_input_buffer_pos;
extern unsigned char *gs_data_output_buffer;
extern size_t gi_data_output_buffer_size;
extern size_t gi_data_output_buffer_pos;
extern STRINGTYPE *gs_output_field_delimiter;
extern int gi_no_output_field_delimiter;
extern STRINGTYPE *gs_output_record_delimiter;
extern STRINGTYPE *gs_data_constant_string;
extern STRINGTYPE *gs_data_missing_string;
extern STRINGTYPE *gs_data_empty_string;
extern STRINGTYPE *gs_data_input_field_delimiter;
extern int gi_data_no_input_field_delimiter;
extern int gi_data_input_field_delimiter_repeat;
extern STRINGTYPE *gs_data_input_record_delimiter;
extern field_id *gra_data_output_field_order;
extern int gi_data_output_field_order_count;
extern int gi_data_input_record_length;
extern int gi_output_counter_size;
extern int gi_counter_ind;
extern int gi_flag_ind;
extern int gi_verbose_ind;
extern int gi_statistics_ind;
extern int gi_checkallref_ind;
extern hm_t gi_hashmovement_ind;
extern int gi_data_write_ind;
extern int gi_data_suppress_empty_ind;
extern int gi_data_optional_ind;
extern int gi_data_exclude_match_ind;
extern char *gs_data_output_file_name;
extern field_range *gra_data_out_ranges;
extern int gi_data_out_end_start;
extern int gi_data_out_range_count;
extern field_range *gra_data_sum_ranges;
extern int gi_data_sum_end_start;
extern int gi_data_sum_range_count;
extern int gi_allocated_reference_info;
extern reference_file_struct *gra_reference_file_info;
extern STRINGTYPE *gs_ext_init_file;
extern STRINGTYPE *gs_ext_begin_command;
extern STRINGTYPE *gs_ext_end_command;
extern STRINGTYPE *gs_data_ext_file_open_command;
extern STRINGTYPE *gs_data_ext_record_read_command;
extern STRINGTYPE *gs_data_ext_file_close_command;
extern STRINGTYPE *gs_data_ext_start_writing_command;
extern STRINGTYPE *gs_data_ext_record_write_command;
extern STRINGTYPE *gs_data_ext_finished_writing_command;
extern STRINGTYPE *gs_data_ext_filter_command;
extern STRINGTYPE *gs_data_ext_match_command;

/* Counters for statistics on the data files */
extern int gi_data_records_read;
extern int gi_data_records_dropped_filter;
extern int gi_data_records_matched;
extern int gi_output_records_written;
extern int gi_file_records_read;
extern int gi_file_records_dropped_filter;
extern int gi_file_records_matched;
extern int gi_file_output_records_written;

/* An error string and number for internal use */
extern char df_errstr[300];
extern int df_errno;

#endif /* No DF_GLOBAL_H_GUARD */
