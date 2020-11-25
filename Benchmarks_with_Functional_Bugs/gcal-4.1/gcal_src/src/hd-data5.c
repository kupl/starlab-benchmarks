/*!
*  \file hd-data5.c
*  \brief All other T-Z countries data which is composed into the
*               eternal holiday list.
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
*  Include definition header file to see whether HD_TOP20CC is defined there.
*    Compile this module only if HD_TOP20CC is NOT defined, otherwise skip it.
*/
#include "tailor.h"



#if !HD_TOP20CC

/*
*  Include header files.
*/
# include "common.h"
# include "globals.h"
# include "hd-defs.h"
# include "hd-data.h"
# include "hd-data1.h"
# include "hd-data2.h"
# include "hd-data3.h"
# include "hd-data4.h"
# include "hd-use.h"
# include "utils.h"
# include "hd-data5.h"



/*
*  All other T-Z countries specific holiday management functions.
*/
void
tc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Turks and Caicos Islands.
*/
{
  register int day;


  ptr_cc_id = "TC";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   30, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HUMAN_RIGHTS_DAY].ht_text),
	   ptr_cc_id, "+", 10, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
	   ptr_cc_id, "+", dvec[9 - 1], 9, year, hd_elems, fday, count);
}



void
td_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Chad.
*/
{
  ptr_cc_id = "TD";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 11, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 13, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 28, 11, year, hd_elems, fday, count);
}



void
tg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Togo.
*/
{
  ptr_cc_id = "TG";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_ECONOMIC_LIBERATION_DAY].ht_text), ptr_cc_id, "+", 24,
	   MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 27, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 13, MONTH_MIN, year, hd_elems, fday, count);
}



void
th_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Thailand.
*/
{
  ptr_cc_id = "TH";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  if (year > 1887)
    {
      register int day;
      register int i;


      day = 6;
      i = weekday_of_date (day, 4, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_KING_RAMA_I_MEMORIAL_DAY].ht_text), ptr_cc_id,
	       "+", day, 4, year, hd_elems, fday, count);
      day = 23;
      i = weekday_of_date (day, 10, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_KING_RAMA_V_MEMORIAL_DAY].ht_text), ptr_cc_id,
	       "+", day, 10, year, hd_elems, fday, count);
      day = 10;
      i = weekday_of_date (day, MONTH_MAX, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	       "+", day, MONTH_MAX, year, hd_elems, fday, count);
      day = 5;
      i = weekday_of_date (day, 5, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_CORONATION_DAY].ht_text),
	       ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
      day = 5;
      i = weekday_of_date (day, MONTH_MAX, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text), ptr_cc_id, "+", day,
	       MONTH_MAX, year, hd_elems, fday, count);
      day = DAY_MIN;
      i = weekday_of_date (day, 5, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, day, 5, year, hd_elems, fday, count);
      day = DAY_MIN;
      i = weekday_of_date (day, 7, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_MID_YEAR_FESTIVAL].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, day, 7, year, hd_elems, fday, count);
      day = 12;
      i = weekday_of_date (day, 8, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	       day, 8, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	       ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	       fday, count);
      /*
         Calculate the year number used in the Thai ***CIVIL***
         (western oriented) calendar based on the Buddhist Era.
       */
      sprintf (s2, "%s %d", _(hd_text[HD_THAI_NEW_YEARS_DAY].ht_text),
	       year + 543);
      day = DAY_MIN;
      if (year > 1939)
	{
	  i = weekday_of_date (day, MONTH_MIN, year);
	  if (i > 5)
	    day += ((DAY_MAX - i) + 1);
	  i = MONTH_MIN;
	}
      else
	i = 4;
      holiday (*init_data, detected, s2, ptr_cc_id,
	       "+", day, i, year, hd_elems, fday, count);
      if (year > 1888)
	{
	  day = eval_holiday (DAY_MIN, MONTH_MIN, year, 6, TRUE) + DAY_MAX;
	  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, day, MONTH_MIN, year, hd_elems,
		   fday, count);
	}
      if (year > 1994)
	{
	  day = 14;
	  i = weekday_of_date (day, 4, year);
	  if (i > 5)
	    day += ((DAY_MAX - i) + 1);
	  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, day, 4, year, hd_elems, fday,
		   count);
	}
    }
}



void
tj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tajikistan.
*/
{
  ptr_cc_id = "TJ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if ((year > 1916) && (year < 1991))
    {
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
		 ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
    }
  if (year > 1990)
    {
      holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	       ptr_cc_id, "+", 23, 2, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", 9, 9, year, hd_elems, fday, count);
    }
}



void
tk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tokelau.
*/
{
  ptr_cc_id = "TK";
  use_other_cc = !use_other_cc;
  nz_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
tm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Turkmenistan.
*/
{
  register int i;
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "TM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if ((year > 1916) && (year < 1991))
    {
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
		 ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
    }
  if (year > 1990)
    {
      holiday (*init_data, detected,
	       _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	       "+", 18, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	       ptr_cc_id, "+", 19, 2, year, hd_elems, fday, count);
      for (i = 27; i <= 28; i++)
	holiday (*init_data, detected,
		 _(hd_text[HD_INDEPENDENCE_DAY].ht_text), ptr_cc_id, "+", i,
		 10, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NEUTRALITY_DAY].ht_text),
	       ptr_cc_id, "+", 12, MONTH_MAX, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	       ptr_cc_id, "+", 6, 10, year, hd_elems, fday, count);
    }
  /*
     The Christians in Turkmenistan compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  if (year != 1924)
    {
      auto int m;
      auto int y;


      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
}



void
tn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tunisia.
*/
{
  ptr_cc_id = "TN";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 20, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 9, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 7, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 18, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 13, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
	   ptr_cc_id, "+", 21, 3, year, hd_elems, fday, count);
  if (year > 1933)
    holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	     ptr_cc_id, "+", 3, 9, year, hd_elems, fday, count);
}



void
to_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tonga.
*/
{
  ptr_cc_id = "TO";
  holiday (*init_data, detected,
	   _(hd_text[HD_THE_CROWN_PRINCES_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   4, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_TOPOU_I_DAY].ht_text),
	   ptr_cc_id, "+", 4, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ANZAC_DAY].ht_text),
	   ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 4,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 4, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 4, 7, year, hd_elems, fday, count);
}



void
tr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Turkey.
*/
{
  ptr_cc_id = "TR";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ATATURK_MEMORIAL_DAY].ht_text),
	   ptr_cc_id, "+", 19, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 23, 4, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 29, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 30, 8, year, hd_elems, fday, count);
}



void
tt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Trinidad and Tobago.
*/
{
  register int i;


  ptr_cc_id = "TT";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  for (i = 2; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDIAN_ARRIVAL_DAY].ht_text),
	   ptr_cc_id, "+", 30, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", 19, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 30, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if ((year > 1961) && (year < 1999))
    holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	     ptr_cc_id, "+", 24, 9, year, hd_elems, fday, count);
  if (year > 1975)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", dvec[8 - 1], 8, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
tv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tuvalu.
*/
{
  register int day;


  ptr_cc_id = "TV";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", 5, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", 11, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOSPEL_DAY].ht_text), ptr_cc_id,
	   "+", 13, 5, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRINCE_OF_WALES_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   14, 11, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
tw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Taiwan.
*/
{
  register int day;


  ptr_cc_id = "TW";
  use_other_cc = !use_other_cc;
  if ((year > 1894) && (year < 1945))
    japanese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
		  FALSE);
  else
    chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
		 FALSE);
  use_other_cc = !use_other_cc;
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 3, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MID_YEAR_FESTIVAL].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CHIANG_KAI_SHEKS_BIRTHDAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[10 - 1], 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_DR_SUN_YAT_SENS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 10, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 28, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 29, 3, year, hd_elems, fday, count);
}



void
tz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Tanzania.
*/
{
  ptr_cc_id = "TZ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_C_MAPINDUZI_DAY].ht_text),
	   ptr_cc_id, "+", 5, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FARMERS_DAY].ht_text),
	   ptr_cc_id, "+", 7, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 9, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 12, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_UNION_DAY].ht_text), ptr_cc_id,
	   "+", 28, 4, year, hd_elems, fday, count);
}



void
ua_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Ukraine.
*/
{
  register int i;
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "UA";
  d = 28;
  i = weekday_of_date (d, 6, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", d,
	   6, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  d = day_of_year (eval_holiday (dvec[5 - 1], 5, year, 6, FALSE), 5, year);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_KIEV_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF2, d + i, 0, year, hd_elems, fday, count);
  d = DAY_MIN;
  i = weekday_of_date (d, 5, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
  d = 2;
  i = weekday_of_date (d, 5, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
  d = DAY_MIN;
  i = weekday_of_date (d, MONTH_MIN, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", d, MONTH_MIN, year, hd_elems, fday, count);
  d = 8;
  i = weekday_of_date (d, 3, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", d, 3, year, hd_elems, fday, count);
  if (year > 1944)
    {
      d = 9;
      i = weekday_of_date (d, 5, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	       ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
    }
  if (year > 1990)
    {
      d = 24;
      i = weekday_of_date (d, 8, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", d, 8, year, hd_elems, fday, count);
    }
  /*
     The Christians in Ukraine compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  if (year != 1924)
    {
      auto int m;
      auto int y;


      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_HOLY_TRINITY].ht_text),
	     ptr_cc_id, "+", easter + 56 + i, 0, year, hd_elems, fday, count);
}



void
ug_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Uganda.
*/
{
  ptr_cc_id = "UG";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 9, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 3, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 9, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 8, 3, year, hd_elems, fday, count);
}



void
us_ak_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Alaska.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_AK";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (dvec[3 - 1], 3, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_SEWARDS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = 18;
  i = weekday_of_date (day, 10, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_ALASKA_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
}



void
us_al_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Alabama.
*/
{
  register int day;


  ptr_cc_id = "US_AL";
  observe_us_hdy = SASA_SUMO;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE);
  holiday (*init_data, detected,
	   _(hd_text[HD_JEFFERSON_DAVIS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   day, 6, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
}



void
us_ar_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Arkansas.
*/
{
  register int day;


  ptr_cc_id = "US_AR";
  observe_us_hdy = SASA_SUSU;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
}



void
us_az_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Arizona.
*/
{
  ptr_cc_id = "US_AZ";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_ca_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/California.
*/
{
  register int day;


  ptr_cc_id = "US_CA";
  observe_us_hdy = SASAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 12;
  if (weekday_of_date (day, 2, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   day, 2, year, hd_elems, fday, count);
  day = dvec[3 - 1];
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CESAR_CHAVEZ_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  day = 9;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_ADMISSION_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
}



void
us_co_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Colorado.
*/
{
  ptr_cc_id = "US_CO";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLORADO_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
}



void
us_ct_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Connecticut.
*/
{
  ptr_cc_id = "US_CT";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
us_dc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/District of Columbia.
*/
{
  ptr_cc_id = "US_DC";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  if ((year > 1964) && !((1965 - year) & 3))
    {
      register int day;


      day = 20;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_PRESIDENTIAL_INAUGURATION].ht_text), ptr_cc_id,
	       "+", day, MONTH_MIN, year, hd_elems, fday, count);
    }
}



void
us_de_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Delaware.
*/
{
  ptr_cc_id = "US_DE";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_RETURN_DAY].ht_text),
	       ptr_cc_id, "#", day + 2, 11, year, hd_elems, fday, count);
    }
}



void
us_fl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Florida.
*/
{
  ptr_cc_id = "US_FL";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
}



void
us_ga_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Georgia.
*/
{
  register int day;


  ptr_cc_id = "US_GA";
  observe_us_hdy = SASA_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
}



void
us_hi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Hawaii.
*/
{
  register int day;


  ptr_cc_id = "US_HI";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, 5, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_ADMISSION_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRINCE_KALANIANAOLE_DAY].ht_text), ptr_cc_id, "+", 26,
	   3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_KING_KAMEHAMEHA_DAY].ht_text),
	   ptr_cc_id, "+", 26, 3, year, hd_elems, fday, count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_ia_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Iowa.
*/
{
  register int day;


  ptr_cc_id = "US_IA";
  observe_us_hdy = SASA_SUSU;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
}



void
us_id_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Idaho.
*/
{
  ptr_cc_id = "US_ID";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_il_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Illinois.
*/
{
  register int day;


  ptr_cc_id = "US_IL";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE);
  holiday (*init_data, detected,
	   _(hd_text[HD_CASIMIR_PULASKIS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_in_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Indiana.
*/
{
  register int day;


  ptr_cc_id = "US_IN";
  observe_us_hdy = SASA_SUSU;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE) + 1;
  holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_ks_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Kansas.
*/
{
  ptr_cc_id = "US_KS";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_ky_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Kentucky.
*/
{
  ptr_cc_id = "US_KY";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
us_la_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Louisiana.
*/
{
  register int day;


  ptr_cc_id = "US_LA";
  observe_us_hdy = SASA_SUMO;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_ma_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Massachusetts.
*/
{
  register int day;


  ptr_cc_id = "US_MA";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 17, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BUNKER_HILL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 17, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PATRIOTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
}



void
us_md_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Maryland.
*/
{
  register int day;


  ptr_cc_id = "US_MD";
  observe_us_hdy = SASA_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 12;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_DEFENDERS_DAY].ht_text),
	   ptr_cc_id, "#", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  day = 25;
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_MARYLAND_DAY].ht_text),
	   ptr_cc_id, "#", day, 3, year, hd_elems, fday, count);
  day = 12;
  if (weekday_of_date (day, 2, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "#",
	   day, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 11, year, hd_elems, fday, count);
  if ((year > 1964) && !((1965 - year) & 3))
    {
      day = 20;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_PRESIDENTIAL_INAUGURATION].ht_text), ptr_cc_id,
	       DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday, count);
    }
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_me_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Maine.
*/
{
  register int day;


  ptr_cc_id = "US_ME";
  observe_us_hdy = SASA_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PATRIOTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
}



void
us_mi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Michigan.
*/
{
  ptr_cc_id = "US_MI";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
}



void
us_mn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Minnesota.
*/
{
  ptr_cc_id = "US_MN";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_mo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Missouri.
*/
{
  ptr_cc_id = "US_MO";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TRUMAN_DAY].ht_text), ptr_cc_id,
	   "+", 8, 5, year, hd_elems, fday, count);
}



void
us_ms_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Mississippi.
*/
{
  register int day;


  ptr_cc_id = "US_MS";
  observe_us_hdy = SASA_SUSU;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
}



void
us_mt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Montana.
*/
{
  ptr_cc_id = "US_MT";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_nc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/North Carolina.
*/
{
  ptr_cc_id = "US_NC";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
us_nd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/North Dakota.
*/
{
  ptr_cc_id = "US_ND";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
us_ne_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Nebraska.
*/
{
  register int day;


  ptr_cc_id = "US_NE";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (dvec[4 - 1], 4, year, 5, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
}



void
us_nh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/New Hampshire.
*/
{
  ptr_cc_id = "US_NH";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_nj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/New Jersey.
*/
{
  ptr_cc_id = "US_NJ";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_nm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/New Mexico.
*/
{
  ptr_cc_id = "US_NM";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_nv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Nevada.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_NV";
  observe_us_hdy = SAFR_SUMO;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  day = day_of_year (dvec[10 - 1], 10, year);
  i = weekday_of_date (dvec[10 - 1], 10, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_NEVADA_DAY].ht_text),
	   ptr_cc_id, "+", day, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
}



void
us_ny_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/New York.
*/
{
  ptr_cc_id = "US_NY";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_oh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Ohio.
*/
{
  ptr_cc_id = "US_OH";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_ok_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Oklahoma.
*/
{
  ptr_cc_id = "US_OK";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_or_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Oregon.
*/
{
  ptr_cc_id = "US_OR";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_pa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Pennsylvania.
*/
{
  ptr_cc_id = "US_PA";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", 14, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
}



void
us_ri_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Rhode Island.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_RI";
  observe_us_hdy = SAMO_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 4;
  i = weekday_of_date (day, 5, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_sc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/South Carolina.
*/
{
  ptr_cc_id = "US_SC";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1844)
    {
      register int day;


      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
    }
}



void
us_sd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/South Dakota.
*/
{
  ptr_cc_id = "US_SD";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_tn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Tennessee.
*/
{
  ptr_cc_id = "US_TN";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_tx_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Texas.
*/
{
  register int day;


  ptr_cc_id = "US_TX";
  observe_us_hdy = SASA_SUSU;
  use_other_cc = !use_other_cc;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 2, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LYNDON_B_JOHNSON_DAY].ht_text),
	   ptr_cc_id, "+", 27, 8, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SAN_JACINTO_DAY].ht_text),
	   ptr_cc_id, "+", 21, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 19, 6, year, hd_elems, fday, count);
}



void
us_ut_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Utah.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_UT";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 24;
  i = weekday_of_date (day, 7, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_PIONEER_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
}



void
us_va_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Virginia.
*/
{
  ptr_cc_id = "US_VA";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  if ((year > 1964) && !((1965 - year) & 3))
    {
      register int day;


      day = 20;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_PRESIDENTIAL_INAUGURATION].ht_text), ptr_cc_id,
	       DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday, count);
    }
}



void
us_vt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Vermont.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_VT";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 16;
  i = weekday_of_date (day, 8, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_BENNINGTON_BATTLE_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = 12;
  i = weekday_of_date (day, 2, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   day, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, 2, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_TOWN_MEETING_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
}



void
us_wa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Washington.
*/
{
  ptr_cc_id = "US_WA";
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 47, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   12, 2, year, hd_elems, fday, count);
}



void
us_wi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Wisconsin.
*/
{
  ptr_cc_id = "US_WI";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
us_wv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/West Virginia.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "US_WV";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 24;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 12;
  i = weekday_of_date (day, 2, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   day, 2, year, hd_elems, fday, count);
  day = 20;
  i = weekday_of_date (day, 6, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_WEST_VIRGINIA_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = dvec[MONTH_MAX - 1];
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i == DAY_MAX)
    i = SPECIAL_VALUE;
  else if (i == 6)
    day--;
  if (i != SPECIAL_VALUE)
    holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	     ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  if (weekday_of_date (dvec[MONTH_MAX - 1], MONTH_MAX, year - 1) == DAY_MAX)
    holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	     ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
us_wy_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States/Wyoming.
*/
{
  ptr_cc_id = "US_WY";
  observe_us_hdy = SAFR_SUMO;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
uy_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Uruguay.
*/
{
  register int i;


  ptr_cc_id = "UY";
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_LAS_PIEDRAS].ht_text),
	   ptr_cc_id, "+", 18, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARTIGAS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 19, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BLESSING_OF_THE_WATER].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 8, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EXPEDITION_OF_THE_33].ht_text),
	   ptr_cc_id, "+", 19, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  for (i = 2; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   18, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
uz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Uzbekistan.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "UZ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if ((year > 1916) && (year < 1991))
    {
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
		 ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
    }
  if (year > 1990)
    {
      holiday (*init_data, detected,
	       _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	       "+", 3, MONTH_MAX, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	       ptr_cc_id, "+", 18, 11, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 9, year, hd_elems, fday, count);
    }

  /*
     The Christians in Uzbekistan compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  if (year != 1924)
    {
      register int i;
      auto int m;
      auto int y;


      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
}



void
vc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in St Vincent and Grenadines.
*/
{
  register int day;


  ptr_cc_id = "VC";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CARICOM_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, 2, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 22, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1978)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 27, 10, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ve_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Venezuela.
*/
{
  auto int day;
  auto int month;
  register int i;


  ptr_cc_id = "VE";
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_CARABOBO].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = DAY_MIN;
  i = weekday_of_date (day, 11, year);
  if ((i > DAY_MIN) && (i < 5))
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 11, year, hd_elems, fday, count);
  for (day = 2; day; day--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - day, 0, year, hd_elems, fday,
	     count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  (void) doy2date (easter + 39, (days_of_february (year) == 29), &day,
		   &month);
  i = weekday_of_date (day, month, year);
  if ((i > DAY_MIN) && (i < 5))
    day = easter + 39 + (DAY_MAX - i) + 1;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 5, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_PROCLAMATION].ht_text), ptr_cc_id, "+",
	   19, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = 15;
  i = weekday_of_date (day, 8, year);
  if ((i > DAY_MIN) && (i < 5))
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 8, year, hd_elems, fday, count);
  day = 8;
  i = weekday_of_date (day, MONTH_MAX, year);
  if ((i > DAY_MIN) && (i < 5))
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter - DAY_MAX, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SIMON_BOLIVAR_DAY].ht_text),
	   ptr_cc_id, "+", 24, 7, year, hd_elems, fday, count);
  day = 19;
  i = weekday_of_date (day, 3, year);
  if ((i > DAY_MIN) && (i < 5))
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = day_of_year (29, 6, year);
  i = weekday_of_date (29, 6, year);
  if ((i > DAY_MIN) && (i < 5))
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  day = 6;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday,
	   count);
}



void
vg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in British Virgin Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "VG";
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_TERRITORY_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_URSULAS_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  for (i = 0; i < 3; i++)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", day + i, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRINCE_OF_WALES_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   14, 11, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
vi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in U.S. Virgin Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "VI";
  holiday (*init_data, detected, _(hd_text[HD_TRANSFER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[3 - 1], 3, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (dvec[7 - 1], 7, year, DAY_MIN, FALSE) - DAY_MAX;
  holiday (*init_data, detected,
	   _(hd_text[HD_HURRICANE_SUPPLICATION_DAY].ht_text), ptr_cc_id, "+",
	   day, 7, year, hd_elems, fday, count);
  day = eval_holiday (dvec[10 - 1], 10, year, DAY_MIN, FALSE) - DAY_MAX;
  holiday (*init_data, detected,
	   _(hd_text[HD_HURRICANE_THANKSGIVING_DAY].ht_text), ptr_cc_id, "+",
	   day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_BRINGING_IN_THE_HARVEST_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 13, 11, year, hd_elems, fday, count);
  day = eval_holiday (dvec[4 - 1], 4, year, DAY_MAX, FALSE);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, day - i, 4, year, hd_elems, fday,
	     count);
  day = 25;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i == DAY_MAX)
    day++;
  else if (i == 6)
    day--;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = weekday_of_date (4, 7, year);
  if (day == DAY_MAX)
    day = 5;
  else if (day != 6)
    day = 4;
  else
    day = 3;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERTY_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  day =
    eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	   day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 3, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  day = 11;
  if (weekday_of_date (day, 11, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_VETERANS_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
}



void
vn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Viet Nam.
*/
{
  ptr_cc_id = "VN";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_HO_CHI_MINHS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 19, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FOUNDING_OF_THE_COMMUNIST_PARTY].ht_text), ptr_cc_id,
	   "+", 3, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", dvec[4 - 1], 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 2, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
vu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Vanuatu.
*/
{
  register int day;


  ptr_cc_id = "VU";
  day = 25;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CUSTOM_CHIEFS_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 24;
  if (weekday_of_date (day, 7, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  day = 25;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  day = 5;
  if (weekday_of_date (day, 10, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 30;
  if (weekday_of_date (day, 7, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 15;
  if (weekday_of_date (day, 8, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = 29;
  if (weekday_of_date (day, 11, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
wf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Wallis and Futuna Islands.
*/
{
  ptr_cc_id = "WF";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
ws_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in (Western) Samoa.
*/
{
  register int day;


  ptr_cc_id = "WS";
  day = eval_holiday (DAY_MIN, 10, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected,
	   _(hd_text[HD_CHILDRENS_WHITE_SUNDAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day + 1, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ANZAC_DAY].ht_text),
	   ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	   ptr_cc_id, "+", 3, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 3; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", 7, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 4, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 24, 11, year, hd_elems, fday, count);
}



void
ye_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Yemen.
*/
{
  ptr_cc_id = "YE";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_UNITY_FACTORY_DAY].ht_text),
	   ptr_cc_id, "+", 7, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 30, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 22, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  if (year > 1961)
    holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	     ptr_cc_id, "+", 26, 9, year, hd_elems, fday, count);
  if (year > 1962)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	     ptr_cc_id, "+", 14, 10, year, hd_elems, fday, count);
}



void
yt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mayotte.
*/
{
  ptr_cc_id = "YT";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
yu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Serbia and Montenegro.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "YU";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_BELGRADES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 20, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "#",
	   28, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 27, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1943)
    holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	     ptr_cc_id, "+", 11, MONTH_MIN, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter, 0, year, hd_elems, fday, count);
  /*
     The Christians in Serbia and Montenegro compute all holiday dates
     which are depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  if (year != 1924)
    {
      register int i;
      auto int m;
      auto int y;


      d = 24;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday, count);
      (void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
  d = day_of_year (14, MONTH_MIN, year);
  if (year > 1924)
    d += greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_JESUS_CIRCUMCISION].ht_text),
	   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
}



void
za_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in South Africa.
*/
{
  register int day;


  ptr_cc_id = "ZA";
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = 25;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_GOODWILL].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1992)
    {
      day = 21;
      if (weekday_of_date (day, 3, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_HUMAN_RIGHTS_DAY].ht_text),
	       ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
      day = 24;
      if (weekday_of_date (day, 9, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_HERITAGE_DAY].ht_text),
	       ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
      day = 16;
      if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_DAY_OF_RECONCILIATION].ht_text), ptr_cc_id, "+",
	       day, MONTH_MAX, year, hd_elems, fday, count);
      day = 27;
      if (weekday_of_date (day, 4, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_LIBERTY_DAY].ht_text),
	       ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
      day = DAY_MIN;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      day = 9;
      if (weekday_of_date (day, 8, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
      day = 16;
      if (weekday_of_date (day, 6, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
	       ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
    }
  else
    {
      holiday (*init_data, detected, _(hd_text[HD_SETTLERS_DAY].ht_text),
	       ptr_cc_id, "+", 6, 9, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	       ptr_cc_id, "+", dvec[5 - 1], 5, year, hd_elems, fday, count);
    }
}



void
zm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Zambia.
*/
{
  register int day;


  ptr_cc_id = "ZM";
  day = 25;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_FARMERS_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = 24;
  if (weekday_of_date (day, 10, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", day + 1, 7, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = 25;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = 12;
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
}



void
zw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Zimbabwe.
*/
{
  ptr_cc_id = "ZW";
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 12, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 18, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text), ptr_cc_id,
	   "+", 11, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 22, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text), ptr_cc_id,
	   "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}
#endif /* !HD_TOP20CC */
