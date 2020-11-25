#ifndef __HD_ASTRO_H
# define __HD_ASTRO_H
/*!
*  \file hd-astro.h
*  \brief Astronomical and support functions for the
*  				eternal holiday list header file.
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
*  $Id: hd-astro.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  Equinox and solstice MODE constants for northern and southern Hemisphere.
*/
# define NH_VE  0.0
# define SH_AE  NH_VE
# define NH_SS  90.0
# define SH_WS  NH_SS
# define NH_AE  180.0
# define SH_VE  NH_AE
# define NH_WS  270.0
# define SH_SS  NH_WS



/*
*  Solar and lunar eclipse type constants.
*/
# define ECLIPSE_PENUMBRAL  100000000.0
# define ECLIPSE_PARTIAL    200000000.0
# define ECLIPSE_ANNULAR    300000000.0
# define ECLIPSE_TOTAL      400000000.0



/*
*  Moon phase constants.
*/
# define MPHASE_NEW  0.00
# define MPHASE_FQT  0.25
# define MPHASE_FUL  0.50
# define MPHASE_LQT  0.75



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `hd_astro.c'.
*/
extern double
  equinox_solstice __P_ ((const double longitude,
			  int *day,
			  int *month, int *year, int hour, int min));
extern double
  delta_t __P_ ((const int day,
		 const int month,
		 const int year, const int hour, const int min));
extern double
  moonphase __P_ ((const double phase_selector,
		   const Bool find_eclipse,
		   double *eclipse_type,
		   Ulint * lunation,
		   int *day, int *month, int *year, int hour, int min));
extern double
  next_sun_longitude __P_ ((double *longitude,
			    const double step,
			    const double mjd,
			    int *day,
			    int *month,
			    int *year, const int hour, const int min));
extern double
  sun_longitude __P_ ((const double longitude,
		       int *day,
		       int *month,
		       int *year, int hour, int min, const Bool next_mode));
extern void val2hours __P_ ((double val, int *hour, int *min, double *sec));
extern void
  val2degrees __P_ ((double val, int *degree, int *min, double *sec));
extern double my_cot __P_ ((double x));
extern double my_acot __P_ ((double x));
extern double my_atan2 __P_ ((double y, double x));
# if !HAVE_LIBM
extern double my_floor __P_ ((double x));
extern double my_ceil __P_ ((double x));
extern double my_sqrt __P_ ((double x));
extern double my_log __P_ ((double x));
extern double my_log10 __P_ ((double x));
extern double my_sin __P_ ((double x));
extern double my_cos __P_ ((double x));
extern double my_tan __P_ ((double x));
extern double my_cot __P_ ((double x));
extern double my_asin __P_ ((double x));
extern double my_acos __P_ ((double x));
extern double my_atan __P_ ((double x));
extern double my_acot __P_ ((double x));
# endif	/* !HAVE_LIBM */
__END_DECLARATIONS
#endif /* __HD_ASTRO_H */
