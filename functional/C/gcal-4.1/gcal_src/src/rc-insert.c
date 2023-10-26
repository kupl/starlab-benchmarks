/*!
*  \file rc-insert.c
*  \brief Inserts a line (of a resource file) into `rc_elems_table[]'.
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
# if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
# else /* !TIME_WITH_SYS_TIME */
#  if HAVE_SYS_TIME_H
#   include <sys/time.h>
#  else	/* !HAVE_SYS_TIME_H */
#   include <time.h>
#  endif /* !HAVE_SYS_TIME_H */
# endif	/* !TIME_WITH_SYS_TIME */
# include "common.h"
# include "rc-defs.h"
# include "globals.h"
# include "hd-astro.h"
# include "hd-use.h"
# include "rc-astro.h"
# include "rc-utils.h"
# include "tty.h"
# include "utils.h"
# include "rc-insert.h"



/*
*  static variables definitions.
*/
/*!
   The table containing the codes that define the output and error format
   types used for formatting the data of the %sun* and %moon* special texts.
*/
static const Aobj_oformat_struct aobj_oformat[][AOBJ_MOON + 1] = {
/*
  { SUN::{char data_format, char error_format}, MOON::{char data_format, char error_format} }
*/
  /* Mode 0...4. */
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  /* Mode 5...9. */
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_HHH, ERR_HHH}, {FMT_DDD, ERR_DDD}},
  {{FMT_HHH, ERR_HHH}, {FMT_DDD, ERR_DDD}},
  {{FMT_HHH, ERR_HHH}, {FMT_NSD, ERR_NNN}},
  {{FMT_HHH, ERR_HHH}, {FMT_NND, ERR_NNN}},
  /* Mode 10...14. */
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_TTT, ERR_TTT}, {FMT_DDS, ERR_DDD}},
  /* Mode 15...19. */
  {{FMT_NND, ERR_NNN}, {FMT_TTT, ERR_TTT}},
  {{FMT_DDD, ERR_DDD}, {FMT_NND, ERR_NNN}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  /* Mode 20...24. */
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_NSD, ERR_NNN}},
  {{FMT_DDD, ERR_DDD}, {FMT_NND, ERR_NNN}},
  {{FMT_TTT, ERR_TTT}, {FMT_DDS, ERR_DDD}},
  {{FMT_NND, ERR_NNN}, {FMT_DDD, ERR_DDD}},
  /* Mode 25...29. */
  {{FMT_DDD, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_NSD, ERR_NNN}, {FMT_DDS, ERR_DDD}},
  {{FMT_HHH, ERR_HHH}, {FMT_TTT, ERR_TTT}},
  {{FMT_TXS, ERR_TTT}, {FMT_NND, ERR_NNN}},
  /* Mode 30...34. */
  {{FMT_NND, ERR_NNN}, {FMT_DDD, ERR_DDD}},
  {{FMT_NND, ERR_NNN}, {FMT_NSD, ERR_NNN}},
  {{FMT_TTS, ERR_TTT}, {FMT_HHH, ERR_HHH}},
  {{FMT_DDS, ERR_DDD}, {FMT_TXS, ERR_TTT}},
  {{FMT_DDS, ERR_DDD}, {FMT_NND, ERR_NNN}},
  /* Mode 35...39. */
  {{FMT_DDS, ERR_DDD}, {FMT_NND, ERR_NNN}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  /* Mode 40...44. */
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_TTS, ERR_TTT}, {FMT_DDS, ERR_DDD}},
  {{FMT_TTS, ERR_TTT}, {FMT_DDS, ERR_DDD}},
  {{FMT_TTS, ERR_TTT}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_TTS, ERR_TTT}},
  /* Mode 45...49. */
  {{FMT_DDS, ERR_DDD}, {FMT_TTS, ERR_TTT}},
  {{FMT_DDS, ERR_DDD}, {FMT_TTS, ERR_TTT}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_NND, ERR_NNN}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  /* Mode 50...54. */
  {{FMT_DDS, ERR_DDD}, {FMT_NND, ERR_NNN}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDS, ERR_DDD}},
  {{FMT_HHH, ERR_HHH}, {FMT_DDD, ERR_DDD}},
  {{FMT_HHH, ERR_HHH}, {FMT_NND, ERR_NNN}},
  {{FMT_NIL, ERR_NIL}, {FMT_DDS, ERR_DDD}},
  /* Mode 55...60. */
  {{FMT_NIL, ERR_NIL}, {FMT_NND, ERR_NNN}},
  {{FMT_NIL, ERR_NIL}, {FMT_DDS, ERR_DDD}},
  {{FMT_NIL, ERR_NIL}, {FMT_NND, ERR_NNN}},
  {{FMT_NIL, ERR_NIL}, {FMT_DDS, ERR_DDD}},
  {{FMT_NIL, ERR_NIL}, {FMT_DDD, ERR_DDD}},
  {{FMT_NIL, ERR_NIL}, {FMT_NND, ERR_NNN}},
  /* Auxiliary modes 61 ..68. */
  {{FMT_NNN, ERR_NNN}, {FMT_NNN, ERR_NNN}},
  {{FMT_NND, ERR_NNN}, {FMT_NND, ERR_NNN}},
  {{FMT_NSD, ERR_NNN}, {FMT_NSD, ERR_NNN}},
  {{FMT_HHH, ERR_HHH}, {FMT_HHH, ERR_HHH}},
  {{FMT_TTT, ERR_TTT}, {FMT_TTT, ERR_TTT}},
  {{FMT_TTS, ERR_TTT}, {FMT_TTS, ERR_TTT}},
  {{FMT_DDD, ERR_DDD}, {FMT_DDD, ERR_DDD}},
  {{FMT_DDS, ERR_DDD}, {FMT_DDS, ERR_DDD}},
};



/*
*  Function implementations.
*/
void
insert_line_into_table (line_buffer, filename, line_number, rc_elems,
			len_date, print_twice)
     char *line_buffer;
     const char *filename;
     const long line_number;
     int *rc_elems;
     int len_date;
     int print_twice;
/*!
   Inserts a line into `rc_elems_table[]',
     but before this the line is checked and evaluated first
     for TVAR text variables and then for %?... special texts.
*/
{
  auto Slint num;
  register int i;
  register int j;
  register int j_buf;
  register int j_diff;
  register int k;
  register int kk;
  register int kpos;
  register int kkpos;
  register int len_fn = len_year_max + 5;	/* Position of the `(' of the "(FILENAME)" text */
  register int tmp_year = year;
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
  auto int bio_emo_phase;
  auto int bio_emo_waxes;
  auto int bio_int_phase;
  auto int bio_int_waxes;
  auto int bio_phy_phase;
  auto int bio_phy_waxes;
  auto int bio_critical_day;
  auto int bio_positive_day;
  auto int bio_negative_day;
  auto int fstyle;
  auto int fwidth;
  static char buffer[40];
  static char date_text[23];
  static char bio_emo_ptext[7];
  static char bio_int_ptext[7];
  static char bio_phy_ptext[7];
  auto char *ptr_date_text;
  auto char *ptr_char = line_buffer + len_date;
  auto char hhc;
  auto char rhc;
  static Bool inclusive_date_map[DAY_LAST + 2];
  static Bool exclusive_date_map[DAY_LAST + 2];
  auto Bool ie_date_maps_set;
  auto Bool hls1_set;
  auto Bool hls2_set;
  auto Bool is_weekday_mode;
  auto Bool ris_weekday_mode;
  auto Bool moon_min_max;
  auto Bool ok;
  auto Bool hls_set;
  auto Bool print_line;
  auto Bool is_obsolete_whitespace;
  auto Bool is_cformat;
  auto Bool is_lformat;
  auto Bool is_sign;
  auto Bool is_lzero;
  auto Bool is_fformat;
  auto Bool is_suffix;
  auto Bool got_command = FALSE;


  if (rc_enable_fn_flag)
    len_fn += (int) strlen (filename) + 3;	/* +3 due to the "(THE_REAL_FILENAME_ITSELF) " text */
  /*
     Skip the delimiting whitespace characters between the "date"-part and the "text"-part.
   */
  while (isspace (*ptr_char))
    ptr_char++;
  if (*ptr_char)
    {
      /*
         If the "text"-part starts with a QUOTEd whitespace character, exchange
         the QUOTE character by a PSEUDO_QUOTE character ('\001') to keep the
         internal sorting order intact.
       */
      if ((*ptr_char == QUOTE_CHAR) && isspace (*(ptr_char + 1)))
	*ptr_char = PSEUDO_QUOTE;
      if ((Uint) len_the_text < maxlen_max)
	{
	  len_the_text = (int) maxlen_max;
	  the_text = (char *) my_realloc ((VOID_PTR) the_text,
					  len_the_text,
					  ERR_NO_MEMORY_AVAILABLE,
					  __FILE__, ((long) __LINE__) - 3L,
					  "the_text", len_the_text);
	}
      strcpy (the_text, ptr_char);
    }
  else
    /*
       No "text"-part given.
     */
    *the_text = '\0';
  do
    {
      /*
         Initialize some controlling variables.
       */
      print_line = TRUE;
      ptr_date_text = date_text;
      j = k = j_buf = kk = j_diff = kpos = kkpos = 0;
      ie_date_maps_set = is_obsolete_whitespace = hls_set = hls1_set =
	hls2_set = FALSE;
      fstyle = FSTYLE_NONE;
      fwidth = SPECIAL_VALUE;
      ok = is_cformat = is_lformat = is_sign = is_lzero = is_fformat =
	is_suffix = FALSE;
      /*
         If `--filter-period=ARG' is given and ARG matches the
         date of the fixed date, suppress this fixed date!
       */
      if ((rc_filter_period != (char *) NULL)
	  && (line_number != SPECIAL_VALUE))
	print_line =
	  rc_valid_period (rc_filter_period, d, m, y, incr_year, decr_year);
      /*
         If `--filter-day=ARG' is given and ARG matches the
         date of the fixed date, suppress this fixed date!
       */
      if (print_line
	  && (rc_filter_day != (char *) NULL)
	  && (line_number != SPECIAL_VALUE))
	print_line =
	  rc_valid_day (rc_filter_day, d, m, year + incr_year - decr_year);
      if (!print_line)
	ok = TRUE;
      else
	/*
	   Check line for %?... special texts and quoted TVAR text variables.
	 */
	*s6 = *date_text = '\0';
      while (!ok)
	{
	  while (*(the_text + j))
	    {
	      if ((Uint) k >= maxlen_max - (Uint) len_fn)
		resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				    (long) __LINE__);
	      if (*(the_text + j) == RC_SPECIAL_TEXT_CHAR
		  || *(the_text + j) == RC_TVAR_CHAR)
		{
		  if (j)
		    {
		      /*
		         Check if the %?... special text or the TVAR text variable is quoted.
		       */
		      if (*(the_text + (j - 1)) == QUOTE_CHAR)
			{
			  k--;
			  kk--;
			}
		      else if (*(the_text + j) == RC_SPECIAL_TEXT_CHAR)
			break;
		    }
		  else if (*(the_text + j) == RC_SPECIAL_TEXT_CHAR)
		    break;
		}
	      if (is_obsolete_whitespace && isspace (*(the_text + j)))
		{
		  is_obsolete_whitespace = FALSE;
		  j++;
		}
	      else
		s1[k++] = s6[kk++] = *(the_text + j++);
	    }
	  if ((Uint) k >= maxlen_max - (Uint) len_fn)
	    resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				(long) __LINE__);
	  if (*(the_text + j))
	    {
	      auto char special_text_char;


	      /*
	         A '%' character (special text prefix) found!
	       */
	      j_buf = j++;
	      /*
	         Check for a special text format.
	       */
	      j = decode_format (the_text, j, &is_cformat,
				 &is_lformat, &is_sign, &is_lzero,
				 &is_suffix, &is_fformat, &fstyle, &fwidth);
	      *s2 = '\0';
	      special_text_char = *(the_text + j);
	      if ((!got_command
		   && (special_text_char == RC_IDATE_CHAR
		       || special_text_char == RC_EDATE_CHAR))
		  || special_text_char == RC_TDATE_CHAR
		  || special_text_char == RC_WDNAME_CHAR
		  || special_text_char == RC_WDNR_M1_2_S7_CHAR
		  || special_text_char == RC_WDNR_M0_2_S6_CHAR
		  || special_text_char == RC_WDNR_S1_2_S7_CHAR
		  || special_text_char == RC_WDNR_S0_2_S6_CHAR
		  || special_text_char == RC_WDNR_X1_2_X7_CHAR
		  || special_text_char == RC_WDNR_X0_2_X6_CHAR
		  || special_text_char == RC_DOYNR_CHAR
		  || special_text_char == RC_DAYNR_CHAR
		  || special_text_char == RC_MONTHNAME_CHAR
		  || special_text_char == RC_MONTHNR_CHAR
		  || special_text_char == RC_YEARNR_CHAR
		  || special_text_char == RC_WEEKNR_CHAR
		  || special_text_char == RC_BYEAR_CHAR
		  || ((special_text_char == RC_YEAR_DIFF_CHAR)
		      && *(the_text + j + 1)
		      && !isspace (*(the_text + j + 1)))
		  || special_text_char == RC_MOON_CHAR
		  || special_text_char == RC_MOON_IMAGE_CHAR
		  || special_text_char == RC_BIO_CHAR
		  || special_text_char == RC_BIO_BAR_CHAR)
		{
		  j++;
		  if (special_text_char == RC_TDATE_CHAR)
		    {
		      auto Slint jdate;


		      /*
		         %textual_date[[+|-]N] special text found.
		       */
		      i = 0;
		      if (*(the_text + j) == *ASC_LIT
			  || *(the_text + j) == *DES_LIT)
			s2[i++] = *(the_text + j++);
		      while (isdigit (*(the_text + j)))
			{
			  if ((Uint) i >= maxlen_max)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  s2[i++] = *(the_text + j++);
			}
		      s2[i] = '\0';
		      num = atol (s2);
		      jdate =
			(Slint) date2num (d, m, year + incr_year - decr_year);
		      if ((jdate + num <=
			   (Slint) date2num (dvec[MONTH_MAX - 1], MONTH_MAX,
					     YEAR_MAX))
			  && (jdate + num >= 1L))
			{
			  num2date (jdate + num, &dd, &mm, &yy);
			  if (transform_year)
			    {
			      yy = yy - transform_year;
			      if ((yy >= 0) && (transform_year > 0))
				yy++;
			    }
			  sprintf (s2, "%02d-%s-%0*d", dd,
				   short_month_name (mm), len_year_max, yy);
			}
		      else
			*s2 = '\0';
		      /*
		         And copy the expanded special text into the target string.
		       */
		      i = 0;
		      while (s2[i])
			{
			  if ((Uint) k >= maxlen_max - (Uint) len_fn)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  s1[k++] = s6[kk++] = s2[i++];
			}
		    }
		  else
		    {
		      auto Bool is_ie = FALSE;
		      auto Bool is_range = FALSE;
		      auto Bool dflt_yy_set = FALSE;
		      auto Bool dflt_ryy_set = FALSE;


		      rhc = '\0';
		      rlen = rdd = rmm = ryy = rnn = 0;
		      /*
		         Check if a range of dates is given.
		       */
		      if (special_text_char == RC_IDATE_CHAR
			  || special_text_char == RC_EDATE_CHAR)
			{
			  is_ie = TRUE;
			  if (!ie_date_maps_set)
			    {
			      for (i = 1; i < DAY_LAST + 2; i++)
				inclusive_date_map[i] =
				  !(exclusive_date_map[i] = TRUE);
			      ie_date_maps_set = !(*inclusive_date_map =
						   *exclusive_date_map =
						   FALSE);
			    }
			  ptr_char = the_text + j;
			  while (*ptr_char
				 && !isspace (*ptr_char)
				 && (*ptr_char != RC_DRANGE_CHAR))
			    ptr_char++;
			  if (*ptr_char == RC_DRANGE_CHAR)
			    is_range = TRUE;
			}
		      if (is_range)
			*ptr_char = '\0';
		      (void) rc_get_date (the_text + j, lptrs2, FALSE,
					  &is_weekday_mode, &dd, &mm, &yy,
					  &nn, &len, &hhc, &hhn, &hhwd,
					  filename, line_number,
					  the_text + j - 2, TRUE);
		      j += len;
		      if (is_range)
			{
			  j++;
			  (void) rc_get_date (the_text + j, lptrs2, FALSE,
					      &ris_weekday_mode, &rdd, &rmm,
					      &ryy, &rnn, &rlen, &rhc, &rhn,
					      &rhwd, filename, line_number,
					      the_text + j, TRUE);
			  j += rlen;
			}
		      if (is_ie)
			{
			  if (!len)
			    dflt_yy_set = TRUE;
			  if (!rlen)
			    dflt_ryy_set = TRUE;
			}
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
			      if (!precomp_date
				  (hhn, hhwd, &dd, &mm, yy + incr_year,
				   (hhc == RC_EASTER_CHAR) ? EAster : TOday))
				yy = SPECIAL_VALUE;
			    }
			  else
			    if (!precomp_date (hhn, hhwd, &dd, &mm, yy,
					       (hhc ==
						RC_EASTER_CHAR) ? EAster :
					       TOday))
			    yy = SPECIAL_VALUE;
			  break;
			case 'D':
			case 'W':
			  if (!dflt_yy_set && (fiscal_month > MONTH_MIN))
			    {
			      auto int fiscal_year = yy + incr_year;


			      if (!precomp_nth_wd
				  (hhn, hhwd, &hhn, &dd, &mm, &fiscal_year,
				   (hhc == 'D') ? DAy : WEek))
				yy = fiscal_year;
			    }
			  else
			    (void) precomp_nth_wd (hhn, hhwd, &hhn, &dd, &mm,
						   &yy,
						   (hhc == 'D') ? DAy : WEek);
			  break;
			default:
			  if (islower (hhc))
			    {
			      if (rc_dvar[IDX (hhc)].dvar_local.dvar_month)
				{
				  mm =
				    (int) rc_dvar[IDX (hhc)].dvar_local.
				    dvar_month;
				  dd =
				    (int) rc_dvar[IDX (hhc)].dvar_local.
				    dvar_day;
				}
			      else
				if (rc_dvar[IDX (hhc)].dvar_global.dvar_month)
				{
				  mm =
				    (int) rc_dvar[IDX (hhc)].dvar_global.
				    dvar_month;
				  dd =
				    (int) rc_dvar[IDX (hhc)].dvar_global.
				    dvar_day;
				}
			      if (!dflt_yy_set && (fiscal_month > MONTH_MIN))
				{
				  if (!precomp_date
				      (hhn, hhwd, &dd, &mm, yy + incr_year,
				       DVar))
				    yy = SPECIAL_VALUE;
				}
			      else
				if (!precomp_date
				    (hhn, hhwd, &dd, &mm, yy, DVar))
				yy = SPECIAL_VALUE;
			    }
			}
		      switch (rhc)
			{
			case RC_EASTER_CHAR:
			case RC_TODAY_CHAR:
			  if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
			    {
			      if (!precomp_date
				  (rhn, rhwd, &rdd, &rmm, ryy + incr_year,
				   (rhc == RC_EASTER_CHAR) ? EAster : TOday))
				ryy = SPECIAL_VALUE;
			    }
			  else
			    if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy,
					       (rhc ==
						RC_EASTER_CHAR) ? EAster :
					       TOday))
			    ryy = SPECIAL_VALUE;
			  break;
			case 'D':
			case 'W':
			  if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
			    {
			      auto int fiscal_year = ryy + incr_year;


			      if (!precomp_nth_wd
				  (rhn, rhwd, &rhn, &rdd, &rmm, &fiscal_year,
				   (rhc == 'D') ? DAy : WEek))
				ryy = fiscal_year;
			    }
			  else
			    (void) precomp_nth_wd (rhn, rhwd, &rhn, &rdd,
						   &rmm, &ryy,
						   (rhc == 'D') ? DAy : WEek);
			  break;
			default:
			  if (islower (rhc))
			    {
			      if (rc_dvar[IDX (rhc)].dvar_local.dvar_month)
				{
				  rmm =
				    (int) rc_dvar[IDX (rhc)].dvar_local.
				    dvar_month;
				  rdd =
				    (int) rc_dvar[IDX (rhc)].dvar_local.
				    dvar_day;
				}
			      else
				if (rc_dvar[IDX (rhc)].dvar_global.dvar_month)
				{
				  rmm =
				    (int) rc_dvar[IDX (rhc)].dvar_global.
				    dvar_month;
				  rdd =
				    (int) rc_dvar[IDX (rhc)].dvar_global.
				    dvar_day;
				}
			      if (!dflt_ryy_set && (fiscal_month > MONTH_MIN))
				{
				  if (!precomp_date
				      (rhn, rhwd, &rdd, &rmm, ryy + incr_year,
				       DVar))
				    ryy = SPECIAL_VALUE;
				}
			      else
				if (!precomp_date
				    (rhn, rhwd, &rdd, &rmm, ryy, DVar))
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
		         All %?[STARTING_DATE][#[ENDING_DATE]] special texts.
		       */
		      if (is_ie)
			{
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
				      if (!dflt_ryy_set
					  && (fiscal_month > MONTH_MIN))
					rdd =
					  days_of_february (ryy + incr_year);
				      else
					rdd = days_of_february (ryy);
				    }
				  else
				    rdd = dvec[rmm - 1];
				}
			    }
			}
		      else
			/*
			   All other %?[DATE] special texts.
			 */
			{
			  /*
			     Assume current month/day.
			   */
			  if (!mm)
			    mm = m;
			  if (!dd)
			    dd = d;
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
			nth_weekday_of_month (&dd, &mm, &yy, &nn,
					      &is_weekday_mode);
		      if (rnn)
			nth_weekday_of_month (&rdd, &rmm, &ryy, &rnn,
					      &ris_weekday_mode);
		      /*
		         Proceed if (optionally specified) date is valid.
		       */
		      if ((!is_range
			   && (yy != SPECIAL_VALUE))
			  || (is_range
			      && (yy != SPECIAL_VALUE)
			      && (ryy != SPECIAL_VALUE)))
			{
			  register int true_year = (y) ? y : year + incr_year;


			  if (!nn
			      && !dflt_yy_set
			      && (fiscal_month > MONTH_MIN
				  || (incr_year
				      && (rc_tomorrow_flag || rc_week_flag))))
			    yy += incr_year;
			  if (!rnn
			      && !dflt_ryy_set
			      && (fiscal_month > MONTH_MIN
				  || (incr_year
				      && (rc_tomorrow_flag || rc_week_flag))))
			    ryy += incr_year;
			  /*
			     Manage the %?[STARTING_DATE[#ENDING_DATE]] special texts.
			   */
			  if (is_ie)
			    {
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
				  if ((s_doy != SPECIAL_VALUE)
				      && (e_doy != SPECIAL_VALUE))
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
			  else
			    /*
			       Manage the %?[DATE] special texts.
			     */
			    {
			      if ((special_text_char != RC_BYEAR_CHAR)
				  && (special_text_char != RC_YEAR_DIFF_CHAR))
				yy += incr_year;
			      i = weekday_of_date (dd, mm, yy);
			      switch (special_text_char)
				{
				case RC_WDNAME_CHAR:
				  /*
				     %weekday_name special text.
				   */
				  (void) use_format (&s2, 0, day_name (i), 0,
						     FALSE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_WDNR_M1_2_S7_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "", i, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_WDNR_M0_2_S6_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "", i - 1, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_WDNR_S1_2_S7_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "",
						     (i ==
						      DAY_MAX) ? DAY_MIN : i +
						     1, TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_WDNR_S0_2_S6_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "",
						     (i ==
						      DAY_MAX) ? DAY_MIN -
						     1 : i, TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_WDNR_X1_2_X7_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "",
						     SYEAR (i, start_day),
						     TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_WDNR_X0_2_X6_CHAR:
				  /*
				     %weekday_number special text.
				   */
				  (void) use_format (&s2, 0, "",
						     (SYEAR (i, start_day)) -
						     1, TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_DOYNR_CHAR:
				  /*
				     %day_of_year_number special text.
				   */
				  (void) use_format (&s2, 0, "",
						     day_of_year (dd, mm, yy),
						     TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_DAYNR_CHAR:
				  /*
				     %day_number special text.
				   */
				  (void) use_format (&s2, 0, "", dd, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_MONTHNAME_CHAR:
				  /*
				     %month_name special text.
				   */
				  (void) use_format (&s2, 0, month_name (mm),
						     0, FALSE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  break;
				case RC_MONTHNR_CHAR:
				  /*
				     %month_number special text.
				   */
				  (void) use_format (&s2, 0, "", mm, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_WEEKNR_CHAR:
				  /*
				     %week_number special text.
				   */
				  i =
				    week_number (dd, mm, yy, iso_week_number,
						 start_day);
				  /*
				     We convert the computed week number to a week number text
				     (this looks nicer in output).
				   */
				  if (i < 0)
				    /*
				       Week starts in previous year and the first days
				       of the actual year are not in its first week.
				     */
				    sprintf (s2, "%02d/0", -i);
				  else if (!i)
				    /*
				       Week starts in previous year and the first days
				       of the actual year are in its first week.
				     */
				    sprintf (s2, "%s", "53/1");
				  else
				    /*
				       Week starts in actual year.
				     */
				    sprintf (s2, "%02d", i);
				  break;
				case RC_YEAR_DIFF_CHAR:
				  /*
				     %year_difference special text.
				   */
				  yy = yy - true_year;
				  if (m < mm || ((m == mm) && (d < dd)))
				    {
				      if (yy < 1)
					yy++;
				      else if (yy > 1)
					yy--;
				    }
				  (void) use_format (&s2, 0, "", yy, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_YEARNR_CHAR:
				  /*
				     %year_number special text.
				   */
				  if (transform_year)
				    {
				      yy = yy - transform_year;
				      if ((yy >= 0) && (transform_year > 0))
					yy++;
				    }
				  (void) use_format (&s2, 0, "", yy, TRUE,
						     is_cformat, is_lformat,
						     is_sign, is_lzero,
						     is_suffix, is_fformat,
						     fstyle, fwidth, 0);
				  break;
				case RC_BYEAR_CHAR:
				  /*
				     %birth_age_number special text.
				   */
				  yy = true_year - yy;
				  if (m < mm || ((m == mm) && (d < dd)))
				    yy--;
				  if (yy >= 1)
				    (void) use_format (&s2, 0, "", yy, TRUE,
						       is_cformat, is_lformat,
						       is_sign, is_lzero,
						       is_suffix, is_fformat,
						       fstyle, fwidth, 0);
				  else
				    *s2 = '\0';
				  break;
				case RC_MOON_CHAR:
				  /*
				     Calculate the Moon phase.
				   */
				  i = moondisk (&moon_min_max, dd, mm, yy,
						time_hour_offset,
						time_min_offset);
				  (void) use_format (&s2, 0, "", abs (i),
						     TRUE, is_cformat,
						     is_lformat, is_sign,
						     is_lzero, is_suffix,
						     is_fformat, fstyle,
						     fwidth, 0);
				  strcat (s2, "%");
				  if (moon_min_max)
				    {
				      if (abs (i) < 50)
					strcat (s2, MOON_MIN);
				      else
					strcat (s2, MOON_MAX);
				    }
				  else if (i < 0)
				    strcat (s2, MOON_WANES);
				  else
				    strcat (s2, MOON_WAXES);
				  break;
				case RC_MOON_IMAGE_CHAR:
				  /*
				     %moon_phase_text_graphics_image special text.
				     Calculate the Moon phase.
				   */
				  draw_moon (moondisk
					     (&moon_min_max, dd, mm, yy,
					      time_hour_offset,
					      time_min_offset),
					     ((fwidth >= MOONIMAGE_MIN)
					      && (fwidth <=
						  MOONIMAGE_MAX)) ? fwidth :
					     rc_moonimage_lines, &s2);
				  /*
				     Exchange all RC_NL_CHAR=='~' by RC_NL2_CHAR=='^'
				     in `s2' in case the Moon text graphics special
				     text is lead by a RC_NL2_CHAR.
				   */
				  if (k)
				    if (s1[k - 1] == RC_NL2_CHAR)
				      for (i = 0; s2[i]; i++)
					if (s2[i] == RC_NL_CHAR)
					  s2[i] = RC_NL2_CHAR;
				  break;
				case RC_BIO_CHAR:
				  /*
				     %biorhythm special text.
				   */
				  if (!y)
				    y = year + incr_year - decr_year;
				  (void) biorhythm (FALSE, rc_bio_axis_len,
						    s2, d, m, y, dd, mm, yy,
						    rc_bio_emo_lit,
						    &bio_emo_phase,
						    &bio_emo_waxes,
						    rc_bio_int_lit,
						    &bio_int_phase,
						    &bio_int_waxes,
						    rc_bio_phy_lit,
						    &bio_phy_phase,
						    &bio_phy_waxes,
						    &bio_critical_day,
						    &bio_positive_day,
						    &bio_negative_day);
				  /*
				     Convert the "emotional" phase value to a string of template `+NNN%+',
				     where `+' indicates a sign and `N' indicates a digit (use '0' if none).
				   */
				  sprintf (bio_emo_ptext, "%4d%%%c",
					   abs (bio_emo_phase),
					   (abs (bio_emo_phase) ==
					    100) ? *BIO_STABLE
					   : ((bio_emo_waxes) ? *BIO_WAXES :
					      *BIO_WANES));
				  ptr_char = bio_emo_ptext + 1;
				  while (isspace (*ptr_char))
				    *ptr_char++ = '0';
				  if (bio_emo_phase < 0)
				    *bio_emo_ptext = *BIO_WANES;
				  else
				    *bio_emo_ptext = *BIO_WAXES;
				  /*
				     Convert the "intellectual" phase value to a string of template `+NNN%+',
				     where `+' indicates a sign and `N' indicates a digit (use '0' if none).
				   */
				  sprintf (bio_int_ptext, "%4d%%%c",
					   abs (bio_int_phase),
					   (abs (bio_int_phase) ==
					    100) ? *BIO_STABLE
					   : ((bio_int_waxes) ? *BIO_WAXES :
					      *BIO_WANES));
				  ptr_char = bio_int_ptext + 1;
				  while (isspace (*ptr_char))
				    *ptr_char++ = '0';
				  if (bio_int_phase < 0)
				    *bio_int_ptext = *BIO_WANES;
				  else
				    *bio_int_ptext = *BIO_WAXES;
				  /*
				     Convert the "physical" phase value to a string of template `+NNN%+',
				     where `+' indicates a sign and `N' indicates a digit (use '0' if none).
				   */
				  sprintf (bio_phy_ptext, "%4d%%%c",
					   abs (bio_phy_phase),
					   (abs (bio_phy_phase) ==
					    100) ? *BIO_STABLE
					   : ((bio_phy_waxes) ? *BIO_WAXES :
					      *BIO_WANES));
				  ptr_char = bio_phy_ptext + 1;
				  while (isspace (*ptr_char))
				    *ptr_char++ = '0';
				  if (bio_phy_phase < 0)
				    *bio_phy_ptext = *BIO_WANES;
				  else
				    *bio_phy_ptext = *BIO_WAXES;
				  /*
				     Create the resulting biorhythm values line.
				   */
				  sprintf (s2,
					   "%d%c %d%c %d%c , %s=%s %s=%s %s=%s",
					   bio_critical_day, *BIO_CRITICAL,
					   bio_positive_day, *BIO_WAXES,
					   bio_negative_day, *BIO_WANES,
					   rc_bio_emo_lit, bio_emo_ptext,
					   rc_bio_int_lit, bio_int_ptext,
					   rc_bio_phy_lit, bio_phy_ptext);
				  break;
				case RC_BIO_BAR_CHAR:
				  /*
				     %biorhythm_text_graphics_bar special text.
				   */
				  if (fwidth >= BIO_AXIS_MIN
				      || fwidth <= BIO_AXIS_MAX)
				    /*
				       Decrease `fwidth' by 1 until it
				       divides BIO_AXIS_MAX without a remainder.
				     */
				    while (BIO_AXIS_MAX % fwidth)
				      fwidth--;
				  if (!y)
				    y = year + incr_year - decr_year;
				  s2 =
				    biorhythm (TRUE,
					       (fwidth !=
						SPECIAL_VALUE) ? fwidth :
					       rc_bio_axis_len, s2, d, m, y,
					       dd, mm, yy, rc_bio_emo_lit,
					       &bio_emo_phase, &bio_emo_waxes,
					       rc_bio_int_lit, &bio_int_phase,
					       &bio_int_waxes, rc_bio_phy_lit,
					       &bio_phy_phase, &bio_phy_waxes,
					       &bio_critical_day,
					       &bio_positive_day,
					       &bio_negative_day);
				  break;
				default:
				  /*
				     This case MUST be an internal error!
				   */
				  abort ();
				}
			      /*
			         And copy the expanded special text into the target string.
			       */
			      i = 0;
			      /*
			         A Moon text graphics special text leads the text part of
			         a resource file line so it is good now to remove its
			         leading newline character, which is represented by
			         a RC_NL_CHAR=='~' in `s2[0]', to avoid a redundant
			         line break-up, but only in this definite case and in
			         the case, all RC_NL_CHAR's of the Moon text graphics
			         string are already exchanged by RC_NL2_CHAR's=='^'!
			       */
			      if ((!k
				   && (*s2 == RC_NL_CHAR))
				  || (*s2 == RC_NL2_CHAR))
				i++;
			      while (s2[i])
				{
				  if ((Uint) k >= maxlen_max - (Uint) len_fn)
				    resize_all_strings (maxlen_max << 1,
							FALSE, __FILE__,
							(long) __LINE__);
				  s1[k++] = s6[kk++] = s2[i++];
				}
			    }
			  if (!*(the_text + j))
			    ok = TRUE;
			}
		      is_obsolete_whitespace = TRUE;
		    }
		}
	      else
		{
		  if (special_text_char == RC_TIME_CHAR
		      || special_text_char == RC_TIME_TMI_CHAR
		      || special_text_char == RC_TIME_HR_CHAR
		      || special_text_char == RC_TIME_MI_CHAR
		      || special_text_char == RC_TIME_AMPM_CHAR
		      || special_text_char == RC_GMTIME_CHAR
		      || special_text_char == RC_GMTIME_TMI_CHAR
		      || special_text_char == RC_GMTIME_HR_CHAR
		      || special_text_char == RC_GMTIME_MI_CHAR
		      || special_text_char == RC_GMTIME_AMPM_CHAR
		      || special_text_char == RC_LT_ZONE_OFFS_CHAR
		      || special_text_char == RC_UT_ZONE_OFFS_CHAR
		      || special_text_char == RC_DISTANCE_CHAR
		      || special_text_char == RC_SUN_RISE_CHAR
		      || special_text_char == RC_SUN_SET_CHAR
		      || special_text_char == RC_SUN_DAY_CHAR
		      || special_text_char == RC_SUN_NIGHT_CHAR
		      || special_text_char == RC_MOON_RISE_CHAR
		      || special_text_char == RC_MOON_SET_CHAR
		      || special_text_char == RC_MOON_DAY_CHAR
		      || special_text_char == RC_MOON_NIGHT_CHAR
		      || special_text_char == RC_ENV_VAR_CHAR
		      || special_text_char == RC_JDAYS_CHAR
		      || special_text_char == RC_DAY_DIFF_CHAR
		      || special_text_char == RC_WEEK_DIFF_CHAR
		      || special_text_char == RC_MONTH_DIFF_CHAR
		      || special_text_char == RC_YEAR_DIFF_CHAR
		      || special_text_char == RC_HLS1S_CHAR
		      || special_text_char == RC_HLS1E_CHAR
		      || special_text_char == RC_HLS2S_CHAR
		      || special_text_char == RC_HLS2E_CHAR
		      || special_text_char == RC_HLS3S_CHAR
		      || special_text_char == RC_HLS3E_CHAR
		      || special_text_char == RC_HLS4S_CHAR
		      || special_text_char == RC_HLS4E_CHAR
		      || special_text_char == RC_HLS5S_CHAR
		      || special_text_char == RC_HLS5E_CHAR)
		    {
		      if (special_text_char == RC_TIME_CHAR
			  || special_text_char == RC_TIME_TMI_CHAR
			  || special_text_char == RC_TIME_HR_CHAR
			  || special_text_char == RC_TIME_MI_CHAR
			  || special_text_char == RC_TIME_AMPM_CHAR
			  || special_text_char == RC_GMTIME_CHAR
			  || special_text_char == RC_GMTIME_TMI_CHAR
			  || special_text_char == RC_GMTIME_HR_CHAR
			  || special_text_char == RC_GMTIME_MI_CHAR
			  || special_text_char == RC_GMTIME_AMPM_CHAR
			  || special_text_char == RC_LT_ZONE_OFFS_CHAR
			  || special_text_char == RC_UT_ZONE_OFFS_CHAR)
			{
			  register int sign = 0;
			  register int state = 1;
			  register int digits = 0;
			  auto Bool skip = FALSE;
			  auto Bool is_last = FALSE;
			  auto Bool is_error = FALSE;
			  auto Bool is_2error = FALSE;
			  auto Bool time_sep_found = FALSE;
			  auto Bool is_leading_zero = TRUE;
			  auto Bool is_format_modifier_given = FALSE;
			  auto Bool is_tzo =
			    (Bool) (special_text_char == RC_LT_ZONE_OFFS_CHAR
				    || special_text_char ==
				    RC_UT_ZONE_OFFS_CHAR);
			  auto Bool is_gmt =
			    (Bool) (special_text_char == RC_GMTIME_CHAR
				    || special_text_char == RC_GMTIME_HR_CHAR
				    || special_text_char == RC_GMTIME_MI_CHAR
				    || special_text_char == RC_GMTIME_TMI_CHAR
				    || special_text_char ==
				    RC_GMTIME_AMPM_CHAR);


			  /*
			     If %time[*][[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current system time HH:MM, if
			     `*' is specified, replace it by the current system
			     time hour converted to the am|pm notation and provide
			     it by a trailing am|pm suffix.
			     If %time_total_minutes[[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the total system time minutes MMMM.
			     If %time_hour[*][[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current system time hour HH, if
			     `*' is specified, replace it by the current system
			     time hour converted to the am|pm notation.
			     If %time_minute[[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current system time minute MM.
			     If %time_am_pm_suffix special text is found:
			     replace it by the current system time `am|pm' suffix.
			     If %gmtime[*][[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current Greenwich Mean Time and date
			     HH:MM/TEXTUAL_DATE, if `*' is specified, replace it by
			     the current GMT hour converted to the am|pm notation
			     and provide it by a trailing am|pm suffix.
			     If %gmtime_total_minutes[[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the total Greenwich Mean Time minutes
			     and date MMMM/TEXTUAL_DATE.
			     If %gmtime_hour[*][[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current Greenwich Mean Time hour HH,
			     if `*' is specified, replace it by the current GMT
			     hour converted to the am|pm notation.
			     If %gmtime_minute[[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the current GMT minute MM.
			     If %gmtime_am_pm_suffix special text is found:
			     replace it by the current Greenwich Mean Time `am|pm'
			     suffix.
			     If %timezone_offset[[+|-]MMMM|HH:[MM]] special text is found:
			     replace it by the timezone offset +|-HHMM.
			     All above mentioned special texts respect a possibly
			     given displacement value [[+|-]MMMM|HH:[MM]].
			     If [+|-]MMMM is given, skip all possibly leading
			     zeroes of MMMM.  The HH:[MM] time separating character
			     ':' is choosen by the used locale at runtime.  If no
			     am|pm notation is allowed but a `*' character is
			     leading an above mentioned displacement value,
			     ignore that statement so it has no further effect.
			   */
			  i = 0;
			  j++;
			  if (*(the_text + j) == RC_12_HOURS_FORMAT_CHAR)
			    {
			      j++;
			      switch (special_text_char)
				{
				case RC_TIME_CHAR:
				case RC_GMTIME_CHAR:
				case RC_TIME_HR_CHAR:
				case RC_GMTIME_HR_CHAR:
				  is_format_modifier_given = TRUE;
				  break;
				default:
				  ;	/* Void, nothing to do! */
				}
			    }
			  while (*(the_text + j)
				 && !isspace (*(the_text + j)))
			    {
			      skip = FALSE;
			      /*
			         Scan the remaining argument of the %[gm]time* special text.
			       */
			      switch (state)
				{
				case 1:
				  if (!isdigit (*(the_text + j)))
				    {
				      if (*(the_text + j) == *ASC_LIT
					  || *(the_text + j) == *DES_LIT)
					{
					  if (sign || time_sep_found)
					    state = 0;
					  else
					    sign++;
					  break;
					}
				      else
					state++;
				    }
				  else
				    {
				      if (*(the_text + j) == '0')
					{
					  if (is_leading_zero)
					    {
					      skip = TRUE;
					      break;
					    }
					  digits++;
					}
				      else
					digits++;
				      is_leading_zero = FALSE;
				      break;
				    }
				  /* Fallthrough. */
				case 2:
				  if (!digits && !is_leading_zero)
				    state = 0;
				  else if (!time_sep_found)
				    {
				      if (*(the_text + j) == *time_sep
					  || *(the_text + j) ==
					  *DEFAULT_TIME_SEP)
					{
					  if (digits > 2)
					    state = 0;
					  else
					    {
					      time_sep_found = TRUE;
					      digits = 0;
					      state--;
					    }
					}
				      else
					state = 0;
				    }
				  else
				    {
				      if (digits > 4)
					state = 0;
				      else
					is_last = TRUE;
				    }
				  break;
				default:
				  state = 0;
				}
			      if (state && !skip)
				s2[i++] = *(the_text + j);
			      j++;
			    }
			  if (!state
			      || is_last
			      || (i
				  && !time_sep_found
				  && !is_leading_zero
				  && !digits)
			      || (time_sep_found
				  && (digits > 2))
			      || ((state == 1) && (digits > 4)))
			    /*
			       Error, invalid condition occurred.
			     */
			    is_error = TRUE;
			  j--;
			  if (i && !is_error)
			    {
			      s2[i] = '\0';
			      /*
			         Process the time displacement value.
			       */
			      i = atoi (s2);
			      if (time_sep_found)
				{
				  i *= MINS_PER_HOUR;
				  ptr_char = strchr (s2, *time_sep);
				  if (ptr_char == (char *) NULL)
				    ptr_char = strchr (s2, *DEFAULT_TIME_SEP);
				  if (*++ptr_char)
				    {
				      state = atoi (ptr_char);
				      if (abs (state) >= MINS_PER_HOUR)
					is_error = TRUE;
				      else if (!i)
					{
					  if (*s2 == *DES_LIT)
					    i = -state;
					  else
					    i = state;
					}
				      else if (SGN (i) < 0)
					i -= state;
				      else
					i += state;
				    }
				}
			    }
			  if (!is_error)
			    {
			      register int the_time = 0;
			      auto int gmt_day;
			      auto int gmt_month;
			      auto int gmt_year;
			      auto char *time_suffix = "";
			      auto Bool is_am_pm =
				(Bool) (special_text_char == RC_TIME_AMPM_CHAR
					|| special_text_char ==
					RC_GMTIME_AMPM_CHAR);


			      if (is_tzo)
				{
				  if (special_text_char ==
				      RC_LT_ZONE_OFFS_CHAR)
				    {
				      if (gmt_loc_diff > 0)
					the_time =
					  (MINS_PER_DAY -
					   HHMM2MM (act_hour,
						    act_min)) +
					  HHMM2MM (gmt_hour, gmt_min);
				      else if (gmt_loc_diff < 0)
					the_time =
					  (MINS_PER_DAY -
					   HHMM2MM (gmt_hour,
						    gmt_min)) +
					  HHMM2MM (act_hour, act_min);
				      else
					the_time =
					  HHMM2MM (act_hour,
						   act_min) -
					  HHMM2MM (gmt_hour, gmt_min);
				    }
				  sign = 1;
				  the_time += i;
				  if (the_time < 0)
				    {
				      sign = -sign;
				      the_time = -the_time;
				    }
				  i = the_time % MINS_PER_HOUR;
				  the_time /= MINS_PER_HOUR;
				}
			      else if (!is_gmt)
				the_time = HHMM2MM (act_hour, act_min) + i;
			      else
				{
				  the_time = HHMM2MM (gmt_hour, gmt_min) + i;
				  /*
				     Adjust the GMT date in case it is not
				     the today's local date.
				   */
				  if (d_between
				      (d, m, year + incr_year - decr_year,
				       buf_gd, buf_gm, buf_gy))
				    {
				      gmt_day = d;
				      gmt_month = m;
				      gmt_year = year + incr_year - decr_year;
				      if (gmt_loc_diff > 0)
					{
					  (void) next_date (&gmt_day,
							    &gmt_month,
							    &gmt_year);
					  if (gmt_year > YEAR_MAX)
					    is_2error = TRUE;
					}
				      else if (gmt_loc_diff < 0)
					{
					  (void) prev_date (&gmt_day,
							    &gmt_month,
							    &gmt_year);
					  if (gmt_year < YEAR_MIN)
					    is_2error = TRUE;
					}
				    }
				  else
				    {
				      gmt_day = buf_gd;
				      gmt_month = buf_gm;
				      gmt_year = buf_gy;
				    }
				}
			      if (!is_error && !is_2error && !is_tzo)
				{
				  i = the_time % MINS_PER_HOUR;
				  the_time /= MINS_PER_HOUR;
				  if (the_time >= HOURS_PER_DAY)
				    while (the_time >= HOURS_PER_DAY)
				      {
					the_time -= HOURS_PER_DAY;
					if (is_gmt)
					  {
					    (void) next_date (&gmt_day,
							      &gmt_month,
							      &gmt_year);
					    if (gmt_year > YEAR_MAX)
					      is_2error = TRUE;
					  }
				      }
				  else if (i < 0 || the_time < 0)
				    {
				      if (the_time <= 0)
					the_time--;
				      while (the_time < 0)
					{
					  the_time += HOURS_PER_DAY;
					  if (is_gmt)
					    {
					      (void) prev_date (&gmt_day,
								&gmt_month,
								&gmt_year);
					      if (gmt_year < YEAR_MIN)
						is_2error = TRUE;
					    }
					}
				      if (i)
					i = MINS_PER_HOUR + i;
				      else
					the_time++;
				    }
				}
			      if (!is_error && !is_2error)
				{
				  if (is_am_pm || is_format_modifier_given)
				    {
				      /*
				         Copy a trailing "am" resp., "pm" suffix
				         to the text.
				       */
				      if (the_time >= HOURS_PER_HALF_DAY)
					{
					  /*
					   *** Translators, please translate this as a fixed 2-character text.
					   *** This text should be a proper abbreviation of "post meridiem".
					   */
					  time_suffix = _("pm");
					  if (!is_am_pm)
					    the_time -= HOURS_PER_HALF_DAY;
					}
				      else
					/*
					 *** Translators, please translate this as a fixed 2-character text.
					 *** This text should be a proper abbreviation of "ante meridiem".
					 */
					time_suffix = _("am");
				    }
				  if (transform_year)
				    {
				      gmt_year = gmt_year - transform_year;
				      if ((gmt_year >= 0)
					  && (transform_year > 0))
					gmt_year++;
				    }
				  *s2 = '\0';
				  switch (special_text_char)
				    {
				    case RC_TIME_CHAR:
				      /*
				         %time special text.
				       */
				      sprintf (buffer, "%02d%s%02d%s",
					       the_time, time_sep, i,
					       time_suffix);
				      break;
				    case RC_GMTIME_CHAR:
				      /*
				         %gmtime special text.
				       */
				      sprintf (buffer,
					       "%02d%s%02d%s/%02d-%s-%0*d",
					       the_time, time_sep, i,
					       time_suffix, gmt_day,
					       short_month_name (gmt_month),
					       len_year_max, gmt_year);
				      break;
				    case RC_TIME_TMI_CHAR:
				      /*
				         %time_total_minutes special text.
				       */
				      (void) use_format (&s2, 0, "",
							 the_time *
							 MINS_PER_HOUR + i,
							 TRUE, is_cformat,
							 is_lformat, is_sign,
							 is_lzero, is_suffix,
							 is_fformat, fstyle,
							 fwidth, 0);
				      break;
				    case RC_GMTIME_TMI_CHAR:
				      /*
				         %gmtime_total_minutes special text.
				       */
				      {
					sprintf (buffer, "/%02d-%s-%0*d",
						 gmt_day,
						 short_month_name (gmt_month),
						 len_year_max, gmt_year);
					(void) use_format (&s2, 0, "",
							   the_time *
							   MINS_PER_HOUR + i,
							   TRUE, is_cformat,
							   is_lformat,
							   is_sign, is_lzero,
							   is_suffix,
							   is_fformat, fstyle,
							   fwidth, 0);
					strcat (s2, buffer);
				      }
				      break;
				    case RC_TIME_HR_CHAR:
				    case RC_GMTIME_HR_CHAR:
				      /*
				         %[gm]time_hour special text.
				       */
				      (void) use_format (&s2, 0, "", the_time,
							 TRUE, is_cformat,
							 is_lformat, is_sign,
							 is_lzero, is_suffix,
							 is_fformat, fstyle,
							 fwidth, 0);
				      break;
				    case RC_TIME_MI_CHAR:
				    case RC_GMTIME_MI_CHAR:
				      /*
				         %[gm]time_minute special text.
				       */
				      (void) use_format (&s2, 0, "", i, TRUE,
							 is_cformat,
							 is_lformat, is_sign,
							 is_lzero, is_suffix,
							 is_fformat, fstyle,
							 fwidth, 0);
				      break;
				    case RC_TIME_AMPM_CHAR:
				    case RC_GMTIME_AMPM_CHAR:
				      /*
				         %[gm]time_am_pm_suffix special text.
				       */
				      (void) use_format (&s2, 0, time_suffix,
							 0, FALSE, is_cformat,
							 is_lformat, is_sign,
							 is_lzero, is_suffix,
							 is_fformat, fstyle,
							 fwidth, 0);
				      break;
				    case RC_LT_ZONE_OFFS_CHAR:
				    case RC_UT_ZONE_OFFS_CHAR:
				      /*
				         %timezone_offset either based on local time
				         zone or UTC/GMT zone special text.
				       */
				      sprintf (buffer, "%s%02d%02d",
					       (sign > 0) ? ASC_LIT : DES_LIT,
					       the_time, i);
				      break;
				    default:
				      /*
				         This case MUST be an internal error!
				       */
				      abort ();
				    }
				}
			    }
			  if (!is_error && !is_2error)
			    {
			      if (!*s2)
				(void) use_format (&s2, 0, buffer, 0, FALSE,
						   is_cformat, is_lformat,
						   is_sign, is_lzero,
						   is_suffix, is_fformat,
						   fstyle, fwidth, 0);
			    }
			  else
			    {
			      for (i = 0; i < len_year_max; i++)
				buffer[i] = *RC_INVALID_PARAM_TEXT;
			      buffer[i] = '\0';
			      switch (special_text_char)
				{
				case RC_TIME_CHAR:
				  sprintf (s2, "%s%s%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT, time_sep,
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT);
				  break;
				case RC_GMTIME_CHAR:
				  sprintf (s2, "%s%s%s/%s-%s%c-%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT, time_sep,
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT,
					   RC_INVALID_PARAM_TEXT,
					   RC_INVALID_PARAM_TEXT,
					   *RC_INVALID_PARAM_TEXT, buffer);
				  break;
				case RC_TIME_TMI_CHAR:
				  sprintf (s2, "%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT);
				  break;
				case RC_GMTIME_TMI_CHAR:
				  sprintf (s2, "%s/%s-%s%c-%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT,
					   RC_INVALID_PARAM_TEXT,
					   RC_INVALID_PARAM_TEXT,
					   *RC_INVALID_PARAM_TEXT, buffer);
				  break;
				case RC_TIME_HR_CHAR:
				case RC_GMTIME_HR_CHAR:
				case RC_TIME_MI_CHAR:
				case RC_GMTIME_MI_CHAR:
				case RC_TIME_AMPM_CHAR:
				case RC_GMTIME_AMPM_CHAR:
				  sprintf (s2, "%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT);
				  break;
				case RC_LT_ZONE_OFFS_CHAR:
				case RC_UT_ZONE_OFFS_CHAR:
				  sprintf (s2, "+%s%s",
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT,
					   (is_error) ? RC_INVALID_PARAM_TEXT
					   : RC_INVALID_VALUE_TEXT);
				  break;
				default:
				  /*
				     This case MUST be an internal error!
				   */
				  abort ();
				}
			      strcpy (buffer, s2);
			      *s2 = '\0';
			      (void) use_format (&s2, 0, buffer, 0, FALSE,
						 is_cformat, is_lformat,
						 is_sign, is_lzero, is_suffix,
						 is_fformat, fstyle, fwidth,
						 0);
			    }
			  is_obsolete_whitespace = TRUE;
			}
		      else
			if (special_text_char == RC_DISTANCE_CHAR
			    || special_text_char == RC_SUN_RISE_CHAR
			    || special_text_char == RC_SUN_SET_CHAR
			    || special_text_char == RC_SUN_DAY_CHAR
			    || special_text_char == RC_SUN_NIGHT_CHAR
			    || special_text_char == RC_MOON_RISE_CHAR
			    || special_text_char == RC_MOON_SET_CHAR
			    || special_text_char == RC_MOON_DAY_CHAR
			    || special_text_char == RC_MOON_NIGHT_CHAR)
			{
			  auto double the_time = 0.0;
			  auto double the_error;
			  auto int dots;
			  auto int sign;
			  register int split;
			  register int state;
			  register int digits;
			  register int decimals;
			  register int num_coord;
			  register int parsed_coordinates =
			    (special_text_char == RC_DISTANCE_CHAR) ? 2 : 1;
			  register int dist_mode = 0;
			  register int aobj_mode = 5;
			  register int astronomical_object = AOBJ_NONE;
			  auto char *sep;
			  auto char *time_suffix = "";
			  auto Bool skip;
			  auto Bool is_last;
			  auto Bool is_error;
			  auto Bool is_decimal;
			  auto Bool got_lat_min;
			  auto Bool got_lon_min;
			  auto Bool time_sep_found;
			  auto Bool is_leading_zero;
			  auto Bool is_format_modifier_given = FALSE;
			  auto Bool is_rise_set =
			    (Bool) (special_text_char == RC_SUN_RISE_CHAR
				    || special_text_char == RC_SUN_SET_CHAR
				    || special_text_char == RC_MOON_RISE_CHAR
				    || special_text_char == RC_MOON_SET_CHAR);


			  /*
			     Decode and evaluate co-ordinate-based special texts.
			   */
			  j++;
			  if (special_text_char == RC_DISTANCE_CHAR)
			    {
			      if (*(the_text + j) == RC_SMILES_FORMAT_CHAR)
				{
				  j++;
				  is_format_modifier_given = TRUE;
				}
			      if (isdigit (*(the_text + j)))
				{
				  dist_mode = CHR2DIG (*(the_text + j));
				  if (dist_mode <= 2)
				    j++;
				  else
				    dist_mode = 0;
				}
			    }
			  else
			    {
			      if (special_text_char == RC_SUN_RISE_CHAR
				  || special_text_char == RC_SUN_SET_CHAR
				  || special_text_char == RC_SUN_DAY_CHAR
				  || special_text_char == RC_SUN_NIGHT_CHAR)
				astronomical_object = AOBJ_SUN;
			      else
				astronomical_object = AOBJ_MOON;
			      if (*(the_text + j) == RC_12_HOURS_FORMAT_CHAR)
				{
				  j++;
				  is_format_modifier_given = TRUE;
				}
			      if (isdigit (*(the_text + j)))
				{
				  aobj_mode = CHR2DIG (*(the_text + j));
				  j++;
				}
			      else if (islower (*(the_text + j)))
				{
				  aobj_mode = IDX (*(the_text + j)) + 10;
				  j++;
				}
			      else if (isupper (*(the_text + j)))
				{
				  aobj_mode = IDX (*(the_text + j)) + 36;
				  if (((astronomical_object == AOBJ_SUN)
				       && (aobj_mode <= 53))
				      || ((astronomical_object == AOBJ_MOON)
					  && (aobj_mode <= 60)))
				    j++;
				  else
				    aobj_mode = 61;
				}
			    }
			  if (astronomical_object == AOBJ_NONE)
			    /*
			     *** Translators, please translate this as a fixed 2-character text.
			     *** This text should contain the proper abbreviations for "degrees" and "minutes".
			     */
			    sep = _("d'\"");
			  else
			    if (is_rise_set
				&&
				(aobj_oformat[aobj_mode][astronomical_object].
				 error_format == ERR_DDD))
			    /*
			     *** Translators, please translate this as a fixed 3-character text.
			     *** This text should contain the proper abbreviations for "degrees", "minutes" and "seconds".
			     */
			    sep = _("d'\"");
			  else
			    if (!is_rise_set
				||
				aobj_oformat[aobj_mode][astronomical_object].
				error_format == ERR_TTT)
			    /*
			     *** Translators, please translate this as a fixed 3-character text.
			     *** This text should contain the proper abbreviations for "hours", "minutes" and "seconds".
			     */
			    sep = _("h'\"");
			  else
			    sep = time_sep;
			LABEL_get_second_coordinate:
			  the_error = 0.0;
			  num_coord = state = 1;
			  i = dots = sign = split = digits = decimals = 0;
			  is_leading_zero = TRUE;
			  is_last = got_lat_min = got_lon_min = FALSE;
			  is_error = is_decimal = time_sep_found = FALSE;
			  coor1->lon_deg = coor1->lon_min = coor1->lon_sec =
			    0;
			  coor1->lat_deg = coor1->lat_min = coor1->lat_sec =
			    0;
			  if (special_text_char == RC_DISTANCE_CHAR)
			    coor1->the_mode = dist_mode;
			  else
			    coor1->the_mode = aobj_mode;
			  coor1->meters_above_sea_level =
			    coor1->time_zone_in_mins = 0;
			  while (((special_text_char != RC_DISTANCE_CHAR)
				  && *(the_text + j)
				  && !isspace (*(the_text + j)))
				 || ((special_text_char == RC_DISTANCE_CHAR)
				     && *(the_text + j)
				     && !isspace (*(the_text + j))
				     && (*(the_text + j) !=
					 *RC_COORDINATE_SEP)))
			    {
			      skip = FALSE;
			      /*
			         Scan the remaining argument of the %distance*,
			         %sun* or %moon* special text and arrange its
			         elements properly separated by SPLIT_SEPs.
			       */
			      switch (state)
				{
				case 1:
				  if (*(the_text + j) == *ASC_LIT
				      || *(the_text + j) == *DES_LIT)
				    state++;
				  else
				    state = 0;
				  break;
				case 2:
				  if (!isdigit (*(the_text + j)))
				    state++;
				  else
				    {
				      digits++;
				      break;
				    }
				  /* Fallthrough. */
				case 3:
				  if (!decimals
				      && (((num_coord == 1)
					   && (digits != 2)
					   && (digits != 4)
					   && (digits != 6))
					  || ((num_coord == 2)
					      && (digits != 3)
					      && (digits != 5)
					      && (digits != 7))
					  || ((num_coord == 3)
					      && (digits > 4))))
				    state = 0;
				  else if (*(the_text + j) == '.')
				    {
				      if (dots)
					state = 0;
				      else
					{
					  if (((num_coord == 1)
					       && (split <= 2))
					      || ((num_coord == 2)
						  && (split - is_decimal <=
						      5)))
					    {
					      s2[i++] = '0';
					      s2[i++] = *SPLIT_SEP;
					      if (((num_coord == 1)
						   && (split == 1))
						  || ((num_coord == 2)
						      && (split -
							  is_decimal == 4)))
						{
						  s2[i++] = '0';
						  s2[i++] = *SPLIT_SEP;
						  split++;
						}
					      split++;
					    }
					  dots++;
					  is_decimal = TRUE;
					  decimals = !(digits = 0);
					  state--;
					}
				    }
				  else
				    if (*(the_text + j) == *ASC_LIT
					|| *(the_text + j) == *DES_LIT)
				    {
				      if (decimals && !digits)
					state = 0;
				      else
					{
					  if (s2[i - 1] != *SPLIT_SEP)
					    {
					      split++;
					      s2[i++] = *SPLIT_SEP;
					    }
					  if (((num_coord == 1)
					       && (split <= 2))
					      || ((num_coord == 2)
						  && (split - is_decimal <=
						      5)))
					    {
					      s2[i++] = '0';
					      s2[i++] = *SPLIT_SEP;
					      if (((num_coord == 1)
						   && (split == 1))
						  || ((num_coord == 2)
						      && (split -
							  is_decimal == 4)))
						{
						  s2[i++] = '0';
						  s2[i++] = *SPLIT_SEP;
						  split++;
						}
					      split++;
					    }
					  decimals = dots = digits = 0;
					  num_coord++;
					  state--;
					}
				    }
				  else
				    if ((num_coord > 1)
					&& (*(the_text + j) == *SPLIT_SEP))
				    {
				      if (decimals && !digits)
					state = 0;
				      else
					{
					  if (split - is_decimal <= 5)
					    {
					      s2[i++] = '0';
					      s2[i++] = *SPLIT_SEP;
					      if (split - is_decimal == 4)
						{
						  s2[i++] = '0';
						  s2[i++] = *SPLIT_SEP;
						  split++;
						}
					      split++;
					    }
					  digits = 0;
					  state++;
					}
				    }
				  else
				    state = 0;
				  break;
				case 4:
				  if (!isdigit (*(the_text + j)))
				    {
				      if (*(the_text + j) == *ASC_LIT
					  || *(the_text + j) == *DES_LIT)
					{
					  if (sign || time_sep_found)
					    state = 0;
					  else
					    sign++;
					  break;
					}
				      else
					state++;
				    }
				  else
				    {
				      if (*(the_text + j) == '0')
					{
					  if (is_leading_zero)
					    {
					      skip = TRUE;
					      break;
					    }
					  digits++;
					}
				      else
					digits++;
				      is_leading_zero = FALSE;
				      break;
				    }
				  /* Fallthrough. */
				case 5:
				  if (!digits && !is_leading_zero)
				    state = 0;
				  else if (!time_sep_found)
				    {
				      if (*(the_text + j) == *time_sep
					  || *(the_text + j) ==
					  *DEFAULT_TIME_SEP)
					{
					  if (digits > 2)
					    state = 0;
					  else
					    {
					      time_sep_found = TRUE;
					      digits = 0;
					      state--;
					    }
					}
				      else
					state = 0;
				    }
				  else
				    {
				      if (digits > 4)
					state = 0;
				      else
					is_last = TRUE;
				    }
				  if (!state
				      && (*(the_text + j) == *SPLIT_SEP))
				    {
				      s2[i++] = *(the_text + j);
				      split++;
				      sign = FALSE;
				      digits = 0;
				      state = 4;
				    }
				  break;
				default:
				  state = 0;
				}
			      if (state && !skip)
				{
				  if (digits <= 7)
				    {
				      if (i)
					{
					  if (s2[i - 1] != *SPLIT_SEP
					      || *(the_text + j) !=
					      *SPLIT_SEP)
					    s2[i++] = *(the_text + j);
					}
				      else
					{
					  if (*(the_text + j) == *SPLIT_SEP)
					    split++;
					  s2[i++] = *(the_text + j);
					}
				      if (!decimals)
					{
					  if ((state == 2)
					      && (((num_coord == 1)
						   && (digits == 2
						       || digits == 4
						       || digits == 6))
						  || ((num_coord == 2)
						      && (digits == 3
							  || digits == 5
							  || digits == 7))))
					    if (s2[i - 1] != *SPLIT_SEP)
					      {
						split++;
						s2[i++] = *SPLIT_SEP;
					      }
					  if (state == 2)
					    {
					      if ((num_coord == 1)
						  && (digits > 2))
						got_lat_min = TRUE;
					      else
						if ((num_coord == 2)
						    && (digits > 3))
						got_lon_min = TRUE;
					    }
					}
				    }
				  else if (s2[i - 1] != *SPLIT_SEP)
				    {
				      split++;
				      s2[i++] = *SPLIT_SEP;
				    }
				}
			      j++;
			    }
			  if (i && (split - is_decimal <= 5))
			    {
			      s2[i++] = '0';
			      if (split - is_decimal == 4)
				{
				  s2[i++] = *SPLIT_SEP;
				  s2[i++] = '0';
				}
			    }
			  if (!state
			      || is_last
			      || num_coord > 3
			      || (i
				  && !time_sep_found
				  && !is_leading_zero
				  && !digits)
			      || (time_sep_found
				  && (digits > 2))
			      || (!is_decimal
				  && (num_coord == state)
				  && (digits != 3)
				  && (digits != 5)
				  && (digits != 7))
			      || ((num_coord == 3
				   || state == 4) && (digits > 4)))
			    /*
			       Error, invalid condition occurred.
			     */
			    is_error = TRUE;
			  if (j)
			    j--;
			  if (i && !is_error)
			    {
			      auto double val = 0.0;
			      auto Bool ok2 = FALSE;


			      /*
			         Store the split argument of the %sun* or %moon*
			         special text in the Coor_struct variable
			         `coordinate' properly.
			       */
			      coor1->lon_deg = coor1->lon_min =
				coor1->lon_sec = SECS_PER_HOUR;
			      coor1->lat_deg = coor1->lat_min =
				coor1->lat_sec = SECS_PER_HOUR;
			      coor1->meters_above_sea_level =
				coor1->time_zone_in_mins = SECS_PER_HOUR * 5;
			      s2[i] = '\0';
			      ptr_char = s2;
			      do
				{
				  i = 0;
				  is_decimal = FALSE;
				  /*
				     Get an element.
				   */
				  while (!ok2)
				    {
				      if (*ptr_char)
					{
					  if (*ptr_char != *SPLIT_SEP)
					    {
					      if (!i && (*ptr_char == '.'))
						is_decimal = TRUE;
					      s2[i++] = *ptr_char++;
					    }
					  else
					    {
					      ptr_char++;
					      break;
					    }
					}
				      else
					ok2 = TRUE;
				    }
				  s2[i] = '\0';
				  /*
				     Process the element.
				   */
				  if (i)
				    {
				      i = SPECIAL_VALUE;
				      if (is_decimal)
					(void) sscanf (s2, "%lf", &val);
				      else
					i = atoi (s2);
				      if (coor1->lat_deg == SECS_PER_HOUR)
					coor1->lat_deg = i;
				      else
					if (coor1->lat_min == SECS_PER_HOUR)
					coor1->lat_min =
					  i *
					  ((SGN (coor1->lat_deg)) ?
					   SGN (coor1->lat_deg) : 1);
				      else if (coor1->lat_sec ==
					       SECS_PER_HOUR)
					coor1->lat_sec =
					  i *
					  ((SGN (coor1->lat_deg)) ?
					   SGN (coor1->lat_deg) : 1);
				      else if (coor1->lon_deg ==
					       SECS_PER_HOUR)
					{
					  if (is_decimal)
					    {
					      if (!got_lat_min
						  && !coor1->lat_min)
						{
						  val *=
						    (double) MINS_PER_HOUR;
						  sprintf (buffer, "%.6f",
							   val);
						  i = atoi (buffer);
						  coor1->lat_min =
						    i *
						    ((SGN (coor1->lat_deg)) ?
						     SGN (coor1->
							  lat_deg) : 1);
						  val -= (double) i;
						}
					      if (!coor1->lat_sec)
						{
						  val *=
						    (double) SECS_PER_MIN;
						  sprintf (buffer, "%.6f",
							   val);
						  i = atoi (buffer);
						  coor1->lat_sec =
						    i *
						    ((SGN (coor1->lat_deg)) ?
						     SGN (coor1->
							  lat_deg) : 1);
						}
					    }
					  else
					    coor1->lon_deg = i;
					}
				      else
					if (coor1->lon_min == SECS_PER_HOUR)
					coor1->lon_min =
					  i *
					  ((SGN (coor1->lon_deg)) ?
					   SGN (coor1->lon_deg) : 1);
				      else if (coor1->lon_sec ==
					       SECS_PER_HOUR)
					coor1->lon_sec =
					  i *
					  ((SGN (coor1->lon_deg)) ?
					   SGN (coor1->lon_deg) : 1);
				      else if (num_coord == 3
					       && (coor1->
						   meters_above_sea_level ==
						   SECS_PER_HOUR * 5))
					coor1->meters_above_sea_level = i;
				      else
					if (coor1->time_zone_in_mins ==
					    SECS_PER_HOUR * 5)
					{
					  if (is_decimal)
					    {
					      if (!got_lon_min
						  && !coor1->lon_min)
						{
						  val *=
						    (double) MINS_PER_HOUR;
						  sprintf (buffer, "%.6f",
							   val);
						  i = atoi (buffer);
						  coor1->lon_min =
						    i *
						    ((SGN (coor1->lon_deg)) ?
						     SGN (coor1->
							  lon_deg) : 1);
						  val -= (double) i;
						}
					      if (!coor1->lon_sec)
						{
						  val *=
						    (double) SECS_PER_MIN;
						  sprintf (buffer, "%.6f",
							   val);
						  i = atoi (buffer);
						  coor1->lon_sec =
						    i *
						    ((SGN (coor1->lon_deg)) ?
						     SGN (coor1->
							  lon_deg) : 1);
						}
					    }
					  else
					    {
					      /*
					         Ensure that we always get the right timezone value
					         if the locale-specific time separator character
					         is also defined as the decimal point character DOT,
					         but we generally don't use a locale-specific decimal
					         point character like COMMA or such, so the DOT
					         indicated the use of a floating point value which
					         we now have to respect!
					       */
					      if (i == SPECIAL_VALUE && is_decimal)
						i = (int) val;
					      coor1->time_zone_in_mins = i;
					      if (time_sep_found)
						{
						  coor1->time_zone_in_mins *=
						    MINS_PER_HOUR;
						  ptr_char =
						    strchr (s2, *time_sep);
						  if (ptr_char ==
						      (char *) NULL)
						    ptr_char =
						      strchr (s2,
							      *DEFAULT_TIME_SEP);
						  if (ptr_char !=
						      (char *) NULL)
						    {
						      if (*++ptr_char)
							{
							  i = atoi (ptr_char);
							  if (abs (i) >=
							      MINS_PER_HOUR)
							    is_error = TRUE;
							  else
							    if (!coor1->
								time_zone_in_mins)
							    {
							      if (*s2 ==
								  *DES_LIT)
								coor1->
								  time_zone_in_mins
								  = -i;
							      else
								coor1->
								  time_zone_in_mins
								  = i;
							    }
							  else
							    if (SGN
								(coor1->
								 time_zone_in_mins)
								< 0)
							    coor1->
							      time_zone_in_mins
							      -= i;
							  else
							    coor1->
							      time_zone_in_mins
							      += i;
							}
						    }
						  else
						    is_error = TRUE;
						}
					    }
					}
				      else
					/*
					   Error, argument contains too many elements.
					 */
					is_error = TRUE;
				    }
				}
			      while (!ok2 && !is_error);
			      if (!is_error)
				{
				  /*
				     Check whether an invalid co-ordinate element is given.
				   */
				  if (abs (coor1->lon_deg) > COOR_LON_DEG_MAX
				      || abs (coor1->lon_min) >=
				      COOR_LON_MIN_MAX
				      || abs (coor1->lon_sec) >=
				      COOR_LON_SEC_MAX
				      ||
				      ((abs (coor1->lon_deg) ==
					COOR_LON_DEG_MAX)
				       && (coor1->lon_min != 0
					   || coor1->lon_sec != 0))
				      || abs (coor1->lat_deg) >
				      COOR_LAT_DEG_MAX
				      || abs (coor1->lat_min) >=
				      COOR_LAT_MIN_MAX
				      || abs (coor1->lat_sec) >=
				      COOR_LAT_SEC_MAX
				      ||
				      ((abs (coor1->lat_deg) ==
					COOR_LAT_DEG_MAX)
				       && (coor1->lat_min != 0
					   || coor1->lat_sec != 0)))
				    /*
				       Error, invalid co-ordinate element found.
				     */
				    is_error = TRUE;
				  else
				    {
				      /*
				         Let's check if we have to use the default
				         meters_above_sea_level value.
				       */
				      if (coor1->meters_above_sea_level ==
					  SECS_PER_HOUR * 5)
					coor1->meters_above_sea_level = 0;
				      /*
				         Let's check if we have to use the default
				         time value.
				       */
				      if (coor1->time_zone_in_mins ==
					  SECS_PER_HOUR * 5)
					coor1->time_zone_in_mins = 0;
				      else
					coor1->time_zone_in_mins =
					  (int) (SGN
						 (coor1->time_zone_in_mins) *
						 (abs
						  (coor1->time_zone_in_mins) %
						  MINS_PER_DAY));
				    }
				}
			    }
			  if (!is_error && --parsed_coordinates)
			    {
			      if (*(the_text + j + 1) == *RC_COORDINATE_SEP)
				{
				  j += 2;
				  coor2->lon_deg = coor1->lon_deg;
				  coor2->lon_min = coor1->lon_min;
				  coor2->lon_sec = coor1->lon_sec;
				  coor2->lat_deg = coor1->lat_deg;
				  coor2->lat_min = coor1->lat_min;
				  coor2->lat_sec = coor1->lat_sec;
				  coor2->meters_above_sea_level =
				    coor1->meters_above_sea_level;
				  coor2->the_mode = coor1->the_mode;
				  goto LABEL_get_second_coordinate;
				}
			      if (digits)
				is_error = TRUE;
			    }
			  /*
			     Avoid a nonsense computation of day and night length if
			     the selection made for the astronomical object's data
			     is the calculation of a non-rise/set time-based mode.
			   */
			  if (!is_rise_set
			      && (astronomical_object != AOBJ_NONE)
			      && (is_error
				  || (aobj_mode <= 1
				      || ((aobj_mode > 9)
					  && (aobj_mode < 52)
					  && (astronomical_object ==
					      AOBJ_SUN)) || ((aobj_mode > 5)
							     &&
							     (astronomical_object
							      ==
							      AOBJ_MOON)))))
			    {
			      aobj_mode = 65;
			      is_error = TRUE;
			    }
			  if (!is_error)
			    {
			      auto double rise = SECS_PER_HOUR;
			      auto double set = SECS_PER_HOUR;


			      if (!y)
				y = year + incr_year - decr_year;
			      /*
			         Get the data.
			       */
			      switch (special_text_char)
				{
				case RC_DISTANCE_CHAR:
				  the_time = compute_distance (coor2, coor1);
				  if (the_time == SPECIAL_VALUE)
				    /*
				       Internal error condition occurred
				       within the `compute_distance()' function.
				     */
				    the_error = the_time;
				  else
				    if (!dist_mode
					&& is_format_modifier_given)
				    /*
				       Convert the distance value into statute miles.
				     */
				    the_time = KM2SM (the_time);
				  break;
				case RC_SUN_DAY_CHAR:
				case RC_SUN_NIGHT_CHAR:
				case RC_SUN_RISE_CHAR:
				  rise =
				    sun_rise_set (RIse, rc_limit, d, m, y,
						  coor1);
				  if (is_rise_set)
				    {
				      if (((aobj_mode <= 9
					    || aobj_mode >= 52)
					   && (rise <= SPECIAL_VALUE))
					  || ((aobj_mode >= 37)
					      && (aobj_mode < 52)
					      && (rise <=
						  HH2SS (SPECIAL_VALUE))))
					/*
					   Internal error condition occurred
					   within the `sun_rise_set()' function.
					 */
					the_error = rise;
				      break;
				    }
				  /* Fallthrough. */
				case RC_SUN_SET_CHAR:
				  set =
				    sun_rise_set (SEt, rc_limit, d, m, y,
						  coor1);
				  if (is_rise_set
				      &&
				      (((aobj_mode <= 9 || aobj_mode >= 52)
					&& (set <= SPECIAL_VALUE))
				       || ((aobj_mode >= 37)
					   && (aobj_mode < 52)
					   && (set <=
					       HH2SS (SPECIAL_VALUE)))))
				    /*
				       Internal error condition occurred
				       within the `sun_rise_set()' function.
				     */
				    the_error = set;
				  break;
				case RC_MOON_DAY_CHAR:
				case RC_MOON_NIGHT_CHAR:
				case RC_MOON_RISE_CHAR:
				  rise = moon_rise_set (RIse, d, m, y, coor1);
				  if (is_rise_set)
				    {
				      if (((aobj_mode <= 5)
					   && (rise <= SPECIAL_VALUE))
					  || ((aobj_mode >= 40)
					      && (rise <=
						  HH2SS (SPECIAL_VALUE))))
					/*
					   Internal error condition occurred
					   within the `moon_rise_set()' function.
					 */
					the_error = rise;
				      break;
				    }
				  /* Fallthrough. */
				case RC_MOON_SET_CHAR:
				  set = moon_rise_set (SEt, d, m, y, coor1);
				  if (is_rise_set
				      && (((aobj_mode <= 5)
					   && (set <= SPECIAL_VALUE))
					  || ((aobj_mode >= 40)
					      && (set <=
						  HH2SS (SPECIAL_VALUE)))))
				    /*
				       Internal error condition occurred
				       within the `moon_rise_set()' function.
				     */
				    the_error = set;
				  break;
				default:
				  /*
				     This case MUST be an internal error!
				   */
				  abort ();
				}
			      if (!is_error
				  && (astronomical_object != AOBJ_NONE))
				{
				  if (is_rise_set)
				    {
				      if (!(int) ROUND (the_error))
					{
					  if (set == SECS_PER_HOUR)
					    the_time = rise;
					  else
					    the_time = set;
					  if (is_format_modifier_given)
					    {
					      if (aobj_oformat[aobj_mode]
						  [astronomical_object].
						  error_format == ERR_HHH)
						{
						  /*
						     Copy a trailing "am" resp., "pm" suffix to the text.
						   */
						  if (the_time >=
						      HOURS_PER_HALF_DAY)
						    {
						      /*
						       *** Translators, please translate this as a fixed 2-character text.
						       *** This text should be a proper abbreviation of "post meridiem".
						       */
						      time_suffix = _("pm");
						      the_time -=
							HOURS_PER_HALF_DAY;
						    }
						  else
						    /*
						     *** Translators, please translate this as a fixed 2-character text.
						     *** This text should be a proper abbreviation of "ante meridiem".
						     */
						    time_suffix = _("am");
						  state =
						    strlen (time_suffix);
						}
					      else
						/*
						   Manage the Sun|Moon/Earth distance and the Moon phase angle modes.
						 */
						switch (astronomical_object)
						  {
						  case AOBJ_SUN:
						    if (aobj_mode == 15
							|| aobj_mode == 24)
						      {
							/*
							   Convert the distance value into kilometers.
							 */
							the_time =
							  AU2KM (the_time);
							aobj_mode = 61;
						      }
						    break;
						  case AOBJ_MOON:
						    if (aobj_mode == 16
							|| aobj_mode == 29)
						      {
							/*
							   Convert the distance value into kilometers.
							 */
							the_time =
							  ER2KM (the_time);
							aobj_mode = 61;
						      }
						    else
						      if (aobj_mode == 9
							  || aobj_mode == 22
							  || aobj_mode == 48
							  || aobj_mode == 50
							  || aobj_mode == 53
							  || aobj_mode == 55
							  || aobj_mode == 57
							  || aobj_mode == 60)
						      /*
						         Convert the phase angle value into percents.
						       */
						      the_time *= 100.0;
						    break;
						  default:
						    ;	/* Void, nothing to do! */
						  }
					    }
					}
				    }
				  else
				    {
				      aobj_mode = 65;
				      if (rise <= SPECIAL_VALUE
					  || set <= SPECIAL_VALUE)
					{
					  if (rise == SPECIAL_VALUE
					      || rise == SPECIAL_VALUE * 2)
					    rise = 0.0;
					  else if (rise == SPECIAL_VALUE * 3)
					    rise = HOURS_PER_DAY;
					  if (set == SPECIAL_VALUE
					      || set == SPECIAL_VALUE * 2)
					    set = 0.0;
					  else if (set == SPECIAL_VALUE * 3)
					    set = HOURS_PER_DAY;
					}
				      if (rise == set)
					the_time = rise;
				      else if (rise > set)
					the_time =
					  (HOURS_PER_DAY - rise) + set;
				      else
					the_time = set - rise;
				      if (special_text_char ==
					  RC_SUN_NIGHT_CHAR
					  || special_text_char ==
					  RC_MOON_NIGHT_CHAR)
					the_time = HOURS_PER_DAY - the_time;
				    }
				}
			    }
			  /*
			     Format the data properly.
			   */
			  if (!is_error && !(int) ROUND (the_error))
			    {
			      if (astronomical_object == AOBJ_NONE)
				{
				  if (!dist_mode)
				    i = FMT_NNN;
				  else
				    i = FMT_DDD;
				}
			      else
				i =
				  aobj_oformat[aobj_mode]
				  [astronomical_object].data_format;
			      /*
			         Force representation of [signed] number with fraction.
			       */
			      if (is_format_modifier_given)
				switch (i)
				  {
				  case FMT_TTT:
				  case FMT_DDD:
				    i = FMT_NND;
				    break;
				  case FMT_TTS:
				  case FMT_TXS:
				  case FMT_DDS:
				    i = FMT_NSD;
				    break;
				  default:
				    ;	/* Void, nothing to do! */
				  }
			      if (rc_precise)
				digits = 6;
			      else
				digits = 3;
			      switch (i)
				{
				case FMT_NNN:
				  if (rc_precise)
				    digits = 3;
				  else
				    digits = 0;
				  /* Number with[out] fraction. */
				  sprintf (buffer, "%.*f", digits,
					   abs (the_time));
				  break;
				case FMT_NND:
				  /* Number with fraction. */
				  sprintf (buffer, "%.*f", digits,
					   abs (the_time));
				  break;
				case FMT_NSD:
				  /* Number with sign and fraction. */
				  sprintf (buffer, "%c%.*f",
					   (the_time <
					    0.0) ? *DES_LIT : *ASC_LIT,
					   digits, abs (the_time));
				  break;
				case FMT_HHH:
				  if (rc_precise)
				    {
				      /* Hour, minute and second+fraction. */
				      val2hours (HH2DAY (abs (the_time)),
						 &dots, &sign, &the_error);
				      digits =
					(int)
					ROUND ((the_error -
						(int) the_error) * 1000.0);
				      if (digits == 1000)
					digits--;
				      sprintf (buffer,
					       "%02d%c%02d%c%02d.%03d%s",
					       dots, *sep, sign, *sep,
					       (int) the_error, digits,
					       time_suffix);
				    }
				  else
				    {
				      /* Hour and minute. */
				      val2hours (HH2DAY (abs (the_time)),
						 &dots, &sign, NULL);
				      sprintf (buffer, "%02d%c%02d%s", dots,
					       *sep, sign, time_suffix);
				    }
				  break;
				case FMT_TTT:
				  the_time = abs (the_time);
				  if (rc_precise)
				    {
				      /* Value in hours, minutes and seconds+fraction. */
				      val2hours (HH2DAY (the_time), &dots,
						 &sign, &the_error);
				      digits =
					(int)
					ROUND ((the_error -
						(int) the_error) * 1000.0);
				      if (digits == 1000)
					digits--;
				    }
				  else
				    /* Value in hours and minutes. */
				    val2hours (HH2DAY (the_time), &dots,
					       &sign, NULL);
				  if ((aobj_mode == 65) && !dots
				      && (the_time >= HOURS_PER_HALF_DAY))
				    {
				      dots = HOURS_PER_DAY;
				      digits = sign = 0;
				      the_error = 0.0;
				    }
				  if (rc_precise)
				    sprintf (buffer,
					     "%02d%c%02d%c%02d.%03d%c", dots,
					     *sep, sign, *(sep + 1),
					     (int) the_error, digits,
					     *(sep + 2));
				  else
				    sprintf (buffer, "%02d%c%02d%c",
					     dots, *sep, sign, *(sep + 1));
				  break;
				case FMT_TTS:
				  if (rc_precise)
				    {
				      /* Value in hours, minutes and seconds+fraction with sign. */
				      val2hours (HH2DAY (the_time), &dots,
						 &sign, &the_error);
				      the_error = abs (the_error);
				      digits =
					(int)
					ROUND ((the_error -
						(int) the_error) * 1000.0);
				      if (digits == 1000)
					digits--;
				      sprintf (buffer,
					       "%c%02d%c%02d%c%02d.%03d%c",
					       (the_time <
						0.0) ? *DES_LIT : *ASC_LIT,
					       abs (dots), *sep, abs (sign),
					       *(sep + 1), (int) the_error,
					       digits, *(sep + 2));
				    }
				  else
				case FMT_TXS:
				    {
				      /* Value in hours and minutes with sign. */
				      val2hours (HH2DAY (the_time), &dots,
						 &sign, NULL);
				      sprintf (buffer, "%c%02d%c%02d%c",
					       (the_time <
						0.0) ? *DES_LIT : *ASC_LIT,
					       abs (dots), *sep, abs (sign),
					       *(sep + 1));
				    }
				  break;
				case FMT_DDD:
				  if (rc_precise)
				    {
				      /* Degree, minute and second+fraction. */
				      val2degrees (abs (the_time), &dots,
						   &sign, &the_error);
				      digits =
					(int)
					ROUND ((the_error -
						(int) the_error) * 1000.0);
				      if (digits == 1000)
					digits--;
				      sprintf (buffer,
					       "%03d%c%02d%c%02d.%03d%c",
					       dots, *sep, sign, *(sep + 1),
					       (int) the_error, digits,
					       *(sep + 2));
				    }
				  else
				    {
				      /* Degree and minute. */
				      val2degrees (abs (the_time), &dots,
						   &sign, NULL);
				      sprintf (buffer, "%03d%c%02d%c", dots,
					       *sep, sign, *(sep + 1));
				    }
				  break;
				case FMT_DDS:
				  if (rc_precise)
				    {
				      /* Degree, minute and second+fraction with sign. */
				      val2degrees (the_time, &dots, &sign,
						   &the_error);
				      the_error = abs (the_error);
				      digits =
					(int)
					ROUND ((the_error -
						(int) the_error) * 1000.0);
				      if (digits == 1000)
					digits--;
				      sprintf (buffer,
					       "%c%03d%c%02d%c%02d.%03d%c",
					       (the_time <
						0.0) ? *DES_LIT : *ASC_LIT,
					       abs (dots), *sep, abs (sign),
					       *(sep + 1), (int) the_error,
					       digits, *(sep + 2));
				    }
				  else
				    {
				      /* Degree and minute with sign. */
				      val2degrees (the_time, &dots, &sign,
						   NULL);
				      sprintf (buffer, "%c%03d%c%02d%c",
					       (the_time <
						0.0) ? *DES_LIT : *ASC_LIT,
					       abs (dots), *sep, abs (sign),
					       *(sep + 1));
				    }
				  break;
				default:
				  /*
				     This case MUST be an internal error!
				   */
				  abort ();
				}
			    }
			  else
			    {
			      sign = 0;
			      if (astronomical_object == AOBJ_NONE)
				{
				  if (is_error)
				    {
				      /*
				         Skip the misspelled argument.
				       */
				      if (*(the_text + j)
					  && !isspace (*(the_text + j)))
					{
					  while (*(the_text + j)
						 &&
						 !isspace (*(the_text + j)))
					    j++;
					  j--;
					}
				    }
				  if (!dist_mode)
				    i = ERR_NNN;
				  else
				    i = ERR_DDD;
				}
			      else
				{
				  i =
				    aobj_oformat[aobj_mode]
				    [astronomical_object].error_format;
				  if (aobj_oformat[aobj_mode]
				      [astronomical_object].data_format ==
				      FMT_TTS
				      ||
				      aobj_oformat[aobj_mode]
				      [astronomical_object].data_format ==
				      FMT_DDS)
				    sign = 1;
				}
			      /*
			         Force representation of numerical error text.
			       */
			      if (is_format_modifier_given)
				switch (i)
				  {
				  case ERR_TTT:
				  case ERR_DDD:
				    i = ERR_NNN;
				    sign = 0;
				    break;
				  default:
				    ;	/* Void, nothing to do! */
				  }
			      if (the_error <= HH2SS (SPECIAL_VALUE))
				the_error = SS2HH (the_error);
			      if (is_error)
				time_suffix = RC_INVALID_PARAM_TEXT;
			      else
				switch ((int) the_error)
				  {
				  case SPECIAL_VALUE * 3:
				    time_suffix = RC_A_ABOVE_VALUE_TEXT;
				    break;
				  case SPECIAL_VALUE * 2:
				    time_suffix = RC_A_BELOW_VALUE_TEXT;
				    break;
				  default:
				    time_suffix = RC_INVALID_VALUE_TEXT;
				  }
			      switch (i)
				{
				case ERR_NNN:
				  strcpy (buffer, time_suffix);
				  break;
				case ERR_HHH:
				  if (rc_precise)
				    sprintf (buffer, "%s%c%s%c%s.%s%c",
					     time_suffix, *sep, time_suffix,
					     *sep, time_suffix, time_suffix,
					     *time_suffix);
				  else
				    sprintf (buffer, "%s%c%s",
					     time_suffix, *sep, time_suffix);
				  if (is_format_modifier_given)
				    for (i = 0; i < state; i++)
				      strcat (buffer, time_suffix + 1);
				  break;
				case ERR_TTT:
				  if (sign)
				    *buffer = *time_suffix;
				  if (rc_precise
				      &&
				      (aobj_oformat[aobj_mode]
				       [astronomical_object].data_format !=
				       FMT_TXS))
				    sprintf (buffer + sign,
					     "%s%c%s%c%s.%s%c%c", time_suffix,
					     *sep, time_suffix, *(sep + 1),
					     time_suffix, time_suffix,
					     *time_suffix, *(sep + 2));
				  else
				    sprintf (buffer + sign, "%s%c%s%c",
					     time_suffix, *sep, time_suffix,
					     *(sep + 1));
				  break;
				case ERR_DDD:
				  if (sign)
				    *buffer = *time_suffix;
				  if (rc_precise)
				    sprintf (buffer + sign,
					     "%s%c%c%s%c%s.%s%c%c",
					     time_suffix, *time_suffix, *sep,
					     time_suffix, *(sep + 1),
					     time_suffix, time_suffix,
					     *time_suffix, *(sep + 2));
				  else
				    sprintf (buffer + sign, "%s%c%c%s%c",
					     time_suffix, *time_suffix, *sep,
					     time_suffix, *(sep + 1));
				  break;
				default:
				  /*
				     This case MUST be an internal error!
				   */
				  abort ();
				}
			    }
			  *s2 = '\0';
			  (void) use_format (&s2, 0, buffer, 0, FALSE,
					     is_cformat, is_lformat, is_sign,
					     is_lzero, is_suffix, is_fformat,
					     fstyle, fwidth, 0);
			  is_obsolete_whitespace = TRUE;
			}
		      else if (special_text_char == RC_ENV_VAR_CHAR)
			{
			  auto char *ptr_env = (char *) NULL;


			  /*
			     %shell_environment_variable special text found:
			     Replace it by the contents of the environment variable.
			   */
			  i = 0;
			  j++;
			  while (*(the_text + j)
				 && !isspace (*(the_text + j)))
			    {
			      if ((Uint) i >= maxlen_max)
				resize_all_strings (maxlen_max << 1, FALSE,
						    __FILE__,
						    (long) __LINE__);
			      s2[i++] = *(the_text + j++);
			    }
			  s2[i] = '\0';
			  j--;
# if !defined(AMIGA) || defined(__GNUC__)
			  ptr_env = getenv (s2);
# endif	/* !AMIGA || __GNUC__ */
			  /*
			     Insert the contents of environment variable into text.
			   */
			  *s2 = '\0';
			  if (ptr_env != (char *) NULL)
			    (void) use_format (&s2, 0, ptr_env, 0, FALSE,
					       is_cformat, is_lformat,
					       is_sign, is_lzero, is_suffix,
					       is_fformat, fstyle, fwidth, 0);
			  is_obsolete_whitespace = TRUE;
			}
		      else
			if (special_text_char == RC_HLS1S_CHAR
			    || special_text_char == RC_HLS1E_CHAR
			    || special_text_char == RC_HLS2S_CHAR
			    || special_text_char == RC_HLS2E_CHAR
			    || special_text_char == RC_HLS3S_CHAR
			    || special_text_char == RC_HLS3E_CHAR
			    || special_text_char == RC_HLS4S_CHAR
			    || special_text_char == RC_HLS4E_CHAR
			    || special_text_char == RC_HLS5S_CHAR
			    || special_text_char == RC_HLS5E_CHAR)
			{
			  /*
			     One of the %highlighting special texts found:
			     Replace it by the real hightlighting sequence.
			   */
			  *s2 = '\0';
			  if (highlight_flag)
			    {
			      hls_set = FALSE;
			      if ((special_text_char == RC_HLS1S_CHAR)
				  && !hls1_set)
				{
				  sprintf (s2, "%s", ehls1s.seq);
				  hls_set = TRUE;
				  hls1_set = TRUE;
				  hls2_set = FALSE;
				}
			      else
				if ((special_text_char == RC_HLS1E_CHAR)
				    && hls1_set)
				{
				  sprintf (s2, "%s", ehls1e.seq);
				  hls_set = TRUE;
				  hls1_set = FALSE;
				}
			      else
				if ((special_text_char == RC_HLS2S_CHAR)
				    && !hls2_set)
				{
				  sprintf (s2, "%s", ehls2s.seq);
				  hls_set = TRUE;
				  hls2_set = TRUE;
				  hls1_set = FALSE;
				}
			      else
				if ((special_text_char == RC_HLS2E_CHAR)
				    && hls2_set)
				{
				  sprintf (s2, "%s", ehls2e.seq);
				  hls_set = TRUE;
				  hls2_set = FALSE;
				}
			      else
				/*
				   Set highlighting sequence 1 only if the
				   fixed date is on today's date!
				 */
			      if ((special_text_char == RC_HLS3S_CHAR
				     || special_text_char == RC_HLS5S_CHAR)
				    && (year + incr_year - decr_year ==
					  act_year) && (m == act_month)
				    && (d == act_day) && !hls1_set)
				{
				  sprintf (s2, "%s", ehls1s.seq);
				  hls_set = TRUE;
				  hls1_set = TRUE;
				  hls2_set = FALSE;
				}
			      else
				if ((special_text_char == RC_HLS3E_CHAR
				     || special_text_char == RC_HLS5E_CHAR)
				    && hls1_set)
				{
				  sprintf (s2, "%s", ehls1e.seq);
				  hls_set = TRUE;
				  hls1_set = FALSE;
				}
			      else
				/*
				   Set highlighting sequence 2 only if the
				   fixed date is on a legal holiday date!
				 */
			      if ((special_text_char == RC_HLS4S_CHAR
				     || special_text_char == RC_HLS5S_CHAR)
				    && hd_ldays[((m - 1) * MONTH_LAST) +
						  (d - 1)] && !hls2_set)
				{
				  sprintf (s2, "%s", ehls2s.seq);
				  hls_set = TRUE;
				  hls2_set = TRUE;
				  hls1_set = FALSE;
				}
			      else
				if ((special_text_char == RC_HLS4E_CHAR
				     || special_text_char == RC_HLS5E_CHAR)
				    && hls2_set)
				{
				  sprintf (s2, "%s", ehls2e.seq);
				  hls_set = TRUE;
				  hls2_set = FALSE;
				}
			    }
			}
		      else
			{
			  auto Slint diff;


			  dd = act_day;
			  mm = act_month;
			  yy = act_year;
			  (void) get_actual_date ();
			  y = year + incr_year - decr_year;
			  /*
			     Now compute the differences:
			     date1 == actual (base)date (act_day, act_month, act_year)
			     date2 == reported date (d, m, y)
			   */
			  if (special_text_char == RC_JDAYS_CHAR)
			    {
			      auto char op = '\0';


			      /*
			         A %julian_day[[+|-]N] special text found
			         (we always suppress the day fraction part of 0.5).
			       */
			      diff = d_between (1, 1, 1, d, m, y);
			      j++;
			      if (*(the_text + j) == *ASC_LIT
				  || *(the_text + j) == *DES_LIT)
				op = *(the_text + j++);
			      i = 0;
			      while (isdigit (*(the_text + j)))
				{
				  if ((Uint) i >= maxlen_max)
				    resize_all_strings (maxlen_max << 1,
							FALSE, __FILE__,
							(long) __LINE__);
				  s2[i++] = *(the_text + j++);
				}
			      s2[i] = '\0';
			      j--;
			      num = atol (s2);
			      *s2 = '\0';
			      diff += MIN_BCE_TO_1_CE;
			      if (op == *ASC_LIT)
				diff += num;
			      else
				diff -= num;
			    }
			  else
			    {
			      if (special_text_char == RC_DAY_DIFF_CHAR)
				/*
				   %day_difference special text found.
				 */
				diff =
				  d_between (act_day, act_month, act_year, d,
					     m, y);
			      else if (special_text_char == RC_WEEK_DIFF_CHAR)
				/*
				   %week_difference special text found.
				 */
				diff =
				  w_between (act_day, act_month, act_year, d,
					     m, y);
			      else if (special_text_char ==
				       RC_MONTH_DIFF_CHAR)
				/*
				   %month_difference special text found.
				 */
				diff = m_between (act_month, act_year, m, y);
			      else
				/*
				   A simple %year_difference special text found.
				 */
				diff = (Slint) y - act_year;
			      if (j_buf)
				/*
				   Check whether a '-' character is put before
				   the `%?' special text, e.g. `x -%d x'.  If
				   so, switch the sign of the computed value.
				   In case the '-' character preceding the
				   special is quoted, like "\-", remove the
				   quote character '-' and do not switch the
				   sign of the computed value.
				 */
				if (*(the_text + j_buf - 1) == *DES_LIT)
				  {
				    if ((j_buf > 1)
					&& (*(the_text + j_buf - 2) ==
					    QUOTE_CHAR))
				      {
					s1[k - 2] = s1[k - 1];
					k--;
					s6[kk - 2] = s6[kk - 1];
					kk--;
				      }
				    else
				      {
					k--;
					kk--;
					diff = -diff;
				      }
				  }
			    }
			  (void) use_format (&s2, 0, "", diff, TRUE,
					     is_cformat, is_lformat, is_sign,
					     is_lzero, is_suffix, is_fformat,
					     fstyle, fwidth, 0);
			  act_day = dd;
			  act_month = mm;
			  act_year = yy;
			}
		      /*
		         And copy the expanded special text into the target string.
		       */
		      i = 0;
		      while (s2[i])
			{
			  if ((Uint) k >= maxlen_max - (Uint) len_fn)
			    resize_all_strings (maxlen_max << 1, FALSE,
						__FILE__, (long) __LINE__);
			  s1[k++] = s2[i];
			  /*
			     Don't add highlighting sequences in that text, in which we search for PATTERN!
			   */
			  if (!remove_hls_in_regex || !hls_set)
			    s6[kk++] = s2[i];
			  i++;
			}
		      j++;
		    }
		  else
		    /*
		       Check for %? special_texts which disable a fixed date.
		     */
		  if (!got_command
			&& (special_text_char == RC_EX_LHDY_CHAR
			      || special_text_char == RC_EX_NLHDY_CHAR
			      || special_text_char == RC_EX_AHDY_CHAR
			      || special_text_char == RC_EX_NAHDY_CHAR
			      || special_text_char == RC_EX_MON_CHAR
			      || special_text_char == RC_EX_NMON_CHAR
			      || special_text_char == RC_EX_TUE_CHAR
			      || special_text_char == RC_EX_NTUE_CHAR
			      || special_text_char == RC_EX_WED_CHAR
			      || special_text_char == RC_EX_NWED_CHAR
			      || special_text_char == RC_EX_THU_CHAR
			      || special_text_char == RC_EX_NTHU_CHAR
			      || special_text_char == RC_EX_FRI_CHAR
			      || special_text_char == RC_EX_NFRI_CHAR
			      || special_text_char == RC_EX_SAT_CHAR
			      || special_text_char == RC_EX_NSAT_CHAR
			      || special_text_char == RC_EX_SUN_CHAR
			      || special_text_char == RC_EX_NSUN_CHAR
			      || special_text_char == RC_EX_MON_2_THU_CHAR
			      || special_text_char == RC_EX_NMON_2_THU_CHAR
			      || special_text_char == RC_EX_MON_2_FRI_CHAR
			      || special_text_char == RC_EX_NMON_2_FRI_CHAR))
		    {
		      if (strchr (date_text, special_text_char) ==
			  (char *) NULL)
			{
			  /*
			     Insert the `special_text_char' into `date_text'
			     only in cases it is not already stored in there!
			   */
			  *ptr_date_text++ = special_text_char;
			  *ptr_date_text = '\0';
			}
		      j++;
		    }
		  else
		    {
		      /*
		         Check for %shell_escape[TEXT-TILL-EOL] special text.
		       */
		      if (rc_execute_command
			  && !got_command
			  && (special_text_char == RC_SHELL_ESC_CHAR))
			{
			  /*
			     Buffer the starting position of the %shell_escape[TEXT-TILL-EOL]
			     special text and process it as usual, so other special texts
			     possibly contained are expanded!
			   */
			  kpos = k;
			  kkpos = kk;
			  j_diff = j - j_buf - 1;
			  got_command = TRUE;
			}
		      for (; j_buf < j; j_buf++, k++, kk++)
			s1[k] = s6[kk] = *(the_text + j_buf);
		    }
		}
	    }
	  else
	    ok = TRUE;
	}
      if (line_number != SPECIAL_VALUE)
	{
	  /*
	     Check whether a period to exclude is marked in the maps and
	     if so, avoid displaying the fixed date entry.
	   */
	  if (ie_date_maps_set
	      && (*inclusive_date_map || *exclusive_date_map))
	    {
	      i = day_of_year (d, m, year + incr_year - decr_year);
	      if (*inclusive_date_map && *exclusive_date_map)
		{
		  if (!inclusive_date_map[i] || !exclusive_date_map[i])
		    print_line = FALSE;
		}
	      else if (*inclusive_date_map)
		{
		  if (!inclusive_date_map[i])
		    print_line = FALSE;
		}
	      else if (!exclusive_date_map[i])
		print_line = FALSE;
	    }
	  /*
	     Check whether a weekday to exclude is marked in the maps and
	     if so, avoid displaying the fixed date entry.
	   */
	  if (print_line && *date_text)
	    print_line =
	      rc_valid_day (date_text, d, m, year + incr_year - decr_year);
	}
      if (print_line)
	{
	  s1[k] = s6[kk] = '\0';
	  if (got_command)
	    {
	      /*
	         Is it necessary to print any leading text before executing
	         the %!shell_command[TEXT-TILL-EOL]?
	       */
	      if (!kpos)
		print_line = FALSE;
	      else
		{
		  /*
		     Allow regular expression search only in the text which
		     leads a later executed %!shell_command[TEXT-TILL-EOL].
		   */
		  kk = kkpos;
		  s1[kpos] = s6[kk] = '\0';
		}
	    }
	  /*
	     If `--filter-text=PATTERN' is given and REGEX doesn't match
	     the text of the fixed date, suppress this fixed date!
	   */
	  if (print_line
	      && (rc_filter_text != (char *) NULL)
	      && (line_number != SPECIAL_VALUE))
	    {
# if HAVE_GNU_RE_COMPILE_PATTERN
	      print_line =
		(Bool) (re_search
			(&regpattern, s6, kk, 0, kk,
			 (struct re_registers *) NULL) >= 0);
# else /* !HAVE_GNU_RE_COMPILE_PATTERN */
#  if HAVE_POSIX_REGCOMP
	      static regmatch_t rm_dummy;
#  endif


	      if (rc_ignore_case_flag)
		{
		  /*
		     Set all letters of the fixed date text to lower-case.
		   */
		  ptr_char = s6;
		  for (; *ptr_char; ptr_char++)
		    *ptr_char = (char) tolower (*ptr_char);
		}
#  if HAVE_POSIX_REGCOMP
	      print_line =
		(Bool) ! regexec (&regpattern, s6, 1, &rm_dummy, 0);
#  endif
#  if HAVE_RE_COMP
	      print_line = (Bool) (re_exec (s6) == 1);
#  endif
# endif	/* !HAVE_GNU_RE_COMPILE_PATTERN */
	      if (rc_revert_match_flag)
		print_line = !print_line;
	    }
	  /*
	     Is it necessary to execute the %!shell_command[TEXT-TILL-EOL] now?
	   */
	  if (got_command
	      && ((print_line
		   && (kpos
		       || rc_filter_text != (char *) NULL))
		  || (!print_line
		      && (!kpos || rc_filter_text == (char *) NULL))))
	    {
	      /*
	         Execute the command.
	       */
	      i = my_system (s1 + kpos + j_diff + 2);
	      if (warning_level >= 0)
		{
		  if (i == -1)
		    {
		      /*
		         Error, `system()' function failed.
		       */
		      sprintf (s2,
			       _
			       ("Cannot execute command in file `%s'\nLine: %ld %s"),
			       filename, line_number,
			       the_text + kpos + j_diff + 2);
		      print_text (stderr, s2);
		      if (warning_level >= WARN_LVL_MAX)
			{
			  k = (int) strlen (s1 + kpos + j_diff + 2);
			  if ((Uint) k >= maxlen_max - 9)
			    resize_all_strings (k + 9, FALSE, __FILE__,
						(long) __LINE__);
			  sprintf (s2, "system(%s)=", s1 + kpos + j_diff + 2);
			  my_error (ERR_INTERNAL_C_FUNC_FAILURE, __FILE__,
				    ((long) __LINE__) - 22L, s2, i);
			}
		    }
		  else
		    {
		      /*
		         Report the exit code of command executed by the `system()' function.
		       */
		      sprintf (s2,
			       _
			       ("Command executed (exit code=%d) in file `%s'\nLine %ld: %s"),
			       i, filename, line_number,
			       s1 + kpos + j_diff + 2);
		      print_text (stderr, s2);
		      /*
		         The command executed by the `system()' function returned
		         a value not equal zero so we terminate all further
		         processing now with ERR_EXTERNAL_CMD_FAILURE exit status.
		       */
		      if (i && (warning_level >= WARN_LVL_MAX))
			my_exit (ERR_EXTERNAL_CMD_FAILURE);
		    }
		}
	      shell_escape_done = TRUE;
	    }
	  if (print_line && highlight_flag && (hls1_set || hls2_set))
	    {
	      /*
	         Hmm, seems the user forgot to give the right %highlighting
	         special text for disabling a highlighting sequence already
	         enabled, let's do this for him/her/it/PER/or...  hehehe!
	       */
	      if (hls1_set)
		{
		  if (ehls1e.len == 1)
		    {
		      if ((Uint) k >= maxlen_max - (Uint) len_fn - 1)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  else
		    {
		      while ((Uint) k >=
			     maxlen_max - (Uint) len_fn - (ehls1e.len + 1))
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  strcat (s1, ehls1e.seq);
		}
	      else
		{
		  if (ehls2e.len == 1)
		    {
		      if ((Uint) k >= maxlen_max - (Uint) len_fn - 1)
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  else
		    {
		      while ((Uint) k >=
			     maxlen_max - (Uint) len_fn - (ehls2e.len + 1))
			resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
					    (long) __LINE__);
		    }
		  strcat (s1, ehls2e.seq);
		}
	    }
	}
      if (print_line)
	{
	  if (rc_enable_fn_flag && (line_number != SPECIAL_VALUE))
	    {
	      sprintf (s6, "%0*d%02d%02d %c%s#%05ld%c",
		       len_year_max, year + incr_year - decr_year, m, d,
		       PSEUDO_QUOTE, filename, line_number, PSEUDO_QUOTE);
	      if (rc_suppr_text_part_flag)
		j = strlen (s6);
	      if (*s1)
		{
		  strcat (s6, " ");
		  strcat (s6, s1);
		}
	    }
	  else
	    {
	      sprintf (s6, "%0*d%02d%02d %s",
		       len_year_max, year + incr_year - decr_year, m, d, s1);
	      if (rc_suppr_text_part_flag)
		j = len_year_max + 4;
	    }
	  /*
	     Now compute whether a filler text for a week entry is needed:
	     Week 51/0   == 7 chars text "|51/0| "  -> no filler text.
	     Week 52/0   == 7 chars text "|52/1| "  -> no filler text.
	     Week 53/0   == 7 chars text "|53/0| "  -> no filler text.
	     Week 53/1   == 7 chars text "|53/1| "  -> no filler text.
	     Week 1...52 == 5 chars text "|nn| "    -> 2 chars filler text.
	   */
	  if (rc_week_number_flag && (line_number != SPECIAL_VALUE))
	    if (week_number
		(d, m, year + incr_year - decr_year, iso_week_number,
		 start_day) <= 0)
	      len_fil_wt = 2;
	  /*
	     Now place the fixed date into the table:
	     Only if the previous fixed date is equal the actual fixed date,
	     avoid insertation of actual fixed date!
	   */
	  i = 1;
	  if (*rc_elems && (line_number != SPECIAL_VALUE))
	    i = strcmp (s6, rc_elems_table[*rc_elems - 1]);
	  /*
	     Store the constructed "raw" line in `rc_elems_table[]'.
	   */
	  if (i)
	    {
	      if ((Uint) * rc_elems >= rc_elems_max)
		{
		  /*
		     Resize the `rc_elems_table[]' table.
		   */
		  rc_elems_max <<= 1;
		  if (rc_elems_max * sizeof (char *) > testval)
		    rc_elems_max--;
		  rc_elems_table =
		    (char **) my_realloc ((VOID_PTR) rc_elems_table,
					  rc_elems_max * sizeof (char *),
					  ERR_NO_MEMORY_AVAILABLE, __FILE__,
					  ((long) __LINE__) - 3L,
					  "rc_elems_table[rc_elems_max]",
					  rc_elems_max);
		}
	      if (rc_suppr_text_part_flag && (line_number != SPECIAL_VALUE))
		{
		  rc_elems_table[*rc_elems] = (char *) my_malloc (j + 1,
								  ERR_NO_MEMORY_AVAILABLE,
								  __FILE__,
								  ((long)
								   __LINE__) -
								  2L,
								  "rc_elems_table[rc_elems]",
								  *rc_elems);
		  s6[j] = '\0';
		}
	      else
		rc_elems_table[*rc_elems] =
		  (char *) my_malloc (strlen (s6) + 1,
				      ERR_NO_MEMORY_AVAILABLE, __FILE__,
				      ((long) __LINE__) - 2L,
				      "rc_elems_table[rc_elems]", *rc_elems);
	      strcpy (rc_elems_table[(*rc_elems)++], s6);
	    }
	}
      /*
         Do we have to construct a 1x2 production of the line?
       */
      if (print_twice > 1)
	{
	  if (is_2easter)
	    {
	      /*
	         Precalculate the next date relative to Easter Sunday's date.
	       */
	      incr_year = 1;
	      precomp_date (hn, hwd, &d, &m, year + incr_year, EAster);
	    }
	  else if (is_2dvar)
	    {
	      /*
	         Precalculate the next date relative to date variable's date and
	         use original date of DVAR, i.e. use buffer of day and month.
	       */
	      incr_year = 1;
	      if (islower (hc))
		(void) precomp_date (hn, hwd, &d_buf, &m_buf,
				     year + incr_year,
				     (hc == RC_TODAY_CHAR) ? TOday : DVar);
	      else
		{
		  y = year + incr_year;
		  (void) precomp_nth_wd (hn, hwd, &hn, &d_buf, &m_buf, &y,
					 (hc == 'D') ? DAy : WEek);
		}
	      d = d_buf;
	      m = m_buf;
	    }
	  else
	    /*
	       Compute tomorrow's date.
	     */
	    (void) next_date (&d, &m, &year);
	  /*
	     If the "new" precomputed date has left the year bounds,
	     we have to exit the loop!
	   */
	  if (year + incr_year > YEAR_MAX)
	    print_twice--;
	  else
	    /*
	       Force the second processing of the line!
	     */
	    print_line = TRUE;
	}
    }
  while (--print_twice);
  year = tmp_year;
}
#endif /* USE_RC */
