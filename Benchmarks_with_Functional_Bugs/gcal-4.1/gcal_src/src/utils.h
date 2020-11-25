#ifndef __UTILS_H
# define __UTILS_H
/*!
*  \file utils.h
*  \brief Pool of common functions header file.
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
*  $Id: utils.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `utils.c'.
*/
extern VOID_PTR
  my_malloc __P_ ((const int amount,
		   const int exit_status,
		   const char *module_name,
		   const long module_line,
		   const char *var_name, const int var_contents));
extern VOID_PTR
  my_realloc __P_ ((VOID_PTR ptr_memblock,
		    const int amount,
		    const int exit_status,
		    const char *module_name,
		    const long module_line,
		    const char *var_name, const int var_contents));
extern void
  allocate_all_strings __P_ ((const int amount,
			      const char *module_name,
			      const long module_line));
extern void
  resize_all_strings __P_ ((const int amount,
			    const Bool with_line_buffer,
			    const char *module_name, const long module_line));
extern void
  my_error __P_ ((const int exit_status,
		  const char *module_name,
		  const long module_line,
		  const char *var_name, const int var_contents));
# if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
extern RETSIGTYPE handle_signal __P_ ((int the_signal));
# endif
extern void my_exit __P_ ((const int exit_status));
extern int my_atoi __P_ ((const char *string));
extern int my_system __P_ ((const char *command));
# if !HAVE_STRSTR
extern char *my_strstr __P_ ((const char *text, const char *pattern));
# endif	/* !HAVE_STRSTR */
# if !HAVE_STRCSPN
extern char *my_strcspn __P_ ((const char *s1, const char *s2));
# endif	/* !HAVE_STRCSPN */
# if !HAVE_STRCASECMP
extern int my_strcasecmp __P_ ((const char *s1, const char *s2));
# endif	/* !HAVE_STRCASECMP */
# if !HAVE_STRNCASECMP
extern int my_strncasecmp __P_ ((const char *s1, const char *s2, int len));
# endif	/* !HAVE_STRNCASECMP */
extern Bool get_actual_date __P_ ((void));
extern int
  compare_d_m_name __P_ ((const char *string, const Cmode_enum mode));
extern int asc_sort __P_ ((const char **a, const char **b));
extern int des_sort __P_ ((const char **a, const char **b));
extern Bool is_presorted __P_ ((char **table, int elems));
extern void reverse_order __P_ ((char **table, const int elems));
extern const char *day_suffix __P_ ((int day));
extern const char *short3_day_name __P_ ((const int day));
extern const char *short_day_name __P_ ((const int day));
extern const char *day_name __P_ ((const int day));
extern const char *short_month_name __P_ ((const int month));
extern const char *month_name __P_ ((const int month));
extern Ulint date2num __P_ ((const int day, const int month, const int year));
extern void num2date __P_ ((Ulint mjd, int *day, int *month, int *year));
extern Bool
  doy2date __P_ ((int doy, const int is_leap_year, int *day, int *month));
extern int
  weekday_of_date __P_ ((const int day, const int month, const int year));
extern int
  day_of_year __P_ ((const int day, const int month, const int year));
extern int days_of_february __P_ ((const int year));
extern Bool
  valid_date __P_ ((const int day, const int month, const int year));
extern Bool prev_date __P_ ((int *day, int *month, int *year));
extern Bool next_date __P_ ((int *day, int *month, int *year));
extern int
  week_number __P_ ((const int day,
		     const int month,
		     const int year,
		     const Bool is_iso_week, const int start_day_of_week));
extern int
  weekno2doy __P_ ((int week,
		    const int year,
		    const Bool is_iso_week, const int start_day_of_week));
extern int knuth_easter_formula __P_ ((const int year));
extern int
  julian_gregorian_diff __P_ ((const int day,
			       const int month, const int year));
__END_DECLARATIONS
#endif /* __UTILS_H */
