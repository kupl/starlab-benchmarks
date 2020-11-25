/*!
*  \file rc-utils.c
*  \brief Pool of special functions necessary for managing
*               the fixed dates.
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
*  Include definition header file to see whether USE_RC is defined there.
*    Compile this module only if USE_RC is defined, otherwise skip it.
*/
#include "tailor.h"


#if USE_RC


/*
*  Include header files.
*/
# if HAVE_CTYPE_H
#  include <ctype.h>
# endif
# if HAVE_UNISTD_H
#  include <unistd.h>
# endif
# if HAVE_MATH_H && HAVE_LIBM
#  include <math.h>
# endif
# include "common.h"
# include "rc-defs.h"
# include "globals.h"
# include "file-io.h"
# include "hd-astro.h"
# include "hd-use.h"
# include "help.h"
# include "rc-astro.h"
# include "tty.h"
# include "utils.h"
# include "rc-utils.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-utils.c'.
*/
static void
  var_warning __P_ ((const int exit_status,
		     const int var_name,
		     const char *line_buffer,
		     const char *filename, const long line_number));
__END_DECLARATIONS
/*
*  Function implementations.
*/


Bool
rc_valid_day (date_text, day, month, year)
     const char *date_text;
     const int day;
     const int month;
     const int year;
/*!
   Checks the `date_text' for "%? special texts without argument", which
     disables a fixed dates and which is stored in `date_text' without a
     leading '%' character, and stores them into maps.  The `date_text'
     has to be proofed by the caller!  If the date given in `day', `month'
     and `year' is marked in the maps, we know that this date must be
     excluded so this function returns FALSE, otherwise TRUE.
*/
{
  register int i;
  register int wd = weekday_of_date (day, month, year);
  auto const char *ptr_date_text = date_text;
  auto Bool hd_found;
  static Bool inclusive_weekday_map[DAY_MAX + 1];
  static Bool exclusive_weekday_map[DAY_MAX + 1];


  for (i = DAY_MIN; i <= DAY_MAX; i++)
    inclusive_weekday_map[i] = !(exclusive_weekday_map[i] = TRUE);
  *inclusive_weekday_map = *exclusive_weekday_map = FALSE;
  while (*ptr_date_text)
    {
      hd_found = FALSE;
      switch (*ptr_date_text)
	{
	case RC_EX_LHDY_CHAR:
	case RC_EX_AHDY_CHAR:
	  /*
	     %exclude_legal_holiday special text or
	     %exclude_all_holiday special text found.
	   */
	  if (hd_ldays[((month - 1) * MONTH_LAST) + (day - 1)])
	    hd_found = TRUE;
	  if (hd_found || *ptr_date_text == RC_EX_LHDY_CHAR)
	    {
	      if (hd_found)
		{
		  exclusive_weekday_map[wd] = FALSE;
		  *exclusive_weekday_map = TRUE;
		}
	      break;
	    }
	  if (hd_mdays[((month - 1) * MONTH_LAST) + (day - 1)])
	    {
	      exclusive_weekday_map[wd] = FALSE;
	      *exclusive_weekday_map = TRUE;
	    }
	  break;
	case RC_EX_NLHDY_CHAR:
	case RC_EX_NAHDY_CHAR:
	  /*
	     %exclude_no_legal_holiday special text or
	     %exclude_no_all_holiday special text found.
	   */
	  if (hd_ldays[((month - 1) * MONTH_LAST) + (day - 1)])
	    {
	      hd_found = TRUE;
	      break;
	    }
	  if (hd_found || *ptr_date_text == RC_EX_NLHDY_CHAR)
	    {
	      if (hd_found)
		inclusive_weekday_map[wd] = TRUE;
	      *inclusive_weekday_map = TRUE;
	      break;
	    }
	  if (hd_mdays[((month - 1) * MONTH_LAST) + (day - 1)])
	    {
	      hd_found = TRUE;
	      break;
	    }
	  if (hd_found)
	    inclusive_weekday_map[wd] = TRUE;
	  *inclusive_weekday_map = TRUE;
	  break;
	case RC_EX_MON_CHAR:
	  /*
	     %exclude_monday special text found.
	   */
	  exclusive_weekday_map[DAY_MIN] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NMON_CHAR:
	  /*
	     %exclude_no_monday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[DAY_MIN] = TRUE;
	  break;
	case RC_EX_TUE_CHAR:
	  /*
	     %exclude_tuesday special text found.
	   */
	  exclusive_weekday_map[2] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NTUE_CHAR:
	  /*
	     %exclude_no_tuesday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[2] = TRUE;
	  break;
	case RC_EX_WED_CHAR:
	  /*
	     %exclude_wednesday special text found.
	   */
	  exclusive_weekday_map[3] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NWED_CHAR:
	  /*
	     %exclude_no_wednesday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[3] = TRUE;
	  break;
	case RC_EX_THU_CHAR:
	  /*
	     %exclude_thursday special text found.
	   */
	  exclusive_weekday_map[4] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NTHU_CHAR:
	  /*
	     %exclude_no_thursday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[4] = TRUE;
	  break;
	case RC_EX_FRI_CHAR:
	  /*
	     %exclude_friday special text found.
	   */
	  exclusive_weekday_map[5] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NFRI_CHAR:
	  /*
	     %exclude_no_friday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[5] = TRUE;
	  break;
	case RC_EX_SAT_CHAR:
	  /*
	     %exclude_saturday special text found.
	   */
	  exclusive_weekday_map[6] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NSAT_CHAR:
	  /*
	     %exclude_no_saturday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[6] = TRUE;
	  break;
	case RC_EX_SUN_CHAR:
	  /*
	     %exclude_sunday special text found.
	   */
	  exclusive_weekday_map[DAY_MAX] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NSUN_CHAR:
	  /*
	     %exclude_no_sunday special text found.
	   */
	  *inclusive_weekday_map = inclusive_weekday_map[DAY_MAX] = TRUE;
	  break;
	case RC_EX_MON_2_THU_CHAR:
	  /*
	     %exclude_monday_to_thursday special text found.
	   */
	  for (i = DAY_MIN; i <= 4; i++)
	    exclusive_weekday_map[i] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NMON_2_THU_CHAR:
	  /*
	     %exclude_no_monday_to_thursday special text found.
	   */
	  for (i = 0; i <= 4; i++)
	    inclusive_weekday_map[i] = TRUE;
	  break;
	case RC_EX_MON_2_FRI_CHAR:
	  /*
	     %exclude_monday_to_friday special text found.
	   */
	  for (i = DAY_MIN; i <= 5; i++)
	    exclusive_weekday_map[i] = FALSE;
	  *exclusive_weekday_map = TRUE;
	  break;
	case RC_EX_NMON_2_FRI_CHAR:
	  /*
	     %exclude_no_monday_to_friday special text found.
	   */
	  for (i = 0; i <= 5; i++)
	    inclusive_weekday_map[i] = TRUE;
	  break;
	default:
	  /*
	     This case MUST be an internal error!
	   */
	  abort ();
	}
      ptr_date_text++;
    }
  /*
     Check whether a weekday to exclude is marked in the maps and
     if so, avoid displaying the fixed date entry.
   */
  if (*inclusive_weekday_map || *exclusive_weekday_map)
    {
      if (*inclusive_weekday_map && *exclusive_weekday_map)
	{
	  if (!inclusive_weekday_map[wd] || !exclusive_weekday_map[wd])
	    return (FALSE);
	}
      else if (*inclusive_weekday_map)
	{
	  if (!inclusive_weekday_map[wd])
	    return (FALSE);
	}
      else if (!exclusive_weekday_map[wd])
	return (FALSE);
    }

  return (TRUE);
}



Bool
rc_valid_period (date_text, d, m, y, incr_year, decr_year)
     char *date_text;
     const int d;
     const int m;
     const int y;
     const int incr_year;
     const int decr_year;
/*!
   Checks the `date_text' for "%? special texts with date argument", which
     disables a fixed dates and which is stored in `date_text' without a
     leading '%' character, but separated by ',' colon characters, and stores
     them into maps.  The `date_text' has to be proofed by the caller!
     If `the_day', `the_month' and `the_year' is marked there, we know
     that this date must be excluded so this function returns FALSE,
     otherwise TRUE.
*/
{
  auto Slint num;
  register int i;
  static Bool inclusive_date_map[DAY_LAST + 2];
  static Bool exclusive_date_map[DAY_LAST + 2];
  auto int len;
  auto int rlen;
  auto int dd;
  auto int rdd;
  auto int mm;
  auto int rmm;
  auto int yy;
  auto int ryy;
  auto int nn;
  auto int rnn;
  auto int hhn;
  auto int rhn;
  auto int hhwd;
  auto int rhwd;
  auto char *ptr_date_text = date_text;
  auto char *ptr_char;
  auto char special_text_char;
  auto char ch;
  auto char hhc;
  auto char rhc;
  auto Bool is_weekday_mode;
  auto Bool ris_weekday_mode;
  auto Bool is_range;
  auto Bool dflt_yy_set;
  auto Bool dflt_ryy_set;


  /*
     Initialize the tables.
   */
  for (i = DAY_MIN; i < DAY_LAST + 2; i++)
    inclusive_date_map[i] = !(exclusive_date_map[i] = TRUE);
  *inclusive_date_map = *exclusive_date_map = FALSE;
  while (*ptr_date_text)
    {
      /*
         Check if a range of dates is given.
       */
      rhc = '\0';
      rlen = rdd = rmm = ryy = rnn = 0;
      is_range = dflt_yy_set = dflt_ryy_set = FALSE;
      special_text_char = *ptr_date_text++;
      ptr_char = ptr_date_text;
      while (*ptr_date_text
	     && (*ptr_date_text != *SPLIT_SEP)
	     && (*ptr_date_text != RC_DRANGE_CHAR))
	ptr_date_text++;
      if (*ptr_date_text == RC_DRANGE_CHAR)
	is_range = TRUE;
      ch = *ptr_date_text;
      *ptr_date_text = '\0';
      (void) rc_get_date (ptr_char, lptrs3, FALSE, &is_weekday_mode, &dd, &mm,
			  &yy, &nn, &len, &hhc, &hhn, &hhwd, _("Internal"),
			  -1L, date_text, FALSE);
      /*
         Error, invalid date encoded.
       */
      if (yy == SPECIAL_VALUE)
	{
	  fprintf (stderr, _("%s: invalid date given -- %s\n%s\n%s\n"),
		   prgr_name, date_text, usage_msg (), lopt_msg ());
	  my_exit (ERR_INVALID_OPTION);
	}
      *ptr_date_text = ch;
      if (is_range)
	{
	  ptr_char = ++ptr_date_text;
	  while (*ptr_date_text && (*ptr_date_text != *SPLIT_SEP))
	    ptr_date_text++;
	  ch = *ptr_date_text;
	  *ptr_date_text = '\0';
	  (void) rc_get_date (ptr_char, lptrs3, FALSE, &ris_weekday_mode,
			      &rdd, &rmm, &ryy, &rnn, &rlen, &rhc, &rhn,
			      &rhwd, _("Internal"), -1L, date_text, FALSE);
	  if (ryy == SPECIAL_VALUE)
	    {
	      fprintf (stderr, _("%s: invalid date given -- %s\n%s\n%s\n"),
		       prgr_name, date_text, usage_msg (), lopt_msg ());
	      my_exit (ERR_INVALID_OPTION);
	    }
	  *ptr_date_text = ch;
	}
      if (ch)
	ptr_date_text++;
      if (!len)
	dflt_yy_set = TRUE;
      if (!rlen)
	dflt_ryy_set = TRUE;
      if (!yy)
	{
	  if (dflt_yy_set)
	    yy = YEAR_MIN;
	  else
	    {
	      yy = year;
	      if (y && (fiscal_month > MONTH_MIN))
		yy = y;
	    }
	}
      if (!ryy)
	{
	  if (dflt_ryy_set)
	    ryy = YEAR_MAX;
	  else
	    {
	      ryy = year;
	      if (y && (fiscal_month > MONTH_MIN))
		ryy = y;
	    }
	}
      /*
         Respect possible fiscal year.
       */
      if (!dflt_yy_set && (yy != SPECIAL_VALUE))
	{
	  yy -= incr_year;
	  yy += decr_year;
	}
      if (!dflt_ryy_set && (ryy != SPECIAL_VALUE))
	{
	  ryy -= incr_year;
	  ryy += decr_year;
	}
      /*
         If @... "date"-part is given, compute the according date.
       */
      switch (hhc)
	{
	case RC_EASTER_CHAR:
	case RC_TODAY_CHAR:
	  if (!dflt_yy_set && (fiscal_month > MONTH_MIN))
	    {
	      if (!precomp_date (hhn, hhwd, &dd, &mm, yy + incr_year,
				 (hhc == RC_EASTER_CHAR) ? EAster : TOday))
		yy = SPECIAL_VALUE;
	    }
	  else
	    if (!precomp_date (hhn, hhwd, &dd, &mm, yy,
			       (hhc == RC_EASTER_CHAR) ? EAster : TOday))
	    yy = SPECIAL_VALUE;
	  break;
	case 'D':
	case 'W':
	  if (!dflt_yy_set && (fiscal_month > MONTH_MIN))
	    {
	      auto int fiscal_year = yy + incr_year;


	      if (!precomp_nth_wd (hhn, hhwd, &hhn, &dd, &mm, &fiscal_year,
				   (hhc == 'D') ? DAy : WEek))
		yy = fiscal_year;
	    }
	  else
	    (void) precomp_nth_wd (hhn, hhwd, &hhn, &dd, &mm, &yy,
				   (hhc == 'D') ? DAy : WEek);
	  break;
	default:
	  if (islower (hhc))
	    {
	      if (rc_dvar[IDX (hhc)].dvar_local.dvar_month)
		{
		  mm = (int) rc_dvar[IDX (hhc)].dvar_local.dvar_month;
		  dd = (int) rc_dvar[IDX (hhc)].dvar_local.dvar_day;
		}
	      else if (rc_dvar[IDX (hhc)].dvar_global.dvar_month)
		{
		  mm = (int) rc_dvar[IDX (hhc)].dvar_global.dvar_month;
		  dd = (int) rc_dvar[IDX (hhc)].dvar_global.dvar_day;
		}
	      if (!dflt_yy_set && (fiscal_month > MONTH_MIN))
		{
		  if (!precomp_date
		      (hhn, hhwd, &dd, &mm, yy + incr_year, DVar))
		    yy = SPECIAL_VALUE;
		}
	      else if (!precomp_date (hhn, hhwd, &dd, &mm, yy, DVar))
		yy = SPECIAL_VALUE;
	    }
	}
      switch (rhc)
	{
	case RC_EASTER_CHAR:
	case RC_TODAY_CHAR:
	  if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
	    {
	      if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy + incr_year,
				 (rhc == RC_EASTER_CHAR) ? EAster : TOday))
		ryy = SPECIAL_VALUE;
	    }
	  else
	    if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy,
			       (rhc == RC_EASTER_CHAR) ? EAster : TOday))
	    ryy = SPECIAL_VALUE;
	  break;
	case 'D':
	case 'W':
	  if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
	    {
	      auto int fiscal_year = ryy + incr_year;


	      if (!precomp_nth_wd (rhn, rhwd, &rhn, &rdd, &rmm, &fiscal_year,
				   (rhc == 'D') ? DAy : WEek))
		ryy = fiscal_year;
	    }
	  else
	    (void) precomp_nth_wd (rhn, rhwd, &rhn, &rdd, &rmm, &ryy,
				   (rhc == 'D') ? DAy : WEek);
	  break;
	default:
	  if (islower (rhc))
	    {
	      if (rc_dvar[IDX (rhc)].dvar_local.dvar_month)
		{
		  rmm = (int) rc_dvar[IDX (rhc)].dvar_local.dvar_month;
		  rdd = (int) rc_dvar[IDX (rhc)].dvar_local.dvar_day;
		}
	      else if (rc_dvar[IDX (rhc)].dvar_global.dvar_month)
		{
		  rmm = (int) rc_dvar[IDX (rhc)].dvar_global.dvar_month;
		  rdd = (int) rc_dvar[IDX (rhc)].dvar_global.dvar_day;
		}
	      if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
		{
		  if (!precomp_date
		      (rhn, rhwd, &rdd, &rmm, ryy + incr_year, DVar))
		    ryy = SPECIAL_VALUE;
		}
	      else if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy, DVar))
		ryy = SPECIAL_VALUE;
	    }
	}
      if (len > len_year_max)
	len -= len_year_max;
      else
	len = 0;
      if (rlen > len_year_max)
	rlen -= len_year_max;
      else
	rlen = 0;
      /*
         Assume current/first month of year.
       */
      if (!mm)
	{
	  if (len >= 1)
	    {
	      mm = m;
	      if (len == 2)
		len = 0;
	      else
		len--;
	    }
	  else
	    mm = MONTH_MIN;
	}
      else
	{
	  if (len == 2)
	    len = 0;
	  else
	    len--;
	}
      /*
         Assume current/first day of month.
       */
      if (!dd)
	{
	  if (len >= 1)
	    dd = d;
	  else
	    dd = DAY_MIN;
	}
      /*
         Assume current/last month of year.
       */
      if (!rmm)
	{
	  if (rlen >= 1)
	    {
	      rmm = m;
	      if (rlen == 2)
		rlen = 0;
	      else
		rlen--;
	    }
	  else
	    rmm = MONTH_MAX;
	}
      else
	{
	  if (rlen == 2)
	    rlen = 0;
	  else
	    rlen--;
	}
      /*
         Assume current/last day of month.
       */
      if (!rdd)
	{
	  if (rlen >= 1)
	    rdd = d;
	  else
	    {
	      if (rmm == 2)
		{
		  if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
		    rdd = days_of_february (ryy + incr_year);
		  else
		    rdd = days_of_february (ryy);
		}
	      else
		rdd = dvec[rmm - 1];
	    }
	}
      /*
         If special value "99" for day `dd' is given,
         set the day to last day of month.
       */
      if (dd == 99)
	{
	  /*
	     Assume the last day of month.
	   */
	  if (mm == 2)
	    dd = days_of_february (yy);
	  else
	    dd = dvec[mm - 1];
	}
      if (rdd == 99)
	{
	  /*
	     Assume the last day of month.
	   */
	  if (rmm == 2)
	    {
	      if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
		rdd = days_of_february (ryy + incr_year);
	      else
		rdd = days_of_february (ryy);
	    }
	  else
	    rdd = dvec[rmm - 1];
	}
      /*
         If "N'th weekday of month" entry set, compute the according date.
       */
      if (nn)
	nth_weekday_of_month (&dd, &mm, &yy, &nn, &is_weekday_mode);
      if (rnn)
	nth_weekday_of_month (&rdd, &rmm, &ryy, &rnn, &ris_weekday_mode);
      /*
         Proceed if (optionally specified) date is valid.
       */
      if ((!is_range
	   && (yy != SPECIAL_VALUE))
	  || (is_range && (yy != SPECIAL_VALUE) && (ryy != SPECIAL_VALUE)))
	{
	  register int true_year = (y) ? y : year + incr_year;


	  if (!nn
	      && !dflt_yy_set
	      && (fiscal_month > MONTH_MIN
		  || (incr_year && (rc_tomorrow_flag || rc_week_flag))))
	    yy += incr_year;
	  if (!rnn
	      && !dflt_ryy_set
	      && (fiscal_month > MONTH_MIN
		  || (incr_year && (rc_tomorrow_flag || rc_week_flag))))
	    ryy += incr_year;
	  /*
	     If starting date of event not greater than ending
	     date of event, mark the period in according map,
	     otherwise ignore the %?... special text completely.
	   */
	  num = d_between (dd, mm, yy, rdd, rmm, ryy);
	  if (num >= 0L)
	    {
	      register int s_doy = DAY_MIN;
	      register int e_doy = DAY_LAST + 1;


	      if (special_text_char == RC_IDATE_CHAR)
		*inclusive_date_map = TRUE;
	      else
		*exclusive_date_map = TRUE;
	      if (yy == true_year)
		s_doy = day_of_year (dd, mm, yy);
	      else if (yy > true_year)
		s_doy = SPECIAL_VALUE;
	      if (ryy == true_year)
		e_doy = day_of_year (rdd, rmm, ryy);
	      else if (ryy < true_year)
		e_doy = SPECIAL_VALUE;
	      if ((s_doy != SPECIAL_VALUE) && (e_doy != SPECIAL_VALUE))
		{
		  if (special_text_char == RC_IDATE_CHAR)
		    for (i = s_doy; i <= e_doy; i++)
		      inclusive_date_map[i] = TRUE;
		  else
		    for (i = s_doy; i <= e_doy; i++)
		      exclusive_date_map[i] = FALSE;
		}
	    }
	}
    }
  /*
     Check whether a period to exclude is marked in the maps and
     if so, avoid displaying the fixed date entry.
   */
  if (*inclusive_date_map || *exclusive_date_map)
    {
      i = day_of_year (d, m, year + incr_year - decr_year);
      if (*inclusive_date_map && *exclusive_date_map)
	{
	  if (!inclusive_date_map[i] || !exclusive_date_map[i])
	    return (FALSE);
	}
      else if (*inclusive_date_map)
	{
	  if (!inclusive_date_map[i])
	    return (FALSE);
	}
      else if (!exclusive_date_map[i])
	return (FALSE);
    }

  return (TRUE);
}



void
rc_clean_flags ()
/*!
   Cleans all global flags (except `rc_period_list')
     which are related to the fixed date period.
*/
{
  rc_tomorrow_flag = rc_week_flag = rc_month_flag = rc_year_flag
    = rc_week_year_flag = rc_forwards_flag = rc_backwards_flag =
    rc_period_flag = FALSE;
}



Line_struct *
rc_get_date (the_line, lineptrs, is_rc_file, is_weekday_mode, d, m, y, n, len,
	     hc, hn, hwd, filename, line_number, line_buffer, on_error_exit)
     char *the_line;
     Line_struct *lineptrs;
     const Bool is_rc_file;
     Bool *is_weekday_mode;
     int *d;
     int *m;
     int *y;
     int *n;
     int *len;
     char *hc;
     int *hn;
     int *hwd;
     const char *filename;
     const long line_number;
     const char *line_buffer;
     const Bool on_error_exit;
/*!
   Converts the textual/string `date' of a RC-file line into a numerical date
     and returns a pointer struct to the "day"-part and the "text"-part of the
     line indicating whether the "day"-part contains a list or a range of days;
     a char pointer to the "repeat"-field and to the "appears"-field if these
     exists, and/or if a @... or *... day is encoded in "date"-part and year
     is set to zero in the line, then this function returns holiday_mode_char
     (==date variable) or upper-case characters 'D' or 'W' in `&hc', the day
     displacement in `&hn' and a possible weekday name (mo...su) converted to
     a number (1...7) in `&hwd' for further managing of such a line.  If any
     invalid date is given in `the_line', then this function either returns
     SPECIAL_VALUE in &y or aborts the program with an error message
     (depending on mode of operation resp., contents of `on_error_exit'
     variable).
*/
{
  register int num_of_range_chars = 0;
  register int num_of_repeat_chars = 0;
  register int num_of_appears_chars = 0;
  register int i;
  static char str7[8];		/* For "date"-parts, length of 7 chars+'\0' maximum! */
  auto char *ptr_char;
  auto Bool is_hdy_mode = FALSE;


  *hc = '\0';
  lineptrs->day_list = lineptrs->day_range = FALSE;
  lineptrs->repeat_part = lineptrs->appears_part = (char *) NULL;
  (*len) = (*hn) = (*hwd) = (*n) = i = 0;
  /*
     Get the year from the year field of the line.
   */
  while (*the_line
	 && !isspace (*the_line) && isdigit (*the_line) && (i < len_year_max))
    str7[i++] = *the_line++;
  str7[i] = '\0';
  *y = my_atoi (str7);
  *len = i;
  /*
     Get the month from the month field of the line.
   */
  i = 0;
  while (*the_line && !isspace (*the_line) && (i < 2))
    str7[i++] = *the_line++;
  if (i)
    /*
       Try to get a short (3 character) textual month name.
     */
    if (isalpha (*the_line) && (isupper (str7[i - 1])
# if USE_EASC
				|| str7[i - 1] == *AE
				|| str7[i - 1] == *OE
				|| str7[i - 1] == *UE
				|| str7[i - 1] == *AAE
				|| str7[i - 1] == *OOE || str7[i - 1] == *UUE
# else /* !USE_EASC */
				|| str7[i - 1] == '"'
# endif	/* !USE_EASC */
				|| islower (str7[i - 1])))
      str7[i++] = *the_line++;
  str7[i] = '\0';
  *m = my_atoi (str7);
  if (!*m)
    /*
       Check for short (3 character) textual month name.
     */
    *m = compare_d_m_name (str7, MOnth);
  else if (i == 3 || ((i == 2) && (!isdigit (str7[1]))))
    {
      /*
         Error, invalid month field.
       */
      if (on_error_exit)
	my_error (ERR_INVALID_MONTH_FIELD, filename, line_number, line_buffer,
		  *m);
      *y = SPECIAL_VALUE;
    }
  /*
     Check if @... date variable statement or *... statement is given.
   */
  if (i)
    {
      *len += i;
      if (*str7 == RC_HDY_CHAR)
	{
	  is_hdy_mode = TRUE;
	  if (i == 2)
	    *hc = (char) tolower (str7[1]);
	}
      else if (*str7 == RC_NWD_CHAR)
	{
	  is_hdy_mode = TRUE;
	  if ((i == 2)
	      && (toupper (str7[1]) == 'D' || toupper (str7[1]) == 'W'))
	    *hc = (char) toupper (str7[1]);
	  else
	    {
	      if (i == 2)
		/*
		   Error, invalid mode specifying character given.
		 */
		*hc = (char) toupper (str7[1]);
	      else
		/*
		   Error, no mode specifying character given.
		 */
		*hc = *str7;
	    }
	}
    }
  /*
     If the special value "99" for a month `&m' is given,
     set the month to 12 (December).
   */
  if (*m == 99)
    *m = MONTH_MAX;
  if (!is_hdy_mode
      && (*m > MONTH_MAX
	  || (!*m
	      && (((i == 1)
		   && !isdigit (*str7))
		  || ((i == 2)
		      && (!isdigit (*str7)
			  || !isdigit (str7[1])))
		  || ((i == 3)
		      && (!isdigit (*str7)
			  || !isdigit (str7[1]) || !isdigit (str7[2])))))))
    {
      /*
         Error, invalid month field given.
       */
      if (on_error_exit)
	my_error (ERR_INVALID_MONTH_FIELD, filename, line_number, line_buffer,
		  *m);
      *y = SPECIAL_VALUE;
    }
  /*
     Get the day (maximum 3 characters in this case, template is either DD, WW  or WWW)
     resp., @... date variable or *... statement (maximum 7 characters in this case,
     template is: [+|-]NNNWWW).
   */
  ptr_char = lineptrs->day_part = the_line;
  i = 0;
  while (*the_line && !isspace (*the_line) && (i < ((is_hdy_mode) ? 7 : 3)))
    str7[i++] = *the_line++;
  str7[i] = '\0';
  *d = atoi (str7);
  *len += i;
  *is_weekday_mode = FALSE;
  the_line--;
  if (isalpha (*the_line) || ((i < 3) && !is_hdy_mode))
    the_line++;
  /*
     Check for a list/range of days/textual day names,
     if such a list is found, let `lineptrs->day_part' point to it
     and return to caller for further managing this list/range.
   */
  while (*ptr_char && !isspace (*ptr_char))
    {
      if (*ptr_char == RC_DLIST_CHAR)
	lineptrs->day_list = TRUE;
      else if (*ptr_char == RC_DRANGE_CHAR)
	{
	  num_of_range_chars++;
	  lineptrs->day_range = TRUE;
	}
      else if (*ptr_char == RC_REPEAT_CHAR)
	{
	  num_of_repeat_chars++;
	  lineptrs->repeat_part = ptr_char;
	}
      else if (*ptr_char == RC_APPEARS_CHAR)
	{
	  num_of_appears_chars++;
	  lineptrs->appears_part = ptr_char;
	}
      ptr_char++;
    }
  if (lineptrs->day_list || lineptrs->day_range)
    {
      if (is_rc_file)
	{
	  if ((num_of_range_chars > 1
	       || *ptr_char == RC_DLIST_CHAR
	       || *ptr_char == RC_DRANGE_CHAR
	       || (lineptrs->day_list
		   && lineptrs->day_range)
	       || (!lineptrs->day_list
		   && !lineptrs->day_range
		   && (num_of_repeat_chars > 1
		       || num_of_appears_chars > 1))) && on_error_exit)
	    /*
	       Error, invalid list/range of days.
	     */
	    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
		      line_buffer, 0);
	  /*
	     Check if a day variable is referenced.
	   */
	  if (islower (*hc)
	      && (*hc != RC_EASTER_CHAR) && (*hc != RC_TODAY_CHAR))
	    {
	      /*
	         Try to assign a local date variable if there is set any,
	         else try to assign a global date variable if there is set any,
	         otherwise we have to skip this part.
	       */
	      if (rc_dvar[IDX (*hc)].dvar_local.dvar_month
		  || rc_dvar[IDX (*hc)].dvar_global.dvar_month)
		{
		  if (rc_dvar[IDX (*hc)].dvar_local.dvar_month)
		    {
		      *m = (int) rc_dvar[IDX (*hc)].dvar_local.dvar_month;
		      *d = (int) rc_dvar[IDX (*hc)].dvar_local.dvar_day;
		    }
		  else
		    {
		      *m = (int) rc_dvar[IDX (*hc)].dvar_global.dvar_month;
		      *d = (int) rc_dvar[IDX (*hc)].dvar_global.dvar_day;
		    }
		}
	      else
		{
		  /*
		     Error, no such date variable defined.
		   */
		  if ((warning_level >= 0) && on_error_exit)
		    var_warning (ERR_INVALID_VAR_REFERENCE, (int) *hc,
				 line_buffer, filename, line_number);
		  *y = SPECIAL_VALUE;
		}
	    }
	  if (!isalpha (str7[i - 1]))
	    (*len)--;
	  i = 0;
	  while (*the_line && !isspace (*the_line))
	    {
	      the_line++;
	      i++;
	    }
	  *len += i;
	}
      else
	{
	  /*
	     Error, list/range of days is given in an expression it may not occur.
	   */
	  if (on_error_exit)
	    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
		      line_buffer, 0);
	  *y = SPECIAL_VALUE;
	}
    }
  else
    {
      if (!is_rc_file && (num_of_repeat_chars || num_of_appears_chars))
	{
	  /*
	     Error, day "repeat" or "appears" coding is given in an expression
	     it may not occur.
	   */
	  if (on_error_exit)
	    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
		      line_buffer, 0);
	  *y = SPECIAL_VALUE;
	}
      else if (num_of_repeat_chars > 1 || num_of_appears_chars > 1)
	{
	  /*
	     Error, "repeat" or "appears" coding given twice or more.
	   */
	  if (on_error_exit)
	    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
		      line_buffer, 0);
	  *y = SPECIAL_VALUE;
	}
      lineptrs->day_part = (char *) NULL;
    }
  /*
     If no list/range of days is given, try to precompute the according date.
   */
  if (lineptrs->day_part == (char *) NULL)
    {
      if (!is_hdy_mode)
	{
	  /*
	     Check for simple textual day name (either two or three characters),
	     template WW or WWW.
	   */
	  if (!*d)
	    {
	      if (*str7)
		*d = compare_d_m_name (str7, DAy);
	      if (*d)
		{
		  *is_weekday_mode = TRUE;
		  if (isdigit (str7[i - 1]))
		    (*len)--;
		}
	      else
		{
		  i = 0;
		  while (isdigit (str7[i]))
		    i++;
		  if (str7[i])
		    {
		      /*
		         Error, invalid day field.
		       */
		      if (on_error_exit)
			my_error (ERR_INVALID_DAY_FIELD, filename,
				  line_number, line_buffer, *d);
		      *y = SPECIAL_VALUE;
		    }
		}
	    }
	  else if ((i > 1) && !isdigit (str7[1]))
	    {
	      /*
	         Error, invalid day field.
	       */
	      if (on_error_exit)
		my_error (ERR_INVALID_DAY_FIELD, filename, line_number,
			  line_buffer, *d);
	      *y = SPECIAL_VALUE;
	    }
	  /*
	     Check whether a "N'th weekday of month" field exists.
	   */
	  if (*the_line && !isspace (*the_line))
	    {
	      if (isdigit (*the_line))
		{
		  *n = CHR2DIG (*the_line);
		  if (*n)
		    {
		      if ((*n > 5) && (*n < 9))
			{
			  /*
			     Error, invalid "N'th weekday of month" field.
			   */
			  if (on_error_exit)
			    my_error (ERR_INVALID_NWD_FIELD, filename,
				      line_number, line_buffer, *n);
			  *y = SPECIAL_VALUE;
			}
		    }
		}
	      else
		if ((lineptrs->repeat_part == (char *) NULL)
		    && (lineptrs->appears_part == (char *) NULL))
		{
		  /*
		     Error, missing separator between "date"-part
		     and "text"-part.
		   */
		  if (on_error_exit)
		    my_error (ERR_NO_SEPARATOR_CHAR, filename, line_number,
			      line_buffer, 0);
		  *y = SPECIAL_VALUE;
		}
	      if (*the_line)
		the_line++;
	      if (*the_line
		  && !isspace (*the_line)
		  && (lineptrs->repeat_part == (char *) NULL)
		  && (lineptrs->appears_part == (char *) NULL))
		{
		  /*
		     Error, missing separator between "date"-part and "text"-part.
		   */
		  if (on_error_exit)
		    my_error (ERR_NO_SEPARATOR_CHAR, filename, line_number,
			      line_buffer, 0);
		  *y = SPECIAL_VALUE;
		}
	      if (*n && (*d < DAY_MIN || *d > DAY_MAX))
		{
		  /*
		     Error, "N'th weekday of month" entry set
		     but invalid day encoded.
		   */
		  if (on_error_exit)
		    my_error (ERR_INVALID_DAY_FIELD, filename, line_number,
			      line_buffer, *d);
		  *y = SPECIAL_VALUE;
		}
	      (*len)++;
	      if (lineptrs->repeat_part != (char *) NULL
		  || lineptrs->appears_part != (char *) NULL)
		while (*the_line && !isspace (*the_line))
		  {
		    the_line++;
		    (*len)++;
		  }
	    }
	}
      else
	{
	  if (isdigit (*the_line))
	    the_line++;
	  if (*the_line
	      && !isspace (*the_line)
	      && (lineptrs->repeat_part == (char *) NULL)
	      && (lineptrs->appears_part == (char *) NULL))
	    {
	      /*
	         Error, missing separator character between "date"-part
	         and "text"-part.
	       */
	      if (on_error_exit)
		my_error (ERR_NO_SEPARATOR_CHAR, filename, line_number,
			  line_buffer, 0);
	      *y = SPECIAL_VALUE;
	    }
	  /*
	     Compute the base date of '@' date variable "date"-part of line
	     or '*' N'th weekday of year/weekday WW[W] of N'th week
	     in case an explicit year YYYY is given in the "date"-part.
	   */
	  i = atoi (str7);
	  ptr_char = str7;
	  if (islower (*hc))
	    {
	      if (*ptr_char == *ASC_LIT || *ptr_char == *DES_LIT)
		ptr_char++;
	      if (*ptr_char == *ASC_LIT
		  || *ptr_char == *DES_LIT || isalpha (*ptr_char))
		{
		  /*
		     Error, simple weekday name or invalid sign given.
		   */
		  if (on_error_exit)
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  *hc = '\0';
		  *d = 0;
		  *y = SPECIAL_VALUE;
		}
	    }
	  else if (*ptr_char == *ASC_LIT || *ptr_char == *DES_LIT)
	    {
	      /*
	         Error, invalid sign given.
	       */
	      if (on_error_exit)
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	      *hc = '\0';
	      *d = 0;
	      *y = SPECIAL_VALUE;
	    }
	  /*
	     Now eat all digits.
	   */
	  while (isdigit (*ptr_char))
	    ptr_char++;
	  if (*ptr_char
	      && (*ptr_char != RC_REPEAT_CHAR)
	      && (*ptr_char != RC_APPEARS_CHAR))
	    {
	      *hwd = compare_d_m_name (ptr_char, DAy);
	      if (!*hwd)
		{
		  /*
		     Error, invalid textual short day name given.
		   */
		  if (on_error_exit)
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  *hc = '\0';
		  *d = 0;
		  *y = SPECIAL_VALUE;
		}
	    }
	  if (*y >= 0)
	    {
	      if (*hc == RC_EASTER_CHAR || *hc == RC_TODAY_CHAR)
		{
		  if (!precomp_date (i, *hwd, d, m, *y,
				     (*hc ==
				      RC_EASTER_CHAR) ? EAster : TOday))
		    {
		      if (!*y)
			{
			  /*
			     No explicit year YYYY given in "date"-part of line.
			   */
			  *hn = i;
			  *d = (*m) = 0;
			}
		      else
			{
			  /*
			     Invalid relative date given.
			   */
			  *hc = '\0';
			  *d = 0;
			  *y = SPECIAL_VALUE;
			}
		    }
		  else
		    *hc = '\0';
		}
	      else if (islower (*hc))
		{
		  /*
		     Try to assign a local date variable if there is set any,
		     else try to assign a global date variable if there is
		     set any, otherwise we have to skip this part.
		   */
		  if (rc_dvar[IDX (*hc)].dvar_local.dvar_month
		      || rc_dvar[IDX (*hc)].dvar_global.dvar_month)
		    {
		      if (rc_dvar[IDX (*hc)].dvar_local.dvar_month)
			{
			  *m = (int) rc_dvar[IDX (*hc)].dvar_local.dvar_month;
			  *d = (int) rc_dvar[IDX (*hc)].dvar_local.dvar_day;
			}
		      else
			{
			  *m =
			    (int) rc_dvar[IDX (*hc)].dvar_global.dvar_month;
			  *d = (int) rc_dvar[IDX (*hc)].dvar_global.dvar_day;
			}
		      if (!precomp_date (i, *hwd, d, m, *y, DVar))
			{
			  if (!*y)
			    /*
			       No explicit year YYYY given in "date"-part of line.
			     */
			    *hn = i;
			  else
			    {
			      /*
			         Invalid relative date given.
			       */
			      *hc = '\0';
			      *d = 0;
			      *y = SPECIAL_VALUE;
			    }
			}
		      else
			*hc = '\0';
		    }
		  else
		    {
		      /*
		         Error, no such date variable defined.
		       */
		      if ((warning_level >= 0) && on_error_exit)
			var_warning (ERR_INVALID_VAR_REFERENCE, (int) *hc,
				     line_buffer, filename, line_number);
		      *hc = '\0';
		      *d = 0;
		      *y = SPECIAL_VALUE;
		    }
		}
	      else if (*hc == 'D' || *hc == 'W')
		{
		  /*
		     Try to compute the '*' N'th weekday of year resp.,
		     weekday WW[W] of N'th week statement.
		   */
		  if (*y == 0)
		    {
		      /*
		         No explicit year YYYY given in "date"-part of line.
		       */
		      *hn = i;
		      *d = 0;
		      *m = 0;
		    }
		  else
		    if (precomp_nth_wd (i, *hwd, hn, d, m, y,
					(*hc == 'D') ? DAy : WEek))
		    *hc = '\0';
		}
	      else
		/*
		   Error, either an invalid date variable character trails
		   the holiday mode character '@', or an invalid character
		   trails the "N'th weekday of year" resp., weekday
		   WW[W] of "N'th week mode" character '*'.
		 */
	      if (on_error_exit)
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	    }
	  if (lineptrs->repeat_part != (char *) NULL
	      || lineptrs->appears_part != (char *) NULL)
	    while (*the_line && !isspace (*the_line))
	      {
		the_line++;
		(*len)++;
	      }
	  if (*the_line)
	    the_line++;
	}
    }
  /*
     Now let's allocate memory for all pointers to texts of the `lineptrs'
     structure if we work on a resource/include file (except `text_part').
     That's absolutely necessary because after a potential resizing of
     "all strings" elsewhere in a later part of the program, these pointers
     could get lost otherwise.  The caller has to free this memory!
   */
  if (is_rc_file)
    {
      /*
         ONLY IF DETECTED!
       */
      if (lineptrs->day_part != (char *) NULL)
	{
	  ptr_char = lineptrs->day_part;
	  i = 0;
	  LOOP
	  {
	    if (!*ptr_char || isspace (*ptr_char))
	      break;
	    i++;
	    ptr_char++;
	  }
	  ptr_char = lineptrs->day_part;
	  lineptrs->day_part =
	    (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				((long) __LINE__) - 1L, "lineptrs->day_part",
				0);
	  strncpy (lineptrs->day_part, ptr_char, i);
	  lineptrs->day_part[i] = '\0';
	}
      /*
         ONLY IF DETECTED!
       */
      if (lineptrs->repeat_part != (char *) NULL)
	{
	  ptr_char = lineptrs->repeat_part;
	  i = 0;
	  LOOP
	  {
	    if (!*ptr_char || isspace (*ptr_char))
	      break;
	    i++;
	    ptr_char++;
	  }
	  ptr_char = lineptrs->repeat_part;
	  lineptrs->repeat_part =
	    (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				((long) __LINE__) - 1L,
				"lineptrs->repeat_part", 0);
	  strncpy (lineptrs->repeat_part, ptr_char, i);
	  lineptrs->repeat_part[i] = '\0';
	}
      /*
         ONLY IF DETECTED!
       */
      if (lineptrs->appears_part != (char *) NULL)
	{
	  ptr_char = lineptrs->appears_part;
	  i = 0;
	  LOOP
	  {
	    if (!*ptr_char || isspace (*ptr_char))
	      break;
	    i++;
	    ptr_char++;
	  }
	  ptr_char = lineptrs->appears_part;
	  lineptrs->appears_part =
	    (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				((long) __LINE__) - 1L,
				"lineptrs->appears_part", 0);
	  strncpy (lineptrs->appears_part, ptr_char, i);
	  lineptrs->appears_part[i] = '\0';
	}
      if ((lineptrs->repeat_part != (char *) NULL
	   || lineptrs->appears_part != (char *) NULL)
	  && !is_hdy_mode
	  && !*is_weekday_mode && !lineptrs->day_list && !lineptrs->day_range)
	(*len)--;
    }
  /*
     ALWAYS!
   */
  lineptrs->text_part = the_line;

  return (lineptrs);
}



Bool
precomp_nth_wd (diff, wd, n, day, month, year, mode)
     int diff;
     const int wd;
     int *n;
     int *day;
     int *month;
     int *year;
     const Cmode_enum mode;
/*!
   Precomputes the date of the "N'th absolute weekday" `wd' of the year
     or the date of weekday `wd' of the "N'th absolute week" of the year
     (returned in `&day', `&month' and `&year'; if `&year' is not concrete
     the computed `diff' is returned by `&n'), and returns TRUE in case such
     a date exits in the year, otherwise FALSE.
*/
{
  register int the_diff = diff;
  register int j = DAY_LAST + (days_of_february (*year) == 29);
  auto int i = 0;


  if (*year)
    {
      if (mode == DAy)
	{
	  *day = DAY_MIN;
	  *month = MONTH_MIN;
	  if (wd)
	    {
	      if (the_diff == WEEK_MAX + 1 || the_diff == 99)
		{
		  i = the_diff;
		  diff = WEEK_MAX;
		}
	    }
	  else
	    {
	      /*
	         If a special value "999" for `diff' is given,
	         set it to last day of year (365|366).
	       */
	      if (the_diff == 999)
		diff = j;
	      i = diff--;
	    }
	}
      else
	{
	  register int k = (iso_week_number) ? DAY_MIN : start_day;


	  /*
	     `mode' == WEek.
	   */
	  diff = i = weekno2doy (the_diff, *year, iso_week_number, k);
	  if (diff > DAY_MIN)
	    {
	      diff--;
	      k = j - diff;
	      if (iso_week_number)
		j = wd;
	      else
		j = SYEAR (wd, start_day);
	      /*
	         If a weekday of the LAST week (==99) is wanted, but this
	         weekday doesn't exist anymore in the last week of the
	         current year by reason it is already located in the next
	         year, let's use the last date at which this weekday occurs
	         in the current year instead.
	       */
	      if ((the_diff == 99) && (*year != YEAR_MAX) && (j > k))
		diff -= DAY_MAX;
	    }
	  else
	    diff = 1;
	  if (doy2date (diff, (days_of_february (*year) == 29), day, month))
	    diff = 1;
	}
    }
  if (!precomp_date (diff, wd, day, month, *year, DVar))
    {
      if (!*year)
	{
	  /*
	     No explicit year YYYY given in "date"-part of line.
	   */
	  *day = 0;
	  *month = 0;
	  *n = diff;
	}
      else
	{
	  /*
	     Invalid relative date given.
	   */
	  *day = 0;
	  *month = 0;
	  *year = SPECIAL_VALUE;
	}
      return (FALSE);
    }
  else
    {
      if (wd && (mode == DAy))
	{
	  register int year_old = (*year);


	  if (i)
	    for (diff = DAY_MIN; diff <= DAY_MAX; diff++)
	      (void) next_date (day, month, year);
	  if (((*day <= DAY_MAX)
	       && (*year != year_old))
	      || weekday_of_date (DAY_MIN, MONTH_MIN, *year) == wd)
	    for (diff = DAY_MIN; diff <= DAY_MAX; diff++)
	      (void) prev_date (day, month, year);
	  if (i == WEEK_MAX + 1)
	    {
	      i = DAY_MIN;
	      *month = MONTH_MIN;
	      (void) precomp_date (WEEK_MAX, wd, &i, month, *year, DVar);
	      if ((*day == i)
		  && (weekday_of_date (DAY_MIN, MONTH_MIN, *year) != wd))
		{
		  /*
		     Error, no such 53rd weekday WW[W] of year.
		   */
		  *day = 0;
		  *month = 0;
		  *year = SPECIAL_VALUE;
		  return (FALSE);
		}
	    }
	}
      else
	/*
	   `mode' == WEek.
	 */
      if (!wd
	    || i < DAY_MIN
	    || ((the_diff <= 1) && (*day == DAY_MAX + 1) && (wd == DAY_MIN)))
	{
	  if (*day >= DAY_MAX + i)
	    *day -= DAY_MAX;
	  else
	    if (!wd
		&& (i < DAY_MIN || ((*day == DAY_MIN + 1) && (i == DAY_MIN))))
	    (*day)--;
	  if (*day < DAY_MIN)
	    {
	      /*
	         Error, N'th week doesn't contain such a weekday WW[W].
	       */
	      *day = 0;
	      *month = 0;
	      *year = SPECIAL_VALUE;
	      return (FALSE);
	    }
	}
    }

  return (TRUE);
}



Bool
precomp_date (diff, wd, day, month, year, mode)
     int diff;
     const int wd;
     int *day;
     int *month;
     const int year;
     const Cmode_enum mode;
/*!
   Precomputes the date relative to Easter Sunday's date (mode==EAster),
     relative to today's date (mode==TOday) or relative to date variables
     date (mode==DVar) plus displacement `diff' or displacement `diff' `wd'
     (returned in `&day' and `&month'), and returns TRUE in case such a date
     exits in the year, otherwise FALSE.
*/
{
  register int i;


  if (((mode == EAster)
       && (year >= EASTER_MIN)
       && (year <= EASTER_MAX))
      || ((mode == TOday
	   || mode == DVar) && (year >= YEAR_MIN) && (year <= YEAR_MAX)))
    {
      switch (mode)
	{
	case EAster:
	  i = knuth_easter_formula (year);
	  break;
	case TOday:
	  *day = act_day;
	  *month = act_month;
	  /* Fallthrough. */
	default:
	  if (!valid_date (*day, *month, year))
	    /*
	       Error, invalid date given (e.g. 19010229).
	     */
	    return (FALSE);
	  i = day_of_year (*day, *month, year);
	}
      if (wd)
	{
	  /*
	     Calculate date like:  3rd(`diff') Friday(`wd') before Easter Sunday's date.
	   */
	  if (wd < DAY_MIN || wd > DAY_MAX)
	    /*
	       Error, invalid weekday specified.
	     */
	    return (FALSE);
	  else if (!diff)
	    /*
	       Error, a weekday but no difference specified.
	     */
	    return (FALSE);
	  else if (diff == -99)
	    {
	      /*
	         Detect first weekday `wd' of year.
	       */
	      *month = MONTH_MIN;
	      *day = eval_holiday (DAY_MIN, *month, year, wd, TRUE);
	      return (TRUE);
	    }
	  else if (diff == 99)
	    {
	      /*
	         Detect last weekday `wd' of year.
	       */
	      *month = MONTH_MAX;
	      *day =
		eval_holiday (dvec[MONTH_MAX - 1], *month, year, wd, FALSE);
	      return (TRUE);
	    }
	  else
	    {
	      register int act_wd;
	      auto int d;
	      auto int m;
	      auto int y = year;


	      (void) doy2date (i, (days_of_february (y) == 29), &d, &m);
	      act_wd = weekday_of_date (d, m, y);
	      if (act_wd != wd)
		{
		  if (diff < 0)
		    {
		      /*
		         Try to detect first weekday `wd' before actual date.
		       */
		      while (act_wd != wd)
			{
			  (void) prev_date (&d, &m, &y);
			  act_wd = weekday_of_date (d, m, y);
			  i--;
			}
		      diff++;
		    }
		  else
		    {
		      /*
		         Try to detect first weekday `wd' after actual date.
		       */
		      while (act_wd != wd)
			{
			  (void) next_date (&d, &m, &y);
			  act_wd = weekday_of_date (d, m, y);
			  i++;
			}
		      diff--;
		    }
		}
	      if (y != year)
		/*
		   Error, we have left the year bounds.
		 */
		return (FALSE);
	      /*
	         Calculate the difference.
	       */
	      i += diff * DAY_MAX;
	    }
	}
      else
	{
	  /*
	     Calculate the difference.
	   */
	  if (diff == -999)
	    i = 1;
	  else if (diff == 999)
	    i = DAY_LAST + (days_of_february (year) == 29);
	  else
	    i += diff;
	}
      if (doy2date (i, (days_of_february (year) == 29), day, month))
	return (TRUE);
    }

  return (FALSE);
}



void
set_dvar (line_buffer, lineptrs, filename, line_number, mode)
     const char *line_buffer;
     Line_struct *lineptrs;
     const char *filename;
     const long line_number;
     const Var_enum mode;
/*!
   Scans given string `line_buffer' and tries to detect a valid date variable
     reference, which can be:
       1) DVAR=``NOTHING''     --> Undefine local DVAR so we are able to use
                                     its global value.  If `mode' is set to
                                     "GLobal", this "empty" assignment results
                                     an error.
       2) DVAR=MMDD            --> Assignment of a constant date expression
                                     MMDD.
       3) DVAR=MMWW[W]N        --> Assignment of a dynamic date expression
                                     N'th weekday WW[W] of month MM.
       4) DVAR=*dN[WW[W]]      --> Assignment of a dynamic date expression
                                     N'th weekday WW[W] of year.
       5) DVAR=*wN[WW[W]]      --> Assignment of a dynamic date expression
                                     weekday WW[W] of N'th week of year.
       6) DVAR=DVAR            --> Assignment of a date variable DVAR,
                                     which must be already defined.
       7) DVAR=DVAR[+|-]N      --> Assignment of a date variable DVAR,
                                     which must be already defined, plus/minus
                                     N days.
       8) DVAR=DVAR[+|-]NWW[W] --> Assignment of a date variable DVAR,
                                     which must be already defined, plus/minus
                                     N weekdays WW[W].
       9) DVAR++               --> Simple incrementation by one day.
      10) DVAR--               --> Simple decrementation by one day.
      11) DVAR+=[+|-]N         --> Addition of a constant numeric
                                     factor [+|-]N.
      12) DVAR-=[+|-]N         --> Subtraction of a constant numeric
                                     factor [+|-]N.
      13) DVAR+=[+|-]NWW[W]    --> Addition of [+|-]N weekdays WW[W].
      14) DVAR-=[+|-]NWW[W]    --> Subtraction of [+|-]N weekdays WW[W].
     A date variable name is valid from a...d, f...s and u...z (24 variables
     totally, case-insensitive), because the `e' variable is always reserved
     for the current Easter Sunday's date and the `t' variable is always
     reserved for today's date, so we must skip any reference to these
     variables.
     No whitespace characters may occur between the date variable, operator
     and value.  Stores assignment (1)...(8) at position `date variable'
     into global date variable vector `rc_dvar[]' (either the local or the
     global ones, depending on given `mode', which can be either "GLobal"
     or "LOcal".  Assignment (1), (3)...(5), (7), (8) and operation
     (9)...(14) may ONLY be used on local date variables.
*/
{
  register int error = 0;
  auto char dvar = '\0';


  /*
     Skip and return error if invalid date variable name is given.
   */
  if (isalpha (*line_buffer)
      && (tolower (*line_buffer) != RC_EASTER_CHAR)
      && (tolower (*line_buffer) != RC_TODAY_CHAR))
    {
      auto int i;
      auto int len;
      auto int d = 0;
      auto int m = 0;
      auto int y = year;
      auto int n;
      auto const char *ptr_char = line_buffer;
      auto char op;
      auto char op2;
      static char str20[21];
      auto Bool is_weekday_mode;
      auto Bool dvar_with_displacement = FALSE;
      auto Bool dvar_add_sub = FALSE;
      auto Bool dvar_inc_dec = FALSE;


      ptr_char++;
      /*
         Check if assignment (1)...(8) is given.
       */
      if (*ptr_char != *RC_VAR_ASSIGN)
	{
	  if ((*ptr_char != *RC_VAR_ADD) && (*ptr_char != *RC_VAR_SUB))
	    /*
	       Error, invalid first operator character found
	       (neither '+' nor '-' given).
	     */
	    error = ERR_ILLEGAL_VAR_DEFINITION;
	  else
	    {
	      /*
	         Check if operation (9)...(14) is given.
	       */
	      op = *ptr_char++;
	      if (*ptr_char)
		{
		  op2 = *ptr_char++;
		  if (op2 == op || op2 == *RC_VAR_ASSIGN)
		    {
		      if (mode == LOcal)
			m =
			  (int) rc_dvar[IDX (*line_buffer)].dvar_local.
			  dvar_month;
		      if (m)
			{
			  if (op == op2)
			    {
			      while (isspace (*ptr_char))
				ptr_char++;
			      if (*ptr_char)
				/*
				   Error, found invalid trailing characters.
				 */
				error = ERR_ILLEGAL_VAR_OPERATION;
			      else
				/*
				   Either DVAR++ or DVAR-- found.
				 */
				dvar_inc_dec = TRUE;
			    }
			  else
			    {
			      /*
			         Either DVAR+=... or DVAR-=... found.
			       */
			      dvar_add_sub = TRUE;
			      /*
			         Respect a trailing sign of the value.
			       */
			      if (*ptr_char == *RC_VAR_ADD
				  || *ptr_char == *RC_VAR_SUB)
				{
				  if (op == *RC_VAR_SUB)
				    {
				      if (*ptr_char == *RC_VAR_ADD)
					op = *RC_VAR_SUB;
				      else
					op = *RC_VAR_ADD;
				    }
				  else
				    op = *ptr_char;
				  ptr_char++;
				}
			    }
			  if (!error)
			    goto LABEL_compute_dvar;
			}
		      else
			{
			  if (mode == GLobal)
			    /*
			       Error, operation given in global mode.
			     */
			    error = ERR_ILLEGAL_VAR_OPERATION;
			  else
			    /*
			       Error, date variable undefined.
			     */
			    error = ERR_INVALID_VAR_REFERENCE;
			}
		    }
		  else
		    /*
		       Error, invalid second operator character found
		       (no '=', '+' or '-' given resp.,
		       illegal combination of '+' and '-').
		     */
		    error = ERR_ILLEGAL_VAR_OPERATION;
		}
	      else
		/*
		   Error, incomplete operator found (neither '+=', '-=', '++'
		   nor '--' given).
		 */
		error = ERR_ILLEGAL_VAR_OPERATION;
	    }
	}
      else
	{
	  /*
	     Assignment (1)...(8) to date variable found (simple '=' given),
	     scan expression part of date variable definition.  Assignments
	     (1), (3)...(5), (7), (8) are ONLY allowed for local date
	     variables.
	   */
	  i = 0;
	  ptr_char++;
	  if (!*ptr_char)
	    {
	      /*
	         No date assigned ("empty" assignment), set the date variable
	         slot to zero so we are able to use its possibly set global
	         value if this variable is referenced again at a later place
	         within the sequence.  This kind of assignment is allowed for
	         local date variables only; for global date variables, we
	         have to report an error instead.
	       */
	      if (mode == GLobal)
		/*
		   Error, "empty" assignment on a global date variable given.
		 */
		error = ERR_ILLEGAL_VAR_DEFINITION;
	    }
	  else
	    {
	      if (isalpha (*ptr_char) && !isalpha (*(ptr_char + 1)))
		{
		  dvar = op = *ptr_char;
		  ptr_char++;
		  if (!*ptr_char || isspace (*ptr_char))
		    {
		      if (tolower (dvar) == RC_EASTER_CHAR
			  || tolower (dvar) == RC_TODAY_CHAR)
			/*
			   Error, date variable is invalid.
			 */
			error = ERR_INVALID_VAR_ASSIGNMENT;
		      else
			{
			  /*
			     If the character after '=' is alphabetic and is not
			     trailed by digits, assume assignment (6) is given.
			   */
			  if (mode == GLobal)
			    {
			      m =
				(int) rc_dvar[IDX (dvar)].dvar_global.
				dvar_month;
			      d =
				(int) rc_dvar[IDX (dvar)].dvar_global.
				dvar_day;
			    }
			  else
			    {
			      m =
				(int) rc_dvar[IDX (dvar)].dvar_local.
				dvar_month;
			      d =
				(int) rc_dvar[IDX (dvar)].dvar_local.dvar_day;
			    }
			  if (!m)
			    /*
			       Error, date variable undefined.
			     */
			    error = ERR_INVALID_VAR_REFERENCE;
			}
		    }
		  else
		    {
		      /*
		         Check if assignments (7)...(8) are given.
		       */
		      if (*ptr_char == *ASC_LIT
			  || *ptr_char == *DES_LIT || isdigit (*ptr_char))
			{
			  ptr_char--;
			  dvar_with_displacement = TRUE;
			  goto LABEL_compute_dvar;
			}
		      else
			/*
			   Error, invalid date variable name given.
			 */
			error = ERR_ILLEGAL_VAR_DEFINITION;
		    }
		}
	      else
		{
		LABEL_compute_dvar:
		  /*
		     Assuming the string vectors have a minimum length of 1024
		     Bytes and the maximum text length of a date variable
		     assignment/operation in a line may not be longer than
		     20 Bytes, let's use these 20 Bytes of the line only.
		   */
		  strncpy (str20, ptr_char, 20);
		  str20[20] = '\0';
		  if (dvar_with_displacement)
		    sprintf (s5, "%0*d%c%s", len_year_max, y, RC_HDY_CHAR,
			     str20);
		  else if (dvar_add_sub)
		    sprintf (s5, "%0*d%c%c%c%s", len_year_max, y, RC_HDY_CHAR,
			     *line_buffer, op, str20);
		  else if (dvar_inc_dec)
		    sprintf (s5, "%0*d%c%c%c1", len_year_max, y, RC_HDY_CHAR,
			     *line_buffer, op);
		  else
		    sprintf (s5, "%0*d%s", len_year_max, y, str20);
		  /*
		     `rc_get_date()' arguments `len' and `i' are dummys
		     only and must be given.  They are not used further!
		   */
		  (void) rc_get_date (s5, lineptrs, FALSE, &is_weekday_mode,
				      &d, &m, &y, &n, &len, &op, &i, &i,
				      filename, line_number, line_buffer,
				      TRUE);
		  if (y != SPECIAL_VALUE)
		    {
		      /*
		         Check if assignments (3)...(5) are given.
		       */
		      if ((mode == GLobal) && (op || is_weekday_mode))
			error = ERR_ILLEGAL_VAR_OPERATION;
		      else
			{
			  /*
			     Assignments (2)...(3) are given.
			   */
			  if (m < MONTH_MIN || m > MONTH_MAX)
			    /*
			       Error, invalid month given.
			     */
			    error = ERR_ILLEGAL_VAR_DEFINITION;
			  else
			    {
			      i = dvec[m - 1];
			      if (m == 2)
				i += is_leap_year;
			      /*
			         Check for assignment (3) DVAR=MMWW[W]N
			         (WW=mo...su, WWW=mon...sun, N=1...5|9),
			         e.g.: x=03mo3  sets `x' to date of 3rd Monday
			         in March.
			         e.g.: x=03mon3  sets `x' to date of 3rd Monday
			         in March, too.
			       */
			      if (is_weekday_mode)
				{
				  if (n == 9)
				    d = eval_holiday (i, m, year, d, FALSE);
				  else
				    {
				      d =
					eval_holiday (DAY_MIN, m, year, d,
						      TRUE);
				      d += (DAY_MAX * (n - 1));
				      if (d > i)
					/*
					   Month contains no such "N'th weekday of
					   month", ignore the assignment.
					 */
					error = ERR_INVALID_VAR_ASSIGNMENT;
				    }
				}
			      else
				{
				  /*
				     Assume assignment (1) is given.
				   */
				  if (d == 99)
				    d = i;
				  /*
				     We must avoid an assigment like DVAR=0229
				     if we are in fiscal year mode and the next
				     year is no leap year and no `--leap-day=ARG'
				     option is given!
				   */
				  if ((fiscal_month > MONTH_MIN + 1)
				      && (days_of_february (year + 1) == 28)
				      && !rc_feb_29_to_feb_28
				      && !rc_feb_29_to_mar_01
				      && (m == 2) && (d == 29))
				    /*
				       Year contains no such date, ignore the assignment.
				     */
				    error = ERR_INVALID_VAR_ASSIGNMENT;
				  else
				    {
				      if (d > i)
					{
					  manage_leap_day (&d, &m, year,
							   line_buffer,
							   filename,
							   line_number);
					  i = d;
					}
				      if (d < DAY_MIN || d > i)
					/*
					   Error, invalid day given.
					 */
					error = ERR_ILLEGAL_VAR_DEFINITION;
				    }
				}
			    }
			}
		    }
		  else
		    /*
		       Year contains no such date, ignore the assignment.
		     */
		    error = ERR_INVALID_VAR_ASSIGNMENT;
		}
	    }
	}
      if (!error)
	{
	  /*
	     Store the assigned/calculated date.
	   */
	  if (mode == GLobal)
	    {
	      rc_dvar[IDX (*line_buffer)].dvar_global.dvar_month = (char) m;
	      rc_dvar[IDX (*line_buffer)].dvar_global.dvar_day = (char) d;
	    }
	  else
	    {
	      rc_dvar[IDX (*line_buffer)].dvar_local.dvar_month = (char) m;
	      rc_dvar[IDX (*line_buffer)].dvar_local.dvar_day = (char) d;
	    }
	}
    }
  else
    /*
       Error, invalid date variable name given (not a...d, f...s, u...z).
     */
    error = ERR_ILLEGAL_VAR_DEFINITION;
  if (error)
    {
      if ((mode == GLobal)
	  && (error == ERR_ILLEGAL_VAR_DEFINITION
	      || error == ERR_ILLEGAL_VAR_OPERATION))
	warning_level = WARN_LVL_MAX;
      if (warning_level >= 0)
	{
	  if (!dvar)
	    dvar = *line_buffer;
	  var_warning (error, (int) dvar, line_buffer, filename, line_number);
	}
    }
}



void
set_tvar (line_buffer, filename, line_number, mode)
     const char *line_buffer;
     const char *filename;
     const long line_number;
     const Var_enum mode;
/*!
   Scans given string `line_buffer' and tries to detect a valid text variable
     reference, which is:
       1) $TVAR=[TEXT]  --> Assignment of a constant text expression TEXT
                              to TVAR.  TEXT may contain references to
                              other TVAR's, which are always expanded
                              recursively before the assignment is performed!
       2) $TVAR?COMMAND --> Interpreted assignment of that text to TVAR, which
                              is created by the COMMAND on the STDOUT channel.
                              The text may contain references to other TVAR's,
                              which are expanded in case TVAR is referenced
                              at a later place of program execution.
       3) $TVAR:COMMAND --> Uninterpreted assignment of that text to TVAR, which
                              is created by the COMMAND on the STDOUT channel.
                              References to other TVAR's are not expanded!
       4) $TVAR++       --> Simple incrementation by one (length preserved).
       5) $TVAR--       --> Simple decrementation by one (length preserved).
       6) $TVAR+=[+|-]N --> Addition of a constant numeric
                              factor [+|-]N (length preserved).
       7) $TVAR-=[+|-]N --> Subtraction of a constant numeric
                              factor [+|-]N (length preserved).
     A text variable name is valid from $a...$z (totally 26 variables,
     case-insensitve).  No whitespace characters may occur between the
     text variable prefix character '$' and the text variable letter itself,
     the operator and the value.
     In general, assignment (1)...(3) is stored at position `text variable'
     into the global text variable vector `rc_tvar[]' (either the local or
     the  global ones, depending on given `mode', which can be either "GLobal"
     or "LOcal".
     Assignment (2) inserts the text created by the COMMAND into the TVAR as is,
     but only if it is allowed (`--execute-command' option must be given).
     Assignment (3) inserts the text created by the COMMAND into the TVAR
     postprocessed by the Txt2gcal program, but only if it is allowed
     (`--execute-command' option must be given).
     Operation (4)...(7) may ONLY be used on local text variables (if they
     contain integer values).
     Uses the global text buffers `s5' and `s7' internally.
     Returns FALSE if an error occurs, otherwise TRUE.
*/
{
  register int error = 0;
  auto char tvar = '\0';


  if (*line_buffer != RC_TVAR_CHAR)
    /*
       Error, no leading '$' character (text variable prefix) given.
     */
    error = ERR_ILLEGAL_VAR_DEFINITION;
  else
    {
      auto char *ptr_char = (char *) line_buffer;


      /*
         Skip the trailing '$' character of a text variable by default.
       */
      ptr_char++;
      /*
         Skip and return error if invalid text variable name is given.
       */
      if (!isalpha (*ptr_char))
	/*
	   Error, invalid text variable name given (not a...z resp., A...Z).
	 */
	error = ERR_ILLEGAL_VAR_DEFINITION;
      else
	{
	  tvar = *ptr_char++;
	  /*
	     Check if assignment (1)...(3) or operation (4)...(7) is given.
	   */
	  if ((*ptr_char != *RC_VAR_ASSIGN)
	      && (*ptr_char != *RC_TVAR_ICMD_ASSIGN)
	      && (*ptr_char != *RC_TVAR_UCMD_ASSIGN)
	      && (*ptr_char != *RC_VAR_ADD) && (*ptr_char != *RC_VAR_SUB))
	    /*
	       Error, invalid first operator character found
	       (neither '=' nor '+' nor '-' nor '?' nor ':' given).
	     */
	    error = ERR_ILLEGAL_VAR_DEFINITION;
	  else
	    {
	      register int i = 0;
	      register int j;
	      register int len = 0;
	      auto char *ptr_tvar;
	      auto char op;
	      auto char op2 = '\0';
	      auto char op3 = op2;
	      auto Bool is_quoted = FALSE;
	      auto Bool restore_tvar = FALSE;


	      op = *ptr_char++;
	      if (op)
		{
		  op2 = *ptr_char;
		  if (op2)
		    op3 = *(ptr_char + 1);
		}
	      /*
	         Check if the assigned TEXT contains any references
	         to other TVAR variables, if so, insert their TEXT's.
	       */
	      ptr_tvar = strchr (ptr_char, RC_TVAR_CHAR);
	      if (ptr_tvar != (char *) NULL)
		{
		  auto Bool global_tvar_defined;
		  auto Bool local_tvar_set;


		  do
		    {
		      len = (int) (ptr_tvar - ptr_char);
		      if (len)
			{
			  while ((Uint) len + i >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, TRUE,
						__FILE__, (long) __LINE__);
			  strncpy (s5 + i, ptr_char, len);
			  i += len;
			}
		      s5[i] = '\0';
		      if (i)
			if (s5[i - 1] == QUOTE_CHAR)
			  is_quoted = TRUE;
		      ptr_tvar++;
		      if (!is_quoted && isalpha (*ptr_tvar))
			{
			  global_tvar_defined = local_tvar_set = FALSE;
			  if (rc_tvar[IDX (*ptr_tvar)].tvar_global.
			      tvar_text != (char *) NULL)
			    global_tvar_defined = TRUE;
			  if (rc_tvar[IDX (*ptr_tvar)].tvar_local.tvar_text !=
			      (char *) NULL)
			    if (*rc_tvar[IDX (*ptr_tvar)].tvar_local.
				tvar_text)
			      local_tvar_set = TRUE;
			  /*
			     Try to insert the value of this TVAR (that's its TEXT).
			   */
			  j = 0;
			  if (global_tvar_defined
			      && (mode == GLobal
				  || ((mode == LOcal) && !local_tvar_set)))
			    {
			      j =
				(int) strlen (rc_tvar[IDX (*ptr_tvar)].
					      tvar_global.tvar_text);
			      if (j)
				{
				  while ((Uint) i + j >= maxlen_max)
				    resize_all_strings (maxlen_max << 1, TRUE,
							__FILE__,
							(long) __LINE__);
				  strcat (s5,
					  rc_tvar[IDX (*ptr_tvar)].
					  tvar_global.tvar_text);
				}
			    }
			  else if ((mode == LOcal) && local_tvar_set)
			    {
			      j =
				(int) strlen (rc_tvar[IDX (*ptr_tvar)].
					      tvar_local.tvar_text);
			      while ((Uint) i + j >= maxlen_max)
				resize_all_strings (maxlen_max << 1, TRUE,
						    __FILE__,
						    (long) __LINE__);
			      strcat (s5,
				      rc_tvar[IDX (*ptr_tvar)].tvar_local.
				      tvar_text);
			    }
			  if (((mode == GLobal)
			       && global_tvar_defined)
			      || ((mode == LOcal)
				  && (global_tvar_defined
				      || local_tvar_set))
			      || ((tvar == *ptr_tvar)
				  && (((mode == GLobal)
				       && !global_tvar_defined)
				      || ((mode == LOcal)
					  && !global_tvar_defined
					  && !local_tvar_set))))
			    {
			      /*
			         Skip TVAR name.
			       */
			      len += 2;
			      if (j)
				i += j;
			      else
				/*
				   If TVAR is "empty", remove a possibly
				   obsolete whitespace character.
				 */
			      if (i)
				if (isspace (s5[i - 1])
				    && isspace (*(ptr_tvar + 1)))
				  s5[--i] = '\0';
			    }
			  else
			    restore_tvar = TRUE;
			}
		      else
			restore_tvar = TRUE;
		      /*
		         If TVAR isn't defined, or quoted, or an invalid
		         TVAR name is found, don't touch it.
		       */
		      if (restore_tvar)
			{
			  if ((Uint) i + 1 >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, TRUE,
						__FILE__, (long) __LINE__);
			  s5[i++] = RC_TVAR_CHAR;
			  len++;
			  if (*ptr_tvar)
			    {
			      if ((Uint) i + 1 >= maxlen_max)
				resize_all_strings (maxlen_max << 1, TRUE,
						    __FILE__,
						    (long) __LINE__);
			      s5[i++] = *ptr_tvar;
			      len++;
			    }
			  s5[i] = '\0';
			}
		      ptr_char += len;
		      ptr_tvar = strchr (ptr_char, RC_TVAR_CHAR);
		      restore_tvar = is_quoted = FALSE;
		    }
		  while (ptr_tvar != (char *) NULL);
		  /*
		     Add possibly trailing ordinary text.
		   */
		  if (*ptr_char)
		    {
		      i += (int) strlen (ptr_char);
		      while ((Uint) i >= maxlen_max)
			resize_all_strings (maxlen_max << 1, TRUE, __FILE__,
					    (long) __LINE__);
		      strcat (s5, ptr_char);
		    }
		  i++;
		  ptr_char = s5;
		}
	      else
		i = (int) strlen (ptr_char) + 1;
	      if (op == *RC_VAR_ASSIGN
		  || op == *RC_TVAR_ICMD_ASSIGN || op == *RC_TVAR_UCMD_ASSIGN)
		{
		  if (rc_execute_command
		      && (i > 1)
		      && (op == *RC_TVAR_ICMD_ASSIGN
			  || op == *RC_TVAR_UCMD_ASSIGN))
		    {
		      static char *txt2gcal_prgr = (char *) NULL;
		      auto char *ptr_tfn;
		      auto char *the_command;


		      /*
		         Assignment (2)...(3) to text variable found,
		         (':' or '?' given), so perform all necessary actions.
		       */
		      ptr_tfn = TMPFILENAME;
		      if (ptr_tfn == (char *) NULL)
			my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
				  ((long) __LINE__) - 2L, "tmpnam()=", 0);
		      rc_tvar_tfn =
			(char *) my_malloc (strlen (ptr_tfn) + 1,
					    ERR_NO_MEMORY_AVAILABLE, __FILE__,
					    ((long) __LINE__) - 2L,
					    "rc_tvar_tfn", 0);
		      strcpy (rc_tvar_tfn, ptr_tfn);
		      rc_tvar_tfp = fopen (rc_tvar_tfn, "w");
		      if (rc_tvar_tfp == (FILE *) NULL)
			my_error (ERR_WRITE_FILE, __FILE__,
				  ((long) __LINE__) - 2L, rc_tvar_tfn, 0);
		      if (op == *RC_TVAR_ICMD_ASSIGN)
			i += (strlen (REDIRECT_OUT) + strlen (rc_tvar_tfn));
		      else
			{
			  if (txt2gcal_prgr == (char *) NULL)
			    {
			      /*
			         Detect the name of the Txt2gcal executable.
			       */
# if !defined(AMIGA) || defined(__GNUC__)
			      txt2gcal_prgr = getenv (ENV_VAR_TXT2GCALPROG);
			      if (txt2gcal_prgr != (char *) NULL)
				{
				  if (!*txt2gcal_prgr)
				    txt2gcal_prgr = TXT2GCAL_PRGR;
				}
			      else
# endif	/* !AMIGA || __GNUC__ */
				txt2gcal_prgr = TXT2GCAL_PRGR;
			    }
			  i += (strlen (PIPELINE) + strlen (txt2gcal_prgr)
				+ strlen (REDIRECT_OUT) +
				strlen (rc_tvar_tfn));
			}
		      j = i;
		      the_command =
			(char *) my_malloc (i, ERR_NO_MEMORY_AVAILABLE,
					    __FILE__, ((long) __LINE__) - 2L,
					    "rc_tvar_tfn", 0);
		      if (op == *RC_TVAR_ICMD_ASSIGN)
			sprintf (the_command, "%s%s%s", ptr_char,
				 REDIRECT_OUT, rc_tvar_tfn);
		      else
			sprintf (the_command, "%s%s%s%s%s", ptr_char,
				 PIPELINE, txt2gcal_prgr, REDIRECT_OUT,
				 rc_tvar_tfn);
		      /*
		         Execute the command and redirect the STDOUT output
		         of it into TEMPFILE NOW.
		       */
		      i = my_system (the_command);
		      if (warning_level >= 0)
			{
			  while ((Uint) j + LEN_SINGLE_LINE >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, TRUE,
						__FILE__, (long) __LINE__);
			  if (i == -1)
			    {
			      /*
			         Error, `system()' function failed.
			       */
			      sprintf (s5,
				       _
				       ("Cannot execute command in file `%s'\nLine: %ld %s"),
				       filename, line_number, the_command);
			      print_text (stderr, s5);
			      if (warning_level >= WARN_LVL_MAX)
				{
				  j = (int) strlen (the_command);
				  if ((Uint) j >= maxlen_max - 9)
				    resize_all_strings (j + 9, FALSE,
							__FILE__,
							(long) __LINE__);
				  sprintf (s5, "system(%s)=", the_command);
				  my_error (ERR_INTERNAL_C_FUNC_FAILURE,
					    __FILE__, ((long) __LINE__) - 22L,
					    s5, i);
				}
			      error = ERR_INVALID_VAR_ASSIGNMENT;
			    }
			  else
			    {
			      /*
			         Report the exit code of command executed by the `system()' function.
			       */
			      sprintf (s5,
				       _
				       ("Command executed (exit code=%d) in file `%s'\nLine %ld: %s"),
				       i, filename, line_number, the_command);
			      print_text (stderr, s5);
			      /*
			         The command executed by the `system()' function returned
			         a value not equal zero, so we terminate all further
			         processing now with ERR_EXTERNAL_CMD_FAILURE exit status.
			       */
			      if (i && (warning_level >= WARN_LVL_MAX))
				my_exit (ERR_EXTERNAL_CMD_FAILURE);
			    }
			}
		      free (the_command);
		      if (!error)
			{
			  auto long lnumber = 0L;
			  auto int llength;
			  auto int in_pool = 0;
			  static char rc_nl[2] = { RC_NL_CHAR, '\0' };
			  auto char *pool = (char *) NULL;
			  auto char *ptr_pool = (char *) NULL;
			  auto Bool b_dummy;	/* Necessary dummy for `file_read_line()' function. */


			  /*
			     Command executed successfully, we can close the
			     TEMPFILE and re-open it.
			   */
			  if (fclose (rc_tvar_tfp) == EOF)
			    my_error (ERR_WRITE_FILE, __FILE__,
				      ((long) __LINE__) - 1L, rc_tvar_tfn, 0);
			  rc_tvar_tfp = fopen (rc_tvar_tfn, "r");
			  if (rc_tvar_tfp == (FILE *) NULL)
			    my_error (ERR_READ_FILE, __FILE__,
				      ((long) __LINE__) - 2L, rc_tvar_tfn, 0);
			  /*
			     Now process then contents of TEMPFILE according
			     to the selected assignment mode.
			   */
			  pool = (char *) my_malloc (BUF_LEN + 1,
						     ERR_NO_MEMORY_AVAILABLE,
						     __FILE__,
						     ((long) __LINE__) - 2L,
						     "pool", 0);
			  j = 0;
			  *s5 = '\0';
			  while ((ptr_pool =
				  file_read_line (rc_tvar_tfp, &s7, &in_pool,
						  pool, ptr_pool, rc_tvar_tfn,
						  &lnumber, &llength, COmmon,
						  &b_dummy, &b_dummy,
						  &b_dummy)) != (char *) NULL)
			    {
			      if (op == *RC_TVAR_ICMD_ASSIGN)
				{
				  /*
				     Interpret the contents of TEMPFILE.
				   */
				  if ((Uint) j + llength + 2 >= maxlen_max)
				    resize_all_strings (maxlen_max << 1, TRUE,
							__FILE__,
							(long) __LINE__);
				  if (*s7)
				    strcat (s5, s7);
				  strcat (s5, rc_nl);
				  j += (llength + 1);
				}
			      else
				{
				  /*
				     Do not interpret the contents of TEMPFILE,
				     so skip the date-part which was created by
				     the Txt2gcal executable.
				   */
				  i = 0;
				  ptr_char = s7;
				  while (!isspace (*ptr_char))
				    {
				      ptr_char++;
				      i++;
				    }
				  ptr_char++;
				  i = llength - i;
				  break;
				}
			    }
			  free (pool);
			  if (op == *RC_TVAR_ICMD_ASSIGN)
			    {
			      /*
			         Remove the last RC_NL_CHAR of the line.
			       */
			      i = j;
			      s5[i - 1] = '\0';
			      /*
			         Check if the assigned TEXT contains any '\n'
			         newline characters, if so, exchange them
			         by Gcal's RC_NL_CHAR (=='~') characters.
			       */
			      ptr_char = strchr (s5, '\n');
			      if (ptr_char != (char *) NULL)
				do
				  {
				    *ptr_char = RC_NL_CHAR;
				    ptr_char = strchr (s5, '\n');
				  }
				while (ptr_char != (char *) NULL);
			      ptr_char = s5;
			    }
			  /*
			     And do the necessary ending operations.
			   */
			  if (fclose (rc_tvar_tfp) == EOF)
			    my_error (ERR_WRITE_FILE, __FILE__,
				      ((long) __LINE__) - 1L, rc_tvar_tfn, 0);
			  j = unlink (rc_tvar_tfn);
			  if (j)
			    my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
				      ((long) __LINE__) - 2L,
				      "unlink(rc_tvar_tfn)=", j);
			  free (rc_tvar_tfn);
			  rc_tvar_tfn = (char *) NULL;
			}
		    }
		  /*
		     Assignment (1)...(3) to text variable found,
		     so store TEXT into the according TVAR text variable slot.
		   */
		  if (mode == GLobal)
		    {
		      if (rc_tvar[IDX (tvar)].tvar_global.tvar_text ==
			  (char *) NULL)
			rc_tvar[IDX (tvar)].tvar_global.tvar_text =
			  (char *) my_malloc (i, ERR_NO_MEMORY_AVAILABLE,
					      __FILE__,
					      ((long) __LINE__) - 1L,
					      "rc_tvar[IDX(tvar)].tvar_global.tvar_text",
					      IDX (tvar));
		      else
			rc_tvar[IDX (tvar)].tvar_global.tvar_text
			  =
			  (char *)
			  my_realloc ((VOID_PTR)
				      (rc_tvar[IDX (tvar)].tvar_global.
				       tvar_text), i, ERR_NO_MEMORY_AVAILABLE,
				      __FILE__, ((long) __LINE__) - 2L,
				      "rc_tvar[IDX(tvar)].tvar_global.tvar_text",
				      IDX (tvar));
		      strcpy (rc_tvar[IDX (tvar)].tvar_global.tvar_text,
			      ptr_char);
		    }
		  else
		    {
		      /*
		         We have to store the assigned text.
		       */
		      if (rc_tvar[IDX (tvar)].tvar_local.tvar_text ==
			  (char *) NULL)
			rc_tvar[IDX (tvar)].tvar_local.tvar_text =
			  (char *) my_malloc (i, ERR_NO_MEMORY_AVAILABLE,
					      __FILE__,
					      ((long) __LINE__) - 1L,
					      "rc_tvar[IDX(tvar)].tvar_local.tvar_text",
					      IDX (tvar));
		      else
			rc_tvar[IDX (tvar)].tvar_local.tvar_text
			  =
			  (char *)
			  my_realloc ((VOID_PTR)
				      (rc_tvar[IDX (tvar)].tvar_local.
				       tvar_text), i, ERR_NO_MEMORY_AVAILABLE,
				      __FILE__, ((long) __LINE__) - 2L,
				      "rc_tvar[IDX(tvar)].tvar_local.tvar_text",
				      IDX (tvar));
		      strcpy (rc_tvar[IDX (tvar)].tvar_local.tvar_text,
			      ptr_char);
		    }
		}
	      else
		{
		  auto Bool tvar_inc_dec = FALSE;


		  /*
		     Check if operation (4)...(7) is given.
		   */
		  if (op2 == op || op2 == *RC_VAR_ASSIGN)
		    {
		      if (mode == LOcal)
			{
			  if (rc_tvar[IDX (tvar)].tvar_local.tvar_text !=
			      (char *) NULL)
			    {
			      if (*rc_tvar[IDX (tvar)].tvar_local.tvar_text)
				{
				  if (op == op2)
				    {
				      if (op3 && !isspace (op3))
					/*
					   Error, invalid trailing character found.
					 */
					error = ERR_ILLEGAL_VAR_OPERATION;
				      else
					{
					  /*
					     Either TVAR++ or TVAR-- found, so
					     check if TVAR contains an integer value.
					   */
					  ptr_char =
					    rc_tvar[IDX (tvar)].tvar_local.
					    tvar_text;
					  /*
					     Eat one possibly leading sign.
					   */
					  if (*ptr_char == *RC_VAR_ADD
					      || *ptr_char == *RC_VAR_SUB)
					    ptr_char++;
					  while (isdigit (*ptr_char))
					    ptr_char++;
					  if (*ptr_char)
					    /*
					       Error, TVAR contains no integer value.
					     */
					    error = ERR_ILLEGAL_VAR_OPERATION;
					  else
					    tvar_inc_dec = TRUE;
					}
				    }
				  else
				    {
				      /*
				         Either TVAR+=... or TVAR-=... found.
				       */
				      ptr_char++;
				      /*
				         Respect a possibly leading sign of value.
				       */
				      if (*ptr_char == *RC_VAR_ADD
					  || *ptr_char == *RC_VAR_SUB)
					{
					  if (op == *RC_VAR_SUB)
					    {
					      if (*ptr_char == *RC_VAR_ADD)
						op = *RC_VAR_SUB;
					      else
						op = *RC_VAR_ADD;
					    }
					  else
					    op = *ptr_char;
					  ptr_char++;
					}
				    }
				}
			      else
				/*
				   Error, text variable unset.
				 */
				error = ERR_INVALID_VAR_REFERENCE;
			    }
			  else
			    /*
			       Error, text variable undefined.
			     */
			    error = ERR_INVALID_VAR_REFERENCE;
			}
		      else
			/*
			   Error, operation given in global mode.
			 */
			error = ERR_ILLEGAL_VAR_OPERATION;
		    }
		  else
		    /*
		       Error, invalid second operator character found (no '=',
		       '+' or '-' given resp., illegal combination of '+'
		       and '-').
		     */
		    error = ERR_ILLEGAL_VAR_OPERATION;
		  if (!error)
		    {
		      static Slint num;


		      /*
		         Perform the operation and store the calculated value.
		       */
		      if (tvar_inc_dec)
			{
			  len =
			    strlen (rc_tvar[IDX (tvar)].tvar_local.tvar_text);
			  num =
			    atol (rc_tvar[IDX (tvar)].tvar_local.tvar_text);
			  if (op == *RC_VAR_ADD)
			    num++;
			  else
			    num--;
			}
		      else
			{
			  i = 0;
			  while (isdigit (*ptr_char))
			    s5[i++] = *ptr_char++;
			  while (isspace (*ptr_char))
			    ptr_char++;
			  if (i && !*ptr_char)
			    {
			      s5[i] = '\0';
			      len =
				strlen (rc_tvar[IDX (tvar)].tvar_local.
					tvar_text);
			      num =
				atol (rc_tvar[IDX (tvar)].tvar_local.
				      tvar_text);
			      if (op == *RC_VAR_ADD)
				num += atol (s5);
			      else
				num -= atol (s5);
			    }
			  else
			    /*
			       Error, non-numerical value given.
			     */
			    error = ERR_ILLEGAL_VAR_OPERATION;
			}
		      /*
		         Store the calculated value.
		       */
		      if (!error)
			{
			  sprintf (s5, "%0*ld", len, num);
			  len = (int) strlen (s5);
			  if (len !=
			      (int) strlen (rc_tvar[IDX (tvar)].tvar_local.
					    tvar_text))
			    rc_tvar[IDX (tvar)].tvar_local.tvar_text =
			      (char *)
			      my_realloc ((VOID_PTR)
					  (rc_tvar[IDX (tvar)].tvar_local.
					   tvar_text), len + 1,
					  ERR_NO_MEMORY_AVAILABLE, __FILE__,
					  ((long) __LINE__) - 2L,
					  "rc_tvar[IDX(tvar)].tvar_local.tvar_text",
					  IDX (tvar));
			  strcpy (rc_tvar[IDX (tvar)].tvar_local.tvar_text,
				  s5);
			}
		    }
		}
	    }
	}
    }
  if (error)
    {
      if ((mode == GLobal)
	  && (error == ERR_ILLEGAL_VAR_DEFINITION
	      || error == ERR_ILLEGAL_VAR_OPERATION))
	/*
	   These errors always cause termination of program in global mode.
	 */
	warning_level = WARN_LVL_MAX;
      if (warning_level >= 0)
	{
	  if (*line_buffer == RC_TVAR_CHAR)
	    tvar = *(line_buffer + 1);
	  if (!tvar)
	    tvar = *line_buffer;
	  var_warning (error, (int) tvar, line_buffer, filename, line_number);
	}
    }
}



void
nth_weekday_of_month (d, m, y, n, is_weekday_mode)
     int *d;
     int *m;
     int *y;
     const int *n;
     Bool *is_weekday_mode;
/*!
   If "N'th weekday of month" field is encoded:
     Compute the according date and return it in `&d', `&m' and `&y'.
     If a conversion error occurs, return SPECIAL_VALUE in `&y'.
*/
{
  register int i;
  register int j = 0;
  auto int dd = 0;
  auto int mm = 0;
  auto Bool year_set = FALSE;
  auto Bool year_modified = FALSE;


  if (*n
      && (!rc_year_flag
	  || (*m
	      && rc_year_flag))
      && (!rc_period_list || (*m && rc_period_list)))
    {
      if (!*m
	  && (is_3month_mode || is_3month_mode2 || fiscal_month > MONTH_MIN))
	/*
	   If fiscal year resp., 3 month mode and no month encoded, skip evaluation.
	 */
	;
      else
	{
	  *is_weekday_mode = FALSE;
	  if (!*y)
	    {
	      year_set = TRUE;
	      *y = year;
	    }
	  if (!*m)
	    {
	      *m = month;
	      /*
	         A `-c[N]w' or `-ct' option set:
	         Lookahead whether the week ends in the month it started.
	       */
	      if (rc_week_flag || rc_tomorrow_flag)
		{
		  /*
		     <0000|YYYY>00WW[W]N event is in last week of last month of previous year.
		   */
		  if ((*n > 3) && (day < DAY_MIN))
		    {
		      i = (days_of_february (year - 1) == 29);
		      j = day + DAY_LAST + i;
		      (void) doy2date (j, i, &dd, &mm);
		    }
		  else if (*n == 1)
		    {
		      /*
		         <0000|YYYY>00WW[W]N event is in first week of next month of actual year.
		       */
		      if ((day + DAY_MAX - 1 > 0)
			  && (day + DAY_MAX - 1 <
			      DAY_LAST + is_leap_year + 1))
			(void) doy2date (day + DAY_MAX - 1, is_leap_year, &dd,
					 &mm);
		      else
			{
			  /*
			     <0000|YYYY>00WW[W]N event is in first week of first month of next year.
			   */
			  i = (days_of_february (year + 1) == 29);
			  j = (day + DAY_MAX - 1) - (DAY_LAST + is_leap_year);
			  (void) doy2date (j, i, &dd, &mm);
			}
		    }
		  dd = *d;
		}
	    }
	  else if (year_set && (rc_week_flag || rc_tomorrow_flag))
	    {
	      if ((*n == 9)
		  && (*m == MONTH_MAX) && (*y > YEAR_MIN) && (day < DAY_MIN))
		{
		  year_modified = TRUE;
		  (*y)--;
		}
	      else
		if ((*n == 1)
		    && (*m == MONTH_MIN)
		    && (*y < YEAR_MAX)
		    && (day + DAY_MAX >= DAY_LAST + is_leap_year))
		{
		  year_modified = TRUE;
		  (*y)++;
		}
	    }
	  if (year_set
	      && (*y < YEAR_MAX)
	      && ((fiscal_month > MONTH_MIN) && (*m < fiscal_month)))
	    if (!year_modified)
	      (*y)++;
	  if (*m == 2)
	    i = days_of_february (*y);
	  else
	    i = dvec[*m - 1];
	  if (*n == 9)
	    *d = eval_holiday (i, *m, *y, *d, FALSE);
	  else
	    {
	      *d = eval_holiday (DAY_MIN, *m, *y, *d, TRUE);
	      *d += (DAY_MAX * (*n - 1));
	      /*
	         The "N'th weekday of month" doesn't occur in month:
	         Skip it.
	       */
	      if (*d > i)
		*y = SPECIAL_VALUE;
	    }
	  /*
	     A `-c[N]w' or `-ct' option set:
	     Correction for lookahead.
	   */
	  if (mm && (rc_week_flag || rc_tomorrow_flag))
	    {
	      if ((*n == 1) && (mm != *m))
		{
		  *m = mm;
		  if ((day + DAY_MAX - 1 > 0)
		      && (day + DAY_MAX - 1 < DAY_LAST + is_leap_year + 1))
		    /*
		       Void, don't change the year of event.
		     */
		    ;
		  else if (year_set && (year < YEAR_MAX))
		    *y = year + 1;
		  *d = eval_holiday (DAY_MIN, *m, *y, dd, TRUE);
		}
	      else
		if ((*n > 3)
		    && ((adate_set
			 && (mm == *m)) || (!adate_set && (mm != *m))))
		{
		  if (!adate_set)
		    *m = mm;
		  if (year_set && (year > YEAR_MIN))
		    *y = year - 1;
		  if (*n == 9)
		    *d =
		      eval_holiday (dvec[MONTH_MAX - 1], *m, *y, dd, FALSE);
		  else
		    {
		      *d = eval_holiday (DAY_MIN, *m, *y, dd, TRUE);
		      *d += (DAY_MAX * (*n - 1));
		      /*
		         The "N'th weekday of month" doesn't occur in month:
		         Skip it
		       */
		      if (*d > dvec[MONTH_MAX - 1])
			*y = SPECIAL_VALUE;
		    }
		}
	    }
	}
    }
}



Slint
d_between (d1, m1, y1, d2, m2, y2)
     const int d1;
     const int m1;
     const int y1;
     const int d2;
     const int m2;
     const int y2;
/*!
   Computes the amount of days between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to the result.
*/
{
  return (date2num (d2, m2, y2) - date2num (d1, m1, y1));
}



Slint
w_between (d1, m1, y1, d2, m2, y2)
     const int d1;
     const int m1;
     const int y1;
     const int d2;
     const int m2;
     const int y2;
/*!
   Computes the amount of weeks between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to the result.
*/
{
  auto Ulint date1 = date2num (d1, m1, y1);
  auto Ulint date2 = date2num (d2, m2, y2);
  auto Slint diff;
  auto Slint result;


  diff =
    (Slint) date2 - (date1 -
		     (SYEAR (weekday_of_date (d1, m1, y1), start_day)) + 1);
  result = diff / DAY_MAX;
  if ((diff % DAY_MAX) && (diff < 0L))
    result--;

  return (result);
}



Slint
m_between (m1, y1, m2, y2)
     const int m1;
     const int y1;
     const int m2;
     const int y2;
/*!
   Computes the amount of months between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to the result.
*/
{
  return (((y2 - y1) * MONTH_MAX) + (m2 - m1));
}



void
manage_leap_day (day, month, year, line_buffer, filename, line_number)
     int *day;
     int *month;
     int year;
     const char *line_buffer;
     const char *filename;
     const long line_number;
/*!
   Tries to set the leap day (29-Feb) either to "28-Feb" or "1-Mar"
     and prints a informational message in case this date modification is
     performed successfully (only if `--debug[=ARG]' option is given).
*/
{
  register int action = 0;


  if ((*month == 2)
      && (*day == 29) && (rc_feb_29_to_feb_28 || rc_feb_29_to_mar_01))
    {
      if ((fiscal_month > MONTH_MIN + 1) && (year < YEAR_MAX))
	{
	  if (days_of_february (year + 1) == 28)
	    {
	      if (rc_feb_29_to_feb_28)
		*day = action = 28;
	      else
		{
		  *day = action = DAY_MIN;
		  (*month)++;
		}
	    }
	}
      else if (days_of_february (year) == 28)
	{
	  if (rc_feb_29_to_feb_28)
	    *day = action = 28;
	  else
	    {
	      *day = action = DAY_MIN;
	      (*month)++;
	    }
	}
      if ((warning_level >= 0) && action)
	{
	  *s5 = '\0';
	  print_text (stderr, s5);
	  action = (int) strlen (filename) + LEN_SINGLE_LINE;
	  if ((Uint) action >= maxlen_max)
	    resize_all_strings (action + 1, FALSE, __FILE__, (long) __LINE__);
	  sprintf (s5, _("Leap-day set to `%02d-%s' in file `%s'."),
		   *day, month_name (*month), filename);
	  print_text (stderr, s5);
	  sprintf (s5, _("Line %ld: %s"), line_number, line_buffer);
	  print_text (stderr, s5);
	}
    }
}



char *
biorhythm (create_bar, axis_len, string,
	   day, month, year, birth_day, birth_month, birth_year,
	   emo_text, emo_phase, emo_waxes,
	   int_text, int_phase, int_waxes,
	   phy_text, phy_phase, phy_waxes,
	   critical_day, positive_day, negative_day)
     const Bool create_bar;
     int axis_len;
     char *string;
     const int day;
     const int month;
     const int year;
     const int birth_day;
     const int birth_month;
     const int birth_year;
     const char *emo_text;
     int *emo_phase;
     int *emo_waxes;
     const char *int_text;
     int *int_phase;
     int *int_waxes;
     const char *phy_text;
     int *phy_phase;
     int *phy_waxes;
     int *critical_day;
     int *positive_day;
     int *negative_day;
/*!
   Computes the biorhythm for a date and creates a text graphics bar line
     according to the computed values in case `create_bar' is set to TRUE.
     Uses the delivered `string' for storing such a line and returns it.
     The caller has to guarantee that enough `string' space is allocated.
     When used within Gcal, the maximum number of 100 that a single `axis_len'
     may have (100*2+6=>206) fits properly into the string vectors, which
     have 1024 Bytes by default.
*/
{
  auto double x;
  auto Slint diff =
    d_between (birth_day, birth_month, birth_year, day, month, year);
  register int yes_phase;
  register int yes_waxes;
  register int i;


  (*critical_day) = (*positive_day) = (*negative_day) = (*emo_waxes) =
    (*int_waxes) = (*phy_waxes) = 0;
  if (create_bar)
    {
      auto char *ptr_string;


# if 0
      /*
         Decrease `axis_len' by 1 until it divides 100 without a remainder.
       */
      if (axis_len < 1)
	axis_len = -axis_len;
      if (axis_len > 100)
	axis_len = 100;
      else
	while (100 % axis_len)
	  axis_len--;
# endif	/* 0 */
      /*
         Initialize the biorhythm text graphics bar.
       */
      for (i = BIO_AXIS_TOTAL (axis_len), ptr_string = string; --i;)
	*ptr_string++ = ' ';
      string[BIO_AXIS_TOTAL (axis_len) - 1] = '\0';
      string[BIO_AXIS_EXTRA - 1] = *BIO_WANES;
      string[BIO_AXIS_TOTAL (axis_len) - BIO_AXIS_EXTRA - 1] = *BIO_WAXES;
    }
  /*
     Reduce the day difference by multiples of 21252, which is the number
     of days at which each biorhythm cycle restarts from the birthdate.
   */
  if (diff < 1L)
    diff = 21252L - (-diff % 21252L);
  else
    diff %= 21252L;
  /*
     Manage the "emotional" phase value (cycle of 28 days).
   */
  x = MY_TWO_PI * (diff - 1L) / 28.0;
  yes_phase = (int) ROUND (100.0 * sin (x));
  yes_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  x = MY_TWO_PI * diff / 28.0;
  *emo_phase = (int) ROUND (100.0 * sin (x));
  *emo_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  if (*emo_phase == 100)
    (*positive_day)++;
  else if ((yes_waxes == 1) && (yes_phase != 100) && (*emo_waxes <= 0))
    (*positive_day)++;
  if (*emo_phase == -100)
    (*negative_day)++;
  else if ((yes_waxes == -1) && (yes_phase != -100) && (*emo_waxes >= 0))
    (*negative_day)++;
  *emo_waxes = (*emo_waxes >= 0);
  if (((SGN (yes_phase) == 1)
       && (SGN (*emo_phase) <= 0))
      || ((SGN (yes_phase) == -1) && (SGN (*emo_phase) >= 0)))
    (*critical_day)++;
  /*
     Manage the "intellectual" phase value (cycle of 33 days).
   */
  x = MY_TWO_PI * (diff - 1L) / 33.0;
  yes_phase = (int) ROUND (100.0 * sin (x));
  yes_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  x = MY_TWO_PI * diff / 33.0;
  *int_phase = (int) ROUND (100.0 * sin (x));
  *int_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  if (*int_phase == 100)
    (*positive_day)++;
  else if ((yes_waxes == 1) && (yes_phase != 100) && (*int_waxes <= 0))
    (*positive_day)++;
  if (*int_phase == -100)
    (*negative_day)++;
  else if ((yes_waxes == -1) && (yes_phase != -100) && (*int_waxes >= 0))
    (*negative_day)++;
  *int_waxes = (*int_waxes >= 0);
  if (((SGN (yes_phase) == 1)
       && (SGN (*int_phase) <= 0))
      || ((SGN (yes_phase) == -1) && (SGN (*int_phase) >= 0)))
    (*critical_day)++;
  /*
     Manage the "physical" phase value (cycle of 23 days).
   */
  x = MY_TWO_PI * (diff - 1L) / 23.0;
  yes_phase = (int) ROUND (100.0 * sin (x));
  yes_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  x = MY_TWO_PI * diff / 23.0;
  *phy_phase = (int) ROUND (100.0 * sin (x));
  *phy_waxes = SGN ((int) ROUND (100.0 * cos (x)));
  if (*phy_phase == 100)
    (*positive_day)++;
  else if ((yes_waxes == 1) && (yes_phase != 100) && (*phy_waxes <= 0))
    (*positive_day)++;
  if (*phy_phase == -100)
    (*negative_day)++;
  else if ((yes_waxes == -1) && (yes_phase != -100) && (*phy_waxes >= 0))
    (*negative_day)++;
  *phy_waxes = (*phy_waxes >= 0);
  if (((SGN (yes_phase) == 1)
       && (SGN (*phy_phase) <= 0))
      || ((SGN (yes_phase) == -1) && (SGN (*phy_phase) >= 0)))
    (*critical_day)++;
  if (create_bar)
    {
      /*
         Place the "emotional", "intellectual" and "physical"
         marker on the text graphics bar.
       */
      i = *emo_phase / BIO_AXIS_SCALE (axis_len);
      if (SGN (*emo_phase) >= 0)
	i += BIO_AXIS_ZERO (axis_len);
      else
	i = BIO_AXIS_ZERO (axis_len) + i;
      if (string[i] == ' ')
	string[i] = *emo_text;
      else
	string[i] = *BIO_OVERLAPS;
      i = *int_phase / BIO_AXIS_SCALE (axis_len);
      if (SGN (*int_phase) >= 0)
	i += BIO_AXIS_ZERO (axis_len);
      else
	i = BIO_AXIS_ZERO (axis_len) + i;
      if (string[i] == ' ')
	string[i] = *int_text;
      else
	string[i] = *BIO_OVERLAPS;
      i = *phy_phase / BIO_AXIS_SCALE (axis_len);
      if (SGN (*phy_phase) >= 0)
	i += BIO_AXIS_ZERO (axis_len);
      else
	i = BIO_AXIS_ZERO (axis_len) + i;
      if (string[i] == ' ')
	string[i] = *phy_text;
      else
	string[i] = *BIO_OVERLAPS;
      /*
         Place the accumulated "negative", "positive" and "critical"
         day counters on the text graphics bar.
       */
      *string = DIG2CHR (*negative_day);
      string[BIO_AXIS_TOTAL (axis_len) - BIO_AXIS_EXTRA] =
	DIG2CHR (*positive_day);
      string[BIO_AXIS_ZERO (axis_len)] = DIG2CHR (*critical_day);
    }

  return (string);
}



double
compute_distance (coor1, coor2)
     const Coor_struct *coor1;
     const Coor_struct *coor2;
/*!
   Returns the air line distance in Kilometers between the two geographical
     point locations which are delivered in the COOR1 and COOR2 structures
     if the member variable `the_mode' is set to zero.
     If `the_mode' is set to 1, the course/direction angle in degrees from
     COOR1 to COOR2 is returned (or SPECIAL_VALUE if an error occurs).
     If `the_mode' is set to 2, the course/direction angle in degrees from
     COOR2 to COOR1 is returned (or SPECIAL_VALUE if an error occurs).
     The course/direction angle is that angle, which one needs to go from
     the geographical point location given in one COOR? structure to the
     geographical point location given in the other COOR? structure.
     The angle values in degrees are:
       * North :=   0.0 <= angle < 90.0
       * East  :=  90.0 <= angle < 180.0
       * South := 180.0 <= angle < 270.0
       * West  := 270.0 <= angle < 0.0
     The longitude coordinates west of the zero meridian have a positive sign.
     The longitude coordinates east of the zero meridian have a negative sign.
     The latitude coordinates north of the equator have a positive sign.
     The latitude coordinates south of the equator have a negative sign.
     For negative numbers, all three of `*_deg', `*_min' and `*_sec' should
     be negative.  For example, the ISO 6709 coordinate `-202233+1100010'
     (==+|-Latitude+|-Longitude) must be defined as `-20 -22 -33 -110 0 -10'.
   The spheric trigonometric formula used to calculate the distance is:
     R := 6371.221, the *mean* Earth radius
     phi1 := Latitude of COOR1
     phi2 := Latitude of COOR2
     delta_lambda := Longitude of COOR1 - Longitude of COOR2
     g := arc cosine (sine phi1 * sine phi2 + cosine phi1 * cosine phi2 * cosine delta_lambda)
     => distance_in_km := 2 * Pi * R * degree(g) / 360
     *** Gcal respects the flattening of the Earth in that it uses the true
     *** Earth radii of the given locations instead of the mean Earth radius,
     *** and their geocentric latitudes instead of their geodetic latitude!
   The spheric trigonometric formula used to calculate the direction angle
   for `the_mode==1' is:
     y := sine delta_lambda
     x := cosine phi1 * tangent phi2 - sine phi1 * cosine delta_lambda
     => (1) direction_angle_in_degrees := degree(arc tangent (y / x))
     An alternative formula is:
     => (2) direction_angle_in_degrees := degree(arc sine (cos phi2 * sine delta_lambda / sine g))
     Both formulaes do not move the direction angle into the correct quadrant
     immediately.  For (1), this can be done by using the mathlib function
     `atan2()' instead of the mathlib function `atan()'.
*/
{
  auto double lon_c1 = TORAD (coor1->lon_deg
			      + MM2DEG (coor1->lon_min)
			      + SS2DEG (coor1->lon_sec));
  auto double lat_c1 = TORAD (coor1->lat_deg
			      + MM2DEG (coor1->lat_min)
			      + SS2DEG (coor1->lat_sec));
  auto double lon_c2 = TORAD (coor2->lon_deg
			      + MM2DEG (coor2->lon_min)
			      + SS2DEG (coor2->lon_sec));
  auto double lat_c2 = TORAD (coor2->lat_deg
			      + MM2DEG (coor2->lat_min)
			      + SS2DEG (coor2->lat_sec));
  auto double delta_lambda;
  auto double x1;
  auto double x2;


  switch (coor1->the_mode)
    {
    case 0:
      x1 =
	gd_latitude2gc_latitude (lat_c1, coor1->meters_above_sea_level,
				 &lat_c1);
      x2 =
	gd_latitude2gc_latitude (lat_c2, coor2->meters_above_sea_level,
				 &lat_c2);
      if (SGN (lat_c1) == 0 || SGN (lat_c2) == 0
	  || SGN (lat_c1) == SGN (lat_c2))
	{
	  delta_lambda = (x1 + x2) * 0.5;
	  x1 = 2.0 * MAX (x1, delta_lambda) + MIN (x1, delta_lambda)
	    + 2.0 * MAX (x2, delta_lambda) + MIN (x2, delta_lambda);
	}
      else
	x1 = 4.0 * EQUATOR_EARTH_RADIUS + x1 + x2;
      delta_lambda = lon_c1 - lon_c2;
      return (DEG2DAY (TODEG (acos (sin (lat_c1) * sin (lat_c2)
				    +
				    cos (lat_c1) * cos (lat_c2) *
				    cos (delta_lambda)))) * (x1 / 6000.0) *
	      MY_TWO_PI);
    case 1:
      delta_lambda = lon_c1 - lon_c2;
      x1 = cos (lat_c1) * tan (lat_c2) - sin (lat_c1) * cos (delta_lambda);
      break;
    case 2:
      delta_lambda = lon_c2 - lon_c1;
      x1 = cos (lat_c2) * tan (lat_c1) - sin (lat_c2) * cos (delta_lambda);
      break;
    default:
      /*
         This case MUST be an internal error!
       */
      abort ();
    }
  x2 = sin (delta_lambda);
  /*
     Emulate the mathlib function `atan2()' so we can handle an error properly.
   */
  if (x1 > 0.0)
    delta_lambda = atan (x2 / x1);
  else if (x1 < 0.0)
    delta_lambda = atan (x2 / x1) + MY_PI;
  else if (x2 > 0.0)
    delta_lambda = MY_HALF_PI;
  else if (x2 < 0.0)
    delta_lambda = -MY_HALF_PI;
  else
    /*
       This case (x2==0 && x1==0) is treated as an error here
       and is managed specially!
     */
    return (SPECIAL_VALUE);
  if (SGN (delta_lambda) > 0)
    delta_lambda = MY_TWO_PI - delta_lambda;
  else if (SGN (delta_lambda) < 0)
    delta_lambda = -delta_lambda;

  return (TODEG (delta_lambda));
}



static void
var_warning (exit_status, var_name, line_buffer, filename, line_number)
     const int exit_status;
     const int var_name;
     const char *line_buffer;
     const char *filename;
     const long line_number;
/*!
   Prints an informational message on STDERR channel in case an operation
     on a date or text variable is invalid.  Terminates the program if
     `warning_level' is set to "WARN_LVL_MAX"  with delivered `exit_status'.
*/
{
  register int i;
  auto Bool with_usage = FALSE;


  if (!line_number)
    S_NEWLINE (stderr);
  else
    {
      *s5 = '\0';
      print_text (stderr, s5);
    }
  if (warning_level >= WARN_LVL_MAX)
    fprintf (stderr, _("%s: abort, "), prgr_name);
  i = (int) strlen (filename) + LEN_SINGLE_LINE;
  if ((Uint) i >= maxlen_max)
    resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
  switch (exit_status)
    {
    case ERR_ILLEGAL_VAR_DEFINITION:
      if (line_number)
	sprintf (s5, _("illegal variable definition in file `%s'"), filename);
      else
	sprintf (s5, _("illegal definition of variable `%c'"),
		 (char) var_name);
      if (!line_number)
	with_usage = TRUE;
      break;
    case ERR_ILLEGAL_VAR_OPERATION:
      if (line_number)
	sprintf (s5, _("illegal variable operation in file `%s'"), filename);
      else
	sprintf (s5, _("illegal operation on variable `%c'"),
		 (char) var_name);
      if (!line_number)
	with_usage = TRUE;
      break;
    case ERR_INVALID_VAR_REFERENCE:
      sprintf (s5, _("variable `%c' undefined in file `%s'"),
	       (char) var_name, filename);
      break;
    case ERR_INVALID_VAR_ASSIGNMENT:
      sprintf (s5, _("invalid value assigned to variable `%c' in file `%s'"),
	       (char) var_name, filename);
      break;
    default:
      /*
         This case MUST be an internal error!
       */
      abort ();
    }
  if (warning_level < WARN_LVL_MAX)
    {
      *s5 = (char) toupper (*s5);
      strcat (s5, ".");
    }
  if (!line_number)
    fprintf (stderr, "%s\n", s5);
  else
    print_text (stderr, s5);
  if (warning_level >= WARN_LVL_MAX)
    {
      if (!line_number)
	fprintf (stderr, _("Invalid argument in command line given -- %s"),
		 line_buffer);
      else
	fprintf (stderr, _("Line %ld: %s"), line_number, line_buffer);
      S_NEWLINE (stderr);
    }
  else
    {
      i = (int) strlen (line_buffer) + LEN_SINGLE_LINE;
      if ((Uint) i >= maxlen_max)
	resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
      if (!line_number)
	sprintf (s5, _("Argument `%s' of command line ignored."),
		 line_buffer);
      else
	sprintf (s5, _("Line %ld ignored: %s"), line_number, line_buffer);
      if (!line_number)
	fprintf (stderr, "%s\n", s5);
      else
	print_text (stderr, s5);
    }
  if (with_usage)
    fprintf (stderr, "%s\n%s\n", usage_msg (), lopt_msg ());
  if (warning_level >= WARN_LVL_MAX)
    my_exit (exit_status);
}
#endif /* USE_RC */
