#ifndef __COMMON_H
# define __COMMON_H
/*!
*  \file common.h
*  \brief Basic program specific statements, macros and typedefs...
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000, 2011, 2012 Thomas Esken
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
*  $Id: common.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  Include (sub)header files   ;<
*/
# if USE_UNICODE
#  include "unistdio.h"
#  include "unistr.h"
#  undef sprintf
#  define sprintf u8_sprintf
#  undef strlen
#  define strlen u8_strlen
# endif
# if HAVE_STDIO_H || STDC_HEADERS
#  include <stdio.h>
# endif
# if HAVE_STDLIB_H || STDC_HEADERS
#  include <stdlib.h>
# endif
# if HAVE_STRING_H || STDC_HEADERS
#  include <string.h>
# else /* !HAVE_STRING_H && !STDC_HEADERS */
#  include <strings.h>
#  if !HAVE_STRCHR
#   ifndef strchr
#    define strchr       index
extern char *strchr __P_ ((const char *s, int c));
#   endif
#  endif /* !HAVE_STRCHR */
#  if !HAVE_STRRCHR
#   ifndef strrchr
#    define strrchr      rindex
extern char *strrchr __P_ ((const char *s, int c));
#   endif
#  endif /* !HAVE_STRRCHR */
# endif	/* !HAVE_STRING_H && !STDC_HEADERS */

# if HAVE_SIGNAL_H && HAVE_SIGNAL
#  if HAVE_SYS_TYPES_H
#   include <sys/types.h>
#  endif
#  include <signal.h>
# endif
# if defined(AMIGA) && !defined(__GNUC__)
extern long atol __P_ ((const char *string));
# endif
# ifdef __50SERIES
extern char *getenv __P_ ((const char *env_var));
#  if HAVE_SIGNAL && defined(SIGTERM)
/* We don't want a signal handler for SIGTERM */
#   undef  SIGTERM
#  endif
# endif



/*
*  Basic preprocessor statements and macros.
*/
# ifdef SPECIAL_VALUE
#  undef  SPECIAL_VALUE
# endif
/*
*  !!! DO NOT TOUCH THIS SPECIAL_VALUE, OTHERWISE YOU DESTROY THE PROGRAM !!!
*/
# define SPECIAL_VALUE    (-1)
# ifndef TRUE
#  define TRUE            (0==0)
# endif
# ifndef FALSE
#  define FALSE           (!TRUE)
# endif
# ifndef __LINE__
#  define __LINE__         0L
# endif
# ifndef __FILE__
#  define __FILE__         "???"
# endif
# ifndef __DATE__
#  define __DATE__         "DD-MMM-YYYY"
# endif
# ifndef __TIME__
#  define __TIME__         "HH:MM:SS"
# endif
# define LOOP              for(;;)
# if HAVE_VOID
#  define VOID_PTR        void *
# else /* !HAVE_VOID */
#  define VOID_PTR        char *
#  define void            int
# endif	/* !HAVE_VOID */
# ifndef RETSIGTYPE
#  define RETSIGTYPE      void
# endif
# if !HAVE_STRSTR
#  ifndef strstr
#   define strstr          my_strstr
#  endif
# endif
# if !HAVE_STRCSPN
#  ifndef strcspn
#   define strcspn          my_strcspn
#  endif
# endif
# if !HAVE_STRCASECMP
#  ifndef strcasecmp
#   define strcasecmp      my_strcasecmp
#  endif
# endif
# if !HAVE_STRNCASECMP
#  ifndef strncasecmp
#   define strncasecmp     my_strncasecmp
#  endif
# endif
# if HAVE_TIME_T
#  define MY_TIME_T       time_t
# else /* !HAVE_TIME_T */
#  define MY_TIME_T       long
# endif	/* !HAVE_TIME_T */
# if !HAVE_UPPER_LOWER
#  ifndef islower
#   define islower(gc_c) (((gc_c) >= 'a') && ((gc_c) <= 'z'))
#  endif
#  ifndef isupper
#   define isupper(gc_c) (((gc_c) >= 'A') && ((gc_c) <= 'Z'))
#  endif
#  ifndef tolower
#   define tolower(gc_c) ((isupper(gc_c)) ? (gc_c) - 'A' + 'a' : (gc_c))
#  endif
#  ifndef toupper
#   define toupper(gc_c) ((islower(gc_c)) ? (gc_c) - 'a' + 'A' : (gc_c))
#  endif
# endif
# if !HAVE_ISDIGIT
#  ifndef isdigit
#   define isdigit(gc_c) (((gc_c) >= '0') && ((gc_c) <= '9'))
#  endif
# endif
# if !HAVE_ISALPHA
#  ifndef isalpha
#   define isalpha(gc_c) (isupper(gc_c) || islower(gc_c))
#  endif
# endif
# if !HAVE_ISALNUM
#  ifndef isalnum
#   define isalnum(gc_c) (isalpha(gc_c) || isdigit(gc_c))
#  endif
# endif
# if !HAVE_ISSPACE
#  ifndef isspace
#   define isspace(gc_c) (   (gc_c) == ' '  || (gc_c) == '\f' || (gc_c) == '\n' \
                              || (gc_c) == '\r' || (gc_c) == '\t' || (gc_c) == '\v')
#  endif
# endif
/*
*  Preprocessor macros used to activate some self-written mathematical
*    functions if no mathematical library is available.
*/
# if !HAVE_LIBM
#  ifdef floor
#   undef  floor
#  endif
#  define floor  my_floor
#  ifdef ceil
#   undef  ceil
#  endif
#  define ceil   my_ceil
#  ifdef sqrt
#   undef  sqrt
#  endif
#  define sqrt   my_sqrt
#  ifdef log
#   undef  log
#  endif
#  define log    my_log
#  ifdef log10
#   undef  log10
#  endif
#  define log10  my_log10
#  ifdef sin
#   undef  sin
#  endif
#  define sin    my_sin
#  ifdef cos
#   undef  cos
#  endif
#  define cos    my_cos
#  ifdef tan
#   undef  tan
#  endif
#  define tan    my_tan
#  ifdef asin
#   undef  asin
#  endif
#  define asin   my_asin
#  ifdef acos
#   undef  acos
#  endif
#  define acos   my_acos
#  ifdef atan
#   undef  atan
#  endif
#  define atan   my_atan
# endif	/* !HAVE_LIBM */



/*!
*  Preprocessor statements for the exit codes.
*/
# define ERR_NONE                      0
/*
*  Common values.
*/
# define ERR_TERMINATION_BY_SIGNAL     3
# define ERR_INVALID_DATE_FORMAT       106
# define ERR_INTERNAL_TABLE_CRASH      107
# define ERR_ILLEGAL_CHAR_IN_FILE      108
# define ERR_READ_FILE                 109
# define ERR_WRITE_FILE                115
# define ERR_NO_MEMORY_AVAILABLE       124
# define ERR_INVALID_EASTER_DATE       125
# define ERR_INVALID_OPTION            126
# define ERR_EXIT_INFO_TEXTS_NON_ZERO  127
/*
*  System/configuration dependent values.
*/
# ifdef GCAL_EMAIL
#  define ERR_EMAIL_SEND_FAILURE        111
# endif
# if defined(GCAL_EPAGER) || defined(GCAL_EMAIL) || USE_RC
#  define ERR_INTERNAL_C_FUNC_FAILURE   110
# endif
/*
*  Force the use of THESE values!
*/
# ifndef EXIT_FATAL
#  define EXIT_FATAL                    2
# endif



/*
*  Program specific preprocessor statements.
*/
/*! Maximum length of a file block buffer. */
# ifndef BUF_LEN
#  define BUF_LEN           16384
# endif

/*! Default length of a "string" text buffer. */
# ifndef MAXLEN_MAX
#  define MAXLEN_MAX       1024
# endif

/*! Default number of command line arguments in `my_argv[]' table. */
# ifndef MY_ARGC_MAX
#  define MY_ARGC_MAX      64
# endif

/*! Gcal's exit status on: help, version, license. */
# ifndef EXIT_STAT_HELP
#  define EXIT_STAT_HELP   EXIT_SUCCESS
# endif

/*! Default length of a displayed (native language translated) text line. */
# ifndef LEN_SINGLE_LINE
#  define LEN_SINGLE_LINE  200
# endif

/*! Maximum amount of debug/warning levels. */
# define WARN_LVL_MAX     4



/*!
*  Preprocessor statements for long options `Lopt_struct' field `symbolic_name'.
*/
# define SYM_NIL                         0
/*
*  Common values.
*/
# define SYM_BLOCKS                      1
# define SYM_CALENDAR_DATES              2
# define SYM_CALENDAR_WITH_WEEK_NUMBER   3
# define SYM_DATE_FORMAT                 4
# define SYM_DEBUG                       5
# define SYM_DESC_HOLIDAY_LIST           6
# define SYM_DISABLE_HIGHLIGHTING        7
# define SYM_EXCLUDE_HD_TITLE            8
# define SYM_EXIT_STAT_HELP_NON_ZERO     9
# define SYM_FORCE_HIGHLIGHTING          10
# define SYM_GREG_REFORM                 11
# define SYM_HELP                        12
# define SYM_HIGHLIGHTING                13
# define SYM_HOLIDAY_DATES               14
# define SYM_HOLIDAY_LIST                15
# define SYM_ISO_WEEK_NUMBER             16
# define SYM_LICENSE1                    17
# define SYM_LICENSE2                    18
# define SYM_LICENSE3                    19
# define SYM_LONG_HELP1                  20
# define SYM_LONG_HELP2                  21
# define SYM_ORTHODOX_CALENDAR           22
# define SYM_RESPONSE_FILE               23
# define SYM_STARTING_DAY                24
# define SYM_SUPPRESS_CALENDAR           25
# define SYM_SUPPRESS_HDLIST_SEP         26
# define SYM_TIME_OFFSET                 27
# define SYM_TRANSFORM_YEAR              28
# define SYM_TRANSLATE_STRING            29
# define SYM_TYPE_OF_CALENDAR            30
# define SYM_VERSION                     31
/*
*  Common holiday list values.
*/
# define SYM_ASTRONOMICAL_HDY            50
# define SYM_MULTICULTURAL_NEW_YEAR_HDY  51
# define SYM_ZODIACAL_MARKER_HDY         52
# define SYM_CC_HDY                      53
/*
*  Calendar specific holiday list values.
*/
# define SYM_BAHAI_HDY                   60
# define SYM_CELTIC_HDY                  61
# define SYM_CHINESE_FLEXIBLE_HDY        62
# define SYM_CHINESE_HDY                 63
# define SYM_CHRISTIAN_HDY               64
# define SYM_HEBREW_HDY                  65
# define SYM_ISLAMIC_HDY                 66
# define SYM_JAPANESE_FLEXIBLE_HDY       67
# define SYM_JAPANESE_HDY                68
# define SYM_ORTHODOX_NEW_HDY            69
# define SYM_ORTHODOX_OLD_HDY            70
# define SYM_PERSIAN_HDY                 71
/*
*  Calendar specific month list values.
*/
# define SYM_BAHAI_MTH                   80
# define SYM_CHINESE_FLEXIBLE_MTH        81
# define SYM_CHINESE_MTH                 82
# define SYM_COPTIC_MTH                  83
# define SYM_ETHIOPIC_MTH                84
# define SYM_FRENCH_REVOLUTIONARY_MTH    85
# define SYM_HEBREW_MTH                  86
# define SYM_INDIAN_CIVIL_MTH            87
# define SYM_ISLAMIC_MTH                 88
# define SYM_JAPANESE_FLEXIBLE_MTH       89
# define SYM_JAPANESE_MTH                90
# define SYM_OLD_ARMENIC_MTH             91
# define SYM_OLD_EGYPTIC_MTH             92
# define SYM_PERSIAN_MTH                 93
/*
*  System dependent values.
*/
# ifdef GCAL_SHELL
#  define SYM_SCRIPT_FILE                 100
# endif
# ifdef GCAL_EMAIL
#  define SYM_MAIL                        101
# endif
/*!
*  Configuration dependent values.
*/
# if USE_PAGER
#  define SYM_PAGER                       150
# endif



/*
*  Preprocessor statements for long options `Lopt_struct' field `larg_mode'.
*/
/*! `--foo'. */
# define LARG_NO          0

/*! `--foo' or `--foo=BAR'. */
# define LARG_NO_OR_ONE   1

/*! `--foo=BAR'. */
# define LARG_ONE         2

/*! `--foo=BAR' or `--foo=ARG'. */
# define LARG_ONE_OR_ARG  3

/*
*  Further preprocessor statements used for long-style options.
*/
/*! Maximum number of arguments a long option may have. */
# define LARG_MAX        6

/*! Argument separator of a long option, e.g. `--foo=BAR'. */
# define LARG_SEP        "="



/*
*  Some other common environment variable names.
*/
/*! Name of the timezone environment variable. */
# define ENV_VAR_TZ    "TZ"

# if USE_PAGER
/*! Name of number of terminal rows environment variable. */
#  define ENV_VAR_LI    "GCAL_LINES"

/*! Name of number of terminal columns environment variable. */
#  define ENV_VAR_CO    "GCAL_COLUMNS"

/*! Name of number of terminal rows environment variable. */
#  define ENV_VAR_LI2   "LINES"

/* Name of number of terminal columns environment variable. */
#  define ENV_VAR_CO2   "COLUMNS"
# endif



/*
*  Further preprocessor symbols.
*/
/*! Comment character of a resource/response file line. */
# define REM_CHAR          ';'

/*! Marker character of a response file (@FILE). */
# define RSP_CHAR          '@'

/*! Character used to quote a newline character etc. */
# define QUOTE_CHAR        '\\'

/*! Character used to replace a QUOTE_CHAR. */
# define PSEUDO_QUOTE      '\001'

/*! Used to insert a blank (' ') in places the OS normally forbits its use. */
# define PSEUDO_BLANK      '_'

/*! Separator of user defined sequences: `-HSEQ:SEQ...' `-vARG:ARG...' `-rARG:ARG...'. */
# define SEP               ":"

/*! Separator used for splitting lists, e.g. `-P e[DATE][,e[DATE],...]. */
# define SPLIT_SEP         ","

/*! Separator used for connecting lists, e.g. `--cc-holidays=CC[+CC+...]'. */
# define CONNECT_SEP       "+"

/*! Default switch char, which defines an command line option. */
# define SWITCH            "-"

/*! First additional switch char. */
# define SWITCH2           "/"

/*! Separator of a list of months (MM,MM). */
# define MLIST_SEP         ","

/*! Separator of a range of months (MM-MM). */
# define MRANGE_SEP        "-"

/*! Separator of a list of years (YYYY;YYYY). */
# define YLIST_SEP         ";"

/*! Separator of a range of years (YYYY+YYYY). */
# define YRANGE_SEP        "+"

/*! Separator of a standard year (MM/YYYY). */
# define YEAR_SEP          "/"

/*! Separator of a fiscal year (MM:YYYY). */
# define FYEAR_SEP         ":"

/*! Command for enabling 3-month mode (.|..|.+|.-). */
# define MONTH3_LIT        "."

/*! Prefix to disable highlighting of a holiday. */
# define DIS_HLS_PREF      "-"

/*! Prefix to disable highlighting of a holiday. */
# define DIS_HLS_PREF2     "*"

/*! Modifier for moving ascending/forwards in date/PLUS sign. */
# define ASC_LIT           "+"

/*! Modifier for moving descending/backwards in date/MINUS sign. */
# define DES_LIT           "-"

/*! Identifier for actual local time based time offsets used in the `--time_offset=ARG' option. */
# if !USE_RC
#  define staticTIME_CHAR    't'
# endif	/* USE_RC */

/*! Default time separator for scanning HH:MM time expressions. */
# define DEFAULT_TIME_SEP  ":"

/*! Mininum length of a day "cell" used in calendar sheets. */
# define FORMAT_LEN_MIN    3

/*! Minimum length of a textual day name. */
# define TXTLEN_DAY        2

/*! Minimum length of a textual month name. */
# define TXTLEN_MONTH      3

/*! Number of standard calsheet format default rows standard date. */
# define S_OUT_ROWS        3

/*! Number of standard calsheet format default columns standard date. */
# define S_OUT_COLS        4

/*! Number of standard calsheet format default rows special date. */
# define J_OUT_ROWS        4

/*! Number of standard calsheet format default columns special date. */
# define J_OUT_COLS        3

/*! Number of standard calsheet format default rows both dates. */
# define B_OUT_ROWS        12

/*! Number of standard calsheet format default columns both dates. */
# define B_OUT_COLS        1

/*! Number of special calsheet format default rows standard date. */
# define SI_OUT_ROWS       4

/*! Number of special calsheet format default columns standard date. */
# define SI_OUT_COLS       3

/*! Number of special calsheet format default rows special date. */
# define JI_OUT_ROWS       6

/*! Number of special calsheet format default columns special date. */
# define JI_OUT_COLS       2

/*! Number of special calsheet format default rows both dates. */
# define BI_OUT_ROWS       B_OUT_ROWS

/*! Number of special calsheet format default columns both dates. */
# define BI_OUT_COLS       B_OUT_COLS

/*! No of standard calsheet format default 3-month mode rows standard date. */
# define S3_OUT_ROWS       1

/*! No of standard calsheet format default 3-month mode columns standard date. */
# define S3_OUT_COLS       3

/*! No of standard calsheet format default 3-month mode rows special date. */
# define J3_OUT_ROWS       S3_OUT_ROWS

/*! No of standard calsheet format default 3-month mode columns special date. */
# define J3_OUT_COLS       S3_OUT_COLS

/*! No of standard calsheet format default 3-month mode rows both dates. */
# define B3_OUT_ROWS       3

/*! No of standard calsheet format default 3-month mode columns both dates. */
# define B3_OUT_COLS       1

/*! No of special calsheet format default 3-month mode rows standard date. */
# define SI3_OUT_ROWS      S3_OUT_ROWS

/*! No of special calsheet format default 3-month mode columns standard date. */
# define SI3_OUT_COLS      S3_OUT_COLS

/*! No of special calsheet format default 3-month mode rows special date. */
# define JI3_OUT_ROWS      B3_OUT_ROWS

/*! No of special calsheet format default 3-month mode columns special date. */
# define JI3_OUT_COLS      B3_OUT_COLS

/*! No of special calsheet format default 3-month mode rows both dates. */
# define BI3_OUT_ROWS      B3_OUT_ROWS

/*! No of special calsheet format default 3-month mode columns both dates. */
# define BI3_OUT_COLS      B3_OUT_COLS



/*
*  Characters used for ordering a displayed date.
*/
/*! Identifier leading the character used for ordering a displayed date. */
# define DFORMAT_CHAR    '%'

/*! %weekday name. */
# define WDNAME_CHAR     'K'

/*! %day number. */
# define DAYNR_CHAR      'D'

/*! %month name. */
# define MONTHNAME_CHAR  'U'

/*! %month number. */
# define MONTHNR_CHAR    'M'

/*! %year number. */
# define YEARNR_CHAR     'Y'

/*! %highlighting sequence 1 enable. */
# define HLS1S_CHAR      '1'

/*! %highlighting sequence 1 disable. */
# define HLS1E_CHAR      '2'



/*
*  The elements used within the optional %... text format field,
*    which template is:  ALIGNMENT [SIGN] [LZERO] WIDTH [STYLE] [SUFFIX] FORMAT
*/
/* Identification values for the different field styles. */
# define FSTYLE_NONE   SPECIAL_VALUE
# define FSTYLE_UPPER  1
# define FSTYLE_LOWER  2
# define FSTYLE_WORD   4

/*! Minimum width of a field. */
# define FWIDTH_MIN    1

/*! Maximum width of a field. */
# define FWIDTH_MAX    256

/*! Field contents is aligned left. */
# define FLEFT_CHAR    '<'

/*! Field contents is centered. */
# define FCENTER_CHAR  ':'

/*! Field contents is aligned right. */
# define FRIGHT_CHAR   '>'

/*! Provides numerical field contents with leading sign. */
# define FSIGN_CHAR    '+'

/*! Provides numerical field contents with leading zero(es) */
# define FLZERO_CHAR   '0'

/*! Field contents is converted to upper-case letters. */
# define FUPPER_CHAR   'u'

/*! Field contents is converted to lower-case letters. */
# define FLOWER_CHAR   'l'

/*! Field contents is converted to capitalized words. */
# define FWORD_CHAR    'w'

/*! Appends ordinal number suffix to numerical field contents. */
# define FSUFFIX_CHAR  '&'

/*! Variable length field format. */
# define FVAR_CHAR     '*'

/*! Fixed length field format. */
# define FFIX_CHAR     '#'

/*
*  Important preprocessor symbols for the internal ranges.
*/
/*! Minimum day of week/month/year. */
# define DAY_MIN     1

/*! Maximum day/amount of days of week. */
# define DAY_MAX     7

/*! Last day in a NON leap year. */
# define DAY_LAST    365

/*! Maximum week number of year. */
# define WEEK_MAX    52

/*! Minimum month of year. */
# define MONTH_MIN   1

/*! Maximum month of year. */
# define MONTH_MAX   12

/*! Highest day number in a month. */
# define MONTH_LAST  31

/*! Minimum year able to compute. */
# define YEAR_MIN    1

/*! Maximum year able to compute. */
# define YEAR_MAX    9999

/*! Operating system standard starting century, DON'T change! */
# define CENTURY     1900

/*! Minimum year for computing Easter Sunday (29+1). */
# define EASTER_MIN  30

/*! Maximum year for computing Easter Sunday. */
# define EASTER_MAX  YEAR_MAX

/*! Maximum number of columns of a month. */
# define MONTH_COLS  6

/*! Maximum number of elements per month (DAY_MAX*MONTH_COLS). */
# define VEC_BLOCK   42

/*! Maximum number of elements per year (VEC_BLOCK*MONTH_MAX). */
# define VEC_ELEMS   504



/*
*  Additional usage texts.
*/
# if !USE_RC
#  define USAGE_RC1  ""
#  define USAGE_RC2  ""
#  define USAGE_RC3  ""
#  define USAGE_RC4  ""
# endif	/* !USE_RC */

# if USE_PAGER
/* Quits the internal pager. */
#  define PAGER_QUIT   "q"
/* Additional usage text. */
#  define USAGE_PAGER  "p"
# else /* !USE_PAGER */
#  define USAGE_PAGER  ""
# endif	/* !USE_PAGER */

# ifdef GCAL_SHELL
/* Additional usage text. */
#  define USAGE_SHELL   "S"
# else /* !GCAL_SHELL */
#  define USAGE_SHELL   ""
# endif	/* !GCAL_SHELL */



/*
*  And last but not least...
*/
# define COPYRIGHT_TXT    "Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken\n" \
"Copyright (c) 2010, 2011, 2012, 2014 Free Software Foundation, Inc."
# define BUG_REPORT_ADR1  "bug-gcal@gnu.org"

# define HOMEPAGE "http://www.gnu.org/software/gcal"
# define HOMEPAGE_GNU_SOFTWARE "http://www.gnu.org/gethelp/"



/*!
*  Number of days between 1.5-Jan-4713 BCE and 1.5-Jan-0001 CE.
*/
# define MIN_BCE_TO_1_CE   1721424L



/*
*  Some important mathematical constants.
*/
# define MY_PI             3.14159265358979323846
# define MY_TWO_PI         6.28318530717958647692
# define MY_HALF_PI        1.57079632679489661923



/*
*  Common preprocessor macros.
*/
# ifdef S_NEWLINE
#  undef  S_NEWLINE
# endif
# define S_NEWLINE(gc_fp)    fputc('\n', (gc_fp))
# ifdef MIN
#  undef  MIN
# endif
# define MIN(gc_x1, gc_x2)   (((gc_x1) < (gc_x2)) ? (gc_x1) : (gc_x2))
# ifdef MAX
#  undef  MAX
# endif
# define MAX(gc_x1, gc_x2)   (((gc_x1) > (gc_x2)) ? (gc_x1) : (gc_x2))
# ifdef SYEAR
#  undef  SYEAR
# endif
# define SYEAR(gc_d, gc_s)   (  ((gc_d)-(gc_s)+1) < DAY_MIN) \
                                ? ((gc_d)-(gc_s)+(DAY_MAX+1)) \
                                : ((gc_d)-(gc_s)+1)
# ifdef SMONTH
#  undef  SMONTH
# endif
# define SMONTH(gc_d, gc_s)  (  ((gc_d)+(gc_s)-1) > MONTH_MAX) \
                                ? ((gc_d)+(gc_s)-(MONTH_MAX+1)) \
                                : ((gc_d)+(gc_s)-1)
# ifdef SDAY
#  undef  SDAY
# endif
# define SDAY(gc_d, gc_s)    (  ((gc_d)+(gc_s)-1) > DAY_MAX) \
                                ? ((gc_d)+(gc_s)-(DAY_MAX+1)) \
                                : ((gc_d)+(gc_s)-1)



/*
*  Some useful mathematical functions.
*/
/*!
*  Reduce a degree value to a value within circle range.
*/
# ifdef FIXANGLE
#  undef FIXANGLE
# endif
# define FIXANGLE(a)  ((a) - 360.0 * (floor((a) / 360.0)))
/*!
*  Degrees to Radians := degree_value * Pi / 180.
*/
# ifdef TORAD
#  undef TORAD
# endif
# define TORAD(r)  ((r)*0.017453292519943295769)
/*!
*  Radians to Degrees := radian_value * 180 / Pi.
*/
# ifdef TODEG
#  undef TODEG
# endif
# define TODEG(d)  ((d)*57.295779513082320877)
/*!
*  Absolute value of a number := |x|.
*/
# ifdef abs
#  undef  abs
# endif
# define abs(x)       (((x) < 0) ? (-(x)) : (x))
/*!
*  Sign of a number.
*/
# ifdef SGN
#  undef  SGN
# endif
# define SGN(x)       (((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))
/*!
*  Round-up factor.
*/
# ifdef ROUND
#  undef  ROUND
# endif
# define ROUND(x)     ((abs(x)+0.5)*SGN(x))


/*
*  Some time values.
*/
# define HOURS_PER_DAY       24
# define HOURS_PER_HALF_DAY  12
# define MINS_PER_DAY        1440
# define MINS_PER_HOUR       60
# define SECS_PER_MIN        MINS_PER_HOUR
# define SECS_PER_HOUR       3600
# define SECS_PER_DAY        86400L



/*
*  Some degree values.
*/
# define DEGS_PER_HOUR        15.0	/* 2 Pi in degrees / 24.0 hours. */
# define DEGS_PER_06_HOURS    90.0	/* Pi / 2 in degrees. */
# define DEGS_PER_12_HOURS   180.0	/* Pi in degrees. */
# define DEGS_PER_24_HOURS   360.0	/* 2 Pi in degrees. */


/*
*  Some degree to time and vice-versa conversion functions.
*/
/*!
*  Degrees to Days := degree_value / 360.0 (15 degrees * 24 hours [2 Pi]).
*/
# define DEG2DAY(d)      ((d) / DEGS_PER_24_HOURS)
/*!
*  Degrees to Hours := degree_value / 15.0 (360 degrees / 24 hours).
*/
# define DEG2HH(d)       ((d) / DEGS_PER_HOUR)
/*!
*  Degrees to Minutes := degree_value * 60.0.
*/
# define DEG2MM(d)       ((d) * (double)MINS_PER_HOUR)
/*!
*  Degrees to Seconds := degree_value * 3600.0 (60 minutes * 60 seconds).
*/
# define DEG2SS(d)       ((d) * (double)SECS_PER_HOUR)
/*!
*  Days to Degrees := day_value * 360.0 (15 degrees * 24 hours [2 Pi]).
*/
# define DAY2DEG(x)      ((x) * DEGS_PER_24_HOURS)
/*!
*  Hours to Degrees := hour_value * 15.0 (360 degrees / 24 hours).
*/
# define HH2DEG(h)       ((h) * DEGS_PER_HOUR)
/*!
*  Minutes to Degrees := minute_value / 60.0.
*/
# define MM2DEG(m)       ((m) / (double)MINS_PER_HOUR)
/*!
*  Seconds to Degrees := second_value / 3600.0 (60 minutes * 60 seconds).
*/
# define SS2DEG(s)       ((s) / (double)SECS_PER_HOUR)
/*!
*  Hours to Days := hour_value / 24.0.
*/
# define HH2DAY(h)       ((h) / (double)HOURS_PER_DAY)
/*!
*  Minutes to Days := minute_value / 1440.0 (24 hours * 60 minutes).
*/
# define MM2DAY(m)       ((m) / (double)MINS_PER_DAY)
/*!
*  Seconds to Days := second_value / 86400.0 (24 hours * 3600 seconds).
*/
# define SS2DAY(s)       ((s) / (double)SECS_PER_DAY)
/*!
*  Days to Hours := day_value * 24.0.
*/
# define DAY2HH(x)       ((x) * (double)HOURS_PER_DAY)
/*!
*  Days to Minutes := day_value * 1440.0 (24 hours * 60 minutes).
*/
# define DAY2MM(x)       ((x) * (double)MINS_PER_DAY)
/*!
*  Days to Seconds := day_value * 86400.0 (24 hours * 3600 seconds).
*/
# define DAY2SS(x)       ((x) * (double)SECS_PER_DAY)
/*!
*  Hours and Minutes to Days := (hour_value * 60 + min_value) / 1440.0.
*/
# define HHMM2DAY(h, m)  (((h) * MINS_PER_HOUR + (m)) / (double)MINS_PER_DAY)
/*!
*  Hours and Minutes to Hours := hour_value + min_value / 60.0.
*/
# define HHMM2HH(h, m)   ((h) + (m) / (double)MINS_PER_HOUR)
/*!
*  Hours and Minutes to Minutes := hour_value * 60 + min_value.
*/
# define HHMM2MM(h, m)   ((h) * MINS_PER_HOUR + (m))
/*!
*  Hours and Minutes to Seconds := hour_value * 3600 + min_value * 60.
*/
# define HHMM2SS(h, m)   ((h) * SECS_PER_HOUR + (m) * SECS_PER_MIN)
/*!
*  Hours to Minutes.
*/
# define HH2MM(x)        DEG2MM(x)
/*!
*  Hours to Seconds.
*/
# define HH2SS(x)        DEG2SS(x)
/*!
*  Minutes to Hours.
*/
# define MM2HH(x)        MM2DEG(x)
/*!
*  Minutes to Seconds.
*/
# define MM2SS(x)        DEG2MM(x)
/*!
*  Seconds to Hours.
*/
# define SS2HH(x)        SS2DEG(x)
/*!
*  Seconds to Minutes.
*/
# define SS2MM(x)        MM2DEG(x)



# ifdef CUT_LONGNAMES
/*
*  Cut resp., replace some long variable or function names to 6 significant
*    places so ANY linker should be able to bind an executable file.
*/
#  define bahai_hdy                           F_bhd
#  define bahai_mth                           F_bmt
#  define chinese_hdy                         F_chd
#  define chinese_mth                         F_cmt
#  define compare_d_m_name                    F_cdm
#  define comparemode                         V_cm
#  if !USE_RC
#   define days_of_feb                         V_dof
#  endif
#  define decode_date_format                  F_ddf
#  define decode_eclipse                      F_de
#  define decode_format                       F_df
#  define days_of_february                    F_dof
#  if USE_HLS || USE_PAGER
#   define get_termcap_hls                     F_ghl
#   define get_termcap_scr_attrib              F_gsc
#  endif
#  define get_tty_hls                         F_gth
#  if USE_PAGER
#   define get_tty_scr_size                    F_gts
#  endif
#  define hd_title_flag                       V_htf
#  define hd_title_shown                      V_hts
#  define hebrew_hdy                          F_hhd
#  define hebrew_mth                          F_hmt
#  define hdy_chinese                         V_hc
#  define hdy_chinese_flexible                V_hcf
#  define hdy_christian                       V_hcr
#  define hdy_japanese                        V_hj
#  define hdy_japanese_flexible               V_hjf
#  define hdy_orthodox_new                    V_hon
#  define hdy_orthodox_old                    V_hoo
#  define holiday                             V_hd
#  define holiday_flag                        V_hf
#  define holiday_name                        V_hn
#  define holiday_prefix                      V_hp
#  define is_3month_mode                      V_3m
#  define is_3month_mode2                     V_3m2
#  define is_ext_list                         V_iel
#  define is_ext_range                        V_ier
#  define is_ext_year                         V_iey
#  define is_tty                              V_it
#  define is_tty1                             V_it1
#  define is_tty2                             V_it2
#  define islamic_hdy                         F_ihd
#  define islamic_mth                         F_imt
#  define longopt                             V_lo
#  define longopt_symbolic                    V_los
#  define module_line                         V_mdl
#  define module_name                         V_mdn
#  define month_list                          V_ml
#  define month_list_max                      V_mlm
#  define month_name                          F_mn
#  define month_set                           V_ms
#  define moon_corrected_anomaly              V_mca
#  define moon_corrected_longitude            V_mcl
#  define moon_correction_equation_of_center  V_mce
#  define moon_correction_term1               V_mc1
#  define moon_correction_term2               V_mc2
#  define moon_mean_anomaly                   V_mma
#  define moon_mean_longitude                 V_mml
#  define moon_present_age                    V_mpa
#  define moon_present_longitude              V_mpl
#  define moon_present_phase                  V_mpp
#  define mth_chinese                         V_mtc
#  define mth_chinese_flexible                V_mcf
#  define mth_japanese                        V_mtj
#  define mth_japanese_flexible               V_mjf
#  define mth_old_armenian                    V_moa
#  define mth_old_egyptian                    V_moe
#  define my_argc                             V_mac
#  define my_argc_max                         V_max
#  define my_argv                             V_mav
#  if !HAVE_LIBM
#   define my_atan                             V_ma1
#   define my_atan2                            V_ma2
#  endif
#  define my_help_head_text                   F_mhh
#  define my_help_tail_text                   F_mht
#  if !HAVE_LIBM
#   define my_log                              V_mle
#   define my_log10                            V_ml1
#  endif
#  define my_strstr                           F_mss
#  define my_strcspn                          F_msp
#  define my_strcasecmp                       F_msc
#  define my_strncasecmp                      F_msn
#  define orthodox_calendar                   V_oc
#  define orthodox_easter                     F_oe
#  define orthodox_hdy                        F_oh
#  define persian_hdy                         F_phy
#  define persian_mth                         F_pmt
#  define print_all_holidays                  F_pah
#  define print_calendar                      F_pc
#  define print_highlighted_date              F_phd
#  if USE_HLS
#   define print_hls                           V_phl
#  endif
#  define print_text                          F_ptx
#  define print_unhighlighted_date            F_pud
#  define ptr_cc                              V_pcc
#  define ptr_cc_holidays                     V_pcd
#  define ptr_cc_id                           V_pid
#  define ptr_info_date_format                V_pdf
#  define short_day_name                      F_sdn
#  define short_month_name                    F_smn
#  define special_flag                        V_sf
#  define special_vector                      V_sv
#  define users_date_format                   V_ud
#  define users_greg                          V_ug
#  define use_day_suffix                      V_uds
#  define use_day_zeroleaded                  V_udz
# endif	/* CUT_LONGNAMES */



/*
*  Type definitions.
*/
typedef unsigned char Uchar;

typedef unsigned int Uint;

# ifdef DJG
typedef
#  if defined(__STDC__) && __STDC__
  unsigned short int
#  else				/* !__STDC__ */
  unsigned short
#  endif			/* !__STDC__ */
  Usint;
# endif	/* DJG */

typedef
# if defined(__STDC__) && __STDC__
  signed long int
# else				/* !__STDC__ */
  long
# endif				/* !__STDC__ */
  Slint;

typedef
# if defined(__STDC__) && __STDC__
  unsigned long int
# else				/* !__STDC__ */
  unsigned long
# endif				/* !__STDC__ */
  Ulint;

typedef int Bool;

/*!
*  The quicksort (`qsort()') compare function type.
*/
typedef int (*Cmp_func) __P_ ((const VOID_PTR, const VOID_PTR));

# if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
/*!
*  The signal (`signal()') handler function return type.
*/
typedef RETSIGTYPE (*Sig_type) __P_ ((int));
# endif

/*!
*  The long option record, see file `gcal.c' for a detailed description of it!
*/
typedef struct long_option_type
{
  int symbolic_name;
  char *long_name;
  char *short_name[LARG_MAX];
  int larg_mode;
  char *largs[LARG_MAX];
}
Lopt_struct;

/*!
*  The highlighting sequence record.
*/
typedef struct hls_type
{
  const char *seq;
  int len;
}
Hls_struct;

/*!
*  The list/range of months/years record.
*/
typedef struct ml_type
{
  int ml_month;
  int ml_year;
}
Ml_struct;

/*!
*  The supported date format record.
*/
typedef struct date_format_type
{
  /* Identifying text of the format. */
  char *df_id;

  /* Description text of the format. */
  char *df_info;

  /* The format text itself. */
  char *df_format;
}
Df_struct;


/*!
*  The Gregorian Reformation date record.
*/
typedef struct greg_type
{
  /* Year of Gregorian Reformation. */
  int year;

  /* Month of Gregorian Reformation. */
  int month;

  /* First missing day of Reformation period. */
  int first_day;

  /* Last missing day of Reformation period. */
  int last_day;
}
Greg_struct;

/*!
*  General purpose enumerations.
*/
typedef enum compare_mode
{
  DAy = 1,
# if USE_RC
  DVar,
  TVar,
  EAster,
  TOday,
  WEek,
# endif
  MOnth
}
Cmode_enum;

typedef enum file_mode
{
  REsponse = 1,
# if USE_RC
  USr_include,
  SYs_include,
  REsource,
  HEre,
# endif
# ifdef GCAL_SHELL
  SCript,
# endif
  COmmon
}
Fmode_enum;
#endif /* __COMMON_H */
