/*!
*  \file print.c
*  \brief Create, construct and print the calendar sheet(s).
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
*  Include header files.
*/
#include "tailor.h"
#include "common.h"
#if USE_RC
# include "rc-defs.h"
#endif /* USE_RC */
#include "globals.h"
#include "hd-defs.h"
#include "hd-use.h"
#if USE_RC
# include "rc-use.h"
#endif /* USE_RC */
#include "tty.h"
#include "utils.h"
#include "print.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `print.c'.
*/
static void fill_year_vector __P_ ((int year));
static Bool print_unhighlighted_date __P_ ((Bool marker_flag));
static Bool
  print_highlighted_date __P_ ((const Bool last_item,
				const Bool marker_flag,
				const char *hls_start,
				const int hls_slen,
				const char *hls_end, const int hls_elen));
__END_DECLARATIONS
/*
*  static variables definitions.
*/
/*! Standard or special year dates. */
static int year_vector[VEC_ELEMS];

/*! Special year dates only. */
static int special_vector[VEC_ELEMS];



/*
*  Function implementations.
*/
void
print_calendar ()
/*!
   Prints one or more single month/year calendar(s).
*/
{
  auto Slint amount = 0L;
  auto Slint count;
  register int tmp_ad = act_day;
  register int i;
  register int j;
  register int n;
  register int lym;
  register int d;
  register int m;
  register int mm;
  register int yy;
  register int outer_end = (special_calsheet_flag) ? MONTH_COLS : DAY_MAX;
  register int inner_end;
  register int blanks_between = (special_calsheet_flag) ? 5 : 2;
  register int hday;
  auto int hmonth;
  auto int this_day;
  auto int this_month;
  auto Bool is_marked = FALSE;
  auto Bool marker_flag = FALSE;
  auto Bool y_backwards = FALSE;
  auto Bool m_backwards = FALSE;


  if (cal_special_flag)
    act_day = day_of_year (tmp_ad, act_month, act_year);
  /*
     Evaluate the entries of `month_list':
     Set the actual list/range/fiscal year mode and
     initialize the controlling variables of the main loop.
   */
  if (is_special_range || is_multi_range)
    {
      y_backwards = (Bool) ((*month_list).ml_year > month_list[1].ml_year);
      if (y_backwards)
	amount = (*month_list).ml_year - month_list[1].ml_year + 1L;
      else
	amount = month_list[1].ml_year - (*month_list).ml_year + 1L;
      if (is_multi_range)
	{
	  m_backwards =
	    (Bool) ((*month_list).ml_month > month_list[1].ml_month);
	  if (m_backwards)
	    i = (*month_list).ml_month - month_list[1].ml_month + 1;
	  else
	    i = month_list[1].ml_month - (*month_list).ml_month + 1;
	  amount *= (Slint) i;

	}
      month = (*month_list).ml_month;
      year = (*month_list).ml_year;
    }
  else if (!is_ext_range)
    for (i = 0; month_list[i].ml_month; i++)
      {
	if (!month_list[i].ml_year)
	  {
	    if (year != act_year)
	      month_list[i].ml_year = year;
	    else
	      month_list[i].ml_year = act_year;
	  }
	amount++;
      }
  else
    {
      if (!is_ext_year)
	{
	  if (!(*month_list).ml_year)
	    (*month_list).ml_year = act_year;
	  if (!month_list[1].ml_year)
	    month_list[1].ml_year = act_year;
	  y_backwards = (Bool) ((*month_list).ml_year > month_list[1].ml_year
				||
				(((*month_list).ml_year ==
				  month_list[1].ml_year)
				 && ((*month_list).ml_month >
				     month_list[1].ml_month)));
	  if (y_backwards)
	    amount =
	      ((((*month_list).ml_year - 1L) -
		month_list[1].ml_year) * MONTH_MAX) + (*month_list).ml_month +
	      ((MONTH_MAX - month_list[1].ml_month) + 1L);
	  else
	    amount =
	      (((month_list[1].ml_year - 1L) -
		(*month_list).ml_year) * MONTH_MAX) + month_list[1].ml_month +
	      ((MONTH_MAX - (*month_list).ml_month) + 1L);
	  month = (*month_list).ml_month;
	  year = (*month_list).ml_year;
	}
      else
	{
	  if (is_fiscal_year)
	    {
	      y_backwards =
		(Bool) ((*month_list).ml_year > month_list[1].ml_year);
	      if (y_backwards)
		amount = ((*month_list).ml_year - month_list[1].ml_year) + 1L;
	      else
		amount = (month_list[1].ml_year - (*month_list).ml_year) + 1L;
	      year = (*month_list).ml_year;
	    }
	  else
	    {
	      y_backwards =
		(Bool) ((*month_list).ml_month > month_list[1].ml_month);
	      if (y_backwards)
		amount =
		  ((*month_list).ml_month - month_list[1].ml_month) + 1L;
	      else
		amount =
		  (month_list[1].ml_month - (*month_list).ml_month) + 1L;
	      year = (*month_list).ml_month;
	    }
	}
    }
  if (is_fiscal_year)
    fiscal_month = (*month_list).ml_month;
  if (!fiscal_month)
    fiscal_month = act_month;
  if (!amount)
    amount++;
#if USE_RC
  /*
     A `-cNw' option and no explicit date given:
     Set the correct month/year to display.
   */
  if (rc_week_year_flag && !rc_period_list && !is_date_given)
    {
      i = weekno2doy (rc_period, act_year, iso_week_number, start_day);
      if (i != -WEEK_MAX)
	{
	  if (i < DAY_MIN || i + DAY_MAX - 1 > DAY_LAST + is_leap_year)
	    {
	      is_2month_mode = is_fiscal_year = TRUE;
	      (*month_list).ml_month = fiscal_month = MONTH_MAX;
	      if (i < DAY_MIN)
		(*month_list).ml_year = --year;
	    }
	  else
	    {
	      (void) doy2date (i, is_leap_year, &day, &month);
	      m = month;
	      (void) doy2date (i + DAY_MAX - 1, is_leap_year, &day, &month);
	      if (m != month)
		{
		  is_2month_mode = is_fiscal_year = TRUE;
		  month = fiscal_month = m;
		}
	      else
		is_1month_mode = TRUE;
	      (*month_list).ml_month = month;
	    }
	  if (is_2month_mode)
	    {
	      if (cal_both_dates_flag)
		{
		  out_rows = B2_OUT_ROWS;
		  out_cols = B2_OUT_COLS;
		}
	      else
		{
		  out_rows = A2_OUT_ROWS;
		  out_cols = A2_OUT_COLS;
		}
	    }
	}
    }
#endif
  /*
     All necessary global initializations done, so let's enter the loop.
   */
  for (count = 0L; count < amount; count++)
    {
      /*
         If the loop must be processed multiple, we have to
         re-initialize the affected variables according to actual mode.
       */
      mm = yy = 0;
      if (!is_ext_list
	  && !is_ext_range && !is_special_range && !is_multi_range)
	{
	  /*
	     If a month calendar of only the current year is wanted
	     resp., a year calendar of only the current year is wanted:
	     Initialize the touched variables.
	   */
	  month = month_list[(int) count].ml_month;
	  if (month_list[(int) count].ml_year)
	    year = month_list[(int) count].ml_year;
	  if (count)
	    yy = year;
#if USE_RC
	  /*
	     Modify the actual date.
	   */
	  if (rc_period_flag
	      && (rc_forwards_flag
		  || rc_backwards_flag) && !rc_period_list && !is_date_given)
	    {
	      if (cal_special_flag)
		i = act_day;
	      else
		i = day_of_year (tmp_ad, act_month, act_year);
	      if (rc_forwards_flag)
		{
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if ((act_year == greg->year)
		      && (i <
			  day_of_year (greg->first_day, greg->month,
				       greg->year))
		      && (i + rc_period >=
			  day_of_year (greg->first_day, greg->month,
				       greg->year)))
		    i += (greg->last_day - greg->first_day + 1);
		  i += rc_period;
		}
	      else
		{
		  /*
		     Correction in case date occurs during Gregorian Reformation period.
		   */
		  if ((act_year == greg->year)
		      && (i >
			  day_of_year (greg->last_day, greg->month,
				       greg->year))
		      && (i - rc_period <=
			  day_of_year (greg->last_day, greg->month,
				       greg->year)))
		    i -= (greg->last_day - greg->first_day + 1);
		  i -= rc_period;
		}
	      if ((i > 0) && (i < DAY_LAST + is_leap_year + 1))
		{
		  (void) doy2date (i, is_leap_year, &day, &month);
		  act_month = month;
		  if (cal_special_flag)
		    act_day = day_of_year (day, month, act_year);
		  else
		    act_day = day;
		}
	    }
#endif /* USE_RC */
	}
      else if (is_ext_list)
	{
	  if (!is_ext_year)
	    {
	      month = month_list[(int) count].ml_month;
	      if (count && (year == month_list[(int) count].ml_year))
		yy = year;
	      else
		year = month_list[(int) count].ml_year;
	    }
	  else
	    {
	      month = 0;
	      if (is_fiscal_year)
		{
		  if (count
		      && (fiscal_month == month_list[(int) count].ml_month)
		      && (year == month_list[(int) count].ml_year))
		    {
		      mm = fiscal_month;
		      yy = year;
		    }
		  else
		    {
		      fiscal_month = month_list[(int) count].ml_month;
		      year = month_list[(int) count].ml_year;
		    }
		}
	      else
		{
		  if (count && (year == month_list[(int) count].ml_month))
		    yy = year;
		  else
		    year = month_list[(int) count].ml_month;
		}
	    }
	}
      else if ((is_ext_range || is_special_range || is_multi_range) && count)
	{
	  if (!is_special_range && !is_multi_range && !is_ext_year)
	    {
	      yy = year;
	      if (y_backwards)
		{
		  month--;
		  if (month < MONTH_MIN)
		    month = MONTH_MAX, year--;
		}
	      else
		{
		  month++;
		  if (month > MONTH_MAX)
		    month = MONTH_MIN, year++;
		}
	    }
	  else
	    {
	      if (!is_special_range && !is_multi_range)
		month = 0;
	      if (is_multi_range)
		{
		  if (m_backwards)
		    {
		      month--;
		      if (month < month_list[1].ml_month)
			{
			  month = (*month_list).ml_month;
			  if (y_backwards)
			    year--;
			  else
			    year++;
			}
		      else
			yy = year;
		    }
		  else
		    {
		      month++;
		      if (month > month_list[1].ml_month)
			{
			  month = (*month_list).ml_month;
			  if (y_backwards)
			    year--;
			  else
			    year++;
			}
		      else
			yy = year;
		    }
		}
	      else if (y_backwards)
		year--;
	      else
		year++;
	    }
	}
      else if (!is_special_range && !is_multi_range && is_ext_year)
	month = 0;
      if (yy != year || (is_fiscal_year && (mm != fiscal_month)))
	{
	  is_leap_year = (days_of_february (year) == 29);
	  if (count)
	    for (i = 0; i < VEC_ELEMS; i++)
	      year_vector[i] = special_vector[i] = 0;
	  /*
	     Build the year calendar data structure.
	   */
	  fill_year_vector (year);
	  /*
	     Get the dates of the eternal holidays;
	     used for highlighting the calendar.
	   */
	  if (((year == EASTER_MIN - 1)
	       && (fiscal_month > MONTH_MIN))
	      || ((year >= EASTER_MIN) && (year <= EASTER_MAX)))
	    print_all_holidays (TRUE, TRUE);
	  else if (count)
	    for (i = 0; i < HD_MAX; i++)
	      hd_ldays[i] = '\0';
	}
      else
	/*
	   Get the actual dates of holidays; used for highlighting the calendar.
	 */
      if (((year == EASTER_MIN - 1)
	     && (fiscal_month > MONTH_MIN))
	    || ((year >= EASTER_MIN) && (year <= EASTER_MAX)))
	print_all_holidays (TRUE, TRUE);
      *s1 = '\0';
      /*
         All necessary initializations done.
         So print a month calendar / year calendar of the required year.
       */
      if (!is_ext_year && !is_fiscal_year
#if USE_RC
	  && !is_2month_mode
#endif
	  && (month
	      || is_ext_list
	      || is_ext_range || is_special_range || is_multi_range))
	{
	  /*
	     If no explicit request for suppressing the calendar sheet is given
	     in the command line (`-u' option), create it!  (What else is the
	     expense in this source file good for?)
	   */
	  if (!suppr_cal_flag)
	    {
	      lym = len_year_max;
	      if (transform_year)
		{
		  n = year - transform_year;
		  if ((n >= 0) && (transform_year > 0))
		    n++;
		  if (abs (n) > YEAR_MAX)
		    lym++;
		}
	      else
		n = year;
	      /*
	         Print the month calendar sheet and if needed the fixed dates,
	         which are related to the month and the eternal holidays
	         related to the month.
	       */
	      print_text (stdout, s1);
	      if (special_calsheet_flag)
		{
		  /*
		     Print the month calendar in the special format:
		     Initialize the terminating value of the inner loop.
		   */
		  inner_end = DAY_MAX;
		  /*
		     Print the month header in centered manner.
		   */
		  if (use_year_zeroleaded)
		    {
		      i = ((format_len * DAY_MAX) >> 1)
			+ ((strlen (month_name (month)) + lym + 1) >> 1) -
			lym;
		      sprintf (s1, "%*s %0*d", i, month_name (month), lym, n);
		    }
		  else
		    {
		      sprintf (s1, "%d", n);
		      d = (int) strlen (s1);
		      i = ((format_len * DAY_MAX) >> 1)
			+ ((strlen (month_name (month)) + d + 1) >> 1) - d;
		      sprintf (s1, "%*s %d", i, month_name (month), n);
		    }
		  print_text (stdout, s1);
		  /*
		     Print the day names.
		   */
		  for (i = DAY_MIN; i <= DAY_MAX; i++)
		    {
		      if (use_short3_day_name
			  && (format_len > FORMAT_LEN_MIN))
			sprintf (s2, "%*s", format_len,
				 short3_day_name (SDAY (i, start_day)));
		      else
			sprintf (s2, "%*s", format_len,
				 short_day_name (SDAY (i, start_day)));
		      strcat (s1, s2);
		    }
		  if (cal_with_week_number)
		    {
		      /*
		         The short week text which should be a proper abbreviation
		         of "calendar week", consists of 2 letters and is
		         separated by one space character of the month calendar.
		       */
		      /*
		       *** Translators, please translate this as a fixed 2-character text.
		       *** This text should be a proper abbreviation of "Calendar Week".
		       */
		      sprintf (s2, " %2s", _("CW"));
		      strcat (s1, s2);
		    }
		}
	      else
		{
		  /*
		     Print the month calendar in the standard format:
		     Initialize the terminating value of the inner loop.
		   */
		  inner_end = MONTH_COLS;
		  /*
		     Print the month header.
		   */
		  if (use_year_zeroleaded)
		    sprintf (s1, "%s %0*d", month_name (month), lym, n);
		  else
		    sprintf (s1, "%s %d", month_name (month), n);
		  print_text (stdout, s1);
		  if (cal_with_week_number)
		    sprintf (s3, "%-*s", len_dayname_max + blanks_between,
			     _("Week"));
		}
	      print_text (stdout, s1);
	      /*
	         Print the days.
	       */
	      for (i = 1; i <= outer_end; i++)
		{
		  if (!special_calsheet_flag)
		    /*
		       Print the day names.
		     */
		    sprintf (s1, "%-*s", len_dayname_max + blanks_between,
			     day_name (SDAY (i, start_day)));
		  for (d = 1; d <= inner_end; d++)
		    {
		      /*
		         Compute the days position in `year_vector'.
		       */
		      if (special_calsheet_flag)
			day = (month - 1) * VEC_BLOCK - 1
			  + (i - 1) * DAY_MAX + d;
		      else
			day = (month - 1) * VEC_BLOCK - 1
			  + d * DAY_MAX - MONTH_COLS + i - 1;
		      /*
		         Is `day' the actual day?
		       */
		      if (highlight_flag
			  && (year_vector[day] == act_day)
			  && (month == act_month) && (year == act_year))
			{
			  if (is_marked && (ehls1s.len == 1))
			    marker_flag =
			      print_highlighted_date (d == inner_end,
						      is_marked, "", 0,
						      ehls1e.seq, ehls1e.len);
			  else
			    is_marked = marker_flag =
			      print_highlighted_date (d == inner_end,
						      is_marked, ehls1s.seq,
						      ehls1s.len, ehls1e.seq,
						      ehls1e.len);
			}
		      else
			{
			  /*
			     Is `day' a legal holiday?
			   */
			  j = 0;
			  if (year_vector[day])
			    {
			      if (cal_special_flag)
				(void) doy2date (year_vector[day],
						 is_leap_year, &this_day,
						 &month);
			      else
				this_day = year_vector[day];
			      if (hd_ldays
				  [((month - 1) * MONTH_LAST) +
				   (this_day - 1)])
				j = 1;
			    }
			  if (j && highlight_flag)
			    {
			      /*
			         `day' is a legal holiday!
			       */
			      if (is_marked && (ehls2s.len == 1))
				marker_flag =
				  print_highlighted_date (d == inner_end,
							  is_marked, "", 0,
							  ehls2e.seq,
							  ehls2e.len);
			      else
				is_marked = marker_flag =
				  print_highlighted_date (d == inner_end,
							  is_marked,
							  ehls2s.seq,
							  ehls2s.len,
							  ehls2e.seq,
							  ehls2e.len);
			    }
			  else
			    {
			      /*
			         `day' is no holiday!
			       */
			      marker_flag =
				print_unhighlighted_date (marker_flag);
			      is_marked = FALSE;
			    }
			}
		      if (cal_with_week_number
			  && ((special_calsheet_flag
			       && (d == inner_end))
			      || (!special_calsheet_flag && (i == 1))))
			{
			  j = SPECIAL_VALUE;
			  if (special_calsheet_flag)
			    {
			      j = 0;
			      day -= (DAY_MAX - 1);
			      if (day >= 0)
				{
				  while ((j < DAY_MAX)
					 && (day + j < VEC_ELEMS)
					 && !year_vector[day + j])
				    j++;
				  if ((j != DAY_MAX) && (day + j < VEC_ELEMS))
				    {
				      if (cal_special_flag)
					{
					  (void)
					    doy2date (year_vector[day + j],
						      is_leap_year, &day,
						      &month);
					  j =
					    week_number (day, month, year,
							 iso_week_number,
							 start_day);
					}
				      else
					j =
					  week_number (year_vector[day + j],
						       month, year,
						       iso_week_number,
						       start_day);
				    }
				  else
				    j = SPECIAL_VALUE;
				}
			      else
				j = SPECIAL_VALUE;
			    }
			  else
			    {
			      j = 0;
			      while ((j < DAY_MAX)
				     && (day + j < VEC_ELEMS)
				     && !year_vector[day + j])
				j++;
			      if ((j != DAY_MAX) && (day + j < VEC_ELEMS))
				{
				  if (cal_special_flag)
				    {
				      (void) doy2date (year_vector[day + j],
						       is_leap_year, &day,
						       &month);
				      j =
					week_number (day, month, year,
						     iso_week_number,
						     start_day);
				    }
				  else
				    j =
				      week_number (year_vector[day + j],
						   month, year,
						   iso_week_number,
						   start_day);
				}
			      else
				j = SPECIAL_VALUE;
			    }
			  if (j != SPECIAL_VALUE)
			    {
			      /*
			         We convert the computed week number to a week number text
			         (this looks nicer in output).
			       */
			      if (j < 0)
				/*
				   Week starts in previous year and the first days
				   of the actual year are not in its first week.
				 */
				sprintf (s2, "%02d/0", -j);
			      else if (!j)
				/*
				   Week starts in previous year and the first days
				   of the actual year are in its first week.
				 */
				sprintf (s2, "%s", "53/1");
			      else
				/*
				   Week starts in actual year.
				 */
				sprintf (s2, "%02d", j);
			      if (!special_calsheet_flag
				  && (strlen (s2) > 2) && (d == 1))
				{
				  j = (int) strlen (s3);
				  s3[j - 2] = '\0';
				}
			    }
			  else
			    if (special_calsheet_flag
				|| (!special_calsheet_flag && (i == 1)))
			    strcpy (s2, "  ");
			  if (special_calsheet_flag)
			    {
			      if (!is_marked
				  || (is_marked
				      && ((ehls1s.len > 1)
					  || (ehls2s.len > 1))))
				strcat (s1, " ");
			      strcat (s1, s2);
			    }
			  else
			    {
			      sprintf (s4, "%*s", format_len - 2, "");
			      strcat (s3, s4);
			      strcat (s3, s2);
			    }
			}
		    }
		  /*
		     Print the constructed line.
		   */
		  print_text (stdout, s1);
		  is_marked = marker_flag = FALSE;
		}
	      if (cal_with_week_number && !special_calsheet_flag)
		{
		  print_text (stdout, s1);
		  print_text (stdout, s3);
		}
	    }
#if USE_RC
	  /*
	     Print the fixed dates list.
	   */
	  if (rc_use_flag
	      && (is_ext_list
		  || is_ext_range
		  || is_special_range || is_multi_range || amount > 1L))
	    rc_use ();
#endif
	  /*
	     Print the eternal holiday list.
	   */
	  if (holiday_flag
	      && (is_ext_list
		  || is_ext_range
		  || is_special_range
		  || is_multi_range
		  || amount > 1L)
	      && (((year == EASTER_MIN - 1)
		   && (fiscal_month > MONTH_MIN))
		  || ((year >= EASTER_MIN) && (year <= EASTER_MAX))))
	    print_all_holidays (FALSE, FALSE);
	}
      else
	{
	  /*
	     If no explicit request for suppressing the calendar sheet
	     is given in command line (`-u' option), create it!
	   */
	  if (!suppr_cal_flag)
	    {
	      register int n2;


	      lym = len_year_max;
	      if (transform_year)
		{
		  n = year - transform_year;
		  if ((n >= 0) && (transform_year > 0))
		    n++;
		  n2 = (year - transform_year) + 1;
		  if ((n2 >= 0) && (transform_year > 0))
		    n2++;
		  if (abs (n) > YEAR_MAX || abs (n2) > YEAR_MAX)
		    lym++;
		}
	      else
		{
		  n = year;
		  n2 = year + 1;
		}
	      /*
	         Print the year calendar sheet and if needed:
	         The fixed dates related to the year and
	         the eternal holidays related to the year.
	       */
	      print_text (stdout, s1);
	      print_text (stdout, s1);
	      if (special_calsheet_flag)
		{
		  /*
		     Print the year calendar in the special format:
		     Initialize the terminating value of the inner loop.
		   */
		  inner_end = DAY_MAX * out_cols;
		  /*
		     Compute the position of the year number.
		   */
		  i =
		    ((out_cols - 1) * blanks_between +
		     out_cols * format_len * DAY_MAX) >> 1;
		}
	      else
		{
		  /*
		     Print the year calendar in the standard format:
		     Initialize the terminating value of the inner loop.
		   */
		  inner_end = MONTH_COLS * out_cols;
		  /*
		     Compute the position of the year number.
		   */
		  i =
		    (out_cols * format_len * MONTH_COLS + 2 +
		     blanks_between) >> 1;
		}
	      /*
	         Print the year header.
	       */
	      if ((!is_3month_mode
#if USE_RC
		   && !is_2month_mode
#endif
		   && !is_3month_mode2
		   && (fiscal_month > MONTH_MIN) && (year + 1 <= YEAR_MAX))
#if USE_RC
		  || (is_2month_mode && (fiscal_month == MONTH_MAX))
#endif
		  || (is_3month_mode && (fiscal_month >= MONTH_MAX - 1)))
		{
		  if (use_year_zeroleaded)
		    sprintf (s1, "%*s%0*d/%0*d", i - lym, "", lym, n, lym,
			     n2);
		  else
		    {
		      sprintf (s1, "%d", n);
		      d = (int) strlen (s1);
		      sprintf (s1, "%d", n2);
		      d += (int) strlen (s1);
		      sprintf (s1, "%*s%d/%d", i - (d >> 1), "", n, n2);
		    }
		}
	      else
		{
		  if (use_year_zeroleaded)
		    sprintf (s1, "%*s%0*d", i - (lym >> 1), "", lym, n);
		  else
		    {
		      sprintf (s1, "%d", n);
		      d = (int) strlen (s1);
		      sprintf (s1, "%*s%d", i - (d >> 1), "", n);
		    }
		}
	      print_text (stdout, s1);
	      print_text (stdout, s1);
	      print_text (stdout, s1);
	      for (m = 0; m < out_rows; m++)
		{
		  if (special_calsheet_flag)
		    {
		      /*
		         Print the month header in centered manner.
		       */
		      for (i = 1; i <= out_cols; i++)
			{
			  d = ((format_len * DAY_MAX) >> 1)
			    +
			    (strlen
			     (month_name
			      (SMONTH (m * out_cols + i, fiscal_month))) >>
			     1);
			  sprintf (s2, "%*s", d,
				   month_name (SMONTH
					       (m * out_cols + i,
						fiscal_month)));
			  strcat (s1, s2);
			  if (i != out_cols)
			    {
			      sprintf (s2, "%*s",
				       (format_len * DAY_MAX) - (d -
								 blanks_between),
				       "");
			      strcat (s1, s2);
			    }
			}
		      print_text (stdout, s1);
		      /*
		         Print the day names.
		       */
		      for (i = 1; i <= out_cols; i++)
			{
			  for (d = DAY_MIN; d <= DAY_MAX; d++)
			    {
			      if (use_short3_day_name
				  && (format_len > FORMAT_LEN_MIN))
				sprintf (s2, "%*s", format_len,
					 short3_day_name (SDAY
							  (d, start_day)));
			      else
				sprintf (s2, "%*s", format_len,
					 short_day_name (SDAY
							 (d, start_day)));
			      strcat (s1, s2);
			    }
			  if (cal_with_week_number)
			    {
			      /*
			         The short week text which should be a proper abbreviation
			         of "calendar week", consists of 2 letters and is
			         separated by one space character of the month calendar.
			       */
			      /*
			       *** Translators, please translate this as a fixed 2-character text.
			       *** This text should be a proper abbreviation of "Calendar Week".
			       */
			      sprintf (s2, " %2s", _("CW"));
			      strcat (s1, s2);
			    }
			  if (i != out_cols)
			    {
			      sprintf (s2, "%*s",
				       (cal_with_week_number) ? blanks_between
				       - 3 : blanks_between, "");
			      strcat (s1, s2);
			    }
			}
		    }
		  else
		    {
		      /*
		         Print the month header.
		       */
		      if (use_short3_day_name)
			strcat (s1, " ");
		      for (i = 1; i <= out_cols; i++)
			{
			  strcpy (s2,
				  month_name (SMONTH
					      (m * out_cols + i,
					       fiscal_month)));
			  d = (int) strlen (s2);
			  sprintf (s2, "%*s%s", 2 + blanks_between + 1, "",
				   month_name (SMONTH
					       (m * out_cols + i,
						fiscal_month)));
			  strcat (s1, s2);
			  if (i != out_cols)
			    {
			      sprintf (s2, "%*s",
				       (format_len * MONTH_COLS) - (d + 2 +
								    blanks_between
								    + 1), "");
			      strcat (s1, s2);
			    }
			}
		      if (cal_with_week_number)
			{
			  if (use_short3_day_name)
			    j = 3;
			  else
			    j = 2;
			  /*
			     The short week text which should be a proper abbreviation
			     of "calendar week", consists of 2 letters and is
			     separated by one space character of the month calendar.
			   */
			  /*
			   *** Translators, please translate this as a fixed 2-character text.
			   *** This text should be a proper abbreviation of "Calendar Week".
			   */
			  sprintf (s3, "%-*s", j + blanks_between, _("CW"));
			}
		      print_text (stdout, s1);
		    }
		  print_text (stdout, s1);
		  /*
		     Print the days.
		   */
		  for (i = 1; i <= outer_end; i++)
		    {
		      if (!special_calsheet_flag)
			{
			  /*
			     Print the day name.
			   */
			  if (use_short3_day_name)
			    sprintf (s1, "%-*s", 3 + blanks_between,
				     short3_day_name (SDAY (i, start_day)));
			  else
			    sprintf (s1, "%-*s", 2 + blanks_between,
				     short_day_name (SDAY (i, start_day)));
			}
		      for (d = 1; d <= inner_end; d++)
			{
			  /*
			     Compute the days position in `year_vector'.
			   */
			  if (special_calsheet_flag)
			    day = m * out_cols * VEC_BLOCK - 1
			      + (((d - 1) / DAY_MAX) * VEC_BLOCK)
			      + ((i - 1) * DAY_MAX) + ((d - 1) % DAY_MAX) + 1;
			  else
			    day = m * out_cols * VEC_BLOCK - 1
			      + (d * DAY_MAX - MONTH_COLS) + (i - 1);
			  hday = (m * out_cols) + ((d - 1)
						   /
						   ((special_calsheet_flag) ?
						    DAY_MAX : MONTH_COLS)) +
			    1;
			  hmonth = SMONTH (hday, fiscal_month);
			  /*
			     Is `day' the actual day?
			   */
			  if (highlight_flag
			      && (year_vector[day] == act_day)
			      && (hmonth == act_month)
			      && (act_year ==
				  ((hday > hmonth) ? year + 1 : year)))
			    {
			      if (is_marked && (ehls1s.len == 1))
				marker_flag = print_highlighted_date (TRUE,
								      is_marked,
								      "", 0,
								      ehls1e.
								      seq,
								      ehls1e.
								      len);
			      else
				is_marked = marker_flag =
				  print_highlighted_date ((ehls1s.len ==
							   1) ? TRUE : FALSE,
							  is_marked,
							  ehls1s.seq,
							  ehls1s.len,
							  ehls1e.seq,
							  ehls1e.len);
			    }
			  else
			    {
			      /*
			         Is `day' a legal holiday?
			       */
			      j = 0;
			      if (year_vector[day])
				{
				  if (cal_special_flag)
				    (void) doy2date (year_vector[day + j],
						     (hday >
						      hmonth) ?
						     days_of_february (year +
								       1) ==
						     29 : is_leap_year,
						     &this_day, &this_month);
				  else
				    {
				      this_day = year_vector[day];
				      this_month = hmonth;
				    }
				  if (hd_ldays
				      [((this_month - 1) * MONTH_LAST) +
				       (this_day - 1)])
				    j = 1;
				}
			      if (j && highlight_flag)
				{
				  /*
				     `day' is a legal holiday!
				   */
				  if (is_marked && (ehls2s.len == 1))
				    marker_flag =
				      print_highlighted_date (TRUE, is_marked,
							      "", 0,
							      ehls2e.seq,
							      ehls2e.len);
				  else
				    is_marked = marker_flag =
				      print_highlighted_date ((ehls2s.len ==
							       1) ? TRUE :
							      FALSE,
							      is_marked,
							      ehls2s.seq,
							      ehls2s.len,
							      ehls2e.seq,
							      ehls2e.len);
				}
			      else
				{
				  /*
				     `day' is no holiday!
				   */
				  marker_flag =
				    print_unhighlighted_date (marker_flag);
				  is_marked = FALSE;
				}
			    }
			  if (cal_with_week_number
			      && ((special_calsheet_flag
				   && !(d % DAY_MAX))
				  || (!special_calsheet_flag && (i == 1))))
			    {
			      j = SPECIAL_VALUE;
			      if (special_calsheet_flag)
				{
				  j = 0;
				  day -= (DAY_MAX - 1);
				  if (day >= 0)
				    {
				      while ((j < DAY_MAX)
					     && (day + j < VEC_ELEMS)
					     && !year_vector[day + j])
					j++;
				      if ((j != DAY_MAX)
					  && (day + j < VEC_ELEMS))
					{
					  if (cal_special_flag)
					    {
					      (void)
						doy2date (year_vector
							  [day + j],
							  (hday >
							   hmonth) ?
							  days_of_february
							  (year + 1) ==
							  29 : is_leap_year,
							  &day, &hmonth);
					      j =
						week_number (day, hmonth,
							     (hday >
							      hmonth) ? year +
							     1 : year,
							     iso_week_number,
							     start_day);
					    }
					  else
					    j =
					      week_number (year_vector
							   [day + j], hmonth,
							   (hday >
							    hmonth) ? year +
							   1 : year,
							   iso_week_number,
							   start_day);
					}
				      else
					j = SPECIAL_VALUE;
				    }
				  else
				    j = SPECIAL_VALUE;
				}
			      else
				{
				  j = 0;
				  while ((j < DAY_MAX)
					 && (day + j < VEC_ELEMS)
					 && !year_vector[day + j])
				    j++;
				  if ((j != DAY_MAX) && (day + j < VEC_ELEMS))
				    {
				      if (cal_special_flag)
					{
					  (void)
					    doy2date (year_vector[day + j],
						      (hday >
						       hmonth) ?
						      days_of_february (year +
									1) ==
						      29 : is_leap_year, &day,
						      &hmonth);
					  j =
					    week_number (day, hmonth,
							 (hday >
							  hmonth) ? year +
							 1 : year,
							 iso_week_number,
							 start_day);
					}
				      else
					j =
					  week_number (year_vector[day + j],
						       hmonth,
						       (hday >
							hmonth) ? year +
						       1 : year,
						       iso_week_number,
						       start_day);
				    }
				  else
				    j = SPECIAL_VALUE;
				}
			      if (j != SPECIAL_VALUE)
				{
				  /*
				     We convert the computed week number to a week number text
				     (this looks nicer in output).
				   */
				  if (j < 0)
				    /*
				       Week starts in previous year and the first days
				       of the actual year are not in its first week.
				     */
				    sprintf (s2, "%02d", -j);
				  else if (!j)
				    /*
				       Week starts in previous year and the first days
				       of the actual year are in its first week.
				     */
				    sprintf (s2, "%02d", 1);
				  else
				    /*
				       Week starts in actual year.
				     */
				    sprintf (s2, "%02d", j);
				}
			      else
				if (special_calsheet_flag
				    || (!special_calsheet_flag && (i == 1)))
				strcpy (s2, "  ");
			      if (special_calsheet_flag)
				{
				  if (!is_marked)
				    strcat (s1, " ");
				  strcat (s1, s2);
				}
			      else
				{
				  sprintf (s4, "%*s", format_len - 2, "");
				  strcat (s3, s4);
				  strcat (s3, s2);
				}
			    }
			  /*
			     If we have completed a day line of a month:
			     Concatenate some separating blanks between the months.
			   */
			  if (special_calsheet_flag
			      && !(d % DAY_MAX) && (d != DAY_MAX * out_cols))
			    {
			      sprintf (s2, "%*s",
				       (cal_with_week_number) ? blanks_between
				       - 3 : blanks_between, "");
			      if (cal_with_week_number && is_marked)
				strcat (s2, " ");
			      strcat (s1, s2);
			    }
			}
		      /*
		         Print the constructed line.
		       */
		      print_text (stdout, s1);
		      marker_flag = is_marked = FALSE;
		    }
		  if (cal_with_week_number && !special_calsheet_flag)
		    {
		      print_text (stdout, s1);
		      print_text (stdout, s3);
		    }
		  /*
		     Print two NEWLINE characters between the month rows.
		   */
		  if (m < out_rows - 1)
		    {
		      print_text (stdout, s1);
		      print_text (stdout, s1);
		    }
		}
	    }
#if USE_RC
	  /*
	     Print the fixed dates list.
	   */
	  if (rc_use_flag && (is_ext_list || is_ext_range))
	    rc_use ();
#endif
	  /*
	     Print the eternal holiday list.
	   */
	  if (is_ext_year
	      && holiday_flag
	      && (((year == EASTER_MIN - 1)
		   && (fiscal_month > MONTH_MIN))
		  || ((year >= EASTER_MIN) && (year <= EASTER_MAX))))
	    print_all_holidays (FALSE, FALSE);
	}
    }
#if USE_RC
  /*
     A `-cNw' option and no explicit date given:
     Reset some affected global variables.
   */
  if (is_1month_mode || is_2month_mode)
    {
      is_fiscal_year = TRUE;
      fiscal_month = month;
      year = act_year;
    }
  else
    /*
       Re-initialize the actual date.
     */
  if (rc_period_flag
	&& (rc_forwards_flag
	      || rc_backwards_flag) && !rc_period_list && !is_date_given)
    (void) get_actual_date ();
  else
#endif /* USE_RC */
  if (cal_special_flag)
    act_day = tmp_ad;
}



static void
fill_year_vector (year)
     int year;
/*!
   Builds the "standard" year vector and the "special" year vector
     according to the delivered `year'.
*/
{
#if !USE_RC
  register int days_of_feb = days_of_february (year);
#endif
  register int i;
  register int j;
  register int count;
  register int d;
  register int m = fiscal_month;


  /*
     First, detect starting day of the fiscal/standard year.
   */
  i = weekday_of_date (DAY_MIN, m, year);
  i = day = SYEAR (i, start_day);
  j = d = 0;
  if (fiscal_month > MONTH_MIN)
    count = day_of_year (DAY_MIN, m, year) - 1;
  else
    count = 0;
  /*
     Then, fill the year vector.
   */
  LOOP
  {
    d++;
    count++;
#if USE_RC
    if (!valid_date (d, m, year))
#else /* !USE_RC */
    if (((m != 2) && (d > dvec[m - 1])) || ((m == 2) && (d > days_of_feb)))
#endif /* !USE_RC */
      {
	j++;
	if ((m < MONTH_MAX) && (j < MONTH_MAX))
	  {
	    if (fiscal_month > MONTH_MIN)
	      i = j * VEC_BLOCK + day;
	    else
	      i = m * VEC_BLOCK + day;
	    d = DAY_MIN;
	    m++;
	  }
	else
	  {
	    if (fiscal_month > MONTH_MIN)
	      {
		year++;
		if (j == MONTH_MAX || year > YEAR_MAX)
		  break;
		i = j * VEC_BLOCK + day;
		count = d = DAY_MIN;
		m = MONTH_MIN;
#if !USE_RC
		days_of_feb = days_of_february (year);
#endif
	      }
	    else
	      break;
	  }
      }
    if ((year == greg->year)
	&& (m == greg->month)
	&& ((d >= greg->first_day) && (d <= greg->last_day)))
      i--;
    else
      {
	day++;
	if (cal_special_flag)
	  year_vector[i - 1] = count;
	else
	  year_vector[i - 1] = d;
	special_vector[i - 1] = count;
      }
    i++;
    if (day > DAY_MAX)
      day = DAY_MIN;
  }
}



static Bool
print_unhighlighted_date (marker_flag)
     Bool marker_flag;
/*!
   Prints a date in NON-highlighted manner.
     If the MARKER_FLAG is set to TRUE when entering this function, this
     indicates, that a highlighting sequence/marking character pair was
     already displayed previously (or better adjacent) in the same row of
     the line, so it can be set to FALSE now and returned by this function,
     otherwise it will be returned untouched (means it was FALSE).  The
     respection of this MARKER_FLAG is absolutely necessary to format the
     line properly.
*/
{
  if (cal_both_dates_flag)
    {
      if (year_vector[day])
	{
	  if (marker_flag)
	    {
	      if (use_day_zeroleaded)
		sprintf (s2, "%02d(%03d)", year_vector[day],
			 special_vector[day]);
	      else
		sprintf (s2, "%2d(%3d)", year_vector[day],
			 special_vector[day]);
	      marker_flag = FALSE;
	    }
	  else if (use_day_zeroleaded)
	    sprintf (s2, " %02d(%03d)", year_vector[day],
		     special_vector[day]);
	  else
	    sprintf (s2, " %2d(%3d)", year_vector[day], special_vector[day]);
	}
      else
	{
	  if (marker_flag)
	    {
	      sprintf (s2, "%*s", format_len - 1, "");
	      marker_flag = FALSE;
	    }
	  else
	    sprintf (s2, "%*s", format_len, "");
	}
    }
  else
    {
      if (year_vector[day])
	{
	  if (marker_flag)
	    {
	      if (use_day_zeroleaded)
		sprintf (s2, "%0*d", format_len - 1, year_vector[day]);
	      else
		sprintf (s2, "%*d", format_len - 1, year_vector[day]);
	      marker_flag = FALSE;
	    }
	  else if (use_day_zeroleaded)
	    sprintf (s2, " %0*d", format_len - 1, year_vector[day]);
	  else
	    sprintf (s2, "%*d", format_len, year_vector[day]);
	}
      else
	{
	  if (marker_flag)
	    {
	      sprintf (s2, "%*s", format_len - 1, "");
	      marker_flag = FALSE;
	    }
	  else
	    sprintf (s2, "%*s", format_len, "");
	}
    }
  strcat (s1, s2);

  return (marker_flag);
}



static Bool
print_highlighted_date (last_item, marker_flag, hls_start, hls_slen, hls_end,
			hls_elen)
     const Bool last_item;
     const Bool marker_flag;
     const char *hls_start;
     const int hls_slen;
     const char *hls_end;
     const int hls_elen;
/*!
   Prints a date in highlighted manner using the given highlighting sequence,
     which consists of the "starting" highlighting sequence HLS_START and the
     "ending/final" highlighting sequence HLS_END, for distinguishing whether
     a real highlighting sequence is given.  The lengths of such a highlighting
     sequence/marking character pair are delivered, too.  If these lengths
     are greater ONE, it's assumed that a "real" (Termcap/ANSI) highlighting
     sequence pair is given, otherwise a marking character pair.  If the
     MARKER_FLAG is set to TRUE when entering this function, this indicates,
     that a highlighting sequence/marking character pair was already displayed
     previously (or better adjacent) in the same row of the line, so only an
     ending marking character may be produced in this case (this doesn't matter
     to "real" highlighting sequences, because their width is zero on the
     display [if interpreted properly by the screen device driver], but a
     marking character has the width of one), otherwise, all components of the
     marking character pair must be produced.  The respection of this
     MARKER_FLAG is absolutely necessary to format the line properly.
     If `last_item' is set to TRUE, this function does not produce a trailing
     blank character after an ending highlighting sequence respectively
     marking character.  Returns always TRUE to indicate, that a
     marking character/highlighting sequence pair is set by it!
*/
{
  if (cal_both_dates_flag)
    {
      if (use_day_zeroleaded)
	sprintf (s2, "%s%s%02d(%03d)%s%s",
		 (((hls_slen > 1) && !marker_flag) ? NO_HLS : ""), hls_start,
		 year_vector[day], special_vector[day],
		 hls_end, (((hls_elen > 1) && !last_item) ? NO_HLS : ""));
      else
	sprintf (s2, "%s%s%2d(%3d)%s%s",
		 (((hls_slen > 1) && !marker_flag) ? NO_HLS : ""), hls_start,
		 year_vector[day], special_vector[day],
		 hls_end, (((hls_elen > 1) && !last_item) ? NO_HLS : ""));
    }
  else
    {
      if (use_day_zeroleaded)
	sprintf (s2, "%s%s%0*d%s%s",
		 (((hls_slen > 1) && !marker_flag) ? NO_HLS : ""), hls_start,
		 format_len - 1, year_vector[day],
		 hls_end, (((hls_elen > 1) && !last_item) ? NO_HLS : ""));
      else
	sprintf (s2, "%s%s%*d%s%s",
		 (((hls_slen > 1) && !marker_flag) ? NO_HLS : ""), hls_start,
		 format_len - 1, year_vector[day],
		 hls_end, (((hls_elen > 1) && !last_item) ? NO_HLS : ""));
    }
  strcat (s1, s2);

  return (TRUE);
}
