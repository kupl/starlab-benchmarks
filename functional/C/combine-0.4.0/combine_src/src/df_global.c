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
#include "dstring.h"
#include "df_common.h"
#include "df_global.h"

/*****************************
 * General global variables
 ****************************/

enum field_type field_type;

/* The name this program was run with. */
char *program_name;

/* The delimeter character for input fields. */
int input_delimiter;

/* The pointer to the input buffer for the data file */
unsigned char *gs_data_input_buffer;

/* The size of the input buffer for the data file */
size_t gi_data_input_buffer_size;

/* The current position in the input buffer for the data file */
size_t gi_data_input_buffer_pos;

/* The pointer to the output buffer for the data file */
unsigned char *gs_data_output_buffer;

/* The size of the output buffer for the data file */
size_t gi_data_output_buffer_size;

/* The current position in the output buffer for the data file */
size_t gi_data_output_buffer_pos;

/* A list of the file name arguments for reading */
char **filenames;
/* The number of file name arguments */
int filecount;

/* SHOULD check if these are still used */
char *input_filename;
char *output_filename;

/* Global pointers to the current input and output file streams */
FILE *input_file;
FILE *output_file;

/* Set to 1 if the reference file should be used as its own matching data file */
int gi_data_is_reference;

/* The number of reference files provided by the user */
int gi_reference_file_count;

/* The string to be placed between fields in the output file(s) */
STRINGTYPE *gs_output_field_delimiter;

/* There is no string to be placed between fields in the output file(s) */
int gi_no_output_field_delimiter;

/* The string to be placed between records in the output file(s) */
STRINGTYPE *gs_output_record_delimiter;

/* The string to write along with information from the data file */
STRINGTYPE *gs_data_constant_string;

/* The string to replace data fields when the data record is optional and missing */
STRINGTYPE *gs_data_missing_string;

/* The string to replace data fields of zero length */
STRINGTYPE *gs_data_empty_string;

/* There is no string that separates two fields from each other in the data file */
int gi_data_no_input_field_delimiter;

/* The string that separates two fields from each other in the data file can 
   repeat without creating multiple empty fields*/
int gi_data_input_field_delimiter_repeat;

/* The string that separates two fields from each other in the data file */
STRINGTYPE *gs_data_input_field_delimiter;

/* The string that separates two records from each other in the data file */
STRINGTYPE *gs_data_input_record_delimiter;

/* The fixed length of a data file record. */
int gi_data_input_record_length;

/* The list of fields in the order they should go into the output file */
field_id *gra_data_output_field_order;

/* The number of fields that will go into the output file */
int gi_data_output_field_order_count;

/* The size of each counter in the output files */
int gi_output_counter_size;

/* Whether to write a counter of matching data records in reference-based output 
 * 1 means yes; 0 means no
 */
int gi_counter_ind;
/* Whether to write a 1/0 floag for  matching data records in reference-based output 
 * 1 means yes; 0 means no
 */
int gi_flag_ind;
/* Whether to really write a lot of information
 */
int gi_verbose_ind;
/* Whether to write record counts
 */
int gi_statistics_ind;
/* Whether to check all possible matches
 */
int gi_checkallref_ind;
/* How to interpret the hash key
 */
hm_t gi_hashmovement_ind;
/* Whether to write data based on the (matching) data records along with any related reference data
 * 1 means yes; 0 means no
 */
int gi_data_write_ind;
/* Whether to suppress empty fields and their delimiters.
 * 1 means yes; 0 means no
 */
int gi_data_suppress_empty_ind;
/* Whether to write records for unmatched reference records into the data-based output.
 * -1 means yes; 0 means no
 */
int gi_data_optional_ind;
/* Whether to exclude records if there is a match to the data record.
 * -1 means yes; 0 means no
 */
int gi_data_exclude_match_ind;
/* The name of the file to write to for data-file-based output */
char *gs_data_output_file_name;
/* An array of positions of fields in the data file to be written to the output */
field_range *gra_data_out_ranges;
/* The starting position of a field that extends to the end of the record */
int gi_data_out_end_start;
/* The number of fields to be written from the data file (excluding one extending unbounded to the end of the record) */
int gi_data_out_range_count;
/* An array of positions of fields that should be summed for output with matching reference records */
field_range *gra_data_sum_ranges;
/* The beginning of an unbounded end field that should be summed for output with matching reference records */
int gi_data_sum_end_start;
/* The number of fields in the data file that should be summed for matching reference records */
int gi_data_sum_range_count;
/* The number of reference files we have allocated data for */
int gi_allocated_reference_info;
/* An array of reference information, including all reference-file options */
reference_file_struct *gra_reference_file_info;

/* Strings to hold extension commands */
STRINGTYPE *gs_ext_init_file;
STRINGTYPE *gs_ext_begin_command;
STRINGTYPE *gs_ext_end_command;
STRINGTYPE *gs_data_ext_file_open_command;
STRINGTYPE *gs_data_ext_record_read_command;
STRINGTYPE *gs_data_ext_file_close_command;
STRINGTYPE *gs_data_ext_start_writing_command;
STRINGTYPE *gs_data_ext_record_write_command;
STRINGTYPE *gs_data_ext_finished_writing_command;
STRINGTYPE *gs_data_ext_filter_command;
STRINGTYPE *gs_data_ext_match_command;


/* Counters for statistics on the data files */
int gi_data_records_read;
int gi_data_records_dropped_filter;
int gi_data_records_matched;
int gi_output_records_written;
int gi_file_records_read;
int gi_file_records_dropped_filter;
int gi_file_records_matched;
int gi_file_output_records_written;

/* An error string and number for internal use */
char df_errstr[300];
int df_errno;
