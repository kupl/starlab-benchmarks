/*!
*  \file gcal.c
*  \brief Main part which controls the extended calendar program.
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
*  Include header files.
*/
#include "tailor.h"
#if HAVE_ASSERT_H
# include <assert.h>
#endif
#if HAVE_CTYPE_H
# include <ctype.h>
#endif
#if HAVE_LIMITS_H
# include <limits.h>
#endif
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#if (!HAVE_SIGNAL_H || !HAVE_SIGNAL) && HAVE_SYS_TYPES_H
/* Otherwise "gcal.h" includes <sys/types.h>. */
# include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef GCAL_EPAGER
# include <fcntl.h>
# if !HAVE_DUP
#  define dup(old)        (fcntl(old, F_DUPFD, 0))
# endif
# if !HAVE_DUP2
#  define dup2(old, new)  (close(new), fcntl(old, F_DUPFD, new))
# endif
# if HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif
#endif /* GCAL_EPAGER */
#include "common.h"
#if USE_RC
# include "rc-defs.h"
#endif /* USE_RC */
#include "globals.h"
#include "file-io.h"
#include "hd-defs.h"
#include "hd-data.h"
#include "hd-use.h"
#include "help.h"
#include "print.h"
#if USE_RC
# include "rc-use.h"
# include "rc-utils.h"
#endif /* USE_RC */
#include "tty.h"
#include "utils.h"
#include "gcal.h"



/*
*  Set Borland/Turbo-C specific MSDOS stack length in case USE_RC is defined  ;<
*/
#if USE_RC
# if defined(MSDOS) && defined(__TURBOC__)
#  include <dos.h>
extern Uint _stklen = 0x4000;
# endif
#endif



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `gcal.c'.
*/
static Bool
  is_correct_date_format __P_ ((char *format_txt,
				Bool * use_day_suffix,
				Bool * use_short3_day_name,
				Bool * use_day_zeroleaded,
				Bool * use_year_zeroleaded));
static void
  rearrange_argv __P_ ((const char *opt_list, int *argc, char *argv[]));
static void check_command_line __P_ ((int argc, char *argv[]));
static void build_month_list __P_ ((char *argv[]));
static void eliminate_invalid_data __P_ ((void));
static void pseudo_blank_conversion __P_ ((char **text));
#if USE_RC
static int further_check __P_ ((char **option));
#endif
__END_DECLARATIONS
/*
*  GLOBAL variables definitions.
*/

/*! Number of days in Julian/Gregorian month. */
const int dvec[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*! Number of past days of Julian/Gregorian month. */
const int mvec[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/*!
   The long option table is a vector of `Lopt_struct' terminated by an element
     containing a `long_name' which is zero!
   All initialization strings of the structure member `long_name' CAN be
     entered case insensitive (it is recommended to use them in lower-case
     letters only),  but in an ascending lexical sort order, and the table
     MUST be initialized by distinguishable texts given to the structure
     member `long_name'.
   The table structure is as follows:
     `symbolic_name': The symbolic name of the long option (a distingushable
                        number [int]).
     `long_name':     The full name of the long option (case insensitive.)
     `short_name':    A list of short options (maximum 5 (==LARG_MAX-1))
                        terminated by NULL, its order corresponds to the order
                        of the field `largs' (case sensitive), means "a" is
                        another option than "A".
     `larg_mode':     States whether an option requires an argument or not.
                        LARG_NO or 0 means:
                          Option may not have an argument, e.g. `--foo'
                        LARG_NO_OR_ONE or 1 means:
                          Option may have an argument,
                          which is either SPECIAL or GIVEN, e.g. `--foo'
                          or `--foo=BAR' or `--foo=ARG'
                        LARG_ONE or 2 means:
                          Option must have an argument,
                          which must be either SPECIAL if one or more `largs'
                          are defined, or which must be GIVEN if no `largs'
                          are defined, e.g. `--foo=BAR' or `--foo=ARG'
                        LARG_ONE_OR_ARG or 3 means:
                          Option must have an argument,
                          which could be either SPECIAL if one or more `largs'
                          are defined, or which could be GIVEN if one ore more
                          `largs' are defined, but none of these `largs' match
                          the GIVEN argument, e.g. `--foo=BAR' or `--foo=ARG'
                        If this field is set to LARG_NO_OR_ONE and NO argument
                        trails the option, the `short_name[0]' contains
                        the default value of the argument and is returned!
                        It's an ILLEGAL case to set this field to
                        LARG_ONE_OR_ARG and NO `largs' are defined in the
                        `lopt[]' table, but one or more `short_name' entries!
     `largs':         A list of possible SPECIAL arguments (maximum 5
                        (==LARG_MAX-1)) terminated by NULL, from which only
                        one may be selected at a time.
                      If its first element is set to NULL, any GIVEN argument
                        is allowed, otherwise only one of the listed SPECIAL
                        arguments.
                      The SPECIAL arguments may be abbreviated in case this
                        is done unambigously; and they can be given case
                        insensitive.
*/
const Lopt_struct lopt[] = {
/*
  { int symbolic_name, char *long_name, char *short_name[LARG_MAX], int larg_mode, char *largs[LARG_MAX] }
*/
#if USE_RC
  {
   SYM_ADJUST_VALUE,
   "adjust-value",
   {NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_ALTERNATIVE_FORMAT,
   "alternative-format",
   {"A", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_ASTRONOMICAL_HDY,
   "astronomical-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_ATMOSPHERE,
   "atmosphere",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_BAHAI_HDY,
   "bahai-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_BAHAI_MTH,
   "bahai-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_BIORHYTHM_AXIS_LEN,
   "biorhythm-axis",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_BLOCKS,
   "blocks",
   {"b", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_CALENDAR_DATES,
   "calendar-dates",
   {"j", "jb", NULL},
   LARG_ONE,
   {"special", "both", NULL}
   },
  {
   SYM_CC_HDY,
   "cc-holidays",
   {"q", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_CELTIC_HDY,
   "celtic-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_CHINESE_FLEXIBLE_HDY,
   "chinese-flexible-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_CHINESE_FLEXIBLE_MTH,
   "chinese-flexible-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_CHINESE_HDY,
   "chinese-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_CHINESE_MTH,
   "chinese-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_CHRISTIAN_HDY,
   "christian-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_COPTIC_MTH,
   "coptic-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_LICENSE2,
   "copyleft",
   {"L", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_LICENSE3,
   "copyright",
   {"L", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_CYCLE_END,
   "cycle-end",
   {NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_CYCLE_STEP,
   "cycle-step",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_DATE_FORMAT,
   "date-format",
   {"!\001", "!\002", "!\003", /* e.g.: "!\004", */ "!", NULL},
   LARG_ONE_OR_ARG,
   {CC_DE, CC_US, CC_GB,	/* e.g.: "OTHER", */
    /*
     *** Translators, please translate this as a fixed 3-character text.
     *** This text should be a proper abbreviation of "ARGUMENT".
     */
    N_("ARG"), NULL}
   },
#if USE_RC
  {
   SYM_DATE_VARIABLE,
   "date-variable",
   {"v", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_DEBUG,
   "debug",
   {"0", "1", "2", "3", "4", NULL},
   LARG_NO_OR_ONE,
   {"internal", "handled", "unhandled", "all", "abort", NULL}
   },
#if USE_RC
  {
   SYM_DESC_FIXED_DATES_LIST,
   "descending-fixed-dates",
   {"c-", "C-", NULL},
   LARG_NO_OR_ONE,
   {"short", "long", NULL}
   },
#endif
  {
   SYM_DESC_HOLIDAY_LIST,
   "descending-holiday-list",
   {"n-", "N-", NULL},
   LARG_NO_OR_ONE,
   {"long", "short", NULL}
   },
  {
   SYM_DISABLE_HIGHLIGHTING,
   "disable-highlighting",
   {"Hno", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_END_OF_MONTH,
   "end-of-month",
   {"m+", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_END_OF_WEEK,
   "end-of-week",
   {"w+", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_END_OF_YEAR,
   "end-of-year",
   {"y+", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_ETHIOPIC_MTH,
   "ethiopic-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_EXCLUDE_RC_TITLE,
   "exclude-fixed-dates-list-title",
   {"x", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_EXCLUDE_HD_TITLE,
   "exclude-holiday-list-title",
   {"X", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_EXECUTE_COMMAND,
   "execute-command",
   {NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_EXIT_STAT_HELP_NON_ZERO,
   "exit-status-help-non-zero",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_extern_static_DVARS,
   "export-date-variables",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_extern_static_TVARS,
   "export-text-variables",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_FILTER_DAY,
   "filter-day",
   {"D", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_FILTER_PERIOD,
   "filter-period",
   {"P", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_FILTER_TEXT,
   "filter-text",
   {"I", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_FIXED_DATES,
   "fixed-dates",
   {"jc", "jcb", NULL},
   LARG_ONE,
   {"special", "both", NULL}
   },
#endif
  {
   SYM_FORCE_HIGHLIGHTING,
   "force-highlighting",
   {"Hyes", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_FRENCH_REVOLUTIONARY_MTH,
   "french-revolutionary-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_GREG_REFORM,
   "gregorian-reform",
   {"$\001", "$\002", "$\003", "$\004", "$", NULL},
   LARG_ONE_OR_ARG,
   {"1582", "1700", "1752", "1753",
    /*
     *** Translators, please translate this as a fixed 3-character text.
     *** This text should be a proper abbreviation of "ARGUMENT".
     */
    N_("ARG"), NULL}
   },
#if USE_RC
  {
   SYM_GROUPING_TEXT,
   "grouping-text",
   {"g", NULL},
   LARG_NO_OR_ONE,
   {NULL}
   },
  {
   SYM_HEADING_TEXT,
   "heading-text",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_HEBREW_HDY,
   "hebrew-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_HEBREW_MTH,
   "hebrew-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_LONG_HELP1,
   "help",
   {"h", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_HERE_FILE,
   "here",
   {"#", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_HIGHLIGHTING,
   "highlighting",
   {"H", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_HOLIDAY_DATES,
   "holiday-dates",
   {"jn", "jnb", NULL},
   LARG_ONE,
   {"special", "both", NULL}
   },
  {
   SYM_HOLIDAY_LIST,
   "holiday-list",
   {"n", "N", NULL},
   LARG_NO_OR_ONE,
   {"long", "short", NULL}
   },
#if USE_RC
  {
   SYM_IGNORE_CASE,
   "ignore-case",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_INCLUDE_CONS_NUMBER,
   "include-consecutive-number",
   {"z", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_INCLUDE_HOLIDAY,
   "include-holidays",
   {"e", "E", NULL},
   LARG_NO_OR_ONE,
   {"long", "short", NULL}
   },
  {
   SYM_INCLUDE_FILENAME,
   "include-resource-file-name",
   {"a", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_INCLUDE_TODAY,
   "include-today",
   {"d", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_INCLUDE_WEEK_NUMBER,
   "include-week-number",
   {"k", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_INDIAN_CIVIL_MTH,
   "indian-civil-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ISLAMIC_HDY,
   "islamic-civil-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ISLAMIC_MTH,
   "islamic-civil-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ISO_WEEK_NUMBER,
   "iso-week-number",
   {"y", "n", NULL},
   LARG_ONE,
   {"yes", "no", NULL}
   },
  {
   SYM_JAPANESE_FLEXIBLE_HDY,
   "japanese-flexible-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_JAPANESE_FLEXIBLE_MTH,
   "japanese-flexible-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_JAPANESE_HDY,
   "japanese-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_JAPANESE_MTH,
   "japanese-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_LEAP_DAY,
   "leap-day",
   {"f", "m", NULL},
   LARG_ONE,
   {"february", "march", NULL}
   },
#endif
  {
   SYM_LICENSE1,
   "license",
   {"L", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_LIMIT,
   "limit",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_LIST_MODE,
   "list-mode",
   {"l", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_FIXED_DATES_LIST,
   "list-of-fixed-dates",
   {"c", "C", NULL},
   LARG_NO_OR_ONE,
   {"short", "long", NULL}
   },
#endif
  {
   SYM_LONG_HELP1,
   "long-help",
   {"hh", NULL},
   LARG_NO_OR_ONE,
   {NULL}
   },
#ifdef GCAL_EMAIL
  {
   SYM_MAIL,
   "mail",
   {NULL},
   LARG_NO_OR_ONE,
   {NULL}
   },
#endif
#if USE_RC
  {
   SYM_MONTH,
   "month",
   {"m", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_MOONIMAGE_LINES,
   "moonimage-lines",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_MULTICULTURAL_NEW_YEAR_HDY,
   "multicultural-new-year-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_OLD_ARMENIC_MTH,
   "old-armenic-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_OLD_EGYPTIC_MTH,
   "old-egyptic-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_OMIT_DATE_PART,
   "omit-multiple-date-part",
   {"o", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_ORTHODOX_CALENDAR,
   "orthodox-calendar",
   {"O", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ORTHODOX_NEW_HDY,
   "orthodox-new-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ORTHODOX_OLD_HDY,
   "orthodox-old-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_PAGER
  {
   SYM_PAGER,
   "pager",
   {"p", NULL},
   LARG_NO,
   {NULL}
   },
#endif
#if USE_RC
  {
   SYM_PERIOD,
   "period-of-fixed-dates",
   {"c", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_PERSIAN_HDY,
   "persian-jalaali-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_PERSIAN_MTH,
   "persian-jalaali-months",
   {NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_PRECISE,
   "precise",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_RESOURCE_FILE,
   "resource-file",
   {"f", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_RESPONSE_FILE,
   "response-file",
   {"R", NULL},
   LARG_ONE,
   {NULL}
   },
#if USE_RC
  {
   SYM_REVERT_MATCH,
   "revert-match",
   {NULL},
   LARG_NO,
   {NULL}
   },
#endif
#ifdef GCAL_SHELL
  {
   SYM_SCRIPT_FILE,
   "shell-script",
   {"S", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
#if USE_RC
  {
   SYM_START_OF_MONTH,
   "start-of-month",
   {"m-", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_START_OF_WEEK,
   "start-of-week",
   {"w-", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_START_OF_YEAR,
   "start-of-year",
   {"y-", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_STARTING_DAY,
   "starting-day",
   {"s", NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_SUPPRESS_CALENDAR,
   "suppress-calendar",
   {"u", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_SUPPRESS_DATE_PART,
   "suppress-date-part",
   {"U", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_SUPPRESS_FDLIST_SEP,
   "suppress-fixed-dates-list-separator",
   {"Q", NULL},
   LARG_NO,
   {NULL}
   },
#endif /* USE_RC */
  {
   SYM_SUPPRESS_HDLIST_SEP,
   "suppress-holiday-list-separator",
   {"G", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_SUPPRESS_TEXT_PART,
   "suppress-text-part",
   {"J", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_TEXT_VARIABLE,
   "text-variable",
   {"r", NULL},
   LARG_ONE,
   {NULL}
   },
#endif
  {
   SYM_TIME_OFFSET,
   "time-offset",
   {NULL},
   LARG_ONE,
   {NULL}
   },
#if USE_RC
  {
   SYM_TODAY,
   "today",
   {"c", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_TOMORROW,
   "tomorrow",
   {"t", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_TRANSFORM_YEAR,
   "transform-year",
   {NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_TRANSLATE_STRING,
   "translate-string",
   {NULL},
   LARG_ONE,
   {NULL}
   },
  {
   SYM_TYPE_OF_CALENDAR,
   "type",
   {"i", "i-", NULL},
   LARG_ONE,
   {"special", "standard", NULL}
   },
  {
   SYM_LONG_HELP2,
   "usage",
   {"hh", NULL},
   LARG_NO_OR_ONE,
   {NULL}
   },
  {
   SYM_VERSION,
   "version",
   {"V", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_WEEK,
   "week",
   {"w", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_CALENDAR_WITH_WEEK_NUMBER,
   "with-week-number",
   {"K", NULL},
   LARG_NO,
   {NULL}
   },
#if USE_RC
  {
   SYM_YEAR,
   "year",
   {"y", NULL},
   LARG_NO,
   {NULL}
   },
  {
   SYM_ZERO_DATES,
   "zero-dates-only",
   {"Z", NULL},
   LARG_NO,
   {NULL}
   },
#endif
  {
   SYM_ZODIACAL_MARKER_HDY,
   "zodiacal-marker-holidays",
   {NULL},
   LARG_NO,
   {NULL}
   },
  {SYM_NIL, NULL, {NULL}, LARG_NO, {NULL}}
};

/*!
   The supported date formats table is a vector of `Df_struct'
     terminated by an element containing a `df_info' which is zero!
   If you extend this table by some more date formats, please extend the
     according `lopt[]' table entry for the `--date-format' long-style option,
     too!  If you do so, remember that each new entry in the `lopt[]' table
     for SYM_DATE_FORMAT must have a new `short_name' text
     "!\NEXT-CONSECUTIVE-OCTAL-NUMBER-OF-THIS-NEW-INDEX" and a corresponding
     identifying text in `largs' (with a length of less than 6 characters).
     The simple "!" text in the `short_name' member and its corresponding
     LARG_TXT must always trail any new entry, which will be possibly added
     in future.
*/
Df_struct supported_date_format[LARG_MAX - 1] = {
/*
  { char *df_id, char *df_info, char *df_format },
*/
  {CC_DE, N_("Germany"), "%<2#K,  %1%>2*D%2 %<3#U %>04*Y"},
  {CC_US, N_("U.S.A."), "%<3#K,  %<3#U %1%>2&*D%2 %>04*Y"},
  {CC_GB, N_("Great Britain"), "%<3#K,  %1%>2&*D%2 %<3#U %>04*Y"},
/*
   The next table field can be provided with further data!
   If more "burned-in" data is needed, LARG_MAX must be increased accordingly,
   because it's necessary to respect the data of this fields in the functions,
   which manage the long-style arguments!
*/
  {NULL, NULL, NULL},
/*
   This next "empty" table field must be kept UNTOUCHED,
     because it marks the end of the table!
*/
  {NULL, NULL, NULL}
};

/* Points to the used date format. */
Df_struct *date_format = supported_date_format;

/*
   The Gregorian Reformation dates table is a vector of `Greg_struct'
     terminated by an element containing a `year' which is zero!
   If you extend this table by some more dates, please extend the according
     `lopt[]' table entry for the `--gregorian-reform' long-style option, too!
     If you do so, remember that each new entry in the `lopt[]' table for
     SYM_GREG_REFORM must have a new `short_name' text
     "$\NEXT-CONSECUTIVE-OCTAL-NUMBER-OF-THIS-NEW-INDEX" and a corresponding
     year of Gregorian Reformation text in `largs'.  The simple "$" text in
     the `short_name' member and its corresponding LARG_TXT must always
     trail any new entry, which will be possibly added in future.
*/
Greg_struct greg_reform_date[LARG_MAX - 1] = {
/*
  { int year, int month, int f_day, int l_day },
*/
  {1582, 10, 5, 14},
  {1700, 2, 19, 28},
  {1752, 9, 3, 13},
  {1753, 2, 18, 28},
/*
   This next "empty" table field must be kept UNTOUCHED,
     because it marks the end of the table!
*/
  {0, 0, 0, 0}
};

/*! Points to the used Gregorian Reformation date. */
Greg_struct *greg = greg_reform_date;

/*! User defined Gregorian Reformation date. */
Greg_struct users_greg;

#ifdef GCAL_EMAIL
/*! Temporary file which is send by the mailer. */
FILE *tfp = (FILE *) NULL;
#endif

/*! Used if a list/range of months/years is given. */
Ml_struct *month_list = (Ml_struct *) NULL;

/*! Effective hls 1 start (current day). */
Hls_struct ehls1s;

/*! Effective hls 1 end (current day). */
Hls_struct ehls1e;

/*! Effective hls 2 start (holiday). */
Hls_struct ehls2s;

/*! Effective hls 2 end (holiday). */
Hls_struct ehls2e;

#ifdef DJG
/*! Set to SHRT_MAX for checking the maximum table range. */
Usint testval = (Usint) 0;
#else
/*! Set to INT_MAX for checking the maximum table range. */
Uint testval = (Uint) 0;
#endif

/*! Actual size of all string vectors. */
Uint maxlen_max = MAXLEN_MAX;

/*! String length of the maximum year able to compute. */
int len_year_max = 0;

/*! Maximum string length of a textual day_name(). */
int len_dayname_max = 0;

/*! Maximum string length of a textual month_name(). */
int len_monthname_max = 0;

/*! Maximum string length of a textual day_suffix() [if any]. */
int len_suffix_max = 0;

/*! `--debug[=0...WARN_LVL_MAX]', SPECIAL VALUE at startup. */
int warning_level = SPECIAL_VALUE;

/*! `-s<0,1...7|day name>'. */
int start_day = 0;

/*! `--transform-year=BASE_YEAR'. */
int transform_year = 0;

/*! `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
int time_hour_offset = 0;

/*! `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
int time_min_offset = 0;

/*! Current day. */
int day = 0;

/*! Current month. */
int month = 0;

/*! Current year. */
int year = 0;

/*! Actual second. */
int act_sec = 0;

/*! Actual minute. */
int act_min = 0;

/*! Actual hour. */
int act_hour = 0;

/*! Actual day. */
int act_day = 0;

/*! Actual month. */
int act_month = 0;

/*! Actual year. */
int act_year = 0;

/*! Buffer of actual day. */
int buf_ad = 0;

/*! Buffer of actual month. */
int buf_am = 0;

/*! Buffer of actual year. */
int buf_ay = 0;

/*! True actual day as reported by the operating system. */
int true_day = 0;

/*! True actual month as reported by the operating system. */
int true_month = 0;

/*! True actual year as reported by the operating system. */
int true_year = 0;

/*! Starting month of a fiscal year. */
int fiscal_month = MONTH_MIN;

/*! Is output displayed on a terminal? */
int is_tty = 0;

/*! Is output directed to channel 1? */
int is_tty1 = 0;

/*! Is output directed to channel 2? */
int is_tty2 = 0;

#if USE_PAGER
/*! Number of tty rows, SPECIAL_VALUE at startup. */
int tty_rows = SPECIAL_VALUE;

/*! Number of tty columns, SPECIAL_VALUE at startup. */
int tty_cols = SPECIAL_VALUE;
#endif

/*! Number of month rows of a year calendar. */
int out_rows = 0;

/*! Number of month columns of a year calendar. */
int out_cols = 0;

/*! Format length of a standard/special/both day. */
int format_len = 0;

/*! Is current year a leap year? */
int is_leap_year = 0;

#ifdef GCAL_EMAIL
/*! Name of tempfile used by the mailer. */
char *tfn = (char *) NULL;

/*! Email address Gcal's output is send to. */
char *email_adr = (char *) NULL;
#endif

/*! `--cc-holidays=CC[+CC+...]'. */
char *cc = (char *) NULL;

/*! The "YY" text. */
char *yy_lit = (char *) NULL;

/*! The "YYYY" text. */
char *yyyy_lit = (char *) NULL;

/*! The "MM" text. */
char *mm_lit = (char *) NULL;

/*! The "WWW" text. */
char *www_lit = (char *) NULL;

/*! The "DD" text. */
char *dd_lit = (char *) NULL;

/*! The "ARG" text. */
char *larg_lit = (char *) NULL;

/*! General purpose text buffer 1. */
char *s1 = (char *) NULL;

/*! General purpose text buffer 2. */
char *s2 = (char *) NULL;

/*! General purpose text buffer 3. */
char *s3 = (char *) NULL;

/*! General purpose text buffer 4. */
char *s4 = (char *) NULL;

/*! Stores the actual program name. */
char *prgr_name = (char *) NULL;

/*! Character for separating HH:MM time values. */
char *time_sep = (char *) NULL;

/*! `--translate-string=CHARACTER_PAIR...'. */
char *translate_string = (char *) NULL;

/*! Pointer to the $TZ (timezone) environment variable. */
char *tz = (char *) NULL;

#ifdef GCAL_EPAGER
/*! Name of external pager program. */
char *ext_pager = (char *) NULL;
#endif

/*! Day suffix format specifier given in date format?. */
Bool use_day_suffix = FALSE;

/*! 3 char day name format specifier given in date format? */
Bool use_short3_day_name = FALSE;

/*! Day number leaded with zeroes format specifier given in date format? */
Bool use_day_zeroleaded = FALSE;

/*! Year number leaded with zeroes format specifier given in date format? */
Bool use_year_zeroleaded = FALSE;

/*! Don't use Astronomical holidays by default. */
Bool hdy_astronomical = FALSE;

/*! Don't use Bahai calendar holidays by default. */
Bool hdy_bahai = FALSE;

/*! Don't use Celtic calendar holidays by default. */
Bool hdy_celtic = FALSE;

/*! Don't use Chinese calendar holidays by default. */
Bool hdy_chinese = FALSE;

/*! Don't use Chinese flexible calendar holidays by default. */
Bool hdy_chinese_flexible = FALSE;

/*! Don't use Christian Western churches calendar holidays by default. */
Bool hdy_christian = FALSE;

/*! Don't use Hebrew calendar holidays by default. */
Bool hdy_hebrew = FALSE;

/*! Don't use Islamic CIVIL calendar holidays by default. */
Bool hdy_islamic = FALSE;

/*! Don't use Japanese calendar holidays by default. */
Bool hdy_japanese = FALSE;

/*! Don't use Japanese flexible calendar holidays by default. */
Bool hdy_japanese_flexible = FALSE;

/*! Don't use Multicultural New_Year's_Day holidays by default. */
Bool hdy_multicultural_new_year = FALSE;

/*! Don't use Orthodox Christian Eastern churches NEW calendar holidays by default. */
Bool hdy_orthodox_new = FALSE;

/*! Don't use Orthodox Christian Eastern churches OLD calendar holidays by default. */
Bool hdy_orthodox_old = FALSE;

/*! Don't use Persian Jalaali calendar holidays by default. */
Bool hdy_persian = FALSE;

/*! Don't use Zodiacal Marker holidays by default. */
Bool hdy_zodiacal_marker = FALSE;

/*! Don't use Bahai calendar months by default. */
Bool mth_bahai = FALSE;

/*! Don't use Chinese calendar months by default. */
Bool mth_chinese = FALSE;

/*! Don't use Chinese flexible calendar months by default. */
Bool mth_chinese_flexible = FALSE;

/*! Don't use Coptic calendar months by default. */
Bool mth_coptic = FALSE;

/*! Don't use Ethiopic calendar months by default. */
Bool mth_ethiopic = FALSE;

/*! Don't use French Revolutionary calendar months by default. */
Bool mth_french_revolutionary = FALSE;

/*! Don't use Hebrew calendar months by default. */
Bool mth_hebrew = FALSE;

/*! Don't use Indian CIVIL calendar months by default. */
Bool mth_indian_civil = FALSE;

/*! Don't use Islamic CIVIL calendar months by default. */
Bool mth_islamic = FALSE;

/*! Don't use Japanese calendar months by default. */
Bool mth_japanese = FALSE;

/*! Don't use Japanese flexible calendar months by default. */
Bool mth_japanese_flexible = FALSE;

/*! Don't use Old-Armenic calendar months by default. */
Bool mth_old_armenic = FALSE;

/*! Don't use Old-Egyptic calendar months by default. */
Bool mth_old_egyptic = FALSE;

/*! Don't use Persian Jalaali calendar months by default. */
Bool mth_persian = FALSE;

/*! `-O' (compute leap years as done by Eastern churches). */
Bool orthodox_calendar = FALSE;

/*! `-u'. */
Bool suppr_cal_flag = FALSE;

/*! `-H<yes>|<no>'. */
Bool highlight_flag = TRUE;

/*! `--iso-week-number=<yes>|<no>'. */
Bool iso_week_number = FALSE;

/*! `-K'. */
Bool cal_with_week_number = FALSE;

/*! `-j'. */
Bool cal_special_flag = FALSE;

/*! `-jb'. */
Bool cal_both_dates_flag = FALSE;

/*! `-n|N'. */
Bool holiday_flag = FALSE;

/*! `-N'. */
Bool hd_legal_days_only = FALSE;

/*! `-n|N-'. */
Bool hd_sort_des_flag = FALSE;

/*! `-jn'. */
Bool hd_special_flag = FALSE;

/*! `-jnb'. */
Bool hd_both_dates_flag = FALSE;

/*! `-G'. */
Bool hd_suppr_list_sep_flag = FALSE;

/*! `-X'. */
Bool hd_title_flag = TRUE;

/*! ':' char found in argument (MM:YYYY). */
Bool is_fiscal_year = FALSE;

/*! Argument is `.' or `.+' or `.-'. */
Bool is_3month_mode = FALSE;

/*! Argument is `..' -> current quarter of actual year. */
Bool is_3month_mode2 = FALSE;

/*! Is an extended list/range of years given? */
Bool is_ext_year = FALSE;

/*! Is an extended list of months/years given? */
Bool is_ext_list = FALSE;

/*! Is an extended range of months/years given? */
Bool is_ext_range = FALSE;

/*! Is a special range of a selected month of years given? */
Bool is_special_range = FALSE;

/*! Is a special range of selected months of years given? */
Bool is_multi_range = FALSE;

#ifdef GCAL_NLS
/*! Support of English language? */
Bool is_en = FALSE;
#endif

/*! `-i[-]'. */
Bool special_calsheet_flag = FALSE;

#if USE_HLS
/*! Must we emulate the highlighting sequences? */
Bool emu_hls = FALSE;
#else /* !USE_HLS */
/*! Must we emulate the highlighting sequences? */
Bool emu_hls = TRUE;
#endif /* !USE_HLS */

#if USE_PAGER
/*! `-p'. */
Bool pager_flag = FALSE;
#endif



/*
*  static variables definitions.
*/
#ifdef GCAL_EPAGER
/*! Child process id of external pager. */
static pid_t child_pid;

/*! Pipe file descriptors. */
static int pipe_fd[2];

/*! Buffer of system file descriptors 0 and 1. */
static int sys_fd[2];
#endif

/*! User defined date format. */
static Df_struct users_date_format;

/*! Maximum number of `month_list[]' table elems. */
static Uint month_list_max = MONTH_MAX + 1;

/*! The index value of a long option. */
static int lopt_id = 0;

/*! Termination status on `--help', `--version' etc... */
static int exit_stat_help = EXIT_STAT_HELP;

/*! Buffers default value of `-s<ARG>' option. */
static int buf_start_day = 0;

#ifdef GCAL_EPAGER
/*!
   Possible options passed to the $PAGER external pager program.
*/
static char **pg_argv = (char **) NULL;

/*!
   The external pager program names table is a vector of char pointer
     elements, which must be terminated by a NULL element!
*/
static char *pagers[] = { PAGER1_PROG, PAGER2_PROG, PAGER3_PROG, NULL };
#endif


#ifdef GCAL_SHELL
/*! File name of shell script to write `-S<NAME>'. */
static char *shl_filename = (char *) NULL;
#endif

/*! Name of response file to read (@FILE) or write (-R<NAME>). */
static char *rsp_filename = (char *) NULL;

/*! Text containing user defined highlighting sequences `-H<>'. */
static char *hl_seq = (char *) NULL;

/*! Points to "date format error location" description text. */
static char *errtxt_dformat = (char *) NULL;

/*! `-b<1|2|3|4|6|12>'. */
static Bool year_flag = FALSE;



/*
*  Function implementations.
*/
int
main (argc, argv)
     int argc;
     char *argv[];
/*!
   The Gcal program entry point   =8^)
*/
{
  auto FILE *fp = (FILE *) NULL;
  auto Uint my_argc_max = MY_ARGC_MAX;
  auto int my_argc = 1;
  auto int i;
  register int j;
  auto char **my_argv = (char **) NULL;
#if USE_RC
# ifdef GCAL_SHELL
  static const char *these_short_opts_need_args = "#DFHIPRSbfqrsv";
# else /* !GCAL_SHELL */
  static const char *these_short_opts_need_args = "#DFHIPRbfqrsv";
# endif	/* !GCAL_SHELL */
#else /* !USE_RC */
# ifdef GCAL_SHELL
  static const char *these_short_opts_need_args = "HRSbqs";
# else /* !GCAL_SHELL */
  static const char *these_short_opts_need_args = "HRbqs";
# endif	/* !GCAL_SHELL */
#endif /* !USE_RC */
  auto char *ptr_char;
  auto char *y_txt;
  auto char *m_txt;
  auto char *w_txt;
  auto char *d_txt;
  auto Bool show_calendar = TRUE;
  auto Bool b_dummy;


  /*
     Let's set `testval' to SHRT_MAX/INT_MAX if SHRT_MAX/INT_MAX itself
     isn't defined.  This solution only works on machines with internal
     arithmethics based on "two complements".
   */
#ifdef DJG
# ifdef SHRT_MAX
  testval = SHRT_MAX;
# else /* !SHRT_MAX */
  testval = ~0;
  testval >>= 1;
# endif	/* !SHRT_MAX */
#else /* !DJG */
# ifdef INT_MAX
  testval = INT_MAX;
# else /* !INT_MAX */
  testval = ~0;
  testval >>= 1;
# endif	/* !INT_MAX */
#endif /* !DJG */
#if HAVE_ASSERT_H
  /*
     To ensure safe program operations,
     MAXLEN_MAX must be 1024 minimum and `testval' maximum!
   */
  assert (MAXLEN_MAX >= 1024);
  assert ((Uint) MAXLEN_MAX <= testval);
#endif
  /*
     Then we have to allocate all string vectors we'll use.
   */
  allocate_all_strings (MAXLEN_MAX, __FILE__, (long) __LINE__);
  /*
     Compute the string length of the maximum year able to compute.
   */
  sprintf (s1, "%d", YEAR_MAX);
  len_year_max = (int) strlen (s1);
  /*
     Perform some more assertations for safe program operation.
   */
#if HAVE_ASSERT_H
  /*
     Check if value for maximum number of table entries
     fits to the positive range of a signed int (SHRT_MAX/INT_MAX)!
   */
  assert (my_argc_max > 0);
  assert (my_argc_max <= testval);
  assert (len_year_max < 11);
  assert (len_year_max > 0);
  assert (YEAR_MAX >= YEAR_MIN);
  assert (MONTH_MAX == 12);
  assert (CENTURY == 1900);
  /*
     For safe program operation, BUF_LEN must be 1 minimum and `testval'-1 maximum!
   */
  assert (BUF_LEN >= 1);
  assert ((Uint) BUF_LEN < testval);
  assert (strlen (PACKAGE_NAME) > 0);
  assert (strlen (PACKAGE_VERSION) > 0);
  assert (MY_ARGC_MAX > 1);
  assert ((Uint) MY_ARGC_MAX <= testval);
  assert (HD_ELEMS_MAX > 0);
  assert ((Uint) HD_ELEMS_MAX <= testval);
  assert (FWIDTH_MAX > 0);
#endif /* HAVE_ASSERT_H */
#ifdef GCAL_NLS
  /*
     Now initialize the NLS functions.
   */
# if HAVE_SETLOCALE
  setlocale (LC_ALL, "");
# endif
# ifndef LOCALEEDIR
#  define LOCALEEDIR  NULL
# endif
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  /*
     Now check whether we use a native language message catalog
     or the internal default (==English) language texts!
   */
# if !defined(AMIGA) || defined(__GNUC__)
  /*
     Detect whether the $LANGUAGE environment variable (GNU specific) is set.
   */
  ptr_char = getenv (ENV_VAR_LANGUAGE);
  if (ptr_char != (char *) NULL)
    if (!*ptr_char)
      ptr_char = (char *) NULL;
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LC_ALL environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LC_ALL);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
#  if HAVE_LC_MESSAGES
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LC_MESSAGES environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LC_MESSAGES);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
#  endif
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LANG environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LANG);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
  /*
     Now check the kind of territory specifics we have to use!
   */
  if (ptr_char != (char *) NULL)
    {
      if (*ptr_char)
	{
	  strncpy (s1, ptr_char, 6);
	  s1[5] = '\0';
	  if (!strncasecmp (s1, "de", 2))
	    /*
	       We have to use German texts and territory specifics!
	     */
	    is_en = FALSE;
	  else if (!strncasecmp (s1, "en", 2))
	    /*
	       We have to use English texts, but we check for territory
	       specifics (either U.S.A. or Great Britain) later!
	     */
	    is_en = TRUE;
	  else
	    /*
	       Hmm... We have to check whether the special settings "C" or
	       "POSIX" are given, if so, we have to use English texts
	       and U.S.A. territory by default!
	     */
	  if (!strncasecmp (s1, "posix", 5) || !strcasecmp (s1, "c"))
	    is_en = TRUE;
	}
      else
	/*
	   Environment variable defined but not set, hmm...
	   We use English texts and U.S.A. territory specifics by default!
	 */
	is_en = TRUE;
    }
  else
    /*
       No environment variable defined.
     */
# endif /* !AMIGA || __GNUC__ */
    /*
       We use English texts and U.S.A. territory specifics by default!
     */
    is_en = TRUE;
  /*
     Set some variables according to the native language used.
   */
  if (is_en)
    {
      /*
         `is_en' means English texts and U.S.A. territory specifics by default for now.
       */
      special_calsheet_flag = TRUE;
      iso_week_number = FALSE;
      out_rows = J_OUT_ROWS;
      /*
         Set the date of Gregorian Reformation to 1752 (table index 2 !!)
       */
      greg += 2;
      /*
         Set the date format to U.S.A. style (table index 1 !!)
       */
      date_format++;
# if !defined(AMIGA) || defined(__GNUC__)
      /*
         Now check whether if we have to use the British date format.
       */
      if (ptr_char != (char *) NULL)
	if (*ptr_char)
	  {
	    /*
	       Skip the first three characters for detecting a possibly territory information.
	     */
	    for (i = 0; i < 3; i++)
	      if (*ptr_char)
		ptr_char++;
	    if (*ptr_char)
	      {
		strncpy (s1, ptr_char, 3);
		s1[2] = '\0';
		/*
		   We have to use the British date format!
		 */
		if (!strcasecmp (s1, CC_GB))
		  /*
		     Set the date format to Great Britain style (table index 2 !!)
		   */
		  date_format++;
	      }
	  }
# endif /* !AMIGA || __GNUC__ */
    }
  else
    {
      /*
         `!is_en' means German territory specifics (useful for most Europeans)
         by default and possibly another native language instead of English.
       */
      special_calsheet_flag = FALSE;
      iso_week_number = TRUE;
      out_rows = S_OUT_ROWS;
    }
#else /* !GCAL_NLS */
  special_calsheet_flag = TRUE;
  iso_week_number = FALSE;
  out_rows = J_OUT_ROWS;
  /*
     Set the date of Gregorian Reformation to 1752 (table index 2 !!)
   */
  greg += 2;
  /*
     Set the date format to U.S.A. style (table index 1 !!)
   */
  date_format++;
#endif	/* !GCAL_NLS */
  /*
     Test if the default date format is valid.
   */
  if (!is_correct_date_format
      (date_format->df_format, &use_day_suffix, &use_short3_day_name,
       &use_day_zeroleaded, &use_year_zeroleaded))
    errtxt_dformat = _("default");
  /*
     Test if the program output is redirected or piped.
   */
#if USE_PAGER
  is_tty1 = isatty (1);
  is_tty2 = isatty (2);
  is_tty = (is_tty1 && is_tty2);
# ifdef GCAL_EPAGER
  if (is_tty)
    {
      /*
         Store the SYS-STDIN/SYS-STDOUT file descriptors.
       */
      dup2 (0, sys_fd[0]);
      dup2 (1, sys_fd[1]);
    }
# endif
#else /* !USE_PAGER */
  is_tty = (int) TRUE;
#endif /* !USE_PAGER */
  /*
     Detect the own program name.
   */
  i = (int) strlen (*argv);
  if ((Uint) i >= maxlen_max)
    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
  if (!i)
    strcpy (s3, PACKAGE_NAME);
  else
    strcpy (s3, *argv);
#ifdef SUFFIX_SEP
  /*
     Eliminate version suffix under VMS.
   */
  ptr_char = strrchr (s3, *SUFFIX_SEP);
  if (ptr_char != (char *) NULL)
    *ptr_char = '\0';
#endif
  i = (int) strlen (s3);
#ifdef DJG
  ptr_char = strrchr (s3, *DIR2_SEP);
#else /* !DJG */
  ptr_char = strrchr (s3, *DIR_SEP);
#endif /* !DJG */
  if (ptr_char != (char *) NULL)
    {
      ptr_char++;
      i = (int) strlen (ptr_char);
    }
  else
    ptr_char = s3;
  if (tolower ('A') == 'a')
    {
      auto char *buf_ptr_char = ptr_char;


      for (; *ptr_char; ptr_char++)
	*ptr_char = (char) tolower (*ptr_char);
      ptr_char = buf_ptr_char;
    }
  /*
     Suppress ".exe" suffix for MSDOS, OS/2 and VMS.
   */
  if ((i > 4) && !strcmp (ptr_char + i - 4, ".exe"))
    {
      i -= 4;
      *(ptr_char + i) = '\0';
    }
  prgr_name = (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE,
				  __FILE__, ((long) __LINE__) - 1L,
				  "prgr_name", 0);
  strcpy (prgr_name, ptr_char);
#if HAVE_SIGNAL
  /*
     Now let's modify the signal handling a bit to make sure that
     temporary files are always deleted if such signals are raised.
   */
# ifdef SIGINT
  if (signal (SIGINT, SIG_IGN) != SIG_IGN)
    (void) signal (SIGINT, (Sig_type) handle_signal);
# endif
# ifdef SIGTERM
  if (signal (SIGTERM, SIG_IGN) != SIG_IGN)
    (void) signal (SIGTERM, (Sig_type) handle_signal);
# endif
# ifdef SIGHUP
  if (signal (SIGHUP, SIG_IGN) != SIG_IGN)
    (void) signal (SIGHUP, (Sig_type) handle_signal);
# endif
#endif /* HAVE_SIGNAL */
  /*
     Assign the character which is used for separating the time HH:MM.
   */

  /*
   *** Translators, please translate this as a fixed 1-character text.
   *** This is the character which is used for separating the time HH:MM.
   */
  time_sep = _(":");

  /*
     Initialize the basic meta texts.
   */

  /*
   *** Translators, please translate this as a fixed 1-character text.
   *** This text should be a proper abbreviation of "Year".
   */
  y_txt = _("Y");
  /*
   *** Translators, please translate this as a fixed 1-character text.
   *** This text should be a proper abbreviation of "Month".
   */
  m_txt = _("M");
  /*
   *** Translators, please translate this as a fixed 1-character text.
   *** This text should be a proper abbreviation of "Week".
   */
  w_txt = _("W");
  /*
   *** Translators, please translate this as a fixed 1-character text.
   *** This text should be a proper abbreviation of "Day".
   */
  d_txt = _("D");

  /*
     Initial memory allocation and initialization of the variables
     which contain the "YY", "YYYY", "MM", "WWW" and "DD" literals.
   */
  yy_lit = (char *) my_malloc (3, ERR_NO_MEMORY_AVAILABLE,
			       __FILE__, ((long) __LINE__) - 1L, "yy_lit", 0);
  for (i = 0; i < 2; i++)
    yy_lit[i] = (char) toupper (*y_txt);
  yy_lit[i] = '\0';
  yyyy_lit = (char *) my_malloc (len_year_max + 1, ERR_NO_MEMORY_AVAILABLE,
				 __FILE__, ((long) __LINE__) - 1L,
				 "yyyy_lit", 0);
  for (i = 0; i < len_year_max; i++)
    yyyy_lit[i] = (char) toupper (*y_txt);
  yyyy_lit[i] = '\0';
  mm_lit = (char *) my_malloc (3, ERR_NO_MEMORY_AVAILABLE,
			       __FILE__, ((long) __LINE__) - 1L, "mm_lit", 0);
  for (i = 0; i < 2; i++)
    mm_lit[i] = (char) toupper (*m_txt);
  mm_lit[i] = '\0';
  www_lit = (char *) my_malloc (4, ERR_NO_MEMORY_AVAILABLE,
				__FILE__, ((long) __LINE__) - 1L,
				"www_lit", 0);
  for (i = 0; i < 3; i++)
    www_lit[i] = (char) toupper (*w_txt);
  www_lit[i] = '\0';
  dd_lit = (char *) my_malloc (3, ERR_NO_MEMORY_AVAILABLE,
			       __FILE__, ((long) __LINE__) - 1L, "dd_lit", 0);
  for (i = 0; i < 2; i++)
    dd_lit[i] = (char) toupper (*d_txt);
  dd_lit[i] = '\0';
  /*
     Initial memory allocation and initialization of the variable which
     contains the 3 characters "ARG" literal (a proper abbreviation of
     the word "ARGUMENT").
   */
  larg_lit = (char *) my_malloc (4, ERR_NO_MEMORY_AVAILABLE,
				 __FILE__, ((long) __LINE__) - 1L,
				 "larg_lit", 0);

  /*
   *** Translators, please translate this as a fixed 3-character text.
   *** This text should be a proper abbreviation of "ARGUMENT".
   */
  strncpy (larg_lit, _("ARG"), 4);

  larg_lit[3] = '\0';
  /*
     Initial memory allocation for the `my_argv[]' table.
   */
  my_argv = (char **) my_malloc (MY_ARGC_MAX * sizeof (char *),
				 ERR_NO_MEMORY_AVAILABLE,
				 __FILE__, ((long) __LINE__) - 2L,
				 "my_argv[MY_ARGC_MAX]", MY_ARGC_MAX);
  /*
     Initial memory allocation for `month_list[]' structure,
     the number of vector elements must be set to MONTH_MAX+1 in this case,
     because the vector must always be terminated by an empty structure
     element (year==0 && month==0).
   */
  month_list = (Ml_struct *) my_malloc ((MONTH_MAX + 1) * sizeof (Ml_struct),
					ERR_NO_MEMORY_AVAILABLE,
					__FILE__, ((long) __LINE__) - 2L,
					"month_list[MONTH_MAX+1]",
					MONTH_MAX + 1);
  /*
     Initialize `month_list[]' structure.
   */
  for (i = 0; i <= MONTH_MAX; i++)
    month_list[i].ml_year = month_list[i].ml_month = 0;
#if USE_RC
  /*
     Initial memory allocation for an element of the `Line_struct' record.
   */
  lineptrs = (Line_struct *) my_malloc (sizeof (Line_struct),
					ERR_NO_MEMORY_AVAILABLE,
					__FILE__, ((long) __LINE__) - 2L,
					"lineptrs", 0);
  /*
     Initial memory allocation for an element of the `Line_struct' record
     which is needed if we have to evaluate %?... special texts.
   */
  lptrs3 = (Line_struct *) my_malloc (sizeof (Line_struct),
				      ERR_NO_MEMORY_AVAILABLE,
				      __FILE__, ((long) __LINE__) - 2L,
				      "lptrs3", 0);
  /*
     Initial memory allocation for the fixed date list title text.
   */
  ptr_char = _("Fixed date list:");

  rc_heading_text = (char *) my_malloc (strlen (ptr_char) + 1 + 2,
					ERR_NO_MEMORY_AVAILABLE,
					__FILE__, ((long) __LINE__) - 2L,
					"rc_heading_text", 0);
  strcpy (rc_heading_text, "0 ");
  strcat (rc_heading_text, ptr_char);
#endif /* USE_RC */
#if !defined(AMIGA) || defined(__GNUC__)
  /*
     Detect whether a $GCAL_DATE_FORMAT environment variable is set.
   */
  ptr_char = getenv (ENV_VAR_GCAL_DATE_FORMAT);
  if (ptr_char != (char *) NULL)
    if (*ptr_char)
      {
	users_date_format.df_info = _("environment variable");

	users_date_format.df_format =
	  (char *) my_malloc (strlen (ptr_char) + 1, ERR_NO_MEMORY_AVAILABLE,
			      __FILE__, ((long) __LINE__) - 2L,
			      "users_date_format.df_format", 0);
	strcpy (users_date_format.df_format, ptr_char);
	date_format = &users_date_format;
	/*
	   Test if the date format given in environment variable is valid.
	 */
	if (!is_correct_date_format
	    (date_format->df_format, &use_day_suffix, &use_short3_day_name,
	     &use_day_zeroleaded, &use_year_zeroleaded))

	  errtxt_dformat = _("environment variable");

	else
	  errtxt_dformat = (char *) NULL;
      }
  /*
     Detect whether a $TZ (timezone) environment variable is set.
   */
  tz = getenv (ENV_VAR_TZ);
  if (tz != (char *) NULL)
    if (!*tz)
      tz = (char *) NULL;
  /*
     Detect whether the $GCAL environment variable is set.
   */
  ptr_char = getenv (ENV_VAR_GCAL);
  if (ptr_char != (char *) NULL)
    {
      if (*ptr_char)
	{
	  /*
	     $GCAL environment variable defined and not empty:
	     Copy the OPTIONS, expanded @FILE arguments or the actual date
	     modifier %DATE argument only; found in environment variable
	     $GCAL; into the private `my_argv[]' table.
	   */
	  while (isspace (*ptr_char))
	    ptr_char++;
	  i = 0;
	  while (*ptr_char)
	    {
	      if ((Uint) i >= maxlen_max)
		resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				    (long) __LINE__);
	      s1[i++] = *ptr_char++;
	      if (!*ptr_char || isspace (*ptr_char))
		{
		  s1[i] = '\0';
		  if (*s1 == *SWITCH || *s1 == *SWITCH2
# if USE_RC
		      || *s1 == RC_ADATE_CHAR
# endif
		      || *s1 == RSP_CHAR)
		    {
		      /*
		         Avoid one or two letter combinations of '-', '/', '%' or '@' characters only!
		       */
		      if (i == 1 || ((i == 2) && (s1[1] == *SWITCH
# if USE_RC
						  || s1[1] == RC_ADATE_CHAR
# endif
						  || s1[1] == *SWITCH2)))
			;	/* Void, don't allocate memory */
		      else
			{
			  if (*s1 == RSP_CHAR)
			    {
			      /*
			         Try to manage a response file @FILE argument given
			         in the environment variable $GCAL.
			       */
			      rsp_filename = (char *) my_malloc (strlen (s1),
								 ERR_NO_MEMORY_AVAILABLE,
								 __FILE__,
								 ((long)
								  __LINE__) -
								 2L,
								 "rsp_filename",
								 0);
			      strcpy (rsp_filename, s1 + 1);
			      fp =
				file_open (&rsp_filename, 0, REsponse,
					   &b_dummy);
			      if (fp != (FILE *) NULL)
				{
				  my_argv =
				    insert_response_file (fp, rsp_filename,
							  these_short_opts_need_args,
							  &my_argc_max,
							  &my_argc, my_argv);
				  (void) fclose (fp);
				}
			      free (rsp_filename);
			      rsp_filename = (char *) NULL;
			    }
			  else
			    {
			      /*
			         Ok, argument is an option or an actual date modifier %DATE.
			       */
			      if ((*s1 == *SWITCH
				   || *s1 == *SWITCH2) && (s1[1] != *SWITCH))
				{
				  /*
				     Short-style option found.
				   */
				  if ((strchr
				       (these_short_opts_need_args,
					s1[1]) != (char *) NULL) && !s1[2])
				    {
				      /*
				         Short-style option requires an argument, which is separated by
				         one or more whitespace characters from the option character;
				         respect this!  This means all separating whitespace characters
				         between the option character and argument will be eliminated.
				       */
				      while (isspace (*ptr_char))
					/*
					   Skip the separating whitespace characters.
					 */
					ptr_char++;
				      /*
				         Store needed argument of option adjacent to option character.
				       */
				      while (*ptr_char
					     && !isspace (*ptr_char))
					{
					  if ((Uint) i >= maxlen_max)
					    resize_all_strings (maxlen_max <<
								1, FALSE,
								__FILE__,
								(long)
								__LINE__);
					  s1[i++] = *ptr_char++;
					}
				      s1[i] = '\0';
				    }
				}
			      if ((Uint) my_argc >= my_argc_max)
				{
				  /*
				     Resize the `my_argv[]' table.
				   */
				  my_argc_max <<= 1;
				  if (my_argc_max * sizeof (char *) > testval)
				    my_argc_max--;
				  my_argv =
				    (char **) my_realloc ((VOID_PTR) my_argv,
							  my_argc_max *
							  sizeof (char *),
							  ERR_NO_MEMORY_AVAILABLE,
							  __FILE__,
							  ((long) __LINE__) -
							  3L,
							  "my_argv[my_argc_max]",
							  my_argc_max);
				}
			      my_argv[my_argc] = (char *) my_malloc (i + 1,
								     ERR_NO_MEMORY_AVAILABLE,
								     __FILE__,
								     ((long)
								      __LINE__)
								     - 2L,
								     "my_argv[my_argc]",
								     my_argc);
			      strcpy (my_argv[my_argc++], s1);
			    }
			}
		      while (isspace (*ptr_char))
			ptr_char++;
		      i = 0;
		    }
		  else if (i)
		    {
		      /*
		         Error, argument is a command.
		       */
		      fprintf (stderr,
			       _
			       ("%s: command in environment variable `%s' found -- %s\n%s\n%s\n"),
			       prgr_name, ENV_VAR_GCAL, s1, usage_msg (),
			       lopt_msg ());

		      exit (ERR_INVALID_OPTION);
		    }
		}
	    }
	}
    }
#endif /* !AMIGA || __GNUC__ */
  /*
     Now insert/append the "arguments" of command line
     into the private `my_argv[]' table.
   */
  while (argc > 1)
    {
      argv++;
      /*
         Preload the contents of response files and don't place this option
         text itself in private `my_argv[]' command line options table!
       */
      if (**argv == RSP_CHAR)
	{
	  /*
	     Try to manage a response file @FILE argument given in the command line.
	   */
	  rsp_filename = (char *) my_malloc (strlen (*argv),
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 2L,
					     "rsp_filename", 0);
	  strcpy (rsp_filename, *argv + 1);
	  fp = file_open (&rsp_filename, 0, REsponse, &b_dummy);
	  if (fp != (FILE *) NULL)
	    {
	      my_argv =
		insert_response_file (fp, rsp_filename,
				      these_short_opts_need_args,
				      &my_argc_max, &my_argc, my_argv);
	      (void) fclose (fp);
	    }
	  free (rsp_filename);
	  rsp_filename = (char *) NULL;
	}
      else
	{
	  register int len = (int) strlen (*argv);


	  /*
	     Avoid one or two letter combinations of '-', '/' or '%' characters only!
	   */
	  if (((len == 1) && (**argv == *SWITCH
#if USE_RC
			      || **argv == RC_ADATE_CHAR
#endif
			      || **argv == *SWITCH2))
	      || ((len == 2) && (**argv == *SWITCH
#if USE_RC
				 || **argv == RC_ADATE_CHAR
#endif
				 || **argv == *SWITCH2)
		  && (*(*argv + 1) == *SWITCH
#if USE_RC
		      || *(*argv + 1) == RC_ADATE_CHAR
#endif
		      || *(*argv + 1) == *SWITCH2)))
	    ;			/* Void, don't allocate memory */
	  else
	    {
	      if ((Uint) my_argc >= my_argc_max)
		{
		  /*
		     Resize the `my_argv[]' table.
		   */
		  my_argc_max <<= 1;
		  if (my_argc_max * sizeof (char *) > testval)
		    my_argc_max--;
		  my_argv = (char **) my_realloc ((VOID_PTR) my_argv,
						  my_argc_max *
						  sizeof (char *),
						  ERR_NO_MEMORY_AVAILABLE,
						  __FILE__,
						  ((long) __LINE__) - 3L,
						  "my_argv[my_argc_max]",
						  my_argc_max);
		}
	      my_argv[my_argc] = (char *) my_malloc (len + 1,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 2L,
						     "my_argv[my_argc]",
						     my_argc);
	      strcpy (my_argv[my_argc++], *argv);
	    }
	}
      argc--;
    }
  /*
     Building of private `my_argv[]' table is done, so check first whether
     warning/debug option (`--debug[=0...WARN_LVL_MAX]') is given (needed
     to set correct warning level in case global date variable definitions
     follow) and the command execution option (`--execute-command') is
     given, and then check whether global date variable definitions `-v<>'
     (or global text variable definitions `-r<>') are given, so we can
     reference them in the actual date modifier, e.g.,
     %YYYY@DVAR[[-]N[WW[W]]].
   */
  if (my_argc > 1)
    {
      /*
         Now it's time to rearrange the `my_argv[]' table, so all short-style
         options with needed argument like `-X foo' are concatenated to `-Xfoo'.
       */
      rearrange_argv (these_short_opts_need_args, &my_argc, my_argv);
      i = 1;
      while (i < my_argc)
	{
	  ptr_char = my_argv[i];
	  if (*ptr_char == *SWITCH || *ptr_char == *SWITCH2)
	    {
	      ptr_char++;
	      if (*ptr_char == *SWITCH)
		{
		  /*
		     Long-style option given:
		     Convert it to an according short-style option.
		   */
		  ptr_char++;
		  /*
		     Copy this long-style option into a buffer.
		   */
		  j = (int) strlen (ptr_char);
		  if ((Uint) j >= maxlen_max)
		    resize_all_strings (j + 1, FALSE, __FILE__,
					(long) __LINE__);
		  strcpy (s3, ptr_char);
		  /*
		     Check this long-style option.
		   */
		  if (!eval_longopt (s3, &lopt_id))
		    ptr_char = s3;
		  else
		    /*
		       Error in given long-style option detected:
		       Skip this long-style option only!
		     */
		    ptr_char = s3 + strlen (s3);
		}
	      else
		lopt_id = SYM_NIL;
	      if (lopt_id == SYM_DEBUG)
		warning_level = my_atoi (ptr_char);
#if USE_RC
	      else if (lopt_id == SYM_EXECUTE_COMMAND)
		rc_execute_command = TRUE;
	      else if (*ptr_char == 'r')
		{
		  ptr_char++;
		  /*
		     Global text variable definition found
		     (e.g.: -r$a=TEXT1:$b=TEXT2:$c=$a...):
		     Try to scan this argument and store found TEXTs
		     into the text variable table.
		   */
		  while (*ptr_char)
		    {
		      /*
		         Split the SEP (colon) separated list of text variables.
		       */
		      j = 0;
		      while (*ptr_char && (*ptr_char != *SEP))
			{
			  if ((Uint) j >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  s1[j++] = *ptr_char++;
			  /*
			     Take a quoted SEP (colon) separator as is,
			     but remove the quote character.
			   */
			  if ((*ptr_char == *SEP)
			      && (s1[j - 1] == QUOTE_CHAR))
			    s1[j - 1] = *ptr_char++;
			}
		      s1[j] = '\0';
		      pseudo_blank_conversion (&s1);
		      set_tvar (s1, _("Internal"), 0L, GLobal);
		      if (*ptr_char)
			ptr_char++;
		    }
		}
	      else if (*ptr_char == 'v')
		{
		  ptr_char++;
		  /*
		     Global text variable definition found
		     (e.g.: -va=1227:b=0514:c=a...):
		     Try to scan this argument and store found MMDD
		     dates into the date variable table.
		   */
		  while (*ptr_char)
		    {
		      /*
		         Split the SEP (colon) separated list of date variables.
		       */
		      j = 0;
		      while (*ptr_char && (*ptr_char != *SEP))
			{
			  if ((Uint) j >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  s1[j++] = *ptr_char++;
			}
		      s1[j] = '\0';
		      set_dvar (s1, lineptrs, _("Internal"), 0L, GLobal);
		      if (*ptr_char)
			ptr_char++;
		    }
		}
#endif /* USE_RC */
	    }
	  /*
	     Stop processing if the first command is found.
	   */
	  if ((*my_argv[i] != *SWITCH) && (*my_argv[i] != *SWITCH2)
#if USE_RC
	      && (*my_argv[i] != RC_ADATE_CHAR)
#endif
	      && (*my_argv[i] != RSP_CHAR))
	    break;
	  i++;
	}
    }
#if USE_RC
  /*
     Check whether an actual date modifier %DATE is given.
   */
  if (my_argc > 1)
    {
      i = 1;
      while (i < my_argc)
	{
	  if (*my_argv[i] == RC_ADATE_CHAR)
	    rc_adate = my_argv[i] + 1;
	  /*
	     Stop processing if the first command is found.
	   */
	  if ((*my_argv[i] != *SWITCH)
	      && (*my_argv[i] != *SWITCH2) && (*my_argv[i] != RC_ADATE_CHAR))
	    break;
	  i++;
	}
    }
#endif /* USE_RC */
#ifdef GCAL_EPAGER
  if (is_tty)
    {
      /*
         Try to detect the external pager program in a safe way.  This
         means first to detect whether a $PAGER environment variable is set,
         if so, try to use this pager program, otherwise try to use one of
         the burned-in pager names of `ext_pager[]' table.
       */
      ptr_char = getenv (ENV_VAR_PAGER);
      if (ptr_char != (char *) NULL)
	{
	  if (*ptr_char)
	    {
	      while (isspace (*ptr_char))
		ptr_char++;
	      i = (int) strlen (ptr_char);
	      if ((Uint) i >= maxlen_max)
		resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	      strcpy (s3, ptr_char);
	      if (*ptr_char != *DIR_SEP)
		{
		  ptr_char = strrchr (s3, *DIR_SEP);
		  if (ptr_char != (char *) NULL)
		    ptr_char++;
		  else
		    ptr_char = s3;
		}
	      j = strlen (ptr_char);
	      ext_pager = (char *) my_malloc (j + 1,
					      ERR_NO_MEMORY_AVAILABLE,
					      __FILE__,
					      ((long) __LINE__) - 2L,
					      "ext_pager", 0);
	      strcpy (ext_pager, ptr_char);
	      i = 0;
	      while (ext_pager[i] && !isspace (ext_pager[i]))
		i++;
	      ext_pager[i] = '\0';
	      if (*ext_pager == *DIR_SEP)
		{
		  /*
		     Absolute pager name given, try to get this executable directly.
		   */
# if HAVE_SYS_STAT_H && defined(S_IFMT) && defined(S_IFREG)
		  auto struct stat statbuf;


		  /*
		     Test if the file is a regular file, if not, ignore it!
		   */
		  if (!stat (ext_pager, &statbuf))
		    if ((statbuf.st_mode & S_IFMT) == S_IFREG)
		      fp = fopen (ext_pager, "r");
# else /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
		  fp = fopen (ext_pager, "r");
# endif	/* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
		}
	      else
		/*
		   Relative pager name given, search this executable
		   using the $PATH environment variable.
		 */
		fp = file_open (&ext_pager, 0, COmmon, &b_dummy);
	      if (fp == (FILE *) NULL)
		{
		  free (ext_pager);
		  ext_pager = (char *) NULL;
		}
	      else
		{
		  (void) fclose (fp);
		  if (i != j)
		    {
		      register Uint pg_argc_max = MY_ARGC_MAX;
		      register int pg_argc = 0;
		      register int k = j;


		      /*
		         Initial memory allocation for the `pg_argv[]' table.
		       */
		      pg_argv =
			(char **) my_malloc (MY_ARGC_MAX * sizeof (char *),
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 2L,
					     "pg_argv[MY_ARGC_MAX]",
					     MY_ARGC_MAX);
		      /*
		         Now copy the name of the $PAGER executable into
		         `pg_argv[]' table at position 0;
		       */
		      pg_argv[pg_argc] = (char *) my_malloc (i + 1,
							     ERR_NO_MEMORY_AVAILABLE,
							     __FILE__,
							     ((long) __LINE__)
							     - 2L,
							     "pg_argv[pg_argc]",
							     pg_argc);
		      strcpy (pg_argv[pg_argc++], ext_pager);
		      /*
		         Now copy the command line arguments which are delivered
		         to the $PAGER executable into `pg_argv[]' table at
		         position `pg_argc'.
		       */
		      j = i + 1;
		      LOOP
		      {
			if ((Uint) pg_argc >= pg_argc_max)
			  {
			    /*
			       Resize the `pg_argv[]' table.
			     */
			    pg_argc_max <<= 1;
			    if (pg_argc_max * sizeof (char *) > testval)
			      pg_argc_max--;
			    pg_argv =
			      (char **) my_realloc ((VOID_PTR) pg_argv,
						    pg_argc_max *
						    sizeof (char *),
						    ERR_NO_MEMORY_AVAILABLE,
						    __FILE__,
						    ((long) __LINE__) - 3L,
						    "pg_argv[pg_argc_max]",
						    pg_argc_max);
			  }
			while (isspace (ext_pager[j]))
			  j++;
			if (!ext_pager[j])
			  break;
			ptr_char = ext_pager + j;
			while (*ptr_char && !isspace (*ptr_char))
			  ptr_char++;
			if (!*ptr_char)
			  ptr_char = ext_pager + k;
			i = ptr_char - (ext_pager + j);
			if (!i)
			  break;
			*ptr_char = '\0';
			pg_argv[pg_argc] = (char *) my_malloc (i + 1,
							       ERR_NO_MEMORY_AVAILABLE,
							       __FILE__,
							       ((long)
								__LINE__) -
							       2L,
							       "pg_argv[pg_argc]",
							       pg_argc);
			strcpy (pg_argv[pg_argc++], ext_pager + j);
			j += i;
			if (j >= k)
			  break;
			j++;
		      }
		      /*
		         And terminate the `pg_argv[]' table by a final NULL element.
		       */
		      if ((Uint) pg_argc >= pg_argc_max)
			{
			  /*
			     Resize the `pg_argv[]' table.
			   */
			  pg_argc_max <<= 1;
			  if (pg_argc_max * sizeof (char *) > testval)
			    pg_argc_max--;
			  pg_argv = (char **) my_realloc ((VOID_PTR) pg_argv,
							  pg_argc_max *
							  sizeof (char *),
							  ERR_NO_MEMORY_AVAILABLE,
							  __FILE__,
							  ((long) __LINE__) -
							  3L,
							  "pg_argv[pg_argc_max]",
							  pg_argc_max);
			}
		      pg_argv[pg_argc] = (char *) NULL;
		    }
		}
	    }
	}
      if (ext_pager == (char *) NULL)
	{
	  i = 0;
	  while (pagers[i] != (char *) NULL)
	    {
	      ext_pager = (char *) my_malloc (strlen (pagers[i]) + 1,
					      ERR_NO_MEMORY_AVAILABLE,
					      __FILE__,
					      ((long) __LINE__) - 2L,
					      "ext_pager", 0);
	      strcpy (ext_pager, pagers[i]);
	      fp = file_open (&ext_pager, 0, COmmon, &b_dummy);
	      if (fp != (FILE *) NULL)
		{
		  (void) fclose (fp);
		  break;
		}
	      free (ext_pager);
	      ext_pager = (char *) NULL;
	      i++;
	    }
	}
      /*
         If we have found an external pager program, we'll use it!
       */
      if (ext_pager != (char *) NULL)
	{
	  /*
	     We work unbuffered in this case so we have to flush the stdout buffer first.
	   */
	  fflush (stdout);
	  /*
	     Open an unnamed pipe.
	   */
	  if (pipe (pipe_fd) < 0)
	    /*
	       Error, `pipe()' function failed.
	     */
	    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
		      ((long) __LINE__) - 4L, "pipe()<", 0);
# if HAVE_SIGNAL && defined(SIGPIPE)
	  /*
	     Ignore the SIGPIPE signal.
	   */
	  (void) signal (SIGPIPE, SIG_IGN);
# endif
	}
    }
#endif /* GCAL_EPAGER */
  /*
     Check and evaluate the command line arguments and detect,
     whether a list/range of extended years or other lists/ranges of
     months/years are wanted.
   */
  check_command_line (my_argc, my_argv);
  /*
     If an invalid date format is found, exit the program with error message!
   */
  if (errtxt_dformat != (char *) NULL)
    my_error (ERR_INVALID_DATE_FORMAT, errtxt_dformat, 0,
	      date_format->df_format, 0);
#ifdef GCAL_EMAIL
  /*
     In case Gcal's output must be send to a user via eMail:
     Create and open a temporary file.
   */
  if (email_adr != (char *) NULL)
    {
      ptr_char = TMPFILENAME;
      if (ptr_char == (char *) NULL)
	my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
		  ((long) __LINE__) - 2L, "tmpnam()=", 0);
      tfn =
	(char *) my_malloc (strlen (ptr_char) + 1, ERR_NO_MEMORY_AVAILABLE,
			    __FILE__, ((long) __LINE__) - 2L, "tfn", 0);
      strcpy (tfn, ptr_char);
      tfp = fopen (tfn, "w");
      if (tfp == (FILE *) NULL)
	my_error (ERR_WRITE_FILE, __FILE__, ((long) __LINE__) - 2L, tfn, 0);
      is_tty = (int) FALSE;
    }
#endif /* GCAL_EMAIL */
  /*
     Do we have to suppress the output of a month calendar in case only
     the `-n|N[-]' option (display eternal holiday list) is found and/or
     only the `-c[]' option (display fixed dates list) is found?
   */
  if (!year && !month && (holiday_flag
#if USE_RC
			  || rc_use_flag
#endif
      ) && !year_flag)
    show_calendar = FALSE;
  else
    /*
       It's sure that we have to suppress a month/year calendar sheet,
       because the `-u' option is given in the command line!
       If NO eternal holiday list option (`-n|N[-]') AND NO fixed dates
       list option (`-c|C[]') is given, we have to avoid the call
       of the calendar sheet creating function `print_calendar()';
       this case is marked by setting `show_calendar' to FALSE!
     */
    if (suppr_cal_flag
#if USE_RC
	&& !rc_use_flag
#endif
	&& !holiday_flag)
    show_calendar = FALSE;
#if USE_RC
  /*
     Check whether an explicit date is given in the command line.
   */
  is_date_given = (Bool) (month || year || is_fiscal_year);
  if (rc_use_flag && ((!month && year) || is_fiscal_year))
    date_enables_year = TRUE;
  if (rc_filename != (char *) NULL)
    pseudo_blank_conversion (&rc_filename);
  if (rc_filter_text != (char *) NULL)
    pseudo_blank_conversion (&rc_filter_text);
#endif /* USE_RC */
  /*
     Check whether the arguments of command line are valid.
   */
  eliminate_invalid_data ();
  /*
     Get the highlighting sequences of the terminal.
   */
  get_tty_hls (hl_seq);
#if USE_PAGER
  /*
     No program output yet, so if the paging option is wanted:
     Set the standard stream buffers to unbuffered mode
     and get actual settings of the tty.
   */
  if (is_tty1 && is_tty2
# ifdef GCAL_EPAGER
      && (ext_pager == (char *) NULL)
# endif
      && pager_flag)
    {
      setbuf (stdout, (char *) NULL);
      setbuf (stdin, (char *) NULL);
      get_tty_scr_size (&tty_rows, &tty_cols);
    }
#endif /* USE_PAGER */
#if USE_RC
  /*
     If any `-# ARG' resp., `--here=ARG' options are found:
     Close temporary file which is used for managing all "here" options
     (it contains all "here" lines now) so it can be used at the end
     of each resource file processing step/cycle.
   */
  if (rc_here_fn != (char *) NULL)
    if (fclose (rc_here_fp) == EOF)
      my_error (ERR_WRITE_FILE, __FILE__, ((long) __LINE__) - 1L, rc_here_fn,
		0);
#endif
  /*
     Log contents of command line:
     I.e. check whether a response file must be written.
   */
  if (rsp_filename != (char *) NULL)
    write_log_file (rsp_filename, REsponse, _("response file"), _("Created"),
		    my_argc, my_argv);
#ifdef GCAL_SHELL
  /*
     Log contents of command line:
     i.e. check whether a shell script must be written
   */
  if (shl_filename != (char *) NULL)
    write_log_file (shl_filename, SCript, _("shell script"), _("Created"),
		    my_argc, my_argv);
#endif /* GCAL_SHELL */
#ifdef GCAL_EPAGER
  if (is_tty1
      && is_tty2
      && pager_flag
      && (ext_pager != (char *) NULL) && (email_adr == (char *) NULL))
    {
      child_pid = fork ();
      switch (child_pid)
	{
	case -1:
	  /*
	     Error, `fork()' function failed.
	   */
	  my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
		    ((long) __LINE__) - 7L, "fork() child_pid=", child_pid);
	  /* Not reached. */
	case 0:
	  /*
	     Child process (read from pipe):
	     Connect PIPE-STDIN to SYS-STDIN in a safe way
	     and launch the external pager program.
	   */
	  close (pipe_fd[1]);
	  if (pipe_fd[0])
	    {
	      close (0);
	      dup (pipe_fd[0]);
	      close (pipe_fd[0]);
	    }
	  if (pg_argv == (char **) NULL)
	    {
	      i = execlp (ext_pager, ext_pager, (char *) NULL);
	      /*
	         Error, `execlp()' function failed
	         (this line should never be executed)!
	       */
	      j = (int) strlen (ext_pager) + LEN_SINGLE_LINE;
	      if ((Uint) j >= maxlen_max)
		resize_all_strings (j + 1, FALSE, __FILE__, (long) __LINE__);
	      sprintf (s1, "execlp(%s)=", ext_pager);
	      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			((long) __LINE__) - 9L, s1, i);
	    }
	  else
	    {
	      i = execvp (ext_pager, pg_argv);
	      /*
	         Error, `execvp()' function failed
	         (this line should never be executed)!
	       */
	      j = (int) strlen (ext_pager) + LEN_SINGLE_LINE;
	      if ((Uint) j >= maxlen_max)
		resize_all_strings (j + 1, FALSE, __FILE__, (long) __LINE__);
	      sprintf (s1, "execvp(%s)=", ext_pager);
	      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			((long) __LINE__) - 9L, s1, i);
	    }
	  /* Not reached. */
	default:
	  /*
	     Parent process (write to pipe):
	     Connect PIPE-stdout to SYS-stdout in a safe way
	     and initiate the action.
	   */
	  close (pipe_fd[0]);
	  if (pipe_fd[1] != 1)
	    {
	      close (1);
	      if (!dup (pipe_fd[1]))
		{
		  dup (pipe_fd[1]);
		  close (0);
		}
	      close (pipe_fd[1]);
	    }
	}
    }
#endif /* GCAL_EPAGER */
  /*
     And now display the calendar sheet(s)!
   */
  if (show_calendar)
    print_calendar ();
  else
    /*
       Only the `-n|N[-]' option (display eternal holiday list) is found
       and/or only the `-c[]' option (display fixed dates) is found,
       display these list(s) without/omitting a leading month calendar.
     */
    if (holiday_flag
#if USE_RC
	|| rc_use_flag
#endif
    )
    {
      register int tmp_ad = act_day;


      is_leap_year = (days_of_february (year) == 29);
      if (is_fiscal_year)
	fiscal_month = (*month_list).ml_month;
      if (cal_special_flag)
	act_day = day_of_year (tmp_ad, act_month, act_year);
      if (((year == EASTER_MIN - 1)
	   && (fiscal_month > MONTH_MIN))
	  || ((year >= EASTER_MIN) && (year <= EASTER_MAX)))
	print_all_holidays (FALSE, TRUE);
      if (cal_special_flag)
	act_day = tmp_ad;
    }
  /*
     If the simple month/year mode is active...
   */
  if (!month_list[1].ml_month)
    {
      if (is_fiscal_year || (holiday_flag && !show_calendar))
	month = 0;
#if USE_RC
      if (rc_use_flag)
	rc_use ();
#endif
      if (holiday_flag)
	{
	  if (((year == EASTER_MIN - 1)
	       && (fiscal_month == 1))
	      || year < EASTER_MIN - 1 || year > EASTER_MAX)
	    /*
	       Error, invalid year given for computing Easter Sunday's date.
	     */
	    my_error (ERR_INVALID_EASTER_DATE, "", 0L, "", 0);
	  print_all_holidays (FALSE, FALSE);
	}
    }
#ifdef GCAL_EPAGER
  if (is_tty1
      && is_tty2
      && pager_flag
      && (ext_pager != (char *) NULL) && (email_adr == (char *) NULL))
    {
      /*
         We work unbuffered in this case so we have to flush the stdout buffer
         for showing all contents of it.
       */
      fflush (stdout);
      /*
         And reconnect the SYS-STDIN/SYS-STDOUT file descriptors.
       */
      close (0);
      dup (sys_fd[0]);
      close (1);
      dup (sys_fd[1]);
      /*
         And wait until the child has performed all actions.
       */
      while (wait ((int *) NULL) != child_pid)
	;
# if HAVE_SIGNAL && defined(SIGPIPE)
      /*
         Reset the SIGPIPE signal.
       */
      (void) signal (SIGPIPE, SIG_DFL);
# endif
    }
#endif /* GCAL_EPAGER */
#ifdef GCAL_EMAIL
  /*
     If the mailing option is selected:
     Close temporary file, redirect it to mailer and then erase it.
   */
  if (email_adr != (char *) NULL)
    {
      if (fclose (tfp) == EOF)
	my_error (ERR_WRITE_FILE, __FILE__, ((long) __LINE__) - 1L, tfn, 0);
      else
	{
	  /*
	     Check whether the temporary file is "empty" (zero contents).
	   */
# if HAVE_SYS_STAT_H
	  auto struct stat statbuf;


	  tfp = (FILE *) NULL;
	  /*
	     Get the size of the temporary file.
	     If it's not accessable, report a serious error.
	   */
	  if (!stat (tfn, &statbuf))
	    i = (statbuf.st_size > 0);
	  else
	    my_error (ERR_READ_FILE, __FILE__, ((long) __LINE__) - 3L, tfn,
		      0);
# else /* !HAVE_SYS_STAT_H */
	  i = 0;
	  tfp = fopen (tfn, "r");
	  if (tfp == (FILE *) NULL)
	    my_error (ERR_READ_FILE, __FILE__, ((long) __LINE__) - 2L, tfn,
		      0);
	  i = fgetc (tfp);
	  if (fclose (tfp) == EOF)
	    my_error (ERR_READ_FILE, __FILE__, ((long) __LINE__) - 1L, tfn,
		      0);
	  tfp = (FILE *) NULL;
	  i = (i != EOF);
# endif	/* !HAVE_SYS_STAT_H */
	}
      if (i)
	{
	  sprintf (s2, _("Mail from \\`%s' (%02d-%s-%04d %02d%s%02d%s%02d"),
		   prgr_name, true_day, short_month_name (true_month),
		   true_year, act_hour, time_sep, act_min, time_sep, act_sec);
	  if (tz != (char *) NULL)
	    {
	      sprintf (s1, " %s", tz);
	      strcat (s2, s1);
	    }
	  strcat (s2, ")");
# if !defined(AMIGA) || defined(__GNUC__)
	  /*
	     Detect whether a $MAILPROG environment variable is set.
	   */
	  ptr_char = getenv (ENV_VAR_MAILPROG);
	  if (ptr_char != (char *) NULL)
	    {
	      if (!*ptr_char)
		ptr_char = MAIL_PRGR;
	    }
	  else
# endif	/* !AMIGA || __GNUC__ */
	    ptr_char = MAIL_PRGR;
	  i =
	    (int) strlen (ptr_char) + strlen (s2) + strlen (email_adr) +
	    strlen (REDIRECT_IN) + 9;
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	  sprintf (s1, "%s -s \"%s\" %s %s %s", ptr_char, s2, email_adr,
		   REDIRECT_IN, tfn);
	  /*
	     And send the eMail to the given `email_adr' NOW.
	   */
	  if (my_system (s1))
	    {
	      /*
	         If the "shell" reports a MAIL exit status not equal 0, first try
	         to erase the temporary file (if this results to an error, this
	         error has a higher preference than pointing out only that the
	         mail program has failed, see the next comment to this a few lines
	         below), and only then, terminate Gcal with error message.
	       */
	      i = unlink (tfn);
	      if (i)
		my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			  ((long) __LINE__) - 2L, "unlink(tfn)=", i);
	      my_error (ERR_EMAIL_SEND_FAILURE, __FILE__,
			((long) __LINE__) - 13L, email_adr, 0);
	    }
	}
      else
	fprintf (stderr,
		 _
		 ("%s: warning, eMail with empty message body not sent to <%s>.\n"),
		 prgr_name, email_adr);

      i = unlink (tfn);
      if (i)
	/*
	   Don't ignore any errors, because NOT being able to erase a non-write
	   protected regular file we have created just some [milli]seconds
	   before is a serious error that DOES produce trouble in future,
	   because this is a detection of a bad block of the storage media
	   in most cases!
	 */
	my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
		  ((long) __LINE__) - 9L, "unlink(tfn)=", i);
    }
#endif /* GCAL_EMAIL */
#if USE_RC
  /*
     Erase temporary file which is used for managing all `-# ARG' resp.,
     `--here=ARG' options if all processing is done.
   */
  if (rc_here_fn != (char *) NULL)
    {
      i = unlink (rc_here_fn);
      if (i)
	/*
	   Don't ignore any errors, because NOT being able to erase a non-write
	   protected regular file we have created some time before is a
	   serious error that DOES produce trouble in future, because this
	   is a detection of a bad block of the storage media in most cases!
	 */
	my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
		  ((long) __LINE__) - 8L, "unlink(rc_here_fn)=", i);
    }
  if (rc_use_flag
      && !shell_escape_done
      && (!rc_elems || (rc_zero_dates_flag && !(rc_elems - rc_zero_pos))))
    exit (ERR_NO_FIXED_DATES_LISTED);
#endif

  exit (EXIT_SUCCESS);
}



int
eval_longopt (longopt, longopt_symbolic)
     char *longopt;
     int *longopt_symbolic;
/*!
   Evaluates a long option and returns -2...0 if success, +1...+6 if fails:
     -2 == Given `longopt' successfully parsed and completed `long_name' with argument returned.
     -1 == Given `longopt' successfully parsed and completed `long_name' returned only.
      0 == Given `longopt' successfully parsed and corresponding `short_name' returned.
     +1 == Given `longopt' is ambiguous (not distinguishable).
     +2 == Given `longopt' is unknown.
     +3 == Given `longopt' requires no argument (completed `long_name' returned).
     +4 == Given `longopt' requires an argument
     +5 == Given `longopt' with invalid argument.
     +6 == Given `longopt' with ambiguous argument.
   This function `abort()'s internally if no SPECIAL longoption argument
     is defined in the `lopt[]' table for a LARG_ONE_OR_ARG mode, but one
     or some `short_name' entries are in it!  Such a case is illegal!
   If success and `short_name[0]' isn't set to NULL, the delivered `longopt'
     is converted to an according `short_name' (UN*X-style) option and returned,
     otherwise `longopt' is returned unmodified.
   If success and `short_name[0]' is set to NULL, return the completed
     `long_name' (with possibly trailing arguments) instead.
   `&longopt_symbolic' is either set to SYM_NIL if given `longopt' isn't member
     of field `long_name' in `lopt[]' table or to the according SYM_???.
*/
{
  auto const Lopt_struct *ptr_lopt = lopt;


  *longopt_symbolic = SYM_NIL;
  if (tolower (*longopt) >= tolower (*ptr_lopt->long_name))
    {
      /*
         Check whether first character of `longopt' can be found
         in `lopt[]' tables structure member `long_name'.
       */
      while (tolower (*ptr_lopt->long_name) < tolower (*longopt))
	{
	  ptr_lopt++;
	  if (ptr_lopt->long_name == NULL)
	    break;
	}
      if (ptr_lopt->long_name != NULL)
	{
	  if (tolower (*ptr_lopt->long_name) == tolower (*longopt))
	    {
	      auto const Lopt_struct *ptr2_lopt = ptr_lopt + 1;
	      register int len_longopt = (int) strlen (longopt);
	      auto Bool larg_sep_found;
	      auto Bool ok = FALSE;


	      if (strchr (longopt, *LARG_SEP) != NULL)
		len_longopt -= (int) strlen (strchr (longopt, *LARG_SEP));
	      /*
	         First character of `longopt' is found in `lopt[]' tables
	         structure member `long_name'; now check for the complete
	         long name or a significant abbreviation of it.
	       */
	      while (!ok && (ptr_lopt->long_name != NULL))
		{
		  if (ptr2_lopt->long_name != NULL)
		    if (!strncasecmp
			(ptr_lopt->long_name, longopt, len_longopt)
			&& !strncasecmp (ptr2_lopt->long_name, longopt,
					 len_longopt))
		      /*
		         Error, `longopt' is ambiguous (not distinguishable).
		       */
		      return (1);
		  if (strncasecmp (ptr_lopt->long_name, longopt, len_longopt))
		    {
		      ptr_lopt++;
		      if (ptr_lopt->long_name != NULL)
			ptr2_lopt++;
		      if ((ptr_lopt->long_name != NULL)
			  && (ptr2_lopt->long_name == NULL))
			if (!strncasecmp
			    (ptr_lopt->long_name, longopt, len_longopt))
			  ok = TRUE;
		    }
		  else
		    ok = TRUE;
		}
	      if (!ok)
		/*
		   Error, `longopt' not found (contains a spelling mistake).
		 */
		return (2);
	      larg_sep_found =
		(Bool) (((int) strlen (longopt) -
			 len_longopt) ? TRUE : FALSE);
	      if (larg_sep_found && (ptr_lopt->larg_mode == LARG_NO))
		{
		  strcpy (longopt, ptr_lopt->long_name);
		  *longopt_symbolic = ptr_lopt->symbolic_name;
		  /*
		     Error, `longopt' requires NO argument.
		   */
		  return (3);
		}
	      if (!larg_sep_found && (ptr_lopt->larg_mode > LARG_NO_OR_ONE))
		{
		  *longopt_symbolic = ptr_lopt->symbolic_name;
		  /*
		     Error, `longopt' requires AN argument.
		   */
		  return (4);
		}
	      /*
	         Valid `longopt' found.
	       */
	      if (ptr_lopt->larg_mode == LARG_NO
		  || ((ptr_lopt->larg_mode == LARG_NO_OR_ONE)
		      && !larg_sep_found))
		{
		  /*
		     `longopt' requires NO argument (e.g. foo).
		     Return the FIRST `short_name' of `lopt[]' table if it's
		     not set to NULL, otherwise return the completed `long_name'.
		   */
		  if (ptr_lopt->short_name[0] == NULL)
		    {
		      strcpy (longopt, ptr_lopt->long_name);
		      *longopt_symbolic = ptr_lopt->symbolic_name;
		      return (-1);
		    }
		  strcpy (longopt, ptr_lopt->short_name[0]);
		}
	      else
		{
		  auto char *ptr_char = longopt + len_longopt + 1;
		  auto char *larg_longopt;


		  /*
		     `longopt' must have ONE trailing argument (e.g. foo=BAR);
		     Return the CORRESPONDING `short_name' stored in `lopt[]' table.
		   */
		  if (!*ptr_char)
		    {
		      *longopt_symbolic = ptr_lopt->symbolic_name;
		      /*
		         Error, `longopt' isn't trailed by an argument.
		       */
		      return (4);
		    }
		  larg_longopt =
		    (char *) my_malloc (strlen (longopt) - len_longopt,
					ERR_NO_MEMORY_AVAILABLE, __FILE__,
					((long) __LINE__) - 2L,
					"larg_longopt", 0);
		  strcpy (larg_longopt, ptr_char);
		  if (ptr_lopt->largs[0] == NULL)
		    {
		      /*
		         Process GIVEN argument.
		       */
		      if (ptr_lopt->short_name[0] == NULL)
			{
			  /*
			     Return the completed `long_name' with GIVEN argument;
			     the format of returned `longopt' is like:  foo=ARG
			   */
			  strcpy (longopt, ptr_lopt->long_name);
			  strcat (longopt, LARG_SEP);
			  strcat (longopt, larg_longopt);
			  *longopt_symbolic = ptr_lopt->symbolic_name;
			  free (larg_longopt);
			  return (-2);
			}
		      if (ptr_lopt->larg_mode == LARG_ONE_OR_ARG)
			/*
			   Internal error, such a mode MUST HAVE one
			   SPECIAL longoption argument minimum in the table!
			 */
			abort ();
		      /*
		         Return the corresponding `short_name' with GIVEN argument;
		         the format of returned `longopt' is like:  fooARG
		       */
		      strcpy (longopt, ptr_lopt->short_name[0]);
		      strcat (longopt, larg_longopt);
		    }
		  else
		    {
		      register int i = 0;


		      /*
		         Process SPECIAL argument given.
		       */
		      ok = FALSE;
		      i = 0;
		      ptr_char = ptr_lopt->largs[i++];
		      while (!ok && (ptr_char != NULL))
			{
			  if (tolower (*ptr_char) == tolower (*larg_longopt))
			    {
			      register int j = i - 1;
			      register int len_larg =
				(int) strlen (larg_longopt);


			      if (!strncasecmp
				  (ptr_lopt->largs[j], larg_longopt,
				   len_larg))
				{
				  if (len_larg ==
				      (int) strlen (ptr_lopt->largs[j]))
				    /*
				       Exact match happened!
				     */
				    ok = TRUE;
				  else
				    {
				      while (!ok
					     && (ptr_lopt->largs[i] != NULL))
					{
					  if (!strncasecmp
					      (ptr_lopt->largs[i],
					       larg_longopt, len_larg)
					      && (len_larg ==
						  (int) strlen (ptr_lopt->
								largs[i])))
					    /*
					       Exact match happened!
					     */
					    ok = TRUE;
					  i++;
					}
				    }
				  if (!ok)
				    {
				      auto Bool ambig = FALSE;


				      /*
				         The given SPECIAL argument doesn't match precisely,
				         so try to detect whether it's ambiguous.
				       */
				      i = j + 1;
				      while (!ambig
					     && (ptr_lopt->largs[i] != NULL))
					{
					  if (!strncasecmp
					      (ptr_lopt->largs[i],
					       larg_longopt, len_larg))
					    ambig = TRUE;
					  i++;
					}
				      if (ambig)
					{
					  *longopt_symbolic =
					    ptr_lopt->symbolic_name;
					  /*
					     Error, the given SPECIAL argument is ambiguous.
					   */
					  return (6);
					}
				      else
					{
					  /*
					     The given SPECIAL argument is valid!
					   */
					  ok = TRUE;
					  i = j + 1;
					}
				    }
				}
			      else
				ptr_char = ptr_lopt->largs[i++];
			    }
			  else
			    ptr_char = ptr_lopt->largs[i++];
			}
		      if (!ok && (ptr_lopt->larg_mode != LARG_ONE_OR_ARG))
			{
			  *longopt_symbolic = ptr_lopt->symbolic_name;
			  /*
			     Error, the given SPECIAL argument not found
			     in `lopt[]' tables structure member `largs'.
			   */
			  return (5);
			}
		      i--;
		      if (ptr_lopt->short_name[0] == NULL)
			{
			  /*
			     Return the completed `long_name' with a completed SPECIAL argument
			     or with a GIVEN argument;
			     the format of returned `longopt' is like:  foo=BAR  or  foo=ARG
			   */
			  strcpy (longopt, ptr_lopt->long_name);
			  strcat (longopt, LARG_SEP);
			  if (ptr_lopt->larg_mode == LARG_ONE_OR_ARG)
			    /*
			       Return with the GIVEN argument.
			     */
			    strcat (longopt, larg_longopt);
			  else
			    /*
			       Return with the completed SPECIAL argument.
			     */
			    strcat (longopt, ptr_lopt->largs[i]);
			  *longopt_symbolic = ptr_lopt->symbolic_name;
			  free (larg_longopt);
			  return (-2);
			}
		      if (!ok && (ptr_lopt->larg_mode == LARG_ONE_OR_ARG))
			{
			  /*
			     Return the corresponding `short_name' with a GIVEN argument;
			     the format of returned `longopt' is like:  fooBAR
			   */
			  strcpy (larg_longopt, longopt + len_longopt + 1);
			  strcpy (longopt, ptr_lopt->short_name[--i]);
			  strcat (longopt, larg_longopt);
			}
		      else
			{
			  /*
			     Return the corresponding `short_name' with a converted SPECIAL argument;
			     the format of returned `longopt' is like:  fooBAR
			   */
			  strcpy (longopt, ptr_lopt->short_name[i]);
			  if (!strcmp (ptr_lopt->largs[i], larg_lit))
			    /*
			       Error, `larg_lit' matched, so return with the GIVEN argument.
			     */
			    strcat (longopt, larg_longopt);
			}
		    }
		  free (larg_longopt);
		}
	    }
	  else
	    /*
	       Error, `longopt' not member of field `long_name' in `lopt[]' table.
	     */
	    return (2);
	}
      else
	/*
	   Error, `longopt' not member of field `long_name' in `lopt[]' table.
	 */
	return (2);
    }
  else
    /*
       Error, first entry in `lopt[]' tables structure member `long_name'
       is greater than the delivered `longopt'.
     */
    return (2);
  *longopt_symbolic = ptr_lopt->symbolic_name;

  return (0);
}



static Bool
is_correct_date_format (format_txt, use_day_suffix, use_short3_day_name,
			use_day_zeroleaded, use_year_zeroleaded)
     char *format_txt;
     Bool *use_day_suffix;
     Bool *use_short3_day_name;
     Bool *use_day_zeroleaded;
     Bool *use_year_zeroleaded;
/*!
   Checks whether the delivered `format_txt' contains valid format directives
     (see the `decode_date_format()' function for a more brief description of
     all used format specifiers).  Returns FALSE in case any invalid format
     specifiers are found, otherwise TRUE.  Boolean `&use_day_suffix' is set
     to TRUE and is returned in case a day GROUP format character defining the
     use of a trailing day suffix is found in `format_txt', otherwise to FALSE.
     Boolean `&use_short3_day_name' is set to TRUE and is returned in case a
     weekday GROUP format character defining the use of a day name truncated to
     three characters WWW is found in `format_txt', otherwise to FALSE.
     Boolean `&use_day_zeroleaded' is set to TRUE and is returned in case a day
     GROUP format character defining the use of a day with leading zeroes is
     found in `format_txt', otherwise to FALSE.  Boolean `&use_year_zeroleaded'
     is set to TRUE and is returned in case a year GROUP format character
     defining the use of a year with leading zeroes is found in `format_txt',
     otherwise to FALSE.
*/
{
  register int start_highlighting = 2;
  register int end_highlighting = 2;
  register int weekday_name_group = 2;
  register int day_group = 2;
  register int month_group = 2;
  register int year_group = 2;
  register int pos;
  auto int fstyle = FSTYLE_NONE;
  auto int fwidth = SPECIAL_VALUE;
  auto Bool is_cformat = FALSE;
  auto Bool is_lformat = FALSE;
  auto Bool is_sign = FALSE;
  auto Bool is_lzero = FALSE;
  auto Bool is_fformat = FALSE;
  auto Bool is_suffix = FALSE;


  *use_day_suffix = (*use_short3_day_name) = (*use_day_zeroleaded) =
    (*use_year_zeroleaded) = FALSE;
  if (!*format_txt)
    /*
       Error, no `format_txt' given.
     */
    return (FALSE);
  do
    {
      while (*format_txt
	     && (*format_txt != DFORMAT_CHAR) && (*format_txt != QUOTE_CHAR))
	format_txt++;
      if (*format_txt)
	{
	  if (*format_txt == DFORMAT_CHAR)
	    {
	      pos = decode_format (format_txt, 1, &is_cformat,
				   &is_lformat, &is_sign, &is_lzero,
				   &is_suffix, &is_fformat, &fstyle, &fwidth);
	      format_txt += pos;
	      /*
	         Format specifier found.
	       */
	      switch (*format_txt)
		{
		case HLS1S_CHAR:
		  if (end_highlighting == 2)
		    if (start_highlighting)
		      start_highlighting--;
		  break;
		case HLS1E_CHAR:
		  if (start_highlighting == 1)
		    if (end_highlighting)
		      end_highlighting--;
		  break;
		case DAYNR_CHAR:
		  if (is_lzero)
		    *use_day_zeroleaded = TRUE;
		  if (is_suffix)
		    *use_day_suffix = TRUE;
		  if (day_group)
		    day_group--;
		  break;
		case WDNAME_CHAR:
		  if (fwidth == 3)
		    *use_short3_day_name = TRUE;
		  if (weekday_name_group)
		    weekday_name_group--;
		  break;
		case MONTHNAME_CHAR:
		case MONTHNR_CHAR:
		  if (month_group)
		    month_group--;
		  break;
		case YEARNR_CHAR:
		  if (is_lzero)
		    *use_year_zeroleaded = TRUE;
		  if (year_group)
		    year_group--;
		  break;
		default:
		  /*
		     Error, invalid or no date format character specified.
		   */
		  return (FALSE);
		}
	    }
	  else
	    /*
	       Quote character found.
	     */
	  if (!*++format_txt)
	    format_txt--;
	  format_txt++;
	}
      else
	break;
    }
  while (*format_txt);
  if (start_highlighting != 1
      || end_highlighting != 1
      || day_group != 1 || month_group != 1 || year_group != 1)
    /*
       Error, either required date format element not given
       or it occurred more than once.
     */
    return (FALSE);
  if (weekday_name_group < 1)
    /*
       Error, optional format element occurred more than once.
     */
    return (FALSE);

  return (TRUE);
}



static void
rearrange_argv (opt_list, argc, argv)
     const char *opt_list;
     int *argc;
     char *argv[];
/*!
   Rearranges `argv[]' internally.
     This means all short-style options which need an argument,
     that is separated by a whitespace character in the command line
     from the short option character, e.g. `-x foo', is concatenated
     to `-xfoo'.  The short option characters which need an argument
     are given in `opt_list'.  This function sets given `&argc' to
     the "new" rearranged amount of arguments stored in `argv[]'.
*/
{
  register int i = (*argc - 1);
  register int n = 1;
  auto char **ptr_argv = argv;
  auto char **ptr2_argv;
  auto Bool is_modified;


  while (i)
    {
      /*
         Skip `argv[0]' generally!
       */
      ptr_argv++;
      is_modified = FALSE;
      if (**ptr_argv == *SWITCH || **ptr_argv == *SWITCH2)
	{
	  /*
	     Found an option in `argv[]'
	     (either a long-style option or a short-style option).
	   */
	  (*ptr_argv)++;
	  if (**ptr_argv && (**ptr_argv != *SWITCH))
	    {
	      /*
	         Short-style option given, e.g. `-x' or `-Xfoo' or `-x bar'.
	       */
	      if (strchr (opt_list, **ptr_argv) != (char *) NULL)
		{
		  /*
		     The option character is found in `opt_list',
		     this means this option needs an argument.
		   */
		  if (i > 1)
		    {
		      /*
		         We are not working on the final option stored in `argv[]'.
		       */
		      (*ptr_argv)++;
		      /*
		         Check if the option doesn't have its argument already
		         unseparated by a whitespace character in the command line,
		         e.g. is not like this `-Xfoo' option.
		       */
		      if (!**ptr_argv)
			{
			  /*
			     Check if the option is trailed by a needed argument,
			     which is the next argument in `argv[]', e.g. `-x bar'.
			   */
			  (*ptr_argv) -= 2;
			  ptr2_argv = ptr_argv;
			  ptr2_argv++;
			  /*
			     Check whether this next argument in `argv[]' is an option.
			   */
			  if ((**ptr2_argv != RSP_CHAR)
#if USE_RC
			      && (**ptr2_argv != RC_ADATE_CHAR)
#endif
			    )
			    {
			      if ((**ptr2_argv == *SWITCH
				   || **ptr2_argv == *SWITCH2)
				  && (strlen (*ptr2_argv) <= 2))
				/*
				   Next argument is definitely a simple short-style option
				   without a given modifier, e.g. `-x' or `/x'.
				 */
				(*ptr_argv)++;
			      else
				{
				  /*
				     Next argument is no option, so let's concatenate the
				     short-style option character and its needed argument.
				   */
				  i--;
				  argv[n] =
				    (char *) my_realloc ((VOID_PTR) (argv[n]),
							 strlen (*ptr_argv) +
							 strlen (*ptr2_argv) +
							 1,
							 ERR_NO_MEMORY_AVAILABLE,
							 __FILE__,
							 ((long) __LINE__) -
							 3L, "argv[n]", n);
				  strcpy (argv[n], *ptr_argv);
				  strcat (argv[n], *ptr2_argv);
				  ptr_argv++;
				  is_modified = TRUE;
				}
			    }
			  else
			    (*ptr_argv)++;
			}
		      else
			(*ptr_argv)--;
		    }
		}
	    }
	}
      else
	(*ptr_argv)++;
      if (!is_modified)
	{
	  /*
	     Unmodified argument found in `argv[]'.
	     This means, the argument we work on here is:
	     either a short-style option, which needs an argument, which isn't given,
	     or     a short-style option, which already contains its argument like `-Xfoo',
	     or     a simple short-style option like `-x',
	     or     a long-style option like `--foo',
	     or     a command (an argument not leaded by a '-', '/', '@' or '%' character.
	   */
	  (*ptr_argv)--;
	  argv[n] = (char *) my_realloc ((VOID_PTR) (argv[n]),
					 strlen (*ptr_argv) + 1,
					 ERR_NO_MEMORY_AVAILABLE,
					 __FILE__, ((long) __LINE__) - 3L,
					 "argv[n]", n);
	  strcpy (argv[n], *ptr_argv);
	}
      n++;
      i--;
    }
  *argc = n;
}



static void
check_command_line (argc, argv)
     int argc;
     char *argv[];
/*!
   Gets and manages the arguments from the command line.
*/
{
  register int opt_error = 0;
  register int len;
  auto int i;
  auto int lopt_help;
  auto char rel_time_offset = '\0';
#if USE_RC
  auto char rel_loop_end = '\0';
#endif
  auto char *ptr_char;
  auto char *option = (char *) NULL;
#if USE_RC
  auto char *rc_period_argv = (char *) NULL;
  auto char *rc_period_option = (char *) NULL;
  auto Bool set_loop_end = FALSE;
  auto Bool rc_period_is_longopt = FALSE;
#endif
  auto Bool is_longopt = FALSE;
  auto Bool license_flag = FALSE;	/* `-L' */
  auto Bool version_flag = FALSE;	/* `-V' */
  auto Bool help_flag = FALSE;	/* `-?' | `-h' */
  auto Bool ext_help_flag = FALSE;	/* `-??' | `-hh' */
  auto Bool help_on_help_flag = FALSE;	/* `--long-help=?' */
  auto Bool skip_option = FALSE;
  auto Bool lopt_ambig = FALSE;


  /*
     Work on the arguments that have been found in the command line.
   */
  while (argc > 1)
    {
      option = *++argv;
      /*
         If a leading switch character is found,
         check the command line for options.
       */
      if (*option == *SWITCH || *option == *SWITCH2)
	{
	  is_longopt = FALSE;
	  /*
	     Check the command line for options.
	   */
	  for (option++; *option; option++)
	    {
	      skip_option = FALSE;
	      /*
	         Check for long-style options, e.g. `--help' ...
	       */
	      if (*option == *SWITCH)
		{
		  option--;
		  if (*option == *SWITCH)
		    {
		      is_longopt = TRUE;
		      option += 2;
		    }
		  if (*option && is_longopt)
		    {
		      /*
		         Copy this long-style option into a buffer.
		       */
		      len = (int) strlen (option);
		      if ((Uint) len >= maxlen_max)
			resize_all_strings (len + 1, FALSE, __FILE__,
					    (long) __LINE__);
		      strcpy (s2, option);
		      /*
		         Check this long-style option.
		       */
		      opt_error = eval_longopt (s2, &lopt_id);
		      if (opt_error < 1)
			{
			  opt_error = 0;
			  switch (lopt_id)
			    {
#if USE_RC
			    case SYM_ADJUST_VALUE:
			      option = strchr (s2, *LARG_SEP) + 1;
			      (void) sscanf (option, "%lf", &adjust_value);
			      /*
			         Avoid nonsense data.
			       */
			      if (abs (adjust_value) > DEGS_PER_06_HOURS)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      break;
			    case SYM_ATMOSPHERE:
			      option = strchr (s2, *LARG_SEP) + 1;
			      ptr_char = strchr (option, *SPLIT_SEP);
			      if (ptr_char == (char *) NULL)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      else
				*ptr_char = '\0';
			      (void) sscanf (option, "%lf", &atm_pressure);
			      if (atm_pressure > 0.0)
				{
				  /*
				     Avoid nonsense data
				     (0.0 mb < pressure <= 1200.0 mb).
				   */
				  if (atm_pressure > 1200.0)
				    /*
				       Error, long-style option is trailed
				       by an invalid argument.
				     */
				    opt_error = 5;
				  else
				    {
				      /*
				         Convert given Millibar (mb) value to a
				         Newton per square meter (Nm^-2) value.
				       */
				      atm_pressure *= 100.0;
				      ptr_char++;
				      if (*ptr_char)
					{
					  (void) sscanf (ptr_char, "%lf",
							 &atm_temperature);
					  /*
					     Avoid nonsense data
					     (-100.0 C <= temperature <= +100.0 C).
					   */
					  if (abs (atm_temperature) > 100.0)
					    /*
					       Error, long-style option is trailed
					       by an invalid argument.
					     */
					    opt_error = 5;
					}
				      else
					/*
					   Error, long-style option is trailed
					   by an invalid argument.
					 */
					opt_error = 5;
				    }
				}
			      else
				opt_error = 0;
			      break;
			    case SYM_EXECUTE_COMMAND:
			      rc_execute_command = TRUE;
			      break;
			    case SYM_extern_static_DVARS:
			      rc_export_ldvar_flag = TRUE;
			      break;
			    case SYM_extern_static_TVARS:
			      rc_export_ltvar_flag = TRUE;
			      break;
			    case SYM_HEADING_TEXT:
			      option = strchr (s2, *LARG_SEP) + 1;
			      rc_heading_text =
				(char *) my_realloc ((VOID_PTR)
						     rc_heading_text,
						     strlen (option) + 1 + 2,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 3L,
						     "rc_heading_text", 0);
			      strcpy (rc_heading_text, "0 ");
			      strcat (rc_heading_text, option);
			      pseudo_blank_conversion (&rc_heading_text);
			      break;
			    case SYM_IGNORE_CASE:
			      rc_ignore_case_flag = TRUE;
			      break;
			    case SYM_LIMIT:
			      rc_limit = TRUE;
			      break;
			    case SYM_LEAP_DAY:
			      rc_feb_29_to_feb_28 = rc_feb_29_to_mar_01 =
				FALSE;
			      if (tolower (*s2) == 'f')
				rc_feb_29_to_feb_28 = TRUE;
			      else
				rc_feb_29_to_mar_01 = TRUE;
			      break;
			    case SYM_PRECISE:
			      rc_precise = TRUE;
			      break;
			    case SYM_REVERT_MATCH:
			      rc_revert_match_flag = TRUE;
			      break;
			    case SYM_BIORHYTHM_AXIS_LEN:
			      option = strchr (s2, *LARG_SEP) + 1;
			      if (*option == '0')
				{
				  while (*option == '0')
				    option++;
				  if (!*option)
				    option--;
				}
			      rc_bio_axis_len = my_atoi (option);
			      if (rc_bio_axis_len > BIO_AXIS_MAX
				  || rc_bio_axis_len < BIO_AXIS_MIN)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      else
				/*
				   Decrease `rc_bio_axis_len' by 1 until
				   it divides BIO_AXIS_MAX without a remainder.
				 */
				while (BIO_AXIS_MAX % rc_bio_axis_len)
				  rc_bio_axis_len--;
			      break;
			    case SYM_MOONIMAGE_LINES:
			      option = strchr (s2, *LARG_SEP) + 1;
			      if (*option == '0')
				{
				  while (*option == '0')
				    option++;
				  if (!*option)
				    option--;
				}
			      rc_moonimage_lines = my_atoi (option);
			      if (rc_moonimage_lines > MOONIMAGE_MAX
				  || rc_moonimage_lines < MOONIMAGE_MIN)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      break;
#endif /* USE_RC */
#ifdef GCAL_EMAIL
			    case SYM_MAIL:
			      option = strchr (s2, *LARG_SEP);
			      if (option == (char *) NULL)
				{
# if !defined(AMIGA) || defined(__GNUC__)
				  /*
				     Detect whether a $MAILTO environment variable is set.
				   */
				  ptr_char = getenv (ENV_VAR_MAILTO);
				  if (ptr_char != (char *) NULL)
				    if (*ptr_char)
				      option = ptr_char;
				  if (option == (char *) NULL)
				    {
				      /*
				         Detect whether a $USER environment variable is set.
				       */
				      ptr_char = getenv (ENV_VAR_USER);
				      if (ptr_char != (char *) NULL)
					if (*ptr_char)
					  option = ptr_char;
				      if (option == (char *) NULL)
					{
					  /*
					     Detect whether a $LOGNAME environment variable is set.
					   */
					  ptr_char = getenv (ENV_VAR_LOGNAME);
					  if (ptr_char != (char *) NULL)
					    if (*ptr_char)
					      option = ptr_char;
					}
				    }
# else /* AMIGA && !__GNUC__ */
				  ;	/* Void, nothing to do now. */
# endif	/* AMIGA && !__GNUC__ */
				}
			      else
				option++;
			      if (option != (char *) NULL)
				if (*option)
				  {
				    if (email_adr == (char *) NULL)
				      email_adr =
					(char *) my_malloc (strlen (option) +
							    1,
							    ERR_NO_MEMORY_AVAILABLE,
							    __FILE__,
							    ((long) __LINE__)
							    - 2L, "email_adr",
							    0);
				    else
				      email_adr =
					(char *) my_realloc ((VOID_PTR)
							     email_adr,
							     strlen (option) +
							     1,
							     ERR_NO_MEMORY_AVAILABLE,
							     __FILE__,
							     ((long) __LINE__)
							     - 3L,
							     "email_adr", 0);
				    strcpy (email_adr, option);
				  }
			      break;
#endif
			    case SYM_DEBUG:
			      break;	/* Void, `--debug[=0...WARN_LVL_MAX]' is already managed in main() */
			    case SYM_EXIT_STAT_HELP_NON_ZERO:
			      exit_stat_help = ERR_EXIT_INFO_TEXTS_NON_ZERO;
			      break;
			    case SYM_ISO_WEEK_NUMBER:
			      if (tolower (*s2) == 'y')
				iso_week_number = TRUE;
			      else
				iso_week_number = FALSE;
			      break;
#if USE_RC
			    case SYM_CYCLE_END:
			    case SYM_CYCLE_STEP:
#endif
			    case SYM_TIME_OFFSET:
			      {
				register int sign = 0;
				register int state = 1;
				register int digits = 0;
				auto Bool skip = FALSE;
				auto Bool is_last = FALSE;
				auto Bool time_sep_found = FALSE;
				auto Bool is_leading_zero = TRUE;


				/*
				   Scan and parse the argument given to the `--cycle-*'
				   and `--time-offset' options if they conform to either
				   the 't'|'@' character or the `[t|@][+|-]MMMM|HH:[MM]'
				   template.  If a 't' character is found, treat
				   the time displacement value relative the
				   actual local time value as given by the system
				   clock.  If a '@' character is found, treat
				   the time displacement value relative the
				   actual gmt value as given by the system
				   clock.  If [+|-]MMMM is given, skip all possibly
				   leading zeroes of MMMM.  The HH:[MM] time
				   separating character ':' is choosen by the
				   used locale at runtime.
				 */
				i = 0;
				switch (lopt_id)
				  {
				  case SYM_TIME_OFFSET:
				    time_hour_offset = time_min_offset = 0;
#if USE_RC
				    break;
				  case SYM_CYCLE_END:
				    loop_end = SPECIAL_VALUE;
				    set_loop_end = FALSE;
				    break;
				  case SYM_CYCLE_STEP:
				    loop_step = DEFAULT_CYCLE_STEP;
#endif
				  }
				option = strchr (s2, *LARG_SEP) + 1;
#if USE_RC
				if (*option == RC_TIME_CHAR
				    || *option == RC_GMTIME_CHAR)
#else /* !USE_RC */
				if (*option == staticTIME_CHAR)
#endif /* !USE_RC */
				  {
				    if (lopt_id == SYM_TIME_OFFSET)
				      rel_time_offset = *option;
#if USE_RC
				    else if (lopt_id == SYM_CYCLE_END)
				      rel_loop_end = *option;
#endif
				    option++;
				  }
				else if (lopt_id == SYM_TIME_OFFSET)
				  rel_time_offset = '\0';
#if USE_RC
				else if (lopt_id == SYM_CYCLE_END)
				  rel_loop_end = '\0';
#endif
				if (*option)
				  {
				    while (*option)
				      {
					skip = FALSE;
					switch (state)
					  {
					  case 1:
					    if (!isdigit (*option))
					      {
						if (*option == *ASC_LIT
						    || *option == *DES_LIT)
						  {
						    if (sign
							|| time_sep_found)
						      state = 0;
						    else
						      sign++;
						    break;
						  }
						else
						  state++;
					      }
					    else
					      {
						if (*option == '0')
						  {
						    if (is_leading_zero)
						      {
							skip = TRUE;
							break;
						      }
						    digits++;
						  }
						else
						  digits++;
						is_leading_zero = FALSE;
						break;
					      }
					    /* Fallthrough. */
					  case 2:
					    if (!digits && !is_leading_zero)
					      state = 0;
					    else if (!time_sep_found)
					      {
						if (*option == *time_sep
						    || *option ==
						    *DEFAULT_TIME_SEP)
						  {
						    if (digits > 2)
						      state = 0;
						    else
						      {
							time_sep_found = TRUE;
							digits = 0;
							state--;
						      }
						  }
						else
						  state = 0;
					      }
					    else
					      {
						if (digits > 4)
						  state = 0;
						else
						  is_last = TRUE;
					      }
					    break;
					  default:
					    state = 0;
					  }
					if (state && !skip)
					  s3[i++] = *option;
					option++;
				      }
				    if (!state
					|| is_last
					|| (i
					    && !time_sep_found
					    && !is_leading_zero
					    && !digits)
					|| (time_sep_found
					    && (digits > 2))
					|| ((state == 1) && (digits > 4)))
				      /*
				         Error, invalid condition occurred.
				       */
				      opt_error = 5;
				    if (i && !opt_error)
				      {
					s3[i] = '\0';
					/*
					   Process the time displacement value.
					 */
					i = atoi (s3);
					if (time_sep_found)
					  {
					    i *= MINS_PER_HOUR;
					    ptr_char = strchr (s3, *time_sep);
					    if (ptr_char == (char *) NULL)
					      ptr_char =
						strchr (s3,
							*DEFAULT_TIME_SEP);
					    if (*++ptr_char)
					      {
						state = atoi (ptr_char);
						if (abs (state) >=
						    MINS_PER_HOUR)
						  opt_error = 5;
						else if (!i)
						  {
						    if (*s3 == *DES_LIT)
						      i = -state;
						    else
						      i = state;
						  }
						else if (SGN (i) < 0)
						  i -= state;
						else
						  i += state;
					      }
					  }
					if (!opt_error)
					  switch (lopt_id)
					    {
					    case SYM_TIME_OFFSET:
					      time_hour_offset =
						MM2HH (abs (i));
					      time_min_offset =
						abs (i) % MINS_PER_HOUR;
					      if (i < 0)
						{
						  time_hour_offset =
						    -time_hour_offset;
						  time_min_offset =
						    -time_min_offset;
						}
#if USE_RC
					      break;
					    case SYM_CYCLE_END:
					      loop_end = i;
					      set_loop_end = TRUE;
					      break;
					    case SYM_CYCLE_STEP:
					      /*
					         Reduce the given cycle-timestep
					         value in minutes to a single day.
					       */
					      if (i <= 0)
						loop_step =
						  DEFAULT_CYCLE_STEP;
					      else if (i >= MINS_PER_DAY)
						loop_step = MINS_PER_DAY - 1;
					      else
						loop_step = i;
#endif
					    }
				      }
				  }
				break;
			      }
			    case SYM_TRANSFORM_YEAR:
			      option = strchr (s2, *LARG_SEP) + 1;
			      if (*option == '0')
				{
				  while (*option == '0')
				    option++;
				  if (!*option)
				    option--;
				}
			      transform_year = atoi (option);
			      if (transform_year > YEAR_MAX
				  || transform_year < -YEAR_MAX)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      break;
			    case SYM_TRANSLATE_STRING:
			      option = strchr (s2, *LARG_SEP) + 1;
			      len = strlen (option);
			      if (len & 1)
				/*
				   Error, long-style option is trailed
				   by an invalid argument.
				 */
				opt_error = 5;
			      else
				{
				  if (translate_string == (char *) NULL)
				    translate_string =
				      (char *) my_malloc (len + 1,
							  ERR_NO_MEMORY_AVAILABLE,
							  __FILE__,
							  ((long) __LINE__) -
							  2L,
							  "translate_string",
							  0);
				  else
				    translate_string =
				      (char *) my_realloc ((VOID_PTR)
							   translate_string,
							   len + 1,
							   ERR_NO_MEMORY_AVAILABLE,
							   __FILE__,
							   ((long) __LINE__) -
							   3L,
							   "translate_string",
							   0);
				  strcpy (translate_string, option);
				}
			      break;
			      /*
			         The common holiday lists.
			       */
			    case SYM_ASTRONOMICAL_HDY:
			      hdy_astronomical = TRUE;
			      break;
			    case SYM_MULTICULTURAL_NEW_YEAR_HDY:
			      hdy_multicultural_new_year = TRUE;
			      break;
			    case SYM_ZODIACAL_MARKER_HDY:
			      hdy_zodiacal_marker = TRUE;
			      break;
			      /*
			         The calendar system specific holiday lists.
			       */
			    case SYM_BAHAI_HDY:
			      hdy_bahai = TRUE;
			      break;
			    case SYM_CELTIC_HDY:
			      hdy_celtic = TRUE;
			      break;
			    case SYM_CHINESE_FLEXIBLE_HDY:
			      hdy_chinese_flexible = TRUE;
			      break;
			    case SYM_CHINESE_HDY:
			      hdy_chinese = TRUE;
			      break;
			    case SYM_CHRISTIAN_HDY:
			      hdy_christian = TRUE;
			      break;
			    case SYM_HEBREW_HDY:
			      hdy_hebrew = TRUE;
			      break;
			    case SYM_ISLAMIC_HDY:
			      hdy_islamic = TRUE;
			      break;
			    case SYM_JAPANESE_FLEXIBLE_HDY:
			      hdy_japanese_flexible = TRUE;
			      break;
			    case SYM_JAPANESE_HDY:
			      hdy_japanese = TRUE;
			      break;
			    case SYM_ORTHODOX_NEW_HDY:
			      hdy_orthodox_new = TRUE;
			      break;
			    case SYM_ORTHODOX_OLD_HDY:
			      hdy_orthodox_old = TRUE;
			      break;
			    case SYM_PERSIAN_HDY:
			      hdy_persian = TRUE;
			      break;
			      /*
			         The calendar system specific month lists.
			       */
			    case SYM_BAHAI_MTH:
			      mth_bahai = TRUE;
			      break;
			    case SYM_CHINESE_FLEXIBLE_MTH:
			      mth_chinese_flexible = TRUE;
			      break;
			    case SYM_CHINESE_MTH:
			      mth_chinese = TRUE;
			      break;
			    case SYM_COPTIC_MTH:
			      mth_coptic = TRUE;
			      break;
			    case SYM_ETHIOPIC_MTH:
			      mth_ethiopic = TRUE;
			      break;
			    case SYM_FRENCH_REVOLUTIONARY_MTH:
			      mth_french_revolutionary = TRUE;
			      break;
			    case SYM_HEBREW_MTH:
			      mth_hebrew = TRUE;
			      break;
			    case SYM_INDIAN_CIVIL_MTH:
			      mth_indian_civil = TRUE;
			      break;
			    case SYM_ISLAMIC_MTH:
			      mth_islamic = TRUE;
			      break;
			    case SYM_JAPANESE_FLEXIBLE_MTH:
			      mth_japanese_flexible = TRUE;
			      break;
			    case SYM_JAPANESE_MTH:
			      mth_japanese = TRUE;
			      break;
			    case SYM_OLD_ARMENIC_MTH:
			      mth_old_armenic = TRUE;
			      break;
			    case SYM_OLD_EGYPTIC_MTH:
			      mth_old_egyptic = TRUE;
			      break;
			    case SYM_PERSIAN_MTH:
			      mth_persian = TRUE;
			      break;
			    default:
			      option = s2;
			      /*
			         Now we have decoded a long-style option into the
			         according short-style option, let's JUMP to that
			         part where the short options are decoded/evaluated
			         resp., processed (to JUMP isn't best coding style,
			         I know [Hi Mr.Dijkstra], but it's the easiest to
			         do now  =8^)
			       */
			      goto LABEL_short_option;
			    }
			}
		      else
			option = s2;
		    }
		  else
		    /*
		       Hmm, seems that a just processed short-style option having
		       permitted modifiers has an invalid switch character
		       within its modifiers.
		     */
		    opt_error = 5;
		  skip_option = TRUE;
		}
	      else
		/*
		   Check for short options...
		 */
	      LABEL_short_option:
		switch (*option)
		  {
		  case '?':
		  case 'h':
		    skip_option = help_flag = TRUE;
		    option++;
		    if (*option)
		      {
			if (*option == '?' || *option == 'h')
			  {
			    ext_help_flag = TRUE;
			    lopt_help = SYM_NIL;
			    option++;
			    if (*option && is_longopt)
			      {
				/*
				   Copy the long-style option argument into buffer.
				 */
				len = (int) strlen (option);
				if ((Uint) len >= maxlen_max)
				  resize_all_strings (len + 1, FALSE,
						      __FILE__,
						      (long) __LINE__);
				strcpy (s3, option);
				/*
				   Get the reference number of the long-style option
				   argument which is a long-style option itself.
				 */
				lopt_ambig =
				  (Bool) (eval_longopt (s3, &lopt_help) == 1);
				if (lopt_help == SYM_NIL)
				  /*
				     Error, no help related to this argument found,
				     so we display the "help on help" screen now.
				   */
				  help_on_help_flag = TRUE;
			      }
			    else if (*option)
			      /*
			         Error, invalid option modifier given.
			       */
			      opt_error = 2;
			  }
			else
			  /*
			     Error, no semi-long option name `-??' or `-hh' given.
			   */
			  opt_error = 2;
		      }
		    break;
		  case 'G':
		  case 'K':
		  case 'L':
		  case 'O':
		  case 'V':
		  case 'X':
#if USE_PAGER
		  case 'p':
#endif
		  case 'u':
		    option++;
		    if (!*option)
		      {
			option--;
			switch (*option)
			  {
			  case 'G':
			    hd_suppr_list_sep_flag = TRUE;
			    break;
			  case 'K':
			    cal_with_week_number = TRUE;
			    break;
			  case 'L':
			    license_flag = TRUE;
			    break;
			  case 'O':
			    orthodox_calendar = TRUE;
			    break;
			  case 'V':
			    version_flag = TRUE;
			    break;
			  case 'X':
			    hd_title_flag = FALSE;
			    break;
#if USE_PAGER
			  case 'p':
			    pager_flag = TRUE;
			    break;
#endif
			  case 'u':
			    suppr_cal_flag = TRUE;
			    break;
			  default:
			    /*
			       This case MUST be an internal error!
			     */
			    abort ();
			  }
			skip_option = TRUE;
		      }
		    else
		      /*
		         Error, invalid option modifier given.
		       */
		      opt_error = 2;
		    break;
		  case 'R':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			if (rsp_filename == (char *) NULL)
			  rsp_filename =
			    (char *) my_malloc (strlen (option) + 1,
						ERR_NO_MEMORY_AVAILABLE,
						__FILE__,
						((long) __LINE__) - 2L,
						"rsp_filename", 0);
			else
			  rsp_filename =
			    (char *) my_realloc ((VOID_PTR) rsp_filename,
						 strlen (option) + 1,
						 ERR_NO_MEMORY_AVAILABLE,
						 __FILE__,
						 ((long) __LINE__) - 3L,
						 "rsp_filename", 0);
			strcpy (rsp_filename, option);
			skip_option = TRUE;
		      }
		    break;
#ifdef GCAL_SHELL
		  case 'S':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			if (shl_filename == (char *) NULL)
			  shl_filename =
			    (char *) my_malloc (strlen (option) + 1,
						ERR_NO_MEMORY_AVAILABLE,
						__FILE__,
						((long) __LINE__) - 2L,
						"shl_filename", 0);
			else
			  shl_filename =
			    (char *) my_realloc ((VOID_PTR) shl_filename,
						 strlen (option) + 1,
						 ERR_NO_MEMORY_AVAILABLE,
						 __FILE__,
						 ((long) __LINE__) - 3L,
						 "shl_filename", 0);
			strcpy (shl_filename, option);
			skip_option = TRUE;
		      }
		    break;
#endif
		  case 'H':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			len = (int) strlen (option);
			/*
			   Check if the special argument `no' is given:
			   If only `long-option=NO' instead of both long-style
			   and short-style option; like `short-optionNO';
			   should be valid, extend following if() statement
			   like this:
			   if (is_longopt && ...
			 */
			if ((len == 2)
			    && (tolower (*option) == 'n')
			    && (tolower (*(option + 1)) == 'o'))
			  highlight_flag = FALSE;
			else
			  /*
			     Check if the special argument `yes' is given:
			     If only `long-option=YES' instead of both
			     long-style and short-style option; like
			     `short-optionYES'; should be valid, extend
			     following if() statement like this:
			     if (is_longopt && ...
			   */
			if ((len == 3)
			      && (tolower (*option) == 'y')
			      && (tolower (*(option + 1)) == 'e')
			      && (tolower (*(option + 2)) == 's'))
			  {
			    highlight_flag = TRUE;
			    is_tty = (int) TRUE;
			  }
			else
			  {
			    if (hl_seq == (char *) NULL)
			      hl_seq =
				(char *) my_malloc (strlen (option) + 1,
						    ERR_NO_MEMORY_AVAILABLE,
						    __FILE__,
						    ((long) __LINE__) - 2L,
						    "hl_seq", 0);
			    else
			      hl_seq = (char *) my_realloc ((VOID_PTR) hl_seq,
							    strlen (option) +
							    1,
							    ERR_NO_MEMORY_AVAILABLE,
							    __FILE__,
							    ((long) __LINE__)
							    - 3L, "hl_seq",
							    0);
			    strcpy (hl_seq, option);
			    highlight_flag = TRUE;
			  }
		      }
		    skip_option = TRUE;
		    break;
		  case 'i':
		    option++;
#ifdef GCAL_NLS
		    if (is_en)
		      {
			special_calsheet_flag = FALSE;
			if (!year_flag)
			  out_rows = S_OUT_ROWS;
		      }
		    else
		      {
			special_calsheet_flag = TRUE;
			if (!year_flag)
			  out_rows = J_OUT_ROWS;
		      }
#else /* !GCAL_NLS */
		    special_calsheet_flag = FALSE;
		    if (!year_flag)
		      out_rows = S_OUT_ROWS;
#endif	/* !GCAL_NLS */

		    if (*option)
		      {
			if (*option == '-')
			  {
			    option++;
			    if (*option)
			      /*
			         Error, option character is trailed
			         by an invalid argument.
			       */
			      opt_error = 5;
			  }
			else
			  /*
			     Error, option character is trailed by
			     an invalid modifier.
			   */
			  opt_error = 5;
		      }
		    option--;
		    break;
		  case 'n':
		  case 'N':
		    skip_option = holiday_flag = TRUE;
		    hd_sort_des_flag = FALSE;
		    hd_legal_days_only =
		      (Bool) ((*option == 'N') ? TRUE : FALSE);
		    option++;
		    if (*option)
		      {
			if (*option == *DES_LIT)
			  hd_sort_des_flag = TRUE;
			else
			  /*
			     Error, option character is trailed
			     by an invalid modifier.
			   */
			  opt_error = 5;
		      }
		    break;
		  case 'q':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed
		         by an argument.
		       */
		      opt_error = 4;
		    else
		      /*
		         Check for country code id's which are listed in the
		         `cc_holidays' table.  If more than one country code
		         is specified, it must be connected by a CONNECT_SEP.
		       */
		      {
			auto const Cc_struct *ptr_cc;


			*s1 = '\0';
			ptr_char = option;
			while (*ptr_char && !opt_error)
			  do
			    {
			      /*
			         Copy a single, given country code into `s2'.
			       */
			      i = 0;
			      while (*ptr_char && (*ptr_char != *CONNECT_SEP))
				s2[i++] = (char) toupper (*ptr_char++);
			      s2[i] = '\0';
			      /*
			         Now skip a possibly trailing CONNECT_SEP.
			       */
			      if (*ptr_char)
				ptr_char++;
			      ptr_cc = binsearch_cc_id (s2);
			      if (ptr_cc == (Cc_struct *) NULL)
				{
				  /*
				     Error, country code not found.
				   */
				  opt_error = 5;
				  break;
				}
			      else
				/*
				   Country code found, but insert it into `s1'
				   only in case it is not already listed there.
				 */
			      if (strstr (s1, s2) == (char *) NULL)
				{
				  strcat (s1, s2);
				  if (*ptr_char)
				    strcat (s1, CONNECT_SEP);
				}
			    }
			  while (*ptr_char);
			if (!opt_error)
			  {
			    if (cc == (char *) NULL)
			      cc = (char *) my_malloc (strlen (s1) + 1,
						       ERR_NO_MEMORY_AVAILABLE,
						       __FILE__,
						       ((long) __LINE__) - 2L,
						       "cc", 0);
			    else
			      cc = (char *) my_realloc ((VOID_PTR) cc,
							strlen (s1) + 1,
							ERR_NO_MEMORY_AVAILABLE,
							__FILE__,
							((long) __LINE__) -
							3L, "cc", 0);
			    strcpy (cc, s1);
			  }
		      }
		    skip_option = TRUE;
		    break;
		  case '!':
		    if (!is_longopt)
		      /*
		         Error, invalid short-style option character given.
		       */
		      opt_error = 2;
		    else
		      {
			/*
			   Get the argument of the `--date-format=PRESET_VALUE|ARG' long-style option.
			 */
			option++;
			if ((Uint) * option < LARG_MAX - 1)
			  {
			    date_format = supported_date_format;
			    date_format += ((Uint) * option - 1);
			    if (date_format->df_id == (char *) NULL)
			      /*
			         Error, index of an "empty" table element referenced.
			       */
			      opt_error = 5;
			  }
			else
			  {
			    /*
			       Respect this given argument now.
			     */
			    users_date_format.df_info = _("command line");
			    if (users_date_format.df_format == (char *) NULL)
			      users_date_format.df_format
				= (char *) my_malloc (strlen (option) + 1,
						      ERR_NO_MEMORY_AVAILABLE,
						      __FILE__,
						      ((long) __LINE__) - 2L,
						      "users_date_format.df_format",
						      0);
			    else
			      users_date_format.df_format
				=
				(char *) my_realloc ((VOID_PTR)
						     users_date_format.
						     df_format,
						     strlen (option) + 1,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 3L,
						     "users_date_format.df_format",
						     0);
			    strcpy (users_date_format.df_format, option);
			    date_format = &users_date_format;
			  }
			if (!opt_error)
			  {
			    /*
			       Test if the date format given in command line is valid.
			     */
			    if (!is_correct_date_format
				(date_format->df_format, &use_day_suffix,
				 &use_short3_day_name, &use_day_zeroleaded,
				 &use_year_zeroleaded))
			      errtxt_dformat = _("command line");
			    else
			      errtxt_dformat = (char *) NULL;
			  }
			skip_option = TRUE;
		      }
		    break;
		  case '$':
		    if (!is_longopt)
		      /*
		         Error, invalid short-style option character given.
		       */
		      opt_error = 2;
		    else
		      {
			/*
			   Get the argument of the `--gregorian-reform=PRESET_VALUE|ARG' long-style option.
			 */
			option++;
			if ((Uint) * option < LARG_MAX - 1)
			  {
			    greg = greg_reform_date;
			    greg += ((Uint) * option - 1);
			    if (!greg->year)
			      /*
			         Error, index of an "empty" table element referenced.
			       */
			      opt_error = 5;
			  }
			else
			  {
			    auto Bool ok = FALSE;


			    /*
			       Check whether the argument is a valid Gregorian
			       Reformation date, which must consist of the year,
			       the month, the first day and the last day (all
			       these date elements must be numbers and separated
			       by a SPLIT_SEP) the Gregorian Reformation has
			       occurred, like:  YYYYY,MM,DD,DD (MAX==10002,12,31,31)
			     */
			    users_greg.year = users_greg.month =
			      users_greg.first_day = users_greg.last_day = 0;
			    ptr_char = option;
			    do
			      {
				i = 0;
				while (!ok && !opt_error)
				  {
				    if (isdigit (*ptr_char)
					&& (i <= len_year_max))
				      s1[i++] = *ptr_char;
				    else
				      {
					if (isdigit (*ptr_char))
					  /*
					     Error, single date element too long.
					   */
					  opt_error = 5;
					else
					  if (i && (*ptr_char == *SPLIT_SEP))
					  {
					    ptr_char++;
					    if (*ptr_char == *SPLIT_SEP)
					      /*
					         Error, argument contains two
					         successive SPLIT_SEPs.
					       */
					      opt_error = 5;
					    else
					      /*
					         Scanning of single date element
					         completed.
					       */
					      break;
					  }
					else if (!i && *ptr_char)
					  /*
					     Error, either argument starts
					     with a SPLIT_SEP or contains
					     invalid characters.
					   */
					  opt_error = 5;
					else
					  /*
					     Scanning of all date elements
					     completed.
					   */
					  ok = TRUE;
				      }
				    if (*ptr_char)
				      ptr_char++;
				  }
				if (!opt_error)
				  {
				    s1[i] = '\0';
				    len = i;
				    i = atoi (s1);
				    if (i)
				      {
					if (!users_greg.year)
					  {
					    if (len > len_year_max)
					      if (atol (s1) > YEAR_MAX + 3)
						/*
						   Error, invalid year.
						 */
						opt_error = 5;
					    users_greg.year = i;
					  }
					else if (!users_greg.month)
					  users_greg.month = i;
					else if (!users_greg.first_day)
					  users_greg.first_day = i;
					else if (!users_greg.last_day)
					  users_greg.last_day = i;
					else
					  /*
					     Error, argument contains too many
					     date elements.
					   */
					  opt_error = 5;
				      }
				    else
				      /*
				         Error, argument contains invalid
				         date element.
				       */
				      opt_error = 5;
				  }
			      }
			    while (!ok && !opt_error);
			    /*
			       Now we have decoded the argument, so let's check
			       if the single date elements are valid.
			     */
			    if (!opt_error)
			      {
				if (users_greg.month
				    && users_greg.first_day
				    && users_greg.last_day
				    && (users_greg.year <= YEAR_MAX + 3)
				    && (users_greg.month <= MONTH_MAX)
				    && (users_greg.last_day >=
					users_greg.first_day)
				    &&
				    (((users_greg.month == 2)
				      && (users_greg.last_day <=
					  ((users_greg.year & 3) ? 28 : 29)))
				     || ((users_greg.month != 2)
					 && (users_greg.last_day <=
					     dvec[users_greg.month - 1]))))
				  /*
				     The given argument is valid, so respect
				     this given date of Gregorian Reformation.
				   */
				  greg = &users_greg;
				else
				  /*
				     Error, invalid date element found.
				   */
				  opt_error = 5;
			      }
			  }
			skip_option = TRUE;
		      }
		    break;
		  case 'j':
		    option++;
		    if (!*option)
		      {
			cal_special_flag = TRUE;
			cal_both_dates_flag = FALSE;
		      }
		    else
		      {
			if (*option == 'b')
			  {
			    option++;
			    if (!*option)
			      {
				cal_special_flag = FALSE;
				cal_both_dates_flag = TRUE;
			      }
			    else
			      /*
			         Error, option character is trailed
			         by an invalid modifier.
			       */
			      opt_error = 5;
			  }
			else
			  {
			    switch (*option)
			      {
			      case 'n':
				option++;
				if (!*option)
				  {
				    hd_special_flag = TRUE;
				    hd_both_dates_flag = FALSE;
				  }
				else if (*option == 'b')
				  {
				    hd_special_flag = FALSE;
				    hd_both_dates_flag = TRUE;
				  }
				else
				  /*
				     Error, option character is trailed
				     by an invalid modifier.
				   */
				  opt_error = 5;
				break;
#if USE_RC
			      case 'c':
				option++;
				if (!*option)
				  {
				    rc_special_flag = TRUE;
				    rc_both_dates_flag = FALSE;
				  }
				else if (*option == 'b')
				  {
				    rc_special_flag = FALSE;
				    rc_both_dates_flag = TRUE;
				  }
				else
				  /*
				     Error, option character is trailed
				     by an invalid modifier.
				   */
				  opt_error = 5;
				break;
#endif
			      default:
				/*
				   Error, option character is trailed
				   by an invalid modifier.
				 */
				opt_error = 5;
			      }
			  }
		      }
		    if (!*option)
		      option--;
		    break;
		  case 'b':
		    skip_option = year_flag = TRUE;
		    option++;
		    if (*option == '0')
		      {
			while (*option == '0')
			  option++;
			if (!*option)
			  option--;
		      }
		    out_rows = my_atoi (option);
		    if (out_rows > MONTH_MAX
			|| out_rows < MONTH_MIN
			|| out_rows == 5
			|| ((out_rows > 6) && (out_rows < MONTH_MAX)))
		      {
			if (!*option)
			  /*
			     Error, option character is not trailed
			     by an argument.
			   */
			  opt_error = 4;
			else
			  /*
			     Error, option character is trailed
			     by an invalid argument.
			   */
			  opt_error = 5;
		      }
		    break;
		  case 's':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			start_day = my_atoi (option);
			/*
			   Check for delivered weekday name.
			 */
			if (!start_day)
			  start_day = compare_d_m_name (option, DAy);
			if (!start_day)
			  {
			    /*
			       Check if the 'today' text is given.
			     */
			    len = (int) strlen (option);
			    if ((Uint) len >= maxlen_max)
			      resize_all_strings (len + 1, FALSE, __FILE__,
						  (long) __LINE__);
			    strcpy (s1, option);
			    if (strlen (s1) > strlen ("today"))
			      /*
			         Error, option character is trailed
			         by an invalid argument.
			       */
			      opt_error = 5;
			    else
			      if (isdigit (*option)
				  || !strncasecmp (s1, "today", len))
			      /*
			         Set a special value for a given `-s0' argument.
			       */
			      start_day = SPECIAL_VALUE;
			    else
			      /*
			         Error, option character is trailed
			         by an invalid argument.
			       */
			      opt_error = 5;
			  }
			else if (start_day > DAY_MAX || start_day < DAY_MIN)
			  /*
			     Error, option character is trailed
			     by an invalid argument.
			   */
			  opt_error = 5;
		      }
		    skip_option = TRUE;
		    break;
#if USE_RC
		  case 'r':
		  case 'v':
		    /*
		       If the define_global_text_variable option `-r<def:def...>'
		       or the define_global_date_variable option `-v<def:def...>'
		       is given, skip/ignore that option because it's already
		       managed by the `main()' function.  Only check if NO
		       definitions are given because this is an error!
		     */
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    skip_option = TRUE;
		    break;
		  case 'f':
		  case 'F':
		    if (isupper (*option))
		      rc_all_dates_flag = TRUE;
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			len = (int) strlen (option);
			/*
			   Check if a file name argument with a leading
			   CONNECT_SEP like '+foo' is given.
			 */
			if (*option == *CONNECT_SEP)
			  /*
			     Error, option character is trailed
			     by an invalid argument.
			   */
			  opt_error = 5;
			else
			  {
			    if (len > 1)
			      {
				/*
				   Check if a file name argument with a trailing
				   CONNECT_SEP like 'foo+' is given.
				 */
				if ((*(option + (len - 1)) == *CONNECT_SEP)
				    && (*(option + (len - 2)) != QUOTE_CHAR))
				  /*
				     Error, option character is trailed
				     by an invalid argument.
				   */
				  opt_error = 5;
				else
				  {
				    /*
				       Check if a file name list argument containing
				       attached unquoted CONNECT_SEP like
				       'foo+bar+++file' is given.
				     */
				    ptr_char = strchr (option, *CONNECT_SEP);
				    if (ptr_char != (char *) NULL)
				      {
					ptr_char--;
					if (*ptr_char == QUOTE_CHAR)
					  ptr_char++;
					ptr_char++;
					while (*ptr_char && !opt_error)
					  {
					    if (*ptr_char == QUOTE_CHAR)
					      ptr_char++;
					    else
					      if (*ptr_char == *CONNECT_SEP)
					      {
						ptr_char++;
						if (*ptr_char == QUOTE_CHAR)
						  ptr_char++;
						else
						  if (*ptr_char ==
						      *CONNECT_SEP)
						  /*
						     Error, option character is trailed
						     by an invalid argument.
						   */
						  opt_error = 5;
					      }
					    if (*ptr_char)
					      ptr_char++;
					  }
				      }
				  }
			      }
			    if (!opt_error)
			      {
				if (rc_filename == (char *) NULL)
				  rc_filename = (char *) my_malloc (len + 1,
								    ERR_NO_MEMORY_AVAILABLE,
								    __FILE__,
								    ((long)
								     __LINE__)
								    - 2L,
								    "rc_filename",
								    0);
				else
				  rc_filename =
				    (char *) my_realloc ((VOID_PTR)
							 rc_filename, len + 1,
							 ERR_NO_MEMORY_AVAILABLE,
							 __FILE__,
							 ((long) __LINE__) -
							 3L, "rc_filename",
							 0);
				strcpy (rc_filename, option);
				skip_option = rc_use_flag = TRUE;
			      }
			  }
		      }
		    break;
		  case '#':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			/*
			   If the first `-# ARG' resp., `--here=ARG' option
			   is found, create and open a temporary file for
			   storing it.
			 */
			if (rc_here_fn == (char *) NULL)
			  {
			    ptr_char = TMPFILENAME;
			    if (ptr_char == (char *) NULL)
			      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
					((long) __LINE__) - 2L, "tmpnam()=",
					0);
			    rc_here_fn =
			      (char *) my_malloc (strlen (ptr_char) + 1,
						  ERR_NO_MEMORY_AVAILABLE,
						  __FILE__,
						  ((long) __LINE__) - 2L,
						  "rc_here_fn", 0);
			    strcpy (rc_here_fn, ptr_char);
			    rc_here_fp = fopen (rc_here_fn, "w");
			    if (rc_here_fp == (FILE *) NULL)
			      my_error (ERR_WRITE_FILE, __FILE__,
					((long) __LINE__) - 2L, rc_here_fn,
					0);
			  }
			/*
			   Copy this long-style option into a buffer.
			 */
			len = (int) strlen (option);
			if ((Uint) len >= maxlen_max)
			  resize_all_strings (len + 1, FALSE, __FILE__,
					      (long) __LINE__);
			strcpy (s2, option);
			pseudo_blank_conversion (&s2);
			/*
			   And write the option argument into the temporary file.
			 */
			i = fprintf (rc_here_fp, "%s\n", s2);
			if (i == EOF)
			  my_error (ERR_WRITE_FILE, __FILE__,
				    ((long) __LINE__) - 2L, rc_here_fn, 0);
		      }
		    skip_option = rc_use_flag = TRUE;
		    break;
		  case 'D':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			/*
			   Check for %? special_texts (without the leading
			   `%' character) that disable a fixed date.
			 */
			ptr_char = option;
			while (*ptr_char && !opt_error)
			  if (*ptr_char == RC_EX_LHDY_CHAR
			      || *ptr_char == RC_EX_NLHDY_CHAR
			      || *ptr_char == RC_EX_AHDY_CHAR
			      || *ptr_char == RC_EX_NAHDY_CHAR
			      || *ptr_char == RC_EX_MON_CHAR
			      || *ptr_char == RC_EX_NMON_CHAR
			      || *ptr_char == RC_EX_TUE_CHAR
			      || *ptr_char == RC_EX_NTUE_CHAR
			      || *ptr_char == RC_EX_WED_CHAR
			      || *ptr_char == RC_EX_NWED_CHAR
			      || *ptr_char == RC_EX_THU_CHAR
			      || *ptr_char == RC_EX_NTHU_CHAR
			      || *ptr_char == RC_EX_FRI_CHAR
			      || *ptr_char == RC_EX_NFRI_CHAR
			      || *ptr_char == RC_EX_SAT_CHAR
			      || *ptr_char == RC_EX_NSAT_CHAR
			      || *ptr_char == RC_EX_SUN_CHAR
			      || *ptr_char == RC_EX_NSUN_CHAR
			      || *ptr_char == RC_EX_MON_2_THU_CHAR
			      || *ptr_char == RC_EX_NMON_2_THU_CHAR
			      || *ptr_char == RC_EX_MON_2_FRI_CHAR
			      || *ptr_char == RC_EX_NMON_2_FRI_CHAR)
			    ptr_char++;
			  else
			    /*
			       Error, invalid %? special text character given.
			     */
			    opt_error = 5;
			if (!opt_error)
			  {
			    if (rc_filter_day == (char *) NULL)
			      rc_filter_day =
				(char *) my_malloc (strlen (option) + 1,
						    ERR_NO_MEMORY_AVAILABLE,
						    __FILE__,
						    ((long) __LINE__) - 2L,
						    "rc_filter_day", 0);
			    else
			      rc_filter_day =
				(char *) my_realloc ((VOID_PTR) rc_filter_day,
						     strlen (option) + 1,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 3L,
						     "rc_filter_day", 0);
			    strcpy (rc_filter_day, option);
			  }
		      }
		    skip_option = TRUE;
		    break;
		  case 'P':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			/*
			   Check for %?[DATE][#[DATE]] special_texts (without the
			   leading `%' character) that disable a fixed date.
			   If more than one special text is specified, it must
			   be separated by a SPLIT_SEP.
			 */
			ptr_char = option;
			while (*ptr_char && !opt_error)
			  if (*ptr_char == RC_IDATE_CHAR
			      || *ptr_char == RC_EDATE_CHAR)
			    {
			      ptr_char++;
			      while (*ptr_char && (*ptr_char != *SPLIT_SEP))
				ptr_char++;
			      if (!*ptr_char || *ptr_char == *SPLIT_SEP)
				{
				  if (*ptr_char)
				    ptr_char++;
				}
			      else
				/*
				   Error, invalid %?[DATE][#[DATE]]
				   special text given.
				 */
				opt_error = 5;
			    }
			  else
			    /*
			       Error, invalid %?[DATE][#[DATE]]
			       special text given.
			     */
			    opt_error = 5;
			if (!opt_error)
			  {
			    if (rc_filter_period == (char *) NULL)
			      rc_filter_period =
				(char *) my_malloc (strlen (option) + 1,
						    ERR_NO_MEMORY_AVAILABLE,
						    __FILE__,
						    ((long) __LINE__) - 2L,
						    "rc_filter_period", 0);
			    else
			      rc_filter_period =
				(char *) my_realloc ((VOID_PTR)
						     rc_filter_period,
						     strlen (option) + 1,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 3L,
						     "rc_filter_period", 0);
			    strcpy (rc_filter_period, option);
			  }
		      }
		    skip_option = TRUE;
		    break;
		  case 'I':
		    option++;
		    if (!*option)
		      /*
		         Error, option character is not trailed by an argument.
		       */
		      opt_error = 4;
		    else
		      {
			if (rc_filter_text == (char *) NULL)
			  rc_filter_text =
			    (char *) my_malloc (strlen (option) + 1,
						ERR_NO_MEMORY_AVAILABLE,
						__FILE__,
						((long) __LINE__) - 2L,
						"rc_filter_text", 0);
			else
			  rc_filter_text =
			    (char *) my_realloc ((VOID_PTR) rc_filter_text,
						 strlen (option) + 1,
						 ERR_NO_MEMORY_AVAILABLE,
						 __FILE__,
						 ((long) __LINE__) - 3L,
						 "rc_filter_text", 0);
			strcpy (rc_filter_text, option);
		      }
		    skip_option = TRUE;
		    break;
		  case 'g':
		    option++;
		    if (*option)
		      {
			if (rc_grp_sep == (char *) NULL)
			  rc_grp_sep =
			    (char *) my_malloc (strlen (option) + 1 + 2,
						ERR_NO_MEMORY_AVAILABLE,
						__FILE__,
						((long) __LINE__) - 2L,
						"rc_grp_sep", 0);
			else
			  rc_grp_sep =
			    (char *) my_realloc ((VOID_PTR) rc_grp_sep,
						 strlen (option) + 1 + 2,
						 ERR_NO_MEMORY_AVAILABLE,
						 __FILE__,
						 ((long) __LINE__) - 3L,
						 "rc_grp_sep", 0);
			strcpy (rc_grp_sep, "0 ");
			strcat (rc_grp_sep, option);
			pseudo_blank_conversion (&rc_grp_sep);
		      }
		    else
		      {
			if (rc_grp_sep == (char *) NULL)
			  rc_grp_sep =
			    (char *) my_malloc (strlen (RC_GROUP_SEP) + 1 + 2,
						ERR_NO_MEMORY_AVAILABLE,
						__FILE__,
						((long) __LINE__) - 2L,
						"rc_grp_sep", 0);
			else
			  rc_grp_sep =
			    (char *) my_realloc ((VOID_PTR) rc_grp_sep,
						 strlen (RC_GROUP_SEP) + 1 +
						 2, ERR_NO_MEMORY_AVAILABLE,
						 __FILE__,
						 ((long) __LINE__) - 3L,
						 "rc_grp_sep", 0);
			strcpy (rc_grp_sep, "0 ");
			strcat (rc_grp_sep, RC_GROUP_SEP);
		      }
		    skip_option = TRUE;
		    break;
		  case 'a':
		    rc_enable_fn_flag = TRUE;
		    break;
		  case 'A':
		    rc_alternative_format_flag = TRUE;
		    break;
		  case 'd':
		    rc_have_today_in_list = 1;
		    break;
		  case 'e':
		  case 'E':
		    if (isupper (*option))
		      {
			rc_enable_hdl_flag = TRUE;
			rc_enable_hda_flag = FALSE;
		      }
		    else
		      {
			rc_enable_hdl_flag = FALSE;
			rc_enable_hda_flag = TRUE;
		      }
		    break;
		  case 'o':
		    rc_omit_date_flag = TRUE;
		    break;
		  case 'l':
		    /*
		       Assume the user wants to have a list
		       of periods instead of a single period!
		     */
		    rc_period_list = TRUE;
		    break;
		  case 'k':
		    rc_week_number_flag = TRUE;
		    break;
		  case 'U':
		    rc_suppr_date_part_flag = TRUE;
		    break;
		  case 'Q':
		    rc_suppr_list_sep_flag = TRUE;
		    break;
		  case 'J':
		    rc_suppr_text_part_flag = TRUE;
		    break;
		  case 'x':
		    rc_title_flag = FALSE;
		    break;
		  case 'z':
		    rc_count_flag = TRUE;
		    break;
		  case 'Z':
		    rc_zero_dates_flag = TRUE;
		    break;
		  case 'c':
		  case 'C':
		    rc_use_flag = TRUE;
		    if (isupper (*option))
		      rc_all_dates_flag = TRUE;
		    option++;
		    if (*option == *DES_LIT)
		      {
			rc_sort_des_flag = TRUE;
			while (*option == *DES_LIT)
			  option++;
		      }
		    option--;
		    break;
		  case '0':
		  case '1':
		  case '2':
		  case '3':
		  case '4':
		  case '5':
		  case '6':
		  case '7':
		  case '8':
		  case '9':
		  case RC_HDY_CHAR:
		  case RC_NWD_CHAR:
		    skip_option = rc_period_flag = TRUE;
		    /*
		       Store some actual values now by reason this part
		       is managed at a later place in the sequence,
		       and it is necessary to restore them there.
		     */
		    if (rc_period_argv == (char *) NULL)
		      rc_period_argv = (char *) my_malloc (strlen (*argv) + 1,
							   ERR_NO_MEMORY_AVAILABLE,
							   __FILE__,
							   ((long) __LINE__) -
							   2L,
							   "rc_period_argv",
							   0);
		    else
		      rc_period_argv =
			(char *) my_realloc ((VOID_PTR) rc_period_argv,
					     strlen (*argv) + 1,
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 3L,
					     "rc_period_argv", 0);
		    strcpy (rc_period_argv, *argv);
		    if (rc_period_option == (char *) NULL)
		      rc_period_option =
			(char *) my_malloc (strlen (option) + 1,
					    ERR_NO_MEMORY_AVAILABLE, __FILE__,
					    ((long) __LINE__) - 2L,
					    "rc_period_option", 0);
		    else
		      rc_period_option =
			(char *) my_realloc ((VOID_PTR) rc_period_option,
					     strlen (option) + 1,
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 3L,
					     "rc_period_option", 0);
		    strcpy (rc_period_option, option);
		    rc_period_is_longopt = is_longopt;
		    break;
		  case 't':
		  case 'T':
		    rc_clean_flags ();
		    skip_option = rc_use_flag = rc_tomorrow_flag = TRUE;
		    if (isupper (*option))
		      rc_all_dates_flag = TRUE;
		    option++;
		    if (*option)
		      /*
		         Error, invalid option modifier given.
		       */
		      opt_error = 5;
		    break;
		  case 'w':
		  case 'W':
		  case 'm':
		  case 'M':
		  case 'y':
		  case 'Y':
		    rc_clean_flags ();
		    switch (*option)
		      {
		      case 'w':
		      case 'W':
			rc_week_flag = TRUE;
			break;
		      case 'm':
		      case 'M':
			rc_month_flag = TRUE;
			break;
		      default:
			rc_year_flag = TRUE;
		      }
		    skip_option = rc_use_flag = TRUE;
		    if (isupper (*option))
		      rc_all_dates_flag = TRUE;
		    opt_error = further_check (&option);
		    break;
#endif /* USE_RC */
		  default:
		    /*
		       Error, any unmanaged options are always invalid.
		     */
		    if (is_longopt)
		      opt_error = 5;
		    else
		      opt_error = 2;
		  }
	      if (opt_error)
		{
		  ptr_char = *argv;
#if USE_RC
		LABEL_option_error:
#endif
		  /*
		     Remove leading switch characters of the given option
		     in some cases.
		   */
		  if (!is_longopt)
		    while (*ptr_char == *SWITCH)
		      ptr_char++;
		  if (*ptr_char == *SWITCH2)
		    if (*(ptr_char + 1) != *SWITCH2)
		      ptr_char++;
		  /*
		     Allocate proper string space for the option error text.
		   */
		  switch (opt_error)
		    {
		    case 1:
		    case 2:
		    case 3:
		    case 4:
		    case 5:
		    case 6:
		      if (is_longopt && (opt_error == 3))
			len = (int) strlen (option) + LEN_SINGLE_LINE;
		      else
			len = (int) strlen (ptr_char) + LEN_SINGLE_LINE;
		      if ((Uint) len >= maxlen_max)
			resize_all_strings (len + 1, FALSE, __FILE__,
					    (long) __LINE__);
		      break;
		    default:
		      ;		/* Void, nothing to do now */
		    }
		  /*
		     Construct the option error text.
		   */
		  switch (opt_error)
		    {
		    case 1:
		      sprintf (s1, _("%s: option `%s' is ambiguous"),
			       prgr_name, ptr_char);
		      break;
		    case 2:
		      if (is_longopt)
			sprintf (s1, _("%s: unrecognized option `%s'"),
				 prgr_name, ptr_char);
		      else
			sprintf (s1, _("%s: invalid option -- %s"),
				 prgr_name, ptr_char);
		      break;
		    case 3:
		      if (is_longopt)
			sprintf (s1,
				 _
				 ("%s: option `--%s' doesn't allow an argument"),
				 prgr_name, option);
		      else
			sprintf (s1,
				 _
				 ("%s: option `%s' doesn't allow an argument"),
				 prgr_name, ptr_char);
		      break;
		    case 4:
		      if (is_longopt)
			sprintf (s1,
				 _("%s: option `%s' requires an argument"),
				 prgr_name, ptr_char);
		      else
			sprintf (s1,
				 _("%s: option requires an argument -- %s"),
				 prgr_name, ptr_char);
		      break;
		    case 5:
		      sprintf (s1,
			       _("%s: option with invalid argument -- %s"),
			       prgr_name, ptr_char);
		      break;
		    case 6:
		      sprintf (s1,
			       _("%s: option with ambiguous argument -- %s"),
			       prgr_name, ptr_char);
		      break;
		    default:
		      /*
		         This case must be an internal error
		       */
		      my_error (opt_error, __FILE__, ((long) __LINE__) - 4L,
				"", 0);
		    }
		  /*
		     Display the option error text and exit with error.
		   */
		  fprintf (stderr, "%s\n%s\n%s\n", s1, usage_msg (),
			   lopt_msg ());
		  my_exit (ERR_INVALID_OPTION);
		}
	      if (skip_option)
		{
		  while (*option)
		    option++;
		  option--;
		}
	    }
	  argc--;
	}
      else
	{
#if USE_RC
	  if (**argv == RC_ADATE_CHAR)
	    /*
	       If an actual date modifier %DATE is given, skip/ignore this
	       option, because we evaluate it some lines later.
	     */
	    argc--;
	  else
#endif /* USE_RC */
	  if (**argv == RSP_CHAR)
	    /*
	       If a response file @FILE is given, skip/ignore this
	       option, because we don't manage nested response files!
	     */
	    argc--;
	  else
	    /*
	       If a command is given, skip processing the options.
	     */
	    break;
	}
    }
#if USE_RC
  /*
     Read the internal system date an evaluate
     a possibly given actual date modifier %DATE.
   */
  if (!get_actual_date ())
    {
      /*
         Error, invalid actual date modifier %DATE given.
       */
      fprintf (stderr, _("%s: invalid date given -- %c%s\n%s\n%s\n"),
	       prgr_name, RC_ADATE_CHAR, rc_adate, usage_msg (), lopt_msg ());
      my_exit (ERR_INVALID_OPTION);
    }
#else /* !USE_RC */
  /*
     Read the actual system date and time that is reported by the operating system.
   */
  (void) get_actual_date ();
#endif /* !USE_RC */
  is_leap_year = (days_of_february (act_year) == 29);
  /*
     Set the starting day of week if it is not set by a command line argument.
   */
  buf_start_day = start_day;
  if (start_day == SPECIAL_VALUE)
    /*
       Set starting day of week to actual weekday.
     */
    start_day = weekday_of_date (act_day, act_month, act_year);
  else if (!start_day)
    {
      /*
         Set starting day of week to language/territory default value.
       */
#if defined GCAL_NLS
# if HAVE__NL_TIME_FIRST_WEEKDAY
      start_day = (nl_langinfo (_NL_TIME_FIRST_WEEKDAY)[0] + 5) % 7 + 1;
# else
      start_day = DAY_MAX;
# endif
#else /* !GCAL_NLS */
#error MUST USE _NL_TIME_FIRST_WEEKDAY !
      start_day = DAY_MAX;
#endif	/* !GCAL_NLS */
    }
  /*
     Post-process a time offset argument, which is based relative to
     the actual time ('t' | '@' found), of the `--time-offset' option NOW.
   */
  if (rel_time_offset)
    {
      if (!time_hour_offset && !time_min_offset)
	{
#if USE_RC
	  if (rel_time_offset == RC_GMTIME_CHAR)
	    {
	      time_hour_offset = gmt_hour;
	      time_min_offset = gmt_min;
	    }
	  else
#endif
	    {
	      time_hour_offset = act_hour;
	      time_min_offset = act_min;
	    }
	}
      else
	{
#if USE_RC
	  if (rel_time_offset == RC_GMTIME_CHAR)
	    i =
	      HHMM2MM (gmt_hour, gmt_min) + HHMM2MM (time_hour_offset,
						     time_min_offset);
	  else
#endif
	    i =
	      HHMM2MM (act_hour, act_min) + HHMM2MM (time_hour_offset,
						     time_min_offset);
	  time_hour_offset = MM2HH (abs (i));
	  time_min_offset = abs (i) % MINS_PER_HOUR;
	  if (i < 0)
	    {
	      time_hour_offset = -time_hour_offset;
	      time_min_offset = -time_min_offset;
	    }
	}
    }
#if USE_RC
  time_offset = HHMM2DAY (time_hour_offset, time_min_offset);
  /*
     Post-process a cycle-ending time argument, which is based relative to
     the actual time ('t' | '@' found), of the `--cycle-end' option NOW.
   */
  if (rel_loop_end)
    {
      if (rel_loop_end == RC_GMTIME_CHAR)
	i = HHMM2MM (gmt_hour, gmt_min);
      else
	i = HHMM2MM (act_hour, act_min);
      if (!set_loop_end && (loop_end == SPECIAL_VALUE))
	loop_end = i;
      else
	loop_end = i + loop_end;
      set_loop_end = TRUE;
    }
  if (set_loop_end)
    {
      /*
         Reduce the given cycle-ending time value in minutes to a single day.
       */
      if (loop_end < 0)
	loop_end = 0;
      else if (loop_end >= MINS_PER_DAY)
	loop_end = MINS_PER_DAY - 1;
    }
  /*
     Post-check for arguments of the `--period-of-fixed-dates=ARG' option NOW.
   */
  if (rc_period_flag)
    {
      if (!rc_use_flag)
	/*
	   Error, use of fixed dates list neither implicitly nor explicitly
	   specified (e.g., option like `-10' or `-*w10fr' given).
	 */
	opt_error = 2;
      else
	{
	  register int act_is_leap_year = (days_of_february (act_year) == 29);
	  auto int wmax = WEEK_MAX;
	  auto char dvar = '\0';
	  auto Bool nth_day_of_year_flag = FALSE;


	  if (act_year == greg->year)
	    wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
		     - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
	  rc_clean_flags ();
	  i = 0;
	  rc_period = 0;
	  rc_period_flag = TRUE;
	  option = rc_period_option;
	  /*
	     Before I forget... All mode specifying characters
	     ('d' or 'w') may be given in upper or lower
	     case letters, i.e. they are managed case insensitive!
	   */
	  /*
	     Check if @e|t|DVAR[[-]N[WW[W]]] or *d|wN[WW[W]] is given.
	   */
	  if (*option == RC_HDY_CHAR || *option == RC_NWD_CHAR)
	    dvar = *option;
	  else
	    /*
	       Compute the period for the Nd modifier.
	     */
	    rc_period = atoi (option);
	  if (!dvar)
	    {
	      /*
	         Check if a N+ modifier is given.
	       */
	      rc_fwdf_buffer = rc_forwards_flag =
		(Bool) (strchr (option, *ASC_LIT) != (char *) NULL);
	      /*
	         Check if a N- modifier is given.
	       */
	      rc_bwdf_buffer = rc_backwards_flag =
		(Bool) (strchr (option, *DES_LIT) != (char *) NULL);
	      /*
	         Check if a Nw modifier is given.
	       */
	      ptr_char = strrchr (option, 'w');
	      if (ptr_char == (char *) NULL)
		ptr_char = strrchr (option, 'W');
	      if (ptr_char != (char *) NULL)
		{
		  ptr_char++;
		  if (!*ptr_char)
		    rc_week_year_flag = TRUE;
		}
	      /*
	         Check if a Nd modifier is given.
	       */
	      ptr_char = strrchr (option, 'd');
	      if (ptr_char == (char *) NULL)
		ptr_char = strrchr (option, 'D');
	      if (ptr_char != (char *) NULL)
		{
		  ptr_char++;
		  if (!*ptr_char)
		    nth_day_of_year_flag = TRUE;
		}
	      i = (int) rc_forwards_flag + rc_backwards_flag
		+ rc_week_year_flag + nth_day_of_year_flag;
	    }
	  /*
	     Compute the period for a Nd modifier.
	   */
	  if (rc_period == 999)
	    rc_period = DAY_LAST + act_is_leap_year;
	  if (!dvar
	      && (i > 1
		  || ((rc_period > DAY_LAST + act_is_leap_year)
		      && (rc_forwards_flag
			  || rc_backwards_flag
			  || nth_day_of_year_flag))
		  || (rc_week_year_flag
		      && (rc_period > wmax + 1) && (rc_period != 99))))
	    /*
	       Error, illegal fixed date period modifier given.
	     */
	    opt_error = 5;
	  else
	    {
	      /*
	         Found one of these fixed date period modifiers:
	         @e|t|DVAR[[-]N[WW[W]]]
	         *d|wN[WW[W]]
	         MMDD
	         MMWW[W]N
	       */
	      if (!rc_week_year_flag
		  && !rc_forwards_flag && !rc_backwards_flag)
		{
		  auto int y = act_year;
		  auto int n;
		  auto char hc;
		  auto Bool is_weekday_mode;


		  if (!nth_day_of_year_flag)
		    {
		      len = (int) strlen (option) + len_year_max;
		      if ((Uint) len >= maxlen_max)
			resize_all_strings (len + 1, FALSE, __FILE__,
					    (long) __LINE__);
		      sprintf (s2, "%0*d%s", len_year_max, y, option);
		      /*
		         The `rc_get_date()' arguments `wmax', `hc' and `i' are
		         only dummys and must be given.  They are not respected!
		       */
		      (void) rc_get_date (s2, lineptrs, FALSE,
					  &is_weekday_mode, &day, &month, &y,
					  &n, &wmax, &hc, &i, &i,
					  _("Internal"), -1L, s2, FALSE);
		      if (y != SPECIAL_VALUE)
			{
			  if (!dvar)
			    {
			      /*
			         MMWW[W]N or MMDD given.
			       */
			      if (!month)
				month = act_month;
			      if (month < MONTH_MIN || month > MONTH_MAX)
				/*
				   Error, invalid month given.
				 */
				opt_error = 5;
			      else
				{
				  i = dvec[month - 1];
				  if (month == 2)
				    i += act_is_leap_year;
				  if (is_weekday_mode)
				    {
				      /*
				         MMWW[W]N given.
				       */
				      if (n == 9)
					day =
					  eval_holiday (i, month, act_year,
							day, FALSE);
				      else
					{
					  day =
					    eval_holiday (DAY_MIN, month,
							  act_year, day,
							  TRUE);
					  day += (DAY_MAX * (n - 1));
					  if (day > i)
					    /*
					       Error, month contains no such
					       "N'th weekday of month".
					     */
					    opt_error = 5;
					  else
					    {
					      /*
					         Now check if the given date is valid.
					       */
					      if (!day
						  || !valid_date (day, month,
								  act_year))
						/*
						   Error, invalid date given.
						 */
						opt_error = 5;
					    }
					}
				    }
				  else
				    {
				      /*
				         MMDD given.
				       */
				      if (day == 99)
					day = i;
				      if (!day)
					day = act_day;
				      if (day < DAY_MIN || day > i)
					/*
					   Error, invalid day given.
					 */
					opt_error = 5;
				    }
				}
			    }
			  else if (!day || !month)
			    /*
			       Error, either invalid date variable or
			       invalid mode specifier given (not d|w).
			     */
			    opt_error = 5;
			  if (!opt_error)
			    rc_period = day_of_year (day, month, act_year);
			}
		      else
			/*
			   Error, invalid date given.
			 */
			opt_error = 5;
		    }
		  else if (!rc_period)
		    /*
		       Error, zero date is invalid.
		     */
		    opt_error = 5;
		  if (!opt_error)
		    {
		      i = day_of_year (act_day, act_month, act_year);
		      if (rc_period >= i)
			{
			  rc_period -= i;
			  /*
			     Set `rc_forwards_flag' only if the recomputed
			     `rc_period' is not zero (it is in case @t
			     is given and the actual date is the last day
			     of the year, so we have to manage this like
			     simple `-c' is given in command line.
			   */
			  if (!rc_period)
			    rc_period_flag = FALSE;
			  else
			    rc_forwards_flag = TRUE;
			}
		      else
			{
			  rc_period = i - rc_period;
			  rc_backwards_flag = TRUE;
			}
		    }
		}
	      else
		{
		  /*
		     Nw|+|- given.
		   */
		  i = 0;
		  while (isdigit (*option))
		    {
		      i++;
		      option++;
		    }
		  option--;
		  if (!rc_period && !rc_week_year_flag)
		    /*
		       Error, zero length date of fixed date period given.
		     */
		    opt_error = 5;
		  else
		    opt_error = further_check (&option);
		}
	    }
	}
      if (opt_error)
	{
	  ptr_char = rc_period_argv;
	  is_longopt = rc_period_is_longopt;
	  /*
	     Sorry! Sorry!  This is so unprofessional!!  :)
	   */
	  goto LABEL_option_error;
	}
      else
	{
	  free (rc_period_argv);
	  free (rc_period_option);
	}
    }
#endif /* USE_RC */
  if (help_flag || license_flag || version_flag)
    {
#if USE_PAGER
      /*
         No program output yet and output goes to a tty:
         Set the standard stream buffers to unbuffered mode,
         then get actual settings of the tty (needed if paging is wanted).
       */
      if (is_tty1
# ifdef GCAL_EPAGER
	  && (ext_pager == (char *) NULL)
# endif
	  && is_tty2)
	{
	  setbuf (stdout, (char *) NULL);
	  setbuf (stdin, (char *) NULL);
	  get_tty_scr_size (&tty_rows, &tty_cols);
	}
      else if (help_on_help_flag)
	/*
	   We have to use the actual screen size in this case!
	 */
	get_tty_scr_size (&tty_rows, &tty_cols);
#endif /* USE_PAGER */
      /*
         Get the highlighting sequences of the terminal.
       */
      get_tty_hls (hl_seq);
#ifdef GCAL_EPAGER
      if (is_tty1
	  && is_tty2
	  && pager_flag
	  && (ext_pager != (char *) NULL) && (email_adr == (char *) NULL))
	{
	  child_pid = fork ();
	  switch (child_pid)
	    {
	    case -1:
	      /*
	         Error, `fork()' function failed.
	       */
	      my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			((long) __LINE__) - 7L, "fork() child_pid=",
			child_pid);
	      /* Not reached. */
	    case 0:
	      /*
	         Child process (read from pipe):
	         Connect PIPE-STDIN to SYS-STDIN in a safe way
	         and launch external pager program.
	       */
	      close (pipe_fd[1]);
	      if (pipe_fd[0])
		{
		  close (0);
		  dup (pipe_fd[0]);
		  close (pipe_fd[0]);
		}
	      if (pg_argv == (char **) NULL)
		{
		  i = execlp (ext_pager, ext_pager, (char *) NULL);
		  /*
		     Error, `execlp()' function failed
		     (this line should never be executed).
		   */
		  len = (int) strlen (ext_pager) + LEN_SINGLE_LINE;
		  if ((Uint) len >= maxlen_max)
		    resize_all_strings (len + 1, FALSE, __FILE__,
					(long) __LINE__);
		  sprintf (s1, "execlp(%s)=", ext_pager);
		  my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			    ((long) __LINE__) - 9L, s1, i);
		}
	      else
		{
		  i = execvp (ext_pager, pg_argv);
		  /*
		     Error, `execvp()' function failed
		     (this line should never be executed).
		   */
		  len = (int) strlen (ext_pager) + LEN_SINGLE_LINE;
		  if ((Uint) len >= maxlen_max)
		    resize_all_strings (len + 1, FALSE, __FILE__,
					(long) __LINE__);
		  sprintf (s1, "execvp(%s)=", ext_pager);
		  my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
			    ((long) __LINE__) - 9L, s1, i);
		}
	      /* Not reached. */
	    default:
	      /*
	         Parent process (write to pipe):
	         Connect PIPE-stdout to SYS-stdout in a safe way
	         and initiate the action.
	       */
	      close (pipe_fd[0]);
	      if (pipe_fd[1] != 1)
		{
		  close (1);
		  if (!dup (pipe_fd[1]))
		    {
		      dup (pipe_fd[1]);
		      close (0);
		    }
		  close (pipe_fd[1]);
		}
	    }
	}
#endif /* GCAL_EPAGER */
      if (help_flag)
	{
	  if (help_on_help_flag)
	    /*
	       "Help on help" page wanted (`--long-help=?').
	     */
#if USE_PAGER
	    my_help_on_help (stdout, s3, lopt_ambig, tty_cols);
#else /* !USE_PAGER */
	    my_help_on_help (stdout, s3, lopt_ambig, SCREEN_COLS);
#endif /* !USE_PAGER */
	  else if (ext_help_flag)
	    /*
	       "Extended" help pages wanted (`--long-help[=ARG]|[=?']
	       or `-hh' or `-??' or `-h?' or `-?h').
	     */
	    my_extended_help (stdout, lopt_help);
	  else
	    /*
	       "Basic" help page wanted (`--help' or `-h' or `-?').
	     */
	    my_basic_help (stdout);
	}
      else
	{
	  if (license_flag)
	    /*
	       License text wanted (`--license' or `-L').
	     */
	    my_license (stdout);
	  else if (version_flag)
	    /*
	       Version text wanted (`--version' or `-V').
	     */
	    my_version (stdout);
	}
#ifdef GCAL_EPAGER
      if (is_tty1
	  && is_tty2
	  && pager_flag
	  && (ext_pager != (char *) NULL) && (email_adr == (char *) NULL))
	{
	  /*
	     We work unbuffered in this case so we have to flush the
	     stdout buffer for showing all contents of it.
	   */
	  fflush (stdout);
	  /*
	     And reconnect the SYS-STDIN/SYS-STDOUT file descriptors.
	   */
	  close (0);
	  dup (sys_fd[0]);
	  close (1);
	  dup (sys_fd[1]);
	  /*
	     And wait until the child has performed all actions.
	   */
	  while (wait ((int *) NULL) != child_pid)
	    ;
# if HAVE_SIGNAL && defined(SIGPIPE)
	  /*
	     Reset the SIGPIPE signal.
	   */
	  (void) signal (SIGPIPE, SIG_DFL);
# endif
	}
#endif /* GCAL_EPAGER */
      my_exit (exit_stat_help);
    }
  month = 0;
  if (argc > 1)
    {
      auto Bool is_list = FALSE;
      auto Bool is_range = FALSE;
      auto Bool month_set = FALSE;
      auto Bool year_sep_found = FALSE;


      for (; argc > 1; argc--, argv++)
	{
	  if (!month)
	    {
	      option = *argv;
	      if (*option == *MONTH3_LIT)
		break;
	      else
		while (*option)
		  {
		    if (*option == *MLIST_SEP || *option == *YLIST_SEP)
		      is_list = TRUE;
		    else if (*option == *MRANGE_SEP || *option == *YRANGE_SEP)
		      is_range = TRUE;
		    else if (*option == *YEAR_SEP)
		      year_sep_found = TRUE;
		    else if (*option == *FYEAR_SEP)
		      is_fiscal_year = TRUE;
		    option++;
		  }
	      option = *argv;
	      if (!is_list
		  && !is_range
		  && !year_sep_found && !is_fiscal_year && !my_atoi (option))
		{
		  month = compare_d_m_name (option, MOnth);
		  if (month)
		    {
		      month_set = TRUE;
		      continue;
		    }
		}
	    }
	  if (!month)
	    {
	      build_month_list (argv);
	      if (!(*month_list).ml_month)
		month = atoi (*argv);
	      else
		month = (*month_list).ml_month;
	      if (month < MONTH_MIN || month > MONTH_MAX)
		{
		  if (!is_fiscal_year)
		    year = month;
		  month = 0;
		  break;
		}
	    }
	  else if (!year)
	    {
	      if ((*month_list).ml_year && (is_list || is_range))
		year = (*month_list).ml_year;
	      else
		year = my_atoi (*argv);
	      /*
	         Check if a `MM YYYY-YYYY' or a `MM-MM YYYY+YYYY' command
	         is given (MM|MM-MM is already managed), if so, evaluate
	         the year range part.
	       */
	      if (month
		  && isdigit (**argv)
		  && (strchr (*argv, *MRANGE_SEP) != (char *) NULL
		      || strchr (*argv, *YRANGE_SEP) != (char *) NULL))
		{
		  i = 0;
		  while (isdigit (*(*(argv) + i)))
		    {
		      s2[i] = *(*(argv) + i);
		      i++;
		    }
		  if (*(*(argv) + i) == *MRANGE_SEP
		      || *(*(argv) + i) == *YRANGE_SEP)
		    {
		      s2[i++] = '\0';
		      year = my_atoi (s2);
		      if (!year)
			year = act_year;
		      (*month_list).ml_year = year;
		      year = my_atoi (*(argv) + i);
		      if (!year)
			year = act_year;
		      month_list[1].ml_year = year;
		      if (*(*(argv) + i - 1) == *MRANGE_SEP)
			{
			  (*month_list).ml_month = month_list[1].ml_month =
			    month;
			  is_special_range = TRUE;
			}
		      else
			{
			  i = 0;
			  while (month_list[i].ml_month)
			    i++;
			  if (!i)
			    {
			      /*
			         `MM YYYY+YYYY' command given, treat it like
			         the `MM YYYY-YYYY' command.
			       */
			      (*month_list).ml_month =
				month_list[1].ml_month = month;
			      is_special_range = TRUE;
			    }
			  else
			    {
			      /*
			         `MM-MM YYYY+YYYY' command given.
			       */
			      i--;
			      month_list[1].ml_month = month_list[i].ml_month;
			      is_multi_range = TRUE;
			    }
			}
		    }
		  else
		    {
		      year = month;
		      month = 0;
		    }
		  break;
		}
	    }
	}
      /*
         `.' or `..' or `.+' or `.-' argument found: 3 month mode wanted
       */
      if (*option == *MONTH3_LIT)
	{
	  len = (int) strlen (option);
	  if (len <= 2)
	    {
	      if (*(option + 1) == *MONTH3_LIT)
		is_3month_mode2 = TRUE;
	      else
		is_3month_mode = TRUE;
	      is_fiscal_year = TRUE;
	      year_flag = FALSE;
	      month = act_month;
	      /*
	         Check if an explicit year trails the 3 month mode command.
	       */
	      if (argc > 2)
		{
		  argv++;
		  year = atoi (*argv);
		  if (!year)
		    year = act_year;
		}
	      else
		year = act_year;
	      if (is_3month_mode)
		{
		  /*
		     Either `.' or `.+' or `.-' 3 month mode command found.
		   */
		  if (!*(option + 1))
		    {
		      /*
		         `.' 3 month mode command found.
		       */
		      month--;
		      if (!month)
			{
			  month = MONTH_MAX;
			  year--;
			}
		    }
		  else if (*(option + 1) == *DES_LIT)
		    {
		      /*
		         `.-' 3 month mode command found.
		       */
		      month -= 2;
		      if (month < MONTH_MIN)
			{
			  month = MONTH_MAX + month;
			  year--;
			}
		    }
		  else if (*(option + 1) == *ASC_LIT)
		    /*
		       `.+' 3 month mode command found.
		     */
		    ;		/* Void, nothing to do! */
		  else
		    {
		      /*
		         Error, invalid modifier trails the dot character:
		         Print default month calendar only.
		       */
		      is_fiscal_year = FALSE;
		      year = act_year;
		      month = act_month;
		    }
		  if (year < YEAR_MIN)
		    {
		      year = YEAR_MIN;
		      month = act_month;
		    }
		}
	      else
		/*
		   `..' 3 month mode command found.
		 */
		month = ((month + 1) - ((month - 1) % 3)) - 1;
	    }
	}
      else
	if (!month
	    && !year
	    && !month_set && !(*month_list).ml_month && !is_fiscal_year)
	{
	  year = month;
	  month = 0;
	}
    }
}



static void
build_month_list (argv)
     char *argv[];
/*!
   If more than a single month/year is wanted (means list or ranges of
     months or years), fill the global data structure `month_list[]'; which
     is used in the `print_calendar()' function; according the values found
     in the command line.
*/
{
  register int i;
  register int j;
  register int k;
  auto const char *option;
  auto Bool is_list = FALSE;
  auto Bool is_range = FALSE;
  auto Bool year_sep_found = FALSE;
  auto Bool buf_is_ext_year;


  option = *argv;
  for (; *option && !is_list && !is_range; option++)
    if (*option == *MLIST_SEP || *option == *YLIST_SEP)
      {
	is_list = TRUE;
	if (*option == *YLIST_SEP)
	  is_ext_list = is_ext_year = TRUE;
      }
    else if (*option == *MRANGE_SEP || *option == *YRANGE_SEP)
      {
	is_range = TRUE;
	if (*option == *YRANGE_SEP)
	  is_ext_range = is_ext_year = TRUE;
      }
    else if (*option == *YEAR_SEP)
      year_sep_found = TRUE;
  buf_is_ext_year = is_ext_year;
  if (year_sep_found
      || (is_fiscal_year
	  && (is_ext_list
	      || is_ext_year
	      || (!is_list
		  && !is_range)))
      || (!is_fiscal_year && (is_list || is_range)))
    {
      i = 0;
      option = *argv;
      while (*option)
	{
	  year_sep_found = FALSE;
	  j = 0;
	  while (*option
		 && !year_sep_found
		 && ((!is_list
		      && !is_range)
		     || ((is_list
			  && ((*option != *MLIST_SEP)
			      && (*option != *YLIST_SEP)))
			 || (is_range
			     && ((*option != *MRANGE_SEP)
				 && (*option != *YRANGE_SEP))))))
	    {
	      year_sep_found = (Bool) (*option == *YEAR_SEP
				       || *option == *FYEAR_SEP);
	      s1[j++] = *option++;
	    }
	  if (year_sep_found)
	    s1[j - 1] = '\0';
	  else
	    s1[j] = '\0';
	  if (!*s1)
	    {
	      if (is_fiscal_year)
		{
		  j = atoi (option);
		  if (j)
		    {
		      is_ext_year = FALSE;
		      month_list[i++].ml_month = act_month;
		    }
		}
	      else
		{
		  j = 0;
		  while (*option)
		    option++;
		}
	    }
	  else
	    j = my_atoi (s1);
	  if (is_ext_year)
	    {
	      if (is_fiscal_year)
		{
		  if ((j >= MONTH_MIN) && (j <= MONTH_MAX))
		    month_list[i++].ml_month = j;
		  else
		    {
		      /*
		         Is the argument a valid month name?
		       */
		      j = compare_d_m_name (s1, MOnth);
		      if (j)
			month_list[i++].ml_month = j;
		      else
			/*
			   Are we at the end of the argument?
			 */
		      if (!*option)
			{
			  if (is_ext_list || is_ext_range)
			    {
			      if (!is_ext_list)
				month_list[i].ml_month =
				  month_list[i - 1].ml_month;
			      else
				month_list[i].ml_month = MONTH_MIN;
			      j = my_atoi (s1);
			      if (j)
				month_list[i++].ml_year = j;
			      else
				month_list[i].ml_month = 0;
			    }
			}
		      else
			/*
			   Not at the end of the argument:
			   Fiscal year list wanted and standard year given.
			 */
		      if (!year_sep_found)
			{
			  j = my_atoi (s1);
			  if (j)
			    month_list[i++].ml_month = j;
			}
		      else
			/*
			   Otherwise ignore given argument.
			 */
			year_sep_found = FALSE;
		    }
		}
	      else
		{
		  if (year_sep_found)
		    {
		      /*
		         Error, list/range of extended years given like
		         5/1999;may/7777... or 5/1999+may/7777
		         --> respect the year part of the date only.
		       */
		      j = 0;
		      while (*option
			     && ((is_list
				  && (*option != *YLIST_SEP))
				 || (is_range && (*option != *YRANGE_SEP))))
			s1[j++] = *option++;
		      s1[j] = '\0';
		      j = my_atoi (s1);
		    }
		  if (j)
		    month_list[i++].ml_month = j;
		  else if (is_list)
		    month_list[i++].ml_month = act_year;
		}
	      if (year_sep_found)
		{
		  while (*option == *YEAR_SEP || *option == *FYEAR_SEP)
		    option++;
		  if (!is_fiscal_year)
		    {
		      year_sep_found = FALSE;
		      while (*option
			     && ((is_list
				  && (*option != *YLIST_SEP))
				 || (is_range && (*option != *YRANGE_SEP))))
			option++;
		    }
		}
	    }
	  else if (*s1)
	    {
	      if ((j >= MONTH_MIN) && (j <= MONTH_MAX))
		month_list[i++].ml_month = j;
	      else
		{
		  j = compare_d_m_name (s1, MOnth);
		  if (j)
		    month_list[i++].ml_month = j;
		}
	      /*
	         Check if range of a selected month of years is given like:
	         MM YYYY-YYYY or MM/YYYY-YYYY; e.g.: 5/1999-2011
	         which means all May months starting in 1999 and ending in 2011.
	       */
	      if (i
		  && is_range
		  && !year_sep_found && (*month_list).ml_year && !*option)
		{
		  if (j)
		    i--;
		  else
		    j = my_atoi (s1);
		  if (j)
		    {
		      month_list[i].ml_month = month_list[i - 1].ml_month;
		      month_list[i++].ml_year = j;
		      is_ext_range = FALSE;
		      is_special_range = TRUE;
		    }
		}
	    }
	  if (year_sep_found)
	    {
	      if (j)
		{
		  j = 0;
		  while (*option == *YEAR_SEP || *option == *FYEAR_SEP)
		    option++;
		  if (!is_list && !is_range)
		    while (*option)
		      s1[j++] = *option++;
		  else if (is_fiscal_year && (is_ext_list || is_ext_range))
		    while (*option
			   && ((is_list
				&& (*option != *YLIST_SEP))
			       || (is_range && (*option != *YRANGE_SEP))))
		      s1[j++] = *option++;
		  else
		    while (*option
			   && ((is_list
				&& (*option != *MLIST_SEP))
			       || (is_range && (*option != *MRANGE_SEP))))
		      s1[j++] = *option++;
		  s1[j] = '\0';
		  j = my_atoi (s1);
		  if (j)
		    {
		      month_list[i - 1].ml_year = j;
		      if (is_list && !is_ext_range)
			is_ext_list = TRUE;
		      else if (is_range && !is_ext_list)
			is_ext_range = TRUE;
		    }
		  else if (is_fiscal_year)
		    month_list[i - 1].ml_year = act_year;
		}
	      else
		{
		  if (is_fiscal_year)
		    while (*option
			   && ((is_list
				&& (*option != *YLIST_SEP))
			       || (is_range && (*option != *YRANGE_SEP))))
		      option++;
		  else
		    while (*option
			   && ((is_list
				&& (*option != *MLIST_SEP))
			       || (is_range && (*option != *MRANGE_SEP))))
		      option++;
		}
	    }
	  while (*option == *MLIST_SEP
		 || *option == *MRANGE_SEP
		 || *option == *YLIST_SEP || *option == *YRANGE_SEP)
	    option++;
	  /*
	     A list/range of fiscal years is wanted and the specified
	     list/range of years is incomplete --> avoid error.
	   */
	  if (i && is_fiscal_year && !month_list[i - 1].ml_year)
	    {
	      month_list[i - 1].ml_year = month_list[i - 1].ml_month;
	      month_list[i - 1].ml_month = MONTH_MIN;
	    }
	  is_ext_year = buf_is_ext_year;
	  if ((Uint) i >= month_list_max)
	    {
	      /*
	         Resize the `month_list[]' table.
	       */
	      month_list_max <<= 1;
	      if (month_list_max * sizeof (Ml_struct) > testval)
		month_list_max--;
	      month_list = (Ml_struct *) my_realloc ((VOID_PTR) month_list,
						     month_list_max *
						     sizeof (Ml_struct),
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 3L,
						     "month_list[month_list_max]",
						     month_list_max);
	      /*
	         Initialize the `month_list[]' structure elements.
	       */
	      for (k = i; k < (int) month_list_max; k++)
		month_list[k].ml_year = month_list[k].ml_month = 0;
	    }
	}
      if (is_range && !is_ext_range && !is_special_range)
	{
	  /*
	     Fiscal year wanted and a range of months is specified only
	     --> avoid error.
	   */
	  if (is_fiscal_year)
	    is_ext_year = TRUE;
	  else
	    {
	      /*
	         Ok, compute the range of months and put it in the `month_list[]' table.
	       */
	      j = (*month_list).ml_month;
	      k = month_list[1].ml_month;
	      i = 0;
	      while (month_list[i].ml_month)
		month_list[i++].ml_month = 0;
	      if (!j && !k)
		(*month_list).ml_month = act_month;
	      else
		{
		  if (!j)
		    j = MONTH_MIN;
		  if (!k)
		    k = MONTH_MAX;
		  i = 0;
		  if (j > k)
		    for (; j >= k; i++, j--)
		      month_list[i].ml_month = j;
		  else
		    for (; j <= k; i++, j++)
		      month_list[i].ml_month = j;
		}
	    }
	}
      /*
         Fiscal/standard year wanted and an invalid range is specified
         --> avoid error.
       */
      if (is_ext_range && month_list[2].ml_month)
	month_list[1].ml_month = 0;
      /*
         Standard year wanted and the list/range of years is incomplete
         --> avoid error.
       */
      if (!month_list[1].ml_month
	  && (is_ext_list || is_ext_range || is_special_range || is_ext_year))
	{
	  is_ext_list = is_ext_range = is_special_range = is_ext_year = FALSE;
	  (*month_list).ml_month = act_month;
	  (*month_list).ml_year = act_year;
	}
    }
}



static void
eliminate_invalid_data ()
/*!
   Eliminates invalid command line argument values
     and sets some internal variables according to the command line arguments.
*/
{
  register int i;
  register int len;


  /*
     Correction of month and year.
   */
  if (!month && !year)
    {
      month = act_month;
      year = act_year;
    }
  else if (year < YEAR_MIN || year > YEAR_MAX)
    {
      if (month && year)
	month = act_month;
      year = act_year;
    }
  if (year_flag && month)
    month = 0;
  if (month && !(*month_list).ml_month)
    (*month_list).ml_month = month;
  if (year && !(*month_list).ml_year)
    (*month_list).ml_year = year;
  /*
     Set amount of month rows and columns according to the selected
     or default year calendar sheet format.
   */
  if (is_3month_mode || is_3month_mode2)
    {
      /*
         Set fixed amount of month rows and columns for 3 month mode
         calendar sheets; the `-b<>' option is ignored.
       */
      if (special_calsheet_flag)
	{
	  if (cal_special_flag)
	    {
	      out_rows = JI3_OUT_ROWS;
	      out_cols = JI3_OUT_COLS;
	    }
	  else if (cal_both_dates_flag)
	    {
	      out_rows = BI3_OUT_ROWS;
	      out_cols = BI3_OUT_COLS;
	    }
	  else
	    {
	      out_rows = SI3_OUT_ROWS;
	      out_cols = SI3_OUT_COLS;
	    }
	}
      else
	{
	  if (cal_special_flag)
	    {
	      out_rows = J3_OUT_ROWS;
	      out_cols = J3_OUT_COLS;
	    }
	  else if (cal_both_dates_flag)
	    {
	      out_rows = B3_OUT_ROWS;
	      out_cols = B3_OUT_COLS;
	    }
	  else
	    {
	      out_rows = S3_OUT_ROWS;
	      out_cols = S3_OUT_COLS;
	    }
	}
    }
  else
    {
      /*
         Set the number of month columns according to year calendar sheet style
         that is either given by the `-b<>' option or by the default value.
       */
      switch (out_rows)
	{
	case MONTH_MIN:
	  out_cols = MONTH_MAX;
	  break;
	case 2:
	  out_cols = 6;
	  break;
	case 3:
	  out_cols = S_OUT_COLS;
	  break;
	case 4:
	  out_cols = J_OUT_COLS;
	  break;
	case 6:
	  out_cols = 2;
	  break;
	case MONTH_MAX:
	  out_cols = MONTH_MIN;
	  break;
	default:
	  /*
	     This case MUST be an internal error!
	   */
	  abort ();
	}
      /*
         If no `-b<>' option is given, set the year calendar sheet style
         according to the `-j' or `jb' options if given.
       */
      if (!year_flag)
	{
	  if (special_calsheet_flag)
	    {
	      if (cal_special_flag)
		{
		  out_rows = JI_OUT_ROWS;
		  out_cols = JI_OUT_COLS;
		}
	      else if (cal_both_dates_flag)
		{
		  out_rows = BI_OUT_ROWS;
		  out_cols = BI_OUT_COLS;
		}
	    }
	  else
	    {
	      if (cal_special_flag)
		{
		  out_rows = J_OUT_ROWS;
		  out_cols = J_OUT_COLS;
		}
	      else if (cal_both_dates_flag)
		{
		  out_rows = B_OUT_ROWS;
		  out_cols = B_OUT_COLS;
		}
	    }
	}
    }
  /*
     Set the length of a single day "cell" used within the calendar
     sheets according to the selected mode (means either NO `-j' option,
     `-j' or the `-jb' option).
   */
  if (cal_special_flag)
    /*
       +1 because the day of year consists of 3 digits maximum
       plus a separating space (==> 4).
     */
    format_len = FORMAT_LEN_MIN + 1;
  else if (cal_both_dates_flag)
    /*
       +5 because such a date consists of the day of month (2 digits
       maximum) plus the day of year (3 digits maximum) plus two
       textual parantheses plus a separating space (==> 8).
     */
    format_len = FORMAT_LEN_MIN + 5;
  else
    /*
       The day of month consists of 2 digits maximum
       plus a separating space (==> 3).
     */
    format_len = FORMAT_LEN_MIN;
  /*
     Now get the maximum length of a textual day name,
     which is returned by the function `day_name()'.
   */
  len_dayname_max = 0;
  for (i = DAY_MIN; i <= DAY_MAX; i++)
    {
      len = (int) strlen (day_name (i));
      if (len > len_dayname_max)
	len_dayname_max = len;
    }
  /*
     Now get the maximum length of a textual month name,
     which is returned by the function `month_name()'.
   */
  len_monthname_max = 0;
  for (i = MONTH_MIN; i <= MONTH_MAX; i++)
    {
      len = (int) strlen (month_name (i));
      if (len > len_monthname_max)
	len_monthname_max = len;
    }
  /*
     Now get the length of a textual day suffix, which is returned
     by the function `day_suffix()'.  We check the length of the
     ordinal number suffixes:  1st 2nd 3rd 4th.
   */
  len_suffix_max = 0;
  for (i = DAY_MIN; i <= 4; i++)
    {
      len = (int) strlen (day_suffix (i));
      if (len > len_suffix_max)
	len_suffix_max = len;
    }
}



static void
pseudo_blank_conversion (text)
     char **text;
/*!
   Perform conversion of quoted or unquoted PSEUDO_BLANK characters
     in TEXT to real ' ' blank characters.
*/
{
  if (strchr (*text, PSEUDO_BLANK) != (char *) NULL)
    {
      auto char *ptr_char;
      auto char *ptr2_char;


      /*
         Manage quoted or unquoted PSEUDO_BLANK characters.
       */
      ptr_char = ptr2_char = *text;
      while (*ptr2_char)
	{
	  if (*ptr2_char == PSEUDO_BLANK)
	    {
	      ptr2_char++;
	      *ptr_char++ = ' ';
	    }
	  else if (*ptr2_char == QUOTE_CHAR)
	    {
	      ptr2_char++;
	      if (*ptr2_char == PSEUDO_BLANK)
		*ptr_char++ = *ptr2_char++;
	      else
		*ptr_char++ = QUOTE_CHAR;
	    }
	  else
	    *ptr_char++ = *ptr2_char++;
	}
      *ptr_char = '\0';
    }
}



#if USE_RC
static int
further_check (option)
     char **option;
/*!
   Checks whether invalid characters trail the
     argument of the `--period-of-fixed-dates=ARG' option.
     If so, return a non-zero value, otherwise zero.
*/
{
  register int opt_error = 0;


  (*option)++;
  if (**option)
    {
      rc_week_year_flag = (Bool) (tolower (**option) == 'w');
      rc_forwards_flag = (Bool) (**option == *ASC_LIT);
      rc_backwards_flag = (Bool) (**option == *DES_LIT);
      (*option)++;
      if (**option
	  || (!rc_week_year_flag && !rc_forwards_flag && !rc_backwards_flag))
	/*
	   Error, either argument is trailed by an illegal
	   character or no mode specifying character (w|+|-) given.
	 */
	opt_error = 5;
    }

  return (opt_error);
}
#endif
