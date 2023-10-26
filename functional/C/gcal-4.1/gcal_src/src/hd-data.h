#ifndef __HD_DATA_H
# define __HD_DATA_H
/*!
*  \file hd-data.h
*  \brief All data which is composed into the eternal holiday list header file.
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
*  $Id: hd-data.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd_data.c'.
*/
extern const Cc_struct *binsearch_cc_id __P_ ((char *id));
extern void
  base_christian_hdy __P_ ((Bool * init_data,
			    const Bool detected,
			    int easter,
			    const int year,
			    int *hd_elems, const int fday, const int count,
			    const char *cc));
extern void
  astronomical_hdy __P_ ((Bool * init_data,
			  const Bool detected,
			  const int year,
			  int *hd_elems, const int fday, const int count));
extern void
  bahai_hdy __P_ ((Bool * init_data,
		   const Bool detected,
		   const int year,
		   int *hd_elems,
		   const int fday, const int count, const char *cc));
extern void
  bahai_mth __P_ ((Bool * init_data,
		   const Bool detected,
		   const int year,
		   int *hd_elems, const int fday, const int count));
extern void
  celtic_hdy __P_ ((Bool * init_data,
		    const Bool detected,
		    const int year,
		    int *hd_elems,
		    const int fday, const int count, const char *cc));
extern void
  chinese_hdy __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems,
		     const int fday,
		     const int count,
		     const char *cc, const Bool is_any_meridian));
extern void
  chinese_mth __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems,
		     const int fday,
		     const int count,
		     const Bool is_any_meridian, const Bool do_chinese));
extern void
  christian_hdy __P_ ((Bool * init_data,
		       const Bool detected,
		       int easter,
		       const int year,
		       int *hd_elems, const int fday, const int count));
extern void
  coptic_mth __P_ ((Bool * init_data,
		    const Bool detected,
		    const int year,
		    int *hd_elems,
		    const int fday, const int count, const Bool do_coptic));
extern void
  french_revolutionary_mth __P_ ((Bool * init_data,
				  const Bool detected,
				  const int year,
				  int *hd_elems,
				  const int fday, const int count));
extern void
  indian_civil_mth __P_ ((Bool * init_data,
			  const Bool detected,
			  const int year,
			  int *hd_elems, const int fday, const int count));
extern void
  hebrew_hdy __P_ ((Bool * init_data,
		    const Bool detected,
		    const int year,
		    int *hd_elems,
		    const int fday, const int count, const char *cc));
extern void
  hebrew_mth __P_ ((Bool * init_data,
		    const Bool detected,
		    const int year,
		    int *hd_elems, const int fday, const int count));
extern void
  islamic_hdy __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems,
		     const int fday, const int count, const char *cc));
extern void
  islamic_mth __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems, const int fday, const int count));
extern void
  japanese_hdy __P_ ((Bool * init_data,
		      const Bool detected,
		      const int year,
		      int *hd_elems,
		      const int fday,
		      const int count,
		      const char *cc, const Bool is_any_meridian));
extern void
  multicultural_new_year_hdy __P_ ((Bool * init_data,
				    const Bool detected,
				    const int year,
				    int *hd_elems,
				    const int fday, const int count));
extern void
  old_egyptic_mth __P_ ((Bool * init_data,
			 const Bool detected,
			 const int year,
			 int *hd_elems,
			 const int fday,
			 const int count, const Bool do_old_egyptic));
extern void
  orthodox_hdy __P_ ((Bool * init_data,
		      const Bool detected,
		      int easter,
		      const int year,
		      int *hd_elems,
		      const int fday,
		      const int count,
		      const char *cc, const Bool is_new_calendar));
extern void
  persian_hdy __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems,
		     const int fday, const int count, const char *cc));
extern void
  persian_mth __P_ ((Bool * init_data,
		     const Bool detected,
		     const int year,
		     int *hd_elems, const int fday, const int count));
extern void
  zodiacal_marker_hdy __P_ ((Bool * init_data,
			     const Bool detected,
			     const int year,
			     int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
#endif /* __HD_DATA_H */
