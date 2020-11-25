/*!
*  \file rc-use.c
*  \brief Central function, manages the resource file handling
*             and displays the results.
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000, 2011 Thomas Esken
*  Copyright (c) 2010, 2011, 2013, 2014 Free Software Foundation, Inc.
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 3, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*/



/*
*  Include definition header file to see whether USE_RC is defined there.
*    Compile this module only if USE_RC is defined, otherwise skip it.
*/
#include "tailor.h"



#if USE_RC


/*
*  Include header files.
*/
# if HAVE_ASSERT_H
#  include <assert.h>
# endif
# if HAVE_CTYPE_H
#  include <ctype.h>
# endif
# if HAVE_LIMITS_H
#  include <limits.h>
# endif
# include "common.h"
# include "rc-defs.h"
# include "globals.h"
# include "file-io.h"
# include "hd-defs.h"
# include "hd-use.h"
# include "rc-astro.h"
# include "rc-check.h"
# include "rc-insert.h"
# include "rc-utils.h"
# include "tty.h"
# include "utils.h"
# include "rc-use.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-use.c'.
*/
static void try_to_include_file __P_ ((int ed, int wd));
static void
  display_table __P_ ((const int tmp_ad,
		       const int tmp_am,
		       const int tmp_ay, int day, int ed, int wd));
static int fn_asc_sort __P_ ((const char **a, const char **b));
static int fn_des_sort __P_ ((const char **a, const char **b));
__END_DECLARATIONS
/*
*  GLOBAL variables definitions.
*/
/*! Date variables a[=MMDD]...z[] (YYYY@{a|b|...|z}[[-]N]). */
  Dvar_struct rc_dvar[RC_DVAR_MAX];

/*! Text variables $a[=TEXT]...$z[]. */
Tvar_struct rc_tvar[RC_TVAR_MAX];

/*! Local co-ordinates used by both the %distance and %sun* special texts. */
Coor_struct lcoor1;

/*! Points to the local co-ordinates 1. */
Coor_struct *coor1 = &lcoor1;

/*! Local co-ordinates used by the %distance special text. */
Coor_struct lcoor2;

/*! Points to the local co-ordinates 2. */
Coor_struct *coor2 = &lcoor2;

/*! Pointers to different parts of a (resource file) line. */
Line_struct *lineptrs = (Line_struct *) NULL;

/*! Pointers to different parts of a (resource file) line. */
Line_struct *lptrs = (Line_struct *) NULL;

/*! Pointers to different parts of a (resource file) line. */
Line_struct *lptrs2 = (Line_struct *) NULL;

/*! Pointers to different parts of a (resource file) line. */
Line_struct *lptrs3 = (Line_struct *) NULL;

/*! Temporary file used when a command is assigned to a TVAR. */
FILE *rc_tvar_tfp = (FILE *) NULL;

/*! Temporary file used for managing `--here=ARG' options. */
FILE *rc_here_fp = (FILE *) NULL;

/*! Time displacement value in days used by %sun* and %moon*
   (also used as cycle-starting time value for %sun* and %moon* special texts). */
double time_offset = 0.0;

/*! Atmospheric pressure in millibar (`--atmosphere=PRESSURE,TEMPERATURE'). */
double atm_pressure = DEFAULT_PRESSURE;

/*! Atmospheric temperature in degrees Celsius (`--atmosphere=PRESSURE,TEMPERATURE'). */
double atm_temperature = DEFAULT_TEMPERATURE;

/*! Adjust rise/set-based reference altitude resp. shadow length factor (`--adjust-value=NUMBER'). */
double adjust_value = DEGS_PER_24_HOURS;

/*! Actual size of `rc_elems_table[]'. */
Uint rc_elems_max = RC_ELEMS_MAX;

/*! Amount of period of fixed dates. */
int rc_period = 0;

/*! Amount of resource file entries. */
int rc_elems = 0;

/*! Starting index of dummy resource file entries in table. */
int rc_zero_pos = 0;

/*! `-cd'. */
int rc_have_today_in_list = 0;

/*! Length of a single axis of a biorhythm text graphics bar. */
int rc_bio_axis_len = BIO_AXIS_DEF;

/*! Number of lines of a Moon phase text graphics image. */
int rc_moonimage_lines = MOONIMAGE_DEF;

/*! Filler length of week number text. */
int len_fil_wt = 0;

/*! Actual size of text buffer of "text"-part of a line. */
int len_the_text = 0;

/*! Cycle-ending time value in minutes for %sun* and %moon* special texts. */
int loop_end = SPECIAL_VALUE;

/*! Cycle-timestep value in minutes for %sun* and %moon* special texts. */
int loop_step = DEFAULT_CYCLE_STEP;

/*! Actual GMT minute. */
int gmt_min = 0;

/*! Actual GMT hour. */
int gmt_hour = 0;

/*! Buffer of actual GMT day. */
int buf_gd = 0;

/*! Buffer of actual GMT month. */
int buf_gm = 0;

/*! Buffer of actual GMT year. */
int buf_gy = 0;

/*! Day difference of GMT and local date. */
int gmt_loc_diff = 0;

/*! Indicates whether event also appears in next year. */
int incr_year = 0;

/*! Indicates whether event also appears in previous year. */
int decr_year = 0;

/*! Day of event found in line. */
int d = 0;

/*! Month of event found in line. */
int m = 0;

/*! Year of event found in line. */
int y = 0;

/*! Buffered day of event. */
int d_buf = 0;

/*! Buffered month of event. */
int m_buf = 0;

/*! The `N'th weekday of month' displacement value. */
int hn = 0;

/*! The weekday number of `N'th weekday of month'. */
int hwd = 0;

/*! Name of tempfile used when a command is assigned to a TVAR. */
char *rc_tvar_tfn = (char *) NULL;

/*! Name of tempfile used for managing `--here=ARG' options. */
char *rc_here_fn = (char *) NULL;

/*! Text buffer of "text"-part of a line. */
char *the_text = (char *) NULL;

/*! General purpose text buffer 5. */
char *s5 = (char *) NULL;

/*! General purpose text buffer 6. */
char *s6 = (char *) NULL;

/*! General purpose text buffer 7. */
char *s7 = (char *) NULL;

/*! Text buffer of a line read from a resource file. */
char *line_buffer = (char *) NULL;

/*! Stores the valid fixed date texts. */
char **rc_elems_table = (char **) NULL;

/*! Text of modified actual date %DATE. */
char *rc_adate = (char *) NULL;

/*! Name of alternative resource file(s) `-f|F<NAME[+...]>'. */
char *rc_filename = (char *) NULL;

/*! Argument used for filtering fixed date days. */
char *rc_filter_day = (char *) NULL;

/*! Argument used for filtering fixed date periods. */
char *rc_filter_period = (char *) NULL;

/*! REGEX used for filtering fixed date texts. */
char *rc_filter_text = (char *) NULL;

/*! Fixed date list grouping separator `-cg[TEXT]'. */
char *rc_grp_sep = (char *) NULL;

/*! Fixed date list heading text `--heading-text=TEXT'. */
char *rc_heading_text = (char *) NULL;

/*! The biorhythm's "Emo" text. */
char *rc_bio_emo_lit = (char *) NULL;

/*! The biorhythm's "Int" text. */
char *rc_bio_int_lit = (char *) NULL;

/*! The biorhythm's "Phy" text. */
char *rc_bio_phy_lit = (char *) NULL;

/*! The mode specifying character. */
char hc = '\0';

/*! `-jc'. */
Bool rc_special_flag = FALSE;

/*! `-jcb'. */
Bool rc_both_dates_flag = FALSE;

/*! `-c'. */
Bool rc_use_flag = FALSE;

/*! `-C[]' or `-C[][T|W|M|Y]' or `-c[][T|W|M|Y]' or `-F<>'. */
Bool rc_all_dates_flag = FALSE;

/*! `-c-'. */
Bool rc_sort_des_flag = FALSE;

/*! `-ca'. */
Bool rc_enable_fn_flag = FALSE;

/*! `-cA'. */
Bool rc_alternative_format_flag = FALSE;

/*! `--execute-command'. */
Bool rc_execute_command = FALSE;

/*! `-ce'. */
Bool rc_enable_hda_flag = FALSE;

/*! `-cE'. */
Bool rc_enable_hdl_flag = FALSE;

/*! `-ck'. */
Bool rc_week_number_flag = FALSE;

/*! `-cl'. */
Bool rc_period_list = FALSE;

/*! `-co'. */
Bool rc_omit_date_flag = FALSE;

/*! `-cU'. */
Bool rc_suppr_date_part_flag = FALSE;

/*! `-cQ'. */
Bool rc_suppr_list_sep_flag = FALSE;

/*! `-cJ'. */
Bool rc_suppr_text_part_flag = FALSE;

/*! `-cx'. */
Bool rc_title_flag = TRUE;

/*! `-cz'. */
Bool rc_count_flag = FALSE;

/*! `-cZ'. */
Bool rc_zero_dates_flag = FALSE;

/*! `-cN[d|w|+|-]|MMDD|MMWW[W]N'. */
Bool rc_period_flag = FALSE;

/*! `-c]t'. */
Bool rc_tomorrow_flag = FALSE;

/*! `-c]w'. */
Bool rc_week_flag = FALSE;

/*! `-c]m'. */
Bool rc_month_flag = FALSE;

/*! `-c]y'. */
Bool rc_year_flag = FALSE;

/*! `-cNw'. */
Bool rc_week_year_flag = FALSE;

/*! `-c<N|w|m|y>+'. */
Bool rc_forwards_flag = FALSE;

/*! Buffers the state of `rc_forwards_flag'. */
Bool rc_fwdf_buffer = FALSE;

/*! `-c<N|w|m|y>-'. */
Bool rc_backwards_flag = FALSE;

/*! Buffers the state of `rc_backwards_flag'. */
Bool rc_bwdf_buffer = FALSE;

/*! `--leap-day=february'. */
Bool rc_feb_29_to_feb_28 = FALSE;

/*! `--leap-day=march'. */
Bool rc_feb_29_to_mar_01 = FALSE;

/*! `--precise' to display precise, non-rounded, times and data. */
Bool rc_precise = FALSE;

/*! `--export-date-variables'. */
Bool rc_export_ldvar_flag = FALSE;

/*! `--export-text-variables'. */
Bool rc_export_ltvar_flag = FALSE;

/*! `--ignore-case' to ignore case distinctions in PATTERN. */
Bool rc_ignore_case_flag = FALSE;

/*! `--limit' to calculate rise/set times limited to the current day only. */
Bool rc_limit = FALSE;

/*! `--revert-match' to select non-matching PATTERN lines. */
Bool rc_revert_match_flag = FALSE;

/*! Is a command (explicit date) given in the command line? */
Bool is_date_given = FALSE;

/*! Does the command enables a year implicitly? */
Bool date_enables_year = FALSE;

/*! Stores whether a %shell escape special text is run. */
Bool shell_escape_done = FALSE;

/*! `-cNw' and complete week is in month. */
Bool is_1month_mode = FALSE;

/*! `-cNw' and only part of week is in month. */
Bool is_2month_mode = FALSE;

/*! Reference to a date variable found in line. */
Bool is_2dvar = FALSE;

/*! Reference to Easter Sundays date found in line. */
Bool is_2easter = FALSE;

/*! `-cNw' and actual date modified. */
Bool adate_set = FALSE;

/*! Remove highlighting sequences before searching PATTERN? */
Bool remove_hls_in_regex = FALSE;



/*
*  The REGEX stuff; global variables that represent the "remembered" search PATTERN.
*/
# if HAVE_GNU_RE_COMPILE_PATTERN
struct re_pattern_buffer regpattern;
char *gnu_fastmap_table = (char *) NULL;
unsigned char *gnu_translate_table = (unsigned char *) NULL;
# endif

# if HAVE_POSIX_REGCOMP
regex_t regpattern;
# endif

# if HAVE_RE_COMP
int re_pattern = 0;
# endif


/*
*  static variables definitions.
*/
/*! Table of resource/include file buffers. */
static File_struct **rc_files_table = (File_struct **) NULL;

/*! Actual size of `rc_files_table[]'. */
static Uint rc_files_max = RC_FILES_MAX;

/*! Amount of resource file buffers. */
static int rc_files = 0;

/*! Number of characters in `line_buffer'. */
static int line_length = 0;

/*! An include statement is found in file. */
static Bool is_include = FALSE;

/*! A date variable statement is found in file. */
static Bool is_dvar = FALSE;

/*! A text variable statement is found in file. */
static Bool is_tvar = FALSE;

/*! File contains an invalid include file name? */
static Bool bad_sys_include = FALSE;



/*
*  Function implementations.
*/
void
rc_use ()
/*!
   Processes a standard/special resource file and displays the valid fixed
   dates found resp., the valid fixed dates of eternal holiday list.
*/
{
  register int wd = weekday_of_date (act_day, act_month, act_year);
  register int ed;
  register int i;
  register int j;
  register int tmp_month = month;
  register int tmp_fiscal_month = fiscal_month;
  register int tmp_start_day = start_day;
  register int tmp_ad = act_day;
  register int tmp_am = act_month;
  register int tmp_ay = act_year;
  register int tindex = 0;
  auto char *ptr_char;
# if HAVE_ASSERT_H
  static Bool is_table_range_checked = FALSE;
# endif
  static Bool tables_initialized = FALSE;


  /*
     Initialize some important module global variables.
   */
  len_fil_wt = rc_elems = rc_files = 0;
  len_the_text = (int) maxlen_max;
# if HAVE_ASSERT_H
  /*
     Check if the value for the maximum number of table entries
     fits to the positive range of a signed int (INT_MAX/SHRT_MAX)!
   */
  if (!is_table_range_checked)
    {
      assert (rc_elems_max > 0);
      assert (rc_elems_max <= testval);
      assert (rc_files_max > 0);
      assert (rc_files_max <= testval);
      assert (len_the_text > 0);
      assert (len_the_text <= testval);
      is_table_range_checked = TRUE;
    }
# endif
  if (!tables_initialized)
    {
      /*
         Initialize the biorhythms phase texts consisting of 3 characters each.
       */
      /*
       *** Translators, please translate this as a fixed 3-character text.
       *** This text should be a proper abbreviation of "Emotional".
       */
      rc_bio_emo_lit = _("Emo");
      /*
       *** Translators, please translate this as a fixed 3-character text.
       *** This text should be a proper abbreviation of "Intellectual".
       */
      rc_bio_int_lit = _("Int");
      /*
       *** Translators, please translate this as a fixed 3-character text.
       *** This text should be a proper abbreviation of "Physical".
       */
      rc_bio_phy_lit = _("Phy");
      /*
         Initial memory allocation for an element of the `Line_struct' record
         which is needed if we have to parse and evaluate a line.
       */
      lptrs = (Line_struct *) my_malloc (sizeof (Line_struct),
					 ERR_NO_MEMORY_AVAILABLE,
					 __FILE__, ((long) __LINE__) - 2L,
					 "lptrs", 0);
      /*
         Initial memory allocation for an element of the `Line_struct' record
         which is needed if we have to evaluate %?... special texts.
       */
      lptrs2 = (Line_struct *) my_malloc (sizeof (Line_struct),
					  ERR_NO_MEMORY_AVAILABLE,
					  __FILE__, ((long) __LINE__) - 2L,
					  "lptrs2", 0);
      /*
         Initial memory allocation for `rc_files_table[]'.
       */
      rc_files_table =
	(File_struct **) my_malloc (RC_FILES_MAX * sizeof (File_struct *),
				    ERR_NO_MEMORY_AVAILABLE, __FILE__,
				    ((long) __LINE__) - 2L,
				    "rc_files_table[RC_FILES_MAX]",
				    RC_FILES_MAX);
      /*
         Initial memory allocation for `rc_elems_table[]'.
       */
      rc_elems_table = (char **) my_malloc (RC_ELEMS_MAX * sizeof (char *),
					    ERR_NO_MEMORY_AVAILABLE,
					    __FILE__, ((long) __LINE__) - 2L,
					    "rc_elems_table[RC_ELEMS_MAX]",
					    RC_ELEMS_MAX);
      /*
         Initial memory allocation for `the_text'.
       */
      the_text = (char *) my_malloc (len_the_text, ERR_NO_MEMORY_AVAILABLE,
				     __FILE__, ((long) __LINE__) - 1L,
				     "the_text", 0);
      /*
         The REGEX stuff in case `--filter-text=PATTERN' is given.
       */
      if (rc_filter_text != (char *) NULL)
	{
	  /*
	     Compile `rc_filter_text' PATTERN.
	   */
# if !HAVE_GNU_RE_COMPILE_PATTERN
	  if (rc_ignore_case_flag)
	    {
	      /*
	         Set PATTERN to lower-case letters
	         if we have to ignore case distinctions.
	       */
	      ptr_char = rc_filter_text;
	      for (; *ptr_char; ptr_char++)
		*ptr_char = (char) tolower (*ptr_char);
	    }
# else /* HAVE_GNU_RE_COMPILE_PATTERN */
	  /*
	     Compute the GNU Regex table size.
	   */
#  ifndef CHAR_BIT
	  auto Uchar bit;


	  for (i = 0, bit = 2; bit; bit <<= 1, i++)
	    ;			/* Void, nothing to do here! */
#  else	/* CHAR_BIT */
	  i = CHAR_BIT - 1;
#  endif /* CHAR_BIT */
	  j = ((1 << i) - 1) + (1 << i);
	  /*
	     Initial memory allocation of GNU Regex fastmap table.
	   */
	  gnu_fastmap_table =
	    (char *) my_malloc (j + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				((long) __LINE__) - 1L, "gnu_fastmap_table",
				0);
	  /*
	     Initial memory allocation and initialization of GNU Regex translate table.
	   */
	  gnu_translate_table =
	    (unsigned char *) my_malloc (j + 1, ERR_NO_MEMORY_AVAILABLE,
					 __FILE__, ((long) __LINE__) - 1L,
					 "gnu_translate_table", 0);
	  if (rc_ignore_case_flag)
	    /*
	       Set PATTERN to lower-case letters
	       if we have to ignore case distinctions.
	     */
	    for (i = 0; i <= j; i++)
	      gnu_translate_table[i] = (unsigned char) tolower (i);
	  else
	    for (i = 0; i <= j; i++)
	      gnu_translate_table[i] = (unsigned char) i;
	  (void)
	    re_set_syntax ((RE_SYNTAX_POSIX_EXTENDED |
			    RE_BACKSLASH_ESCAPE_IN_LISTS) &
			   ~(RE_DOT_NOT_NULL));
	  regpattern.fastmap = gnu_fastmap_table;
	  regpattern.translate = gnu_translate_table;
	  if (re_compile_pattern
	      (rc_filter_text, (int) strlen (rc_filter_text),
	       &regpattern) != (char *) NULL)
	    my_error (ERR_INVALID_REGEX_PATTERN, "", 0L, rc_filter_text, 0);
# endif	/* HAVE_GNU_RE_COMPILE_PATTERN */
# if HAVE_POSIX_REGCOMP
	  if (regcomp (&regpattern, rc_filter_text, REGCOMP_FLAG))
	    my_error (ERR_INVALID_REGEX_PATTERN, "", 0L, rc_filter_text, 0);
# endif
# if HAVE_RE_COMP
	  if (re_comp (rc_filter_text) != (char *) NULL)
	    my_error (ERR_INVALID_REGEX_PATTERN, "", 0L, rc_filter_text, 0);
	  re_pattern = 1;
# endif
	  /*
	     Is it necessary to remove highlighting sequences
	     in each text before searching the PATTERN?
	   */
	  remove_hls_in_regex = highlight_flag;
	}
      tables_initialized = TRUE;
    }
  /*
     Depending on fixed date mode,
     compute last valid day `ed' a fixed date may occur.
   */
  ed = day = day_of_year (act_day, act_month, act_year);
  if (is_date_given)
    {
      /*
         NOT in simple month/year mode (an explicit date is given in the command line):
         Compute the starting/ending loop values of the requested period.
       */
      ed = DAY_LAST + is_leap_year + 1;
      if (!month)
	day = DAY_MIN;
      else
	{
	  day = day_of_year (DAY_MIN, month, year);
	  if (month < MONTH_MAX)
	    ed = day_of_year (DAY_MIN, month + 1, year);
	}
      /*
         Then clean all flags which are related to the fixed date period.
       */
      rc_clean_flags ();
      if (date_enables_year)
	rc_year_flag = TRUE;
    }
  else
    {
      auto int dd;
      auto int mm;
      auto int yy = act_year;
      auto int greg_missing_days = (greg->last_day - greg->first_day + 1);
      auto Bool greg_correction = FALSE;
      auto Bool swap_flag = FALSE;


      if (rc_tomorrow_flag)
	{
	  /*
	     `-ct' option found.
	   */
	  rc_period_list = FALSE;
	  ed += 2;
	  if ((act_year == greg->year)
	      && (act_month == greg->month)
	      && (act_day == greg->first_day - 1))
	    ed += greg_missing_days;
	}
      else if (rc_week_flag)
	{
	  /*
	     `-cw[+|-]' option found.
	   */
	  rc_period_list = FALSE;
	  if (!rc_forwards_flag && !rc_backwards_flag)
	    day += (-DAY_MAX + SDAY (DAY_MAX - wd + 1, start_day));
	  else if (rc_forwards_flag)
	    day += !rc_have_today_in_list;
	  else
	    day += rc_have_today_in_list;
	  if (rc_backwards_flag
	      || (!rc_forwards_flag
		  && !rc_backwards_flag && (start_day > wd)))
	    {
	      swap_flag = TRUE;
	      ed -= DAY_MAX;
	    }
	  ed += SDAY (DAY_MAX - wd + 1, start_day);
	  if (act_year == greg->year)
	    greg_correction = TRUE;
	}
      else if (rc_month_flag)
	{
	  /*
	     `-cm[+|-]' option found.
	   */
	  rc_period_list = FALSE;
	  if (!rc_forwards_flag && !rc_backwards_flag)
	    day = day_of_year (DAY_MIN, act_month, act_year);
	  else if (rc_forwards_flag)
	    day += !rc_have_today_in_list;
	  else
	    day += rc_have_today_in_list;
	  if (rc_backwards_flag)
	    ed = day_of_year (DAY_MIN, act_month, act_year);
	  else
	    {
	      if (act_month < MONTH_MAX)
		ed = day_of_year (DAY_MIN, act_month + 1, act_year);
	      else
		ed = DAY_LAST + is_leap_year + 1;
	    }
	}
      else if (rc_year_flag)
	{
	  /*
	     `-cy[+|-]' option found.
	   */
	  rc_period_list = FALSE;
	  if (!rc_forwards_flag && !rc_backwards_flag)
	    day = DAY_MIN;
	  else if (rc_forwards_flag)
	    day += !rc_have_today_in_list;
	  else
	    day += rc_have_today_in_list;
	  if (rc_backwards_flag)
	    ed = DAY_MIN;
	  else
	    ed = DAY_LAST + is_leap_year + 1;
	}
      else if (rc_period_flag)
	{
	  if (rc_week_year_flag)
	    {
	      /*
	         `-cNw' option found
	       */
	      if (iso_week_number)
		/*
		   Those ISO-8601:1988 weeks starts on Monday.
		 */
		wd = start_day = DAY_MIN;
	      /*
	         Compute the day number of year the week starts at.
	       */
	      j = day =
		weekno2doy (rc_period, act_year, iso_week_number, start_day);
	      if (day != -WEEK_MAX)
		{
		  if (rc_period_list)
		    {
		      /*
		         `-cNw' option found.
		       */
		      if (day > ed)
			{
			  if (day <= DAY_LAST + is_leap_year)
			    {
			      rc_forwards_flag = TRUE;
			      day = ed + 1 - rc_have_today_in_list;
			      ed = j + DAY_MAX;
			      /*
			         Correction in case week occurs during Gregorian Reformation period.
			       */
			      if (act_year == greg->year)
				{
				  (void) doy2date (j, is_leap_year, &dd, &mm);
				  for (i = j; i < ed; i++)
				    if (!next_date (&dd, &mm, &yy))
				      {
					ed += greg_missing_days;
					break;
				      }
				}
			      if (ed > DAY_LAST + is_leap_year + 1)
				ed = DAY_LAST + is_leap_year + 1;
			    }
			  else
			    /*
			       No fixed date messages to display.
			     */
			    day = ed;
			}
		      else
			{
			  if (ed > DAY_MIN)
			    {
			      rc_backwards_flag = TRUE;
			      day = ed + rc_have_today_in_list;
			      ed = j;
			      if (ed < DAY_MIN)
				ed = DAY_MIN;
			    }
			  else
			    /*
			       No fixed date messages to display.
			     */
			    day = ed;
			}
		    }
		  else
		    {
		      /*
		         `-cNw' option found:
		         Set actual date to Mondays date of week
		         and omit highlighting that date; boolean
		         `adate_set' is set to distinct this
		         SPECIAL mode from other modes...
		       */
		      rc_week_flag = TRUE;
		      ed = day + DAY_MAX;
		      /*
		         Correction in case week occurs during Gregorian Reformation period.
		       */
		      if (act_year == greg->year)
			{
			  (void) doy2date (day, is_leap_year, &dd, &mm);
			  for (i = day; i < ed; i++)
			    if (!next_date (&dd, &mm, &yy))
			      {
				ed += greg_missing_days;
				break;
			      }
			}
		      if (day < DAY_MIN)
			{
			  act_year--;
			  j +=
			    (DAY_LAST + (days_of_february (act_year) == 29));
			}
		      else
			j = day;
		      (void) doy2date (j, is_leap_year, &act_day, &act_month);
		      month = act_month;
		      fiscal_month = MONTH_MIN;
		      adate_set = TRUE;
		    }
		}
	      else
		/*
		   Wanted week doesn't exist:
		   Don't display any fixed date messages.
		 */
		day = ed;
	    }
	  else
	    /*
	       `-cNd', `-cMMDD', `-cMMWW[W]N', `-c*d|wN[WW[W]' and
	       `-c@e|t|DVAR[[-]N] options are implicitly managed in
	       this subsection, too.
	     */
	  if (rc_forwards_flag && (day < DAY_LAST + is_leap_year))
	    {
	      if (rc_period_list)
		{
		  /*
		     `-clN+' option found (list of dates).
		   */
		  day += !rc_have_today_in_list;
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if (rc_fwdf_buffer
		      && (act_year == greg->year)
		      && (ed <
			  day_of_year (greg->first_day, greg->month,
				       greg->year))
		      && (ed + rc_period >=
			  day_of_year (greg->first_day, greg->month,
				       greg->year)))
		    ed += greg_missing_days;
		  ed += (rc_period + 1);
		  if (ed > DAY_LAST + is_leap_year + 1)
		    ed = DAY_LAST + is_leap_year + 1;
		}
	      else
		{
		  /*
		     `-cN+ option found (single date).
		   */
		  rc_forwards_flag = FALSE;
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if (rc_fwdf_buffer
		      && (act_year == greg->year)
		      && (day <
			  day_of_year (greg->first_day, greg->month,
				       greg->year))
		      && (day + rc_period >=
			  day_of_year (greg->first_day, greg->month,
				       greg->year)))
		    day += greg_missing_days;
		  day += rc_period;
		  ed = day;
		  if (day < DAY_LAST + is_leap_year + 1)
		    {
		      rc_period_flag = FALSE;
		      (void) doy2date (day, is_leap_year, &act_day,
				       &act_month);
		      month = act_month;
		      ed++;
		    }
		}
	    }
	  else if (rc_backwards_flag && (day > DAY_MIN))
	    {
	      if (rc_period_list)
		{
		  /*
		     `-clN-' option found (list of dates).
		   */
		  day += rc_have_today_in_list;
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if (rc_bwdf_buffer
		      && (act_year == greg->year)
		      && (ed >
			  day_of_year (greg->last_day, greg->month,
				       greg->year))
		      && (ed - rc_period <=
			  day_of_year (greg->last_day, greg->month,
				       greg->year)))
		    ed -= greg_missing_days;
		  ed -= rc_period;
		  if (ed < DAY_MIN)
		    ed = DAY_MIN;
		}
	      else
		{
		  /*
		     `-cN-' option found (single date).
		   */
		  rc_backwards_flag = FALSE;
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if (rc_bwdf_buffer
		      && (act_year == greg->year)
		      && (day >
			  day_of_year (greg->last_day, greg->month,
				       greg->year))
		      && (day - rc_period <=
			  day_of_year (greg->last_day, greg->month,
				       greg->year)))
		    day -= greg_missing_days;
		  day -= rc_period;
		  ed = day;
		  if (day > 0)
		    {
		      rc_period_flag = FALSE;
		      (void) doy2date (day, is_leap_year, &act_day,
				       &act_month);
		      month = act_month;
		      ed++;
		    }
		}
	    }
	}
      else
	{
	  /*
	     Only a simple option `-c' (without any modifiers) found.
	   */
	  rc_period_list = FALSE;
	  ed++;
	}
      /*
         Swap the starting date `day' and final date `ed' of the period.
       */
      if (swap_flag || rc_backwards_flag)
	{
	  int tmp = ed;
	  ed = day;
	  day = tmp;
	}
      /*
         Correction in case date occurs during Gregorian Reformation period.
       */
      if (greg_correction)
	{
	  (void) doy2date (day, is_leap_year, &dd, &mm);
	  if (!prev_date (&dd, &mm, &yy))
	    day -= greg_missing_days;
	  else
	    {
	      if (!rc_forwards_flag && !rc_backwards_flag)
		{
		  (void) doy2date (day, is_leap_year, &dd, &mm);
		  if (weekday_of_date (dd, mm, yy) != start_day)
		    day -= greg_missing_days;
		  else
		    for (i = day; i < ed; i++)
		      if (!next_date (&dd, &mm, &yy))
			{
			  ed += greg_missing_days;
			  break;
			}
		}
	      else
		{
		  auto Bool ed_set = FALSE;


		  for (i = day; i < ed; i++)
		    if (!next_date (&dd, &mm, &yy))
		      {
			ed += greg_missing_days;
			ed_set = TRUE;
			break;
		      }
		  if (rc_backwards_flag && ed_set)
		    {
		      (void) doy2date (day, is_leap_year, &dd, &mm);
		      if (weekday_of_date (dd, mm, yy) != start_day)
			{
			  day -= greg_missing_days;
			  ed -= greg_missing_days;
			}
		    }
		}
	    }
	}
    }
  if (day != ed)
    {
      auto double save_time_offset = time_offset;
      register int save_hour_offset = time_hour_offset;
      register int save_min_offset = time_min_offset;
      register int save_loop_end = loop_end;
      auto char *tmp_rc_here_fn;
      auto Bool cycle_increment = TRUE;
      auto Bool is_here_file;
      auto Bool ok;


      /*
         Now include the eternal holidays, which are valid fixed dates,
         into `rc_elems_table[]'.
       */
      if (rc_enable_hda_flag || rc_enable_hdl_flag)
	{
	  while ((hd_table[tindex] != (char *) NULL)
		 && (tindex < HD_ELEMS_MAX))
	    {
	      strcpy (line_buffer, hd_table[tindex]);
	      i = LEN_HD_NAME + len_year_max + 4 + 2;
	      if (rc_enable_hda_flag
		  || (rc_enable_hdl_flag
		      && (line_buffer[i] != *DIS_HLS_PREF)
		      && (line_buffer[i] != *DIS_HLS_PREF2)))
		{
		  i -= 2;
		  while (i && isspace (line_buffer[i]))
		    line_buffer[i--] = '\0';
		  if (i > len_year_max + 4)
		    rc_check (line_buffer, _("`Eternal holiday list'"),
			      (long) tindex, i, &rc_elems, day, ed, wd);
		}
	      if (!holiday_flag)
		{
		  free (hd_table[tindex]);
		  hd_table[tindex] = (char *) NULL;
		}
	      tindex++;
	    }
	  /*
	     If the fixed date mode is related to a week or to tomorrow
	     and the period has left the current year, include those
	     eternal holidays in year +/- 1 into `rc_elems_table',
	     which are valid fixed dates.
	   */
	  if ((rc_tomorrow_flag
	       || rc_week_flag)
	      && !is_date_given
	      && !is_1month_mode
	      && !is_2month_mode
	      && ((!rc_forwards_flag
		   && !rc_backwards_flag
		   && (((year + 1 <= YEAR_MAX)
			&& (ed > DAY_LAST + is_leap_year + 1))
		       || ((year - 1 >= YEAR_MIN)
			   && (day < DAY_MIN))))
		  || (rc_forwards_flag
		      && ((year + 1 <= YEAR_MAX)
			  && (ed > DAY_LAST + is_leap_year + 1)))
		  || (rc_backwards_flag
		      && ((year - 1 >= YEAR_MIN) && (day < DAY_MIN)))))
	    {
	      register int jtmp_ad = act_day;
	      register int tay = act_year;
	      register int tam = act_month;
	      register int tad = act_day;


	      if (holiday_flag)
		for (i = 0; i < tindex; i++)
		  {
		    free (hd_table[i]);
		    hd_table[i] = (char *) NULL;
		  }
	      tindex = 0;
	      if (day < DAY_MIN)
		{
		  year--;
		  month = MONTH_MAX;
		}
	      else
		{
		  year++;
		  month = MONTH_MIN;
		}
	      is_leap_year = (days_of_february (year) == 29);
	      if (cal_special_flag)
		act_day = day_of_year (jtmp_ad, act_month, act_year);
	      if (adate_set)
		fiscal_month = tmp_fiscal_month;
	      if (((year == EASTER_MIN - 1)
		   && (fiscal_month > MONTH_MIN))
		  || ((year >= EASTER_MIN) && (year <= EASTER_MAX)))
		print_all_holidays (TRUE, TRUE);
	      if (adate_set)
		fiscal_month = MONTH_MIN;
	      if (day < DAY_MIN)
		{
		  year++;
		  month = MONTH_MIN;
		}
	      else
		{
		  year--;
		  month = MONTH_MAX;
		}
	      while ((hd_table[tindex] != (char *) NULL)
		     && (tindex < HD_ELEMS_MAX))
		{
		  strcpy (line_buffer, hd_table[tindex]);
		  i = LEN_HD_NAME + len_year_max + 4 + 2;
		  if (rc_enable_hda_flag
		      || (rc_enable_hdl_flag
			  && (line_buffer[i] == *DIS_HLS_PREF)
			  && (line_buffer[i] == *DIS_HLS_PREF2)))
		    {
		      i -= 2;
		      while (i && isspace (line_buffer[i]))
			line_buffer[i--] = '\0';
		      if (i > len_year_max + 4)
			rc_check (line_buffer, _("`Eternal holiday list'"),
				  (long) tindex, i, &rc_elems, day, ed, wd);
		    }
		  free (hd_table[tindex]);
		  hd_table[tindex] = (char *) NULL;
		  tindex++;
		}
	      is_leap_year = (days_of_february (year) == 29);
	      if (adate_set)
		{
		  fiscal_month = tmp_fiscal_month;
		  act_year = tmp_ay;
		  act_month = tmp_am;
		  jtmp_ad = act_day = tmp_ad;
		}
	      if (cal_special_flag)
		act_day = day_of_year (jtmp_ad, act_month, act_year);
	      if (holiday_flag
		  && (((year == EASTER_MIN - 1)
		       && (fiscal_month > MONTH_MIN))
		      || ((year >= EASTER_MIN) && (year <= EASTER_MAX))))
		print_all_holidays (TRUE, TRUE);
	      if (cal_special_flag)
		act_day = jtmp_ad;
	      if (adate_set)
		{
		  fiscal_month = MONTH_MIN;
		  act_year = tay;
		  month = act_month = tam;
		  act_day = tad;
		}
	      else
		month = tmp_month;
	    }
	}
      /*
         Try to open the resource file(s).
       */
      if (rc_filename == (char *) NULL)
	{
	  /*
	     Use the standard resource file.
	   */
	  i = (int) strlen (PACKAGE_NAME) + strlen (RC_SUFFIX);
# ifdef GCAL_SHELL
	  i++;
# endif
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
# ifdef GCAL_SHELL
	  strcpy (s3, ".");
# else /* !GCAL_SHELL */
	  *s3 = '\0';
# endif	/* !GCAL_SHELL */
	  strcat (s3, PACKAGE_NAME);
	  strcat (s3, RC_SUFFIX);
	}
      else
	{
	  /*
	     Use the list of resource file names.
	   */
	  i = (int) strlen (rc_filename);
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	  strcpy (s3, rc_filename);
	}
      /*
         Allocate the file buffers for the main resource file itself
         (the main resource file is always buffered at position 0 in `rc_files_table[]').
       */
      rc_files_table[rc_files] =
	(File_struct *) my_malloc (sizeof (File_struct),
				   ERR_NO_MEMORY_AVAILABLE, __FILE__,
				   ((long) __LINE__) - 2L,
				   "rc_files_table[rc_files]", rc_files);
      rc_files_table[rc_files]->fp = (FILE *) NULL;
      rc_files_table[rc_files]->pool = (char *) my_malloc (BUF_LEN + 1,
							   ERR_NO_MEMORY_AVAILABLE,
							   __FILE__,
							   ((long) __LINE__) -
							   2L,
							   "rc_files_table[rc_files]->pool",
							   rc_files);
      if (loop_end != SPECIAL_VALUE)
	{
	  /*
	     Set the cycle-starting time value for %sun* and %moon*.
	   */
	  tindex = HHMM2MM (time_hour_offset, time_min_offset);
	  /*
	     Reduce the given cycle-starting time value in minutes to a single day.
	   */
	  if (tindex < 0)
	    tindex = 0;
	  else if (tindex >= MINS_PER_DAY)
	    tindex = MINS_PER_DAY - 1;
	  if (tindex > loop_end)
	    cycle_increment = FALSE;
	  time_hour_offset = MM2HH (tindex);
	  time_min_offset = tindex % MINS_PER_HOUR;
	  time_offset = MM2DAY (tindex);
	}
      else
	tindex = 0;
      do
	{
	  tmp_rc_here_fn = rc_here_fn;
	  ok = is_here_file = FALSE;
	  while (!ok)
	    {
	      /*
	         Single file or list of resource file names given in the command line.
	       */
	      while (rc_files_table[rc_files]->fp == (FILE *) NULL)
		{
		  i = 0;
		  ptr_char = s3;
		  LOOP
		  {
		    if (*ptr_char == QUOTE_CHAR)
		      {
			if (*(ptr_char + 1) == *CONNECT_SEP
			    || *(ptr_char + 1) == QUOTE_CHAR)
			  ptr_char++;
			s7[i++] = *ptr_char++;
		      }
		    else if (*ptr_char != *CONNECT_SEP)
		      s7[i++] = *ptr_char++;
		    if (!*ptr_char || *ptr_char == *CONNECT_SEP)
		      break;
		  }
		  s7[i] = '\0';
		  /*
		     Now check if the file exists.
		   */
		  rc_files_table[rc_files]->filename
		    = (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE,
					  __FILE__, ((long) __LINE__) - 1L,
					  "rc_files_table[rc_files]->filename",
					  rc_files);
		  strcpy (rc_files_table[rc_files]->filename, s7);
		  rc_files_table[rc_files]->fp =
		    file_open (&rc_files_table[rc_files]->filename, rc_files,
			       (is_here_file) ? HEre : REsource,
			       &bad_sys_include);
		  if (!*ptr_char)
		    {
		      /*
		         Finished, THE file respectively ALL files are managed
		         so check whether any `--here=ARG' options must be processed
		         at last coming from the temporary file already created.
		       */
		      if (tmp_rc_here_fn != (char *) NULL)
			{
			  /*
			     Use the temporary "here" filename for processing next.
			   */
			  i = (int) strlen (tmp_rc_here_fn);
			  if ((Uint) i >= maxlen_max)
			    resize_all_strings (i + 1, FALSE, __FILE__,
						(long) __LINE__);
			  strcpy (s3, tmp_rc_here_fn);
			  tmp_rc_here_fn = (char *) NULL;
			  is_here_file = !ok;	/* Nomen est Omen??? :-) */
			}
		      else
			ok = TRUE;
		      break;
		    }
		  else
		    {
		      /*
		         Now skip a trailing '+' character of a file name list.
		       */
		      ptr_char++;
		      /*
		         Copy the rest of the file name list and start
		         the file search again if the file was not found.
		       */
		      strcpy (s7, ptr_char);
		      strcpy (s3, s7);
		      if (rc_files_table[rc_files]->fp == (FILE *) NULL)
			free (rc_files_table[rc_files]->filename);
		    }
		}
	      /*
	         Now read and check contents of a resource file `filename'
	         and include valid fixed dates into `rc_elems_table[]'.
	       */
	      if (rc_files_table[rc_files]->fp != (FILE *) NULL)
		{
		  rc_files_table[rc_files]->in_pool = 0;
		  rc_files_table[rc_files]->line_number = 0L;
		  /*
		     First of all, copy the contents of the global date variables to the
		     local variables (if one of these isn't defined) so we can perform
		     local operations (++, --, +=, -=) on global variables.
		   */
		  for (i = 0; i < RC_DVAR_MAX; i++)
		    if (rc_dvar[i].dvar_global.dvar_month
			&& !rc_dvar[i].dvar_local.dvar_month)
		      {
			rc_dvar[i].dvar_local.dvar_month =
			  rc_dvar[i].dvar_global.dvar_month;
			rc_dvar[i].dvar_local.dvar_day =
			  rc_dvar[i].dvar_global.dvar_day;
		      }
		  /*
		     Then copy the contents of the global text variables to the
		     local variables (if one of these isn't defined) so we can
		     perform local operations (++, --, +=, -=) on global variables.
		   */
		  for (i = 0; i < RC_TVAR_MAX; i++)
		    if ((rc_tvar[i].tvar_global.tvar_text != (char *) NULL)
			&& (rc_tvar[i].tvar_local.tvar_text == (char *) NULL))
		      {
			rc_tvar[i].tvar_local.tvar_text
			  =
			  (char *)
			  my_malloc (strlen (rc_tvar[i].tvar_global.tvar_text)
				     + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				     ((long) __LINE__) - 2L,
				     "rc_tvar[i].tvar_local.tvar_text", i);
			strcpy (rc_tvar[i].tvar_local.tvar_text,
				rc_tvar[i].tvar_global.tvar_text);
		      }
		  while ((rc_files_table[rc_files]->ptr_pool =
			  file_read_line (rc_files_table[rc_files]->fp,
					  &line_buffer,
					  &rc_files_table[rc_files]->in_pool,
					  rc_files_table[rc_files]->pool,
					  rc_files_table[rc_files]->ptr_pool,
					  rc_files_table[rc_files]->filename,
					  &rc_files_table[rc_files]->
					  line_number, &line_length, REsource,
					  &is_include, &is_dvar,
					  &is_tvar)) != (char *) NULL)
		    {
		      /*
		         Check whether an "#include" statement is found.
		       */
		      if (is_include)
			/*
			   We have to manage an include file.
			 */
			try_to_include_file (ed, wd);
		      else
			/*
			   We are still in the main resource file.
			 */
		      if (*line_buffer && !is_dvar && !is_tvar)
			rc_check (line_buffer,
				  rc_files_table[rc_files]->filename,
				  rc_files_table[rc_files]->line_number,
				  line_length, &rc_elems, day, ed, wd);
		    }
		  if (rc_files_table[rc_files]->fp != stdin)
		    (void) fclose (rc_files_table[rc_files]->fp);
		  rc_files_table[rc_files]->fp = (FILE *) NULL;
		  free (rc_files_table[rc_files]->filename);
		  /*
		     Next file -> reset all local date variables to zero,
		     if `--export-date-variables' flag set, don't reset them!
		   */
		  if (!rc_export_ldvar_flag)
		    for (i = 0; i < RC_DVAR_MAX; i++)
		      rc_dvar[i].dvar_local.dvar_month = (char) 0;
		  /*
		     Next file -> reset all local text variables to NULL,
		     if `--export-text-variables' flag set, don't reset them!
		   */
		  if (!rc_export_ltvar_flag)
		    for (i = 0; i < RC_TVAR_MAX; i++)
		      if (rc_tvar[i].tvar_local.tvar_text != (char *) NULL)
			{
			  free (rc_tvar[i].tvar_local.tvar_text);
			  rc_tvar[i].tvar_local.tvar_text = (char *) NULL;
			}
		}
	      else
		/*
		   Yeah, we have not found any main resource file so it's absolutely
		   necessary to free the allocated memory area of the "file name",
		   because it's possible that we enter this function again,
		   e.g. if we produce month/year lists or ranges.
		 */
		free (rc_files_table[rc_files]->filename);
	    }
	  if (loop_end != SPECIAL_VALUE)
	    {
	      /*
	         Increase/decrease the cycle-time counter properly and set
	         a ``new'' time value for the %sun* and %moon* special texts.
	       */
	      if (cycle_increment)
		tindex += loop_step;
	      else
		tindex -= loop_step;
	      time_hour_offset = MM2HH (tindex);
	      time_min_offset = tindex % MINS_PER_HOUR;
	      time_offset = MM2DAY (tindex);
	    }
	}
      while ((cycle_increment
	      && (tindex <= loop_end))
	     || (!cycle_increment && (tindex >= loop_end)));
      time_hour_offset = save_hour_offset;
      time_min_offset = save_min_offset;
      time_offset = save_time_offset;
      loop_end = save_loop_end;
      free (rc_files_table[rc_files]->pool);
      for (i = 0; i < RC_TVAR_MAX; i++)
	if (rc_tvar[i].tvar_local.tvar_text != (char *) NULL)
	  {
	    free (rc_tvar[i].tvar_local.tvar_text);
	    rc_tvar[i].tvar_local.tvar_text = (char *) NULL;
	  }
      free (rc_files_table[rc_files]);
      fiscal_month = tmp_fiscal_month;
      /*
         Now display the constructed contents of `rc_elems_table[]'.
       */
      if (rc_elems || rc_all_dates_flag || rc_zero_dates_flag)
	display_table (tmp_ad, tmp_am, tmp_ay, day, ed, wd);
    }
  start_day = tmp_start_day;
  month = tmp_month;
  act_day = tmp_ad;
  act_month = tmp_am;
  act_year = tmp_ay;
}



static void
try_to_include_file (ed, wd)
     int ed;
     int wd;
/*!
   Manages an include file.
*/
{
  register int i;
  register int j;
  auto Bool is_usr_file = FALSE;
  auto Bool is_sys_file = FALSE;


  i = (int) strlen (line_buffer);
  if ((Uint) i >= maxlen_max)
    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
  strcpy (s7, line_buffer);
  i = (int) strlen (RC_INCL_STMENT);
  if (strncasecmp (s7, RC_INCL_STMENT, i))
    /*
       Error, misspelled "#include" directive found.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files]->filename,
	      rc_files_table[rc_files]->line_number, line_buffer, 0);
  /*
     Skip any leading whitespace characters of the line.
   */
  while (isspace (s7[i]))
    i++;
  if (!s7[i] || s7[i] == '\n')
    /*
       Error, no include file "argument" encoded.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files]->filename,
	      rc_files_table[rc_files]->line_number, line_buffer, 0);
  if (s7[i] == *RC_INCL_USR_ID)
    is_usr_file = TRUE;
  else if (s7[i] == *RC_INCL_SYS_ID)
    is_sys_file = TRUE;
  else
    /*
       Error, illegal leading include file name delimiter found.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files]->filename,
	      rc_files_table[rc_files]->line_number, line_buffer, 0);
  i++;
  j = 0;
  /*
     Get the include file name.
   */
  while (s7[i] && (s7[i] != *RC_INCL_USR_OD) && (s7[i] != *RC_INCL_SYS_OD))
    s7[j++] = s7[i++];
  if ((is_usr_file
       && (s7[i] != *RC_INCL_USR_OD))
      || (is_sys_file && (s7[i] != *RC_INCL_SYS_OD)))
    /*
       Error, illegal trailing include file name delimiter found.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files]->filename,
	      rc_files_table[rc_files]->line_number, line_buffer, 0);
  if (!j)
    /*
       Error, no include file "name" encoded.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files]->filename,
	      rc_files_table[rc_files]->line_number, line_buffer, 0);
  /*
     Now we have the name of include file and the mode of operation.
   */
  s7[j] = '\0';
  /*
     Before we have to manage this next include file,
     let's check for recursive/cyclic includes, which we MUST avoid!
   */
  for (i = 0; i <= rc_files; i++)
    {
      /*
         Check if any file buffered in `rc_files_table[]' is just included by this file.
       */
      if (*s7 == *rc_files_table[i]->filename)
	if (!strcmp (s7 + 1, rc_files_table[i]->filename + 1))
	  /*
	     Error, invalid recursive/cyclic include statement found.
	   */
	  my_error (ERR_CYCLIC_INCLUDE, rc_files_table[rc_files]->filename,
		    rc_files_table[rc_files]->line_number, line_buffer, 0);
    }
  /*
     First or next include file, so we increment the counter
     of buffered files in `rc_files_table[]' by one first before we check
     the table size, because `rc_files' wasn't just postincremented
   */
  rc_files++;
  /*
     Now check if it's necessary to resize the `rc_files_table[]'.
   */
  if ((Uint) rc_files >= rc_files_max)
    {
      /*
         Resize the `rc_files_table[]' table.
       */
      rc_files_max <<= 1;
      if (rc_elems_max * sizeof (File_struct *) > testval)
	rc_files_max--;
      rc_files_table = (File_struct **) my_realloc ((VOID_PTR) rc_files_table,
						    rc_files_max *
						    sizeof (File_struct *),
						    ERR_NO_MEMORY_AVAILABLE,
						    __FILE__,
						    ((long) __LINE__) - 3L,
						    "rc_files_table[rc_files_max]",
						    rc_files_max);
    }
  /*
     Allocate the file buffers for an include file.
   */
  rc_files_table[rc_files] = (File_struct *) my_malloc (sizeof (File_struct),
							ERR_NO_MEMORY_AVAILABLE,
							__FILE__,
							((long) __LINE__) -
							2L,
							"rc_files_table[rc_files]",
							rc_files);
  rc_files_table[rc_files]->filename =
    (char *) my_malloc (j + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
			((long) __LINE__) - 2L,
			"rc_files_table[rc_files]->filename", rc_files);
  strcpy (rc_files_table[rc_files]->filename, s7);
  /*
     Try to open it.
   */
  rc_files_table[rc_files]->fp =
    file_open (&rc_files_table[rc_files]->filename, rc_files,
	       (is_usr_file) ? USr_include : SYs_include, &bad_sys_include);
  if (bad_sys_include)
    /*
       Error, invalid root directory based system include file name found
       in the include statement, e.g.:  #include </foo>   or   #include </foo/bar>.
     */
    my_error (ERR_MALFORMED_INCLUDE, rc_files_table[rc_files - 1]->filename,
	      rc_files_table[rc_files - 1]->line_number, line_buffer, 0);
  /*
     If include file exists, read it.
   */
  if (rc_files_table[rc_files]->fp != (FILE *) NULL)
    {
      rc_files_table[rc_files]->in_pool = 0;
      rc_files_table[rc_files]->line_number = 0L;
      rc_files_table[rc_files]->pool = (char *) my_malloc (BUF_LEN + 1,
							   ERR_NO_MEMORY_AVAILABLE,
							   __FILE__,
							   ((long) __LINE__) -
							   2L,
							   "rc_files_table[rc_files]->pool",
							   rc_files);
      /*
         Buffer all local date variables of the include file.
       */
      for (i = 0; i < RC_DVAR_MAX; i++)
	{
	  rc_files_table[rc_files]->local_dvars[i].dvar_month =
	    rc_dvar[i].dvar_local.dvar_month;
	  rc_files_table[rc_files]->local_dvars[i].dvar_day =
	    rc_dvar[i].dvar_local.dvar_day;
	}
      /*
         Buffer all local text variables of the include file.
       */
      for (i = 0; i < RC_TVAR_MAX; i++)
	{
	  if (rc_tvar[i].tvar_local.tvar_text != (char *) NULL)
	    {
	      rc_files_table[rc_files]->local_tvars[i].tvar_text
		=
		(char *) my_malloc (strlen (rc_tvar[i].tvar_local.tvar_text) +
				    1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				    ((long) __LINE__) - 2L,
				    "rc_tvar[i].tvar_local.tvar_text", i);
	      strcpy (rc_files_table[rc_files]->local_tvars[i].tvar_text,
		      rc_tvar[i].tvar_local.tvar_text);
	    }
	  else
	    rc_files_table[rc_files]->local_tvars[i].tvar_text =
	      (char *) NULL;
	}
      while ((rc_files_table[rc_files]->ptr_pool =
	      file_read_line (rc_files_table[rc_files]->fp, &line_buffer,
			      &rc_files_table[rc_files]->in_pool,
			      rc_files_table[rc_files]->pool,
			      rc_files_table[rc_files]->ptr_pool,
			      rc_files_table[rc_files]->filename,
			      &rc_files_table[rc_files]->line_number,
			      &line_length, REsource, &is_include, &is_dvar,
			      &is_tvar)) != (char *) NULL)
	/*
	   Check whether an "#include" statement is found.
	 */
	if (is_include)
	  /*
	     Ok, let's manage a next include file by calling this function recursively.
	   */
	  try_to_include_file (ed, wd);
	else
	  /*
	     We are still in the actual include file.
	   */
	if (*line_buffer && !is_dvar && !is_tvar)
	  rc_check (line_buffer, rc_files_table[rc_files]->filename,
		    rc_files_table[rc_files]->line_number, line_length,
		    &rc_elems, day, ed, wd);
      (void) fclose (rc_files_table[rc_files]->fp);
      free (rc_files_table[rc_files]->pool);
      /*
         Restore all local date variables of the include file.
       */
      for (i = 0; i < RC_DVAR_MAX; i++)
	{
	  rc_dvar[i].dvar_local.dvar_month =
	    rc_files_table[rc_files]->local_dvars[i].dvar_month;
	  rc_dvar[i].dvar_local.dvar_day =
	    rc_files_table[rc_files]->local_dvars[i].dvar_day;
	}
      /*
         Restore all local text variables of the include file.
       */
      for (i = 0; i < RC_TVAR_MAX; i++)
	if (rc_files_table[rc_files]->local_tvars[i].tvar_text !=
	    (char *) NULL)
	  {
	    if (rc_tvar[i].tvar_local.tvar_text == (char *) NULL)
	      rc_tvar[i].tvar_local.tvar_text
		=
		(char *)
		my_malloc (strlen
			   (rc_files_table[rc_files]->local_tvars[i].
			    tvar_text) + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
			   ((long) __LINE__) - 2L,
			   "rc_tvar[i].tvar_local.tvar_text", i);
	    else
	      rc_tvar[i].tvar_local.tvar_text
		=
		(char *)
		my_realloc ((VOID_PTR) (rc_tvar[i].tvar_local.tvar_text),
			    strlen (rc_files_table[rc_files]->local_tvars[i].
				    tvar_text) + 1, ERR_NO_MEMORY_AVAILABLE,
			    __FILE__, ((long) __LINE__) - 3L,
			    "rc_tvar[i].tvar_local.tvar_text", i);
	    strcpy (rc_tvar[i].tvar_local.tvar_text,
		    rc_files_table[rc_files]->local_tvars[i].tvar_text);
	    free (rc_files_table[rc_files]->local_tvars[i].tvar_text);
	    rc_files_table[rc_files]->local_tvars[i].tvar_text =
	      (char *) NULL;
	  }
    }
  /*
     Now deallocate the include file buffers.
   */
  free (rc_files_table[rc_files]->filename);
  free (rc_files_table[rc_files]);
  /*
     And back to the previous include file resp., main resource file.
   */
  rc_files--;
}



static void
display_table (tmp_ad, tmp_am, tmp_ay, day, ed, wd)
     const int tmp_ad;
     const int tmp_am;
     const int tmp_ay;
     int day;
     int ed;
     int wd;
/*!
   Displays all entries in `rc_elems_table[]' in sorted order.
*/
{
  register int n;
  register int i;
  register int j = 0;
  register int tindex = 0;
  auto int ld = 0;
  auto int lm = 0;
  auto int ly = 0;
  auto int dd = 0;
  auto int mm = 0;
  auto int yy = 0;
  auto int i_dummy;
  auto char c_dummy;
  auto Bool b_dummy;
  auto Bool ok;


  rc_zero_pos = rc_elems;
  /*
     If `rc_all_dates_flag' or `rc_zero_dates_flag' is set, complete the
     contents of `rc_elems_table[]', i.e., generate "empty" texts for
     each day in the requested period.
   */
  if (rc_all_dates_flag || rc_zero_dates_flag)
    {
      register int rc_e = rc_zero_pos;


      /*
         Lets sort ascending first only in case `rc_elems_table[]' isn't presorted!
       */
      if ((rc_elems > 1) && !is_presorted (rc_elems_table, rc_elems))
	qsort ((VOID_PTR) rc_elems_table, rc_elems, sizeof *rc_elems_table,
	       (Cmp_func) asc_sort);
      if (rc_elems)
	(void) rc_get_date (rc_elems_table[j], lineptrs, FALSE, &b_dummy, &ld,
			    &lm, &ly, &i_dummy, &i_dummy, &c_dummy, &i_dummy,
			    &i_dummy, _("Internal"), (long) j,
			    rc_elems_table[j], TRUE);
      /*
         Correction for the respected period if we are in fiscal year mode.
       */
      if ((fiscal_month > MONTH_MIN) && !adate_set)
	{
	  day = mvec[fiscal_month - 1] + 1;
	  if (fiscal_month > 2)
	    day += is_leap_year;
	}
      /*
         Let's produce the dummy entries ("empty" fixed dates)...
       */
      for (i = day; i < ed; i++)
	{
	  yy = year;
	  ok = doy2date (i, is_leap_year, &dd, &mm);
	  if (!ok)
	    {
	      if (rc_tomorrow_flag || rc_week_flag)
		{
		  if (i < DAY_MIN)
		    {
		      yy = year - 1;
		      if (yy < YEAR_MIN)
			break;
		      n = (days_of_february (yy) == 29);
		      (void) doy2date (DAY_LAST + n + i, n, &dd, &mm);
		    }
		  else if (i > DAY_LAST + is_leap_year)
		    {
		      yy = year + 1;
		      if (yy > YEAR_MAX)
			break;
		      n = (days_of_february (yy) == 29);
		      (void) doy2date (i - (DAY_LAST + is_leap_year), n, &dd,
				       &mm);
		    }
		}
	      else
		break;
	    }
	  if (j == rc_e
	      || mm < lm
	      || yy < ly || ((dd < ld) && (mm <= lm) && (yy <= ly)))
	    {
	      sprintf (line_buffer, "%0*d%02d%02d ", len_year_max, yy, mm,
		       dd);
	      rc_check (line_buffer, _("`Internal'"), (long) tindex,
			strlen (line_buffer), &rc_elems, day, ed, wd);
	    }
	  else
	    while ((dd == ld) && (mm == lm) && (yy == ly))
	      if (j + 1 < rc_e)
		{
		  j++;
		  (void) rc_get_date (rc_elems_table[j], lineptrs, FALSE,
				      &b_dummy, &ld, &lm, &ly, &i_dummy,
				      &i_dummy, &c_dummy, &i_dummy, &i_dummy,
				      _("Internal"), (long) j,
				      rc_elems_table[j], TRUE);
		}
	      else
		{
		  j++;
		  break;
		}
	}
      /*
         Respect fiscal year mode.
       */
      if ((fiscal_month > MONTH_MIN) && (year < YEAR_MAX) && !adate_set)
	{
	  /*
	     Correction for the respected period.
	   */
	  day = DAY_MIN;
	  yy = year + 1;
	  is_leap_year = (days_of_february (yy) == 29);
	  if (fiscal_month < MONTH_MAX)
	    ed = day_of_year (DAY_MIN, fiscal_month + 1, yy) + 1;
	  else
	    ed = DAY_LAST + is_leap_year + 1;
	  /*
	     Let's produce the dummy entries ("empty" fixed dates)...
	   */
	  for (i = day; i < ed; i++)
	    {
	      ok = doy2date (i, is_leap_year, &dd, &mm);
	      if (!ok)
		break;
	      if (j == rc_e
		  || mm < lm
		  || yy < ly || ((dd < ld) && (mm <= lm) && (yy <= ly)))
		{
		  sprintf (line_buffer, "%0*d%02d%02d ", len_year_max, yy, mm,
			   dd);
		  rc_check (line_buffer, _("`Internal'"), (long) tindex,
			    strlen (line_buffer), &rc_elems, day, ed, wd);
		}
	      else
		while ((dd == ld) && (mm == lm) && (yy == ly))
		  if (j + 1 < rc_e)
		    {
		      j++;
		      (void) rc_get_date (rc_elems_table[j], lineptrs, FALSE,
					  &b_dummy, &ld, &lm, &ly, &i_dummy,
					  &i_dummy, &c_dummy, &i_dummy,
					  &i_dummy, _("Internal"), (long) j,
					  rc_elems_table[j], TRUE);
		    }
		  else
		    {
		      j++;
		      break;
		    }
	    }
	}
      is_leap_year = (days_of_february (year) == 29);
    }
  if ((rc_zero_dates_flag
       && (rc_elems - rc_zero_pos)) || (!rc_zero_dates_flag && rc_elems))
    {
      register int tstart;
      register int tend;
      register int skipped;
      register int len_rce = 0;
      register int len_line;
      register int len_fn_part;
      register int len_text_part;
      register int hls_len = 0;
      auto char *ptr_char;
      auto Bool same_date = FALSE;
      auto Bool ok2;


      /*
         At last, sort the fixed dates only if they are either not presorted
         or contain no resource file name reference, and not ONLY "empty"
         fixed dates shall be displayed.
       */
      if (!rc_zero_dates_flag && (rc_elems > 1))
	{
	  if (rc_enable_fn_flag)
	    {
	      if (rc_sort_des_flag)
		qsort ((VOID_PTR) rc_elems_table, rc_elems,
		       sizeof *rc_elems_table, (Cmp_func) fn_des_sort);
	      else
		qsort ((VOID_PTR) rc_elems_table, rc_elems,
		       sizeof *rc_elems_table, (Cmp_func) fn_asc_sort);
	    }
	  else if (!is_presorted (rc_elems_table, rc_elems))
	    {
	      if (rc_sort_des_flag)
		qsort ((VOID_PTR) rc_elems_table, rc_elems,
		       sizeof *rc_elems_table, (Cmp_func) des_sort);
	      else
		qsort ((VOID_PTR) rc_elems_table, rc_elems,
		       sizeof *rc_elems_table, (Cmp_func) asc_sort);
	    }
	  else if (rc_sort_des_flag)
	    /*
	       `rc_elems_table[]' is presorted and must be shown in
	       descending sort order, rearrange its internal sort
	       order from ascending to descending sort order.
	     */
	    reverse_order (rc_elems_table, rc_elems);
	}
      d = tmp_ad;
      m = tmp_am;
      /*
         Copy `rc_grp_sep' [-c]g[] to text buffer variable `s3' for further use,
         which will be first evaluated for text variable references and
         some special texts.  Thereafter, perform the '~'-TILDE or '^'-CARET
         expansion on the one hand.
       */
      if (rc_grp_sep != (char *) NULL)
	{
	  register int x1 = year;
	  register int x2 = incr_year;
	  register int x3 = decr_year;


	  year = act_year;
	  incr_year = decr_year = 0;
	  insert_line_into_table (rc_grp_sep, _("`Internal'"),
				  (long) SPECIAL_VALUE, &rc_elems, 1, 1);
	  decr_year = x3;
	  incr_year = x2;
	  year = x1;
	  i = (int) strlen (rc_elems_table[--rc_elems]);
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	  strcpy (s3, rc_elems_table[rc_elems] + len_year_max + 5);
	  free (rc_elems_table[rc_elems]);
	  ptr_char = s3;
	  if (*ptr_char)
	    {
	      /*
	         A text for grouping is defined.
	       */
	      i = 0;
	      while (*ptr_char)
		{
		  switch (*ptr_char)
		    {
		    case RC_NL_CHAR:
		    case RC_NL2_CHAR:
		      if (i)
			{
			  /*
			     RC_NL[2]_CHAR is single `\~' or `\^' quoted
			     or double `\\~' or `\\^' quoted:
			     Replace QUOTE_CHAR by RC_NL[2]_CHAR resp.,
			     replace last QUOTE_CHAR by RC_NL_CHAR.
			   */
			  if (s3[i - 1] == QUOTE_CHAR)
			    s3[i - 1] = *ptr_char;
			  else
			    /*
			       RC_NL[2]_CHAR is not quoted '~' or '^':
			       Insert a real `\n'-NEWLINE character.
			     */
			    s3[i++] = '\n';
			}
		      else
			/*
			   RC_NL[2]_CHAR is not quoted '~' or '^':
			   Insert a real `\n'-NEWLINE character.
			 */
			s3[i++] = '\n';
		      break;
		    case QUOTE_CHAR:
		      ptr_char++;
		      if (*ptr_char)
			{
			  if (*ptr_char == RC_NL_CHAR
			      || *ptr_char == RC_NL2_CHAR)
			    s3[i++] = *ptr_char;
			  else
			    {
			      s3[i++] = QUOTE_CHAR;
			      s3[i++] = *ptr_char;
			    }
			}
		      else
			s3[i++] = QUOTE_CHAR;
		      break;
		    default:
		      s3[i++] = *ptr_char;
		    }
		  ptr_char++;
		}
	      s3[i] = '\0';
	    }
	}
      (*s1) = (*s6) = '\0';
      /*
         Now display a leading NEWLINE character before the text/title.
       */
      if (!rc_suppr_list_sep_flag)
	print_text (stdout, s1);
      /*
         Now display the leading title/heading text of the fixed date list,
         which will be first evaluated for text variable references and
         some special texts.  Thereafter, perform the '~'-TILDE or '^'-CARET
         expansion on the one hand.
       */
      if (rc_title_flag)
	{
	  register int x1 = year;
	  register int x2 = incr_year;
	  register int x3 = decr_year;


	  year = act_year;
	  incr_year = decr_year = 0;
	  insert_line_into_table (rc_heading_text, _("`Internal'"),
				  (long) SPECIAL_VALUE, &rc_elems, 1, 1);
	  decr_year = x3;
	  incr_year = x2;
	  year = x1;
	  i = (int) strlen (rc_elems_table[--rc_elems]);
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	  strcpy (s1, rc_elems_table[rc_elems] + len_year_max + 5);
	  free (rc_elems_table[rc_elems]);
	  ptr_char = s1;
	  i = 0;
	  while (*ptr_char)
	    {
	      switch (*ptr_char)
		{
		case RC_NL_CHAR:
		case RC_NL2_CHAR:
		  if (i)
		    {
		      /*
		         RC_NL[2]_CHAR is single `\~' or `\^' quoted
		         or double `\\~' `\\^' quoted:
		         Replace QUOTE_CHAR by RC_NL[2]_CHAR resp.,
		         replace last QUOTE_CHAR by RC_NL[2]_CHAR.
		       */
		      if (s1[i - 1] == QUOTE_CHAR)
			s1[i - 1] = *ptr_char;
		      else
			/*
			   RC_NL[2]_CHAR is not quoted '~' or '^':
			   Insert a real `\n'-NEWLINE character.
			 */
			s1[i++] = '\n';
		    }
		  else
		    /*
		       RC_NL[2]_CHAR is not quoted '~' or '^':
		       Insert a real `\n'-NEWLINE character.
		     */
		    s1[i++] = '\n';
		  break;
		case QUOTE_CHAR:
		  ptr_char++;
		  if (*ptr_char)
		    {
		      if (*ptr_char == RC_NL_CHAR || *ptr_char == RC_NL2_CHAR)
			s1[i++] = *ptr_char;
		      else
			{
			  s1[i++] = QUOTE_CHAR;
			  s1[i++] = *ptr_char;
			}
		    }
		  else
		    s1[i++] = QUOTE_CHAR;
		  break;
		default:
		  s1[i++] = *ptr_char;
		}
	      ptr_char++;
	    }
	  s1[i] = '\0';
	  print_text (stdout, s1);
	  print_text (stdout, s1);
	}
      /*
         Detect the number of digits of `rc_elems'.
       */
      sprintf (s2, "%d",
	       (rc_zero_dates_flag) ? rc_elems - rc_zero_pos : rc_elems);
      len_rce = (int) strlen (s2);
      /*
         Initialize the variables which control the loop.
       */
      skipped = tstart = 0;
      tend = rc_elems - 1;
      if (rc_zero_dates_flag)
	{
	  if (rc_sort_des_flag)
	    {
	      tstart = rc_elems - 1;
	      tend = rc_zero_pos;
	      skipped = rc_elems + 1;
	    }
	  else
	    skipped = tstart = rc_zero_pos;
	}
      /*
         Now initialize the loop counter.
       */
      tindex = tstart;
      /*
         And display all fixed dates of `rc_elems_table[]'.
       */
      LOOP
      {
	lineptrs =
	  rc_get_date (rc_elems_table[tindex], lineptrs, FALSE, &b_dummy,
		       &day, &lm, &ly, &i_dummy, &i_dummy, &c_dummy, &i_dummy,
		       &i_dummy, _("Internal"), (long) tindex,
		       rc_elems_table[tindex], TRUE);
	if ((tindex == tstart)
	    && (rc_omit_date_flag
		|| rc_alternative_format_flag || rc_grp_sep != (char *) NULL))
	  {
	    dd = day;
	    mm = lm;
	    yy = ly;
	  }
	/*
	   Avoid displaying duplicate resource file entries by storing
	   the actual entry into `s6' and comparing it with the previous
	   entry in line.
	 */
	if (tindex == tstart
	    || ((tindex != tstart) && strcmp (s6, rc_elems_table[tindex])))
	  {
	    strcpy (s6, rc_elems_table[tindex]);
	    /*
	       Now display fixed date list group separator resp.,
	       detect if date of text differs from previous text.
	     */
	    if (rc_omit_date_flag
		|| rc_alternative_format_flag || rc_grp_sep != (char *) NULL)
	      {
		if ((day == dd) && (lm == mm) && (ly == yy))
		  {
		    /*
		       Same date:
		       Avoid displaying of group separator.
		     */
		    if (tindex != tstart)
		      same_date = TRUE;
		  }
		else
		  {
		    /*
		       Date differs.
		     */
		    same_date = FALSE;
		    dd = day;
		    mm = lm;
		    yy = ly;
		    if (rc_grp_sep != (char *) NULL)
		      {
			/*
			   Display constructed group separator text `-cg[TEXT]'.
			 */
			strcpy (s2, s3);
			print_text (stdout, s2);
		      }
		  }
	      }
	    /*
	       Construct the leading (partitially highlighted) "date"-part of the fixed date.
	     */
	    if (rc_week_number_flag)
	      {
		j = week_number (day, lm, ly, iso_week_number, start_day);
		/*
		   We convert the computed week number to a week number text
		   (this looks nicer in output).
		 */
		if (j < 0)
		  /*
		     Week starts in previous year and the first days
		     of the actual year are not in its first week.
		   */
		  sprintf (s7, "|%02d/0|", -j);
		else if (!j)
		  /*
		     Week starts in previous year and the first days
		     of the actual year are in its first week.
		   */
		  sprintf (s7, "|%s|", "53/1");
		else
		  /*
		     Week starts in actual year.
		   */
		  sprintf (s7, "|%02d|", j);
		if (!rc_alternative_format_flag)
		  {
		    if (strlen (s7) > 4)
		      {
			if (rc_count_flag)
			  sprintf (s1, "%0*d)  %s ", len_rce,
				   abs ((tindex - skipped) + 1), s7);
			else
			  sprintf (s1, "%s ", s7);
		      }
		    else if (rc_count_flag)
		      sprintf (s1, "%0*d)  %s%*s ", len_rce,
			       abs ((tindex - skipped) + 1), s7, len_fil_wt,
			       "");
		    else
		      sprintf (s1, "%s%*s ", s7, len_fil_wt, "");
		  }
	      }
	    else if (!rc_alternative_format_flag && rc_count_flag)
	      sprintf (s1, "%0*d)  ", len_rce, abs ((tindex - skipped) + 1));
	    wd = day;
	    ld = day_of_year (day, lm, ly);
	    if (!rc_both_dates_flag)
	      {
		if (!rc_special_flag)
		  ld = 0;
		else
		  day = 0;
	      }
	    if (!rc_suppr_date_part_flag)
	      {
		if ((ly == tmp_ay) && (lm == tmp_am) && (wd == tmp_ad))
		  hls_len =
		    decode_date_format (date_format->df_format, &s1, day, lm,
					ly, ld, TRUE, FALSE,
					!rc_alternative_format_flag);
		else
		  {
		    if (hd_ldays[((lm - 1) * MONTH_LAST) + (wd - 1)])
		      hls_len =
			decode_date_format (date_format->df_format, &s1, day,
					    lm, ly, ld, FALSE, TRUE,
					    !rc_alternative_format_flag);
		    else
		      hls_len =
			decode_date_format (date_format->df_format, &s1, day,
					    lm, ly, ld, FALSE, FALSE,
					    !rc_alternative_format_flag);
		  }
		if (rc_alternative_format_flag && rc_week_number_flag)
		  {
		    sprintf (s2, _(" ; Week %s"), s7);
		  }
		else
		  *s2 = '\0';
	      }
	    else
	      {
		if (rc_alternative_format_flag && rc_week_number_flag)
		  {
		    sprintf (s2, _("Week %s"), s7);
		  }
		else
		  *s2 = '\0';
	      }
	    if (*s2 || !rc_suppr_date_part_flag)
	      {
		strcat (s2, ":");
		strcat (s1, s2);
	      }
	    j = (int) strlen (s1) - hls_len;
	    /*
	       Skip the separating whitespace character between
	       the "date"-part and the "text"-part.
	     */
	    if (*lineptrs->text_part == ' ')
	      lineptrs->text_part++;
	    /*
	       Construct the "text"-part of the fixed date.
	     */
	    len_text_part = (int) strlen (lineptrs->text_part);
	    if (rc_alternative_format_flag)
	      {
		if (!same_date && *s1)
		  /*
		     Print the constructed date on a separate line
		     only if it appears the first time!
		   */
		  print_text (stdout, s1);
		if (rc_count_flag)
		  {
		    sprintf (s1, "%0*d)", len_rce,
			     abs ((tindex - skipped) + 1));
		    if (len_text_part)
		      strcat (s1, "  ");
		  }
		else
		  *s1 = '\0';
		j = len_line = (int) strlen (s1);
	      }
	    else
	      len_line = j + hls_len + (int) !rc_suppr_date_part_flag;
	    /*
	       Now check if it's necessary to modify the "text"-part of the fixed date.
	     */
	    ok = FALSE;
	    len_fn_part = 0;
	    /*
	       "Empty" texts have no trailing blank character!
	     */
	    if (len_text_part)
	      {
		if (!rc_alternative_format_flag && !rc_suppr_date_part_flag)
		  strcat (s1, " ");
		/*
		   Get the textual length of the file name,
		   which has been inserted into the line.
		 */
		if (rc_enable_fn_flag)
		  {
		    auto Bool pseudo_quote_found = FALSE;


		    /*
		       Add the length of the "file name"+3 (+3 because the file
		       name itself is leaded by a blank character and is
		       enclosed in PSEUDO_QUOTE characters to the number of
		       leading blanks, which must be displayed if the line
		       wrapped by a given `~'-TILDE.
		     */
		    ptr_char = lineptrs->text_part;
		    while (*ptr_char)
		      {
			len_fn_part++;
			if (*ptr_char == PSEUDO_QUOTE)
			  {
			    if (!pseudo_quote_found)
			      pseudo_quote_found = TRUE;
			    else
			      break;
			  }
			ptr_char++;
		      }
		    if (!*ptr_char)
		      /*
		         Internal error, a maintainer has modified the internal
		         format of a line and forgots to respect this
		         modification here!
		       */
		      my_error (ERR_MAINTAINER_FAILURE, __FILE__,
				(long) __LINE__, "", 0);
		    c_dummy = *++ptr_char;
		    if (*ptr_char)
		      {
			len_line++;
			ptr_char++;
			c_dummy = *ptr_char;
			*ptr_char = '\0';
		      }
		    strcat (s1, lineptrs->text_part);
		    i = len_fn_part;
		    len_line += i;
		    if (c_dummy)
		      {
			*ptr_char = c_dummy;
			i++;
			j += (int) !rc_alternative_format_flag;
		      }
		    len_text_part -= i;
		    lineptrs->text_part += i;
		    /*
		       Now exchange the first two PSEUDO_QUOTE characters
		       -- which embrace the "file name" -- by parentheses.
		     */
		    ptr_char = strchr (s1, PSEUDO_QUOTE);
		    if (ptr_char == (char *) NULL)
		      /*
		         Internal error, a maintainer has modified the internal
		         format of a line and forgots to respect this
		         modification here!
		       */
		      my_error (ERR_MAINTAINER_FAILURE, __FILE__,
				(long) __LINE__, "", 0);
		    *ptr_char = '(';
		    ptr_char = strchr (s1, PSEUDO_QUOTE);
		    if (ptr_char == (char *) NULL)
		      /*
		         Internal error, a maintainer has modified the internal
		         format of a line and forgots to respect this
		         modification here!
		       */
		      my_error (ERR_MAINTAINER_FAILURE, __FILE__,
				(long) __LINE__, "", 0);
		    *ptr_char = ')';
		    /*
		       Check if the FIRST character of the REAL "text"-part was a quoted
		       whitespace character (this is indicated by a PSEUDO_QUOTE
		       character ('\001') followed by a "whitespace" character,
		       see `rc-check.c').  If this case is TRUE, skip the PSEUDO_QUOTE.
		     */
		    if (*lineptrs->text_part)
		      if ((*lineptrs->text_part == PSEUDO_QUOTE)
			  && isspace (lineptrs->text_part[1]))
			{
			  lineptrs->text_part++;
			  len_text_part--;
			}
		  }
		else
		  {
		    /*
		       Check if the FIRST character of the "text"-part was a quoted
		       whitespace character (this is indicated by a PSEUDO_QUOTE
		       character ('\001') followed by a "whitespace" character,
		       see `rc-check.c').  If this case is TRUE, skip the PSEUDO_QUOTE.
		     */
		    if ((*lineptrs->text_part == PSEUDO_QUOTE)
			&& isspace (lineptrs->text_part[1]))
		      {
			lineptrs->text_part++;
			len_text_part--;
		      }
		  }
		ok =
		  (Bool) (strchr (lineptrs->text_part, RC_NL_CHAR) ==
			  (char *) NULL);
		ok2 =
		  (Bool) (strchr (lineptrs->text_part, RC_NL2_CHAR) ==
			  (char *) NULL);
		if (ok && ok2)
		  {
		    if (*lineptrs->text_part)
		      {
			if ((Uint) len_line + len_text_part >= maxlen_max)
			  resize_all_strings (len_line + len_text_part + 1,
					      FALSE, __FILE__,
					      (long) __LINE__);
			strcat (s1, lineptrs->text_part);
		      }
		  }
		else
		  {
		    /*
		       `~'-TILDE, `^'-CARET or quoted `\~'-TILDE resp., CARET found
		       in "text"-part of fixed date:
		       Quote TILDE/CARET resp., insert a real `\n'-NEWLINE character
		     */
		    while (*lineptrs->text_part)
		      {
			if ((Uint) len_line >= maxlen_max)
			  resize_all_strings (maxlen_max << 1, FALSE,
					      __FILE__, (long) __LINE__);
			if (*lineptrs->text_part == RC_NL_CHAR)
			  {
			    /*
			       RC_NL_CHAR is single `\~' quoted
			       or double `\\~' quoted:
			       Replace QUOTE_CHAR by RC_NL_CHAR resp.,
			       replace last QUOTE_CHAR by RC_NL_CHAR.
			     */
			    if (s1[len_line - 1] == QUOTE_CHAR)
			      s1[len_line - 1] = *lineptrs->text_part;
			    else
			      {
				/*
				   RC_NL_CHAR is not quoted '~':
				   Insert a real `\n'_NEWLINE character.
				 */
				s1[len_line++] = '\n';
				/*
				   Insert some leading blanks.
				 */
				if (rc_alternative_format_flag
				    && !rc_enable_fn_flag)
				  n = j;
				else
				  {
				    n = j + len_fn_part + 1;
				    if (!rc_alternative_format_flag)
				      n -= (int) rc_suppr_date_part_flag;
				  }
				for (i = 0; i < n; i++)
				  {
				    if ((Uint) len_line >= maxlen_max)
				      resize_all_strings (maxlen_max << 1,
							  FALSE, __FILE__,
							  (long) __LINE__);
				    s1[len_line++] = ' ';
				  }
			      }
			  }
			else if (*lineptrs->text_part == RC_NL2_CHAR)
			  {
			    /*
			       RC_NL2_CHAR is single `\^' quoted
			       or double `\\^' quoted:
			       Replace QUOTE_CHAR by RC_NL2_CHAR resp.,
			       replace last QUOTE_CHAR by RC_NL2_CHAR.
			     */
			    if (s1[len_line - 1] == QUOTE_CHAR)
			      s1[len_line - 1] = *lineptrs->text_part;
			    else
			      /*
			         RC_NL2_CHAR is not quoted '^':
			         Insert a real `\n'_NEWLINE character only!
			       */
			      s1[len_line++] = '\n';
			  }
			else
			  s1[len_line++] = *lineptrs->text_part;
			lineptrs->text_part++;
		      }
		    s1[len_line] = '\0';
		  }
	      }
	    if (rc_omit_date_flag && !rc_alternative_format_flag && same_date)
	      {
		/*
		   Overwrite leading "date"-part of output with blanks.
		 */
		if (rc_count_flag)
		  i = len_rce + 3;	/* +3 because of the ")  " text */
		else
		  i = 0;
		n = j - (int) rc_suppr_date_part_flag;
		for (; i < n; i++)
		  s1[i] = ' ';
		ptr_char = s1 + j + hls_len;
		for (i = j; *ptr_char; i++)
		  s1[i] = *ptr_char++;
		s1[i] = '\0';
	      }
	    /*
	       Now we display the constructed fixed date text.
	     */
	    if (!rc_alternative_format_flag || len_text_part || len_line)
	      print_text (stdout, s1);
	  }
	else
	  skipped++;
	/*
	   Now deallocate the just displayed/skipped fixed date in the table.
	 */
	free (rc_elems_table[tindex]);
	/*
	   Check if all fixed date entries are displayed.
	 */
	if (tindex == tend)
	  break;
	/*
	   If not, increase/decrease the loop counter.
	 */
	if (rc_zero_dates_flag && rc_sort_des_flag)
	  tindex--;
	else
	  tindex++;
      }
    }
  /*
     Now deallocate all NON-"empty" fixed dates in the table
     in case only "empty" fixed dates were displayed.
   */
  if (rc_zero_dates_flag && (rc_elems - rc_zero_pos))
    for (tindex = 0; tindex < rc_zero_pos; tindex++)
      free (rc_elems_table[tindex]);
}



static int
fn_asc_sort (a, b)
     const char **a;
     const char **b;
/*!
   The (q)sort compare function for fixed dates which texts
     have an included resource "file name"; ascending order.
*/
{
  static Uint previous_len;
  static int i;
  static char *a_text;
  static char *b_text;
  static char *ptr_char;
  static Bool is_initialized = FALSE;


  /*
     Don't sort on the included resource "file name",
     sort on the REAL text of the fixed date  O N L Y  !!!
   */
  if (!is_initialized)
    {
      a_text = (char *) my_malloc (maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				   __FILE__, ((long) __LINE__) - 1L,
				   "a_text", 0);
      b_text = (char *) my_malloc (maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				   __FILE__, ((long) __LINE__) - 1L,
				   "b_text", 0);
      previous_len = maxlen_max;
      is_initialized = TRUE;
    }
  else if (previous_len < maxlen_max)
    {
      a_text = (char *) my_realloc ((VOID_PTR) a_text,
				    maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				    __FILE__, ((long) __LINE__) - 2L,
				    "a_text", maxlen_max);
      b_text = (char *) my_realloc ((VOID_PTR) b_text,
				    maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				    __FILE__, ((long) __LINE__) - 2L,
				    "b_text", maxlen_max);
      previous_len = maxlen_max;
    }
  i = len_year_max + 4;
  /*
     Copy the "date"-part AS IS.
   */
  strncpy (a_text, *a, i);
  a_text[i] = '\0';
  /*
     Skip the filename part.
   */
  if (*(*a + i + 1) != PSEUDO_QUOTE)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char = strchr (*a + i + 2, PSEUDO_QUOTE);
  if (ptr_char == (char *) NULL)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char++;
  /*
     And concatenate the "text"-part.
   */
  if (*ptr_char)
    strcat (a_text, ptr_char);
  /*
     Copy the "date"-part AS IS.
   */
  strncpy (b_text, *b, i);
  b_text[i] = '\0';
  /*
     Skip the filename part.
   */
  if (*(*b + i + 1) != PSEUDO_QUOTE)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char = strchr (*b + i + 2, PSEUDO_QUOTE);
  if (ptr_char == (char *) NULL)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char++;
  /*
     And concatenate the "text"-part.
   */
  if (*ptr_char)
    strcat (b_text, ptr_char);

  return (strcmp (a_text, b_text));
}



static int
fn_des_sort (a, b)
     const char **a;
     const char **b;
/*!
   The (q)sort compare function for fixed dates which texts
     have an included resource "file name"; descending order.
*/
{
  static Uint previous_len;
  static int i;
  static char *a_text;
  static char *b_text;
  static char *ptr_char;
  static Bool is_initialized = FALSE;


  /*
     Don't sort on the included resource "file name",
     sort on the REAL text of the fixed date  O N L Y  !!!
   */
  if (!is_initialized)
    {
      a_text = (char *) my_malloc (maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				   __FILE__, ((long) __LINE__) - 1L,
				   "a_text", 0);
      b_text = (char *) my_malloc (maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				   __FILE__, ((long) __LINE__) - 1L,
				   "b_text", 0);
      previous_len = maxlen_max;
      is_initialized = TRUE;
    }
  else if (previous_len < maxlen_max)
    {
      a_text = (char *) my_realloc ((VOID_PTR) a_text,
				    maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				    __FILE__, ((long) __LINE__) - 2L,
				    "a_text", maxlen_max);
      b_text = (char *) my_realloc ((VOID_PTR) b_text,
				    maxlen_max, ERR_NO_MEMORY_AVAILABLE,
				    __FILE__, ((long) __LINE__) - 2L,
				    "b_text", maxlen_max);
      previous_len = maxlen_max;
    }
  i = len_year_max + 4;
  /*
     Copy the "date"-part AS IS.
   */
  strncpy (a_text, *a, i);
  a_text[i] = '\0';
  /*
     Skip the filename part.
   */
  if (*(*a + i + 1) != PSEUDO_QUOTE)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char = strchr (*a + i + 2, PSEUDO_QUOTE);
  if (ptr_char == (char *) NULL)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char++;
  /*
     And concatenate the "text"-part.
   */
  if (*ptr_char)
    strcat (a_text, ptr_char);
  /*
     Copy the "date"-part AS IS.
   */
  strncpy (b_text, *b, i);
  b_text[i] = '\0';
  /*
     Skip the filename part.
   */
  if (*(*b + i + 1) != PSEUDO_QUOTE)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char = strchr (*b + i + 2, PSEUDO_QUOTE);
  if (ptr_char == (char *) NULL)
    /*
       Internal error, a maintainer has modified the internal format
       of a line and forgots to respect this modification here!
     */
    my_error (ERR_MAINTAINER_FAILURE, __FILE__, (long) __LINE__, "", 0);
  ptr_char++;
  /*
     And concatenate the "text"-part.
   */
  if (*ptr_char)
    strcat (b_text, ptr_char);

  return (strcmp (b_text, a_text));
}
#endif /* USE_RC */
