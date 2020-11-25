#ifndef __GLOBALS_H
# define __GLOBALS_H
/*!
*  \file globals.h
*  \brief Declarations of all GLOBAL common variables used
*              (except all common variables that depend to the RC section).
*/
/*
*  Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken
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
*  $Id: globals.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL variables declarations.
*/
/*! The table of supported date formats. */
extern Df_struct supported_date_format[];

/*! Points to the used date format. */
extern Df_struct *date_format;

/*! The table of long-style options. */
extern const Lopt_struct lopt[];

/*! The table of Gregorian Reformation dates. */
extern Greg_struct greg_reform_date[];

/*! Points to the used Gregorian Reformation date. */
extern Greg_struct *greg;

/*! Effective hls 1 end (current day). */
extern Hls_struct ehls1e;

/*! Effective hls 1 start (current day). */
extern Hls_struct ehls1s;

/*! Effective hls 2 end (holiday). */
extern Hls_struct ehls2e;

/*! Effective hls 2 start (holiday). */
extern Hls_struct ehls2s;

/*! Used if a list/range of months/years is given. */
extern Ml_struct *month_list;

/*! Actual size of all string vectors. */
extern Uint maxlen_max;

/*! Amount of days in months. */
extern const int dvec[];

/*! Number of past days of month. */
extern const int mvec[];

/*! Actual day. */
extern int act_day;

/*! Actual hour. */
extern int act_hour;

/*! Actual minute. */
extern int act_min;

/*! Actual month. */
extern int act_month;

/*! Actual second. */
extern int act_sec;

/*! Actual year. */
extern int act_year;

/*! Buffer of actual day. */
extern int buf_ad;

/*! Buffer of actual month. */
extern int buf_am;

/*! Buffer of actual year. */
extern int buf_ay;

/*! True actual day as reported by the operating system. */
extern int true_day;

/*! True actual month as reported by the operating system. */
extern int true_month;

/*! True actual year as reported by the operating system. */
extern int true_year;

/*! Current day. */
extern int day;

/*! Starting month of a fiscal year. */
extern int fiscal_month;

/*! Format length of a standard/special/both day. */
extern int format_len;

/*! Is current year a leap year? */
extern int is_leap_year;

/*! Is output directed to channel 1? */
extern int is_tty1;

/*! Is output directed to channel 2? */
extern int is_tty2;

/*! Is output displayed on a terminal? */
extern int is_tty;

/*! Maximum string length of a textual day_name(). */
extern int len_dayname_max;

/*! Maximum string length of a textual month_name(). */
extern int len_monthname_max;

/*! Maximum string length of a textual day_suffix() [if any]. */
extern int len_suffix_max;

/*! String length of the maximum year able to compute. */
extern int len_year_max;

/*! Current month. */
extern int month;

/*! Number of month columns of a year calendar. */
extern int out_cols;

/*! Number of month rows of a year calendar. */
extern int out_rows;

/*! `-s<0,1...7|day name>'. */
extern int start_day;

/*! `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
extern int time_hour_offset;

/*! `--time-offset=t|@|[t|@][+|-]MMMM|HH:[MM]' for correcting astronomically based data. */
extern int time_min_offset;

/*! `--transform-year=BASE_YEAR'. */
extern int transform_year;

/*! `--debug[=0...WARN_LVL_MAX]'. */
extern int warning_level;

/*! Current year. */
extern int year;

/*! Vector of holiday dates (legal days). */
extern char hd_ldays[];

/*! `--cc-holidays=CC[+CC+...]'. */
extern char *cc;

/*! The "DD" text. */
extern char *dd_lit;

/*! The "ARG" text. */
extern char *larg_lit;

/*! The "MM" text. */
extern char *mm_lit;

/*! Stores the actual program name. */
extern char *prgr_name;

/*! General purpose text buffer 1. */
extern char *s1;

/*! General purpose text buffer 2. */
extern char *s2;

/*! General purpose text buffer 3. */
extern char *s3;

/*! General purpose text buffer 4. */
extern char *s4;

/*! Character for separating HH:MM time values. */
extern char *time_sep;

/*! `--translate-string=CHARACTER_PAIR...'. */
extern char *translate_string;

/*! Pointer to the $TZ (timezone) environment variable. */
extern char *tz;

/*! The "WWW" text. */
extern char *www_lit;

/*! The "YY" text. */
extern char *yy_lit;

/*! The "YYYY" text. */
extern char *yyyy_lit;

/*! `-jb'. */
extern Bool cal_both_dates_flag;

/*! `-j'. */
extern Bool cal_special_flag;

/*! `-K'. */
extern Bool cal_with_week_number;

/*! Use Astronomical holidays. */
extern Bool hdy_astronomical;

/*! Use Bahai calendar holidays. */
extern Bool hdy_bahai;

/*! Use Celtic calendar holidays. */
extern Bool hdy_celtic;

/*! Use Chinese calendar holidays. */
extern Bool hdy_chinese;

/*! Use Chinese flexible calendar holidays. */
extern Bool hdy_chinese_flexible;

/*! Use Christian Western churches calendar holidays. */
extern Bool hdy_christian;

/*! Use Hebrew calendar holidays. */
extern Bool hdy_hebrew;

/*! Use Islamic CIVIL calendar holidays. */
extern Bool hdy_islamic;

/*! Use Japanese calendar holidays. */
extern Bool hdy_japanese;

/*! Use Japanese flexible calendar holidays. */
extern Bool hdy_japanese_flexible;

/*! Use Multicultural New_Year's_Day holidays. */
extern Bool hdy_multicultural_new_year;

/*! Use Orthodox Christian Eastern churches NEW calendar holidays. */
extern Bool hdy_orthodox_new;

/*! Use Orthodox Christian Eastern churches OLD calendar holidays. */
extern Bool hdy_orthodox_old;

/*! Use Persian Jalaali calendar holidays. */
extern Bool hdy_persian;

/*! Use Zodiacal Marker holidays. */
extern Bool hdy_zodiacal_marker;

/*! Use Bahai calendar months. */
extern Bool mth_bahai;

/*! Use Chinese calendar months. */
extern Bool mth_chinese;

/*! Use Chinese flexible calendar months. */
extern Bool mth_chinese_flexible;

/*! Use Coptic calendar months. */
extern Bool mth_coptic;

/*! Use Ethiopic calendar months. */
extern Bool mth_ethiopic;

/*! Use French Revolutionary calendar months. */
extern Bool mth_french_revolutionary;

/*! Use Hebrew calendar months. */
extern Bool mth_hebrew;

/*! Use Indian CIVIL calendar months. */
extern Bool mth_indian_civil;

/*! Use Islamic CIVIL calendar months. */
extern Bool mth_islamic;

/*! Use Japanese calendar months. */
extern Bool mth_japanese;

/*! Use Japanese flexible calendar months. */
extern Bool mth_japanese_flexible;

/*! Use Old-Armenic calendar months. */
extern Bool mth_old_armenic;

/*! Use Old-Egyptic calendar months. */
extern Bool mth_old_egyptic;

/*! Use Persian Jalaali calendar months. */
extern Bool mth_persian;

/*! Must we emulate the highlighting sequences? */
extern Bool emu_hls;

/*! `-jnb'. */
extern Bool hd_both_dates_flag;

/*! `-N'. */
extern Bool hd_legal_days_only;

/*! `-n|N-'. */
extern Bool hd_sort_des_flag;

/*! `-jn'. */
extern Bool hd_special_flag;

/*! `-G'. */
extern Bool hd_suppr_list_sep_flag;

/*! `-X'. */
extern Bool hd_title_flag;

/*! `-H<yes>|<no>'. */
extern Bool highlight_flag;

/*! `-n|N'. */
extern Bool holiday_flag;

/*! Argument is `..' -> current quarter of actual year. */
extern Bool is_3month_mode2;

/*! Argument is `.' or `.+' or `.-'. */
extern Bool is_3month_mode;

/*! Is an extended list of months/years given? */
extern Bool is_ext_list;

/*! Is an extended range of months/years given? */
extern Bool is_ext_range;

/*! Is an extended list/range of years given? */
extern Bool is_ext_year;

/*! `:' char found in argument (MM:YYYY). */
extern Bool is_fiscal_year;

/*! Is a special range of a selected month of years given? */
extern Bool is_special_range;

/*! Is a special range of selected months of years given? */
extern Bool is_multi_range;

/*! `--iso-week-number=<yes>|<no>'. */
extern Bool iso_week_number;

/*! `-O' (compute leap years as done by Eastern churches). */
extern Bool orthodox_calendar;

/*! `-i'. */
extern Bool special_calsheet_flag;

/*! `-u'. */
extern Bool suppr_cal_flag;

/*! Day number leaded with zeroes format specifier given in date format? */
extern Bool use_day_zeroleaded;

/*! 3 char day name format specifier given in date format? */
extern Bool use_short3_day_name;

/*! Year number leaded with zeroes format specifier given in date format? */
extern Bool use_year_zeroleaded;



# ifdef DJG
/*! Set to SHRT_MAX for checking the maximum table range. */
extern Usint testval;
# else /* !DJG */
/* Set to INT_MAX for checking the maximum table range. */
extern Uint testval;
# endif	/* !DJG */



# ifdef GCAL_EMAIL
/*! Temporary file which is send by the mailer. */
extern FILE *tfp;

/*! Name of tempfile used by the mailer. */
extern char *tfn;

/*! Email address Gcal's output is send to. */
extern char *email_adr;
# endif	/* GCAL_EMAIL */



# ifdef GCAL_EPAGER
/*! Name of external pager program. */
extern char *ext_pager;
# endif	/* GCAL_EPAGER */



# ifdef GCAL_NLS
/*! Support of English language? */
extern Bool is_en;
# endif	/* GCAL_NLS */



# if USE_PAGER
/*! Number of tty columns. */
extern int tty_cols;

/*! Number of tty rows. */
extern int tty_rows;

/*! `-p'. */
extern Bool pager_flag;
# endif	/* USE_PAGER */



# if (USE_PAGER || USE_HLS) && defined(GCAL_TCAP)
#  if MUST_DCL_OSPEED && USE_HLS
/*! Terminal output baud rate. */
extern short ospeed;

/*!
   On Solaris2, sys/types.h #includes sys/reg.h, which #defines PC.
   Unfortunately, PC is a global variable used by the Termcap library.
*/
#   ifdef PC
#    undef PC
#   endif

/*! Padding character. */
extern char PC;
#  endif /* MUST_DCL_OSPEED && USE_HLS */
# endif	/* (USE_PAGER || USE_HLS) && GCAL_TCAP */
#endif /* __GLOBALS_H */
