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
#include <getopt.h>
#include <stdio.h>
#include <limits.h>
#include "config.h"
#include "version-etc.h"
#include "df_common.h"
#include "df_global.h"
#include "field.h"
#include "dstring.h"

/* The official name of this program (e.g., no `g' prefix).  */
#define PROGRAM_NAME "combine"

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
enum
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3),
  GETOPT_JOIN_HELP_CHAR = (CHAR_MIN - 4),
  GETOPT_CUT_HELP_CHAR = (CHAR_MIN - 5),
  };

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, 0, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, 0, GETOPT_VERSION_CHAR
#define GETOPT_JOIN_HELP_OPTION_DECL \
  "help", no_argument, 0, GETOPT_JOIN_HELP_CHAR
#define GETOPT_CUT_HELP_OPTION_DECL \
  "help", no_argument, 0, GETOPT_CUT_HELP_CHAR

#define case_GETOPT_HELP_CHAR                   \
  case GETOPT_HELP_CHAR:                        \
    usage (EXIT_SUCCESS);                       \
    break;

#define case_GETOPT_JOIN_HELP_CHAR                   \
  case GETOPT_JOIN_HELP_CHAR:                        \
    usage_join (EXIT_SUCCESS);                       \
    break;

#define case_GETOPT_CUT_HELP_CHAR                   \
  case GETOPT_CUT_HELP_CHAR:                        \
    usage_cut (EXIT_SUCCESS);                       \
    break;

#define HELP_OPTION_DESCRIPTION \
  _("  --help     display this help and exit\n")
#define VERSION_OPTION_DESCRIPTION \
  _("  --version  output version information and exit\n")

#define case_GETOPT_VERSION_CHAR(Program_name, Authors)                 \
  case GETOPT_VERSION_CHAR:                                             \
    version_etc (stdout, Program_name, PACKAGE, VERSION, Authors);      \
    exit (EXIT_SUCCESS);                                                \
    break;

#define OPT_NO_CHECK_ALL_REFERENCE 300
#define OPT_STATISTICS 301
#define OPT_NO_STATISTICS 302
#define CUT_OPT_OUTPUT_DELIMITER 303
#define OPT_INPUT_DELIMITER_REPEAT 304
#define OPT_NO_INPUT_DELIMITER 305
#define OPT_NO_OUTPUT_DELIMITER 306

#define COMMON_SHORT_OPTIONS "+a:b:B:d:D:e:fF:h:H:k:K:lL:m:Mno:O:pPr:Rs:t:uUvwx:X:z:"

static struct option const long_options[] = {
  {"hierarchy-key-fields", required_argument, NULL, 'a'},
  {"output-record-delimiter", required_argument, NULL, 'b'},
  {"input-record-delimiter", required_argument, NULL, 'B'},
  {"output-field-delimiter", required_argument, NULL, 'd'},
  {"no-output-field-delimiter", required_argument, NULL,
   OPT_NO_OUTPUT_DELIMITER},
  {"input-field-delimiter", required_argument, NULL, 'D'},
  {"no-input-field-delimiter", required_argument, NULL, OPT_NO_INPUT_DELIMITER},
  {"input-field-delimiter-repeat", no_argument, NULL,
   OPT_INPUT_DELIMITER_REPEAT},
  {"emulate", required_argument, NULL, 'e'},
  {"flag", no_argument, NULL, 'f'},
  {"flatten-hierarchy", required_argument, NULL, 'F'},
  {"hash-size", required_argument, NULL, 'h'},
  {"hash-movement", required_argument, NULL, 'H'},
  {"key-fields", required_argument, NULL, 'k'},
  {"output-constant", required_argument, NULL, 'K'},
  {"hierarchy-leaf-only", no_argument, NULL, 'l'},
  {"input-record-length", required_argument, NULL, 'L'},
  {"data-key-fields", required_argument, NULL, 'm'},
  {"data-is-reference", no_argument, NULL, 'M'},
  {"count", no_argument, NULL, 'n'},
  {"match-optional", no_argument, NULL, 'p'},
  {"exclude-match", no_argument, NULL, 'P'},
  {"output-fields", required_argument, NULL, 'o'},
  {"field-order", required_argument, NULL, 'O'},
  {"reference-file", required_argument, NULL, 'r'},
  {"check-all-reference", no_argument, NULL, 'R'},
  {"no-check-all-reference", no_argument, NULL,
   OPT_NO_CHECK_ALL_REFERENCE},
  {"sum-fields", required_argument, NULL, 's'},
  {"output-file", required_argument, NULL, 't'},
  {"unique", no_argument, NULL, 'u'},
  {"up-hierarchy", no_argument, NULL, 'U'},
  {"verbose", no_argument, NULL, 'v'},
  {"write-output", no_argument, NULL, 'w'},
  {"extension", required_argument, NULL, 'x'},
  {"extension-init-file", required_argument, NULL, 'X'},
  {"counter-size", required_argument, NULL, 'z'},
  {"statistics", no_argument, NULL, OPT_STATISTICS},
  {"no-statistics", no_argument, NULL, OPT_NO_STATISTICS},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {0, 0, 0, 0},
  };

#define JOIN_COMMON_SHORT_OPTIONS "1:2:a:e:ij:o:t:v:"

static struct option const join_long_options[] = {
  {"file-1-key", required_argument, NULL, '1'},
  {"file-2-key", required_argument, NULL, '2'},
  {"print-unmatched-from", required_argument, NULL, 'a'},
  {"empty-replacement", required_argument, NULL, 'e'},
  {"ignore-case", no_argument, NULL, 'i'},
  {"join-key", required_argument, NULL, 'j'},
  {"output-format", required_argument, NULL, 'o'},
  {"delimiter", required_argument, NULL, 't'},
  {"print-only-unmatched-from", required_argument, NULL,
   'v'},
  {GETOPT_JOIN_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {0, 0, 0, 0},
  };

#define CUT_COMMON_SHORT_OPTIONS "+b:c:d:f:ns"

static struct option const cut_long_options[] = {
  {"bytes", required_argument, NULL, 'b'},
  {"characters", required_argument, NULL, 'c'},
  {"delimiter", required_argument, NULL, 'd'},
  {"fields", required_argument, NULL, 'f'},
  {"only-delimited", no_argument, NULL, 's'},
  {"output-delimiter", required_argument, NULL,
   CUT_OPT_OUTPUT_DELIMITER},
  {GETOPT_CUT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {0, 0, 0, 0},
  };

void expand_delimited_ranges (field_range **, int *);

/********
 * Like the name says, set up the beginning values of variables.  Accepts
 *   the same arguments as main, but uses them only to get the program
 *   name.
 */
int
initialize_variables (argc, argv)
     int argc;
     char **argv;
{
  gi_reference_file_count = 0;
  gi_data_is_reference = 0;
  gi_output_counter_size = 0;
  gi_counter_ind = 0;
  gi_flag_ind = 0;
  gi_verbose_ind = 0;
  gi_statistics_ind = 1;
  gi_checkallref_ind = 1;
  gi_hashmovement_ind = hm_binary_long;	/* Assume that most keys will react well to having the low order 4 bits from every byte used in the hash key. */
  gi_no_output_field_delimiter = 0;
  gi_data_write_ind = 0;
  gi_data_suppress_empty_ind = 0;
  gi_data_optional_ind = 0;
  gi_data_exclude_match_ind = 0;
  gi_data_out_end_start = 0;
  gi_data_out_range_count = 0;
  gi_data_sum_end_start = 0;
  gi_data_sum_range_count = 0;
  gi_allocated_reference_info = 0;
  gi_data_records_read = 0;
  gi_data_records_dropped_filter = 0;
  gi_data_records_matched = 0;
  gi_output_records_written = 0;
  gi_file_records_read = 0;
  gi_file_records_dropped_filter = 0;
  gi_file_records_matched = 0;
  gi_file_output_records_written = 0;
  gi_data_input_buffer_size = 0;
  gi_data_input_buffer_pos = 0;
  gi_data_output_buffer_size = 0;
  gi_data_output_buffer_pos = 0;
  input_filename = NULL;
  output_filename = NULL;
  gs_data_output_buffer = NULL;
  gs_data_input_buffer = NULL;
  gs_output_field_delimiter = NULL;
  gs_output_record_delimiter = dstrdupstr ("\n");
  gs_data_input_field_delimiter = NULL;
  gi_data_input_field_delimiter_repeat = 0;
  gs_data_input_record_delimiter = dstrdupstr ("\n");
  gs_data_constant_string = NULL;
  gs_data_missing_string = NULL;
  gs_data_empty_string = NULL;
  gs_data_output_file_name = NULL;
  gra_data_output_field_order = NULL;
  gi_data_output_field_order_count = 0;
  gs_ext_init_file = NULL;
  gs_ext_begin_command = NULL;
  gs_ext_end_command = NULL;
  gs_data_ext_file_open_command = NULL;
  gs_data_ext_record_read_command = NULL;
  gs_data_ext_file_close_command = NULL;
  gs_data_ext_start_writing_command = NULL;
  gs_data_ext_record_write_command = NULL;
  gs_data_ext_finished_writing_command = NULL;
  gs_data_ext_filter_command = NULL;
  gs_data_ext_match_command = NULL;

  gi_allocated_reference_info = 16;
  gra_reference_file_info
    = malloc (gi_allocated_reference_info * (sizeof *gra_reference_file_info));

  program_name = argv[0];

  field_type = undefined_mode;

  input_delimiter = '\0';

  return EXIT_SUCCESS;
  }


/********
 * From a character string from the option list, parse out the information
 *   on the fields that are to be processed.  string is the option value.
 *   ranges and range count are an array and its size specifying the position
 *   of all the fixed fields.  end_range_start is the beginning point of an
 *   optional final field which is unlimited until the end of the record.
 */
int
calculate_ranges (string, ranges, end_range_start, range_count)
     char *string;
     field_range **ranges;
     int *end_range_start;
     int *range_count;
{
  /* Parse a list of field specifications.
     Each will be separated by commas.
     Each will have the form m, m-n, -n, or m- (m and n being integers).
     Each may be suffixed with a .n (for precision)
     Each may have a suffix in parentheses to state a format. */

  int field_found;		/* Non-zero if at least one field spec
				   has been processed.  */
  unsigned int start_value;
  unsigned int end_value;
  unsigned int precision;

  unsigned int n_ranges;
  unsigned int n_ranges_allocated;

  char *string_copy;
  char *current_range;
  char *rest_of_string;
  char *current_comment;

  if (strlen (string) == 0) {
    *end_range_start = 0;
    *range_count = 0;
    *ranges = NULL;
    return 0;
    }

  n_ranges = 0;
  n_ranges_allocated = 16;
  *ranges = (field_range *) malloc (n_ranges_allocated * sizeof (**ranges));

  string_copy = strdup (string);
  if (string_copy == NULL)
    return EXIT_FAILURE;

  rest_of_string = string_copy;
  while (rest_of_string != NULL) {
    current_range = strsep (&rest_of_string, ",");

    start_value = 0;
    end_value = 0;
    precision = 0;
    current_comment = NULL;

    if (*current_range != '-') {
      start_value = strtol (current_range, &current_range, 0);
      }

    if (*current_range == '-') {
      current_range++;
      end_value = strtol (current_range, &current_range, 0);
      }
    else
      end_value = start_value;

    if (*current_range == '.') {
      current_range++;
      precision = strtol (current_range, &current_range, 0);
      }

    current_comment = current_range;
    if (*current_comment == '\0')
      current_comment = NULL;

    if (start_value == 0 && end_value == 0)
      FATAL_ERROR1 (_("invalid field specification: %s"), string);
    if (start_value == 0)
      start_value = 1;

    if (end_value == 0) {
      /* A range that extends to the end. */
      *end_range_start = start_value;
      }
    else {
      /* Validate the range */
      if (start_value > end_value)
	FATAL_ERROR2 (_("invalid field range: start %d, end %d"), start_value,
		      end_value);

      /* A normal range. */
      ADD_RANGE_PAIR ((*ranges), start_value, end_value, precision,
		      current_comment);
      }
    }
  /* Think about whether it matters if we have overlapping ranges.  I once thought it did,
     but I removed that code. */
  free (string_copy);
  *range_count = n_ranges;
  return field_found;
  }

/********
 * Write out information on the requested command from the global variables.
 * this will be useful diagnostic information, especially in the future if
 * there is the possibility of options taken from configuration files.
 */
int
write_user_request ()
{
  int i;
  int j;

  fprintf (stderr, _("General options:\n"));
  if (gs_output_field_delimiter != NULL)
    fprintf (stderr, _("  String separating fields in ouput: [%.*s]\n"),
	     (int) (gs_output_field_delimiter->length),
	     gs_output_field_delimiter->string);
  if (gi_output_counter_size != 0)
    fprintf (stderr,
	     _
	     ("  Length of any counter or sum values in reference-based output: %d\n"),
	     gi_output_counter_size);
  if (gi_counter_ind != 0)
    fprintf (stderr,
	     _
	     ("  Writing a count of data file matches in reference-based output.\n"));
  if (gi_flag_ind != 0)
    fprintf (stderr,
	     _
	     ("  Writing a flag for data file matches in reference-based output.\n"));

  fprintf (stderr, _("Data files to process: "));

  if (filecount == 0)
    fprintf (stderr, "<stdin>");
  else
    for (i = 0; i < filecount; i++) {
      if (i > 0)
	fprintf (stderr, ", ");
      fprintf (stderr, "%s", filenames[i]);
      }
  fprintf (stderr, "\n");

  if (gi_data_write_ind != 0) {
    if (gs_data_output_file_name != NULL)
      fprintf (stderr, _("  Writing data-file-based output to file %s\n"),
	       gs_data_output_file_name);
    else
      fprintf (stderr, _("  Writing data-file-based output to stdout\n"));

    if (gi_data_out_range_count != 0) {
      fprintf (stderr,
	       _
	       ("  Data fields written to the data-based output file:\n  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
      for (i = 0; i < gi_data_out_range_count; i++) {
	fprintf (stderr, "  %5ld %5ld %9d %s\n",
		 gra_data_out_ranges[i].lower_bound,
		 gra_data_out_ranges[i].upper_bound,
		 gra_data_out_ranges[i].precision,
		 gra_data_out_ranges[i].format);
	}
      /* Here we assume that a record-ending specification won't happen alone. */
      if (gi_data_out_end_start != 0)
	fprintf (stderr, _("  %5d   End %9d %s\n"), gi_data_out_end_start, 0,
		 (char *) NULL);
      }
    }

  if (gi_data_sum_range_count != 0) {
    fprintf (stderr,
	     _
	     ("  Data fields to be summed in reference-based output:\n  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
    for (i = 0; i < gi_data_sum_range_count; i++) {
      fprintf (stderr, "  %5ld %5ld %9d %s\n",
	       gra_data_sum_ranges[i].lower_bound,
	       gra_data_sum_ranges[i].upper_bound,
	       gra_data_sum_ranges[i].precision, gra_data_sum_ranges[i].format);
      }
    /* Here we assume that a record-ending specification won't happen alone. */
    if (gi_data_sum_end_start != 0)
      fprintf (stderr, _("  %5d   End %9d %s\n"), gi_data_sum_end_start, 0,
	       (char *) NULL);
    }

  if (gs_data_constant_string != NULL)
    fprintf (stderr,
	     _("  Constant value accompanying data fields in ouput: [%.*s]\n"),
	     (int) (gs_data_constant_string->length),
	     gs_data_constant_string->string);

  for (i = 0; i < gi_reference_file_count; i++) {
    /* Print out the reference file information from the structure */
    fprintf (stderr, _("Reference file number %d: %s\n"), i + 1,
	     gra_reference_file_info[i].file_name);
    if (gra_reference_file_info[i].out_range_count != 0) {
      fprintf (stderr,
	       _
	       ("  Reference fields to be written in all output:\n  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
      for (j = 0; j < gra_reference_file_info[i].out_range_count; j++) {
	fprintf (stderr, "  %5ld %5ld %9d %s\n",
		 gra_reference_file_info[i].out_ranges[j].lower_bound,
		 gra_reference_file_info[i].out_ranges[j].upper_bound,
		 gra_reference_file_info[i].out_ranges[j].precision,
		 gra_reference_file_info[i].out_ranges[j].format);
	}
      /* Here we assume that a record-ending specification won't happen alone. */
      if (gra_reference_file_info[i].out_end_start != 0)
	fprintf (stderr, _("  %5d   End %9d %s\n"),
		 gra_reference_file_info[i].out_end_start, 0, (char *) NULL);
      }
    if (gra_reference_file_info[i].key_range_count != 0) {
      fprintf (stderr,
	       _
	       ("  Reference fields to match data file keys:\n  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
      for (j = 0; j < gra_reference_file_info[i].key_range_count; j++) {
	fprintf (stderr, "  %5ld %5ld %9d %s\n",
		 gra_reference_file_info[i].key_ranges[j].lower_bound,
		 gra_reference_file_info[i].key_ranges[j].upper_bound,
		 gra_reference_file_info[i].key_ranges[j].precision,
		 gra_reference_file_info[i].key_ranges[j].format);
	}
      /* Here we assume that a record-ending specification won't happen alone. */
      if (gra_reference_file_info[i].key_end_start != 0)
	fprintf (stderr, _("  %5d   End %9d %s\n"),
		 gra_reference_file_info[i].key_end_start, 0, (char *) NULL);
      }
    if (gra_reference_file_info[i].data_key_range_count != 0) {
      fprintf (stderr,
	       _
	       ("  Data fields to match keys in this reference file:\n  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
      for (j = 0; j < gra_reference_file_info[i].data_key_range_count; j++) {
	fprintf (stderr, "  %5ld %5ld %9d %s\n",
		 gra_reference_file_info[i].data_key_ranges[j].lower_bound,
		 gra_reference_file_info[i].data_key_ranges[j].upper_bound,
		 gra_reference_file_info[i].data_key_ranges[j].precision,
		 gra_reference_file_info[i].data_key_ranges[j].format);
	}
      /* Here we assume that a record-ending specification won't happen alone. */
      if (gra_reference_file_info[i].data_key_end_start != 0)
	fprintf (stderr, _("  %5d   End %9d %s\n"),
		 gra_reference_file_info[i].data_key_end_start, 0,
		 (char *) NULL);
      }
    if (gra_reference_file_info[i].hrchy_key_range_count != 0) {
      fprintf (stderr,
	       _
	       ("  Traversing hierarchy using these keys to connect to next record in this file:\n"));
      fprintf (stderr,
	       _
	       ("  Start   End Precision Instructions\n  ----- ----- --------- ------------\n"));
      for (j = 0; j < gra_reference_file_info[i].hrchy_key_range_count; j++) {
	fprintf (stderr, "  %5ld %5ld %9d %s\n",
		 gra_reference_file_info[i].hrchy_key_ranges[j].lower_bound,
		 gra_reference_file_info[i].hrchy_key_ranges[j].upper_bound,
		 gra_reference_file_info[i].hrchy_key_ranges[j].precision,
		 gra_reference_file_info[i].hrchy_key_ranges[j].format);
	}
      /* Here we assume that a record-ending specification won't happen alone. */
      if (gra_reference_file_info[i].hrchy_key_end_start != 0)
	fprintf (stderr, _("  %5d   End %9d %s\n"),
		 gra_reference_file_info[i].hrchy_key_end_start, 0,
		 (char *) NULL);
      if (gra_reference_file_info[i].hrchy_flatten_levels != 0) {
	fprintf (stderr,
		 _
		 ("  Reference fields from this file will be repeated %d times to report each level"),
		 gra_reference_file_info[i].hrchy_flatten_levels);
	}
      else if (gra_reference_file_info[i].hrchy_leaf_only)
	fprintf (stderr,
		 _
		 ("  Terminal nodes encountered in the hierarchy will be treated as matching reference records.\n"));
      else
	fprintf (stderr,
		 _
		 ("  Each node encountered in the hierarchy will be treated as a matching reference record.\n"));
      }
    if (gra_reference_file_info[i].unique_ind)
      fprintf (stderr, _("  Keeping only one copy of each key value.\n"));
    else
      fprintf (stderr, _("  Keeping all copies of each key value.\n"));
    if (gra_reference_file_info[i].optional_ind)
      fprintf (stderr, _("  Matches to this file optional for data output.\n"));
    else
      fprintf (stderr, _("  Matches to this file required for data output.\n"));
    if (gra_reference_file_info[i].write_ind) {
      if (gra_reference_file_info[i].output_file_name != NULL)
	fprintf (stderr, _("  Writing output to %s\n"),
		 gra_reference_file_info[i].output_file_name);
      else
	fprintf (stderr, _("  Writing output to stdout\n"));
      }
    if (gra_reference_file_info[i].constant_string != NULL)
      fprintf (stderr,
	       _
	       ("  Constant value to accompany all reference output: [%.*s]\n"),
	       (int) (gra_reference_file_info[i].constant_string->length),
	       gra_reference_file_info[i].constant_string->string);
    }

  return EXIT_SUCCESS;
  }

/********
 * Validate the options to combine.
 */
int
validate_options ()
{
  int i;
  int j;


  /* If we specified a data input field delimiter, pass it to any reference
   * files that neither have a delimiter specified nor have the "no delimiter"
   * option set.
   */
  if (gs_data_input_field_delimiter != NULL) {
    for (i = 0; i < gi_reference_file_count; i++) {
      if (gra_reference_file_info[i].input_field_delimiter == NULL
	  && gra_reference_file_info[i].no_input_field_delimiter == 0)
	gra_reference_file_info[i].input_field_delimiter
	  = gs_data_input_field_delimiter;
      }
    if (gs_output_field_delimiter == NULL && gi_no_output_field_delimiter == 0)
      gs_output_field_delimiter = gs_data_input_field_delimiter;
    }

  /* Work through all the field ranges in any delimited-field input
     files and expand the ranges to a single field per range. */
  if (gs_data_input_field_delimiter != NULL) {
    expand_delimited_ranges (&gra_data_out_ranges, &gi_data_out_range_count);
    expand_delimited_ranges (&gra_data_sum_ranges, &gi_data_sum_range_count);
    }

  for (j = 0; j < gi_reference_file_count; j++) {
    if (gs_data_input_field_delimiter != NULL)
      expand_delimited_ranges (&(gra_reference_file_info[j].data_key_ranges),
			       &(gra_reference_file_info[j]
				 .data_key_range_count));
    if (gra_reference_file_info[j].input_field_delimiter != NULL) {
      expand_delimited_ranges (&(gra_reference_file_info[j].key_ranges),
			       &(gra_reference_file_info[j].key_range_count));
      expand_delimited_ranges (&(gra_reference_file_info[j].hrchy_key_ranges),
			       &(gra_reference_file_info[j]
				 .hrchy_key_range_count));
      expand_delimited_ranges (&(gra_reference_file_info[j].out_ranges),
			       &(gra_reference_file_info[j].out_range_count));
      }
    }


  /* Check ordering requests.  First for data file output, then for
     reference file output. */

  if (gra_data_output_field_order != NULL) {
    for (i = 0; i < gi_data_output_field_order_count; i++) {
      switch (gra_data_output_field_order[i].source) {
      case 'r':
	if (gi_reference_file_count < 1
	    || gra_data_output_field_order[i].source_num
	    > gi_reference_file_count)
	  FATAL_ERROR1 (_
			("requested non-existent reference file %d in data-output ordering"),
			gra_data_output_field_order[i].source_num);
	switch (gra_data_output_field_order[i].type) {
	case 'f':
	case 'n':
	case 's':
	  FATAL_ERROR (_
		       ("requested flag, counter, or sum in data-output ordering"));
	  break;
	case 'o':
	  if (gra_data_output_field_order[i].type_num == 0
	      || gra_data_output_field_order[i].type_num
	      > TOTAL_REF_OUT_RANGES (gra_data_output_field_order[i].source_num
				      - 1)
	      || gra_data_output_field_order[i].type_num
	      < -1
	      * TOTAL_REF_OUT_RANGES (gra_data_output_field_order[i].source_num
				      - 1))
	    FATAL_ERROR2 (_
			  ("requested non-existent reference field %d in refrence file %d in data-output ordering"),
			  gra_data_output_field_order[i].type_num,
			  gra_data_output_field_order[i].source_num);
	  break;
	case 'k':
	  if (gra_reference_file_info[gra_data_output_field_order[i].source_num]
	      .constant_string == NULL)
	    FATAL_ERROR1 (_
			  ("requested non-existent reference constant string from reference file %d in data-output ordering"),
			  gra_data_output_field_order[i].source_num);
	  break;
	default:
	  FATAL_ERROR1 (_
			("unknown reference field type '%c' requested in data-output ordering"),
			gra_data_output_field_order[i].type);
	  }
	break;
      case 'd':
      case 's':
	switch (gra_data_output_field_order[i].type) {
	case 'f':
	case 'n':
	case 's':
	  FATAL_ERROR (_
		       ("requested flag, counter, or sum from data file in data-output ordering"));
	  break;
	case 'o':
	  if (gra_data_output_field_order[i].type_num == 0
	      || gra_data_output_field_order[i].type_num
	      > gi_data_out_range_count + (gi_data_out_end_start != 0 ? 1 : 0)
	      || gra_data_output_field_order[i].type_num
	      < -1 * gi_data_out_range_count - (gi_data_out_end_start != 0 ? 1
						: 0))
	    FATAL_ERROR1 (_
			  ("requested non-existent data field %d in data-output ordering"),
			  gra_data_output_field_order[i].type_num);
	  break;
	case 'k':
	  if (gs_data_constant_string == NULL)
	    FATAL_ERROR (_
			 ("requested non-existent data constant string in data-output ordering"));
	  break;
	default:
	  FATAL_ERROR1 (_
			("unknown data field type %c requested in data-output ordering"),
			gra_data_output_field_order[i].type);
	  }
	break;
      default:
	FATAL_ERROR1 (_
		      ("unknown field source %c requested in data-output ordering"),
		      gra_data_output_field_order[i].source);
	}
      }
    }


  if (gi_data_is_reference != 0) {
    if (gi_reference_file_count != 1)
      FATAL_ERROR (_
		   ("single-pass reading of one file as both reference and data requires exactly one reference file"));
    if (gra_reference_file_info[0].data_key_end_start
	!= gra_reference_file_info[0].key_end_start)
      FATAL_ERROR (_
		   ("single-pass reading of one file as both reference and data requires identical keys"));
    if (gra_reference_file_info[0].data_key_range_count
	!= gra_reference_file_info[0].key_range_count)
      FATAL_ERROR (_
		   ("single-pass reading of one file as both reference and data requires identical keys"));
    for (i = 0; i < gra_reference_file_info[0].key_range_count; i++) {
      if ((gra_reference_file_info[0].data_key_ranges[i].upper_bound
	   != gra_reference_file_info[0].key_ranges[i].upper_bound)
	  || (gra_reference_file_info[0].data_key_ranges[i].lower_bound
	      != gra_reference_file_info[0].key_ranges[i].lower_bound)
	  || (gra_reference_file_info[0].data_key_ranges[i].precision
	      != gra_reference_file_info[0].key_ranges[i].precision)
	  || (gra_reference_file_info[0].data_key_ranges[i].format != NULL
	      && gra_reference_file_info[0].key_ranges[i].format != NULL
	      && strcmp (gra_reference_file_info[0].data_key_ranges[i].format,
			 gra_reference_file_info[0].key_ranges[i].format) != 0))
	FATAL_ERROR (_
		     ("single-pass reading of one file as both reference and data requires identical keys"));
      }
    if (gra_reference_file_info[0].unique_ind == 0)
      FATAL_ERROR (_
		   ("single-pass reading of one file as both reference and data requires unique keys"));
    if (gi_data_write_ind != 0)
      FATAL_ERROR (_
		   ("single-pass reading of one file as both reference and data makes no sense with data=file-based output"));
    }

  for (j = 0; j < gi_reference_file_count; j++) {
    if (gra_reference_file_info[j].output_field_order != NULL) {
      for (i = 0; i < gra_reference_file_info[j].output_field_order_count; i++) {
	switch (gra_reference_file_info[j].output_field_order[i].source) {
	case 'r':
	  if ((gra_reference_file_info[j].output_field_order[i].source_num
	       > gi_reference_file_count)
	      || (gra_reference_file_info[j].output_field_order[i].source_num
		  < -1 * gi_reference_file_count)
	      || (gra_reference_file_info[j].output_field_order[i].source_num
		  == 0))
	    FATAL_ERROR2 (_
			  ("requested non-existent reference file %d in output ordering for reference file %d"),
			  gra_reference_file_info[j].output_field_order[i]
			  .source_num, j + 1);
	  if ((gra_reference_file_info[j].output_field_order[i].source_num
	       != j + 1)
	      && (gra_reference_file_info[j].output_field_order[i].source_num
		  != 0))
	    FATAL_ERROR2 (_
			  ("reference file %d order requests fields from reference file %d"),
			  j + 1,
			  gra_reference_file_info[j].output_field_order[i]
			  .source_num);
	  switch (gra_reference_file_info[j].output_field_order[i].type) {
	  case 'n':
	    if (gi_counter_ind == 0)
	      FATAL_ERROR1 (_
			    ("requested non-existent counter in output from reference file %d"),
			    j);
	    break;
	  case 's':
	    if (NUM_COUNTERS - 2
		< gra_reference_file_info[j].output_field_order[i].type_num)
	      FATAL_ERROR2 (_
			    ("requested sum %d when only %d sum fields were specified"),
			    gra_reference_file_info[j].output_field_order[i]
			    .type_num, NUM_COUNTERS - 2);
	    break;
	  case 'f':
	    if (gi_flag_ind == 0)
	      FATAL_ERROR1 (_
			    ("requested non-existent flag in output from reference file %d"),
			    j);
	    break;
	  case 'o':
	    if (gra_reference_file_info[j].output_field_order[i].type_num < 1
		|| gra_reference_file_info[j].output_field_order[i].type_num
		> gra_reference_file_info[j].out_range_count)
	      FATAL_ERROR2 (_
			    ("requested non-existent reference field %d in reference file %d in reference-output ordering"),
			    gra_reference_file_info[j].output_field_order[i]
			    .type_num,
			    gra_reference_file_info[j].output_field_order[i]
			    .source_num);
	    break;
	  case 'k':
	    if (gra_reference_file_info[j].constant_string == NULL)
	      FATAL_ERROR1 (_
			    ("requested non-existent reference constant string from reference file %d in reference-output ordering"),
			    gra_reference_file_info[j].output_field_order[i]
			    .source_num);
	    break;
	  default:
	    FATAL_ERROR1 (_
			  ("unknown reference field type '%c' requested in reference-output ordering"),
			  gra_reference_file_info[j].output_field_order[i]
			  .type);
	    }
	  break;
	case 'd':
	case 's':
	  switch (gra_reference_file_info[j].output_field_order[i].type) {
	  case 'f':
	  case 'n':
	  case 's':
	    FATAL_ERROR (_
			 ("requested flag, counter, or sum from data file in reference-output ordering"));
	    break;
	  case 'o':
	    if (gra_reference_file_info[j].output_field_order[i].type_num < 1
		|| gra_reference_file_info[j].output_field_order[i].type_num
		> gi_data_out_range_count)
	      FATAL_ERROR1 (_
			    ("requested non-existent data field %d in reference-output ordering"),
			    gra_reference_file_info[j].output_field_order[i]
			    .type_num);
	    break;
	  case 'k':
	    if (gs_data_constant_string == NULL)
	      FATAL_ERROR (_
			   ("requested non-existent data constant string in reference-output ordering"));
	    break;
	  default:
	    FATAL_ERROR1 (_
			  ("unknown data field type %c requested in reference-output ordering"),
			  gra_reference_file_info[j].output_field_order[i]
			  .type);
	    }
	  break;
	default:
	  FATAL_ERROR1 (_
			("unknown field source %c requested in reference-output ordering"),
			gra_reference_file_info[j].output_field_order[i]
			.source);
	  }
	}
      }
    }

#ifndef HAVE_LIBGUILE
  /* Check for any of the options that would call guile.  If they 
   * are used, complain. */
  if (gs_ext_init_file != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_ext_init_file->length, gs_ext_init_file->string);
  if (gs_ext_begin_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_ext_begin_command->length, gs_ext_begin_command->string);
  if (gs_ext_end_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_ext_end_command->length, gs_ext_end_command->string);
  if (gs_data_ext_file_open_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_file_open_command->length,
		  gs_data_ext_file_open_command->string);
  if (gs_data_ext_record_read_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_record_read_command->length,
		  gs_data_ext_record_read_command->string);
  if (gs_data_ext_file_close_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_file_close_command->length,
		  gs_data_ext_file_close_command->string);
  if (gs_data_ext_start_writing_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_start_writing_command->length,
		  gs_data_ext_start_writing_command->string);
  if (gs_data_ext_record_write_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_record_write_command->length,
		  gs_data_ext_record_write_command->string);
  if (gs_data_ext_finished_writing_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_finished_writing_command->length,
		  gs_data_ext_finished_writing_command->string);
  if (gs_data_ext_filter_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_filter_command->length,
		  gs_data_ext_filter_command->string);
  if (gs_data_ext_match_command != NULL)
    FATAL_ERROR2 (_
		  ("specified extension processing without Guile installed: %.*s"),
		  gs_data_ext_match_command->length,
		  gs_data_ext_match_command->string);

  /* FIXME Still need to check fields and reference file things. */
#  endif /* No HAVE_LIBGUILE */

  return EXIT_SUCCESS;
  }

/* Run through the ranges.  If there are any whose upper bound is
   higher than the lower bound, create a set of ranges with upper and
   lower bound equal, with an entry for every number in between. */
void
expand_delimited_ranges (out_ranges, range_count)
     field_range **out_ranges;
     int *range_count;
{
  int i;
  field_range *replace_ranges;
  int replace_count;

  replace_ranges = NULL;
  replace_count = 0;

  for (i = 0; i < *range_count; i++) {
    if ((*out_ranges)[i].upper_bound != 0
	&& (*out_ranges)[i].upper_bound != (*out_ranges)[i].lower_bound) {
      int j;
      /* Expand the ranges in the replacement */
      if (replace_ranges == NULL) {
	replace_ranges
	  =
	  malloc ((i + (*out_ranges)[i].upper_bound
		   - (*out_ranges)[i].lower_bound + 1) * sizeof (field_range));
	if (replace_ranges == NULL) {
	  *out_ranges = NULL;
	  *range_count = 0;
	  return;
	  }
	if (i > 0)
	  memmove (replace_ranges, *out_ranges, i * sizeof (field_range));
	replace_count = i;
	}
      else {
	replace_ranges
	  = realloc (replace_ranges,
		     (replace_count + (*out_ranges)[i].upper_bound
		      - (*out_ranges)[i].lower_bound
		      + 1) * sizeof (field_range));
	if (replace_ranges == NULL) {
	  *out_ranges = NULL;
	  *range_count = 0;
	  return;
	  }
	}
      for (j = (*out_ranges)[i].lower_bound; j <= (*out_ranges)[i].upper_bound;
	   j++) {
	replace_count++;
	replace_ranges[replace_count - 1].lower_bound = j;
	replace_ranges[replace_count - 1].upper_bound = j;
	replace_ranges[replace_count - 1].precision
	  = (*out_ranges)[i].precision;
	replace_ranges[replace_count - 1].format = (*out_ranges)[i].format;
	}

      }
    else if (replace_ranges != NULL) {
      /* Copy the range to the replacement */
      replace_ranges
	= realloc (replace_ranges, (replace_count + 1) * sizeof (field_range));
      if (replace_ranges == NULL) {
	*out_ranges = NULL;
	*range_count = 0;
	return;
	}
      replace_count++;
      replace_ranges[replace_count - 1].upper_bound
	= (*out_ranges)[i].upper_bound;
      replace_ranges[replace_count - 1].lower_bound
	= (*out_ranges)[i].lower_bound;
      replace_ranges[replace_count - 1].precision = (*out_ranges)[i].precision;
      replace_ranges[replace_count - 1].format = (*out_ranges)[i].format;
      }
    }

  if (replace_ranges != NULL) {
    *out_ranges = replace_ranges;
    *range_count = replace_count;
    }

  return;
  }

int
assert_reference_file (option)
     int option;
{
  if (gi_reference_file_count <= 0)
    FATAL_ERROR1 (_("option %c listed before any reference file was specified"),
		  option);
  return EXIT_SUCCESS;
  }

int
set_reference_option (option)
     int option;
{
  switch (option) {
  case 'a':
    calculate_ranges (optarg,
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.hrchy_key_ranges),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.hrchy_key_end_start),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.hrchy_key_range_count));
    break;
  case 'F':
    gra_reference_file_info[gi_reference_file_count - 1].hrchy_flatten_levels
      = strtol (optarg, NULL, 0);
    break;
  case 'h':
    if ((gra_reference_file_info[gi_reference_file_count - 1].hash_size
	 = atoi (optarg)) <= 0)
      FATAL_ERROR1 (_("hash table size must be a positive number (got %s)"),
		    optarg);
    break;
  case 'H':
    if (strcasecmp (optarg, "beginning") == 0) {
      gi_hashmovement_ind = hm_beginning;
      }
    else if (strcasecmp (optarg, "end") == 0) {
      gi_hashmovement_ind = hm_end;
      }
    else if (strcasecmp (optarg, "number") == 0) {
      gi_hashmovement_ind = hm_number;
      }
    else if (strcasecmp (optarg, "binary") == 0) {
      gi_hashmovement_ind = hm_binary;
      }
    else if (strcasecmp (optarg, "binary_long") == 0) {
      gi_hashmovement_ind = hm_binary_long;
      }
    else
      FATAL_ERROR (_("hash-movement option must be `beginning' or `end'"));
    break;
  case 'k':
    calculate_ranges (optarg,
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.key_ranges),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.key_end_start),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.key_range_count));
    break;
  case 'l':
    gra_reference_file_info[gi_reference_file_count - 1].hrchy_leaf_only = -1;
    break;
  case 'm':
    calculate_ranges (optarg,
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.data_key_ranges),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.data_key_end_start),
		      &(gra_reference_file_info[gi_reference_file_count - 1]
			.data_key_range_count));
    break;
  case 'p':
    gra_reference_file_info[gi_reference_file_count - 1].optional_ind = -1;
    break;
  case 'P':
    gra_reference_file_info[gi_reference_file_count - 1].optional_ind = -1;
    gra_reference_file_info[gi_reference_file_count - 1].exclude_match_ind = -1;
    break;
  case 'u':
    gra_reference_file_info[gi_reference_file_count - 1].unique_ind = -1;
    break;
  case 'U':
    gra_reference_file_info[gi_reference_file_count - 1].hrchy_direction = -1;
    break;
    }
  }

int
set_file_option (option)
     int option;
{
  STRINGTYPE temp_dstring;

  switch (option) {
  case 'B':
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gs_data_input_record_delimiter = dstrdupstr (optarg);
      if (gs_data_input_record_delimiter == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    else {
      gra_reference_file_info[gi_reference_file_count
			      - 1].input_record_delimiter = dstrdupstr (optarg);
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .input_record_delimiter == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    break;
  case 'D':
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gs_data_input_field_delimiter = dstrdupstr (optarg);
      if (gs_data_input_field_delimiter == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      gi_data_no_input_field_delimiter = 0;
      }
    else {
      gra_reference_file_info[gi_reference_file_count - 1].input_field_delimiter
	= dstrdupstr (optarg);
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .input_field_delimiter == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      gra_reference_file_info[gi_reference_file_count
			      - 1].no_input_field_delimiter = 0;
      }
    break;
  case OPT_NO_INPUT_DELIMITER:
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gi_data_no_input_field_delimiter = -1;
      if (gs_data_input_field_delimiter != NULL) {
	dstrfree (gs_data_input_field_delimiter);
	gs_data_input_field_delimiter = NULL;
	}
      }
    else {
      gra_reference_file_info[gi_reference_file_count
			      - 1].no_input_field_delimiter = -1;
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .input_field_delimiter != NULL) {
	dstrfree (gra_reference_file_info[gi_reference_file_count - 1]
		  .input_field_delimiter);
	gra_reference_file_info[gi_reference_file_count
				- 1].input_field_delimiter = NULL;
	}
      }
    break;
  case OPT_INPUT_DELIMITER_REPEAT:
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gi_data_input_field_delimiter_repeat = -1;
      }
    else {
      gra_reference_file_info[gi_reference_file_count
			      - 1].input_field_delimiter_repeat = -1;
      }
    break;
  case 'K':
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gs_data_constant_string = dstrdupstr (optarg);
      if (gs_data_constant_string == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    else {
      gra_reference_file_info[gi_reference_file_count - 1].constant_string
	= dstrdupstr (optarg);
      if (gra_reference_file_info[gi_reference_file_count - 1].constant_string
	  == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    break;
  case 'L':
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gi_data_input_record_length = atol (optarg);
      if (gi_data_input_record_length <= 0)
	FATAL_ERROR1 (_("record length must be postitive number (got %s)"),
		      optarg);
      }
    else {
      gra_reference_file_info[gi_reference_file_count - 1].input_record_length
	= atol (optarg);
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .input_record_length == 0)
	FATAL_ERROR1 (_("record length must be postitive number (got %s)"),
		      optarg);
      }
    break;
  case 'o':
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      calculate_ranges (optarg, &gra_data_out_ranges, &gi_data_out_end_start,
			&gi_data_out_range_count);
      }
    else {
      calculate_ranges (optarg,
			&(gra_reference_file_info[gi_reference_file_count - 1]
			  .out_ranges),
			&(gra_reference_file_info[gi_reference_file_count - 1]
			  .out_end_start),
			&(gra_reference_file_info[gi_reference_file_count - 1]
			  .out_range_count));
      }
    break;
  case 'O':
    temp_dstring.string = optarg;
    temp_dstring.length = strlen (optarg);
    if (gi_reference_file_count == 0) {
      /* Data file info. */
      gra_data_output_field_order
	= parse_field_order_string (&temp_dstring,
				    &gi_data_output_field_order_count);
      if (gra_data_output_field_order == NULL)
	FATAL_ERROR (_("unable to parse data output field order"));
      }
    else {
      gra_reference_file_info[gi_reference_file_count - 1].output_field_order
	= parse_field_order_string (&temp_dstring,
				    &(gra_reference_file_info
				      [gi_reference_file_count
				       - 1].output_field_order_count));
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .output_field_order == NULL)
	FATAL_ERROR ("Unable to parse reference output field order");
      }
    break;
  case 'p':
    if (gi_reference_file_count == 0)
      gi_data_optional_ind = -1;
    else
      gra_reference_file_info[gi_reference_file_count - 1].optional_ind = -1;
    break;
  case 'P':
    if (gi_reference_file_count == 0)
      gi_data_optional_ind = -1;
    else
      gra_reference_file_info[gi_reference_file_count - 1].optional_ind = -1;
    if (gi_reference_file_count == 0)
      gi_data_exclude_match_ind = -1;
    else
      gra_reference_file_info[gi_reference_file_count - 1].exclude_match_ind
	= -1;
    break;
  case 't':
    if (gi_reference_file_count == 0) {
      gs_data_output_file_name = strdup (optarg);
      if (gs_data_output_file_name == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    else {
      gra_reference_file_info[gi_reference_file_count - 1].output_file_name
	= strdup (optarg);
      if (gra_reference_file_info[gi_reference_file_count - 1].output_file_name
	  == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      }
    break;
  case 'w':
    if (gi_reference_file_count == 0)
      gi_data_write_ind = 1;
    else
      gra_reference_file_info[gi_reference_file_count - 1].write_ind = 1;
    break;
    }
  }


int
set_reference_file (filename)
     char *filename;
{
  if (++gi_reference_file_count > gi_allocated_reference_info)
    FATAL_ERROR ("too many reference files.");	/*reallocate_reference_info(); */
  gra_reference_file_info[gi_reference_file_count - 1].file_name
    = strdup (filename);
  if (gra_reference_file_info[gi_reference_file_count - 1].file_name == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  gra_reference_file_info[gi_reference_file_count - 1].input_buffer = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].input_buffer_size = 0;
  gra_reference_file_info[gi_reference_file_count - 1].input_buffer_pos = 0;
  gra_reference_file_info[gi_reference_file_count - 1].output_buffer = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].output_buffer_size = 0;
  gra_reference_file_info[gi_reference_file_count - 1].output_buffer_pos = 0;
  gra_reference_file_info[gi_reference_file_count - 1].out_ranges = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].out_end_start = 0;
  gra_reference_file_info[gi_reference_file_count - 1].out_range_count = 0;
  gra_reference_file_info[gi_reference_file_count - 1].key_ranges = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].key_end_start = 0;
  gra_reference_file_info[gi_reference_file_count - 1].key_range_count = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_key_ranges = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_key_end_start = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_key_range_count
    = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_flatten_levels = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_leaf_only = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_direction = 0;
  gra_reference_file_info[gi_reference_file_count - 1].data_key_ranges = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].constant_string = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].missing_string = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].empty_string = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].input_field_delimiter
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].no_input_field_delimiter
    = 0;
  gra_reference_file_info[gi_reference_file_count
			  - 1].input_field_delimiter_repeat = 0;
  gra_reference_file_info[gi_reference_file_count - 1].input_record_delimiter
    = dstrdupstr ("\n");
  gra_reference_file_info[gi_reference_file_count - 1].input_record_length = 0;
  gra_reference_file_info[gi_reference_file_count - 1].data_key_end_start = 0;
  gra_reference_file_info[gi_reference_file_count - 1].data_key_range_count = 0;
  gra_reference_file_info[gi_reference_file_count - 1].unique_ind = 0;
  gra_reference_file_info[gi_reference_file_count - 1].optional_ind = 0;
  gra_reference_file_info[gi_reference_file_count - 1].exclude_match_ind = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_path = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_path_size = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hrchy_curr_depth = 0;
  gra_reference_file_info[gi_reference_file_count - 1].records_read = 0;
  gra_reference_file_info[gi_reference_file_count - 1].records_dropped_filter
    = 0;
  gra_reference_file_info[gi_reference_file_count - 1].records_stored = 0;
  gra_reference_file_info[gi_reference_file_count - 1].records_matched_key = 0;
  gra_reference_file_info[gi_reference_file_count - 1].records_matched = 0;
  gra_reference_file_info[gi_reference_file_count - 1].data_records_matched = 0;
  gra_reference_file_info[gi_reference_file_count - 1].write_ind = 0;
  gra_reference_file_info[gi_reference_file_count - 1].suppress_empty_ind = 0;
  gra_reference_file_info[gi_reference_file_count - 1].output_file_name = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_file_open_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_record_read_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_file_close_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_start_writing_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_record_write_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count
			  - 1].ext_finished_writing_command = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_filter_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_match_command = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].ext_hrchy_match_command
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].output_field_order
    = NULL;
  gra_reference_file_info[gi_reference_file_count - 1].output_field_order_count
    = 0;
  gra_reference_file_info[gi_reference_file_count - 1].hash_size
    = DEFAULT_HASHTABLE_SIZE;
  gra_reference_file_info[gi_reference_file_count - 1].hash_table = 0;
  }

set_general_option (option)
     int option;
{
  switch (option) {
  case 'b':
    gs_output_record_delimiter = dstrdupstr (optarg);
    if (gs_output_record_delimiter == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    break;
  case 'd':
    gs_output_field_delimiter = dstrdupstr (optarg);
    if (gs_output_field_delimiter == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    gi_no_output_field_delimiter = 0;
    break;
  case OPT_NO_OUTPUT_DELIMITER:
    gi_no_output_field_delimiter = -1;
    if (gs_output_field_delimiter != NULL) {
      dstrfree (gs_output_field_delimiter);
      gs_output_field_delimiter = NULL;
      }
    break;
  case 'f':
    gi_flag_ind = 1;
    break;
  case 'M':
    gi_data_is_reference = 1;
    break;
  case 'n':
    gi_counter_ind = 1;
    break;
  case 'R':
    gi_checkallref_ind = 1;
    break;
  case OPT_NO_CHECK_ALL_REFERENCE:
    gi_checkallref_ind = 0;
    break;
  case 's':
    calculate_ranges (optarg, &gra_data_sum_ranges, &gi_data_sum_end_start,
		      &gi_data_sum_range_count);
    break;
  case 'v':
    gi_verbose_ind = 1;
    break;
  case 'x':
    switch (optarg[0]) {
    case 'b':
      gs_ext_begin_command = dstrdupstr (&(optarg[1]));
      if (gs_ext_begin_command == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      break;
    case 'e':
      gs_ext_end_command = dstrdupstr (&(optarg[1]));
      if (gs_ext_end_command == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      break;
    case 'o':
      if (gi_reference_file_count == 0) {
	gs_data_ext_file_open_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_file_open_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_file_open_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_file_open_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'r':
      if (gi_reference_file_count == 0) {
	gs_data_ext_record_read_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_record_read_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_record_read_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_record_read_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'c':
      if (gi_reference_file_count == 0) {
	gs_data_ext_file_close_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_file_close_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_file_close_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_file_close_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 's':
      if (gi_reference_file_count == 0) {
	gs_data_ext_start_writing_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_start_writing_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_start_writing_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_start_writing_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'w':
      if (gi_reference_file_count == 0) {
	gs_data_ext_record_write_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_record_write_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_record_write_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_record_write_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'f':
      if (gi_reference_file_count == 0) {
	gs_data_ext_finished_writing_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_finished_writing_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count
				- 1].ext_finished_writing_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_finished_writing_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'l':
      if (gi_reference_file_count == 0) {
	gs_data_ext_filter_command = dstrdupstr (&(optarg[1]));
	if (gs_data_ext_filter_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      else {
	gra_reference_file_info[gi_reference_file_count - 1].ext_filter_command
	  = dstrdupstr (&(optarg[1]));
	if (gra_reference_file_info[gi_reference_file_count - 1]
	    .ext_filter_command == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	}
      break;
    case 'm':
      assert_reference_file ('m');
      gra_reference_file_info[gi_reference_file_count - 1].ext_match_command
	= dstrdupstr (&(optarg[1]));
      if (gra_reference_file_info[gi_reference_file_count - 1].ext_match_command
	  == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      break;
    case 'h':
      assert_reference_file ('h');
      gra_reference_file_info[gi_reference_file_count
			      - 1].ext_hrchy_match_command
	= dstrdupstr (&(optarg[1]));
      if (gra_reference_file_info[gi_reference_file_count - 1]
	  .ext_hrchy_match_command == NULL)
	FATAL_ERROR (_("unable to allocate memory"));
      break;
      }
    break;
  case 'X':
    gs_ext_init_file = dstrdupstr (optarg);
    if (gs_ext_init_file == NULL)
      FATAL_ERROR (_("unable to allocate memory"));
    break;
  case 'z':
    gi_output_counter_size = strtol (optarg, NULL, 10);
    if (gi_output_counter_size <= 0)
      FATAL_ERROR1 (_("counter size must be a positive number (got '%s')"),
		    optarg);
    break;
  case OPT_STATISTICS:
    gi_statistics_ind = 1;
    break;
  case OPT_NO_STATISTICS:
    gi_statistics_ind = 0;
    break;
    }
  }


int
usage (status)
     int status;
{
  fprintf (stdout, _("combine -- flexible file joins (and more)\n"));
  fprintf (stdout,
	   _("\n"
	     "Reads a data file, checks for matches in any specified reference files, and \n"
	     "writes specified output.\n" "\n"
	     "The following are the main options.  General options and options referring to \n"
	     "the data file should come before any reference file is specified.  Options \n"
	     "appearing after a reference file is specified are applied to the latest reference \n"
	     "file named.  \n"));
  fprintf (stdout,
	   _("\n" "General Options\n" "\n"
	     "  -b, --output-record-delimiter=STRING  put STRING between all output records \n"
	     "  -d, --output-field-delimiter=STRING   put STRING between all output fields \n"
	     "  -f, --flag                            make 1/0 flags indicating matches\n"
	     "  -H, --hash-movement=OPTION            `beginning' or `end' for best variation\n"
	     "  -n, --count                           make counts of the matching data records\n"
	     "  -R, --check-all-reference             keep checking after a non-optional non-match\n"
	     "      --no-check-all-reference          stop checking after a non-optional non-match\n"
	     "  -x, --extension=COMMAND               run a guile COMMAND as specified\n"
	     "  -X, --extension-init-file=FILENAME    load FILENAME as initialization for guile\n"
	     "  -z, --counter-size=NUMBER             make all counters and sums NUMBER bytes long\n"
	     "      --statistics                      write record counts to stderr\n"
	     "      --no-statistics                   do not write record counts to stderr\n"
	     "      --verbose                         write detailed information to stderr\n"));
  fputs (HELP_OPTION_DESCRIPTION, stdout);
  fputs (VERSION_OPTION_DESCRIPTION, stdout);
  fprintf (stdout,
	   _("\n" "Options for Any File\n" "\n"
	     "  -B, --input-record-delimiter=STRING  use STRING to separate input records\n"
	     "  -L, --input-record-length=NUMBER     separate records at NUMBER byte intervals\n"
	     "  -D, --input-field-delimiter=STRING   use STRING to separate fields\n"
	     "  -K, --output-constant=STRING         write STRING in output record\n"
	     "  -o, --output-fields=FIELD-LIST       write FIELD-LIST in output records\n"
	     "  -O, --field-order=FIELD-ORDER-LIST   write output in order of FIELD-ORDER-LIST\n"
	     "  -t, --output-file=FILENAME           write output to FILENAME (default stdout)\n"
	     "  -w, --write-output                   write output based on this file\n"
	     ""));
  fprintf (stdout,
	   _("\n" "Data File Options\n" "\n"
	     "  -s, --sum-fields=FIELD-LIST  sum the values of FIELD-LIST from data records\n"
	     ""));
  fprintf (stdout,
	   _("\n" "Reference File Options\n" "\n"
	     "  -r, --reference-file=FILENAME     FILENAME is the (next) reference file\n"
	     "  -k, --key-fields=FIELD-LIST       match FIELD-LIST to the data file\n"
	     "  -m, --data-key-fields=FIELD-LIST  match FIELD-LIST in the data file to reference\n"
	     "  -p, --match-optional              do not require a match to this file\n"
	     "  -u, --unique                      store only one copy of each distinct set of keys\n"
	     "  -h, --hash-size=NUMBER            initial hash table size (in records)\n"
	     ""));
  fprintf (stdout,
	   _("\n"
	     "FIELD-LIST is a comma-separated list of ranges of bytes or fields (if delimited)\n"
	     "FIELD-ORDER-LIST is a comma-separated list of entries like SOURCE[NUM].TYPE[NUM]\n"));
  fprintf (stdout,
	   _("\n"
	     "For complete documentation, see the info manual at `info combine'.\n"
	     "\n" "Please report bugs to <dpv@world.std.com>\n"));
  exit (status);
  }

int
handle_cut_arguments (argc, argv)
     int argc;
     char **argv;
{
  int c;
  int i;
  char const *short_options = CUT_COMMON_SHORT_OPTIONS;
  version_etc_copyright = "Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine";

  gi_data_write_ind = 1;
  c = getopt_long (argc, argv, short_options, cut_long_options, NULL);
  for (;;) {
    if (c == -1) {
      /* Handle data file names */
      filecount = argc - optind;
      filenames = argv + optind;
      break;
      }
    else
      switch (c) {
      case 'b':
      case 'c':
	/* FIXME: There should be better handling of characters. */
	calculate_ranges (optarg, &gra_data_out_ranges, &gi_data_out_end_start,
			  &gi_data_out_range_count);
	if (gs_data_input_field_delimiter != NULL)
	  FATAL_ERROR (_
		       ("bad combination of bytes and fields in cut emulation"));
	break;
      case 'd':
	/* If the delimiter string is empty, assume the user meant a null 
	   character.  Otherwise, they'd have used -b. */
	if (strlen (optarg) == 0)
	  gs_data_input_field_delimiter = dstrdupstr ("X");
	else
	  gs_data_input_field_delimiter = dstrdupstr (optarg);
	if (gs_data_input_field_delimiter == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	if (strlen (optarg) == 0)
	  gs_data_input_field_delimiter->string[0] = '\0';
	if (gs_output_field_delimiter == NULL)
	  gs_output_field_delimiter = gs_data_input_field_delimiter;
	break;
      case 'f':
	calculate_ranges (optarg, &gra_data_out_ranges, &gi_data_out_end_start,
			  &gi_data_out_range_count);
	if (gs_data_input_field_delimiter == NULL) {
	  gs_data_input_field_delimiter = dstrdupstr ("	");
	  if (gs_data_input_field_delimiter == NULL)
	    FATAL_ERROR (_("unable to allocate memory"));
	  }
	break;
      case 's':
	break;
      case CUT_OPT_OUTPUT_DELIMITER:
	if (strlen (optarg) == 0)
	  gs_output_field_delimiter = dstrdupstr ("X");
	else
	  gs_output_field_delimiter = dstrdupstr (optarg);
	if (gs_output_field_delimiter == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	if (strlen (optarg) == 0)
	  gs_output_field_delimiter->string[0] = '\0';
	break;
	case_GETOPT_HELP_CHAR;
	case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
	usage (EXIT_FAILURE);
	}
    c = getopt_long (argc, argv, short_options, cut_long_options, NULL);
    }
  if (gi_data_out_end_start + gi_data_out_range_count == 0)
    FATAL_ERROR (_("exactly one of -b, -c, or -f needed in cut emulation"));
  }

int
handle_join_arguments (argc, argv)
     int argc;
     char **argv;
{
  int c;
  int i;
  size_t max_ref_field;
  size_t max_data_field;
  size_t end_ref_field;
  size_t end_data_field;
  char temp_string[100];
  char const *short_options = JOIN_COMMON_SHORT_OPTIONS;
  STRINGTYPE temp_dstring;

  version_etc_copyright = "Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine";

  /* Set up defaults for join, a special case of combine. 
     join defaults demand 2 input files (here one reference and one data), the first field in
     each record is the match key, there is exactly one match key field, the output consists of
     the match key, the rest of file 1 (reference here) and the rest of file 2 (data here) . */
  set_reference_file ("temp_reference_file");
  gi_data_write_ind = 1;
  gs_data_input_field_delimiter = dstrdupstr (" ");
  gi_data_no_input_field_delimiter = 0;
  if (gs_data_input_field_delimiter == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  gra_reference_file_info[0].input_field_delimiter = dstrdupstr (" ");
  if (gra_reference_file_info[0].input_field_delimiter == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  gs_output_field_delimiter = dstrdupstr (" ");
  gi_data_input_field_delimiter_repeat = -1;
  gra_reference_file_info[0].input_field_delimiter_repeat = -1;
  gi_data_optional_ind = 0;
  gra_reference_file_info[0].optional_ind = 0;
  gra_reference_file_info[0].unique_ind = -1;
  gs_data_empty_string = dstrdupstr ("");
  if (gs_data_empty_string == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  gra_reference_file_info[0].empty_string = gs_data_empty_string;
  gs_data_missing_string = dstrdupstr ("");
  if (gs_data_missing_string == NULL)
    FATAL_ERROR (_("unable to allocate memory"));
  gra_reference_file_info[0].missing_string = gs_data_missing_string;
  gi_data_suppress_empty_ind = -1;
  gra_reference_file_info[0].suppress_empty_ind = -1;

  if (gs_output_field_delimiter == NULL)
    FATAL_ERROR (_("unable to allocate memory"));

  c = getopt_long (argc, argv, short_options, join_long_options, NULL);
  for (;;) {
    if (c == -1) {
      /* Handle data file names */
      filecount = argc - optind;
      filenames = argv + optind;
      if (filecount != 2)
	FATAL_ERROR (_("join emulation requires exactly 2 input files."));

      /* Get the reference file name */
      gra_reference_file_info[gi_reference_file_count - 1].file_name
	= strdup (filenames[0]);
      if (gra_reference_file_info[gi_reference_file_count - 1].file_name == NULL)
	FATAL_ERROR (_("unable to allocate memory"));

      filecount--;
      filenames++;
      break;
      }
    else
      switch (c) {
      case '1':
	calculate_ranges (optarg, &(gra_reference_file_info[0].key_ranges),
			  &(gra_reference_file_info[0].key_end_start),
			  &(gra_reference_file_info[0].key_range_count));
	break;
      case '2':
	calculate_ranges (optarg, &(gra_reference_file_info[0].data_key_ranges),
			  &(gra_reference_file_info[0].data_key_end_start),
			  &(gra_reference_file_info[0].data_key_range_count));
	break;
      case 'a':
	if (strcmp ("1", optarg) == 0)
	  gi_data_optional_ind = -1;	/* Match to data (2) is optional */
	else if (strcmp ("2", optarg) == 0)
	  gra_reference_file_info[0].optional_ind = -1;	/* 1 is optional */
	else
	  FATAL_ERROR1 (_("file number for join -a must be 1 or 2.  Got %s"),
			optarg);
	break;
      case 'e':
	gs_data_empty_string = dstrdupstr (optarg);
	if (gs_data_empty_string == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	gra_reference_file_info[0].empty_string = gs_data_empty_string;
	gs_data_missing_string = gs_data_empty_string;
	gra_reference_file_info[0].missing_string = gs_data_empty_string;
	gi_data_input_field_delimiter_repeat = 0;
	gra_reference_file_info[0].input_field_delimiter_repeat = 0;
	break;
      case 'i':
	/* FIXME: Need new option to ignore case in comparisons. */
	break;
      case 'j':
	calculate_ranges (optarg, &(gra_reference_file_info[0].key_ranges),
			  &(gra_reference_file_info[0].key_end_start),
			  &(gra_reference_file_info[0].key_range_count));
	calculate_ranges (optarg, &(gra_reference_file_info[0].data_key_ranges),
			  &(gra_reference_file_info[0].data_key_end_start),
			  &(gra_reference_file_info[0].data_key_range_count));
	break;
      case 'o':
	temp_dstring.string = optarg;
	temp_dstring.length = strlen (optarg);
	gra_data_output_field_order
	  = parse_join_order_string (&temp_dstring,
				     &gi_data_output_field_order_count);
	if (gra_data_output_field_order == NULL)
	  FATAL_ERROR (_("unable to parse data output field order"));
	break;
      case 't':
	gs_data_input_field_delimiter = dstrdupstr (optarg);
	if (gs_data_input_field_delimiter == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	gra_reference_file_info[0].input_field_delimiter = dstrdupstr (optarg);
	if (gra_reference_file_info[0].input_field_delimiter == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	gs_output_field_delimiter = dstrdupstr (optarg);
	if (gs_output_field_delimiter == NULL)
	  FATAL_ERROR (_("unable to allocate memory"));
	gi_data_input_field_delimiter_repeat = 0;
	gra_reference_file_info[0].input_field_delimiter_repeat = 0;
	break;
      case 'v':
	if (strcmp ("1", optarg) == 0) {
	  gi_data_optional_ind = -1;	/* Match to data (2) is optional */
	  gi_data_exclude_match_ind = -1;
	  }
	else if (strcmp ("2", optarg) == 0) {
	  gra_reference_file_info[0].optional_ind = -1;	/* 1 is optional */
	  gra_reference_file_info[0].exclude_match_ind = -1;
	  }
	else
	  FATAL_ERROR1 (_("file number for join -v must be 1 or 2.  Got %s"),
			optarg);
	break;
	case_GETOPT_HELP_CHAR;
	case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
	usage (EXIT_FAILURE);
	}
    c = getopt_long (argc, argv, short_options, join_long_options, NULL);
    }

  if (gra_reference_file_info[0].key_range_count == 0
      && gra_reference_file_info[0].key_end_start == 0) {
    calculate_ranges ("1", &(gra_reference_file_info[0].key_ranges),
		      &(gra_reference_file_info[0].key_end_start),
		      &(gra_reference_file_info[0].key_range_count));
    }
  if (gra_reference_file_info[0].data_key_range_count == 0
      && gra_reference_file_info[0].data_key_end_start == 0) {
    calculate_ranges ("1", &(gra_reference_file_info[0].data_key_ranges),
		      &(gra_reference_file_info[0].data_key_end_start),
		      &(gra_reference_file_info[0].data_key_range_count));
    }
  /* FIXME: Need to post-process to ensure we are explicitly listing the fields we want when there is an order. 
     Also, want to make sure that once a key is known, we create the appropriate order to supply the remaining fields. */
  if (gra_data_output_field_order == NULL) {
    temp_dstring.string = "r1.o1,s0.o1,r1.o2-,d.o2-";
    temp_dstring.length = strlen (temp_dstring.string);
    gra_data_output_field_order
      = parse_field_order_string (&temp_dstring,
				  &gi_data_output_field_order_count);
    if (gra_data_output_field_order == NULL)
      FATAL_ERROR (_("unable to parse data output field order"));
    }

  /* Find the fields necessary from reference and data. */
  max_data_field = 0;
  max_ref_field = 0;
  end_data_field = 0;
  end_ref_field = 0;
  for (i = 0; i < gi_data_output_field_order_count; i++) {
    if (gra_data_output_field_order[i].source == 'd'
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num > 0
	&& gra_data_output_field_order[i].type_num > max_data_field)
      max_data_field = gra_data_output_field_order[i].type_num;
    if (gra_data_output_field_order[i].source == 'r'
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num > 0
	&& gra_data_output_field_order[i].type_num > max_ref_field)
      max_ref_field = gra_data_output_field_order[i].type_num;
    if (gra_data_output_field_order[i].source == 'd'
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num < 0)
      end_data_field = -1 * gra_data_output_field_order[i].type_num;
    if (gra_data_output_field_order[i].source == 'r'
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num < 0)
      end_ref_field = -1 * gra_data_output_field_order[i].type_num;
    if (gra_data_output_field_order[i].source == 's'
	&& gra_data_output_field_order[i].source_num == 0
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num > 0
	&& gra_data_output_field_order[i].type_num > max_data_field)
      max_data_field = gra_data_output_field_order[i].type_num;
    if (gra_data_output_field_order[i].source == 's'
	&& gra_data_output_field_order[i].source_num == 1
	&& gra_data_output_field_order[i].type == 'o'
	&& gra_data_output_field_order[i].type_num > 0
	&& gra_data_output_field_order[i].type_num > max_ref_field)
      max_ref_field = gra_data_output_field_order[i].type_num;
    }

  if (max_data_field > 0 && end_data_field > 0) {
    sprintf (temp_string, "1-%ld,%ld-", max_data_field, end_data_field);
    calculate_ranges (temp_string, &(gra_data_out_ranges),
		      &(gi_data_out_end_start), &(gi_data_out_range_count));
    for (i = 0; i < gi_data_output_field_order_count; i++) {
      if (gra_data_output_field_order[i].source == 'd'
	  && gra_data_output_field_order[i].type == 'o'
	  && gra_data_output_field_order[i].type_num < 0)
	gra_data_output_field_order[i].type_num = (gi_data_out_range_count + 1);
      }
    }
  else if (max_data_field > 0) {
    sprintf (temp_string, "1-%ld", max_data_field);
    calculate_ranges (temp_string, &(gra_data_out_ranges),
		      &(gi_data_out_end_start), &(gi_data_out_range_count));
    }
  else {
    gi_data_out_range_count = 0;
    gi_data_out_end_start = 0;
    }

  if (max_ref_field > 0 && end_ref_field > 0) {
    sprintf (temp_string, "1-%ld,%ld-", max_ref_field, end_ref_field);
    calculate_ranges (temp_string, &(gra_reference_file_info[0].out_ranges),
		      &(gra_reference_file_info[0].out_end_start),
		      &(gra_reference_file_info[0].out_range_count));
    for (i = 0; i < gi_data_output_field_order_count; i++) {
      if (gra_data_output_field_order[i].source == 'r'
	  && gra_data_output_field_order[i].type == 'o'
	  && gra_data_output_field_order[i].type_num < 0)
	gra_data_output_field_order[i].type_num
	  = (gra_reference_file_info[0].out_range_count + 1);
      }
    }
  else if (max_ref_field > 0) {
    sprintf (temp_string, "1-%ld", max_ref_field);
    calculate_ranges (temp_string, &(gra_reference_file_info[0].out_ranges),
		      &(gra_reference_file_info[0].out_end_start),
		      &(gra_reference_file_info[0].out_range_count));
    }
  else {
    gra_reference_file_info[0].out_end_start = 0;
    gra_reference_file_info[0].out_range_count = 0;
    }
  }

int
handle_arguments (argc, argv)
     int argc;
     char **argv;
{
  int c;
  char const *short_options = COMMON_SHORT_OPTIONS;

  version_etc_copyright = "Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine";

  c = getopt_long (argc, argv, short_options, long_options, NULL);
  if (c == 'e') {		/* Emulating another command */
    if (strcmp (optarg, "join") == 0)
      handle_join_arguments (argc, argv);
    else if (strcmp (optarg, "cut") == 0)
      handle_cut_arguments (argc, argv);
    else
      FATAL_ERROR1 ("unknown emulation requested: %s", optarg);
    }
  else
    for (;;) {
      if (c == -1) {
	/* Handle data file names */
	filecount = argc - optind;
	filenames = argv + optind;
	break;
	}
      else
	switch (c) {
	case 0:
	  break;
	case 'a':		/* Hierarchy Key */
	case 'F':		/* Hierarchy Flatten */
	case 'h':		/* Hash Size */
	case 'H':		/* Hash Movement */
	case 'k':		/* Key Fields */
	case 'l':		/* Hierarchy Leaf Only */
	case 'm':		/* Data Key Fields */
	case 'u':		/* Unique Keys Only */
	case 'U':		/* Up the Hierarchy */
	  assert_reference_file (c);
	  set_reference_option (c);
	  break;
	case 'B':		/* Input Record Delimiter */
	case 'D':		/* Input Field Delimiter */
	case OPT_INPUT_DELIMITER_REPEAT:	/* Input Field Delimiter Repeat */
	case 'K':		/* Output Constant Field */
	case 'L':		/* Output Record Length */
	case 'o':		/* Output Fields */
	case 'O':		/* Order of Output Fields */
	case 'p':		/* Optional Match */
	case 'P':		/* Exclude Match */
	case 't':		/* Output File */
	case 'w':		/* Write Output */
	  set_file_option (c);
	  break;
	case 'r':		/* Reference file */
	  set_reference_file (optarg);
	  break;
	case 'b':		/* Output Record Delimiter */
	case 'd':		/* Output Field Delimiter */
	case 'f':		/* Flag */
	case 'M':		/* Use reference file as data */
	case 'n':		/* Count */
	case 'R':		/* Check all reference */
	case OPT_NO_CHECK_ALL_REFERENCE:	/* Do not check all reference */
	case 's':		/* Sum Fields */
	case 'v':		/* Verbose */
	case 'x':		/* Extension */
	case 'X':		/* Extension Init File */
	case 'z':		/* Counter Size */
	case OPT_STATISTICS:
	case OPT_NO_STATISTICS:
	  set_general_option (c);
	  break;
	  case_GETOPT_HELP_CHAR;

	  case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

	default:
	  usage (EXIT_FAILURE);
	  }
      c = getopt_long (argc, argv, short_options, long_options, NULL);
      }
  }
