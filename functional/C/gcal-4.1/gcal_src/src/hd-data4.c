/*!
*  \file hd-data4.c
*  \brief All other M-S countries data which is composed into the
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
# include "hd-data5.h"
# include "hd-use.h"
# include "utils.h"
# include "hd-data4.h"



/*
*  All other M-S countries specific holiday management functions.
*/
void
ma_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Morocco.
*/
{
  if (!use_other_cc)
    ptr_cc_id = "MA";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_GREEN_MARCH].ht_text), ptr_cc_id, "+",
	   6, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REUNIFICATION_DAY].ht_text),
	   ptr_cc_id, "+", 14, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 18, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_PROCLAMATION].ht_text), ptr_cc_id, "+",
	   11, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 3, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 23, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 20, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 9, 7, year, hd_elems, fday, count);
}



void
mc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Monaco.
*/
{
  register int day;


  ptr_cc_id = "MC";
  holiday (*init_data, detected, _(hd_text[HD_MI_CAREME_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 19, 0, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  for (day = 26; day <= 27; day++)
    holiday (*init_data, detected, _(hd_text[HD_ST_DEVOTE_DAY].ht_text),
	     ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 39, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 3, 0, year, hd_elems, fday,
	   count);
  for (day = 18; day <= 19; day++)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	     ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 47, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
md_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Republic of Moldova.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "MD";
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
  if (year > 1991)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 27, 8, year, hd_elems, fday, count);
  /*
     The Christians in Moldova compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  d = DAY_MIN;
  if (year != 1924)
    d += greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_ORTHODOX_NEW_YEARS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, d, 0, year, hd_elems, fday, count);
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
      (void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
}



void
mg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Madagascar.
*/
{
  ptr_cc_id = "MG";
  holiday (*init_data, detected,
	   _(hd_text[HD_ST_VINCENT_DE_PAULS_DAY].ht_text), ptr_cc_id, "+", 27,
	   9, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 26, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 29, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 30, MONTH_MAX, year, hd_elems, fday, count);
}



void
mh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Marshall Islands.
*/
{
  ptr_cc_id = "MH";
  holiday (*init_data, detected, _(hd_text[HD_COMPACT_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_FISHERMANS_DAY].ht_text),
	   ptr_cc_id, "+", 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MERCHANT_DAY].ht_text),
	   ptr_cc_id, "+", dvec[9 - 1], 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", 17, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", 4, MONTH_MAX, year, hd_elems, fday, count);
}



void
mk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Former Yugoslav Republic of Macedonia.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "MK";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  /*
     The Christians in Macedonia compute all holiday dates which are
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


      d = 24;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
      (void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
  d = day_of_year (20, 7, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_ST_ELIAHS_DAY].ht_text),
	   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
  d = day_of_year (11, 5, year) + greg_diff;
  holiday (*init_data, detected,
	   _(hd_text[HD_KONSTANTIN_AND_METHODIUS].ht_text), ptr_cc_id, "+", d,
	   0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  if (year > 1992)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 8, 9, year, hd_elems, fday, count);
}



void
ml_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mali.
*/
{
  ptr_cc_id = "ML";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ARMY_COUP_DAY].ht_text),
	   ptr_cc_id, "+", 19, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 20, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 22, 9, year, hd_elems, fday, count);
}



void
mn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mongolia.
*/
{
  register int day;


  ptr_cc_id = "MN";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  for (day = 11; day <= 13; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 26, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
mo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Macau.
*/
{
  ptr_cc_id = "MO";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CAMOES_DAY].ht_text),
	   ptr_cc_id, "+", 10, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 5, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JEAN_BAPTISTE_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  if (year > 1948)
    holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	     ptr_cc_id, "+", DAY_MIN, 10, year, hd_elems, fday, count);
}



void
mp_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Northern Marian Islands (Saipan).
*/
{
  ptr_cc_id = "MP";
  /*
     I assume that no U.S. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CITIZENSHIP_DAY].ht_text),
	   ptr_cc_id, "+", 4, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", 8, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COMPACT_DAY].ht_text),
	   ptr_cc_id, "+", 24, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 9,
	   MONTH_MAX, year, hd_elems, fday, count);
}



void
mq_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Martinique.
*/
{
  register int i;


  if (!use_other_cc)
    ptr_cc_id = "MQ";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  for (i = 5; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 14, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 27, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTOR_SCHOELCHER_DAY].ht_text),
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
mr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mauritania.
*/
{
  ptr_cc_id = "MR";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 10, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 28, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 8, 3, year, hd_elems, fday, count);
}



void
ms_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Montserrat.
*/
{
  register int day;


  ptr_cc_id = "MS";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PATRICKS_DAY].ht_text),
	   ptr_cc_id, "+", 17, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1966)
    {
      day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
    }
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
mt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Malta.
*/
{
  register int i;


  ptr_cc_id = "MT";
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_VICTORIES_DAY].ht_text), ptr_cc_id, "+",
	   8, 9, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ST_PAULS_SHIPSWRECK].ht_text),
	   ptr_cc_id, "+", 10, 2, year, hd_elems, fday, count);
  for (i = 4; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 21, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 13, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1918)
    holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	     ptr_cc_id, "+", 7, 6, year, hd_elems, fday, count);
  if (year > 1943)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", dvec[3 - 1], 3, year, hd_elems, fday, count);
}



void
mu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mauritius.
*/
{
  register int day;


  ptr_cc_id = "MU";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FATHER_LEVAL_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 9, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 12, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
}



void
mv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Maldives.
*/
{
  register int day;


  ptr_cc_id = "MV";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  for (day = 26; day <= 27; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  for (day = 11; day <= 12; day++)
    holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	     ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 3, 11, year, hd_elems, fday, count);
}



void
mw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Malawi.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "MW";
  day = 15;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_J_CHILEMBWE_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
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
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 14;
  i = weekday_of_date (day, 6, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = 3;
  i = weekday_of_date (day, 3, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = 6;
  i = weekday_of_date (day, 7, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
my_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Malaysia.
*/
{
  register int day;


  ptr_cc_id = "MY";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[8 - 1], 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
mz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Mozambique.
*/
{
  ptr_cc_id = "MZ";
  holiday (*init_data, detected, _(hd_text[HD_MAPUTO_CITY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 10, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 25, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 3, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 7, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 7, 4, year, hd_elems, fday, count);
}



void
na_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Namibia.
*/
{
  ptr_cc_id = "NA";
  holiday (*init_data, detected, _(hd_text[HD_CASSINGA_DAY].ht_text),
	   ptr_cc_id, "+", 4, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_GOODWILL].ht_text),
	   ptr_cc_id, "+", 7, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text), ptr_cc_id,
	   "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text), ptr_cc_id,
	   "+", 26, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HUMAN_RIGHTS_DAY].ht_text),
	   ptr_cc_id, "+", 10, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 21, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text), ptr_cc_id,
	   "+", 25, 5, year, hd_elems, fday, count);
}



void
nc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in New Caledonia.
*/
{
  ptr_cc_id = "NC";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
ne_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Niger.
*/
{
  ptr_cc_id = "NE";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ARMY_COUP_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 3, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 24, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 18, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
nf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Norfolk Island.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "NF";
  /*
     I assume that the Australian holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  au_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 8;
  i = weekday_of_date (day, 6, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_BOUNTY_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = 6;
  i = weekday_of_date (day, 3, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id, "+",
	   day, 3, year, hd_elems, fday, count);
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
ng_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Nigeria.
*/
{
  ptr_cc_id = "NG";
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
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
ni_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Nicaragua.
*/
{
  ptr_cc_id = "NI";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 19, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, "+", 30, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SAN_JACINTO_DAY].ht_text),
	   ptr_cc_id, "+", 14, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
nm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Myanmar (Burma).
*/
{
  ptr_cc_id = "NM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_DRY_SEASON_CELEBRATION].ht_text), ptr_cc_id, "+", 10,
	   3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 27, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FARMERS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 4, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_UNION_DAY].ht_text), ptr_cc_id,
	   "+", 12, 2, year, hd_elems, fday, count);
}



void
no_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Norway.
*/
{
  if (!use_other_cc)
    ptr_cc_id = "NO";
  holiday (*init_data, detected, _(hd_text[HD_LAPP_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, 2, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_UNION_WITH_SWEDEN_DISSOLVED].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 7, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_OLAVS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_OLAVS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 28, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 21, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 17, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter - DAY_MAX, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_UNITED_NATIONS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 8, 3, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, 8, 5, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
np_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Nepal.
*/
{
  ptr_cc_id = "NP";
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   15, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 29, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 19, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 11, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 29, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
nr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Nauru.
*/
{
  ptr_cc_id = "NR";
  holiday (*init_data, detected, _(hd_text[HD_ANGAM_DAY].ht_text),
	   ptr_cc_id, "+", 26, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   17, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MIN - 1], MONTH_MIN, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
nu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Niue.
*/
{
  ptr_cc_id = "NU";
  use_other_cc = !use_other_cc;
  nz_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
om_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Oman.
*/
{
  ptr_cc_id = "OM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_SULTAN_QABOOS_BIRTHDAY].ht_text), ptr_cc_id, "+", 19,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 18, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
pa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Panama.
*/
{
  register int i;


  ptr_cc_id = "PA";
  holiday (*init_data, detected, _(hd_text[HD_DIA_DE_LOS_MAESTROS].ht_text),
	   ptr_cc_id, "+", 11, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_DIA_DEL_PADRE].ht_text),
	   ptr_cc_id, "+", 16, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_COLON_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 5, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PANAMA_CITY_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 46, 0, year, hd_elems, fday, count);
  for (i = 2; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, easter - 46 - i, 0, year, hd_elems,
	     fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text), ptr_cc_id,
	   "+", 4, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 28, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 9, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, "+", 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SEPARATION_DAY].ht_text),
	   ptr_cc_id, "+", 3, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
pe_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Peru.
*/
{
  register int day;


  ptr_cc_id = "PE";
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_ANGAMOS].ht_text),
	   ptr_cc_id, "+", 8, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 5, year, hd_elems, fday, count);
  for (day = 28; day <= 29; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_ROSE_OF_LIMA].ht_text),
	   ptr_cc_id, "+", 30, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
pf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in French Polynesia.
*/
{
  ptr_cc_id = "PF";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_INTERNAL_AUTONOMY_DAY].ht_text),
	   ptr_cc_id, "+", 5, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MISSIONARY_DAY].ht_text),
	   ptr_cc_id, "+", 8, 9, year, hd_elems, fday, count);
}



void
pg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Papua New Guinea.
*/
{
  ptr_cc_id = "PG";
  holiday (*init_data, detected, _(hd_text[HD_ST_MICHAELS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 9, year, hd_elems, fday, count);
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
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 16, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 23, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
ph_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Philippines.
*/
{
  ptr_cc_id = "PH";
  holiday (*init_data, detected, _(hd_text[HD_PEOPLE_POWER_DAY].ht_text),
	   ptr_cc_id, "#", 22, 2, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BATAAN_DAY].ht_text),
	   ptr_cc_id, "+", 9, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_ELECTIONS_FOR_NATIONAL_ASSEMBLY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 8, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MANILA_DAY].ht_text), ptr_cc_id,
	   "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PHILIPPINE_USA_FRIENDSHIP].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text), ptr_cc_id,
	   "+", dvec[11 - 1], 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RIZAL_DAY].ht_text), ptr_cc_id,
	   "+", 30, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 12, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", dvec[8 - 1], 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
}



void
pk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Pakistan.
*/
{
  ptr_cc_id = "PK";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_DEATH_OF_QAID_I_AZAM].ht_text),
	   ptr_cc_id, "+", 11, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_QAID_I_AZAMS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 27, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 26, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_DEFENDERS_OF_MOTHERLAND_DAY].ht_text), ptr_cc_id, "+",
	   6, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 14, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 23, 3, year, hd_elems, fday, count);
}



void
pl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Poland.
*/
{
  ptr_cc_id = "PL";
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
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 3,
	   5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 26, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  if ((year > 1944) && (year < 1990))
    {
      holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	       ptr_cc_id, "+", 22, 7, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	       ptr_cc_id, "+", 9, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 8, 3, year, hd_elems, fday, count);
    }
  if (year > 1989)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 11, 11, year, hd_elems, fday, count);
}



void
pm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Saint-Pierre and Miquelon.
*/
{
  ptr_cc_id = "PM";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
}



void
pn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Pitcairn.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "PN";
  /*
     I assume that the U.K. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 28;
  i = weekday_of_date (day, 4, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_BOUNTY_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
pr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Puerto Rico.
*/
{
  ptr_cc_id = "PR";
  /*
     I assume that no U.S. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_E_HOSTOS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 9, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_J_BARBOSAS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 27, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_J_DIEGOS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 16, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_L_RIVERAS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 17, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   25, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", 19, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 22, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JEAN_BAPTISTE_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
pw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Palau.
*/
{
  ptr_cc_id = "PW";
  /*
     I assume that no U.S. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 9,
	   7, year, hd_elems, fday, count);
}



void
py_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Paraguay.
*/
{
  ptr_cc_id = "PY";
  holiday (*init_data, detected, _(hd_text[HD_ASUNCION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOQUERON_BATTLE_DAY].ht_text),
	   ptr_cc_id, "+", 29, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMISTICE_DAY].ht_text),
	   ptr_cc_id, "+", 12, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 16, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
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
qa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Qatar.
*/
{
  ptr_cc_id = "QA";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_AMIRS_SUCCESSION].ht_text), ptr_cc_id,
	   "+", 27, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 3, 9, year, hd_elems, fday, count);
}



void
re_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in R'eunion.
*/
{
  ptr_cc_id = "RE";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
ro_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Romania.
*/
{
  register int day;


  ptr_cc_id = "RO";
  /*
     The Christians in Romania compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches new calendar.
   */
  easter = orthodox_easter (year, NULL, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
rw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Rwanda.
*/
{
  ptr_cc_id = "RW";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_KAMARAMPAKA_DAY].ht_text),
	   ptr_cc_id, "+", 25, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_MOURNING_DAY].ht_text),
	   ptr_cc_id, "+", 7, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 5, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
sa_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Saudi Arabia.
*/
{
  ptr_cc_id = "SA";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 9, year, hd_elems, fday, count);
}



void
sb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Solomon Islands.
*/
{
  register int day;


  ptr_cc_id = "SB";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
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
	   ptr_cc_id, "+", 7, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
sc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Seychelles.
*/
{
  ptr_cc_id = "SC";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 5, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
sd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Sudan.
*/
{
  ptr_cc_id = "SD";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 3, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEOPLES_UPRISING_DAY].ht_text),
	   ptr_cc_id, "+", 6, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 30, 6, year, hd_elems, fday, count);
}



void
se_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Sweden.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "SE";
  holiday (*init_data, detected, _(hd_text[HD_ST_CANUTES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 13, MONTH_MIN, year, hd_elems, fday,
	   count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_NAME_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 28, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_THE_CROWN_PRINCESSES_NAME_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 12, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVE_OF_EPIPHANY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 5, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_WHITSUN_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter + 48, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_THE_CROWN_PRINCESSES_BIRTHDAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 14, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_NAME_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 8, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GUSTAVUS_ADOLPHUS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NOBEL_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 10, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_LUCIAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 13, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_HOLY_INNOCENTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 28, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_FOOLS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
  day = day_of_year (31, 10, year);
  i = weekday_of_date (31, 10, year);
  if (i != 6)
    {
      if (i < 6)
	day += ((DAY_MAX - i) - 1);
      else
	day += 6;
    }
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[4 - 1], 4, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_ANNUNCIATION_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 25, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 3, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (20, 6, year, 6, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day - 1, 6, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, DAY_MAX, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, year >= 2005 ? "+" : DIS_HLS_PREF2, 6, 6, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 47, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VALBORGS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[4 - 1], 4, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, year < 2005 ? "+" : DIS_HLS_PREF2, easter + 50, 0, year,
	   hd_elems, fday, count);
}



void
sg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Singapore.
*/
{
  register int day;


  ptr_cc_id = "SG";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  day = 25;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 9;
  if (weekday_of_date (day, 8, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, MONTH_MIN, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
}



void
sh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Saint Helena.
*/
{
  register int day;


  ptr_cc_id = "SH";
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
}



void
si_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Slovenia.
*/
{
  register int day;


  ptr_cc_id = "SI";
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CULTURE_DAY].ht_text),
	   ptr_cc_id, "+", 8, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	     ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 25, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_RESISTANCE_DAY].ht_text), ptr_cc_id, "+", 27,
	   4, year, hd_elems, fday, count);
  for (day = DAY_MIN; day <= 2; day++)
    holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	     ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1516)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, "+", dvec[10 - 1], 10, year, hd_elems, fday, count);
}



void
sj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Svalbard and Jan Mayen Islands.
*/
{
  ptr_cc_id = "SJ";
  use_other_cc = !use_other_cc;
  no_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
sk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Slovakia.
*/
{
  register int day;


  ptr_cc_id = "SK";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "#", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_KONSTANTIN_AND_METHODIUS].ht_text), ptr_cc_id, "+", 5,
	   7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_RESISTANCE_DAY].ht_text), ptr_cc_id, "+", 29,
	   8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_SEVEN_SORROWS].ht_text), ptr_cc_id, "+",
	   15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "#", 8, 5, year, hd_elems, fday, count);
}



void
sl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Sierra Leone.
*/
{
  ptr_cc_id = "SL";
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
	   ptr_cc_id, "+", 27, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
sm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in San Marino.
*/
{
  ptr_cc_id = "SM";
  holiday (*init_data, detected, _(hd_text[HD_REGENCY_EXCHANGE].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 4, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_REGENCY_EXCHANGE].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id, "+", 3,
	   9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1739)
    holiday (*init_data, detected, _(hd_text[HD_ST_AGATA_DAY].ht_text),
	     ptr_cc_id, "+", 5, 2, year, hd_elems, fday, count);
  if (year > 1905)
    holiday (*init_data, detected, _(hd_text[HD_ARENGO_ANNIVERSARY].ht_text),
	     ptr_cc_id, "+", 25, 3, year, hd_elems, fday, count);
  if (year > 1943)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 28, 7, year, hd_elems, fday, count);
}



void
sn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Senegal.
*/
{
  ptr_cc_id = "SN";
  holiday (*init_data, detected,
	   _(hd_text[HD_CONFEDERAL_AGREEMENT_DAY].ht_text), ptr_cc_id, "+",
	   DAY_MIN, 2, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 20, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 4, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
so_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Somalia.
*/
{
  ptr_cc_id = "SO";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "#", 26, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 7, year, hd_elems, fday, count);
}



void
sr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Suriname.
*/
{
  ptr_cc_id = "SO";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 25, 2, year, hd_elems, fday, count);
}



void
st_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Sao Tom'e and Principe.
*/
{
  ptr_cc_id = "ST";
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 12, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 3, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
sv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in El Salvador.
*/
{
  register int day;


  ptr_cc_id = "SV";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[6 - 1], 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GUADALUPE_VIRGINS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 12, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_HOLY_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 4, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 5, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_TEACHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 22, 6, year, hd_elems, fday, count);
  for (day = 4; day <= 6; day++)
    holiday (*init_data, detected, _(hd_text[HD_TRANSFIGURATION_DAY].ht_text),
	     ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
}



void
sy_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Syrian Arab Republic.
*/
{
  ptr_cc_id = "SY";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ARAB_LEAGUE_DAY].ht_text),
	   ptr_cc_id, "+", 22, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 17, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 6, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 22, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
sz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Swaziland.
*/
{
  ptr_cc_id = "SZ";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text),
	   ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 6, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 22, 7, year, hd_elems, fday, count);
}
#endif /* !HD_TOP20CC */
