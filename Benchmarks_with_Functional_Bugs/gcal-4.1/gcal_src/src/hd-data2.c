/*!
*  \file hd-data2.c
*  \brief All other A-C countries data which is composed into the
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
# include "hd-data3.h"
# include "hd-data4.h"
# include "hd-data5.h"
# include "hd-use.h"
# include "utils.h"
# include "hd-data2.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
  static void
  ca_base_hdy __P_ ((Bool * init_data,
		     const Bool detected,
		     int easter,
		     const int year,
		     int *hd_elems, const int fday, const int count));
static void
  ch_base_hdy __P_ ((Bool * init_data,
		     const Bool detected,
		     int easter,
		     const int year,
		     int *hd_elems, const int fday, const int count));
__END_DECLARATIONS
/*
*  All other A-C countries specific holiday management functions.
*/
  void
ad_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Andorra.
*/
{
  ptr_cc_id = "AD";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   14, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 8, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_CHARLES_DAY].ht_text),
	   ptr_cc_id, "+", 4, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ae_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in United Arab Emirates.
*/
{
  ptr_cc_id = "AE";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_SHEIKH_ZAYEDS_ASCENSION_DAY].ht_text), ptr_cc_id, "+",
	   6, 8, year, hd_elems, fday, count);
}



void
af_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Afghanistan.
*/
{
  ptr_cc_id = "AF";
  use_other_cc = !use_other_cc;
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LOSS_OF_MUSLIM_NATION].ht_text),
	   ptr_cc_id, "+", 28, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 4, 5, year, hd_elems, fday, count);
  if (year > 1989)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 19, 8, year, hd_elems, fday, count);
}



void
ag_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Antigua and Barbuda.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AG";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CARICOM_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", day + i, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MERCHANT_DAY].ht_text),
	   ptr_cc_id, "+", 7, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  if (year > 1980)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ai_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Anguilla.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AI";
  holiday (*init_data, detected, _(hd_text[HD_SEPARATION_DAY].ht_text),
	   ptr_cc_id, "+", 19, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  for (i = 0; i < DAY_MAX; i++)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF2, day + i, 8, year, hd_elems, fday,
	     count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   day + 4, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, 5, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
al_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Albania.
*/
{
  ptr_cc_id = "AL";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", 28, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1943)
    {
      holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	       ptr_cc_id, "+", 29, 11, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	       ptr_cc_id, "+", 11, MONTH_MIN, year, hd_elems, fday, count);
    }
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  /*
     The Christians in Albania compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches new calendar.
   */
  easter = orthodox_easter (year, NULL, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
}



void
am_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Armenia.
*/
{
  ptr_cc_id = "AM";
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 5,
	   7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 21, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MOTHERHOOD_AND_BEAUTY_DAY].ht_text), ptr_cc_id, "+",
	   7, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 28, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1894)
    holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	     ptr_cc_id, "+", 24, 4, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, "+", 29, 5, year, hd_elems, fday, count);
  if (year > 1991)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 21, 9, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  /*
     The Christians in Armenia compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar, but they
     celebrate the Easter feast itself on the same date as done by
     Christian Western churches.
   */
  if (year != 1924)
    {
      register int i;
      auto int greg_diff;
      auto int d;
      auto int m;
      auto int y;


      (void) orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday, count);
    }
}



void
an_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Netherlands Antilles/Netherlands Antilles-wide.
*/
{
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 48, 0, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", dvec[4 - 1], 4, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
an_bo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Netherlands Antilles/Bonaire.
*/
{
  ptr_cc_id = "AN_BO";
  an_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BONAIRE_DAY].ht_text),
	   ptr_cc_id, "+", 6, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	   ptr_cc_id, "+", 19, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
}



void
an_cu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Netherlands Antilles/Cura,cao.
*/
{
  ptr_cc_id = "AN_CU";
  an_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ANTILLIAN_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", 2, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_EVE].ht_text),
	   ptr_cc_id, "+", 5, MONTH_MAX, year, hd_elems, fday, count);
}



void
an_ma_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Netherlands Antilles/St Maarten.
*/
{
  ptr_cc_id = "AN_MA";
  an_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTINIMAS].ht_text),
	   ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 15, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTOR_SCHOELCHER_DAY].ht_text),
	   ptr_cc_id, "+", 21, 7, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	     ptr_cc_id, "+", 8, 5, year, hd_elems, fday, count);
}



void
an_sa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Netherlands Antilles/Saba and Statia.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AN_SA";
  an_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ANTILLIAN_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_STATIA_AMERICA_DAY].ht_text),
	   ptr_cc_id, "#", 16, 11, year, hd_elems, fday, count);
  for (i = day = eval_holiday (DAY_MIN, MONTH_MAX, year, 6, TRUE);
       day < i + 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_SABA_DAY].ht_text), ptr_cc_id,
	     "#", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (dvec[7 - 1], 7, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 7, year, hd_elems, fday, count);
  if (year % 5)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 5, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	     ptr_cc_id, "+", 8, 5, year, hd_elems, fday, count);
}



void
ao_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Angola.
*/
{
  ptr_cc_id = "AO";
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_ARMED_STRUGGLE].ht_text), ptr_cc_id,
	   "+", 4, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FOUNDATION_OF_NPLA_DAY].ht_text), ptr_cc_id, "+", 10,
	   MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 17, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PIONEER_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 27, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 14, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 8, 3, year, hd_elems, fday, count);
}



void
ar_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Argentina.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AR";
  day = 10;
  i = weekday_of_date (day, 6, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_MALVINAS_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 20;
  i = weekday_of_date (day, 6, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_GENERAL_SAN_MARTINS_ANNIVERSARY].ht_text), ptr_cc_id,
	   "+", 17, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_STUDENTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 21, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MAX, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 9, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MAX, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1809)
    holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	     ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
}



void
as_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in American Samoa.
*/
{
  ptr_cc_id = "AS";
  /*
     I assume that no U.S. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", 17, 4, year, hd_elems, fday, count);
}



void
at_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Austria.
*/
{
  ptr_cc_id = "AT";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
//according to §7 Arbeitsruhegesetz
// (BGBl. Nr. 144/1983 zuletzt geändert durch BGBl. I Nr. 159/2004)
//All Souls day is no holiday in Austria
//   holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
//           ptr_cc_id, DIS_HLS_PREF2, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 26, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
au_ct_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Australia Capital Territory (Canberra).
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_CT";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_CANBERRA_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
}



void
au_nt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Northern Territory.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_NT";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_PICNIC_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected,
	   _(hd_text[HD_ALICE_SPRINGS_SHOW_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected,
	   _(hd_text[HD_TENNANT_CREEK_SHOW_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_KATHERINE_SHOW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_DARWIN_SHOW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
}



void
au_qu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Queensland.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_QU";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
}



void
au_sa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Southern Australia.
*/
{
  register int day;


  ptr_cc_id = "AU_SA";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ADELAIDE_CUP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 18, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PROCLAMATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 26, MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
au_sw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/New South Wales.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_SW";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
au_ta_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Tasmania.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_TA";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, 2, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_REGATTA_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 2, year, hd_elems, fday, count);
  day = eval_holiday (dvec[2 - 1], 2, year, 3, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LAUNCESTON_CUP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_EIGHT_HOURS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 14, 4, year, hd_elems, fday, count);
  day = eval_holiday (18, 10, year, 4, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_HOBART_SHOW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RECREATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 2, 11, year, hd_elems, fday, count);
}



void
au_vi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Victoria.
*/
{
  register int day;


  ptr_cc_id = "AU_VI";
  use_other_cc = !use_other_cc;
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, 2, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MELBOURNE_CUP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 11, year, hd_elems, fday, count);
}



void
au_wa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Australia/Western Australia.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "AU_WA";
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id, "+",
	   day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
aw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Aruba.
*/
{
  ptr_cc_id = "AW";
  an_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_EVE].ht_text),
	   ptr_cc_id, "+", 5, MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1985)
    holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	     ptr_cc_id, "+", 18, 3, year, hd_elems, fday, count);
  else
    holiday (*init_data, detected, _(hd_text[HD_ANTILLIAN_DAY].ht_text),
	     ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
}



void
az_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Azerbaijan.
*/
{
  ptr_cc_id = "AZ";
  use_other_cc = !use_other_cc;
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if (year > 1917)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 28, 5, year, hd_elems, fday, count);
  if ((year > 1916) && (year < 1991))
    {
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
    }
  if (year > 1990)
    {
      holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	       ptr_cc_id, "+", 9, 10, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	       ptr_cc_id, "+", 20, MONTH_MIN, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	       ptr_cc_id, "+", 17, 11, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", 18, 10, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_SOLIDARITY_DAY].ht_text),
	       ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	       fday, count);
    }
}



void
ba_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bosnia-Herzegovina.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "BA";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 25, 11, year, hd_elems, fday, count);
  if (year > 1992)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", DAY_MIN, 3, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_NATIVITY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 8, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	   count);
  /*
     The Orthodox Christians in Bosnia-Herzegovina compute all holiday
     dates which are depending on Easter Sunday's date in the way done
     by the Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  d = day_of_year (23, 4, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_GEORGES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
  d = day_of_year (15, 8, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  d = day_of_year (8, 9, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_MARYS_NATIVITY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  d = day_of_year (29, 6, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  d = day_of_year (15, 6, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_VIDOVDAN].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  d = day_of_year (20, 7, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_ELIAHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
  d = day_of_year (26, 10, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_DEMETRIUS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, d, 0, year, hd_elems, fday, count);
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
}



void
bb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Barbados.
*/
{
  register int day;


  ptr_cc_id = "BB";
  holiday (*init_data, detected, _(hd_text[HD_ERROL_BARROW_DAY].ht_text),
	   ptr_cc_id, "+", 21, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 28, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_UNITED_NATIONS_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  if (year > 1965)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", dvec[11 - 1], 11, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
bd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bangladesh.
*/
{
  register int day;


  ptr_cc_id = "BD";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_SHAHEED_DAY].ht_text),
	   ptr_cc_id, "+", 21, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 2, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_MOURNING_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 16, MONTH_MAX, year, hd_elems, fday, count);
}



void
bf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Burkina Faso.
*/
{
  ptr_cc_id = "BF";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 5, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 15, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 11, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEOPLES_UPRISING_DAY].ht_text),
	   ptr_cc_id, "+", 3, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 4, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
bg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bulgaria.
*/
{
  register int day;
  auto int greg_diff;


  ptr_cc_id = "BG";
  /*
     The Orthodox Christians in Bulgaria compute all holiday dates
     which are depending on Easter Sunday's date in the way done
     by the Orthodox Christian Eastern churches new calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CULTURE_DAY].ht_text),
	   ptr_cc_id, "+", 24, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 3, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_REVIVAL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = day_of_year (23, 4, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_GEORGES_DAY].ht_text),
	   ptr_cc_id, "+", day, 0, year, hd_elems, fday, count);
  if (year > 1884)
    holiday (*init_data, detected, _(hd_text[HD_UNION_DAY].ht_text),
	     ptr_cc_id, "+", 6, 9, year, hd_elems, fday, count);
  if (year > 1990)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 22, 9, year, hd_elems, fday, count);
}



void
bh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bahrain.
*/
{
  ptr_cc_id = "BH";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 16, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
bi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Burundi.
*/
{
  ptr_cc_id = "BI";
  holiday (*init_data, detected, _(hd_text[HD_RWAGASORE_DAY].ht_text),
	   ptr_cc_id, "+", 13, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_NDADAYE_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 5, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 18, 9, year, hd_elems, fday, count);
}



void
bj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Benin.
*/
{
  ptr_cc_id = "BJ";
  holiday (*init_data, detected, _(hd_text[HD_TRADITIONAL_DAY].ht_text),
	   ptr_cc_id, "+", 10, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
bm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bermuda.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "BM";
  day = 24;
  i = weekday_of_date (day, 5, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day =
    day_of_year (eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE), 8, year) - 4;
  holiday (*init_data, detected, _(hd_text[HD_CUP_MATCH_DAY].ht_text),
	   ptr_cc_id, "+", day, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SOMERS_DAY].ht_text), ptr_cc_id,
	   "+", day + 1, 0, year, hd_elems, fday, count);
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
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = 11;
  i = weekday_of_date (day, 11, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
bn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Brunei.
*/
{
  ptr_cc_id = "BN";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", dvec[5 - 1], 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 23, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 15, 7, year, hd_elems, fday, count);
}



void
bo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bolivia.
*/
{
  register int i;


  ptr_cc_id = "BO";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  for (i = DAY_MAX; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 6, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 27, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
bs_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bahamas.
*/
{
  register int day;


  ptr_cc_id = "BS";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 5, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
bt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bhutan.
*/
{
  ptr_cc_id = "BT";
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 8, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 17, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
}



void
bv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Bouvet Island.
*/
{
  ptr_cc_id = "BV";
  use_other_cc = !use_other_cc;
  no_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
bw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Botswana.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "BW";
  holiday (*init_data, detected, _(hd_text[HD_SERETSE_KHAMA_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = day_of_year (dvec[9 - 1], 9, year);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_BOTSWANA_DAY].ht_text),
	     ptr_cc_id, "+", day + i, 0, year, hd_elems, fday, count);
  for (day = 15; day <= 16; day++)
    holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
by_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Belarus.
*/
{
  auto int greg_diff;


  ptr_cc_id = "BY";
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
  if (year > 1916)
    holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	     ptr_cc_id, "+", 7, 11, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
  if (year > 1991)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 3, 7, year, hd_elems, fday, count);
  if (year > 1993)
    holiday (*init_data, detected,
	     _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	     15, 3, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  /*
     The Orthodox Christians in Belarus compute all holiday dates
     which are depending on Easter Sunday's date in the way done
     by the Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  if (year != 1924)
    {
      register int i;
      auto int d;
      auto int m;
      auto int y;


      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
}



void
bz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Belize.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "BZ";
  day = 9;
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BARON_BLISS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 19;
  if (weekday_of_date (day, 11, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_GARIFUNA_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  for (i = DAY_MAX; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  day = 25;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = 24;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 21;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = 10;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_ST_GEORGES_DAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
ca_al_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Alberta.
*/
{
  register int day;


  ptr_cc_id = "CA_AL";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 2, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_bc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/British Columbia.
*/
{
  register int day;


  ptr_cc_id = "CA_BC";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_ma_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Manitoba.
*/
{
  register int day;


  ptr_cc_id = "CA_MA";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_nb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/New Brunswick.
*/
{
  register int day;


  ptr_cc_id = "CA_NB";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_nf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Newfoundland and Labrador.
*/
{
  register int day;


  ptr_cc_id = "CA_NF";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, 3, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_REGATTA_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 8, year, hd_elems, fday, count);
}



void
ca_ns_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Nova Scotia.
*/
{
  register int day;


  ptr_cc_id = "CA_NS";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "#", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
}



void
ca_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Northwest Territories.
*/
{
  ptr_cc_id = "CA_NW";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
ca_on_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Ontario.
*/
{
  register int day;


  ptr_cc_id = "CA_ON";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_pe_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Prince Edward Island.
*/
{
  register int day;


  ptr_cc_id = "CA_PE";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 8, year, hd_elems, fday, count);
}



void
ca_qu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Qu'ebec.
*/
{
  ptr_cc_id = "CA_QU";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JEAN_BAPTISTE_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
}



void
ca_sa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*
   Manages all specific holidays celebrated in Canada/Saskatchewan.
*/
{
  register int day;


  ptr_cc_id = "CA_SA";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
ca_yu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Canada/Yukon.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "CA_YU";
  ca_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 17;
  i = weekday_of_date (day, 8, year);
  if (i > 4)
    day += ((DAY_MAX - i) + 1);
  else
    day += (DAY_MIN - i);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = eval_holiday (dvec[2 - 1], 2, year, DAY_MAX, FALSE) - 2;
  holiday (*init_data, detected, _(hd_text[HD_HERITAGE_DAY].ht_text),
	   ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
}



void
cc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cocos (Keeling) Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "CC";
  /*
     I assume that the Australian holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
cd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Democratic Republic of Congo.
*/
{
  ptr_cc_id = "CD";
  holiday (*init_data, detected, _(hd_text[HD_NAMING_DAY].ht_text),
	   ptr_cc_id, "+", 27, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_NEW_REGIME_ANNIVERSARY].ht_text), ptr_cc_id, "+", 24,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 17, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_MOVEMENT_DAY].ht_text), ptr_cc_id, "+",
	   20, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id, "+", 14,
	   10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PARENTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   "+", 14, 10, year, hd_elems, fday, count);
  if (year > 1959)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 30, 6, year, hd_elems, fday, count);
}



void
cf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Central African Republic.
*/
{
  ptr_cc_id = "CF";
  holiday (*init_data, detected, _(hd_text[HD_BOGANDA_DAY].ht_text),
	   ptr_cc_id, "+", 29, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 13, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
cg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Republic of Congo.
*/
{
  ptr_cc_id = "CG";
  holiday (*init_data, detected, _(hd_text[HD_MARIEN_NGOUABI_DAY].ht_text),
	   ptr_cc_id, "+", 18, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 22, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_MOVEMENT_DAY].ht_text), ptr_cc_id, "+",
	   dvec[7 - 1], 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", 5, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 8, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   "+", 8, 2, year, hd_elems, fday, count);
}



void
ch_ar_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Appenzell-Ausserrhoden.
*/
{
  ptr_cc_id = "CH_AR";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 13, MONTH_MIN, year, hd_elems, fday, count);
}



void
ch_ag_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Aargau.
*/
{
  register int day;


  ptr_cc_id = "CH_AG";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 7, year, 5, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
}



void
ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Appenzell-Innerrhoden.
*/
{
  if (!use_other_cc)
    {
      ptr_cc_id = "CH_AI";
      ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
}



void
ch_bl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Basel-Landschaft.
*/
{
  ptr_cc_id = "CH_BL";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", 1, 5, year, hd_elems, fday, count);
}



void
ch_be_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Bern.
*/
{
  register int i;


  ptr_cc_id = "CH_BE";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  for (i = 3; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 42 - i, 0, year, hd_elems, fday, count);
}



void
ch_bs_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Basel-Stadt.
*/
{
  ptr_cc_id = "CH_BS";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", 1, 5, year, hd_elems, fday, count);
}



void
ch_fr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Fribourg.
*/
{
  ptr_cc_id = "CH_FR";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
ch_gr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Graub"unden.
*/
{
  ptr_cc_id = "CH_GR";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
ch_gl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Glarus.
*/
{
  register int day;


  ptr_cc_id = "CH_GL";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, 4, TRUE);
  if (day_of_year (day, 4, year) == easter - 3)
    day += DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_NAEFELS].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
}



void
ch_ge_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*
   Manages all specific holidays celebrated in Switzerland/Gen`eve.
*/
{
  register int day;


  ptr_cc_id = "CH_GE";
  use_other_cc = !use_other_cc;
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARRIVAL_OF_THE_SWISS].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, 4, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 50, 0, year, hd_elems, fday, count);
}



void
ch_ju_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Jura.
*/
{
  ptr_cc_id = "CH_JU";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 23, 6, year, hd_elems, fday, count);
}



void
ch_lu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Luzern.
*/
{
  ptr_cc_id = "CH_LU";
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTINIMAS].ht_text), ptr_cc_id,
	   "+", 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
}



void
ch_ne_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Neuch^atel.
*/
{
  register int day;


  ptr_cc_id = "CH_NE";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MAX, TRUE) + (2 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 3, year, hd_elems, fday, count);
}



void
ch_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Nidwalden.
*/
{
  ptr_cc_id = "CH_NW";
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
}



void
ch_ow_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Obwalden.
*/
{
  ptr_cc_id = "CH_OW";
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, 9, year, hd_elems, fday, count);
}



void
ch_sg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/St Gallen.
*/
{
  ptr_cc_id = "CH_SG";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 16, 10, year, hd_elems, fday, count);
}



void
ch_sh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Schaffhausen.
*/
{
  ptr_cc_id = "CH_SH";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
}



void
ch_so_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Solothurn.
*/
{
  ptr_cc_id = "CH_SO";
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
}



void
ch_sz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Schwyz.
*/
{
  ptr_cc_id = "CH_SZ";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
ch_ti_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Ticino.
*/
{
  ptr_cc_id = "CH_TI";
  use_other_cc = !use_other_cc;
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 50, 0, year, hd_elems, fday, count);
}



void
ch_tg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Thurgau.
*/
{
  ptr_cc_id = "CH_TG";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
}



void
ch_ur_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Uri.
*/
{
  ptr_cc_id = "CH_UR";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
ch_vd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Vaud.
*/
{
  register int day;


  ptr_cc_id = "CH_VD";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MAX, TRUE) + (2 * DAY_MAX) + 1;
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
}



void
ch_vs_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Valais.
*/
{
  ptr_cc_id = "CH_VS";
  use_other_cc = !use_other_cc;
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
}



void
ch_zg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Zug.
*/
{
  ptr_cc_id = "CH_ZG";
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  ch_ai_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
}



void
ch_zh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Switzerland/Z"urich.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "CH_ZH";
  ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  for (i = 3; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 42 - i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 4, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  if (day_of_year (day, 4, year) == easter + 1)
    day += DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_SECHSELAEUTEN].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, 6, TRUE) + DAY_MAX;
  for (i = 0; i < 3; i++)
    holiday (*init_data, detected, _(hd_text[HD_KNABENSCHIESSEN].ht_text),
	     ptr_cc_id, "+", day + i, 9, year, hd_elems, fday, count);
}



void
ci_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in C^ote d'Ivoire.
*/
{
  ptr_cc_id = "CI";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 7, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 7, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ck_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cook Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "CK";
  day = 25;
  i = weekday_of_date (day, 7, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_GOSPEL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = 26;
  i = weekday_of_date (day, 10, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_GOSPEL_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = 25;
  i = weekday_of_date (day, 4, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ANZAC_DAY].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
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
  day = 4;
  i = weekday_of_date (day, 8, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
}



void
cl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Chile.
*/
{
  register int day;


  ptr_cc_id = "CL";
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_IQUIQUE].ht_text),
	   ptr_cc_id, "+", 21, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 19, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 18, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 6, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_RECONCILIATION].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, (year > 1995) ? DIS_HLS_PREF : "+", 11, 9, year,
	   hd_elems, fday, count);
}



void
cm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cameroon.
*/
{
  ptr_cc_id = "CM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_SHEEP_FESTIVAL].ht_text),
	   ptr_cc_id, "+", 21, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "#", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "#", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "#", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 20, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   "+", 11, 2, year, hd_elems, fday, count);
}



void
co_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Colombia.
*/
{
  register int i;
  register int j;
  auto int day;
  auto int month;


  ptr_cc_id = "CO";
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_BOYACA].ht_text),
	   ptr_cc_id, "+", 7, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 20, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, 11, year);
  if (i != DAY_MIN)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  day = 12;
  i = weekday_of_date (day, 10, year);
  if (i != DAY_MIN)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = 11;
  i = weekday_of_date (day, 11, year);
  if (i != DAY_MIN)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_OF_CARTAGENA].ht_text), ptr_cc_id, "+",
	   day, 11, year, hd_elems, fday, count);
  day = 15;
  i = weekday_of_date (day, 8, year);
  if (i != DAY_MIN)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = 6;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i != DAY_MIN)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = 29;
  month = 6;
  i = weekday_of_date (day, month, year);
  if (i != DAY_MIN)
    {
      j = day_of_year (day, month, year) + ((DAY_MAX - i) + 1);
      (void) doy2date (j, (days_of_february (year) == 29), &day, &month);
    }
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", day, month, year, hd_elems, fday, count);
  j = easter + 39;
  (void) doy2date (j, (days_of_february (year) == 29), &day, &month);
  i = weekday_of_date (day, month, year);
  if (i != DAY_MIN)
    j += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", j, 0, year, hd_elems, fday, count);
  j = easter + 60;
  (void) doy2date (j, (days_of_february (year) == 29), &day, &month);
  i = weekday_of_date (day, month, year);
  if (i != DAY_MIN)
    j += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+", j,
	   0, year, hd_elems, fday, count);
  j = easter + 68;
  (void) doy2date (j, (days_of_february (year) == 29), &day, &month);
  i = weekday_of_date (day, month, year);
  if (i != DAY_MIN)
    j += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_FEAST_OF_HEART_JESUS].ht_text),
	   ptr_cc_id, "+", j, 0, year, hd_elems, fday, count);
  if (year > 1917)
    {
      day = 19;
      i = weekday_of_date (day, 3, year);
      if (i != DAY_MIN)
	day += ((DAY_MAX - i) + 1);
      holiday (*init_data, detected, _(hd_text[HD_SAN_JOSE_DAY].ht_text),
	       ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
    }
}



void
cr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Costa Rica.
*/
{
  register int day;


  ptr_cc_id = "CR";
  holiday (*init_data, detected, _(hd_text[HD_GUACANASTE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 7, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_LOS_ANGELES_DAY].ht_text), ptr_cc_id, "#",
	   2, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "#", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "#",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "#", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "#",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "#", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  for (day = 28; day <= dvec[MONTH_MAX - 1]; day++)
    holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	     ptr_cc_id, "#", day, MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1855)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	     ptr_cc_id, "+", 11, 4, year, hd_elems, fday, count);
}



void
cu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cuba.
*/
{
  register int day;


  ptr_cc_id = "CU";
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  for (day = 25; day <= 27; day++)
    holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
}



void
cv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cape Verde.
*/
{
  ptr_cc_id = "CV";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text), ptr_cc_id,
	   "+", 20, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 5, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 12, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
cx_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Christmas Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "CX";
  /*
     I assume that the Australian holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
cy_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cyprus.
*/
{
  ptr_cc_id = "CY";
  holiday (*init_data, detected,
	   _(hd_text[HD_ARCHBISHOP_MAKARIOS_NAME_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 19, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_ARCHBISHOP_MAKARIOS_MEMORIAL_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 3, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 4, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  gr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
cz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Czech Republic.
*/
{
  register int day;

  ptr_cc_id = "CZ";
  if (*init_data)
    *init_data = FALSE;

  /*
   * Several important days which are not state holidays.
   */
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEOPLES_UPRISING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 5, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 1, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_NICHOLAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, 12, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);

  /*
   * I've tried to capture the somewhat complex evolution of the Czech holidays,
   * but so far I have only a little information on the state before year 1975,
   * so if you know more, please let me know.
   *                           Martin Mares <mj@ucw.cz>, September 2003
   *
   * This is a brief sketch of the history:
   *
   * Law:              56/75   141/88  167/90  204/90  218/91  245/00
   * Effective:        23.6.75 21.9.88 10.5.90 18.5.90 31.5.91 9.8.00
   *  1. 1.            +       +       +       +       +       +
   *  Easter           +       +       +       +       +       +
   *  1. 5.            +       +       +       +       +       +
   *  5. 5.
   *  8. 5.                                            +       +
   *  9. 5.            +       +       +       +
   *  5. 7.                            +       +       +       +
   *  6. 7.                                    +       +       +
   * 28. 9.                                                    +
   * 28.10.                    +       +       +       +       +
   * 17.11.                                                    +
   * 24.12.                            +       +       +       +
   * 25.12.            +       +       +       +       +       +
   * 26.12.            +       +       +       +       +       +
   */
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", 1, 1, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", 1, 5, year, hd_elems, fday, count);
  if (year >= 1992)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 8, 5, year, hd_elems, fday, count);
  else
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
  if (year >= 1990)
    {
      holiday (*init_data, detected,
	       _(hd_text[HD_KONSTANTIN_AND_METHODIUS].ht_text), ptr_cc_id,
	       "+", 5, 7, year, hd_elems, fday, count);
      holiday (*init_data, detected,
	       _(hd_text[HD_BURNING_OF_JAN_HUS].ht_text), ptr_cc_id, "+", 6,
	       7, year, hd_elems, fday, count);
    }
  if (year >= 2000)
    holiday (*init_data, detected,
	     _(hd_text[HD_DAY_OF_CZECH_STATEHOOD].ht_text), ptr_cc_id, "+",
	     28, 9, year, hd_elems, fday, count);
  if (year >= 1988)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 28, 10, year, hd_elems, fday, count);
  if (year >= 2000)
    holiday (*init_data, detected,
	     _(hd_text[HD_DAY_OF_STUDENTS_REVOLT].ht_text), ptr_cc_id, "+",
	     17, 11, year, hd_elems, fday, count);
  if (year >= 1990)
    holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	     ptr_cc_id, "+", 24, 12, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, 12, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, 12, year, hd_elems, fday, count);
}



static void
ca_base_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all base holidays celebrated in Canada.
*/
{
  register int day;


  day = 26;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (24, 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_VICTORIA_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  if (!use_other_cc)
    holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	     ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
}



static void
ch_base_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all base holidays celebrated in Switzerland.
*/
{
  ch_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
}
#endif /* !HD_TOP20CC */
