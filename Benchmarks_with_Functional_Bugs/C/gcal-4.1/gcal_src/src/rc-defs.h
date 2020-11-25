#ifndef __RC_DEFS_H
# define __RC_DEFS_H
/*!
*  \file rc-defs.h
*  \brief Fixed dates/resource file specific statements, macros,
*  			typedefs, global variable declarations...
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000, 2011 Thomas Esken
*  Copyright (c) 2010, 2011, 2013 Free Software Foundation, Inc.
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
*  $Id: rc-defs.h 3.01 2000/06/20 03:00:01 tom Exp $
*/



/*
*  Preprocessor statements for the exit codes.
*/
/*
*  Configuration dependent values.
*/
# define ERR_NO_FIXED_DATES_LISTED   1
# define ERR_EXTERNAL_CMD_FAILURE    2
# define ERR_MAINTAINER_FAILURE      50
# define ERR_INVALID_REGEX_PATTERN   104
# define ERR_ILLEGAL_VAR_DEFINITION  105
# define ERR_INVALID_VAR_ASSIGNMENT  112
# define ERR_INVALID_VAR_REFERENCE   113
# define ERR_ILLEGAL_VAR_OPERATION   114
# define ERR_NO_SEPARATOR_CHAR       116
# define ERR_INVALID_NWD_FIELD       117
# define ERR_FILE_NOT_FOUND          118
# define ERR_CYCLIC_INCLUDE          119
# define ERR_MALFORMED_INCLUDE       120
# define ERR_INVALID_DAY_FIELD       121
# define ERR_INVALID_MONTH_FIELD     122
# define ERR_INVALID_DATE_FIELD      123



/*
*  Preprocessor statements for long options `Lopt_struct' field `symbolic_name'.
*/
/* Configuration dependent values. */
# define SYM_ADJUST_VALUE            200
# define SYM_ALTERNATIVE_FORMAT      201
# define SYM_ATMOSPHERE              202
# define SYM_BIORHYTHM_AXIS_LEN      203
# define SYM_CYCLE_END               204
# define SYM_CYCLE_STEP              205
# define SYM_DATE_VARIABLE           206
# define SYM_DESC_FIXED_DATES_LIST   207
# define SYM_END_OF_MONTH            208
# define SYM_END_OF_WEEK             209
# define SYM_END_OF_YEAR             210
# define SYM_EXCLUDE_RC_TITLE        211
# define SYM_EXECUTE_COMMAND         212
# define SYM_extern_static_DVARS      213
# define SYM_extern_static_TVARS      214
# define SYM_FILTER_DAY              215
# define SYM_FILTER_PERIOD           216
# define SYM_FILTER_TEXT             217
# define SYM_FIXED_DATES             218
# define SYM_FIXED_DATES_LIST        219
# define SYM_GROUPING_TEXT           220
# define SYM_HEADING_TEXT            221
# define SYM_HERE_FILE               222
# define SYM_IGNORE_CASE             223
# define SYM_INCLUDE_CONS_NUMBER     224
# define SYM_INCLUDE_FILENAME        225
# define SYM_INCLUDE_HOLIDAY         226
# define SYM_INCLUDE_TODAY           227
# define SYM_INCLUDE_WEEK_NUMBER     228
# define SYM_LEAP_DAY                229
# define SYM_LIMIT                   230
# define SYM_LIST_MODE               231
# define SYM_MONTH                   232
# define SYM_MOONIMAGE_LINES         233
# define SYM_OMIT_DATE_PART          234
# define SYM_PERIOD                  235
# define SYM_PRECISE                 236
# define SYM_RESOURCE_FILE           237
# define SYM_REVERT_MATCH            238
# define SYM_START_OF_MONTH          239
# define SYM_START_OF_WEEK           240
# define SYM_START_OF_YEAR           241
# define SYM_SUPPRESS_DATE_PART      242
# define SYM_SUPPRESS_FDLIST_SEP     243
# define SYM_SUPPRESS_TEXT_PART      244
# define SYM_TEXT_VARIABLE           245
# define SYM_TODAY                   246
# define SYM_TOMORROW                247
# define SYM_WEEK                    248
# define SYM_YEAR                    249
# define SYM_ZERO_DATES              250



/*
*  Further preprocessor symbols.
*/
/*! Number of default 2-month mode rows both dates. */
# define A2_OUT_ROWS  1

/*! Number of default 2-month mode columns both dates. */
# define A2_OUT_COLS  2

/*! Number of default 2-month mode rows both dates. */
# define B2_OUT_ROWS  2

/*! Number of default 2-month mode columns both dates. */
# define B2_OUT_COLS  1

/*
*  Additional usage texts.
*/
# define USAGE_RC1  "CDFIP"
# define USAGE_RC2  "cf"
# define USAGE_RC3  "r"
# define USAGE_RC4  "v"



/*
*  Now define the user and system data directories of Gcal's resource files.
*  Please note:  If your compiler does not support the ANSI-C string
*                  concatenation feature like `"foo""bar" => "foobar"'
*                  you have to provide your Makefile with the appropriate
*                  texts referenced by GCAL_USR_DATADIR and GCAL_SYS_DATADIR
*                  to avoid compilation errors.
*/
/*! Access file relative to $HOME directory. */
# ifndef GCAL_USR_DATADIR
#  define GCAL_USR_DATADIR  "share"DIR_SEP""PACKAGE_NAME
# endif

/*! Access file absolute to root directory. */
# ifndef GCAL_SYS_DATADIR
#  define GCAL_SYS_DATADIR  DIR_SEP"usr"DIR_SEP"local"DIR_SEP"share"DIR_SEP""PACKAGE_NAME
# endif



/*
*  Specific preprocessor statements for fixed dates/resource files.
*/
/*! Default number of `rc_elems_table[]' entries. */
# ifndef RC_ELEMS_MAX
#  define RC_ELEMS_MAX             1024
# endif

/*! Default number of `rc_files_table[]' entries. */
# ifndef RC_FILES_MAX
#  define RC_FILES_MAX             8
# endif

/*! Maximum number of date variables. */
# define RC_DVAR_MAX              26

/*! Maximum number of text variables. */
# define RC_TVAR_MAX              RC_DVAR_MAX

/*! Separator of a list of days (WWW,WWW... or NWWW,NWWW... or WWWN,WWWN...). */
# define RC_DLIST_CHAR            ','

/*! Separator of a range of days (WWW#WWW or NWWW#NWWW or WWWN#WWWN). */
# define RC_DRANGE_CHAR           '#'

/*! Separator of a "repeat for N days since..." coding (e.g.: YYYYMMDD:10). */
# define RC_REPEAT_CHAR           ':'

/*! Separator of a "appears each N'th day since..." coding (e.g.: YYYYMMDD:10.3). */
# define RC_APPEARS_CHAR          '.'

/*! Leading char of a modified actual date %DATE. */
# define RC_ADATE_CHAR            '%'

/*! RC-file holiday/date-variable prefix in date part. */
# define RC_HDY_CHAR              '@'

/*! RC-file N'th weekday prefix in date part. */
# define RC_NWD_CHAR              '*'

/*! Inserts a real newline '\n' character if found and breaks to colomn 1. */
# define RC_NL_CHAR               '~'

/*! Inserts a real newline '\n' character if found. */
# define RC_NL2_CHAR              '^'

/*! Leading char of a @easter date part. */
# define RC_EASTER_CHAR           'e'

/*! Leading char of a @today date part. */
# define RC_TODAY_CHAR            't'

/*! Modifier for selecting the 12-hrs format in %[gm]time* or %sun* or %moon* special texts. */
# define RC_12_HOURS_FORMAT_CHAR  '*'

/*! Assignment operator of a date/text variable (DVAR=MMDD, $TVAR=TEXT). */
# define RC_VAR_ASSIGN            "="

/*! Interpreted command assignment of a text variable ($TVAR?COMMAND). */
# define RC_TVAR_ICMD_ASSIGN      "?"

/*! Uninterpreted command assignment of a text variable ($TVAR:COMMAND). */
# define RC_TVAR_UCMD_ASSIGN      ":"

/*! Add operator of a date/text variable ([D|$T]VAR++ | [D|$T]VAR+=N ...). */
# define RC_VAR_ADD               "+"

/*! Subtract operator of a date variable ([D|$T]VAR-- | [D|$T]VAR-=N ...). */
# define RC_VAR_SUB               "-"

/*! RC-file text variable prefix. */
# define RC_TVAR_CHAR             '$'

/*! Default grouping text of fixed date list option `-cg<TEXT>'. */
# define RC_GROUP_SEP             ""

/*! Name of RC-file include statement (upper-case). */
# define RC_INCL_STMENT           "#include"

/*! User include file name starts with. */
# define RC_INCL_USR_ID           "\""

/*! User include file name ends with. */
# define RC_INCL_USR_OD           RC_INCL_USR_ID

/*! System include file name start with. */
# define RC_INCL_SYS_ID           "<"

/*! System include file name ends with. */
# define RC_INCL_SYS_OD           ">"

/*! Environment variable name for user datadir. */
# define ENV_VAR_USR_DATADIR      "GCAL_USR_DATADIR"

/*! Environment variable name for system datadir. */
# define ENV_VAR_SYS_DATADIR      "GCAL_SYS_DATADIR"

/*! Name part of default resource file. */
# define RC_SUFFIX                "rc"

/*! Text displayed if an invalid parameter is given to %[gm]time* or %distance or %sun* or %moon*. */
# define RC_INVALID_PARAM_TEXT    "??"

/*! Text displayed if an invalid value is computed in %[gm]time* or %distance or %sun* or %moon*. */
# define RC_INVALID_VALUE_TEXT    "**"

/*! Text displayed if there is no rise/set/transit time because the astronomical
   object is always above the requested horizon in %sun* or %moon*. */
# define RC_A_ABOVE_VALUE_TEXT    "++"

/*! Text displayed if there is no rise/set/transit time because the astronomical
   object is always below the requested horizon in %sun* or %moon*. */
# define RC_A_BELOW_VALUE_TEXT    "--"

/*! Separator used between two coordinates in %distance. */
# define RC_COORDINATE_SEP        "/"

/*! Modifier for selecting the statute miles format in %distance special texts. */
# define RC_SMILES_FORMAT_CHAR    RC_12_HOURS_FORMAT_CHAR

/*! Output redirection symbol. */
# ifndef REDIRECT_OUT
#  define REDIRECT_OUT              ">"
# endif

/*! Pipelining symbol. */
# ifndef PIPELINE
#  define PIPELINE                  "|"
# endif

/*! Name of default Txt2gcal program if $TXT2GCALPROG is not set. */
# ifndef TXT2GCAL_PRGR
#  define TXT2GCAL_PRGR            "txt2gcal"
# endif

/*! Name of environment variable for accessing the Txt2gcal program. */
# ifndef ENV_VAR_TXT2GCALPROG
#  define ENV_VAR_TXT2GCALPROG     "TXT2GCALPROG"
# endif



/*!
*  Kilometers to statute miles.
*/
# define KM2SM(x)  ((x) * 0.621371192237)
/*!
*  Astronomical units to kilometers.
*/
# define AU2KM(x)  ((x) * ASTRONOMICAL_UNIT)
/*!
*  Earth-equator radii to kilometers.
*/
# define ER2KM(x)  ((x) * EQUATOR_EARTH_RADIUS * 0.001)



/*
*  The suffixes used by the Moon phase special texts.
*/
# define MOON_MIN    "!"
# define MOON_MAX    "@"
# define MOON_WANES  "-"
# define MOON_WAXES  "+"

/*
*  The minimum, maximum and default number of lines of the Moon phase text graphics image.
*/
# define MOONIMAGE_MIN  6
# define MOONIMAGE_MAX  30
# define MOONIMAGE_DEF  12

/*
*  The widgets of the Moon phase text graphics image.
*/
# define MOONIMAGE_BRIGHT  MOON_MAX
# define MOONIMAGE_LEDGE   "("
# define MOONIMAGE_REDGE   ")"



/*
*  Some constant values necessary for using the %distance*
*  or %sun* and %moon* special texts.
*/
# define COOR_LON_DEG_MAX  ((int)DEGS_PER_12_HOURS)
# define COOR_LON_MIN_MAX  MINS_PER_HOUR
# define COOR_LON_SEC_MAX  SECS_PER_MIN
# define COOR_LAT_DEG_MAX  ((int)DEGS_PER_06_HOURS)
# define COOR_LAT_MIN_MAX  MINS_PER_HOUR
# define COOR_LAT_SEC_MAX  SECS_PER_MIN



/*
*  The astronomical objects.
*/
# define AOBJ_NONE  SPECIAL_VALUE
# define AOBJ_SUN   0
# define AOBJ_MOON  1



/*!
*  The codes that define the output and error format types used for
*  formatting the data of the %sun*, %moon* and %distance special texts.
*/
# define FMT_NIL   SYM_NIL
/*! Number with[out] fraction. */
# define FMT_NNN   1
/*! Number with fraction. */
# define FMT_NND   2
/*! Number with sign and fraction. */
# define FMT_NSD   3
/*! Hour and minute [and second+fraction]. */
# define FMT_HHH   4
/*! Value in hours and minutes [and seconds+fraction]. */
# define FMT_TTT   5
/*! Value in hours and minutes [and seconds+fraction] with sign. */
# define FMT_TTS   6
/*! Value in hours and minutes with sign. */
# define FMT_TXS   7
/*! Degree and minute [and second+fraction]. */
# define FMT_DDD   8
/* Degree and minute [and second+fraction] with sign. */
# define FMT_DDS   9
# define ERR_NIL   FMT_NIL
# define ERR_NNN   1
# define ERR_HHH   2
# define ERR_TTT   3
# define ERR_DDD   4



/*
*  The suffixes used by the biorhythm special texts.
*/
# define BIO_WANES     MOON_WANES
# define BIO_WAXES     MOON_WAXES
# define BIO_STABLE    MOON_MAX
# define BIO_CRITICAL  MOON_MIN
# define BIO_OVERLAPS  BIO_STABLE

/*
*  The minimum, maximum and default single length of the biorhythm text graphics bar.
*/
/*! Minimum value that is divided by 2 without a remainder. */
# define BIO_AXIS_MIN  1

/*! Maximum value that is divided by 2 without a remainder. */
# define BIO_AXIS_MAX  100

/*! Default value that divides BIO_AXIS_MAX without a remainder. */
# define BIO_AXIS_DEF  20

/*
*  Some scaling symbols/macros needed for creating the biorhythm text graphics bar.
*/
# define BIO_AXIS_EXTRA     2
# define BIO_AXIS_SCALE(x)  (100/(x))
# define BIO_AXIS_ZERO(x)   (BIO_AXIS_EXTRA+(x))
# define BIO_AXIS_TOTAL(x)  (((x)*2)+(BIO_AXIS_EXTRA*2)+2)



/*! Default timestep value in minutes used within a cycle. */
# define DEFAULT_CYCLE_STEP  1



/*!
*  Prefix character of a "special text" given in a line of a Gcal resource file.
*/
# define RC_SPECIAL_TEXT_CHAR   DFORMAT_CHAR

/*
*  %?[TEXT_TILL_EOL]  ---  Runs commands.
*/
/*! %shell escape special text. */
# define RC_SHELL_ESC_CHAR      '!'

/*
*  %?TEXT  ---  Inserts contents of environment variable.
*/
/*! %shell environment variable special text. */
# define RC_ENV_VAR_CHAR        '-'

/*
*  %?  ---  Excludes special dates.
*/
/*! Exclude if date is in eternal holiday list (legal days only). */
# define RC_EX_LHDY_CHAR        'V'

/*! Exclude if date is not in eternal holiday list (legal days only). */
# define RC_EX_NLHDY_CHAR       'v'

/*! Exclude if date is in eternal holiday list (all days). */
# define RC_EX_AHDY_CHAR        'X'

/*! Exclude if date is not in eternal holiday list (all days). */
# define RC_EX_NAHDY_CHAR       'x'

/*! Exclude if date is a Monday. */
# define RC_EX_MON_CHAR         'A'

/*! Exclude if date is no Monday. */
# define RC_EX_NMON_CHAR        'a'

/*! Exclude if date is a Tuesday. */
# define RC_EX_TUE_CHAR         'C'

/*! Exclude if date is no Tuesday. */
# define RC_EX_NTUE_CHAR        'c'

/*! Exclude if date is a Wednesday. */
# define RC_EX_WED_CHAR         'F'

/*! Exclude if date is no Wednesday. */
# define RC_EX_NWED_CHAR        'f'

/*! Exclude if date is a Thursday. */
# define RC_EX_THU_CHAR         'G'

/*! Exclude if date is no Thursday. */
# define RC_EX_NTHU_CHAR        'g'

/*! Exclude if date is a Friday. */
# define RC_EX_FRI_CHAR         'H'

/*! Exclude if date is no Friday. */
# define RC_EX_NFRI_CHAR        'h'

/*! Exclude if date is a Saturday. */
# define RC_EX_SAT_CHAR         'L'

/*! Exclude if date is no Saturday. */
# define RC_EX_NSAT_CHAR        'l'

/*! Exclude if date is a Sunday. */
# define RC_EX_SUN_CHAR         'P'

/*! Exclude if date is no Sunday. */
# define RC_EX_NSUN_CHAR        'p'

/*! Exclude if date is a Monday...Thursday. */
# define RC_EX_MON_2_THU_CHAR   'Q'

/*! Exclude if date is no Monday...Thursday. */
# define RC_EX_NMON_2_THU_CHAR  'q'

/*! Exclude if date is a Monday...Friday. */
# define RC_EX_MON_2_FRI_CHAR   'R'

/*! Exclude if date is no Monday...Friday. */
# define RC_EX_NMON_2_FRI_CHAR  'r'

/*
*  %?[STARTING_DATE][#[ENDING_DATE]]  ---  Excludes special dates.
*/
/*! %inclusive date special text. */
# define RC_IDATE_CHAR          'i'

/*! %exclusive date special text. */
# define RC_EDATE_CHAR          'e'

/*
*  %?[DATE]  --- Creates special texts.
*/
/*! %weekday name special text. */
# define RC_WDNAME_CHAR         WDNAME_CHAR

/*! %weekday number (Mon=1...Sun=7) special text. */
# define RC_WDNR_M1_2_S7_CHAR   'W'

/*! %weekday number (Mon=0...Sun=6) special text. */
# define RC_WDNR_M0_2_S6_CHAR   'E'

/*! %weekday number (Sun=1...Sat=7) special text. */
# define RC_WDNR_S1_2_S7_CHAR   'I'

/*! %weekday number (Sun=0...Sat=6) special text. */
# define RC_WDNR_S0_2_S6_CHAR   'J'

/*! %weekday number (Monday=1...Sunday=7) special text. */
# define RC_WDNR_X1_2_X7_CHAR   'S'

/*! %weekday number (Monday=0...Sunday=6) special text. */
# define RC_WDNR_X0_2_X6_CHAR   'T'

/*! %day of year number special text. */
# define RC_DOYNR_CHAR          'N'

/*! %day number special text. */
# define RC_DAYNR_CHAR          DAYNR_CHAR

/*! %month name special text. */
# define RC_MONTHNAME_CHAR      MONTHNAME_CHAR

/*! %month number special text. */
# define RC_MONTHNR_CHAR        MONTHNR_CHAR

/*! %year number special text. */
# define RC_YEARNR_CHAR         YEARNR_CHAR

/*! %week number special text. */
# define RC_WEEKNR_CHAR         'k'

/*! %birth age number special text. */
# define RC_BYEAR_CHAR          'B'

/*! %moon phase special text. */
# define RC_MOON_CHAR           'O'

/*! %moon phase text graphics image. */
# define RC_MOON_IMAGE_CHAR     'Z'

/*! %biorhythm special text. */
# define RC_BIO_CHAR            ','

/*! %biorhythm text graphics bar. */
# define RC_BIO_BAR_CHAR        ';'

/*
*  %?[OTHER_MODIFIER]  ---  Creates special texts.
*/
/*! %textual date special text. */
# define RC_TDATE_CHAR          'n'

/*! %julian days since 1.5-Jan-4713 BCE special text. */
# define RC_JDAYS_CHAR          'j'

/*! %time special text. */
# define RC_TIME_CHAR           't'

/*! %time total minutes special text. */
# define RC_TIME_TMI_CHAR       '\''

/*! %time hour special text. */
# define RC_TIME_HR_CHAR        '_'

/*! %time minute special text. */
# define RC_TIME_MI_CHAR        '?'

/*! %time `am|pm' suffix special text. */
# define RC_TIME_AMPM_CHAR      '{'

/*! %gmtime special text. */
# define RC_GMTIME_CHAR         '@'

/*! %gmtime total minutes special text. */
# define RC_GMTIME_TMI_CHAR     '`'

/*! %gmtime hour special text. */
# define RC_GMTIME_HR_CHAR      '.'

/*! %gmtime minute special text. */
# define RC_GMTIME_MI_CHAR      '/'

/*! %gmtime `am|pm' suffix special text. */
# define RC_GMTIME_AMPM_CHAR    '}'

/*! %timezone offset based on local time zone special text. */
# define RC_LT_ZONE_OFFS_CHAR   '='

/*! %timezone offset based on UTC/GMT zone special text. */
# define RC_UT_ZONE_OFFS_CHAR   '"'

/*! %distance between two geographical point locations special text */
# define RC_DISTANCE_CHAR       'b'

/*! %sunrise special text. */
# define RC_SUN_RISE_CHAR       'o'

/*! %sunset special text. */
# define RC_SUN_SET_CHAR        's'

/*! %day length special text. */
# define RC_SUN_DAY_CHAR        'u'

/*! %night length special text. */
# define RC_SUN_NIGHT_CHAR      'z'

/*! %moonrise special text. */
# define RC_MOON_RISE_CHAR      '('

/*! %moonset special text. */
# define RC_MOON_SET_CHAR       ')'

/*! %moons-day length special text. */
# define RC_MOON_DAY_CHAR       '['

/*! %moons-night length special text. */
# define RC_MOON_NIGHT_CHAR     ']'

/*
*  %?  ---  Creates difference values.
*/
/*! %day difference special text. */
# define RC_DAY_DIFF_CHAR       'd'

/*! %week difference special text. */
# define RC_WEEK_DIFF_CHAR      'w'

/*! %month difference special text. */
# define RC_MONTH_DIFF_CHAR     'm'

/*! %year | %year[DATE] difference special text. */
# define RC_YEAR_DIFF_CHAR      'y'

/*
*  %?  ---  Emits highlighting sequences/marking characters.
*/
/*! %highlighting sequence 1 enable special text. */
# define RC_HLS1S_CHAR          HLS1S_CHAR

/*! %highlighting sequence 1 disable special text. */
# define RC_HLS1E_CHAR          HLS1E_CHAR

/*! %highlighting sequence 2 enable special text. */
# define RC_HLS2S_CHAR          '3'

/*! %highlighting sequence 2 disable special text. */
# define RC_HLS2E_CHAR          '4'

/*! %highlighting sequence 1 enable special text if fixed date is on today's date. */
# define RC_HLS3S_CHAR          '5'

/*! %highlighting sequence 1 disable special text if fixed date is on today's date. */
# define RC_HLS3E_CHAR          '6'

/*! %highlighting sequence 2 enable special text if fixed date is on a legal holiday date. */
# define RC_HLS4S_CHAR          '7'

/*! %highlighting sequence 2 disable special text if fixed date is on a legal holiday date. */
# define RC_HLS4E_CHAR          '8'

/*! %highlighting sequence 1|2 enable special text if fixed date is on today's or legal holiday date. */
# define RC_HLS5S_CHAR          '9'

/*! %highlighting sequence 1|2 disable special text if fixed date is on today's or legal holiday date. */
# define RC_HLS5E_CHAR          '0'



/*
*  Common preprocessor macros for fixed dates/resource files.
*/
# ifdef IDX
#  undef  IDX
# endif
# define IDX(gc_c)      (tolower((gc_c)) - 'a')
# ifdef CHR2DIG
#  undef  CHR2DIG
# endif
# define CHR2DIG(gc_c)  ((gc_c) - '0')
# ifdef DIG2CHR
#  undef  DIG2CHR
# endif
# define DIG2CHR(gc_x)  ((gc_x) + '0')



# ifdef CUT_LONGNAMES
/*
*  Cut resp., replace some long variable or function names to 6 significant
*    places so ANY linker should be able to bind an executable file.
*/
#  define bio_emo_ptext                       V_bet
#  define bio_emo_phase                       V_bep
#  define bio_emo_waxes                       V_bew
#  define bio_int_ptext                       V_bit
#  define bio_int_phase                       V_bip
#  define bio_int_waxes                       V_biw
#  define bio_phy_ptext                       V_bpt
#  define bio_phy_phase                       V_bpp
#  define bio_phy_waxes                       V_bpw
#  define bio_positive_day                    V_bpd
#  define exclusive_date_map                  V_edm
#  define exclusive_weekday_map               V_ewm
#  define fiscal_month                        V_fm
#  define fiscal_year                         V_fy
#  define got_lat_min                         V_gmi
#  define got_lat_max                         V_gma
#  define inclusive_date_map                  V_idm
#  define inclusive_weekday_map               V_iwm
#  define insert_line_into_table              F_ilt
#  define insert_response_file                F_irf
#  define num_of_range_chars                  V_nrc
#  define num_of_repeat_chars                 V_noc
#  define num_of_appears_chars                V_nfc
#  define precomp_date                        F_pd
#  define precomp_nth_wd                      F_pwd
#  define print_line                          V_pl
#  define ptr_info_exclusive_day              V_ped
#  define print_twice                         V_ptw
#  define time_sep                            V_ts
#  define time_sep_found                      V_tsf
#  define rc_bio_axis_len                     V_rba
#  define rc_bio_emo_lit                      V_rbe
#  define rc_bio_int_lit                      V_rbi
#  define rc_bio_phy_lit                      V_rbp
#  define rc_elems_table                      V_ret
#  define rc_elems_max                        V_rem
#  define rc_elems                            V_rei
#  define rc_enable_fn_flag                   V_rff
#  define rc_enable_hda_flag                  V_rha
#  define rc_enable_hdl_flag                  V_rhl
#  define rc_export_ldvar_flag                V_red
#  define rc_export_ltvar_flag                V_ret
#  define rc_feb_29_to_feb_28                 V_f2f
#  define rc_feb_29_to_mar_01                 V_f2m
#  define rc_files_table                      V_rft
#  define rc_files_max                        V_rfm
#  define rc_files                            V_rfi
#  define rc_filename                         V_rfn
#  define rc_filter_day                       V_rfd
#  define rc_filter_period                    V_rfp
#  define rc_filter_text                      V_rfx
#  define rc_here_fn                          V_rhn
#  define rc_here_fp                          V_rhp
#  define rc_period                           V_rp
#  define rc_period_argv                      V_rpa
#  define rc_period_flag                      V_rpf
#  define rc_period_is_longopt                V_rpi
#  define rc_period_list                      V_rpl
#  define rc_period_option                    V_rpo
#  define rc_suppr_date_part_flag             V_rsd
#  define rc_suppr_list_sep_flag              V_rsl
#  define rc_suppr_text_part_flag             V_rst
#  define rc_tvar                             V_rt
#  define rc_tvar_tfn                         V_rtf
#  define rc_tvar_tfp                         V_rtp
#  define rc_use                              F_ru
#  define rc_use_flag                         V_ruf
#  define rc_valid_day                        F_rvd
#  define rc_valid_period                     F_rvp
#  define rc_week_flag                        V_rwf
#  define rc_weekno_flag                      V_rwn
#  define rc_week_year_flag                   V_rwy
#  define weekday_of_date                     F_wod
#  define weekday_map                         V_wm
#  define weekday_name_group                  V_wng
# endif	/* CUT_LONGNAMES */



/*
*  Type definitions.
*/
/*
*  The date variable records.
*/
typedef struct dvar_base_type
{
  char dvar_month;
  char dvar_day;
}
Dvar_base_struct;

typedef struct dvar_type
{
  /* Global date variables. */
  Dvar_base_struct dvar_global;

  /* Local date variables. */
  Dvar_base_struct dvar_local;
}
Dvar_struct;

/*
*  The text variable records.
*/
typedef struct tvar_base_type
{
  char *tvar_text;
}
Tvar_base_struct;

typedef struct tvar_type
{
  /* Global text variables. */
  Tvar_base_struct tvar_global;

  /* Local text variables. */
  Tvar_base_struct tvar_local;
}
Tvar_struct;

/*
*  The line part record.
*/
typedef struct line_type
{
  /* List of days given? */
  Bool day_list;

  /* Range of days given? */
  Bool day_range;

  /* Points to the list/range of days of a line, otherwise NULL. */
  char *day_part;

  /* Points to the "repeat for N days since..." field of a line, otherwise NULL. */
  char *repeat_part;

  /* Points to the "appears each N'th day since..." field of a line, otherwise NULL. */
  char *appears_part;

  /* Points to the text part of a line (always). */
  char *text_part;
}
Line_struct;

/*
*  The file type record.
*/
typedef struct file_type
{
  /* Actual file pointer. */
  FILE *fp;

  /* Name of actual resource/include file. */
  char *filename;

  /* Actual line number of actual resource/include file. */
  long line_number;

  /* Number of bytes still unhandled in self-managed buffer. */
  int in_pool;

  /* Memory region of self-managed buffer. */
  char *pool;

  /* Pointer to next unmanaged character in self-managed buffer. */
  char *ptr_pool;

  /* Buffered local date variables of a "previous" file. */
  Dvar_base_struct local_dvars[RC_DVAR_MAX];

  /* Buffered local text variables of a "previous" file. */
  Tvar_base_struct local_tvars[RC_TVAR_MAX];
}
File_struct;

/*
*  The local coordinates record used by the %distance, %sun* and %moon* special texts.
*/
typedef struct coor_type
{
  int lon_deg;
  int lon_min;
  int lon_sec;
  int lat_deg;
  int lat_min;
  int lat_sec;
  int the_mode;
  int meters_above_sea_level;
  int time_zone_in_mins;
}
Coor_struct;

/*
*  The output format of an astronomical object record.
*/
typedef struct astronomical_object_output_format_type
{
  char data_format;
  char error_format;
}
Aobj_oformat_struct;

/*
*  General purpose enumerations.
*/
typedef enum var_mode
{
  GLobal = 1,
  LOcal
}
Var_enum;

typedef enum astronomical_event
{
  RIse = 1,
  SEt
}
Aevent_enum;



/*
*  SUB-include one of the regex header file  ;<
*/
/*
*  The REGEX stuff.
*/
# if HAVE_GNU_RE_COMPILE_PATTERN
#  if HAVE_SYS_TYPES_H
#   include <sys/types.h>
#  endif
#  if HAVE_REGEX_H
#   include <regex.h>
#  endif
# endif
# if HAVE_POSIX_REGCOMP
#  if HAVE_SYS_TYPES_H
#   include <sys/types.h>
#  endif
#  if HAVE_REGEX_H
#   include <regex.h>
#  endif
# endif


/*
*  GLOBAL variables declarations.
*/
/*! Points to the local coordinates 1. */
extern Coor_struct *coor1;

/*! Points to the local coordinates 2. */
extern Coor_struct *coor2;

/*! Date variables a[=MMDD]...z[] (YYYY@{a|b|...|z}[[-]N]). */
extern Dvar_struct rc_dvar[];

/*! Temporary file used when a command is assigned to a TVAR. */
extern FILE *rc_tvar_tfp;

/*! Temporary file used for managing `--here=ARG' options. */
extern FILE *rc_here_fp;

/*! Pointers to different parts of a (resource file) line. */
extern Line_struct *lineptrs;

/*! Pointers to different parts of a (resource file) line. */
extern Line_struct *lptrs2;

/*! Pointers to different parts of a (resource file) line. */
extern Line_struct *lptrs3;

/*! Pointers to different parts of a (resource file) line. */
extern Line_struct *lptrs;

/*! Text variables $a[=TEXT]...$z[]. */
extern Tvar_struct rc_tvar[];

/*! Time displacement value in days used by %sun* and %moon*
   (also used as cycle-starting time value for %sun* and %moon* special texts). */
extern double time_offset;

/*! Atmospheric pressure in millibar (`--atmosphere=PRESSURE,TEMPERATURE'). */
extern double atm_pressure;

/*! Atmospheric temperature in degrees Celsius (`--atmosphere=PRESSURE,TEMPERATURE'). */
extern double atm_temperature;

/*! Adjust rise/set-based reference altitude resp. shadow length factor (`--adjust-value=NUMBER'). */
extern double adjust_value;

/*! Actual size of `rc_elems_table[]'. */
extern Uint rc_elems_max;

/*! Cycle-ending time value in minutes for %sun* and %moon* special texts. */
extern int loop_end;

/*! Cycle-timestep value in minutes for %sun* and %moon* special texts. */
extern int loop_step;

/*! Buffer of actual GMT day. */
extern int buf_gd;

/*! Buffer of actual GMT month. */
extern int buf_gm;

/*! Buffer of actual GMT year. */
extern int buf_gy;

/*! Day of event found in line. */
extern int d;

/*! Buffered day of event. */
extern int d_buf;

/*! Indicates whether event also appears in previous year. */
extern int decr_year;

/*! Actual GMT hour. */
extern int gmt_hour;

/*! Day difference of GMT and local date. */
extern int gmt_loc_diff;

/*! Actual GMT minute. */
extern int gmt_min;

/*! The `N'th weekday of month' displacement value. */
extern int hn;

/*! The weekday number of `N'th weekday of month'*/
extern int hwd;

/*! Indicates whether event also appears in next year. */
extern int incr_year;

/*! Filler length of week number text. */
extern int len_fil_wt;

/*! Actual size of text buffer of "text"-part of a line. */
extern int len_the_text;

/*! Month of event found in line. */
extern int m;

/*! Buffered month of event. */
extern int m_buf;

/*! Length of a single axis of a biorhythm text graphics bar. */
extern int rc_bio_axis_len;

/*! Amount of resource file entries. */
extern int rc_elems;

/*! `-cd'. */
extern int rc_have_today_in_list;

/*! Number of lines of a Moon phase text graphics image. */
extern int rc_moonimage_lines;

/*! Amount of period of fixed dates. */
extern int rc_period;

/*! Starting index of dummy resource file entries in table. */
extern int rc_zero_pos;

/*! Year of event found in line. */
extern int y;

/*! The mode specifying character. */
extern char hc;

/*! Vector of holiday dates (memorial days). */
extern char hd_mdays[];

/*! Stores the eternal holiday list texts. */
extern char *hd_table[];

/*! Text buffer of a line read from a resource file. */
extern char *line_buffer;

/*! Text containing modified actual date %DATE. */
extern char *rc_adate;

/*! The biorhythm's "Emo" text. */
extern char *rc_bio_emo_lit;

/*! The biorhythm's "Int" text. */
extern char *rc_bio_int_lit;

/*! The biorhythm's "Phy" text. */
extern char *rc_bio_phy_lit;

/*! Name of alternative resource file(s) `-f|F<NAME[+...]>'. */
extern char *rc_filename;

/*! Argument used for filtering fixed date days. */
extern char *rc_filter_day;

/*! Argument used for filtering fixed date periods. */
extern char *rc_filter_period;

/*! REGEX used for filtering fixed date. */
extern char *rc_filter_text;

/*! Fixed date list grouping separator `-cg[TEXT]'. */
extern char *rc_grp_sep;

/*! Fixed date list heading text `--heading-text=TEXT'. */
extern char *rc_heading_text;

/*! Name of tempfile used when a command is assigned to a TVAR. */
extern char *rc_tvar_tfn;

/*! Name of tempfile used for managing `--here=ARG' options. */
extern char *rc_here_fn;

/*! General purpose text buffer 5. */
extern char *s5;

/*! General purpose text buffer 6. */
extern char *s6;

/*! General purpose text buffer 7. */
extern char *s7;

/*! Text buffer of "text"-part of a line. */
extern char *the_text;

/*! Stores the valid fixed date texts. */
extern char **rc_elems_table;

/*! `-cNw' and actual date modified. */
extern Bool adate_set;

/*! Does the command enables a year implicitly? */
extern Bool date_enables_year;

/*! `-cNw' and complete week is in month. */
extern Bool is_1month_mode;

/*! `-cNw' and only part of week is in month. */
extern Bool is_2month_mode;

/*! Reference to a date variable found in line. */
extern Bool is_2dvar;

/*! Reference to Easter Sundays date found in line. */
extern Bool is_2easter;

/*! Is a command (explicit date) given in the command line? */
extern Bool is_date_given;

/*! `-C[]' or `-C[][T|W|M|Y]' or `-c[][T|W|M|Y]' or `-F<>'. */
extern Bool rc_all_dates_flag;

/*! `-cA'. */
extern Bool rc_alternative_format_flag;

/*! `-c<N|w|m|y>'-. */
extern Bool rc_backwards_flag;

/*! `-jcb'. */
extern Bool rc_both_dates_flag;

/*! Buffers the state of `rc_backwards_flag'. */
extern Bool rc_bwdf_buffer;

/*! `-cz'. */
extern Bool rc_count_flag;

/*! `-ca'. */
extern Bool rc_enable_fn_flag;

/*! `-ce'. */
extern Bool rc_enable_hda_flag;

/*! `-cE'. */
extern Bool rc_enable_hdl_flag;

/*! `--execute-command'. */
extern Bool rc_execute_command;

/*! `--export-date-variables'. */
extern Bool rc_export_ldvar_flag;

/*! `--export-text-variables'. */
extern Bool rc_export_ltvar_flag;

/*! `--leap-day=february'. */
extern Bool rc_feb_29_to_feb_28;

/*! `--leap-day=march'. */
extern Bool rc_feb_29_to_mar_01;

/*! `--precise' to display precise, non-rounded, times and data. */
extern Bool rc_precise;

/*! `-c<N|w|m|y>+'. */
extern Bool rc_forwards_flag;

/*! Buffers the state of `rc_forwards_flag'. */
extern Bool rc_fwdf_buffer;

/*! `--ignore-case' to ignore case distinctions in PATTERN. */
extern Bool rc_ignore_case_flag;

/*! `--limit' to calculate rise/set time limited to the current day only. */
extern Bool rc_limit;

/*! `-cm'. */
extern Bool rc_month_flag;

/*! `-co'. */
extern Bool rc_omit_date_flag;

/*! `-cN[d|w|+|-]|MMDD|MMWW[W]N'. */
extern Bool rc_period_flag;

/*! `-cl'. */
extern Bool rc_period_list;

/*! `--revert-match' to select non-matching PATTERN lines. */
extern Bool rc_revert_match_flag;

/*! `-c-'. */
extern Bool rc_sort_des_flag;

/*! `-jc'. */
extern Bool rc_special_flag;

/*! `-cU'. */
extern Bool rc_suppr_date_part_flag;

/*! `-cQ'. */
extern Bool rc_suppr_list_sep_flag;

/*! `-cJ'. */
extern Bool rc_suppr_text_part_flag;

/*! `-cx'. */
extern Bool rc_title_flag;

/*! `-ct'. */
extern Bool rc_tomorrow_flag;

/*! `-c'. */
extern Bool rc_use_flag;

/*! `-cw'. */
extern Bool rc_week_flag;

/*! `-ck'. */
extern Bool rc_week_number_flag;

/*! `-cNw'. */
extern Bool rc_week_year_flag;

/*! `-cy'. */
extern Bool rc_year_flag;

/*! `-cZ'. */
extern Bool rc_zero_dates_flag;

/*! Remove highlighting sequences before searching PATTERN? */
extern Bool remove_hls_in_regex;

/*! Stores whether a %shell escape special text is run. */
extern Bool shell_escape_done;

# if HAVE_GNU_RE_COMPILE_PATTERN
extern struct re_pattern_buffer regpattern;
# endif

# if HAVE_POSIX_REGCOMP
extern regex_t regpattern;
# endif

# if HAVE_RE_COMP
extern int re_pattern;
# endif

#endif /* __RC_DEFS_H */
