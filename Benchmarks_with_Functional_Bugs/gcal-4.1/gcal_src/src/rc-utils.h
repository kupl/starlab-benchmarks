#ifndef __RC_UTILS_H
# define __RC_UTILS_H
/*!
*  \file rc-utils.h
*  \brief Pool of special functions necessary for managing
*  			the fixed dates header file.
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
*  $Id: rc-utils.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-utils.c'.
*/
extern Bool
  rc_valid_day __P_ ((const char *date_text,
		      const int day, const int month, const int year));
extern Bool
  rc_valid_period __P_ ((char *date_text,
			 const int d,
			 const int m,
			 const int y,
			 const int incr_year, const int decr_year));
extern void rc_clean_flags __P_ ((void));
extern Line_struct *rc_get_date __P_ ((char *the_line,
				       Line_struct * lineptrs,
				       const Bool is_rc_file,
				       Bool * is_weekday_mode,
				       int *d,
				       int *m,
				       int *y,
				       int *n,
				       int *len,
				       char *hc,
				       int *hn,
				       int *hwd,
				       const char *filename,
				       const long line_number,
				       const char *line_buffer,
				       const Bool on_error_exit));
extern Bool
  precomp_nth_wd __P_ ((int diff,
			const int wd,
			int *n,
			int *day,
			int *month, int *year, const Cmode_enum mode));
extern Bool
  precomp_date __P_ ((int diff,
		      const int wd,
		      int *day,
		      int *month, const int year, const Cmode_enum mode));
extern void
  set_dvar __P_ ((const char *line_buffer,
		  Line_struct * lineptrs,
		  const char *filename,
		  const long line_number, const Var_enum mode));
extern void
  set_tvar __P_ ((const char *line_buffer,
		  const char *filename,
		  const long line_number, const Var_enum mode));
extern void
  nth_weekday_of_month __P_ ((int *d,
			      int *m,
			      int *y, const int *n, Bool * is_weekday_mode));
extern Slint
  d_between __P_ ((const int d1,
		   const int m1,
		   const int y1, const int d2, const int m2, const int y2));
extern Slint
  w_between __P_ ((const int d1,
		   const int m1,
		   const int y1, const int d2, const int m2, const int y2));
extern Slint
  m_between __P_ ((const int m1, const int y1, const int m2, const int y2));
extern void
  manage_leap_day __P_ ((int *day,
			 int *month,
			 int year,
			 const char *line_buffer,
			 const char *filename, const long line_number));
extern char *biorhythm __P_ ((const Bool create_bar,
			      int axis_len,
			      char *string,
			      const int day,
			      const int month,
			      const int year,
			      const int birth_day,
			      const int birth_month,
			      const int birth_year,
			      const char *emo_text,
			      int *emo_phase,
			      int *emo_waxes,
			      const char *int_text,
			      int *int_phase,
			      int *int_waxes,
			      const char *phy_text,
			      int *phy_phase,
			      int *phy_waxes,
			      int *critical_day,
			      int *positive_day, int *negative_day));
extern double
  compute_distance __P_ ((const Coor_struct * coor1,
			  const Coor_struct * coor2));
__END_DECLARATIONS
#endif /* __RC_UTILS_H */
