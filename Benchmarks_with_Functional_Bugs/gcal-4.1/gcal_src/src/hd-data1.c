/*!
*  \file hd-data1.c
*  \brief All top 20 countries data which is composed into the
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
*  Include header files.
*/
#include "tailor.h"
#include "common.h"
#include "globals.h"
#include "hd-astro.h"
#include "hd-defs.h"
#include "hd-data.h"
#include "hd-use.h"
#include "utils.h"
#include "hd-data1.h"



/*
*  The top 20 countries specific holiday management functions.
*/
void
au_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Australia-wide.
*/
{
  register int day;
  register int i;


#if HD_TOP20CC
  ptr_cc_id = "AU";
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "#", easter - 1, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "#", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "#", day, 6, year, hd_elems, fday, count);
#endif /* HD_TOP20CC */
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (!use_other_cc)
    {
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
    }
  else
    {
      if (i == DAY_MAX)
	day++;
    }
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 26;
  if (!use_other_cc)
    {
      i = weekday_of_date (day, MONTH_MIN, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
    }
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ABORIGINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 9, year, hd_elems, fday, count);
  day = 25;
  if (!use_other_cc)
    {
      if (weekday_of_date (day, 4, year) == DAY_MAX)
	day++;
    }
  holiday (*init_data, detected, _(hd_text[HD_ANZAC_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  day = 25;
  if (!use_other_cc)
    {
      i = weekday_of_date (day, MONTH_MAX, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
    }
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MAX, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
be_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Belgium.
*/
{
  ptr_cc_id = "BE";
  holiday (*init_data, detected, _(hd_text[HD_FLEMISH_CULTURE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 11, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_DYNASTY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FRENCH_CULTURE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 27, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 21, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1917)
    holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	     ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
br_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Brazil.
*/
{
  register int i;


  ptr_cc_id = "BR";
  holiday (*init_data, detected, _(hd_text[HD_SAO_PAULO_ANNIVERSARY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MIN, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_RIO_DE_JANEIRO_ANNIVERSARY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 20, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TIRADENTES_DAY].ht_text),
	   ptr_cc_id, "+", 21, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_APARECIDA_DAY].ht_text), ptr_cc_id, "+", 12,
	   10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  for (i = 5; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 7, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 15, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
}



void
ca_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada.
*/
{
  register int day;


#if HD_TOP20CC
  ptr_cc_id = "CA";
  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "#", day, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "#", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 1, 0, year, hd_elems, fday, count);
  day = eval_holiday (24, 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_VICTORIA_DAY].ht_text),
	   ptr_cc_id, "#", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "#", day, 10, year, hd_elems, fday, count);
#endif /* HD_TOP20CC */
  holiday (*init_data, detected, _(hd_text[HD_ALL_FOOLS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HALLOWEEN].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[10 - 1], 10, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 21, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1867)
    {
      day = DAY_MIN;
      if (weekday_of_date (day, 7, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	       ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
    }
}



void
ch_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland.
*/
{
#if HD_TOP20CC
  ptr_cc_id = "CH";
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "#", easter + 39, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 50, 0, year, hd_elems, fday, count);
#else /* !HD_TOP20CC */
  if (!use_other_cc)
    {
      holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	       ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
      holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	       ptr_cc_id, "+", easter + 50, 0, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year,
	       hd_elems, fday, count);
    }
#endif /* !HD_TOP20CC */
  holiday (*init_data, detected, _(hd_text[HD_ST_BERCHTOLDS_DAY].ht_text),
	   ptr_cc_id, "#", 2, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text), ptr_cc_id,
	   "+", easter + 49, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_STEPHENS_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_QUINQUAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 49, 0, year, hd_elems, fday,
	   count);
  if (year > 1992)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	     ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
}



void
cn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in China.
*/
{
  ptr_cc_id = "CN";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  if (year > 1911)
    {
      register int day;


      holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
      for (day = DAY_MIN; day <= 2; day++)
	holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
		 ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
      for (day = DAY_MIN; day <= 2; day++)
	holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
		 ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 9, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
      if (year > 1918)
	holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
		 ptr_cc_id, "+", 4, 5, year, hd_elems, fday, count);
      if (year > 1920)
	holiday (*init_data, detected,
		 _(hd_text[HD_FOUNDING_OF_THE_COMMUNIST_PARTY].ht_text),
		 ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
      if (year > 1926)
	holiday (*init_data, detected,
		 _(hd_text[HD_ARMED_FORCES_DAY].ht_text), ptr_cc_id, "+",
		 DAY_MIN, 8, year, hd_elems, fday, count);
      if (year > 1948)
	for (day = DAY_MIN; day <= 2; day++)
	  holiday (*init_data, detected,
		   _(hd_text[HD_NATIONAL_HOLIDAY].ht_text), ptr_cc_id, "+",
		   day, 10, year, hd_elems, fday, count);
    }
}



void
de_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany.
*/
{
  register int day;


#if HD_TOP20CC
  ptr_cc_id = CC_DE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEACE_FESTIVAL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 8, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "#",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  if (year > 1516)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF2, dvec[10 - 1], 10, year, hd_elems, fday,
	     count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  day = weekday_of_date (DAY_MIN, 11, year);
  day = (day < 3) ? 18 - day : 25 - day;
  holiday (*init_data, detected,
	   _(hd_text[HD_DAY_OF_PRAYER_AND_REPENTANCE].ht_text), ptr_cc_id,
	   (year > 1994) ? DIS_HLS_PREF2 : "+", day, 11, year, hd_elems, fday,
	   count);
  if (year > 1953)
    {
      register int month;


      if (year > 1989)
	{
	  day = 3;
	  month = 10;
	}
      else
	{
	  day = 17;
	  month = 6;
	}
      holiday (*init_data, detected, _(hd_text[HD_GERMAN_UNITY_DAY].ht_text),
	       ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
    }
#else /* !HD_TOP20CC */
  if (!use_other_cc)
    if (year > 1953)
      {
	register int month;


	if (year > 1989)
	  {
	    day = 3;
	    month = 10;
	  }
	else
	  {
	    day = 17;
	    month = 6;
	  }
	holiday (*init_data, detected,
		 _(hd_text[HD_GERMAN_UNITY_DAY].ht_text), ptr_cc_id, "+", day,
		 month, year, hd_elems, fday, count);
	if (*init_data)
	  *init_data = FALSE;
      }
#endif /* !HD_TOP20CC */
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_SHROVE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 52, 0, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
  day -= (DAY_MAX * 4);
  holiday (*init_data, detected, _(hd_text[HD_SUNDAY_OF_THE_DEAD].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[11 - 1] + day, 11, year, hd_elems,
	   fday, count);
  day -= DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_MOURNING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[11 - 1] + day, 11, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_FOOLS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 39, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
  if (day_of_year (day, 5, year) != easter + 49)
    day += DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SEVEN_SLEEPERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 27, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 5, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 47, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MAX, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VALBORGS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[4 - 1], 4, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
  if (year == 2017)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, "+", dvec[10 - 1], 10, year, hd_elems, fday,
	     count);
}



void
es_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Spain.
*/
{
  ptr_cc_id = "ES";
  holiday (*init_data, detected, _(hd_text[HD_ST_JAMES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_ANDALUCIA].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 28, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_CATALONIA].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 11, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_GALICIA].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BASQUE_NATIONAL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 6,
	   MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "#", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - DAY_MAX, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_STEPHENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 26, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
fr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in France.
*/
{
  if (!use_other_cc)
    ptr_cc_id = "FR";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 26, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_QUINQUAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 49, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1788)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	     ptr_cc_id, "+", 14, 7, year, hd_elems, fday, count);
  if (year > 1917)
    holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	     ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, "+", 8, 5, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
gb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United Kingdom of Great Britain.
*/
{
  auto Greg_struct tmp_greg;
  register int day;
  register int i;


#if HD_TOP20CC
  ptr_cc_id = CC_GB;
  holiday (*init_data, detected, _(hd_text[HD_BURNS_NIGHT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MIN, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  day = 12;
  i = weekday_of_date (day, 7, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_BOYNE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 7, year, hd_elems, fday, count);
  day = 2;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 8, year, hd_elems, fday, count);
  day = 17;
  i = weekday_of_date (day, 3, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ST_PATRICKS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = eval_holiday (dvec[8 - 1], 8, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "#", day, 8, year, hd_elems, fday, count);
#else /* !HD_TOP20CC */
  if (!use_other_cc)
    {
      day = eval_holiday (dvec[8 - 1], 8, year, DAY_MIN, FALSE);
      holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	       ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
    }
#endif /* !HD_TOP20CC */
  day = 5;
  i = weekday_of_date (day, 7, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_TYNWALD_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 9;
  i = weekday_of_date (day, 5, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_DAVIDS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SWALLOW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_GEORGES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GROTTO_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_EDWARDS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 13, 10, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 25;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HALLOWEEN].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[10 - 1], 10, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_ANNUNCIATION_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 25, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 21, 0, year, hd_elems, fday,
	   count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   (year == 1999) ? "+" : DIS_HLS_PREF, dvec[MONTH_MAX - 1],
	   MONTH_MAX, year, hd_elems, fday, count);
  /*
     So we first need to buffer the Gregorian Reformation period actually used.
   */
  tmp_greg.first_day = greg->first_day;
  tmp_greg.last_day = greg->last_day;
  tmp_greg.month = greg->month;
  tmp_greg.year = greg->year;
  /*
     Then we set the Gregorian Reformation period to the first date
     which is out of the range of dates Gcal is able to manage,
     for computing the Easter Sunday of a Julian date, means
     we behave as if no Gregorian Reformation has ever happened.
   */
  greg->last_day = greg->first_day = DAY_MIN;
  greg->month = MONTH_MIN;
  greg->year = YEAR_MAX + 1;
  /*
     Compute the Julian `day_of_year' number for the
     BEGINNING_OF_FINANCIAL_YEAR (Lady Day) holiday.
   */
  day = day_of_year (25, 3, year);
  /*
     Now we set the Gregorian Reformation period to the date respected in the U.K.
   */
  greg->year = 1752;
  greg->month = 9;
  greg->first_day = 3;
  greg->last_day = 13;
  /*
     The computation is done for the next date after the given Gregorian
     Reformation period, but for the current year (Gregorian date).
     If this is not proper, I would like to hear *your* solution how
     to compute it in the right way!
   */
  if (year > greg->year)
    day += julian_gregorian_diff (greg->last_day + 1, greg->month, year);
  /*
     And restore the buffered Gregorian Reformation date actually used.
   */
  greg->year = tmp_greg.year;
  greg->month = tmp_greg.month;
  greg->last_day = tmp_greg.last_day;
  greg->first_day = tmp_greg.first_day;
  holiday (*init_data, detected,
	   _(hd_text[HD_BEGINNING_OF_FINANCIAL_YEAR].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, day, 0, year, hd_elems, fday, count);
  if (year > 1604)
    holiday (*init_data, detected, _(hd_text[HD_GUY_FAWKES_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, 5, 11, year, hd_elems, fday, count);
}



void
il_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Israel.
*/
{
  ptr_cc_id = "IL";
  use_other_cc = !use_other_cc;
  hebrew_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if (year > 1999)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, 9, 5, year, hd_elems, fday, count);
}



void
it_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Italy.
*/
{
  register int day;
  register int month;
  register int i;


  ptr_cc_id = "IT";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
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
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = 2;
  month = 6;
  i = weekday_of_date (day, month, year);
  if (i > 3)
    day += (DAY_MAX - i);
  else if (i > 1)
    {
      i -= 2;
      day = dvec[--month - 1] - i;
    }
  else
    day -= i;
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, month, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VENICE_CARNIVAL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 58, 0, year, hd_elems, fday,
	   count);
  if (year > 1917)
    {
      day = 4;
      i = weekday_of_date (day, 11, year);
      if (i > 3)
	day += (DAY_MAX - i);
      else
	day -= i;
      holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 11, year, hd_elems, fday, count);
    }
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
}



void
jp_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Japan.
*/
{
  ptr_cc_id = "JP";
  if (year < 1873)
    {
      use_other_cc = !use_other_cc;
      japanese_hdy (init_data, detected, year, hd_elems, fday, count,
		    ptr_cc_id, FALSE);
      use_other_cc = !use_other_cc;
    }
  else
    {
      register int hour;
      register int min;
      register int i;
      auto int day;
      auto int month;
      auto int y = year;


      /*
         Astronomical calculations are based on China/Japan local time.
       */
      if (year < 1888)
	{
	  /*
	     Meridian Beijing (116 25' degrees East), UTC/GMT time difference.
	   */
	  hour = 7;
	  min = 45;
	}
      else
	{
	  /*
	     UTC/GMT-9 for the meridian 135 degrees East.
	   */
	  hour = 9;
	  min = 0;
	}
      day = 15;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_COMING_OF_AGE_DAY].ht_text),
	       ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
      day = eval_holiday (DAY_MIN, 2, year, 4, TRUE);
      holiday (*init_data, detected,
	       _(hd_text[HD_SAPPORO_SNOW_FESTIVAL].ht_text), ptr_cc_id,
	       DIS_HLS_PREF, day, 2, year, hd_elems, fday, count);
      day = 2;
      if (weekday_of_date (day, 2, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_SPRING_FESTIVAL].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 2, year, hd_elems, fday, count);
      day = 3;
      if (weekday_of_date (day, 3, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_PEACH_FESTIVAL].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 3, year, hd_elems, fday, count);
      day = 8;
      if (weekday_of_date (day, 4, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_BUDDHAS_BIRTHDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 4, year, hd_elems, fday, count);
      day = 29;
      if (weekday_of_date (day, 4, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_GREENERY_DAY].ht_text),
	       ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
      day = 7;
      if (weekday_of_date (day, 7, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_STAR_FESTIVAL].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 7, year, hd_elems, fday, count);
      day = 20;
      if (weekday_of_date (day, 7, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_MARINE_DAY].ht_text),
	       ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
      day = 13;
      if (weekday_of_date (day, 8, year) == DAY_MAX)
	day++;
      for (i = 0; i < 4; i++)
	holiday (*init_data, detected,
		 _(hd_text[HD_FEAST_OF_FORTUNE].ht_text), ptr_cc_id,
		 DIS_HLS_PREF, day + i, 8, year, hd_elems, fday, count);
      day = 9;
      if (weekday_of_date (day, 9, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_CHRYSANTHENUM_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 9, year, hd_elems, fday, count);
      day = 15;
      if (weekday_of_date (day, 9, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_RESPECT_FOR_THE_AGED_DAY].ht_text), ptr_cc_id,
	       "+", day, 9, year, hd_elems, fday, count);
      day = 10;
      if (weekday_of_date (day, 10, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_HEALTH_AND_SPORTS_DAY].ht_text), ptr_cc_id, "+",
	       day, 10, year, hd_elems, fday, count);
      day = 15;
      if (weekday_of_date (day, 11, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_7_5_3_FESTIVAL].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, 11, year, hd_elems, fday, count);
      day = 23;
      if (weekday_of_date (day, 11, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_LABOUR_THANKSGIVING_DAY].ht_text), ptr_cc_id, "+",
	       day, 11, year, hd_elems, fday, count);
      day = 13;
      if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_SOOT_SWEEPING_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	       count);
      day = 23;
      if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_EMPERORS_BIRTHDAY].ht_text),
	       ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
      day = 5;
      if (weekday_of_date (day, 5, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	       ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
      day = 3;
      if (weekday_of_date (day, 11, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_CULTURE_DAY].ht_text),
	       ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
      day = 4;
      i = weekday_of_date (day, 5, year);
      if (i < 6)
	holiday (*init_data, detected,
		 _(hd_text[HD_NATIONAL_HOLIDAY].ht_text), ptr_cc_id, "+", day,
		 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year,
	       hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, 2, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, 3, MONTH_MIN, year, hd_elems, fday,
	       count);
      if (equinox_solstice (NH_VE, &day, &month, &y, hour, min) !=
	  (double) SPECIAL_VALUE)
	{
	  if (weekday_of_date (day, month, y) == DAY_MAX)
	    (void) next_date (&day, &month, &y);
	  holiday (*init_data, detected, _(hd_text[HD_EQUINOX_DAY].ht_text),
		   ptr_cc_id, "+", day, month, y, hd_elems, fday, count);
	}
      else
	y = year;
      if (equinox_solstice (NH_AE, &day, &month, &y, hour, min) !=
	  (double) SPECIAL_VALUE)
	{
	  if (weekday_of_date (day, month, y) == DAY_MAX)
	    (void) next_date (&day, &month, &y);
	  holiday (*init_data, detected, _(hd_text[HD_EQUINOX_DAY].ht_text),
		   ptr_cc_id, "+", day, month, y, hd_elems, fday, count);
	}
      else
	y = year;
      if (equinox_solstice (NH_WS, &day, &month, &y, hour, min) !=
	  (double) SPECIAL_VALUE)
	holiday (*init_data, detected,
		 _(hd_text[HD_JAPANESE_NEW_YEARS_DAY].ht_text), ptr_cc_id,
		 DIS_HLS_PREF, day, month, y, hd_elems, fday, count);
      else
	y = year;
      if (sun_longitude (315.0, &day, &month, &y, hour, min, FALSE) !=
	  (double) SPECIAL_VALUE)
	{
	  (void) prev_date (&day, &month, &y);
	  if (y == year)
	    holiday (*init_data, detected,
		     _(hd_text[HD_SEASON_CHANGE].ht_text), ptr_cc_id,
		     DIS_HLS_PREF, day, month, y, hd_elems, fday, count);
	}
      if (year > 1946)
	{
	  day = 3;
	  if (weekday_of_date (day, 5, year) == DAY_MAX)
	    day++;
	  holiday (*init_data, detected,
		   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
		   "+", day, 5, year, hd_elems, fday, count);
	  day = 11;
	  if (weekday_of_date (day, 2, year) == DAY_MAX)
	    day++;
	  holiday (*init_data, detected,
		   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id,
		   "+", day, 2, year, hd_elems, fday, count);
	}
    }
}



void
kr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Republic of Korea.
*/
{
  ptr_cc_id = "KR";
  use_other_cc = !use_other_cc;
  if ((year > 1909) && (year < 1945))
    japanese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
		  FALSE);
  else
    chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
		 FALSE);
  use_other_cc = !use_other_cc;
  if (year > 1911)
    {
      register int day;


      holiday (*init_data, detected, _(hd_text[HD_PARENTS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 8, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	       ptr_cc_id, "+", 5, 4, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	       ptr_cc_id, "+", 5, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, 10, 3, year, hd_elems, fday, count);
      for (day = DAY_MIN; day <= 2; day++)
	holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
		 ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_ALPHABET_DAY].ht_text),
	       ptr_cc_id, (year > 1995) ? DIS_HLS_PREF : "+", 9, 10, year,
	       hd_elems, fday, count);
      if (year > 1918)
	holiday (*init_data, detected,
		 _(hd_text[HD_INDEPENDENCE_MOVEMENT_DAY].ht_text), ptr_cc_id,
		 "+", DAY_MIN, 3, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
		 ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
      if (year > 1947)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_ARMED_FORCES_DAY].ht_text), ptr_cc_id, "+",
		   DAY_MIN, 10, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
		   DIS_HLS_PREF, 17, 7, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id,
		   "+", 3, 10, year, hd_elems, fday, count);
	}
      if (year > 1952)
	holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
		 ptr_cc_id, "+", 6, 6, year, hd_elems, fday, count);
    }
}



void
mx_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mexico.
*/
{
  register int day;


  ptr_cc_id = "MX";
  holiday (*init_data, detected, _(hd_text[HD_BENITO_JUAREZ_DAY].ht_text),
	   ptr_cc_id, "+", 21, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_PUEBLA].ht_text),
	   ptr_cc_id, "#", 5, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GUADALUPE_VIRGINS_DAY].ht_text),
	   ptr_cc_id, "#", 12, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  for (day = DAY_MAX; day; day--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - day, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[4 - 1], 4, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "#", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 5,
	   2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 24, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HOLY_INNOCENTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 28, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 16, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_PROCLAMATION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "#", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 10, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (!(year % 6))
    holiday (*init_data, detected,
	     _(hd_text[HD_PRESIDENTIAL_INAUGURATION].ht_text), ptr_cc_id, "+",
	     DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_INFORM].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 20, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
}



void
nl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in the Netherlands.
*/
{
  register int day;


  ptr_cc_id = "NL";
  holiday (*init_data, detected, _(hd_text[HD_EINDHOVENS_LIBERATION].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 18, 9, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 9, year, 2, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRINCESS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WORLD_ANIMAL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 4, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
  if (day_of_year (day, 5, year) != easter + 49)
    day += DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 5, MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1890 && year < 1949)
    holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	     ptr_cc_id, "+", 31, 8, year, hd_elems, fday, count);
  else if (year > 1948 && year < 2014)
    holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	     ptr_cc_id, "+", 30, 4, year, hd_elems, fday, count);
  else if (year > 2013)
    holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	     ptr_cc_id, "+", 27, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 4, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if ((year > 1944) && !(year % 5))
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 5, 5, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
nz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in New Zealand.
*/
{
  register int day;
  register int i;


  if (!use_other_cc)
    ptr_cc_id = "NZ";
  day = 29;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_AUCKLAND].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_CANTERBURY_SOUTH].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 9, year, hd_elems, fday, count);
  day = day_of_year (30, 11, year);
  i = weekday_of_date (30, 11, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_CHATHAM_ISLANDS].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  day = day_of_year (DAY_MIN, 2, year);
  i = weekday_of_date (DAY_MIN, 2, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_ANNIVERSARY_OF_NELSON].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  day = 23;
  i = weekday_of_date (day, 3, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_ANNIVERSARY_OF_OTAGO].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = 17;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_SOUTHLAND].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_TARANAKI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = 22;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_WELLINGTON].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday, count);
  day = day_of_year (DAY_MIN, MONTH_MAX, year);
  i = weekday_of_date (DAY_MIN, MONTH_MAX, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_WESTLAND].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ANZAC_DAY].ht_text), ptr_cc_id,
	   "+", 25, 4, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 25;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_HAWKES_BAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day - 3, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_MARLBOROUGH].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day_of_year (day, 10, year) + DAY_MAX, 0, year,
	   hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_CANTERBURY_NORTH].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day_of_year (day, 10, year) + (2 * DAY_MAX) + 4, 0,
	   year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 6, 2, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = 2;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
pt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Portugal.
*/
{
  ptr_cc_id = "PT";
  holiday (*init_data, detected, _(hd_text[HD_CAMOES_DAY].ht_text),
	   ptr_cc_id, "+", 10, 6, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 48, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "#", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 3, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 5, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 47, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
}



void
ru_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Russian Federation.
*/
{
  register int i;
  auto int greg_diff;
  auto int d;
  auto int m;
  auto int y;


  ptr_cc_id = "RU";
  /*
     The Christians in Russia compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_COSMONAUTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 12, 4, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  d = 23;
  i = weekday_of_date (d, 2, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_DEFENDERS_OF_MOTHERLAND_DAY].ht_text), ptr_cc_id, "+",
	   d, 2, year, hd_elems, fday, count);
  d = DAY_MIN;
  i = weekday_of_date (d, 5, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
  d = 2;
  i = weekday_of_date (d, 5, year);
  if (i > 5)
    d += 2;
  else if (i == DAY_MIN)
    d++;
  holiday (*init_data, detected, _(hd_text[HD_SPRING_FESTIVAL].ht_text),
	   ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
  d = DAY_MIN;
  i = weekday_of_date (d, MONTH_MIN, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", d, MONTH_MIN, year, hd_elems, fday, count);
  d = 2;
  i = weekday_of_date (d, MONTH_MIN, year);
  if (i > 5)
    d += 2;
  else if (i == DAY_MIN)
    d++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", d, MONTH_MIN, year, hd_elems, fday, count);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  d = 8;
  i = weekday_of_date (d, 3, year);
  if (i > 5)
    d += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", d, 3, year, hd_elems, fday, count);
  if (year != 1924)
    {
      d = 24;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
      (void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
      (void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  if (year > 1944)
    {
      d = 9;
      i = weekday_of_date (d, 5, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	       ptr_cc_id, "+", d, 5, year, hd_elems, fday, count);
    }
  if (year > 1991)
    {
      d = 12;
      i = weekday_of_date (d, 6, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", d, 6, year, hd_elems, fday, count);
      d = 12;
      i = weekday_of_date (d, MONTH_MAX, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	       "+", d, MONTH_MAX, year, hd_elems, fday, count);
      d = 7;
      i = weekday_of_date (d, 11, year);
      if (i > 5)
	d += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected,
	       _(hd_text[HD_DAY_OF_RECONCILIATION].ht_text), ptr_cc_id, "+",
	       d, 11, year, hd_elems, fday, count);
    }
  else if (year > 1916)
    for (i = 6; i <= 7; i++)
      holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	       ptr_cc_id, "+", i, 11, year, hd_elems, fday, count);
}



void
us_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United States.
*/
{
  register int day;
  register int month;
  register int i = 0;


#if HD_TOP20CC
  ptr_cc_id = CC_US;
  observe_us_hdy = SAFR_SUMO;
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "#", day, 2, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "#", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRESIDENT_LINCOLNS_BIRTHDAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 12, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 47, 0, year, hd_elems, fday,
	   count);
  if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE) + 1;
      holiday (*init_data, detected, _(hd_text[HD_ELECTION_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, day, 11, year, hd_elems, fday,
	       count);
    }
  if ((year > 1964) && !((1965 - year) & 3))
    {
      day = 20;
      if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected,
	       _(hd_text[HD_PRESIDENTIAL_INAUGURATION].ht_text), ptr_cc_id,
	       DIS_HLS_PREF2, day, MONTH_MIN, year, hd_elems, fday, count);
    }
#else /* !HD_TOP20CC */
  if (!use_other_cc)
    {
      day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
      holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	       ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
      if (*init_data)
	*init_data = FALSE;
      day =
	eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN,
		      TRUE) + (2 * DAY_MAX);
      holiday (*init_data, detected,
	       _(hd_text[HD_MARTIN_LUTHER_KINGS_DAY].ht_text), ptr_cc_id, "+",
	       day, MONTH_MIN, year, hd_elems, fday, count);
      day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
      holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	       ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
    }
#endif /* !HD_TOP20CC */
  day = 25;
  month = MONTH_MAX;
  switch (observe_us_hdy)
    {
    case SASA_SUSU:
      break;
    case SASA_SUMO:
    case SASAFR_SUMO:
      if (weekday_of_date (day, month, year) == DAY_MAX)
	day++;
      break;
    case SAMO_SUMO:
      i = weekday_of_date (day, month, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      break;
    case SAFR_SUMO:
      i = weekday_of_date (day, month, year);
      if (i == DAY_MAX)
	day++;
      else if (i == 6)
	day--;
      break;
    default:
      abort ();			/* Error, invalid case. */
    }
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 4;
  month = 7;
  switch (observe_us_hdy)
    {
    case SASA_SUSU:
      break;
    case SASA_SUMO:
    case SASAFR_SUMO:
      if (weekday_of_date (day, month, year) == DAY_MAX)
	day++;
      break;
    case SAMO_SUMO:
      i = weekday_of_date (day, month, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      break;
    case SAFR_SUMO:
      i = weekday_of_date (day, month, year);
      if (i == DAY_MAX)
	day++;
      else if (i == 6)
	day--;
      break;
    default:
      abort ();			/* Error, invalid case. */
    }
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
  day = DAY_MIN;
  month = MONTH_MIN;
  switch (observe_us_hdy)
    {
    case SASA_SUSU:
      break;
    case SASA_SUMO:
    case SASAFR_SUMO:
      if (weekday_of_date (day, month, year) == DAY_MAX)
	day++;
      break;
    case SAMO_SUMO:
      i = weekday_of_date (day, month, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      break;
    case SAFR_SUMO:
      i = weekday_of_date (day, month, year);
      if (i == DAY_MAX)
	day++;
      else if (i == 6)
	i = SPECIAL_VALUE;
      break;
    default:
      abort ();			/* Error, invalid case. */
    }
  if (i != SPECIAL_VALUE)
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
  if ((observe_us_hdy == SAFR_SUMO)
      && (weekday_of_date (DAY_MIN, MONTH_MIN, year + 1) == 6))
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	     fday, count);
  day = 11;
  month = 11;
  switch (observe_us_hdy)
    {
    case SASA_SUSU:
      break;
    case SASA_SUMO:
      if (weekday_of_date (day, month, year) == DAY_MAX)
	day++;
      break;
    case SAMO_SUMO:
      i = weekday_of_date (day, month, year);
      if (i > 5)
	day += ((DAY_MAX - i) + 1);
      break;
    case SAFR_SUMO:
    case SASAFR_SUMO:
      i = weekday_of_date (day, month, year);
      if (i == DAY_MAX)
	day++;
      else if (i == 6)
	day--;
      break;
    default:
      abort ();			/* Error, invalid case. */
    }
  holiday (*init_data, detected, _(hd_text[HD_VETERANS_DAY].ht_text),
	   ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GRANDPARENTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day + 6, 9, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_FOOLS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, 6, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOSSES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 16, 10, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CITIZENSHIP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 17, 9, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MAX, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_FRIENDSHIP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HALLOWEEN].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[10 - 1], 10, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GROUNDHOG_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 2, year, hd_elems, fday, count);
  for (day = 26; day <= dvec[MONTH_MAX - 1]; day++)
    holiday (*init_data, detected, _(hd_text[HD_KWANZAA].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	     count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHER_IN_LAWS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 26, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NURSES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MAX, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_PARENTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, 4, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_PRAYER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PATRICKS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 17, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, 6, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_SWEETEST_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
}
