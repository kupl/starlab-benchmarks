#ifndef __HD_USE_H
# define __HD_USE_H
/*!
*  \file hd-use.h
*  \brief Manage the eternal holiday list header file.
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
*  $Id: hd-use.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd_use.c'.
*/
extern int
  eval_holiday __P_ ((int day,
		      const int month,
		      const int year, const int wd, const Bool forwards));
extern int
  orthodox_easter __P_ ((const int year,
			 int *greg_diff,
			 const int greg_year,
			 const int greg_month,
			 const int greg_first_day, const int greg_last_day));
extern int tishri_1 __P_ ((const int year));
extern int muharram_1 __P_ ((const int year, int *doy));
extern int
  find_chinese_leap_month __P_ ((Ulint * conjunction_vector,
				 Ulint * nh_ws_prev_year,
				 const int year,
				 const int hour, const int min));
extern int
  decode_date_format __P_ ((char *format_txt,
			    char **result_txt,
			    int day,
			    int month,
			    const int year,
			    const int doy,
			    const Bool hls1_set,
			    const Bool hls2_set,
			    const Bool fixed_length_names));
extern int
  decode_format __P_ ((char *format_txt,
		       int pos,
		       Bool * is_cformat,
		       Bool * is_lformat,
		       Bool * is_sign,
		       Bool * is_lzero,
		       Bool * is_suffix,
		       Bool * is_fformat, int *fstyle, int *fwidth));
extern int
  use_format __P_ ((char **result_txt,
		    const int pos,
		    const char *string,
		    const long value,
		    const Bool is_numeric,
		    const Bool is_cformat,
		    const Bool is_lformat,
		    const Bool is_sign,
		    const Bool is_lzero,
		    const Bool is_suffix,
		    const Bool is_fformat,
		    const int fstyle, const int fwidth, const int extra_len));
extern void print_all_holidays __P_ ((Bool init_data, const Bool detected));
extern void
  holiday __P_ ((Bool init_data,
		 const Bool detected,
		 const char *holiday_name,
		 const char *country_code,
		 const char *holiday_prefix,
		 int day,
		 int month,
		 const int year,
		 int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
#endif /* __HD_USE_H */
