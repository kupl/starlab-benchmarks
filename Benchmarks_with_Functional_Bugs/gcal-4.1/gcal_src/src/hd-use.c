/*!
*  \file hd-use.c
*  \brief Manage the eternal holiday list.
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
#if HAVE_CTYPE_H
# include <ctype.h>
#endif
#include "common.h"
#if USE_RC
# include "rc-defs.h"
#endif /* USE_RC */
#include "globals.h"
#include "hd-astro.h"
#include "hd-defs.h"
#include "hd-data.h"
#include "tty.h"
#include "utils.h"
#include "hd-use.h"



/*
*  GLOBAL variables definitions.
*/
/*! Stores the fixed eternal holiday list texts. */
char *hd_table[HD_ELEMS_MAX] = { (char *) NULL };

/*! Vector of holiday dates (legal days). */
char hd_ldays[HD_MAX] = { '\0' };

/*! Vector of holiday dates (memorial days). */
char hd_mdays[HD_MAX] = { '\0' };



/*
*  Function implementations.
*/
int
eval_holiday (day, month, year, wd, forwards)
     int day;
     const int month;
     const int year;
     const int wd;
     const Bool forwards;
/*!
   Detects first (FORWARDS==TRUE) or last (FORWARDS==FALSE) appearance
     of weekday `wd' in given date starting the search at `day'
     and returns the day number of `wd' in month.
*/
{
  LOOP
  {
    if (weekday_of_date (day, month, year) == wd)
      break;
    else
      (forwards) ? day++ : day--;
  }
  /*
     Correction for missing days during Gregorian Reformation.
   */
  if ((year == greg->year)
      && (month == greg->month)
      && (day >= greg->first_day) && (day <= greg->last_day))
    day =
      (forwards) ? eval_holiday (greg->last_day + 1, month, year, wd,
				 forwards) : eval_holiday (greg->first_day -
							   1, month, year, wd,
							   forwards);

  return (day);
}



int
orthodox_easter (year, greg_diff, greg_year, greg_month, greg_first_day,
		 greg_last_day)
     const int year;
     int *greg_diff;
     const int greg_year;
     const int greg_month;
     const int greg_first_day;
     const int greg_last_day;
/*!
   Calculates the day and month of Orthodox Easter giving the year
     and the specific Gregorian Reformation date.  It returns the
     "actual day_of_year number" of Eastern Easter of the Eastern Orthodox
     churches after AD 29 and passes the number of days between the given
     Gregorian calendar date and the according Julian calendar date
     in `&greg_diff'.
     If my references are right, the Eastern Orthodox churches celebrated
     Easter on the date of the Hebrew Pesach feast for dates prior AD 325
     (Council of Nicaea).  Later dated Easter is celebrated according to
     the Nicaean Easter date rule (Metonic cycle, Epacts and so forth...),
     but conforming to the Julian calendar.  So we have to compute the date
     of Easter by using Julian dates for a definite year, means, we have to
     ignore any missing days of a Gregorian Reformation for this year.
     Then, we simply add the day difference of the given Gregorian calendar
     date and the according Julian calendar date -- i.e. the amount of days,
     the Julian calendar is past the Gregorian calendar -- to this date!
   See the `knuth_easter_formula()' function for further details.
*/
{
  auto Greg_struct tmp_greg;
  register int easter;
  register int diff;


  /*
     We first need to buffer the Gregorian Reformation period actually used.
   */
  tmp_greg.first_day = greg->first_day;
  tmp_greg.last_day = greg->last_day;
  tmp_greg.month = greg->month;
  tmp_greg.year = greg->year;
  if (year < 325)
    easter = tishri_1 (year) - 163;
  else
    {
      /*
         Then we set the Gregorian Reformation period to a date
         which is out of the range of dates Gcal is able to manage,
         for computing the Easter Sunday of a Julian date, means
         we behave as if no Gregorian Reformation has ever happened.
       */
      greg->last_day = greg->first_day = DAY_MIN;
      greg->month = MONTH_MIN;
      greg->year = YEAR_MAX + 3;
      /*
         Compute the Julian Easter Sunday's date.
       */
      easter = knuth_easter_formula (year);
    }
  /*
     Now we set the Gregorian Reformation period to the date given.
   */
  greg->year = greg_year;
  greg->month = greg_month;
  greg->first_day = greg_first_day;
  greg->last_day = greg_last_day;
  /*
     The computation is done for the next date after the given Gregorian
     Reformation period, but for the current year (Gregorian date).
     If this is not proper, I would like to hear *your* solution how
     to compute it in the right way!
   */
  diff = julian_gregorian_diff (greg->last_day + 1, greg->month, year);
  if (greg_diff != (int *) NULL)
    *greg_diff = diff;
  /*
     And restore the buffered Gregorian Reformation date actually used.
   */
  greg->year = tmp_greg.year;
  greg->month = tmp_greg.month;
  greg->last_day = tmp_greg.last_day;
  greg->first_day = tmp_greg.first_day;

  return (easter + diff);
}



int
tishri_1 (year)
     const int year;
/*!
   Returns the `day_of_year' number of the (arithmetical lunisolar)
     Hebrew/Jewish Hebrew_New_Year/Rosh_Hashana/Tishri_1 for the
     given Julian/Gregorian year YEAR.
   Used formula:  John H. Conway, Guy and Berlekamp: "Winning Ways", Vol. 2.
   References:    Lot's of, but see `doc/calendar.faq' for the basics.
*/
{
  auto double n;
  register int golden_number = (year % 19) + 1;
  register int g12 = (12 * golden_number) % 19;
  register int doy = day_of_year (dvec[8 - 1], 8, year);


  /*
     Calculation for Julian years.
   */
  n = ((765433.0 / 492480.0) * g12)
    + ((year % 4) / 4.0) - (((313.0 * year) + 89081.0) / 98496.0);
  if (year >= greg->year)
    /*
       Correction term for Gregorian years.
     */
    n += ((year / 100) - (year / 400) - 2.0);
  doy += (int) n;
  switch (weekday_of_date ((int) n, 9, year))
    {
    case 3:
    case 5:
    case DAY_MAX:
      doy++;
      break;
    case DAY_MIN:
      if ((n - (int) n >= 23269.0 / 25920.0) && (g12 > 11))
	doy++;
      break;
    case 2:
      if ((n - (int) n >= 1367.0 / 2160.0) && (g12 > 6))
	doy++;
      break;
    default:
      ;				/* Void, nothing to do. */
    }

  return (doy);
}



int
muharram_1 (year, doy)
     const int year;
     int *doy;
/*!
   Returns the `day_of_year' number of the (arithmetical lunar)
     Islamic *CIVIL* calendar's Islamic_New_Year/MuHarram_1 for the
     given Julian/Gregorian year YEAR, which must be greater than AD 621.
     The caller has to guarantee this!
     If MuHarram_1 appears twice in the year YEAR, the `day_of_year' number of
     the second (later) date is passed via the address of DOY, otherwise zero.
   Used formula:  My own one :)
   References:    Lot's of, but see `doc/calendar.faq' for the basics.
*/
{
  auto Ulint mjd;
  register int i;
  register int hijri_year;
  auto int d;
  auto int m;
  auto int y;
  auto int d2;
  auto int m2;
  auto int y2 = 0;


  /*
     Calculate the approximate Hijri calendar year number for YEAR.
     Since the Hijri year is consistently shorter than a Julian
     or Gregorian year, we can simply multiply the year difference
     of `year-621' with a proper constant factor.  This factor is
     1.03071206847898 in Julian years, and 1.03069090396012 in
     Gregorian years.  The average length of a Hijri year is
     354.36~ days (10631 days within a 30-year cycle divided by 30),
     and when one divides this average length by the average length
     of a Julian year (365.25 days) or of a Gregorian year (365.2425),
     these above mentioned constant factors are the result.
   */
  if (year > greg->year)
    /*
       Gregorian years.
     */
    hijri_year = (greg->year - 621) * 1.03071206847898
      + (year - greg->year) * 1.03069090396012;
  else
    /*
       Julian years.
     */
    hijri_year = (year - 621) * 1.03071206847898;
  /*
     Perform at least two checks to detect the date(s) of MuHarram_1.
   */
  LOOP
  {
    /*
       Detect the number of complete 30-year cycles passed since
       MuHarram_1, 1 Anno Hegirae, which corresponds to AD 16-Jul-0622
       historically.  Then multiply this number by 10631 (the total
       number of days of such a cycle) and add 227017 (the number of
       days passed from AD 01-Jan-0001 until AD 16-Jul-0622).
     */
    mjd = 227017L + (Ulint) (hijri_year / 30) * 10631L;
    /*
       Calculate the number of Hijri years in the actual 30-year cycle.
     */
    i = hijri_year % 30;
    /*
       Is the actual 30-year cycle completed?
     */
    if (i)
      {
	/*
	   No, add the number of days of the remaining Hijri years `years-1'.
	 */
	mjd += (Ulint) ((i - 1) * 354);
	/*
	   And also add the number of leap days of the Hirji years which
	   occurred in the actual 30-year cycle except the actual Hirjri year.
	 */
	for (--i; i; i--)
	  if (i == 2
	      || i == 5
	      || i == 7
	      || i == 10
	      || i == 13
	      || i == 16
	      || i == 18 || i == 21 || i == 24 || i == 26 || i == 29)
	    mjd++;
      }
    else
      /*
         Yes, a completed 30-year cycle detected,
         so subtract one Hijri non-leap year.
       */
      mjd -= 354L;
    /*
       Check if MuHarram_1 occurs twice in the year YEAR.
     */
    if (mjd < date2num (13, MONTH_MIN, year))
      {
	num2date (mjd, &d2, &m2, &y2);
	/*
	   Detect if the *next* Hirji year completes a 30-year cycle.
	 */
	if (!(++hijri_year % 30))
	  {
	    /*
	       Yes, so add one Hijri leap year and
	       skip processing (this case is the first check).
	     */
	    mjd += 355L;
	    break;
	  }
      }
    else
      break;
  }
  num2date (mjd, &d, &m, &y);
  /*
     Check if MuHarram_1 occurs twice in the year YEAR.
   */
  if (y == y2)
    {
      /*
         Yes, MuHarram_1 occurs twice in the year YEAR.
       */
      *doy = day_of_year (d, m, y);
      return (day_of_year (d2, m2, y2));
    }
  /*
     No, MuHarram_1 occurs only once in the year YEAR.
   */
  *doy = 0;
  if (y2 == year)
    /*
       MuHarram_1 found during the first check.
     */
    return (day_of_year (d2, m2, y2));

  /*
     MuHarram_1 found during the second check.
   */
  return (day_of_year (d, m, y));
}



int
find_chinese_leap_month (conjunction_vector, nh_ws_prev_year, year, hour, min)
     Ulint *conjunction_vector;
     Ulint *nh_ws_prev_year;
     const int year;
     const int hour;
     const int min;
/*!
   Calculates the leap month of the Chinese calendar (based on the method
     used since AD 1645, which implemented the use of true -- astronomically
     detected -- Sun), which occurs in the given Julian/Gregorian year YEAR.
     * If the computation of the leap month fails by any reason,
       SPECIAL_VALUE is returned.
     * If there is no leap month in the YEAR,
       0 is returned.
     * If there is a leap month and it belongs to the YEAR,
       +1...+12 is returned.
     * If there is a leap month and it belongs to the previous YEAR,
       -11...-12 is returned.
   The date of the winter solstice (major solar term/Zhong-Qi 11) of the
     previous YEAR is returned via the address of NH_WS_PREV_YEAR.  All
     conjunction dates (starting on the date of NH_WS_PREV_YEAR or later)
     which are necessary to calculate the Chinese calendar for the YEAR,
     are returned via the address of CONJUNCTION_VECTOR.  The caller has to
     guarantee that the CONJUNCTION_VECTOR has 16 elements!
   Calculations are done for a line at a definite meridian expressed as
     a time value in HOUR and MIN.  If HOUR and MIN are set to zero,
     calculations are done for UTC/GMT.  If HOUR and MIN have a positive
     sign, calculations are done for meridians East of Greenwich, otherwise
     for meridians West of Greenwich.
   For a good and detailed reference of the Chinese calendar, see:
     "The Mathematics of the Chinese Calendar" by Helmer Aslaksen,
     <http://www.math.nus.edu.sg/aslaksen/>.
*/
{
  auto double n;
  auto Ulint the_conjunction = 0L;
  auto Ulint cj_prev;
  auto Ulint cj;
  auto Ulint zq_prev;
  auto Ulint zq;
  auto Ulint zq1;
  register int lm_prev = 0;
  register int lm = 0;
  register int i = 0;
  register int j = 0;
  auto int d;
  auto int m;
  auto int y;
  auto int d2;
  auto int m2;
  auto int y2;
  auto Bool lm_found = FALSE;
  auto Bool double_zq_found = FALSE;
  auto Bool second_double_zq_found = FALSE;


  /*
     Get the latest date that's necessary to check,
     which is the major solar term/Zhong-Qi 1 of the next YEAR.
   */
  y = year;
  n = NH_WS;
  zq1 = (Ulint) sun_longitude (n, &d, &m, &y, hour, min, FALSE);
  if (zq1 == (Ulint) SPECIAL_VALUE)
    return ((int) zq1);
  zq1 =
    (Ulint) next_sun_longitude (&n, 60.0, (double) zq1, &d, &m, &y, hour,
				min);
  if (zq1 == (Ulint) SPECIAL_VALUE)
    return ((int) zq1);
  /*
     Get the earliest date that's necessary to check,
     which is the major solar term/Zhong-Qi 10 of the previous YEAR.
   */
  y = year - 1;
  n = NH_WS - 30.0;
  zq_prev = (Ulint) sun_longitude (n, &d, &m, &y, hour, min, FALSE);
  if (zq_prev == (Ulint) SPECIAL_VALUE)
    return ((int) zq_prev);
  /*
     Get the major solar term/Zhong-Qi 11 of the previous YEAR,
     the winter solstice, which is the base date for computing
     the Chinese_New_Year for the YEAR.
   */
  *nh_ws_prev_year = zq =
    (Ulint) next_sun_longitude (&n, 30.0, (double) zq_prev, &d, &m, &y, hour,
				min);
  if (zq == (Ulint) SPECIAL_VALUE)
    return ((int) zq);
  /*
     And reset to the major solar term/Zhong-Qi 11 of the previous YEAR
     used in the ``next'' search later.
   */
  n -= 30.0;
  /*
     Get the conjunction date of the previous YEAR, which is on or after
     the winter solstice date.
   */
  *conjunction_vector = cj =
    (Ulint) moonphase (MPHASE_NEW, FALSE, NULL, &the_conjunction, &d, &m, &y,
		       hour, min);
  /*
     If the conjunction date is on a later date as the winter solstice,
     get the previous conjunction date.
   */
  if (cj > zq)
    the_conjunction--;
  /*
     All preconditioning is done, now detect a possible leap month
     (that's a month without a major solar term).
   */
  y2 = y = year - 1;
  LOOP
  {
    /*
       Get the actual conjunction date.
     */
    cj_prev = cj;
    cj =
      (Ulint) moonphase (MPHASE_NEW, FALSE, NULL, &the_conjunction, &d, &m,
			 &y, hour, min);
    if (j)
      {
	if (*conjunction_vector != cj)
	  *(conjunction_vector + j) = cj;
	else
	  j--;
      }
    /*
       Check for leap month.
     */
    if (lm || (!lm_found && (zq < cj)))
      {
	if (lm)
	  {
	    if (!lm_found)
	      {
		if (zq >= cj)
		  {
		    /*
		       Raw leap month detected.
		     */
		    lm_found = TRUE;
		    lm = ((i + 10) % MONTH_MAX) - 1;
		    if (lm <= 0)
		      {
			lm += MONTH_MAX;
			if (i < MONTH_MAX)
			  lm = -lm;
		      }
		  }
	      }
	  }
	else
	  lm = SPECIAL_VALUE;
      }
    else
      lm = 0;
    /*
       Check for months with two solar terms.
     */
    if (lm_found && (zq_prev >= cj_prev) && (zq < cj))
      {
	if (!double_zq_found)
	  {
	    double_zq_found = TRUE;
	    lm_prev = lm;
	  }
	else
	  second_double_zq_found = TRUE;
	if (lm > 0)
	  lm_found = FALSE;
      }
    /*
       And get the date of the actual major solar term.
     */
    zq_prev = zq;
    zq =
      (Ulint) next_sun_longitude (&n, 30.0, (double) zq_prev, &d2, &m2, &y2,
				  hour, min);
    if (zq == (Ulint) SPECIAL_VALUE)
      return ((int) zq);
    /*
       Detect if the necessary range of dates is examined.
     */
    if (cj >= zq1)
      break;
    /*
       Next conjunction.
     */
    the_conjunction++;
    /*
       Increase the month and conjunction vector counter.
     */
    i++;
    j++;
  }
  /*
     Correct the raw leap month found.
   */
  if (double_zq_found && !second_double_zq_found)
    {
      y = year;
      zq = (Ulint) sun_longitude (NH_WS, &d, &m, &y, hour, min, FALSE);
      if (zq == (Ulint) SPECIAL_VALUE)
	return ((int) zq);
      if (lm == MONTH_MAX)
	{
	  the_conjunction = 0L;
	  cj =
	    (Ulint) moonphase (MPHASE_NEW, FALSE, NULL, &the_conjunction, &d,
			       &m, &y, hour, min);
	  /*
	     A 12th leap month can only occur if there is NO New Moon
	     on the same date as the winter solstice of the next YEAR.
	   */
	  if (cj == zq)
	    /*
	       The true leap month is the previously detected raw leap month.
	     */
	    lm = lm_prev;
	}
      else if (lm != MONTH_MAX - 1)
	{
	  j = MONTH_MAX - 2;
	  while (*(conjunction_vector + j) <= zq)
	    j++;
	  if (j > MONTH_MAX)
	    /*
	       The true leap month is the previously detected raw leap month.
	     */
	    lm = lm_prev;
	  else
	    /*
	       This YEAR is no leap sui, so it does not have a leap month!
	       (A sui is the solar year starting at the winter solstice
	       of the previous YEAR and ending at the winter solstice
	       of the YEAR.  A sui is a leap sui if there are 12 complete
	       months between the two 11th months at the beginning and
	       end of the sui.)
	     */
	    lm = SPECIAL_VALUE;
	}
    }
  if (lm == SPECIAL_VALUE)
    lm = 0;

  return (lm);
}



int
decode_date_format (format_txt, result_txt, day, month, year, doy,
		    hls1_set, hls2_set, fixed_length_names)
     char *format_txt;
     char **result_txt;
     int day;
     int month;
     const int year;
     const int doy;
     const Bool hls1_set;
     const Bool hls2_set;
     const Bool fixed_length_names;
/*!
   Concatenates the elements of a date (inclusive highlighting) in formatted
     manner to `&result_txt' according to the order of format specifiers, which
     are given in the `format_txt.  The core area of the string vector
     `&result_txt' must be allocated and "filled" properly by the caller.
     "Filled" either means `&result_txt' contains a ZERO terminated text at
     function startup or a previous "*result_txt = '\0';" statement was
     performed by the caller before entering this function.
     If `day' is not zero but `doy' is zero:
       Use day of month and all other GROUPS.
     If `day' is zero but `doy' is not zero:
       Use day of year, insert a "day" text, use all other GROUPS except the month GROUP.
     If `day' is not zero and `doy' is not zero:
       Use both day of month and day of year and all other GROUPS.
     If `day' is zero and `doy' is zero:
       This results to an unmanaged internal error, because this
       case is unchecked due to faster internal processing speed.
     If `hls1_set' is TRUE and `hls2_set' is FALSE:
       Use `ehls1' highlighting sequences/marking characters.
     If `hls1_set' is FALSE and `hls2_set' is TRUE:
       Use `ehls2' highlighting sequences/marking characters.
     If `hls1_set' is FALSE and `hls2_set' is FALSE:
       Use no highlighting sequences/marking characters.
     If `hls1_set' is TRUE and `hls2_set' is TRUE:
       This case is unchecked due to faster internal processing speed
       but doesn't result to an unmanaged internal error, because
       the `ehls1' sequence is used by default.
     If `fixed_length_names' is set to TRUE, the texts returned by the
       `day_name()' and `month_name()' functions are set to a fixed length
       of `len_???name_max' characters (means trailing spaces), otherwise
       these texts are used as is.
     The `format_txt' must have exactly ONE day and year format specifier,
       exactly ONE format specifier from the month GROUP, BOTH format
       specifiers from the highlighting GROUP (%1 must be given before %2),
       and may have ONE weekday name format specifier.
     All format specifiers -- except the ones from the highlighting group --
       may be lead by an optional format instruction, which template is
       `[ ALIGNMENT [SIGN] [LZERO] WIDTH [STYLE] [SUFFIX] FORMAT ]', (see the
       `decode_format()' function for a more brief description of the
       format instruction), which will be evaluated and respected in case
       it is stated correctly:
     Day:
       %[FORMAT]D
     Year:
       %[FORMAT]Y
     Weekday name:
       %[FORMAT]K
     Month GROUP:
       %[FORMAT]M
       %[FORMAT]U
     Highlighting GROUP:
       %1 - Starting highlighting sequence
       %2 - Ending highlighting sequence
     Other (no group):
       _  - Blank (' ') character
       \_ - Underscore ('_') character
       \% - Percent ('%') character
       \\ - Backslash ('\') character
   The function return value is the text length of the "real" highlighting
   sequence pair if "real" highlighting must be performed, otherwise zero.
*/
{
  register int skip_space = 0;
  register int i = (int) strlen (format_txt);
  register int j;
  register int k;
  register int n;
  register int hls_len = 0;
  register int len = (int) strlen (*result_txt);
  register int pos;
  auto int fstyle = FSTYLE_NONE;
  auto int fwidth = SPECIAL_VALUE;
  auto int d = day;
  auto char *ptr_char;
  auto Bool hlsxe_before_month = TRUE;
  auto Bool is_obsolete_space = FALSE;
  auto Bool is_marking_char = (Bool) (ehls1s.len == 1);
  auto Bool is_special_date = (!day && doy);
  auto Bool is_cformat = FALSE;
  auto Bool is_lformat = FALSE;
  auto Bool is_sign = FALSE;
  auto Bool is_lzero = FALSE;
  auto Bool is_fformat = FALSE;
  auto Bool is_suffix = FALSE;


  j = len + i;
  if ((Uint) j >= maxlen_max)
    resize_all_strings ((Uint) j + 1, FALSE, __FILE__, (long) __LINE__);
  if ((hls1_set || hls2_set) && !is_marking_char)
    {
      if (hls1_set)
	hls_len = ehls1s.len + ehls1e.len;
      else
	hls_len = ehls2s.len + ehls2e.len;
    }
  if (is_special_date)
    (void) doy2date (doy, (days_of_february (year) == 29), &day, &month);
  do
    {
      while (*format_txt
	     && (*format_txt != DFORMAT_CHAR) && (*format_txt != QUOTE_CHAR))
	{
	  if (!skip_space
	      || ((*format_txt != ' ') && (*format_txt != PSEUDO_BLANK)))
	    {
	      if ((Uint) len >= maxlen_max)
		resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				    (long) __LINE__);
	      if (*format_txt == PSEUDO_BLANK)
		*(*result_txt + len++) = ' ';
	      else
		*(*result_txt + len++) = *format_txt;
	    }
	  format_txt++;
	  if (skip_space)
	    skip_space--;
	}
      if ((Uint) len >= maxlen_max)
	resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
			    (long) __LINE__);
      *(*result_txt + len) = '\0';
      if (*format_txt)
	{
	  if (*format_txt == DFORMAT_CHAR)
	    {
	      pos = decode_format (format_txt, 1, &is_cformat,
				   &is_lformat, &is_sign, &is_lzero,
				   &is_suffix, &is_fformat, &fstyle, &fwidth);
	      format_txt += pos;
	      if (fwidth != SPECIAL_VALUE)
		if ((Uint) len + fwidth >= maxlen_max)
		  resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				      (long) __LINE__);
	      /*
	         Format specifier found.
	       */
	      switch (*format_txt)
		{
		case HLS1S_CHAR:
		  if (highlight_flag)
		    {
		      if (!hls1_set && !hls2_set)
			{
			  if (!len && is_marking_char)
			    {
			      len++;
			      strcat (*result_txt, NO_HLS);
			    }
			}
		      else if (len)
			if (is_marking_char
			    && (*(*result_txt + (len - 1)) == ' '))
			  *(*result_txt + --len) = '\0';
		      if (hls1_set)
			{
			  len += ehls1s.len;
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, ehls1s.seq);
			}
		      else if (hls2_set)
			{
			  len += ehls2s.len;
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, ehls2s.seq);
			}
		    }
		  break;
		case HLS1E_CHAR:
		  if (highlight_flag)
		    {
		      if (is_marking_char
			  && (*(format_txt + 1) != ' ')
			  && (*(format_txt + 1) != PSEUDO_BLANK)
			  && !hls1_set && !hls2_set)
			{
			  len++;
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, NO_HLS);
			}
		      if (hls1_set)
			{
			  len += ehls1e.len;
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, ehls1e.seq);
			  if (is_marking_char)
			    {
			      skip_space++;
			      hlsxe_before_month = is_obsolete_space = TRUE;
			    }
			}
		      else if (hls2_set)
			{
			  len += ehls2e.len;
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, ehls2e.seq);
			  if (is_marking_char)
			    {
			      skip_space++;
			      hlsxe_before_month = is_obsolete_space = TRUE;
			    }
			}
		    }
		  break;
		case DAYNR_CHAR:
		  if (hlsxe_before_month)
		    {
		      hlsxe_before_month = FALSE;
		      if (skip_space)
			skip_space--;
		    }
		  if (is_special_date)
		    i = doy;
		  else
		    i = day;
		  if (is_suffix)
		    k = len_suffix_max;
		  else
		    k = 0;
		  if (fwidth == SPECIAL_VALUE)
		    {
		      if (is_special_date)
			fwidth = 3;
		      else
			fwidth = 2;
		      if ((Uint) len + fwidth + k >= maxlen_max)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  else if (is_special_date)
		    {
		      fwidth++;
		      if ((Uint) len + fwidth >= maxlen_max)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  j = use_format (result_txt, len, "", i, TRUE,
				  is_cformat, is_lformat, is_sign, is_lzero,
				  is_suffix, is_fformat, fstyle, fwidth, 0);
		  len += j;
		  if (is_suffix)
		    {
		      k += fwidth;
		      if (j != k)
			{
			  if (j < k)
			    {
			      if ((Uint) len + k - j >= maxlen_max)
				resize_all_strings (maxlen_max << 1, FALSE,
						    __FILE__,
						    (long) __LINE__);
			      for (i = 0; i <= j; i++)
				*(*result_txt + len - i + k - j) =
				  *(*result_txt + len - i);
			      for (i = 0; i < k - j; i++)
				*(*result_txt + len - j + i) = ' ';
			      len += (k - j);
			    }
			  else if (is_fformat)
			    len -= (j - k);
			}
		    }
		  if (doy)
		    {
		      if (!d)
			{
			  ptr_char = _(" day");
			  len += (int) strlen (ptr_char);
			  if ((Uint) len >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, ptr_char);
			}
		      else
			{
			  fwidth++;
			  if ((Uint) len + 2 + fwidth >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  strcat (*result_txt, "(");
			  len +=
			    use_format (result_txt, len + 1, "", doy, TRUE,
					is_cformat, is_lformat, is_sign,
					is_lzero, FALSE, is_fformat, fstyle,
					fwidth, 0);
			  strcat (*result_txt, ")");
			  len += 2;
			}
		    }
		  break;
		case WDNAME_CHAR:
		  if (hlsxe_before_month)
		    {
		      hlsxe_before_month = FALSE;
		      if (skip_space)
			skip_space--;
		    }
		  if (fixed_length_names && (fwidth == SPECIAL_VALUE))
		    {
		      fwidth = len_dayname_max;
		      is_lformat = TRUE;
		      if ((Uint) len + fwidth >= maxlen_max)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  len +=
		    use_format (result_txt, len,
				day_name (weekday_of_date (day, month, year)),
				0, FALSE, is_cformat, is_lformat, is_sign,
				is_lzero, is_suffix, is_fformat, fstyle,
				fwidth, 0);
		  break;
		case MONTHNAME_CHAR:
		  if (!is_special_date)
		    {
		      if (fixed_length_names && (fwidth == SPECIAL_VALUE))
			{
			  fwidth = len_monthname_max;
			  is_lformat = TRUE;
			  if ((Uint) len + fwidth >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			}
		      len +=
			use_format (result_txt, len, month_name (month), 0,
				    FALSE, is_cformat, is_lformat, is_sign,
				    is_lzero, is_suffix, is_fformat, fstyle,
				    fwidth, 0);
		    }
		  else
		    {
		      if (len)
			{
			  if ((*(format_txt + 1) == ' '
			       || *(format_txt + 1) == PSEUDO_BLANK)
			      && (*(*result_txt + (len - 1)) == ' '))
			    skip_space++;
			  else
			    if (!skip_space
				&& is_obsolete_space && hlsxe_before_month)
			    skip_space++;
			}
		      hlsxe_before_month = is_obsolete_space = FALSE;
		    }
		  break;
		case MONTHNR_CHAR:
		  if (!is_special_date)
		    {
		      if (is_suffix)
			k = len_suffix_max;
		      else
			k = 0;
		      if (fwidth == SPECIAL_VALUE)
			{
			  fwidth = 2;
			  if ((Uint) len + fwidth + k >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			}
		      j = use_format (result_txt, len, "", month, TRUE,
				      is_cformat, is_lformat, is_sign,
				      is_lzero, is_suffix, is_fformat, fstyle,
				      fwidth, 0);
		      len += j;
		      if (is_suffix)
			{
			  k += fwidth;
			  if (j != k)
			    {
			      if (j < k)
				{
				  if ((Uint) len + k - j >= maxlen_max)
				    resize_all_strings (maxlen_max << 1,
							FALSE, __FILE__,
							(long) __LINE__);
				  for (i = 0; i <= j; i++)
				    *(*result_txt + len - i + k - j) =
				      *(*result_txt + len - i);
				  for (i = 0; i < k - j; i++)
				    *(*result_txt + len - j + i) = ' ';
				  len += (k - j);
				}
			      else if (is_fformat)
				len -= (j - k);
			    }
			}
		    }
		  else
		    {
		      if (len)
			{
			  if ((*(format_txt + 1) == ' '
			       || *(format_txt + 1) == PSEUDO_BLANK)
			      && (*(*result_txt + (len - 1)) == ' '))
			    skip_space++;
			  else
			    if (!skip_space
				&& is_obsolete_space && hlsxe_before_month)
			    skip_space++;
			}
		      hlsxe_before_month = is_obsolete_space = FALSE;
		    }
		  break;
		case YEARNR_CHAR:
		  if (hlsxe_before_month)
		    {
		      hlsxe_before_month = FALSE;
		      if (skip_space)
			skip_space--;
		    }
		  n = len_year_max;
		  if (transform_year)
		    {
		      j = year - transform_year;
		      if ((j >= 0) && (transform_year > 0))
			j++;
		      if (abs (j) > YEAR_MAX)
			n++;
		    }
		  else
		    j = year;
		  if (is_suffix)
		    k = len_suffix_max;
		  else
		    k = 0;
		  if (fwidth == SPECIAL_VALUE)
		    {
		      fwidth = n;
		      if ((Uint) len + fwidth + k >= maxlen_max)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  j = use_format (result_txt, len, "", j, TRUE,
				  is_cformat, is_lformat, is_sign, is_lzero,
				  is_suffix, is_fformat, fstyle, fwidth, 0);
		  len += j;
		  if (is_suffix)
		    {
		      k += fwidth;
		      if (j != k)
			{
			  if (j < k)
			    {
			      if ((Uint) len + k - j >= maxlen_max)
				resize_all_strings (maxlen_max << 1, FALSE,
						    __FILE__,
						    (long) __LINE__);
			      for (i = 0; i <= j; i++)
				*(*result_txt + len - i + k - j) =
				  *(*result_txt + len - i);
			      for (i = 0; i < k - j; i++)
				*(*result_txt + len - j + i) = ' ';
			      len += (k - j);
			    }
			  else if (is_fformat)
			    len -= (j - k);
			}
		    }
		  break;
		default:
		  /*
		     This case must be an internal error!
		   */
		  abort ();
		}
	    }
	  else
	    {
	      if ((Uint) len + 2 >= maxlen_max)
		resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				    (long) __LINE__);
	      /*
	         Quote character found.
	       */
	      switch (*++format_txt)
		{
		case PSEUDO_BLANK:
		case DFORMAT_CHAR:
		  *(*result_txt + len++) = *format_txt;
		  *(*result_txt + len) = '\0';
		  break;
		default:
		  if (*format_txt)
		    {
		      if (*format_txt != QUOTE_CHAR)
			*(*result_txt + len++) = QUOTE_CHAR;
		      *(*result_txt + len++) = *format_txt;
		      *(*result_txt + len) = '\0';
		    }
		  else
		    format_txt--;
		}
	    }
	}
      else
	break;
    }
  while (*++format_txt);

  return (hls_len);
}



int
decode_format (format_txt, pos, is_cformat, is_lformat, is_sign,
	       is_lzero, is_suffix, is_fformat, fstyle, fwidth)
     char *format_txt;
     int pos;
     Bool *is_cformat;
     Bool *is_lformat;
     Bool *is_sign;
     Bool *is_lzero;
     Bool *is_suffix;
     Bool *is_fformat;
     int *fstyle;
     int *fwidth;
/*!
   Checks for an optional format instruction which can either be used
     in case a TVAR or a `%...' special text is referenced.  The template
     of such a format is:  [ ALIGNMENT [SIGN] [LZERO] WIDTH [STYLE] [SUFFIX] FORMAT ],
     e.g., $[<|:|>[+][0]N[u|U|l|L|w|W][&]*|#]TVAR, resp., %[<|:|>[+][0]N[u|U|l|L|w|W][&]*|#]?,
     like `$:+010u&#a' or `%>20l&*Y'.
     ALIGNMENT: '<' == adjusts field contents at the left margin using width WIDTH.
                ':' == adjusts field contents in centered manner using width WIDTH.
                '>' == ajdusts field contents at the right margin using width WIDTH.
     SIGN     : '+' == leads a numerical value always by its sign.
     LZERO    : '0' == fills a numerical value up to WIDTH leading zeroes
                       inclusivly a possibly leading explicit SIGN or
                       an implicitly leading negative sign.
     WIDTH    : FWIDTH_MIN...FWIDTH_MAX == is the width of the field.
     STYLE    : 'u' == converts field contents to upper-case letters.
                'l' == converts field contents to lower-case letters.
                'w' == converts field contents to capitalized words.
     SUFFIX   : '&' == provides a numerical value with an ordinal number suffix.
     FORMAT   : '*' == does not cut the field contents after
                       position WIDTH if it is longer than WIDTH.
                '#' == cuts the field contents after
                       position WIDTH if it is longer than WIDTH.
   On success, this function returns the position `pos' at which the format
     instruction ends in the `format_txt' and the width of the field in `&fwidth'.
     `&is_cformat', `&is_lformat', `&is_sign', `&is_lzero', `&is_suffix' and
     `&is_fformat' are set to TRUE if the according directive characters are
     found in proper order in `format_txt', otherwise they are set to FALSE;
     and `&fstyle' is set to the according style value, otherwise to FSTYLE_NONE.
   `&fwidth' is set to SPECIAL_VALUE and the unchanged position `pos' is
     returned in case an error occurs during the scan of the format instruction,
     which either means that it was not defined, or it contained a wrong
     or misplaced or misspelled element.
*/
{
  register int old_pos = pos;
  register int i;
  register int len;


  *is_cformat = (*is_lformat) = (*is_sign) = (*is_lzero) = (*is_suffix) =
    (*is_fformat) = FALSE;
  *fstyle = FSTYLE_NONE;
  *fwidth = SPECIAL_VALUE;
  switch (*(format_txt + pos))
    {
    case FLEFT_CHAR:
      *is_lformat = TRUE;
      pos++;
      break;
    case FCENTER_CHAR:
      *is_cformat = TRUE;
      /* Fallthrough. */
    case FRIGHT_CHAR:
      pos++;
      break;
    default:
      return (old_pos);
    }
  if (*(format_txt + pos) == FSIGN_CHAR)
    {
      pos++;
      *is_sign = TRUE;
    }
  if (*(format_txt + pos) == FLZERO_CHAR)
    {
      pos++;
      *is_lzero = TRUE;
    }
  if (isdigit (*(format_txt + pos)))
    {
      if (*(format_txt + pos) != FLZERO_CHAR)
	{
	  *fwidth = atoi (format_txt + pos);
	  if (*fwidth > FWIDTH_MAX)
	    *fwidth = i = FWIDTH_MAX;
	  else
	    i = *fwidth;
	  if (!i)
	    len = 1;
	  else
	    {
	      len = 0;
	      while (i)
		{
		  len++;
		  i /= 10;
		}
	    }
	  pos += len;
	  if (toupper (*(format_txt + pos)) == toupper (FUPPER_CHAR))
	    {
	      pos++;
	      *fstyle = FSTYLE_UPPER;
	    }
	  if (toupper (*(format_txt + pos)) == toupper (FLOWER_CHAR))
	    {
	      if (*fstyle == FSTYLE_NONE)
		{
		  pos++;
		  *fstyle = FSTYLE_LOWER;
		}
	      else
		*fwidth = SPECIAL_VALUE;
	    }
	  if (toupper (*(format_txt + pos)) == toupper (FWORD_CHAR))
	    {
	      if (*fstyle == FSTYLE_NONE)
		{
		  pos++;
		  *fstyle = FSTYLE_WORD;
		}
	      else
		*fwidth = SPECIAL_VALUE;
	    }
	  if (*(format_txt + pos) == FSUFFIX_CHAR)
	    {
	      pos++;
	      *is_suffix = TRUE;
	    }
	  switch (*(format_txt + pos))
	    {
	    case FFIX_CHAR:
	      *is_fformat = TRUE;
	      /* Fallthrough. */
	    case FVAR_CHAR:
	      pos++;
	      break;
	    default:
	      *fwidth = SPECIAL_VALUE;
	    }
	}
      else
	*fwidth = SPECIAL_VALUE;
    }
  else
    *fwidth = SPECIAL_VALUE;
  if (*fwidth == SPECIAL_VALUE)
    {
      *is_cformat = (*is_lformat) = (*is_sign) = (*is_lzero) = (*is_suffix) =
	(*is_fformat) = FALSE;
      *fstyle = FSTYLE_NONE;
      return (old_pos);
    }

  return (pos);
}



int
use_format (result_txt, pos, string, value, is_numeric, is_cformat,
	    is_lformat, is_sign, is_lzero, is_suffix, is_fformat, fstyle,
	    fwidth, extra_len)
     char **result_txt;
     const int pos;
     const char *string;
     const long value;
     const Bool is_numeric;
     const Bool is_cformat;
     const Bool is_lformat;
     const Bool is_sign;
     const Bool is_lzero;
     const Bool is_suffix;
     const Bool is_fformat;
     const int fstyle;
     const int fwidth;
     const int extra_len;
/*!
   Formats either the text `string' points to, or the number stored in `value'
     and returns the effective length of the text formatted.
     If `is_numeric' is set to TRUE, `value' is formatted, otherwise `string'.
     If `extra_len' is not set to zero and this value is greater than the
     computed length of `value' resp., `string', this value is taken instead
     of the computed length of `value' resp., `string'.  The formatted result
     is stored in `&result_txt', which must be allocated and "filled" properly
     by the caller.  "Filled" either means `&result_txt' contains a ZERO
     terminated text at function startup or a previous "*result_txt = '\0';"
     statement was performed by the caller before entering this function.
     `pos' is the zero based index at which this function adds/inserts
     text into the `&result_txt'.
*/
{
  register int i = (int) strlen (string);
  register int j;
  register int n;
  register int len = (int) strlen (*result_txt);
  register int suffix_len = 0;
  auto char *buf;
  auto char *tmp;
  auto char *ptr_char;
  auto Bool is_translated;


  j = len + fwidth;
  if (!is_fformat && (i > fwidth))
    j = len + i;
  if (is_suffix)
    j += len_suffix_max;
  if ((Uint) j >= maxlen_max)
    resize_all_strings ((Uint) j + 1, FALSE, __FILE__, (long) __LINE__);
  if (is_numeric)
    {
      auto long num = value;


      if (!num)
	len = 1;
      else
	{
	  len = 0;
	  if (is_sign || (num < 0L))
	    len++;
	  while (num)
	    {
	      len++;
	      num /= 10L;
	    }
	}
      if (is_lzero && (len < fwidth))
	len = fwidth;
      if (is_suffix)
	len += len_suffix_max;
      buf = (char *) my_malloc (len + 1, ERR_NO_MEMORY_AVAILABLE,
				__FILE__, ((long) __LINE__) - 1L, "buf", 0);
      if (is_sign)
	{
	  if (is_lzero)
	    sprintf (buf, "%+0*ld", fwidth, value);
	  else
	    sprintf (buf, "%+ld", value);
	}
      else
	{
	  if (is_lzero)
	    sprintf (buf, "%0*ld", fwidth, value);
	  else
	    sprintf (buf, "%ld", value);
	}
      if (is_suffix)
	{
	  tmp = (char *) day_suffix ((int) value);
	  suffix_len = (int) strlen (tmp);
	  strcat (buf, tmp);
	}
    }
  else
    {
      buf = (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE,
				__FILE__, ((long) __LINE__) - 1L, "buf", 0);
      strcpy (buf, string);
    }
  j = len = (int) strlen (buf);
  switch (fstyle)
    {
    case FSTYLE_UPPER:
      for (i = 0; i < len; i++)
	{
	  buf[i] = (char) toupper (buf[i]);
	  if (!isupper (buf[i]) && (translate_string != (char *) NULL))
	    {
	      ptr_char = translate_string + 1;
	      while (*ptr_char)
		{
		  if (*ptr_char == buf[i])
		    {
		      buf[i] = *--ptr_char;
		      break;
		    }
		  ptr_char++;
		  if (*ptr_char)
		    ptr_char++;
		}
	    }
	}
      break;
    case FSTYLE_LOWER:
      for (i = 0; i < len; i++)
	{
	  buf[i] = (char) tolower (buf[i]);
	  if (!islower (buf[i]) && (translate_string != (char *) NULL))
	    {
	      ptr_char = translate_string;
	      while (*ptr_char)
		{
		  if (*ptr_char == buf[i])
		    {
		      buf[i] = *++ptr_char;
		      break;
		    }
		  ptr_char++;
		  if (*ptr_char)
		    ptr_char++;
		}
	    }
	}
      break;
    case FSTYLE_WORD:
      *buf = (char) toupper (*buf);
      if (translate_string == (char *) NULL)
	for (i = 1; i < len; i++)
	  {
	    if (!isalpha (buf[i - 1]))
	      buf[i] = (char) toupper (buf[i]);
	    else
	      buf[i] = (char) tolower (buf[i]);
	  }
      else
	{
	  is_translated = FALSE;
	  if (!isupper (*buf))
	    {
	      ptr_char = translate_string + 1;
	      while (*ptr_char)
		{
		  if (*ptr_char == *buf)
		    {
		      *buf = *--ptr_char;
		      is_translated = TRUE;
		      break;
		    }
		  if (*(ptr_char - 1) == *buf)
		    {
		      is_translated = TRUE;
		      break;
		    }
		  ptr_char++;
		  if (*ptr_char)
		    ptr_char++;
		}
	    }
	  for (i = 1; i < len; i++)
	    {
	      if (!isalpha (buf[i - 1]) && !is_translated)
		{
		  buf[i] = (char) toupper (buf[i]);
		  if (!isupper (buf[i]))
		    {
		      ptr_char = translate_string + 1;
		      while (*ptr_char)
			{
			  if (*ptr_char == buf[i])
			    {
			      buf[i] = *--ptr_char;
			      is_translated = TRUE;
			      break;
			    }
			  if (*(ptr_char - 1) == buf[i])
			    {
			      is_translated = TRUE;
			      break;
			    }
			  ptr_char++;
			  if (*ptr_char)
			    ptr_char++;
			}
		    }
		  else if (isupper (buf[i]))
		    is_translated = TRUE;
		}
	      else
		{
		  buf[i] = (char) tolower (buf[i]);
		  if (!islower (buf[i]))
		    {
		      ptr_char = translate_string;
		      while (*ptr_char)
			{
			  if (*ptr_char == buf[i])
			    {
			      buf[i] = *++ptr_char;
			      break;
			    }
			  ptr_char++;
			  if (*ptr_char == buf[i])
			    break;
			  if (*ptr_char)
			    ptr_char++;
			}
		      if (!*ptr_char)
			is_translated = FALSE;
		    }
		}
	    }
	}
      break;
    default:
      ;				/* Void, nothing to do! */
    }
  if (!is_numeric && extra_len && (len != extra_len))
    len = extra_len;
  if (is_cformat)
    {
      if (len < fwidth)
	{
	  n = fwidth - len;
	  n >>= 1;
	  tmp = (char *) my_malloc (n + 2, ERR_NO_MEMORY_AVAILABLE,
				    __FILE__, ((long) __LINE__) - 1L,
				    "tmp", 0);
	  for (i = 0; i < n; i++)
	    tmp[i] = ' ';
	  tmp[i] = '\0';
	  strcat (*result_txt, tmp);
	  strcat (*result_txt, buf);
	  if ((fwidth - len) % 2)
	    {
	      tmp[i] = ' ';
	      tmp[i + 1] = '\0';
	    }
	  strcat (*result_txt, tmp);
	  free (tmp);
	  j = fwidth;
	}
      else
	{
	  if (is_fformat)
	    {
	      if (is_numeric)
		{
		  if (is_suffix)
		    {
		      i = len - fwidth - suffix_len;
		      if (i <= 0)
			strncpy (*result_txt + pos, buf, fwidth + suffix_len);
		      else
			strncpy (*result_txt + pos,
				 buf + len - fwidth - suffix_len,
				 fwidth + suffix_len);
		      *(*result_txt + pos + fwidth + suffix_len) = '\0';
		    }
		  else
		    {
		      strncpy (*result_txt + pos, buf + len - fwidth, fwidth);
		      *(*result_txt + pos + fwidth) = '\0';
		      j = fwidth;
		    }
		}
	      else
		{
		  strncpy (*result_txt + pos, buf, fwidth);
		  *(*result_txt + pos + fwidth) = '\0';
		  j = fwidth;
		}
	    }
	  else
	    strcat (*result_txt, buf);
	}
    }
  else if (is_lformat)
    {
      if (is_fformat && (len > fwidth))
	{
	  if (is_numeric)
	    {
	      if (is_suffix)
		{
		  i = len - fwidth - suffix_len;
		  if (i <= 0)
		    strncpy (*result_txt + pos, buf, fwidth + suffix_len);
		  else
		    strncpy (*result_txt + pos,
			     buf + len - fwidth - suffix_len,
			     fwidth + suffix_len);
		  *(*result_txt + pos + fwidth + suffix_len) = '\0';
		}
	      else
		{
		  strncpy (*result_txt + pos, buf + len - fwidth, fwidth);
		  *(*result_txt + pos + fwidth) = '\0';
		  j = fwidth;
		}
	    }
	  else
	    {
	      strncpy (*result_txt + pos, buf, fwidth);
	      *(*result_txt + pos + fwidth) = '\0';
	      j = fwidth;
	    }
	}
      else
	{
	  strcat (*result_txt, buf);
	  if (len < fwidth)
	    {
	      n = fwidth - len;
	      tmp = (char *) my_malloc (n + 1, ERR_NO_MEMORY_AVAILABLE,
					__FILE__, ((long) __LINE__) - 1L,
					"tmp", 0);
	      for (i = 0; i < n; i++)
		tmp[i] = ' ';
	      tmp[i] = '\0';
	      strcat (*result_txt, tmp);
	      free (tmp);
	      j = fwidth;
	    }
	}
    }
  else
    {
      if (is_fformat && (len > fwidth))
	{
	  if (is_numeric)
	    {
	      if (is_suffix)
		{
		  i = len - fwidth - suffix_len;
		  if (i <= 0)
		    strncpy (*result_txt + pos, buf, fwidth + suffix_len);
		  else
		    strncpy (*result_txt + pos,
			     buf + len - fwidth - suffix_len,
			     fwidth + suffix_len);
		  *(*result_txt + pos + fwidth + suffix_len) = '\0';
		}
	      else
		{
		  strncpy (*result_txt + pos, buf + len - fwidth, fwidth);
		  *(*result_txt + pos + fwidth) = '\0';
		  j = fwidth;
		}
	    }
	  else
	    {
	      strncpy (*result_txt + pos, buf, fwidth);
	      *(*result_txt + pos + fwidth) = '\0';
	      j = fwidth;
	    }
	}
      else
	{
	  if (len < fwidth)
	    {
	      n = fwidth - len;
	      tmp = (char *) my_malloc (n + 1, ERR_NO_MEMORY_AVAILABLE,
					__FILE__, ((long) __LINE__) - 1L,
					"tmp", 0);
	      for (i = 0; i < n; i++)
		tmp[i] = ' ';
	      tmp[i] = '\0';
	      strcat (*result_txt, tmp);
	      free (tmp);
	      j = fwidth;
	    }
	  strcat (*result_txt, buf);
	}
    }
  free (buf);

  return (j);
}



void
print_all_holidays (init_data, detected)
     Bool init_data;
     const Bool detected;
/*!
   Generate and display all holidays (global `hd_table') in sorted manner.
*/
{
  auto const Cc_struct *ptr_cc;
  register int easter;
  register int i = 0;
  register int j = 0;
  register int k;
  register int fday = 0;
  register int count = (fiscal_month > MONTH_MIN) ? 2 : 1;
  auto int hd_elems = 0;
  auto char *ptr_char;


  if (init_data || detected)
    {
      if (init_data)
	for (i = 0; i < HD_MAX; i++)
	  hd_ldays[i] = hd_mdays[i] = '\0';
      if (fiscal_month > MONTH_MIN)
	{
	  fday = day_of_year (DAY_MIN, fiscal_month, year);
	  j = is_leap_year;
	  i = year;
	}
      do
	{
	  count--;
	  easter = knuth_easter_formula (year);
	  /*
	     Use the global holiday management functions.
	   */
	  if (hdy_astronomical)
	    astronomical_hdy (&init_data, detected, year, &hd_elems, fday,
			      count);
	  if (hdy_bahai)
	    bahai_hdy (&init_data, detected, year, &hd_elems, fday, count,
		       NULL);
	  if (hdy_celtic)
	    celtic_hdy (&init_data, detected, year, &hd_elems, fday, count,
			NULL);
	  if (hdy_chinese_flexible)
	    chinese_hdy (&init_data, detected, year, &hd_elems, fday, count,
			 NULL, TRUE);
	  if (hdy_chinese)
	    chinese_hdy (&init_data, detected, year, &hd_elems, fday, count,
			 NULL, FALSE);
	  if (hdy_christian)
	    christian_hdy (&init_data, detected, easter, year, &hd_elems,
			   fday, count);
	  if (hdy_hebrew)
	    hebrew_hdy (&init_data, detected, year, &hd_elems, fday, count,
			NULL);
	  if (hdy_islamic)
	    islamic_hdy (&init_data, detected, year, &hd_elems, fday, count,
			 NULL);
	  if (hdy_japanese_flexible)
	    japanese_hdy (&init_data, detected, year, &hd_elems, fday, count,
			  NULL, TRUE);
	  if (hdy_japanese)
	    japanese_hdy (&init_data, detected, year, &hd_elems, fday, count,
			  NULL, FALSE);
	  if (hdy_multicultural_new_year)
	    multicultural_new_year_hdy (&init_data, detected, year, &hd_elems,
					fday, count);
	  if (hdy_orthodox_new)
	    orthodox_hdy (&init_data, detected, easter, year, &hd_elems, fday,
			  count, NULL, TRUE);
	  if (hdy_orthodox_old)
	    orthodox_hdy (&init_data, detected, easter, year, &hd_elems, fday,
			  count, NULL, FALSE);
	  if (hdy_persian)
	    persian_hdy (&init_data, detected, year, &hd_elems, fday, count,
			 NULL);
	  if (hdy_zodiacal_marker)
	    zodiacal_marker_hdy (&init_data, detected, year, &hd_elems, fday,
				 count);
	  if (mth_bahai)
	    bahai_mth (&init_data, detected, year, &hd_elems, fday, count);
	  if (mth_chinese_flexible)
	    chinese_mth (&init_data, detected, year, &hd_elems, fday, count,
			 TRUE, TRUE);
	  if (mth_chinese)
	    chinese_mth (&init_data, detected, year, &hd_elems, fday, count,
			 FALSE, TRUE);
	  if (mth_coptic)
	    coptic_mth (&init_data, detected, year, &hd_elems, fday, count,
			TRUE);
	  if (mth_ethiopic)
	    coptic_mth (&init_data, detected, year, &hd_elems, fday, count,
			FALSE);
	  if (mth_french_revolutionary)
	    french_revolutionary_mth (&init_data, detected, year, &hd_elems,
				      fday, count);
	  if (mth_hebrew)
	    hebrew_mth (&init_data, detected, year, &hd_elems, fday, count);
	  if (mth_indian_civil)
	    indian_civil_mth (&init_data, detected, year, &hd_elems, fday,
			      count);
	  if (mth_islamic)
	    islamic_mth (&init_data, detected, year, &hd_elems, fday, count);
	  if (mth_japanese_flexible)
	    chinese_mth (&init_data, detected, year, &hd_elems, fday, count,
			 TRUE, FALSE);
	  if (mth_japanese)
	    chinese_mth (&init_data, detected, year, &hd_elems, fday, count,
			 FALSE, FALSE);
	  if (mth_old_armenic)
	    old_egyptic_mth (&init_data, detected, year, &hd_elems, fday,
			     count, FALSE);
	  if (mth_old_egyptic)
	    old_egyptic_mth (&init_data, detected, year, &hd_elems, fday,
			     count, TRUE);
	  if (mth_persian)
	    persian_mth (&init_data, detected, year, &hd_elems, fday, count);
	  if (cc != (char *) NULL)
	    {
	      /*
	         Use the country specific holiday management functions.
	       */
	      ptr_char = cc;
	      do
		{
		  /*
		     Copy a single country code id into `s1'.
		   */
		  k = 0;
		  while (*ptr_char && (*ptr_char != *CONNECT_SEP))
		    s1[k++] = *ptr_char++;
		  s1[k] = '\0';
		  ptr_cc = binsearch_cc_id (s1);
		  if (ptr_cc == (Cc_struct *) NULL)
		    /*
		       This case MUST be an internal error (`cc_holidays[]' corrupt')!
		     */
		    abort ();
		  else
		    (*ptr_cc->Cc_hdy_handler) (&init_data, detected, easter,
					       year, &hd_elems, fday, count);
		  /*
		     Now skip a possibly trailing CONNECT_SEP.
		   */
		  if (*ptr_char)
		    ptr_char++;
		}
	      while (*ptr_char);
	    }
	  if (fiscal_month > MONTH_MIN)
	    {
	      year++;
	      is_leap_year = (days_of_february (year) == 29);
	    }
	}
      while (count);
      if (fiscal_month > MONTH_MIN)
	{
	  is_leap_year = j;
	  year = i;
	}
    }
  if (hd_elems && holiday_flag)
    {
      /*
         At last, sort the eternal holiday dates only if they are not presorted.
       */
      if (hd_elems > 1)
	{
	  if (!is_presorted (hd_table, hd_elems))
	    {
	      if (hd_sort_des_flag)
		qsort ((VOID_PTR) hd_table, hd_elems, sizeof *hd_table,
		       (Cmp_func) des_sort);
	      else
		qsort ((VOID_PTR) hd_table, hd_elems, sizeof *hd_table,
		       (Cmp_func) asc_sort);
	    }
	  else if (hd_sort_des_flag)
	    /*
	       `hd_table[]' is presorted and must be shown in descending sort order,
	       rearrange its internal sort order from ascending to descending sort order.
	     */
	    reverse_order (hd_table, hd_elems);
	}
    }
  else if (!detected)
    {
      register int n;
      register int lym;
      register int m2 = fiscal_month + 1;
      register int m3 = fiscal_month + 2;
      register int tmp_month = month;
      auto Bool hd_title_shown = FALSE;
      auto Bool tmp_ify = is_fiscal_year;


      /*
         Display leading title text for a current year.
       */
      if (hd_title_flag
#if USE_RC
	  && !is_1month_mode && !is_2month_mode
#endif
	  && ((!is_3month_mode
	       && !is_3month_mode2
	       && ((!is_fiscal_year
		    && !month)
		   || (is_fiscal_year
		       && (fiscal_month == MONTH_MIN))))
	      || is_3month_mode2
	      || (is_3month_mode && (fiscal_month < MONTH_MAX - 1))))
	{
	  /*
	     Now display a leading NEWLINE character before the title text
	     only if not explicitly suppressed by a `-G' command line
	     option given.
	   */
	  *s1 = '\0';
	  if (!hd_suppr_list_sep_flag)
	    print_text (stdout, s1);
	  if (hd_title_flag)
	    {
	      /*
	         Now display the leading title text only if not explicitly
	         suppressed by a `-X' command line option given.
	       */
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
	      i = (int) strlen (_("Eternal holiday list"));
	      if (use_year_zeroleaded)
		{
		  if (is_leap_year)
		    sprintf (s1, _("%s:%*sThe year %0*d is A leap year"),
			     _("Eternal holiday list"), LEN_HD_NAME - i + 2,
			     "", lym, n);
		  else
		    sprintf (s1, _("%s:%*sThe year %0*d is NO leap year"),
			     _("Eternal holiday list"), LEN_HD_NAME - i + 2,
			     "", lym, n);
		}
	      else
		{
		  if (is_leap_year)
		    sprintf (s1, _("%s:%*sThe year %d is A leap year"),
			     _("Eternal holiday list"), LEN_HD_NAME - i + 2,
			     "", n);
		  else
		    sprintf (s1, _("%s:%*sThe year %d is NO leap year"),
			     _("Eternal holiday list"), LEN_HD_NAME - i + 2,
			     "", n);
		}
	      print_text (stdout, s1);
	      if (hd_table[0] != (char *) NULL)
		print_text (stdout, s1);
	    }
	  hd_title_shown = TRUE;
	}
      /*
         Now display the (complete) eternal holiday list.
       */
      if (is_3month_mode
#if USE_RC
	  || is_1month_mode || is_2month_mode
#endif
	  || is_3month_mode2)
	{
	  /*
	     Set the range of the 2|3 months, which have to be respected.
	   */
	  if (fiscal_month >= MONTH_MAX - 1)
	    {
	      m3 = MONTH_MIN;
	      if (fiscal_month == MONTH_MAX)
		{
		  m2 = MONTH_MIN;
		  m3++;
		}
	    }
	  is_fiscal_year = FALSE;
	  month = fiscal_month;
	}
      i = j = 0;
      while ((hd_table[i] != (char *) NULL) && (i < HD_ELEMS_MAX))
	{
	  /*
	     One month resp., 2|3-month calendar mode:
	     Get holiday month of holiday text.
	   */
	  if (is_3month_mode
#if USE_RC
	      || is_1month_mode || is_2month_mode
#endif
	      || is_3month_mode2 || (month && !is_fiscal_year))
	    {
	      s1[0] = hd_table[i][len_year_max];
	      s1[1] = hd_table[i][len_year_max + 1];
	      s1[2] = '\0';
	      j = my_atoi (s1);
	    }
	  /*
	     Display dates of eternal holiday list:
	     Year calendar  --> Display all eternal holidays.
	     Month calendar --> Display only those eternal holidays
	     which refer to current month.
	   */
	  if (is_fiscal_year || !month || month == j
#if USE_RC
	      || (is_1month_mode
		  && (month == j)) || (is_2month_mode && (j == m2))
#endif
	      || ((is_3month_mode
		   || is_3month_mode2) && (j == m2 || j == m3)))
	    {
	      if (!hd_legal_days_only
		  || (hd_legal_days_only
		      &&
		      (*(*(hd_table + i) + LEN_HD_NAME + len_year_max + 4 + 2)
		       != *DIS_HLS_PREF)
		      &&
		      (*(*(hd_table + i) + LEN_HD_NAME + len_year_max + 4 + 2)
		       != *DIS_HLS_PREF2)))
		{
		  /*
		     Month calendar mode:
		     Display a leading NEWLINE character only in case
		     an eternal holiday refers to the current month.
		   */
		  if (!hd_title_shown)
		    {
		      /*
		         Now display a leading NEWLINE character before the
		         title text only if not explicitly suppressed by
		         a `-G' command line option given.
		       */
		      *s1 = '\0';
		      if (!hd_suppr_list_sep_flag)
			print_text (stdout, s1);
		      if (hd_title_flag)
			{
			  /*
			     Now display the leading title text only if not
			     explicitly suppressed by a `-X' command line
			     option given.
			   */
			  sprintf (s1, "%s:", _("Eternal holiday list"));
			  print_text (stdout, s1);
			  print_text (stdout, s1);
			}
		      hd_title_shown = TRUE;
		    }
		  /*
		     Display the eternal holiday text.
		   */
		  sprintf (s1, "%s", *(hd_table + i) + len_year_max + 4 + 1);
		  print_text (stdout, s1);
		}
	    }
	  free (hd_table[i]);
	  hd_table[i++] = (char *) NULL;
	}
      if (is_3month_mode
#if USE_RC
	  || is_1month_mode || is_2month_mode
#endif
	  || is_3month_mode2)
	{
	  month = tmp_month;
	  is_fiscal_year = tmp_ify;
	}
    }
}



void
holiday (init_data, detected, holiday_name, country_code, holiday_prefix,
	 day, month, year, hd_elems, fday, count)
     Bool init_data;
     const Bool detected;
     const char *holiday_name;
     const char *country_code;
     const char *holiday_prefix;
     int day;
     int month;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Generates a single holiday date text in formatted manner and stores the
     result into global `hd_table[]'.
     ONLY if `holiday_prefix' is a '-' (DIS_HLS_PREF) or '*' (DIS_HLS_PREF2)
     character, this holiday is not stored into global `hd_ldays' and
     will not be highlighted in the generated list, but is stored into
     global `hd_mdays'!
     If `holiday_prefix' is a '#' character, this marks the case the
     holiday is only valid in some parts of the country, but is stored
     into global `hd_ldays' and will be highlighted in the generated list.
     If `holiday_prefix' is a '\0' or any other character, the same rules
     concering to '#' character applies.
     If `month' is set to zero, it's assumed that the delivered date is a
     date relative to the Easter Sunday's date (`day' contains the according
     day_of_year number), otherwise the date is an absolute date of the `year'.
*/
{
  register int i = day;
  register int hd;
  register int hd2;


  if (!month)
    (void) doy2date (i, is_leap_year, &day, &month);
  if (month == 2)
    i = is_leap_year;
  else
    i = 0;
  hd = hd2 = day_of_year (day, month, year);
  if ((year >= EASTER_MIN)
      && (year <= EASTER_MAX)
      && (day >= DAY_MIN)
      && (day <= dvec[month - 1] + i)
      && (!fday
	  || (count
	      && (hd >= fday))
	  || (!count && (hd < fday - (days_of_february (year - 1) == 29)))))
    {
      if (holiday_flag
#if USE_RC
	  || rc_enable_hda_flag || rc_enable_hdl_flag
#endif
	)
	{
	  register int d = day;


	  sprintf (s1, " (%s)", country_code);
	  i = (int) strlen (s1);
	  strncpy (s2, holiday_name, LEN_HD_NAME - i);
	  s2[LEN_HD_NAME - i - 1] = '\0';
	  strcat (s2, s1);
	  sprintf (s1, "%0*d%02d%02d %-*s  %1s ",
		   len_year_max, year, month, day, LEN_HD_NAME - 1, s2,
		   holiday_prefix);
	  strcpy (s2, s1);
	  i =
	    hd -
	    ((cal_special_flag) ? act_day :
	     day_of_year (act_day, act_month, act_year));
	  if (!hd_both_dates_flag)
	    {
	      if (!hd_special_flag)
		hd2 = 0;
	      else
		d = 0;
	    }
	  if (!i && (year == act_year))
	    (void) decode_date_format (date_format->df_format, &s2, d, month,
				       year, hd2, TRUE, FALSE, TRUE);
	  else if ((*holiday_prefix != *DIS_HLS_PREF)
		   && (*holiday_prefix != *DIS_HLS_PREF2))
	    (void) decode_date_format (date_format->df_format, &s2, d, month,
				       year, hd2, FALSE, TRUE, TRUE);
	  else
	    (void) decode_date_format (date_format->df_format, &s2, d, month,
				       year, hd2, FALSE, FALSE, TRUE);
	  /*
	     Add the day displacement, which is relative to the "actual date"
	     to the eternal holiday text.
	   */
	  if ((fday
	       && (((year == act_year - 1)
		    && (month >= fiscal_month))
		   || ((year == act_year + 1)
		       && (month < fiscal_month)))) || year == act_year)
	    {
	      if (fday && (year != act_year))
		{
		  if (year == act_year - 1)
		    /*
		       Eternal holiday to compute is in "actual year - 1".
		     */
		    i = hd - (DAY_LAST + is_leap_year
			      +
			      ((cal_special_flag) ? act_day :
			       day_of_year (act_day, act_month, act_year)));
		  else
		    /*
		       Eternal holiday to compute is in "actual year + 1".
		     */
		    i = hd + (DAY_LAST + (days_of_february (act_year) == 29)
			      -
			      ((cal_special_flag) ? act_day :
			       day_of_year (act_day, act_month, act_year)));
		}
	      if (i)
		{
		  if (abs (i) == 1)
		    sprintf (s1, _(" = %+4d day"), i);
		  else
		    sprintf (s1, _(" = %+4d days"), i);
		  strcat (s2, s1);
		}
	    }
	  /*
	     Store the constructed "raw" line in `hd_table[]'.
	   */
	  if (*hd_elems < HD_ELEMS_MAX)
	    {
	      hd_table[*hd_elems] = (char *) my_malloc (strlen (s2) + 1,
							ERR_NO_MEMORY_AVAILABLE,
							__FILE__,
							((long) __LINE__) -
							2L,
							"hd_table[hd_elems]",
							*hd_elems);
	      strcpy (hd_table[(*hd_elems)++], s2);
	    }
	}
      /*
         This case can occur only, if the code was extended by more
         eternal holidays and the table size for `hd_table[]', namely
         HD_ELEMS_MAX, isn't increased/adjusted correctly so this
         warning/informational is for maintainers only!!
       */
      if ((warning_level >= 0) && (*hd_elems >= HD_ELEMS_MAX))
	{
	  sprintf (s1, _("%s%s contains %d maximum entries now!%s"),
		   ((ehls1s.len != 1) ? ehls1s.seq : ""),
		   _("Eternal holiday list"), HD_ELEMS_MAX,
		   ((ehls1s.len != 1) ? ehls1e.seq : ""));
	  print_text (stderr, s1);
	}
      if (init_data)
	for (i = 0; i < HD_MAX; i++)
	  hd_ldays[i] = hd_mdays[i] = '\0';
      if (detected)
	{
	  if (*holiday_prefix == *DIS_HLS_PREF
	      || *holiday_prefix == *DIS_HLS_PREF2)
	    hd_mdays[((month - 1) * MONTH_LAST) + (day - 1)] = '@';
	  else
	    hd_ldays[((month - 1) * MONTH_LAST) + (day - 1)] = '@';
	}
    }
  else if (init_data)
    for (i = 0; i < HD_MAX; i++)
      hd_ldays[i] = hd_mdays[i] = '\0';
}
