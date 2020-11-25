/*!
*  \file hd-data3.c
*  \brief All other D-L countries data which is composed into the
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
# include "hd-astro.h"
# include "hd-defs.h"
# include "hd-data.h"
# include "hd-data1.h"
# include "hd-data2.h"
# include "hd-data4.h"
# include "hd-data5.h"
# include "hd-use.h"
# include "utils.h"
# include "hd-data3.h"



/*
*  All other D-L countries specific holiday management functions.
*/
void
de_by_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Bavaria.
*/
{
  ptr_cc_id = "DE_BY";
  use_other_cc = !use_other_cc;
  de_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "#", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEACE_FESTIVAL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 8, 8, year, hd_elems, fday, count);
}



void
de_bb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Brandenburg.
*/
{
  if (!use_other_cc)
    {
      ptr_cc_id = "DE_BB";
      use_other_cc = !use_other_cc;
      de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
      use_other_cc = !use_other_cc;
    }
  else
    de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  if (year > 1516 && year != 2017)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, "+", dvec[10 - 1], 10, year, hd_elems, fday, count);
  if (year > 1989)
    holiday (*init_data, detected, _(hd_text[HD_GERMAN_UNITY_DAY].ht_text),
	     ptr_cc_id, "+", 3, 10, year, hd_elems, fday, count);
}



void
de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Berlin.
*/
{
  register int day;


  if (!use_other_cc)
    {
      ptr_cc_id = "DE_BE";
      de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
    }
  day = weekday_of_date (DAY_MIN, 11, year);
  day = (day < 3) ? 18 - day : 25 - day;
  holiday (*init_data, detected,
	   _(hd_text[HD_DAY_OF_PRAYER_AND_REPENTANCE].ht_text), ptr_cc_id,
	   (year > 1994) ? DIS_HLS_PREF : "+", day, 11, year, hd_elems, fday,
	   count);
}



void
de_hb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Bremen.
*/
{
  ptr_cc_id = "DE_HB";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
de_bw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Baden-W"urttemberg.
*/
{
  ptr_cc_id = "DE_BW";
  use_other_cc = !use_other_cc;
  de_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
de_hh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Hamburg.
*/
{
  ptr_cc_id = "DE_HH";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
de_he_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Hessen.
*/
{
  ptr_cc_id = "DE_HE";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
}



void
de_mv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Mecklenburg-Vorpommern.
*/
{
  ptr_cc_id = "DE_MV";
  use_other_cc = !use_other_cc;
  de_bb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
de_ni_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Lower Saxony.
*/
{
  ptr_cc_id = "DE_NI";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
de_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/North Rhine-Westphalia.
*/
{
  if (!use_other_cc)
    {
      ptr_cc_id = "DE_NW";
      use_other_cc = !use_other_cc;
      de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
      use_other_cc = !use_other_cc;
      de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
}



void
de_rp_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Rhineland Palatinate.
*/
{
  ptr_cc_id = "DE_RP";
  use_other_cc = !use_other_cc;
  de_nw_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
de_st_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Saxony-Anhalt.
*/
{
  ptr_cc_id = "DE_ST";
  use_other_cc = !use_other_cc;
  de_bb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
de_sh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Schleswig-Holstein.
*/
{
  ptr_cc_id = "DE_SH";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
}



void
de_sl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Saarland.
*/
{
  ptr_cc_id = "DE_SL";
  use_other_cc = !use_other_cc;
  de_be_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
}



void
de_sn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Saxony.
*/
{
  register int day;


  ptr_cc_id = "DE_SN";
  use_other_cc = !use_other_cc;
  de_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, easter + 60, 0, year, hd_elems, fday, count);
  day = weekday_of_date (DAY_MIN, 11, year);
  day = (day < 3) ? 18 - day : 25 - day;
  holiday (*init_data, detected,
	   _(hd_text[HD_DAY_OF_PRAYER_AND_REPENTANCE].ht_text), ptr_cc_id,
	   "+", day, 11, year, hd_elems, fday, count);
  if (year > 1516 && year != 2017)
    holiday (*init_data, detected, _(hd_text[HD_REFORMATION_DAY].ht_text),
	     ptr_cc_id, "+", dvec[10 - 1], 10, year, hd_elems, fday, count);
  if (year > 1989)
    holiday (*init_data, detected, _(hd_text[HD_GERMAN_UNITY_DAY].ht_text),
	     ptr_cc_id, "+", 3, 10, year, hd_elems, fday, count);
}



void
de_th_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Germany/Thuringia.
*/
{
  ptr_cc_id = "DE_TH";
  use_other_cc = !use_other_cc;
  de_bb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, easter + 60, 0, year, hd_elems, fday, count);
}



void
dj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Djibouti.
*/
{
  ptr_cc_id = "DJ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 27, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
dk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Denmark.
*/
{
  register int day;


  if (!use_other_cc)
    ptr_cc_id = "DK";
  holiday (*init_data, detected, _(hd_text[HD_PRAYER_DAY].ht_text),
	   ptr_cc_id, "+", easter + 26, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "#", 5,
	   6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTINIMAS].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - DAY_MAX, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 16, 4, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
dm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Dominica.
*/
{
  register int i;


  ptr_cc_id = "DM";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  for (i = 2; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  i = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", i, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  i = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "+", i, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MERCHANT_DAY].ht_text),
	   ptr_cc_id, "#", 2, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1977)
    for (i = 3; i <= 4; i++)
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", i, 11, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
do_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Dominican Republic.
*/
{
  ptr_cc_id = "DO";
  holiday (*init_data, detected, _(hd_text[HD_DUARTES_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MIN, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_ALTAGRACIA_DAY].ht_text), ptr_cc_id, "+",
	   21, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_LAS_MERCEDES_DAY].ht_text), ptr_cc_id,
	   "+", 24, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+", 6,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 27, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RESTORATION_DAY].ht_text),
	   ptr_cc_id, "+", 16, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
}



void
dz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Algeria.
*/
{
  ptr_cc_id = "DZ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 5, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 19, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
}



void
ec_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Ecuador.
*/
{
  register int i;


  ptr_cc_id = "EC";
  holiday (*init_data, detected, _(hd_text[HD_PICHINCHA_DAY].ht_text),
	   ptr_cc_id, "+", 24, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_OF_CUENCA].ht_text), ptr_cc_id, "+", 3,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_OF_GUAYAQUIL].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 9, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_OF_QUITO].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SIMON_BOLIVAR_DAY].ht_text),
	   ptr_cc_id, "+", 24, 7, year, hd_elems, fday, count);
  i = eval_holiday (dvec[6 - 1], 6, year, 5, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", i, 6, year, hd_elems, fday, count);
  for (i = 2; i; i--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
}



void
ee_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Estonia.
*/
{
  ptr_cc_id = "EE";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
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
	   ptr_cc_id, "+", 24, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, "+", 23, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text), ptr_cc_id,
	   "+", easter + 49, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RESTORATION_DAY].ht_text),
	   ptr_cc_id, "+", 20, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_WHIT_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 50, 0, year, hd_elems, fday, count);
}



void
eg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Egypt.
*/
{
  ptr_cc_id = "EG";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_UNION_DAY].ht_text),
	   ptr_cc_id, "+", 22, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SNIFF_THE_BREEZE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 22, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SINAI_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 6, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", 18, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 23, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 23, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 23, MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1974)
    holiday (*init_data, detected, _(hd_text[HD_SUEZ_VICTORY_DAY].ht_text),
	     ptr_cc_id, "+", 24, 10, year, hd_elems, fday, count);
  if (year > 283)
    {
      auto int greg_diff;
      auto int d;


      /*
         The Coptic Christians in Egyt compute all holiday dates which are
         depending on Easter Sunday's date in the way done by the
         Orthodox Christian Eastern churches old calendar.
       */
      easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
      if ((year != 284) && (year != 1924))
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
		   ptr_cc_id, DIS_HLS_PREF2, d, m, year, hd_elems, fday,
		   count);
	}
      d = day_of_year (29, 8, year) + ((year & 3) == 3) + greg_diff;
      /*
         Calculate the year number used in the Coptic calendar.
       */
      sprintf (s2, "%s %d", _(hd_text[HD_COPTIC_NEW_YEARS_DAY].ht_text),
	       year - 283);
      holiday (*init_data, detected, s2, ptr_cc_id, DIS_HLS_PREF2, d, 0, year,
	       hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text),
	       ptr_cc_id, DIS_HLS_PREF2, easter + 49, 0, year, hd_elems, fday,
	       count);
    }
}



void
eh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Western Sahara.
*/
{
  ptr_cc_id = "EH";
  use_other_cc = !use_other_cc;
  ma_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
er_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Eritrea.
*/
{
  ptr_cc_id = "ER";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_ANNIVERSARY_OF_ARMED_STRUGGLE].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 24, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 20, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 8, 3, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "#", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "#", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  if (year > 283)
    {
      auto int greg_diff;
      auto int d;


      /*
         The Coptic Christians in Eritrea compute all holiday dates which are
         depending on Easter Sunday's date in the way done by the
         Orthodox Christian Eastern churches old calendar.
       */
      easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
      if ((year != 284) && (year != 1924))
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
		   ptr_cc_id, "#", d, m, year, hd_elems, fday, count);
	}
      d = day_of_year (29, 8, year) + ((year & 3) == 3) + greg_diff;
      /*
         Calculate the year number used in the Coptic calendar.
       */
      sprintf (s2, "%s %d", _(hd_text[HD_COPTIC_NEW_YEARS_DAY].ht_text),
	       year - 283);
      holiday (*init_data, detected, s2, ptr_cc_id, "#", d, 0, year, hd_elems,
	       fday, count);
      holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	       ptr_cc_id, "#", easter, 0, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	       ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
    }
}



void
et_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Ethiopia.
*/
{
  register int i;
  auto int greg_diff;
  auto int d;
  auto int m;
  auto int y;


  ptr_cc_id = "ET";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if (year > 1990)
    holiday (*init_data, detected,
	     _(hd_text[HD_DOWNFALL_OF_THE_DERGUE].ht_text), ptr_cc_id, "+",
	     28, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	   ptr_cc_id, "+", 2, 3, year, hd_elems, fday, count);
  /*
     The Ethiopian Christians in Ethiopia compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar.
   */
  easter = orthodox_easter (year, &greg_diff, 1924, 3, 10, 22);
  if (year != 1924)
    {
      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
  d = day_of_year (6, MONTH_MIN, year);
  if (year > 1924)
    d += greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
  d = day_of_year (29, 8, year) + (days_of_february (year) == 29) + greg_diff;
  /*
     Calculate the year number used in the Ethiopic calendar.
   */
  sprintf (s2, "%s %d", _(hd_text[HD_ETHIOPIC_NEW_YEARS_DAY].ht_text),
	   year - 7);
  holiday (*init_data, detected, s2, ptr_cc_id, "+", d, 0, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CROSS_RAISING_DAY].ht_text),
	   ptr_cc_id, "+", d + 16, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
fi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Finland.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "FI";
  holiday (*init_data, detected, _(hd_text[HD_ALEKSIS_KIVI_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 10, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_SWEDEN_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, 11, year, hd_elems, fday, count);
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
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 11, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_FATHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 3, 0, year, hd_elems, fday,
	   count);
  day = eval_holiday (20, 6, year, 6, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day - 1, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - DAY_MAX, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text), ptr_cc_id,
	   "+", easter + 49, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 19, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_VALENTINES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
fj_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Fiji.
*/
{
  register int day;


  ptr_cc_id = "FJ";
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
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  /*
     The following holidays are oftenly observed on another day
     by the Government, but the rules change almost every year!
   */
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 10, 10, year, hd_elems, fday, count);
  day = eval_holiday (dvec[5 - 1], 5, year, 5, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_RATU_SUKUMA_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   28, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text),
	   ptr_cc_id, "+", 7, 3, year, hd_elems, fday, count);
}



void
fk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Falkland Islands.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "FK";
  /*
     I assume that the U.K. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 14;
  i = weekday_of_date (day, 6, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
fm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Federation States of Micronesia.
*/
{
  register int day;


  ptr_cc_id = "FM";
  holiday (*init_data, detected, _(hd_text[HD_CHARTER_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 23, 9, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_YAP_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 11, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "#", 8,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CULTURE_DAY].ht_text),
	   ptr_cc_id, "#", dvec[3 - 1], 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_PROCLAMATION].ht_text), ptr_cc_id, "+",
	   10, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "#", 11, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 4, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 11, year, 4, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_UNITED_NATIONS_DAY].ht_text),
	   ptr_cc_id, "+", 24, 10, year, hd_elems, fday, count);
}



void
fo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Faroes.
*/
{
  ptr_cc_id = "FO";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 5, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text), ptr_cc_id,
	   "+", 25, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRAYER_DAY].ht_text), ptr_cc_id,
	   "+", easter + 26, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_OLAVS_DAY].ht_text),
	   ptr_cc_id, "+", 29, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_OLAVS_EVE].ht_text),
	   ptr_cc_id, "+", 28, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ga_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Gabon.
*/
{
  register int day;


  ptr_cc_id = "GA";
  day = DAY_MIN;
  if (weekday_of_date (day, 11, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 6;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 15;
  if (weekday_of_date (day, 8, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = 16;
  if (weekday_of_date (day, 8, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
gb_en_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Great Britain/England and Wales.
*/
{
  register int day;


  ptr_cc_id = "GB_EN";
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
gb_ni_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Great Britain/Northern Ireland.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "GB_NI";
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = 12;
  i = weekday_of_date (day, 7, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_BATTLE_OF_BOYNE].ht_text),
	   ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  day = 17;
  i = weekday_of_date (day, 3, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_ST_PATRICKS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
gb_sl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Great Britain/Scotland.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "GB_SL";
  use_other_cc = !use_other_cc;
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  day = 2;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += 2;
  else if (i == DAY_MIN)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BURNS_NIGHT].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, MONTH_MIN, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
gd_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Grenada.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "GD";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE) + DAY_MAX;
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", day + i, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  for (i = 0; i < 2; i++)
    holiday (*init_data, detected,
	     _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text), ptr_cc_id, "+",
	     day + i, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1973)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 7, 2, year, hd_elems, fday, count);
  if (year > 1982)
    holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	     ptr_cc_id, "+", 25, 10, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
ge_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Georgia.
*/
{
  register int i;
  auto int greg_diff;
  auto int d;
  auto int m;
  auto int y;


  ptr_cc_id = "GE";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_SVETITSKHOVLOBA].ht_text),
	   ptr_cc_id, "+", 14, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  /*
     The Christians in Georgia compute all holiday dates which are
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
      d = 25;
      m = MONTH_MAX;
      y = year - 1;
      for (i = 0; i < greg_diff; i++)
	(void) next_date (&d, &m, &y);
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
    }
  d = day_of_year (6, MONTH_MIN, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
  d = day_of_year (15, 8, year) + greg_diff;
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", d, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, "+", 13, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_GEORGES_DAY].ht_text),
	   ptr_cc_id, "+", 23, 11, year, hd_elems, fday, count);
  if (year > 1991)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 26, 5, year, hd_elems, fday, count);
}



void
gf_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in French Guiana.
*/
{
  ptr_cc_id = "GF";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CAYENNE_FESTIVAL].ht_text),
	   ptr_cc_id, "+", 16, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 46, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 10, 6, year, hd_elems, fday, count);
}



void
gh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Ghana.
*/
{
  ptr_cc_id = "GH";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_FARMERS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MAX, year, hd_elems, fday, count);
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
	   ptr_cc_id, "+", 6, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  if (year > 1978)
    holiday (*init_data, detected,
	     _(hd_text[HD_PEOPLES_UPRISING_DAY].ht_text), ptr_cc_id, "+", 4,
	     6, year, hd_elems, fday, count);
}



void
gi_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Gibraltar.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "GI";
  day = eval_holiday (dvec[5 - 1], 5, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (dvec[8 - 1], 8, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
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
  day = 9;
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_COMMONWEALTH_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = DAY_MIN;
  if (weekday_of_date (day, 5, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = 10;
  if (weekday_of_date (day, 9, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 9, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX + 2;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
gl_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Greenland.
*/
{
  ptr_cc_id = "GL";
  use_other_cc = !use_other_cc;
  dk_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 21, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
}



void
gm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Gambia.
*/
{
  ptr_cc_id = "GM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 18, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
gn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guinea.
*/
{
  ptr_cc_id = "GN";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_REFERENDUM_DAY].ht_text),
	   ptr_cc_id, "+", 28, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 3, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 2, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 11, year, hd_elems, fday, count);
}



void
gp_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guadeloupe.
*/
{
  ptr_cc_id = "GP";
  use_other_cc = !use_other_cc;
  mq_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_ST_BARTHOLOMEW_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_LAURENTIUS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 10, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTINIMAS].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 11, 11, year, hd_elems, fday, count);
}



void
gq_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Equatorial Guinea.
*/
{
  ptr_cc_id = "GQ";
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 3, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HUMAN_RIGHTS_DAY].ht_text),
	   ptr_cc_id, "+", 10, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRESIDENTS_DAY].ht_text),
	   ptr_cc_id, "+", 5, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
gr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Greece.
*/
{
  if (!use_other_cc)
    ptr_cc_id = "GR";
  /*
     The Christians in Greece compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches new calendar.
   */
  easter = orthodox_easter (year, NULL, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_ASH_MONDAY].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CROSS_RAISING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 14, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_OUR_THEOTOKOS].ht_text), ptr_cc_id, "+", 26,
	   MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_TUESDAY].ht_text),
	   ptr_cc_id, "#", easter + 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "#", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 25, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 3, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 28, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SCHOOL_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 30, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
gs_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in South Georgia and South Sandwich Islands.
*/
{
  ptr_cc_id = "GS";
  gb_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
}



void
gt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guatemala.
*/
{
  ptr_cc_id = "GT";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", dvec[6 - 1], 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HOLY_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 4, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 20, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
}



void
gu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guam.
*/
{
  register int day;


  ptr_cc_id = "GU";
  /*
     I assume that no U.S. holiday observance rule is valid in this
     country/territory, if not, please let me know!
   */
  observe_us_hdy = SASA_SUSU;
  us_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_OUR_LADY_OF_CAMARIN_DAY].ht_text), ptr_cc_id, "+", 2,
	   11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 3, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 21, 7, year, hd_elems, fday, count);
}



void
gw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guinea-Bissau.
*/
{
  ptr_cc_id = "GW";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 20, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 10, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_RECONCILIATION].ht_text),
	   ptr_cc_id, "+", 14, 1, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
gy_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Guyana.
*/
{
  register int day;


  ptr_cc_id = "GY";
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
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CARICOM_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 26, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", 23, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 5, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
hk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Hong Kong.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "HK";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_SAR_ESTABLISHMENT_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 7, year, hd_elems, fday, count);
  day = 26;
  i = weekday_of_date (day, MONTH_MAX, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_SATURDAY].ht_text),
	   ptr_cc_id, "+", easter - 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MIN, year, hd_elems, fday, count);
  if (year == 1999)
    holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text),
	     ptr_cc_id, "#", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	     fday, count);
  if (year > 1948)
    for (day = DAY_MIN; day <= 2; day++)
      holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	       ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
}



void
hm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Heard and Mc Donald Islands.
*/
{
  ptr_cc_id = "HM";
  use_other_cc = !use_other_cc;
  fr_hdy (init_data, detected, easter, year, hd_elems, fday, count);
  use_other_cc = !use_other_cc;
}



void
hn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Honduras.
*/
{
  ptr_cc_id = "HN";
  holiday (*init_data, detected, _(hd_text[HD_MORAZAN_DAY].ht_text),
	   ptr_cc_id, "+", 3, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_PANAMERICAN_DAY].ht_text),
	   ptr_cc_id, "+", 14, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 21, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_COLUMBUS_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
hr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Croatia.
*/
{
  ptr_cc_id = "HR";
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
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", 5, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1943)
    holiday (*init_data, detected,
	     _(hd_text[HD_ANNIVERSARY_OF_ARMED_STRUGGLE].ht_text), ptr_cc_id,
	     "+", 22, 6, year, hd_elems, fday, count);
  if (year > 1991)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 30, 5, year, hd_elems, fday, count);
}



void
ht_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Haiti.
*/
{
  ptr_cc_id = "HT";
  holiday (*init_data, detected, _(hd_text[HD_AMERICAS_DAY].ht_text),
	   ptr_cc_id, "+", 14, 4, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected,
	   _(hd_text[HD_RESPECT_FOR_THE_AGED_DAY].ht_text), ptr_cc_id, "+", 2,
	   MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DEATH_OF_H_CHRISTOPHE].ht_text),
	   ptr_cc_id, "+", 8, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DEATH_OF_J_DESSALINES].ht_text),
	   ptr_cc_id, "+", 17, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DEATH_OF_T_LOUVERTURE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 7, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 18, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", 5, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text), ptr_cc_id,
	   "+", 18, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_ANNUNCIATION_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 25, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_ANDREWS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, dvec[11 - 1], 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 22, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_UNITED_NATIONS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, 10, year, hd_elems, fday, count);
}



void
hu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Hungary.
*/
{
  ptr_cc_id = "HU";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   20, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 15, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 23, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year < 1990)
    {
      if (year > 1916)
	holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
		 ptr_cc_id, "+", 7, 11, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
		 ptr_cc_id, "+", 4, 4, year, hd_elems, fday, count);
    }

  if (year > 1999)
	holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
		 ptr_cc_id, "+", 1, 11, year, hd_elems, fday, count);

  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
id_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Indonesia.
*/
{
  ptr_cc_id = "ID";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_KARTINI_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 21, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 39, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1944)
    {
      holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 5, 10, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", 17, 8, year, hd_elems, fday, count);
    }
}



void
ie_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Ireland.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "IE";
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
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
	   ptr_cc_id, "#", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_JUNE_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = DAY_MIN;
  i = weekday_of_date (day, MONTH_MIN, year);
  if (i > 5)
    day += ((DAY_MAX - i) + 1);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (dvec[10 - 1], 10, year, DAY_MIN, FALSE);
  holiday (*init_data, detected, _(hd_text[HD_OCTOBER_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  day = 17;
  if (weekday_of_date (day, 3, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_ST_PATRICKS_DAY].ht_text),
	   ptr_cc_id, "+", day, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, DAY_MIN, 11, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 39, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, 8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 6, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1903)
    holiday (*init_data, detected, _(hd_text[HD_BLOOMSDAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, 16, 6, year, hd_elems, fday, count);
}



void
in_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in India.
*/
{
  ptr_cc_id = "IN";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_MAHATMA_GANDHIS_BIRTHDAY].ht_text), ptr_cc_id, "+", 2,
	   10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 26, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1956)
    {
      register int day = 22;


      /*
         Calculate the year number used in the Indian **CIVIL** (reformed)
         calendar corresponding to Caitra 1.
       */
      sprintf (s2, "%s %d", _(hd_text[HD_INDIAN_NEW_YEARS_DAY].ht_text),
	       year - 78);
      day -= (days_of_february (year) == 29);
      holiday (*init_data, detected, s2, ptr_cc_id,
	       DIS_HLS_PREF2, day, 3, year, hd_elems, fday, count);
    }
}



void
iq_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Iraq.
*/
{
  ptr_cc_id = "IQ";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_BAATH_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 17, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FAO_DAY].ht_text),
	   ptr_cc_id, "+", 17, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 14, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PEACE_FESTIVAL].ht_text),
	   ptr_cc_id, "+", 8, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 8, 2, year, hd_elems, fday, count);
}



void
ir_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Iran.
*/
{
  ptr_cc_id = "IR";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_OIL_INDUSTRY_DAY].ht_text),
	   ptr_cc_id, "+", 20, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 2, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 11, 2, year, hd_elems, fday, count);
}



void
is_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Iceland.
*/
{
  register int day;


  ptr_cc_id = "IS";
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  day = eval_holiday (19, 4, year, 4, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BEGINNING_OF_SUMMER].ht_text),
	   ptr_cc_id, "+", day, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "#", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "#", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "#", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAUNDY_THURSDAY].ht_text),
	   ptr_cc_id, "+", easter - 3, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_BUN_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 18, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 17, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "#", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
jm_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Jamaica.
*/
{
  register int day;


  ptr_cc_id = "JM";
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 46, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  day = day_of_year (day, 8, year);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day - 1, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE) + (3 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
jo_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Jordan.
*/
{
  ptr_cc_id = "JO";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  /*
     The Christians in Jordan compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches new calendar.
   */
  easter = orthodox_easter (year, NULL, 1924, 3, 10, 22);
  holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 15, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 10, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 25, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CORONATION_DAY].ht_text),
	   ptr_cc_id, "+", 11, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PALM_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - DAY_MAX, 0, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 14, 11, year, hd_elems, fday, count);
}



void
ke_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Kenya.
*/
{
  ptr_cc_id = "KE";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_MADARAKA_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NOI_DAY].ht_text),
	   ptr_cc_id, "+", 10, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 20, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CIVIC_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 12, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
}



void
kg_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Kyrgyzstan.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "KG";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  persian_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if ((year > 1916) && (year < 1991))
    {
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
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
	       "+", 5, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", dvec[8 - 1], 8, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	       ptr_cc_id, "+", 2, MONTH_MAX, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	       ptr_cc_id, "+", 13, 6, year, hd_elems, fday, count);
    }
  /*
     The Christians in Kyrgyzstan compute all holiday dates which are
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
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
kh_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Cambodia.
*/
{
  register int day;
  register int i;


  ptr_cc_id = "KH";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_PARIS_PEACE_AGREEMENT_DAY].ht_text), ptr_cc_id, "+",
	   23, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   24, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HUMAN_RIGHTS_DAY].ht_text),
	   ptr_cc_id, "+", 10, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 9, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  day = day_of_year (30, 10, year);
  for (i = 0; i < 3; i++)
    holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	     ptr_cc_id, "+", day + i, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
}



void
ki_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Kiribati.
*/
{
  register int day;


  ptr_cc_id = "KI";
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_HEALTH_AND_SPORTS_DAY].ht_text),
	   ptr_cc_id, "+", 7, 8, year, hd_elems, fday, count);
  for (day = 12; day <= 14; day++)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", day, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   "+", 7, 8, year, hd_elems, fday, count);
}



void
km_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Comoros.
*/
{
  ptr_cc_id = "KM";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected,
	   _(hd_text[HD_DEATH_OF_PRESIDENT_CHEIKH].ht_text), ptr_cc_id, "+",
	   18, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_DEATH_OF_PRESIDENT_SOILIH].ht_text), ptr_cc_id, "+",
	   29, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_DEATH_OF_PRESIDENT_ABDALLAH].ht_text), ptr_cc_id, "+",
	   26, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 6, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text), ptr_cc_id,
	   "+", 25, 5, year, hd_elems, fday, count);
}



void
kn_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in St Kitts and Nevis.
*/
{
  register int day;


  ptr_cc_id = "KN";
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BANK_HOLIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, day + 1, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_PRINCE_OF_WALES_BIRTHDAY].ht_text), ptr_cc_id, "+",
	   14, 11, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  if (year > 1982)
    holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	     ptr_cc_id, "+", 19, 9, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
kp_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in North Korea.
*/
{
  ptr_cc_id = "KP";
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
      holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, DAY_MIN, 6, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, 5, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	       ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday,
	       count);
      holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text),
	       ptr_cc_id, DIS_HLS_PREF, 8, 3, year, hd_elems, fday, count);
      if (year > 1918)
	holiday (*init_data, detected,
		 _(hd_text[HD_INDEPENDENCE_MOVEMENT_DAY].ht_text), ptr_cc_id,
		 DIS_HLS_PREF, DAY_MIN, 3, year, hd_elems, fday, count);
      if (year > 1944)
	holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
		 ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
      if (year > 1947)
	{
	  holiday (*init_data, detected,
		   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
		   "+", 27, MONTH_MAX, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_ANNIVERSARY_OF_ARMED_STRUGGLE].ht_text),
		   ptr_cc_id, DIS_HLS_PREF, 25, 6, year, hd_elems, fday,
		   count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_KIM_JONG_ILS_BIRTHDAY].ht_text), ptr_cc_id,
		   "+", 16, 2, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_KIM_IL_SUNGS_BIRTHDAY].ht_text), ptr_cc_id,
		   "+", 15, 4, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_ARMED_FORCES_DAY].ht_text), ptr_cc_id, "+",
		   25, 4, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_FOUNDING_OF_THE_COMMUNIST_PARTY].ht_text),
		   ptr_cc_id, "+", 10, 10, year, hd_elems, fday, count);
	  holiday (*init_data, detected,
		   _(hd_text[HD_NATIONAL_FOUNDATION_DAY].ht_text), ptr_cc_id,
		   "+", 9, 9, year, hd_elems, fday, count);
	}
      if (year > 1952)
	holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
		 ptr_cc_id, "+", 27, 7, year, hd_elems, fday, count);
    }
}


void
kw_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Kuwait.
*/
{
  ptr_cc_id = "KW";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 25, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  if (year > 1989)
    holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	     ptr_cc_id, "+", 26, 2, year, hd_elems, fday, count);
}



void
ky_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Caiman Islands.
*/
{
  register int day;


  ptr_cc_id = "KY";
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, "+", easter - 46, 0, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  day = eval_holiday (DAY_MIN, 7, year, DAY_MIN, TRUE);
  holiday (*init_data, detected,
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id, "+",
	   day, 7, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE) + (2 * DAY_MAX);
  holiday (*init_data, detected, _(hd_text[HD_DISCOVERY_DAY].ht_text),
	   ptr_cc_id, "+", day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX + 2;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  day = 11;
  if (weekday_of_date (day, 11, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, "+", day, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
kz_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Kazakhstan.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "KZ";
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
      holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text),
	       ptr_cc_id, "+", 25, 10, year, hd_elems, fday, count);
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", 15, MONTH_MAX, year, hd_elems, fday, count);
    }
  /*
     The Christians in Kazakhstan compute all holiday dates which are
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
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
}



void
la_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Laos.
*/
{
  ptr_cc_id = "LA";
  use_other_cc = !use_other_cc;
  chinese_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id,
	       FALSE);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_DAY_OF_THE_FREE_LAOS].ht_text),
	   ptr_cc_id, "+", 13, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FOUNDING_OF_PEOPLES_PARTY].ht_text), ptr_cc_id, "+",
	   22, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 20, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHILDRENS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", 12, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_WOMENS_DAY].ht_text), ptr_cc_id,
	   "+", 8, 3, year, hd_elems, fday, count);
}



void
lb_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Lebanon.
*/
{
  auto int d;
  auto int greg_diff;


  ptr_cc_id = "LB";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  if (year > 674)
    holiday (*init_data, detected, _(hd_text[HD_ST_MAROONS_DAY].ht_text),
	     ptr_cc_id, "+", 9, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SOLIDARITY_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems,
	   fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 22, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARTYRS_DAY].ht_text),
	   ptr_cc_id, "+", 6, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 16, 4, year, hd_elems, fday, count);
  /*
     The Christian Western churches holidays.
   */
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  /*
     The Christians in Lebanon compute all holiday dates which are
     depending on Easter Sunday's date in the way done by the
     Orthodox Christian Eastern churches old calendar, AND also
     in the way done by Christian Western churches.
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
	       ptr_cc_id, "#", d, m, year, hd_elems, fday, count);
    }
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter + 1, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
}



void
lc_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in St Lucia.
*/
{
  register int day;


  ptr_cc_id = "LC";
  holiday (*init_data, detected, _(hd_text[HD_ST_MARGUERITES_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 17, 10, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ST_CEDILIAS_DAY].ht_text),
	   ptr_cc_id, "+", 22, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, "+", 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ASH_WEDNESDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 46, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text), ptr_cc_id,
	   "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  for (day = 2; day; day--)
    holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	     ptr_cc_id, "+", easter - 46 - day, 0, year, hd_elems, fday,
	     count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = 13;
  if (weekday_of_date (day, MONTH_MAX, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 6, year, 6, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_THE_QUEENS_BIRTHDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 11, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_PETER_AND_ST_PAUL].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 29, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_ROSE_OF_LIMA].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 30, 8, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 8, year, 5, TRUE);
  holiday (*init_data, detected, _(hd_text[HD_SLAVE_LIBERATION_DAY].ht_text),
	   ptr_cc_id, "+", day, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  day = 5;
  if (weekday_of_date (day, 10, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_THANKSGIVING_DAY].ht_text),
	   ptr_cc_id, "+", day, 10, year, hd_elems, fday, count);
  if (year > 1978)
    {
      day = 22;
      if (weekday_of_date (day, 2, year) == DAY_MAX)
	day++;
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", day, 2, year, hd_elems, fday, count);
    }
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
li_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Liechtenstein.
*/
{
  ptr_cc_id = "LI";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, "+", 24, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_FEAST_OF_CORPUS_CHRISTI].ht_text), ptr_cc_id, "+",
	   easter + 60, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_CANDLEMAS].ht_text),
	   ptr_cc_id, "+", 2, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_MARYS_IMMACULATE_CONCEPTION].ht_text), ptr_cc_id, "+",
	   8, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_NATIVITY].ht_text),
	   ptr_cc_id, "+", 8, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SHROVE_TUESDAY].ht_text),
	   ptr_cc_id, "+", easter - 47, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_BERCHTOLDS_DAY].ht_text),
	   ptr_cc_id, "+", 2, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_JOSEPHS_DAY].ht_text),
	   ptr_cc_id, "+", 19, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_STEPHENS_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_THREE_KINGS_DAY].ht_text),
	   ptr_cc_id, "+", 6, MONTH_MIN, year, hd_elems, fday, count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
lk_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Sri Lanka.
*/
{
  auto double mjd;
  auto Ulint lunation = 0L;
  register int doy;
  register int doy2;
  register int i;
  auto int the_hour;
  auto int the_min;
  auto int d = DAY_MIN;
  auto int m = MONTH_MIN;
  auto int y = year;
  auto Bool set_hdy;
  auto Bool hd1_set = FALSE;
  auto Bool hd2_set = FALSE;
  auto Bool hd3_set = FALSE;


  ptr_cc_id = "LK";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, easter - 2, 0, year, hd_elems, fday,
	   count);
  /*
     Detect the Full Moon phases for Sri Lanka, also called ``Poya Days'',
     which are celebrated as holidays.  The calculation of them is
     based on Sri Lanka local time, not on UTC/GMT.  Colombo local time
     (not respecting Daylight Savings) is UTC/GMT-5.5.
   */
  while (y == year)
    {
      mjd = moonphase (MPHASE_FUL, FALSE, NULL, &lunation, &d, &m, &y, 5, 30);
      if (y == year)
	{
	  val2hours (mjd, &the_hour, &the_min, NULL);
	  sprintf (s2, "%s %02d%s%02d", _(hd_text[HD_POYA_DAY].ht_text),
		   the_hour, time_sep, the_min);
	  holiday (*init_data, detected, s2,
		   ptr_cc_id, "+", d, m, year, hd_elems, fday, count);
	  /*
	     The following public holidays are observed at the previous working
	     day if they fall on a Saturday.  If they fall on a Sunday or
	     ``Poya Day'', they are observed at the next working day.
	   */
	  doy2 = day_of_year (d, m, year);
	  if (!hd1_set)
	    {
	      set_hdy = FALSE;
	      doy = day_of_year (4, 2, year);
	      i = weekday_of_date (4, 2, year);
	      if (i == 6)
		{
		  if (doy - 1 == doy2)
		    {
		      doy -= 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (i == DAY_MAX)
		{
		  if (doy + 1 == doy2)
		    {
		      doy += 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (doy == doy2)
		{
		  if (i < 5)
		    doy++;
		  else
		    doy += ((DAY_MAX - i) + 1);
		  set_hdy = TRUE;
		}
	      if (set_hdy)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_INDEPENDENCE_DAY].ht_text), ptr_cc_id,
			   "+", doy, 0, year, hd_elems, fday, count);
		  hd1_set = TRUE;
		}
	    }
	  if (!hd2_set)
	    {
	      set_hdy = FALSE;
	      doy = day_of_year (DAY_MIN, 5, year);
	      i = weekday_of_date (DAY_MIN, 5, year);
	      if (i == 6)
		{
		  if (doy - 1 == doy2)
		    {
		      doy -= 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (i == DAY_MAX)
		{
		  if (doy + 1 == doy2)
		    {
		      doy += 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (doy == doy2)
		{
		  if (i < 5)
		    doy++;
		  else
		    doy += ((DAY_MAX - i) + 1);
		  set_hdy = TRUE;
		}
	      if (set_hdy)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id, "+",
			   doy, 0, year, hd_elems, fday, count);
		  hd2_set = TRUE;
		}
	    }
	  if (!hd3_set)
	    {
	      set_hdy = FALSE;
	      doy = day_of_year (25, MONTH_MAX, year);
	      i = weekday_of_date (25, MONTH_MAX, year);
	      if (i == 6)
		{
		  if (doy - 1 == doy2)
		    {
		      doy -= 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (i == DAY_MAX)
		{
		  if (doy + 1 == doy2)
		    {
		      doy += 2;
		      set_hdy = TRUE;
		    }
		}
	      else if (doy == doy2)
		{
		  if (i < 5)
		    doy++;
		  else
		    doy += ((DAY_MAX - i) + 1);
		  set_hdy = TRUE;
		}
	      if (set_hdy)
		{
		  holiday (*init_data, detected,
			   _(hd_text[HD_CHRISTMAS_DAY].ht_text), ptr_cc_id,
			   "+", doy, 0, year, hd_elems, fday, count);
		  hd3_set = TRUE;
		}
	    }
	  lunation++;
	}
    }
  if (!hd1_set)
    {
      doy = day_of_year (4, 2, year);
      i = weekday_of_date (4, 2, year);
      if (i == 6)
	doy--;
      else if (i == DAY_MAX)
	doy++;
      holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	       ptr_cc_id, "+", doy, 0, year, hd_elems, fday, count);
    }
  if (!hd2_set)
    {
      doy = day_of_year (DAY_MIN, 5, year);
      i = weekday_of_date (DAY_MIN, 5, year);
      if (i == 6)
	doy--;
      else if (i == DAY_MAX)
	doy++;
      holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text),
	       ptr_cc_id, "+", doy, 0, year, hd_elems, fday, count);
    }
  if (!hd3_set)
    {
      doy = day_of_year (25, MONTH_MAX, year);
      i = weekday_of_date (25, MONTH_MAX, year);
      if (i == 6)
	doy--;
      else if (i == DAY_MAX)
	doy++;
      holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	       ptr_cc_id, "+", doy, 0, year, hd_elems, fday, count);
    }
}



void
lr_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Liberia.
*/
{
  ptr_cc_id = "LR";
  holiday (*init_data, detected,
	   _(hd_text[HD_NATIONAL_REDEMPTION_DAY].ht_text), ptr_cc_id, "+", 12,
	   4, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_J_ROBERTS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 15, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_S_DOES_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 6, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_W_TUBMANS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 29, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ARMED_FORCES_DAY].ht_text),
	   ptr_cc_id, "+", 11, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FLAG_DAY].ht_text), ptr_cc_id,
	   "+", 24, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 26, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HEROES_DAY].ht_text),
	   ptr_cc_id, "+", 8, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_UNITY_DAY].ht_text),
	   ptr_cc_id, "+", 14, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_OAU_DAY].ht_text), ptr_cc_id,
	   "+", 25, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PRAYER_DAY].ht_text), ptr_cc_id,
	   "+", 14, 4, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_YOUTH_DAY].ht_text), ptr_cc_id,
	   "+", 29, 10, year, hd_elems, fday, count);
}



void
ls_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Lesotho.
*/
{
  ptr_cc_id = "LS";
  holiday (*init_data, detected, _(hd_text[HD_MOSHESHOES_DAY].ht_text),
	   ptr_cc_id, "+", 12, 3, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ARBOR_DAY].ht_text),
	   ptr_cc_id, "+", 21, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_BOXING_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FAMILY_DAY].ht_text), ptr_cc_id,
	   "+", 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_GOOD_FRIDAY].ht_text),
	   ptr_cc_id, "+", easter - 2, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 4, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_THE_KINGS_BIRTHDAY].ht_text),
	   ptr_cc_id, "+", 2, 5, year, hd_elems, fday, count);
}



void
lt_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Lithuania.
*/
{
  ptr_cc_id = "LT";
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
	   _(hd_text[HD_CONSTITUTION_ANNIVERSARY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 25, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_DEFENDERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 13, MONTH_MIN, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_MONDAY].ht_text),
	   ptr_cc_id, "+", easter + 1, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EASTER_SUNDAY].ht_text),
	   ptr_cc_id, "+", easter, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 16, 2, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 23, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_MOURNING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RE_DAY].ht_text), ptr_cc_id,
	   "+", 6, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_RESTORATION_DAY].ht_text),
	   ptr_cc_id, "+", 11, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
}



void
lu_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Luxembourg.
*/
{
  register int day;


  ptr_cc_id = "LU";
  holiday (*init_data, detected, _(hd_text[HD_ALL_SAINTS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 11, year, hd_elems, fday, count);
  if (*init_data)
    *init_data = FALSE;
  holiday (*init_data, detected, _(hd_text[HD_ALL_SOULS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF2, 2, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CARNIVAL].ht_text),
	   ptr_cc_id, "+", easter - 48, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_EVE].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 24, MONTH_MAX, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_CHRISTS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", easter + 39, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_FRENCH_CULTURE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 27, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_LUXEMBOURG_CITY_KERMIS].ht_text), ptr_cc_id,
	   DIS_HLS_PREF2, DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MARYS_ASCENSION_DAY].ht_text),
	   ptr_cc_id, "+", 15, 8, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MAY_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  day = 23;
  if (weekday_of_date (day, 6, year) == DAY_MAX)
    day++;
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", day, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_QUINQUAGESIMA_SUNDAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, easter - 49, 0, year, hd_elems, fday,
	   count);
  holiday (*init_data, detected, _(hd_text[HD_ST_STEPHENS_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday,
	   count);
  base_christian_hdy (init_data, detected, easter, year, hd_elems, fday,
		      count, ptr_cc_id);
}



void
lv_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Latvia.
*/
{
  register int day;


  ptr_cc_id = "LV";
  holiday (*init_data, detected, _(hd_text[HD_CHRISTMAS_DAY].ht_text),
	   ptr_cc_id, "+", 25, MONTH_MAX, year, hd_elems, fday, count);
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
  holiday (*init_data, detected, _(hd_text[HD_INDEPENDENCE_DAY].ht_text),
	   ptr_cc_id, "+", 18, 11, year, hd_elems, fday, count);
  holiday (*init_data, detected,
	   _(hd_text[HD_INDEPENDENCE_PROCLAMATION].ht_text), ptr_cc_id,
	   DIS_HLS_PREF, 4, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_LABOUR_DAY].ht_text), ptr_cc_id,
	   "+", DAY_MIN, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMER_DAY].ht_text),
	   ptr_cc_id, "+", 24, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_MIDSUMMERS_EVE].ht_text),
	   ptr_cc_id, "+", 23, 6, year, hd_elems, fday, count);
  day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE) + DAY_MAX;
  holiday (*init_data, detected, _(hd_text[HD_MOTHERS_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, day, 5, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_MOURNING_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 14, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NEW_YEARS_DAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_PENTECOST].ht_text),
	   ptr_cc_id, "+", easter + 49, 0, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 25, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REMEMBRANCE_DAY].ht_text),
	   ptr_cc_id, DIS_HLS_PREF, 4, 7, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_ST_STEPHENS_DAY].ht_text),
	   ptr_cc_id, "+", 26, MONTH_MAX, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_SYLVESTER].ht_text), ptr_cc_id,
	   "+", dvec[MONTH_MAX - 1], MONTH_MAX, year, hd_elems, fday, count);
  if (year > 1944)
    holiday (*init_data, detected, _(hd_text[HD_VICTORY_DAY].ht_text),
	     ptr_cc_id, DIS_HLS_PREF, 9, 5, year, hd_elems, fday, count);
}



void
ly_hdy (init_data, detected, easter, year, hd_elems, fday, count)
     Bool *init_data;
     const Bool detected;
     int easter;
     const int year;
     int *hd_elems;
     const int fday;
     const int count;
/*!
   Manages all specific holidays celebrated in Libyan Arab Jamahiriya.
*/
{
  ptr_cc_id = "LY";
  use_other_cc = !use_other_cc;
  islamic_hdy (init_data, detected, year, hd_elems, fday, count, ptr_cc_id);
  use_other_cc = !use_other_cc;
  holiday (*init_data, detected, _(hd_text[HD_AUTHORITIYS_POWER_DAY].ht_text),
	   ptr_cc_id, "+", 3, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", 28, 3, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", 11, 6, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_EVACUATION_DAY].ht_text),
	   ptr_cc_id, "+", 7, 10, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_NATIONAL_HOLIDAY].ht_text),
	   ptr_cc_id, "+", DAY_MIN, 9, year, hd_elems, fday, count);
  holiday (*init_data, detected, _(hd_text[HD_REVOLUTION_DAY].ht_text),
	   ptr_cc_id, "+", 23, 7, year, hd_elems, fday, count);
}
#endif /* !HD_TOP20CC */
