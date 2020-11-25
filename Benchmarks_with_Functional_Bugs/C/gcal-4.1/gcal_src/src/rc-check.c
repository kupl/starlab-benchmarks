/*!
*  \file rc-check.c
*  \brief Checks if a line (of a resource file)
*               must be inserted into `rc_elems_table[]'.
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
# include "common.h"
# include "rc-defs.h"
# include "globals.h"
# include "rc-insert.h"
# include "rc-utils.h"
# include "utils.h"
# include "rc-check.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `rc-check.c'.
*/
static Bool
  date_matches_period __P_ ((int *print_twice,
			     int day, int ed, const int wd));
static int get_number __P_ ((char **string));
__END_DECLARATIONS
/*
*  static variables definitions.
*/
/*! Textual weekday name is found and must be respected. */
static Bool is_weekday_mode = FALSE;

/*! No explicit month command is given in the command line. */
static Bool changed_month = FALSE;



/*
*  Function implementations.
*/
void
rc_check (line_buffer, filename, line_number, line_length, rc_elems, day, ed,
	  wd)
     char *line_buffer;
     const char *filename;
     const long line_number;
     const int line_length;
     int *rc_elems;
     const int day;
     const int ed;
     const int wd;
/*!
   Checks whether a single line of a resource file resp.,
     eternal holiday must be displayed.
*/
{
  register int i;
  register int j;
  register int buf_d = 0;
  register int buf_m;
  register int buf_y;
  register int dvar_base_day = 0;
  register int dvar_base_month = 0;
  register int repeat = 0;
  register int appears = 0;
  register int productions = 2;
  register int tmp_month = month;
  auto int print_twice;
  auto int len_date;
  auto int n;
  auto int tmp_d;
  auto int tmp_m;
  auto int tmp_y;
  auto int tmp_n;
  auto int tmp_hn;
  auto int tmp_hwd = 0;
  auto Bool is_valid_date;
  auto Bool is_day_given;
  auto Bool is_month_given;
  auto Bool is_coded_month;
  auto Bool is_first = TRUE;
  auto Bool is_range = FALSE;
  auto Bool date_unset = FALSE;
  auto Bool must_ignore_feb_29 = FALSE;


  /*
     Decode a delivered line:
     This means all necessary information is stored in the according
     variables, which are set by decoding the "date"-part of the line.
   */
  lineptrs =
    rc_get_date (line_buffer, lineptrs, TRUE, &is_weekday_mode, &d, &m, &y,
		 &n, &len_date, &hc, &hn, &hwd, filename, line_number,
		 line_buffer, TRUE);
  if (len_date < 1)
    /*
       Error, invalid date field given.
     */
    my_error (ERR_INVALID_DATE_FIELD, filename, line_number, line_buffer, 0);
  if ((len_date < line_length) && !*lineptrs->text_part)
    /*
       Error, missing whitespace character between "date"-part and "text"-part.
     */
    my_error (ERR_NO_SEPARATOR_CHAR, filename, line_number, line_buffer, 0);
  if (!month)
    {
      month = act_month;
      changed_month = TRUE;
    }
  else
    changed_month = FALSE;
  buf_m = m;
  buf_y = y;
  /*
     We work with a buffer of the returned (allocated and filled)
     char pointers of the `lineptrs' struct.
   */
  lptrs->day_part = lineptrs->day_part;
  lptrs->repeat_part = lineptrs->repeat_part;
  lptrs->appears_part = lineptrs->appears_part;
  do
    {
      /*
         Look if a list/range of days/weekday names is given,
         if so then let's create 1xN productions of such a line.
       */
      if (!is_range && (lptrs->day_part != (char *) NULL))
	{
	  auto char *ptr2_char;
	  auto char ch;


	  repeat = appears = hwd = tmp_hn = hn = tmp_n = n = i = 0;
	  is_range = is_coded_month = is_month_given = is_day_given =
	    is_weekday_mode = FALSE;
	  /*
	     Store the base date if a date variable is given.
	   */
	  if (is_first && islower (hc))
	    {
	      if (y == SPECIAL_VALUE)
		/*
		   Error, date variable is undefined.
		 */
		break;
	      dvar_base_day = d;
	      dvar_base_month = m;
	    }
	  if (lineptrs->day_list)
	    /*
	       Manage a list of days/weekday names.
	     */
	    {
	      /*
	         If a list of days/weekday names is given, we have to set the
	         number of productions to any value greater 1, because we
	         terminate the production creating loop by a `break' statement
	         after all elements of the list are evaluated.
	       */
	      productions = 2;
	      /*
	         Copy an element of the list into `s6'.
	       */
	      while (*lptrs->day_part
		     && !isspace (*lptrs->day_part)
		     && (*lptrs->day_part != RC_DLIST_CHAR
			 && (*lptrs->day_part != RC_REPEAT_CHAR)
			 && (*lptrs->day_part != RC_APPEARS_CHAR)))
		s6[i++] = *(lptrs->day_part++);
	      s6[i] = '\0';
	      if (!i && (!is_first || !islower (hc)))
		/*
		   Error, an empty list element given (e.g.:  DD,,DD or WW[W],,WW[W]N).
		   The only place where an empty list element may occur is the
		   trailing character after a @e|t|DVAR text, e.g.:  @e,+10,-3fr ...,
		   which means -> first evaluate  @e  and then  @e+10  and then  @e-3fr.
		 */
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	      is_first = FALSE;
	      ptr2_char = s6;
	      if (hc && *ptr2_char)
		{
		  hn = atoi (s6);
		  if (islower (hc))
		    {
		      if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
			ptr2_char++;
		      if (*ptr2_char == *ASC_LIT
			  || *ptr2_char == *DES_LIT || isalpha (*ptr2_char))
			/*
			   Error, simple weekday name or invalid sign given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  else if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
		    /*
		       Error, invalid sign given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  /*
		     Now eat all digits.
		   */
		  while (isdigit (*ptr2_char))
		    ptr2_char++;
		}
	      if (!hc || *ptr2_char)
		{
		  if (!hc && i)
		    {
		      if (i > 2)
			{
			  if (!isdigit (*ptr2_char))
			    {
			      ch = s6[TXTLEN_MONTH];
			      s6[TXTLEN_MONTH] = '\0';
			      m = compare_d_m_name (ptr2_char, MOnth);
			      s6[TXTLEN_MONTH] = ch;
			      if (!m)
				{
				  if (compare_d_m_name (ptr2_char, DAy))
				    goto LABEL_list_day_name_given;
				  /*
				     Error, invalid textual month name given.
				   */
				  my_error (ERR_INVALID_DATE_FIELD, filename,
					    line_number, line_buffer, 0);
				}
			      ptr2_char++;
			    }
			  else
			    {
			      ch = s6[2];
			      s6[2] = '\0';
			      m = atoi (ptr2_char);
			      if (m == 99)
				m = MONTH_MAX;
			      if (m > MONTH_MAX)
				/*
				   Error, invalid month number (mm) given.
				 */
				my_error (ERR_INVALID_DATE_FIELD, filename,
					  line_number, line_buffer, 0);
			      s6[2] = ch;
			    }
			  ptr2_char += 2;
			  if (m)
			    is_month_given = TRUE;
			  else
			    is_coded_month = TRUE;
			}
		    LABEL_list_day_name_given:
		      d = atoi (ptr2_char);
		      if (d)
			is_day_given = TRUE;
		      else if (isdigit (*ptr2_char))
			/*
			   Error, invalid day number (DD) given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  if (!isdigit (*ptr2_char))
		    {
		      d = compare_d_m_name (ptr2_char, DAy);
		      if (!d)
			/*
			   Error, invalid textual day name (WW[W]) given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		}
	      if (!is_month_given && !is_coded_month)
		m = buf_m;
	      y = buf_y;
	      if (!is_day_given)
		{
		  if (!hc)
		    {
		      if (isdigit (s6[i - 1]))
			{
			  if (isdigit (s6[i - 2]))
			    /*
			       Error, "N'th weekday of month" field contains more than one digit.
			     */
			    my_error (ERR_INVALID_DATE_FIELD, filename,
				      line_number, line_buffer, 0);
			  n = CHR2DIG (s6[i - 1]);
			  /*
			     Error, invalid "N'th weekday of month" number given.
			   */
			  if ((n > 5) && (n < 9))
			    my_error (ERR_INVALID_NWD_FIELD, filename,
				      line_number, line_buffer, n);
			}
		      if (!n)
			is_weekday_mode = TRUE;
		    }
		  else
		    {
		      if (*ptr2_char)
			hwd = d;
		      if (!islower (hc))
			d = 0;
		    }
		}
	      if (islower (hc))
		{
		  d = dvar_base_day;
		  m = dvar_base_month;
		}
	      if (hc && y)
		{
		  if (islower (hc))
		    {
		      if (!precomp_date (hn, hwd, &d, &m, y,
					 (hc == RC_EASTER_CHAR) ? EAster :
					 ((hc ==
					   RC_TODAY_CHAR) ? TOday : DVar)))
			/*
			   If the date is not computable, skip this list entry.
			 */
			y = SPECIAL_VALUE;
		    }
		  else
		    if (!precomp_nth_wd (hn, hwd, &hn, &d, &m, &y,
					 (hc == 'D') ? DAy : WEek))
		    /*
		       If the date is not computable, skip this list entry.
		     */
		    y = SPECIAL_VALUE;
		}
	      if (!y && !is_coded_month)
		manage_leap_day (&d, &m, year, line_buffer, filename,
				 line_number);
	      /*
	         Check if a "repeat for N days since..." field or a
	         "for each N'th day since..." field is given in this list element.
	       */
	      if (*lptrs->day_part == RC_REPEAT_CHAR
		  || *lptrs->day_part == RC_APPEARS_CHAR)
		{
		  if (is_weekday_mode)
		    /*
		       Error, it's invalid to specify a simple weekday name WWW
		       with a "repeat" or "appears" coding (e.g.: WWW:10,WWW.3,WWW:10.3 ...).
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  else
		    {
		      register int num_repeat = 0;
		      register int num_appears = 0;


		      do
			{
			  if (*lptrs->day_part == RC_REPEAT_CHAR)
			    {
			      repeat = get_number (&lptrs->day_part);
			      num_repeat++;
			    }
			  if (*lptrs->day_part == RC_APPEARS_CHAR)
			    {
			      appears = get_number (&lptrs->day_part);
			      num_appears++;
			    }
			}
		      while (--productions);
		      if (num_repeat > 1
			  || num_appears > 1 || (!repeat && !appears))
			/*
			   Error, either "repeat" or "appears" coding given twice
			   or invalid by other reasons (a number > 999 given).
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		      if (appears)
			appears--;
		      /*
		         Well, we have to use an unoptimized number of productions now.
		       */
		      productions = repeat;
		    }
		}
	      /*
	         Skip the ONE separating character of the list of days.
	       */
	      if (*lptrs->day_part == RC_DLIST_CHAR)
		lptrs->day_part++;
	      else if (!*lptrs->day_part || isspace (*lptrs->day_part))
		{
		  free (lineptrs->day_part);
		  lptrs->day_part = lineptrs->day_part = (char *) NULL;
		}
	      else
		/*
		   Error, invalid "repeat" or "appears" coding given
		   (one of the fields contains invalid characters).
		 */
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	    }
	  else
	    /*
	       Manage a range of days/weekday names.
	     */
	    {
	      if (lptrs->repeat_part != (char *) NULL)
		/*
		   Error, a "repeat" coding makes no sense in ranges of days.
		 */
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	      /*
	         Copy the starting element of the range into `s6'.
	       */
	      while (*lptrs->day_part != RC_DRANGE_CHAR)
		s6[i++] = *(lptrs->day_part++);
	      s6[i] = '\0';
	      if (strchr (s6, RC_APPEARS_CHAR) != (char *) NULL)
		/*
		   Error, a "appears" coding may only be given last to a range of days.
		 */
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	      ptr2_char = s6;
	      /*
	         Skip THE separating character of the range of days.
	       */
	      lptrs->day_part++;
	      if (hc && *ptr2_char)
		{
		  hn = atoi (s6);
		  if (islower (hc))
		    {
		      if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
			ptr2_char++;
		      if (*ptr2_char == *ASC_LIT
			  || *ptr2_char == *DES_LIT || isalpha (*ptr2_char))
			/*
			   Error, simple weekday name WWW or invalid sign given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  else if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
		    /*
		       Error, invalid sign given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  /*
		     Now eat all digits.
		   */
		  while (isdigit (*ptr2_char))
		    ptr2_char++;
		}
	      if (!hc || *ptr2_char)
		{
		  if (!hc)
		    {
		      d = atoi (ptr2_char);
		      if (d)
			is_day_given = TRUE;
		      else if (isdigit (*ptr2_char))
			/*
			   Error, invalid day number (==0) given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  if (!isdigit (*ptr2_char))
		    {
		      buf_d = d = compare_d_m_name (ptr2_char, DAy);
		      if (!d)
			/*
			   Error, invalid weekday name (WW[W]) given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  if (!is_day_given)
		    {
		      if (!hc && isdigit (s6[i - 1]))
			{
			  if (isdigit (s6[i - 2]))
			    /*
			       Error, "N'th weekday of month" field contains more than one digit.
			     */
			    my_error (ERR_INVALID_DATE_FIELD, filename,
				      line_number, line_buffer, 0);
			  n = CHR2DIG (s6[i - 1]);
			  /*
			     Error, invalid "N'th weekday of month" number given.
			   */
			  if ((n > 5) && (n < 9))
			    my_error (ERR_INVALID_NWD_FIELD, filename,
				      line_number, line_buffer, n);
			  if (n)
			    {
			      is_range = TRUE;
			      nth_weekday_of_month (&d, &m, &y, &n,
						    &is_weekday_mode);
			      if (!m || y == SPECIAL_VALUE)
				/*
				   If the date is not computable, exit the loop.
				 */
				break;
			    }
			  else
			    is_weekday_mode = TRUE;
			}
		      else
			{
			  if (!hc)
			    is_weekday_mode = TRUE;
			  else
			    {
			      is_range = TRUE;
			      hwd = d;
			    }
			}
		    }
		  else
		    {
		      if (!m
			  && (rc_year_flag
			      || rc_period_list
			      || is_3month_mode
			      || is_3month_mode2 || fiscal_month > MONTH_MIN))
			/*
			   If no month `m' is given and we are in one of these modes,
			   exit the loop.
			 */
			break;
		      is_range = TRUE;
		      if (!y)
			y = year;
		      if (!m)
			m = month;
		    }
		}
	      else
		{
		  is_range = TRUE;
		  hwd = 0;
		}
	      if (hc)
		{
		  if (!y)
		    y = year;
		  if (islower (hc))
		    {
		      d = dvar_base_day;
		      m = dvar_base_month;
		      if (!precomp_date (hn, hwd, &d, &m, y,
					 (hc == RC_EASTER_CHAR) ? EAster :
					 ((hc ==
					   RC_TODAY_CHAR) ? TOday : DVar)))
			date_unset = TRUE;
		    }
		  else
		    if (!precomp_nth_wd (hn, hwd, &hn, &d, &m, &y,
					 (hc == 'D') ? DAy : WEek))
		    date_unset = TRUE;
		  if (date_unset)
		    {
		      /*
		         If the date is not computable, set `d' and `m'
		         to 1-JAN if `hn' is less zero, otherwise to 31-DEC
		         if `hn' is greater than +1.
		       */
		      if (!hn || hwd)
			/*
			   If the date isn't computable at all, exit the loop.
			 */
			break;
		      date_unset = FALSE;
		      if (hn < 0)
			{
			  m = MONTH_MIN;
			  d = DAY_MIN;
			}
		      else
			{
			  m = MONTH_MAX;
			  d = dvec[m - 1];
			}
		    }
		}
	      /*
	         Copy the final element of the range into `s6'.
	       */
	      is_day_given = FALSE;
	      i = 0;
	      while (*lptrs->day_part
		     && !isspace (*lptrs->day_part)
		     && (*lptrs->day_part != RC_APPEARS_CHAR))
		s6[i++] = *(lptrs->day_part++);
	      s6[i] = '\0';
	      ptr2_char = s6;
	      free (lineptrs->day_part);
	      lptrs->day_part = lineptrs->day_part = (char *) NULL;
	      if (hc && *ptr2_char)
		{
		  tmp_hn = atoi (s6);
		  if (islower (hc))
		    {
		      if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
			ptr2_char++;
		      if (*ptr2_char == *ASC_LIT
			  || *ptr2_char == *DES_LIT || isalpha (*ptr2_char))
			/*
			   Error, simple weekday name or invalid sign given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  else if (*ptr2_char == *ASC_LIT || *ptr2_char == *DES_LIT)
		    /*
		       Error, invalid sign given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  /*
		     Now eat all digits.
		   */
		  while (isdigit (*ptr2_char))
		    ptr2_char++;
		  /*
		     Now eat an "appears" field (if given).
		   */
		  if (*ptr2_char == RC_APPEARS_CHAR)
		    {
		      ptr2_char++;
		      while (isdigit (*ptr2_char))
			ptr2_char++;
		    }
		}
	      if (!hc || *ptr2_char)
		{
		  if (!hc && i)
		    {
		      if (i > 2)
			{
			  if (!isdigit (*ptr2_char))
			    {
			      ch = s6[TXTLEN_MONTH];
			      s6[TXTLEN_MONTH] = '\0';
			      tmp_m = compare_d_m_name (ptr2_char, MOnth);
			      s6[TXTLEN_MONTH] = ch;
			      if (!tmp_m)
				{
				  if (compare_d_m_name (ptr2_char, DAy))
				    goto LABEL_range_day_name_given;
				  /*
				     Error, invalid textual month name given.
				   */
				  my_error (ERR_INVALID_DATE_FIELD, filename,
					    line_number, line_buffer, 0);
				}
			      s6[3] = ch;
			      ptr2_char++;
			    }
			  else
			    {
			      ch = s6[2];
			      s6[2] = '\0';
			      tmp_m = atoi (ptr2_char);
			      if (tmp_m == 99)
				tmp_m = MONTH_MAX;
			      if (tmp_m > MONTH_MAX)
				/*
				   Error, invalid month number (MM) given.
				 */
				my_error (ERR_INVALID_DATE_FIELD, filename,
					  line_number, line_buffer, 0);
			      s6[2] = ch;
			    }
			  ptr2_char += 2;
			  if (tmp_m)
			    is_month_given = TRUE;
			  else
			    is_coded_month = TRUE;
			}
		    LABEL_range_day_name_given:
		      tmp_d = atoi (ptr2_char);
		      if (tmp_d)
			is_day_given = TRUE;
		    }
		  if (!isdigit (*ptr2_char))
		    {
		      buf_d = tmp_d = compare_d_m_name (ptr2_char, DAy);
		      if (!tmp_d)
			/*
			   Error, invalid textual day name (WW[W]) given.
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  if (!is_day_given)
		    {
		      if (isdigit (s6[i - 1]))
			{
			  if (isdigit (s6[i - 2]))
			    /*
			       Error, "N'th weekday of month" field contains more than one digit.
			     */
			    my_error (ERR_INVALID_DATE_FIELD, filename,
				      line_number, line_buffer, 0);
			  tmp_n = CHR2DIG (s6[i - 1]);
			}
		      if (!hc && tmp_n && is_range)
			{
			  /*
			     Error, invalid "N'th weekday of month" number given.
			   */
			  if ((tmp_n > 5) && (tmp_n < 9))
			    my_error (ERR_INVALID_NWD_FIELD, filename,
				      line_number, line_buffer, tmp_n);
			  if (!is_month_given)
			    tmp_m = buf_m;
			  tmp_y = buf_y;
			  nth_weekday_of_month (&tmp_d, &tmp_m, &tmp_y,
						&tmp_n, &is_weekday_mode);
			  if (!tmp_m || tmp_y == SPECIAL_VALUE)
			    /*
			       If the date is not computable, exit the loop.
			     */
			    break;
			  if (y != tmp_y)
			    /*
			       If the dates are in different years, exit the loop.
			     */
			    break;
			}
		      else
			{
			  if (!hc && (tmp_n || is_range))
			    /*
			       Error, mixed range of days given (DD#WW[W], WW[W]#WW[W]N or WW[W]N#WW[w]).
			     */
			    my_error (ERR_INVALID_DATE_FIELD, filename,
				      line_number, line_buffer, 0);
			  else if (!tmp_hn && is_range)
			    /*
			       Error, invalid range of days given (NWW[W]#WW[W]).
			     */
			    my_error (ERR_INVALID_DATE_FIELD, filename,
				      line_number, line_buffer, 0);
			  else
			    tmp_hwd = tmp_d;
			}
		    }
		  else
		    {
		      if (is_day_given && !is_range)
			/*
			   Error, invalid range of days given (WW[W]#DD).
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		      if (!tmp_m && is_coded_month
			  && (rc_year_flag || rc_period_list || is_3month_mode
			      || is_3month_mode2 || fiscal_month > MONTH_MIN))
			/*
			   If the month of a final day is coded like: ...#MMDD
			   or ...#MMWW[W]N, but MM is explicitly set to zero (==00 coded)
			   and we are in one of these modes, exit the loop.
			 */
			break;
		      if (!is_month_given)
			tmp_m = m;
		      tmp_y = y;
		    }
		}
	      else
		tmp_hwd = 0;
	      /*
	         We have to avoid constructions like: YYYY00DD|WW[W]N#MMDD|WW[W]N
	         which means, no starting month but a final month is given and
	         if both months differ after pre-evaluating the starting month
	         (which is set to the current month), exit the loop.
	       */
	      if (!buf_m && is_month_given && (m != tmp_m))
		break;
	      if (hc)
		{
		  tmp_y = y;
		  if (islower (hc))
		    {
		      tmp_d = dvar_base_day;
		      tmp_m = dvar_base_month;
		      if (!precomp_date
			  (tmp_hn, tmp_hwd, &tmp_d, &tmp_m, tmp_y,
			   (hc ==
			    RC_EASTER_CHAR) ? EAster : ((hc ==
							 RC_TODAY_CHAR) ?
							TOday : DVar)))
			date_unset = TRUE;
		    }
		  else
		    if (!precomp_nth_wd
			(tmp_hn, tmp_hwd, &tmp_hn, &tmp_d, &tmp_m, &tmp_y,
			 (hc == 'D') ? DAy : WEek))
		    date_unset = TRUE;
		  if (date_unset)
		    {
		      /*
		         If the date is not computable, set `tmp_d' and `tmp_m'
		         to 1-JAN if `tmp_hn' is less zero, otherwise to 31-DEC
		         if `tmp_hn' is greater than +1.
		       */
		      if (!tmp_hn || tmp_hwd)
			/*
			   If the date isn't computable at all, exit the loop.
			 */
			break;
		      if (tmp_hn < 0)
			{
			  tmp_m = MONTH_MIN;
			  tmp_d = DAY_MIN;
			}
		      else
			{
			  tmp_m = MONTH_MAX;
			  tmp_d = dvec[tmp_m - 1];
			}
		    }
		  hc = '\0';
		}
	      n = 0;
	      /*
	         Let's decode the "for each N'th day since..." field of a line.
	       */
	      if (lptrs->appears_part != (char *) NULL)
		{
		  appears = get_number (&lptrs->appears_part);
		  if (!appears
		      || (*lptrs->appears_part
			  && !isspace (*lptrs->appears_part)))
		    /*
		       Error, invalid "appears" field given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  appears--;
		}
	      /*
	         Compute the number of 1xN productions.
	       */
	      if (is_range)
		{
		  /*
		     If the special value "99" for a day is given,
		     set the day to the last day of the month.
		   */
		  if (d == 99)
		    {
		      if (m == 2)
			d = days_of_february (y);
		      else
			d = dvec[m - 1];
		    }
		  if (tmp_d == 99)
		    {
		      if (tmp_m == 2)
			tmp_d = days_of_february (tmp_y);
		      else
			tmp_d = dvec[tmp_m - 1];
		    }
		  if (d > MONTH_LAST || tmp_d > MONTH_LAST)
		    /*
		       Error, invalid day number (>MONTH_LAST) is given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  if (!buf_y)
		    {
		      if (buf_m)
			manage_leap_day (&d, &m, y, line_buffer, filename,
					 line_number);
		      if (is_month_given || (buf_m && !is_month_given))
			manage_leap_day (&tmp_d, &tmp_m, tmp_y, line_buffer,
					 filename, line_number);
		      /*
		         Check if a range of days starting or ending at
		         00000229 must be ignored in non-leap years.
		       */
		      if ((days_of_february (y) == 28)
			  && (((d == 29)
			       && (m == 2)
			       && (m == buf_m))
			      || ((tmp_d == 29)
				  && (tmp_m == 2)
				  && (tmp_m == buf_m || is_month_given))))
			must_ignore_feb_29 = TRUE;
		    }
		  /*
		     If invalid dates are given, try to correct them.
		   */
		  while (!valid_date (d, m, y))
		    {
		      if (!buf_m || must_ignore_feb_29)
			d--;
		      else
			/*
			   Error, invalid date given (e.g.: YYYY0230 or YYYY0931).
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  while (!valid_date (tmp_d, tmp_m, tmp_y))
		    {
		      if ((!buf_m && !is_month_given) || must_ignore_feb_29)
			tmp_d--;
		      else
			/*
			   Error, invalid date given (e.g.: YYYY0230 or YYYY0931).
			 */
			my_error (ERR_INVALID_DATE_FIELD, filename,
				  line_number, line_buffer, 0);
		    }
		  /*
		     Compute the raw number of 1xN productions.
		   */
		  i = day_of_year (d, m, y);
		  j = day_of_year (tmp_d, tmp_m, tmp_y);
		  if (i > j)
		    {
		      /*
		         Swap the starting date of the event.
		       */
		      int tmp = i;
		      i = j;
		      j = tmp;

		      d = tmp_d;
		      m = tmp_m;
		      y = tmp_y;
		    }
		  productions = j - i;
		  if (productions)
		    {
		      /*
		         Optimize the number of 1xN productions.
		       */
		      if (i < day)
			{
			  if (appears)
			    {
			      register int k;


			      /*
			         Advance to the first date in period if an "appears" coding is given.
			       */
			      do
				{
				  k = i;
				  (void) doy2date (i + appears + 1,
						   (days_of_february (y) ==
						    29), &d, &m);
				  i = day_of_year (d, m, y);
				  if (i == k)
				    /*
				       The date can't be advanced by an "appears" factor
				       because the computed date would be leaving the year bounds!
				     */
				    i = day;
				}
			      while (i < day);
			    }
			  else
			    i = day;
			}
		      /*
		         Set a new starting date of the event.
		       */
		      (void) doy2date (i, (days_of_february (y) == 29), &d,
				       &m);
		      if (j > ed - 1)
			j = ed - 1;
		      productions = j - i;
		      if (productions < 0)
			/*
			   Date doesn't match the period, exit the loop.
			 */
			break;
		    }
		}
	      else
		{
		  if (is_month_given)
		    /*
		       Error, simple textual weekday ranges (WW[W]#WW[W]) may
		       not have a specified final month like: WW[W]#MMWW[W].
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		  /*
		     Compute the number of 1xN productions for simple weekdays!
		   */
		  if (d > tmp_d)
		    productions = DAY_MAX - d + tmp_d;
		  else
		    productions = tmp_d - d;
		  buf_d = d;
		}
	      productions++;
	      is_first = FALSE;
	    }
	}
      else if (is_first)
	{
	  /*
	     Single date given (means no list of days AND no range of days).
	   */
	  if (!y)
	    manage_leap_day (&d, &m, year, line_buffer, filename,
			     line_number);
	  /*
	     Let's decode the "repeat for N days since..." field of a line
	     and/or the "for each N'th day since..." field of a line.
	   */
	  if (lptrs->repeat_part != (char *) NULL
	      || lptrs->appears_part != (char *) NULL)
	    {
	      if (is_weekday_mode && !n)
		/*
		   Error, "repeat" or "appears" coding given to a
		   simple weekday name (e.g.: YYYYMMWW[W]:10.3 ...).
		 */
		my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			  line_buffer, 0);
	      if (lptrs->repeat_part != (char *) NULL)
		{
		  repeat = get_number (&lptrs->repeat_part);
		  if (!repeat
		      || (*lptrs->repeat_part
			  && !isspace (*lptrs->repeat_part)
			  && (*lptrs->repeat_part != RC_APPEARS_CHAR)))
		    /*
		       Error, invalid "repeat" field given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		}
	      if (lptrs->appears_part != (char *) NULL)
		{
		  appears = get_number (&lptrs->appears_part);
		  if (!appears
		      || (*lptrs->appears_part
			  && !isspace (*lptrs->appears_part)
			  && (*lptrs->appears_part != RC_REPEAT_CHAR)))
		    /*
		       Error, invalid "appears" field given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		}
	    }
	  if (!repeat)
	    /*
	       Let's create either 1x1 or 1x2 productions of such a line by default.
	     */
	    productions = 1;
	  else
	    {
	      /*
	         Now we have the number of productions of this date!
	       */
	      is_first = FALSE;
	      if (appears)
		appears--;
	      /*
	         Well, we have to use an unoptimized number of productions
	         in this case.
	       */
	      productions = repeat;
	    }
	}
      /*
         If the "date"-part of the current line is valid,
         try to respect the current line.
       */
      if (y >= 0)
	{
	  /*
	     Necessary pre-initializations of module local variables.
	   */
	  is_valid_date = TRUE;
	  is_2dvar = is_2easter = FALSE;
	  incr_year = decr_year = d_buf = m_buf = 0;
	  /*
	     Default is a 1x1 production of a line
	     (except some cases the `-c[]t' "tomorrow" flag is given).
	   */
	  print_twice = 1;
	  if (!is_range)
	    {
	      if (d)
		{
		  /*
		     If the special value "99" for a day is given,
		     set the day to the last day of the month.
		   */
		  if (d == 99)
		    {
		      if (m)
			{
			  if (m == 2)
			    d = days_of_february ((y) ? y : year);
			  else
			    d = dvec[m - 1];
			}
		      else
			{
			  if (month == 2)
			    d = days_of_february ((y) ? y : year);
			  else
			    d = dvec[month - 1];
			}
		    }
		  is_valid_date =
		    valid_date (d, (m) ? m : month, (y) ? y : year);
		}
	      if (!is_valid_date)
		{
		  if ((!y
		       && (((m == 2
			     || (!m
				 && (month == 2)))
			    && (d == 29))
			   || (!m
			       && (d <= MONTH_LAST))))
		      || (y && !m && (d <= MONTH_LAST)))
		    /*
		       If no month/year entry given, ignore that date.
		     */
		    ;
		  else
		    /*
		       Invalid date field given.
		     */
		    my_error (ERR_INVALID_DATE_FIELD, filename, line_number,
			      line_buffer, 0);
		}
	      /*
	         If a "N'th weekday of month" field is given, compute the according date.
	       */
	      if (n)
		{
		  nth_weekday_of_month (&d, &m, &y, &n, &is_weekday_mode);
		  if (!m)
		    /*
		       If the date is not computable (`m' is still 0), exit the loop.
		     */
		    break;
		}
	      else if (!y)
		{
		  if (hwd
		      && ((fiscal_month > MONTH_MIN)
			  || ((hc == RC_EASTER_CHAR)
			      && (day < DAY_MIN
				  || ed > DAY_LAST + is_leap_year + 1))))
		    /*
		       FIXME!
		       Actually it's not possible to evaluate events, which
		       occur in every year and are set to a "N'th weekday"
		       displacement, and the mode of operation is generally
		       for fiscal years or for weeks, which have left the
		       bounds of the actual year and are related to the Easter
		       Sundays date, because Easter Sunday is on one date in
		       the current year and on another date in the other year!!
		       Grrr... seems I've to rework this for future releases
		       ...or will it be a feature???
		     */
		    y = SPECIAL_VALUE;
		  else
		    {
		      /*
		         If @e... Easter holiday "date"-part is given, compute the according date.
		       */
		      if (hc == RC_EASTER_CHAR)
			{
			  register int epy = knuth_easter_formula (year - 1);	/* Easter Sunday in previous year */
			  register int eay = knuth_easter_formula (year);	/* Easter Sunday in current year */
			  register int eny = knuth_easter_formula (year + 1);	/* Easter Sunday in next year */


			  if (fiscal_month > MONTH_MIN)
			    {
			      /*
			         Precalculate the number of days in last month of fiscal year.
			       */
			      if (fiscal_month - 1 == 2)
				i = days_of_february (year + 1);
			      else
				i = dvec[fiscal_month - 2];
			      /*
			         Manage fiscal year request.
			       */
			      if (year == EASTER_MIN - 1)
				{
				  j =
				    day_of_year (i, fiscal_month - 1,
						 year + 1);
				  if (hn + eny <= j)
				    {
				      if (!precomp_date
					  (hn, hwd, &d, &m, year + 1, EAster))
					y = SPECIAL_VALUE;
				    }
				  else
				    y = SPECIAL_VALUE;
				}
			      else
				{
				  j =
				    day_of_year (DAY_MIN, fiscal_month, year);
				  if (hn + eay >= j)
				    {
				      if (!precomp_date
					  (hn, hwd, &d, &m, year, EAster))
					y = SPECIAL_VALUE;
				      else
					{
					  /*
					     Check whether this single event also occurs in the next year.
					   */
					  j =
					    day_of_year (i, fiscal_month - 1,
							 year + 1);
					  if (!is_3month_mode
					      && !is_3month_mode2
					      && (hn + eny <= j))
					    {
					      /*
					         We need a 1x2 production of the line.
					       */
					      print_twice++;
					      is_2easter = TRUE;
					    }
					}
				    }
				  else
				    {
				      if (is_3month_mode || is_3month_mode2)
					{
					  if (fiscal_month >= MONTH_MAX - 1)
					    {
					      j = dvec[MONTH_MIN - 1];
					      if (fiscal_month == MONTH_MAX)
						j +=
						  days_of_february (year +
								    1) +
						  is_leap_year;
					      if (hn + eny <= j)
						{
						  if (!precomp_date
						      (hn, hwd, &d, &m,
						       year + 1, EAster))
						    y = SPECIAL_VALUE;
						}
					      else
						y = SPECIAL_VALUE;
					    }
					  else
					    y = SPECIAL_VALUE;
					}
				      else
					{
					  j =
					    day_of_year (i, fiscal_month - 1,
							 year + 1);
					  if (hn + eny <= j)
					    {
					      if (!precomp_date
						  (hn, hwd, &d, &m, year + 1,
						   EAster))
						y = SPECIAL_VALUE;
					    }
					  else
					    y = SPECIAL_VALUE;
					}
				    }
				}
			    }
			  else
			    /*
			       Manage `-c0w' request resp.,
			       `-cw' in case date is in first days of January.
			     */
			  if (day < DAY_MIN)
			    {
			      j =
				DAY_LAST + (days_of_february (year - 1) ==
					    29);
			      if ((hn + epy >= j + day) && (hn + epy <= j))
				{
				  if (!precomp_date
				      (hn, hwd, &d, &m, year - 1, EAster))
				    y = SPECIAL_VALUE;
				}
			      else
				{
				  if (hn + eay < ed)
				    {
				      if (!precomp_date
					  (hn, hwd, &d, &m, year, EAster))
					y = SPECIAL_VALUE;
				    }
				  else
				    y = SPECIAL_VALUE;
				}
			    }
			  else
			    /*
			       Manage `-c99w' (`-c52w' | `-c53w') request resp.,
			       `-ct' or `-cw' in case date is in last days of December.
			     */
			  if (ed > DAY_LAST + is_leap_year + 1)
			    {
			      j = DAY_LAST + is_leap_year;
			      if ((hn + eay >= day) && (hn + eay <= j))
				{
				  if (!precomp_date
				      (hn, hwd, &d, &m, year, EAster))
				    y = SPECIAL_VALUE;
				}
			      else
				{
				  if (hn + eny < ed - j)
				    {
				      if (!precomp_date
					  (hn, hwd, &d, &m, year + 1, EAster))
					y = SPECIAL_VALUE;
				    }
				  else
				    y = SPECIAL_VALUE;
				}
			    }
			  else
			    /*
			       All other "ordinary" requests...
			     */
			  if (!precomp_date (hn, hwd, &d, &m, year, EAster))
			    y = SPECIAL_VALUE;
			}
		      else
			/*
			   If @t|DVAR... "date"-part is given, compute the according date.
			 */
		      if (islower (hc))
			{
			  if (fiscal_month > MONTH_MIN)
			    {
			      register int date_dvar =
				day_of_year (d, m, year);


			      /*
			         Precalculate number of days in the last month of fiscal year.
			       */
			      if (fiscal_month - 1 == 2)
				i = days_of_february (year + 1);
			      else
				i = dvec[fiscal_month - 2];
			      /*
			         Manage fiscal year request.
			       */
			      j = day_of_year (DAY_MIN, fiscal_month, year);
			      /*
			         Buffer day and month.
			       */
			      d_buf = d;
			      m_buf = m;
			      if (hn + date_dvar >= j)
				{
				  if (!precomp_date (hn, hwd, &d, &m, year,
						     (hc ==
						      RC_TODAY_CHAR) ? TOday :
						     DVar))
				    y = SPECIAL_VALUE;
				  else
				    {
				      /*
				         Check whether this single event also occurs in next year.
				       */
				      j =
					day_of_year (i, fiscal_month - 1,
						     year + 1);
				      if (!is_3month_mode && !is_3month_mode2
					  && (hn + date_dvar <= j))
					{
					  /*
					     We need a 1x2 production of the line.
					   */
					  print_twice++;
					  is_2dvar = TRUE;
					}
				    }
				}
			      else
				{
				  if (is_3month_mode || is_3month_mode2)
				    {
				      if (fiscal_month >= MONTH_MAX - 1)
					{
					  j = dvec[MONTH_MIN - 1];
					  if (fiscal_month == MONTH_MAX)
					    j +=
					      days_of_february (year + 1) +
					      is_leap_year;
					  if (hn + date_dvar <= j)
					    {
					      if (!precomp_date
						  (hn, hwd, &d, &m, year + 1,
						   (hc ==
						    RC_TODAY_CHAR) ? TOday :
						   DVar))
						y = SPECIAL_VALUE;
					    }
					  else
					    y = SPECIAL_VALUE;
					}
				      else
					y = SPECIAL_VALUE;
				    }
				  else
				    {
				      j =
					day_of_year (i, fiscal_month - 1,
						     year + 1);
				      if (hn + date_dvar <= j)
					{
					  if (!precomp_date
					      (hn, hwd, &d, &m, year + 1,
					       (hc ==
						RC_TODAY_CHAR) ? TOday :
					       DVar))
					    y = SPECIAL_VALUE;
					}
				      else
					y = SPECIAL_VALUE;
				    }
				}
			    }
			  else
			    /*
			       All other "ordinary" requests...
			     */
			  if (!precomp_date (hn, hwd, &d, &m, year,
					       (hc ==
						  RC_TODAY_CHAR) ? TOday :
					       DVar))
			    y = SPECIAL_VALUE;
			}
		      else
			/*
			   If a 0*d|wN[WW[W]] "date"-part is given, compute the according date.
			 */
		      if (hc == 'D' || hc == 'W')
			{
			  y = year;
			  if (precomp_nth_wd (hn, hwd, &hn, &d, &m, &y,
					      (hc == 'D') ? DAy : WEek))
			    {
			      register int date_dvar = day_of_year (d, m, y);


			      if (fiscal_month > MONTH_MIN)
				{
				  /*
				     Precalculate the number of days of the last month of fiscal year.
				   */
				  if (fiscal_month - 1 == 2)
				    i = days_of_february (year + 1);
				  else
				    i = dvec[fiscal_month - 2];
				  /*
				     Manage fiscal year request.
				   */
				  j =
				    day_of_year (DAY_MIN, fiscal_month, year);
				  if (date_dvar >= j)
				    {
				      /*
				         Check whether this single event also occurs in the next year
				       */
				      j =
					day_of_year (i, fiscal_month - 1,
						     year + 1);
				      if (!is_3month_mode && !is_3month_mode2
					  && (date_dvar <= j))
					{
					  /*
					     Buffer day and month.
					   */
					  d_buf = d;
					  m_buf = m;
					  /*
					     We need a 1x2 production of the line.
					   */
					  print_twice++;
					  is_2dvar = TRUE;
					}
				    }
				  else
				    {
				      if (is_3month_mode || is_3month_mode2)
					{
					  if (fiscal_month >= MONTH_MAX - 1)
					    {
					      j = dvec[MONTH_MIN - 1];
					      if (fiscal_month == MONTH_MAX)
						j +=
						  days_of_february (year +
								    1) +
						  is_leap_year;
					      if (date_dvar <= j)
						{
						  y++;
						  (void) precomp_nth_wd (hn,
									 hwd,
									 &hn,
									 &d,
									 &m,
									 &y,
									 (hc
									  ==
									  'D')
									 ? DAy
									 :
									 WEek);
						}
					      else
						y = SPECIAL_VALUE;
					    }
					  else
					    y = SPECIAL_VALUE;
					}
				      else
					{
					  j =
					    day_of_year (i, fiscal_month - 1,
							 year + 1);
					  if (date_dvar <= j)
					    {
					      y++;
					      (void) precomp_nth_wd (hn, hwd,
								     &hn, &d,
								     &m, &y,
								     (hc ==
								      'D') ?
								     DAy :
								     WEek);
					    }
					  else
					    y = SPECIAL_VALUE;
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	  /*
	     Check whether this current line must be displayed.
	   */
	  if (y >= 0)
	    {
	      if (!is_range && (repeat > 1))
		is_range = TRUE;
	      if (date_matches_period (&print_twice, day, ed, wd))
		/*
		   Now insert this current line into `rc_elems_table[]'
		   (lines with no "text"-part are valid and are displayed).
		 */
		insert_line_into_table (line_buffer, filename, line_number,
					rc_elems, len_date, print_twice);
	    }
	}
      /*
         We have to restore some elements of the date and
         to precompute the next date if a range of days is given!
       */
      if (lineptrs->day_range || repeat)
	{
	  if (is_range)
	    {
	      if (m)
		{
		  if (y <= 0)
		    y = year;
		  if (appears)
		    {
		      /*
		         Advance to the next date in period if an "appears" coding is given.
		       */
		      i = day_of_year (d, m, y);
		      (void) doy2date (i + appears + 1,
				       (days_of_february (y) == 29), &d, &m);
		      productions -= appears;
		    }
		  else
		    (void) next_date (&d, &m, &y);
		  /*
		     If we evaluate a list of days and found "repeat" or "appears" fields
		     in it and made all necessary productions, we must manage this
		     explicitly by setting `is_range' to FALSE and `productions'
		     to any value greater 1, otherwise the next element of the
		     list would not be managed!
		   */
		  if (lineptrs->day_list
		      && (productions <= 1)
		      && (lptrs->day_part != (char *) NULL))
		    {
		      productions = 2;
		      is_range = FALSE;
		    }
		}
	      else
		/*
		   Such a date is not computable.
		 */
	      if (lineptrs->day_list)
		{
		  /*
		     The list of days still contains elements.
		   */
		  if (lptrs->day_part != (char *) NULL)
		    {
		      productions = 2;
		      is_range = FALSE;
		    }
		  else
		    /*
		       We are at the end of a list of days.
		     */
		    break;
		}
	    }
	  else
	    {
	      if (appears)
		{
		  /*
		     Advance to the next date in period if an "appears" coding is given.
		   */
		  productions -= appears;
		  if (productions > 0)
		    for (i = 0; i <= appears; i++)
		      if (++buf_d > DAY_MAX)
			buf_d = DAY_MIN;
		}
	      else if (++buf_d > DAY_MAX)
		buf_d = DAY_MIN;
	      d = buf_d;
	      m = buf_m;
	      y = buf_y;
	    }
	}
      else if (lineptrs->day_list && (lptrs->day_part == (char *) NULL))
	/*
	   We have already managed the last element of a list of days.
	 */
	break;
    }
  while (--productions > 0);
  month = tmp_month;
  if (lineptrs->repeat_part != (char *) NULL)
    free (lineptrs->repeat_part);
  if (lineptrs->appears_part != (char *) NULL)
    free (lineptrs->appears_part);
}



static Bool
date_matches_period (print_twice, day, ed, wd)
     int *print_twice;
     int day;
     int ed;
     const int wd;
/*!
   Checks if a given date (module global variables `d' for the day, `m' for
     the month and `y' for the year) matches a given period, which starts at
     absolute day of year `day' and ends at absolute day of year `ed' (the
     actual weekday is given in `wd').  If the given date matches the
     specified period, return TRUE, otherwise FALSE.
*/
{
  register int i;
  register int j;
  auto int dd;
  auto int mm;
  auto int yy;
  auto Bool print_line = FALSE;
  auto Bool is_valid_date = TRUE;


  if (is_date_given
      || ((year != act_year)
	  && (day > 0))
      || ((month && (month != act_month)) && (year == act_year)))
    {
      /*
         NOT in simple month-/year mode (an explicit date is given in command line):
         Manage `-c[]' arguments.
       */
      if (d && !is_weekday_mode)
	{
	  incr_year = ((fiscal_month > MONTH_MIN)
		       && (m < fiscal_month) && (!y || y == year + 1));
	  if (is_date_given && month && !changed_month && !m)
	    {
	      m = month;
	      is_valid_date = valid_date (d, m, (y) ? y : year);
	    }
	  if (is_valid_date && (!y || y - incr_year == year) && m && d)
	    {
	      if (month && !changed_month)
		{
		  if (m == month)
		    {
		      if (!is_leap_year && (m == 2) && (d == 29))
			;	/* If the year is no leap year, ignore that date */
		      else
			print_line = TRUE;
		    }
		}
	      else
		{
		  if (fiscal_month > MONTH_MIN)
		    {
		      /*
		         Respect fiscal years!
		       */
		      if ((incr_year
			   && (year < YEAR_MAX))
			  || ((m >= fiscal_month) && (!y || y == year)))
			{
			  if ((days_of_february (year + incr_year) == 28)
			      && (m == 2) && (d == 29))
			    ;	/* If the year is no leap year, ignore that date */
			  else
			    print_line = TRUE;
			}
		    }
		  else
		    /*
		       Respect non fiscal years!
		     */
		  if (!is_leap_year && (m == 2) && (d == 29))
		    ;		/* If the year is no leap year, ignore that date */
		  else
		    print_line = TRUE;
		}
	    }
	}
    }
  else
    {
      /*
         Simple month-/year mode (NO explicit date is given in command line):
         Manage `-c[]w|m|y[+|-]' arguments.
       */
      if ((rc_period_flag
	   || rc_week_flag
	   || rc_month_flag
	   || rc_year_flag)
	  && !is_date_given
	  && (!y
	      || y == year
	      || (rc_week_flag
		  && (((year + 1 <= YEAR_MAX)
		       && (ed > DAY_LAST + is_leap_year + 1))
		      || ((year - 1 >= YEAR_MIN) && (day < DAY_MIN))))))
	{
	  register int td;


	  if (m && d)
	    {
	      if (rc_week_flag && !y && (m != month))
		{
		  if ((ed > DAY_LAST + is_leap_year + 1)
		      && !adate_set
		      && (rc_forwards_flag
			  || (!rc_forwards_flag && !rc_backwards_flag)))
		    y = year + 1;
		  else
		    if ((day < DAY_MIN)
			&& !adate_set
			&& (rc_backwards_flag
			    || (!rc_forwards_flag && !rc_backwards_flag)))
		    y = year - 1;
		}
	      if (y < YEAR_MIN || y > YEAR_MAX)
		{
		  if (adate_set)
		    {
		      if (m == month)
			{
			  if (day < DAY_MIN)
			    y = year - 1;
			}
		      else
			{
			  if (ed > DAY_LAST + is_leap_year + 1)
			    y = year + 1;
			}
		      if (y < YEAR_MIN || y > YEAR_MAX)
			y = year;
		    }
		  else
		    y = year;
		}
	    }
	  if ((rc_week_flag
	       && is_weekday_mode) || (!is_weekday_mode && (y || m || d)))
	    {
	      /*
	         Respect short day name entry YYYYMMWW[W] ... (WW[W]==short dayname).
	       */
	      if (rc_week_flag && is_weekday_mode)
		{
		  static struct
		  {
		    char day[DAY_MAX];
		    char dst[DAY_MAX];
		  } wday_list;
		  static Bool fill_wday_list = FALSE;


		  j = 0;
		  if (!fill_wday_list)
		    {
		      if (rc_forwards_flag
			  || (!rc_forwards_flag && !rc_backwards_flag))
			{
			  i = wd;
			  LOOP
			  {
			    wday_list.day[i - 1] = (char) i;
			    wday_list.dst[i - 1] = (char) j++;
			    i++;
			    if (i > DAY_MAX)
			      i = DAY_MIN;
			    if (i == start_day)
			      break;
			  }
			}
		      j = 0;
		      if (rc_backwards_flag
			  || (!rc_forwards_flag && !rc_backwards_flag))
			{
			  i = wd;
			  LOOP
			  {
			    if (i < DAY_MIN)
			      i = DAY_MAX;
			    wday_list.day[i - 1] = (char) i;
			    wday_list.dst[i - 1] = (char) ++j;
			    if (i == start_day)
			      break;
			    i--;
			  }
			}
		      fill_wday_list = TRUE;
		    }
		  if (wday_list.day[d - 1])
		    {
		      yy = year;
		      if (rc_week_year_flag)
			{
			  j = day;
			  if (day < 1)
			    {
			      yy = year - 1;
			      j += (DAY_LAST + (days_of_february (yy) == 29));
			    }
			  (void) doy2date (j, (days_of_february (yy) == 29),
					   &dd, &mm);
			}
		      else
			{
			  dd = act_day;
			  mm = month;
			}
		      if (rc_forwards_flag)
			for (i = 0; i < wday_list.dst[d - 1]; i++)
			  (void) next_date (&dd, &mm, &yy);
		      else if (rc_backwards_flag)
			for (i = 1; i < wday_list.dst[d - 1]; i++)
			  (void) prev_date (&dd, &mm, &yy);
		      else
			{
			  i = SYEAR (d, start_day);
			  j = SYEAR (wd, start_day);
			  if (i - j <= 0)
			    for (i = 1; i < wday_list.dst[d - 1]; i++)
			      (void) prev_date (&dd, &mm, &yy);
			  else
			    for (i = 0; i < wday_list.dst[d - 1]; i++)
			      (void) next_date (&dd, &mm, &yy);
			}
		      if ((!m || m == mm) && (!y || y == yy))
			{
			  d = dd;
			  m = mm;
			  y = yy;
			}
		      else
			y = SPECIAL_VALUE;
		    }
		  else
		    y = SPECIAL_VALUE;
		}
	      if (rc_week_flag
		  && (y >= 0)
		  && (day < DAY_MIN || ed > DAY_LAST + is_leap_year + 1))
		{
		  if ((rc_backwards_flag
		       || (!rc_forwards_flag
			   && !rc_backwards_flag))
		      && (y == year - 1) && m && d)
		    {
		      ed = DAY_LAST + (days_of_february (y) == 29);
		      day = ed + day;
		      td = day_of_year (d, m, y);
		      if ((td <= ed) && (td >= day))
			{
			  decr_year = 1;
			  print_line = TRUE;
			}
		    }
		  else
		    if ((rc_forwards_flag
			 || (!rc_forwards_flag
			     && !rc_backwards_flag))
			&& (y == year + 1) && m && d)
		    {
		      td = day_of_year (d, m, y) + DAY_LAST + is_leap_year;
		      if ((td < ed) && (td >= day))
			{
			  incr_year = 1;
			  print_line = TRUE;
			}
		    }
		  else
		    {
		      if ((y == year
			   || ((y == year - 1)
			       && !rc_forwards_flag)
			   || ((y == year + 1)
			       && !rc_backwards_flag)) && m && d)
			{
			  td = day_of_year (d, m, y);
			  if ((td < ed) && (td >= day))
			    print_line = TRUE;
			}
		      else if (d)
			{
			  td = 0;
			  m = month;
			  if (day < DAY_MIN)
			    {
			      if (!y || y == year || y == year - 1)
				{
				  if (adate_set)
				    i = dvec[MONTH_MAX - 1] + day - 1;
				  else
				    i =
				      dvec[MONTH_MAX - 1] + act_day -
				      (SYEAR (wd, start_day));
				  if (d > i)
				    {
				      m = MONTH_MAX;
				      if (!y)
					y = year - 1;
				      if (y == year)
					td = day_of_year (d, m, year);
				      else
					{
					  ed =
					    DAY_LAST +
					    (days_of_february (y) == 29);
					  day = ed + day;
					  td = day_of_year (d, m, y);
					  decr_year = 1;
					}
				    }
				  else if (!y || y == year)
				    {
				      if (adate_set)
					{
					  m = MONTH_MIN;
					  td = day_of_year (d, m, year) + 1;
					}
				      else
					td = day_of_year (d, m, year) + 1;
				    }
				  else
				    td = ed + 1;
				  if ((td <= ed) && (td >= day))
				    print_line = TRUE;
				}
			    }
			  else if (!y || y == year || y == year + 1)
			    {
			      i = act_day - (SYEAR (wd, start_day));
			      if (d < i)
				{
				  m = MONTH_MIN;
				  if (!y)
				    y = year + 1;
				  if (y == year + 1)
				    {
				      td =
					day_of_year (d, m,
						     y) + DAY_LAST +
					is_leap_year;
				      incr_year = 1;
				    }
				}
			      else if (!y || y == year)
				td = day_of_year (d, m, year);
			      if ((td < ed) && (td >= day))
				print_line = TRUE;
			    }
			}
		    }
		}
	      else if (y >= 0 || y == year)
		{
		  if (((rc_month_flag
			|| rc_week_flag
			|| rc_period_flag)
		       && m
		       && !d)
		      || ((rc_month_flag
			   || rc_week_flag)
			  && !m
			  && !d)
		      || ((rc_year_flag || rc_period_list) && (!m || !d)))
		    ;		/* Void, ignore such entries! */
		  else
		    {
		      register int mmm = m;


		      if (!y)
			y = year;
		      if (!m)
			{
			  mmm = m = month;
			  (void) doy2date (day, is_leap_year, &dd, &mm);
			  if ((dd > d) && (mm == m))
			    m++;
			  else if ((dd < d) && (mm < m))
			    m--;
			}
		      /*
		         If a valid date is given, respect it.
		       */
		      if (valid_date (d, m, y))
			{
			  td = day_of_year (d, m, y);
			  if ((td >= day) && (td < ed))
			    print_line = TRUE;
			  else
			    m = mmm;
			}
		      else
			m = mmm;
		    }
		}
	    }
	}
      else
	if (!rc_period_flag
	    && !rc_week_flag && !rc_month_flag && !rc_year_flag)
	{
	  /*
	     Simple month-/year month mode (NO explicit date is given in command line):
	     Manage `-c[]' or `-c[]t' arguments.
	   */
	  dd = act_day;
	  mm = month;
	  yy = year;
	  if (rc_tomorrow_flag)
	    (void) next_date (&dd, &mm, &yy);
	  if ((!y
	       || y == year
	       || (rc_tomorrow_flag
		   && (ed > DAY_LAST + is_leap_year + 1)
		   && (y == year + 1)
		   && (!m
		       || m == mm)))
	      && (!m
		  || m == month
		  || (rc_tomorrow_flag
		      && (m == mm)
		      && (!y
			  || ((ed > DAY_LAST + is_leap_year + 1)
			      && (y == year + 1))
			  || ((ed <= DAY_LAST + is_leap_year + 1)
			      && (y == year))))))
	    {
	      if (is_weekday_mode)
		{
		  /*
		     Respect short day name entry YYYYMMWW[W] ... (WW[W]==short dayname).
		   */
		  i = weekday_of_date (act_day, month, year);
		  j = 0;
		  if (rc_tomorrow_flag)
		    j = (d == weekday_of_date (dd, mm, yy));
		  if (((d == i)
		       && (ed <= DAY_LAST + is_leap_year + 1)
		       && (!m
			   || m == month))
		      || ((d == i)
			  && ((ed > DAY_LAST + is_leap_year + 1)
			      && ((!y
				   && (!m
				       || m == month))
				  || y == year)))
		      || (j
			  && (!m
			      || m == month + 1
			      || m == mm)
			  && (!y
			      || ((ed > DAY_LAST + is_leap_year + 1)
				  && (y == year + 1))
			      || ((ed <= DAY_LAST + is_leap_year + 1)
				  && (y == year)))))
		    {
		      if (j)
			{
			  m = mm;
			  d = dd;
			  if (rc_tomorrow_flag && (yy != year))
			    incr_year = 1;
			}
		      else
			{
			  m = month;
			  d = act_day;
			}
		      if (rc_tomorrow_flag
			  && !rc_have_today_in_list && (d != dd))
			;	/* Void, ignore such entries! */
		      else
			print_line = TRUE;
		    }
		}
	      else
		if (!d
		    || ((d == act_day)
			&& (!m
			    || m == month)
			&& (!y
			    || y == year))
		    || ((d == dd)
			&& (mm == month
			    || (rc_tomorrow_flag
				&& ((!m
				     && (ed <= DAY_LAST + is_leap_year + 1)
				     && (!y
					 || y == year))
				    || m == month + 1 || m == mm)))))
		{
		  if (rc_tomorrow_flag)
		    {
		      if (!d
			  && (!m
			      || (mm == month || m == month + 1 || m == mm)))
			{
			  if ((yy > year) && (d || m || y))
			    {
			      if (!y || y == yy)
				{
				  m = mm;
				  d = dd;
				  incr_year = 1;
				}
			    }
			  else
			    if (m
				|| y != year
				|| ed <= DAY_LAST + is_leap_year + 1)
			    {
			      if (!d
				  && m && (mm != month) && (!y || y == year))
				{
				  m = mm;
				  d = dd;
				}
			      else
				{
				  if (rc_have_today_in_list)
				    /*
				       We need a 1x2 production of the line.
				     */
				    (*print_twice)++;
				  else
				    {
				      m = mm;
				      d = dd;
				      if (yy != year)
					incr_year = 1;
				    }
				}
			    }
			}
		      else
			{
			  if ((d == dd) && (!m || m == mm))
			    {
			      m = mm;
			      if (ed > DAY_LAST + is_leap_year + 1)
				incr_year = 1;
			    }
			}
		    }
		  if (!m)
		    m = month;
		  if (!d)
		    d = act_day;
		  if (rc_tomorrow_flag && !rc_have_today_in_list && (d != dd))
		    ;		/* Void, ignore such entries! */
		  else
		    print_line = TRUE;
		}
	    }
	}
    }
  /*
     Avoid an incorrect assignment in case fixed date mentioned occurred
     during the missing period in month of Gregorian Reformation.
   */
  if (print_line
      && (year + incr_year - decr_year == greg->year)
      && (m == greg->month)
      && ((d >= greg->first_day) && (d <= greg->last_day)))
    print_line = FALSE;
  /*
     If 3-month mode is wanted, insert only those fixed dates
     which occur in that period.
   */
  if (print_line && (is_3month_mode || is_3month_mode2))
    {
      register int m2 = fiscal_month + 1;
      register int m3 = fiscal_month + 2;


      if (fiscal_month >= MONTH_MAX - 1)
	{
	  m3 = MONTH_MIN;
	  if (fiscal_month == MONTH_MAX)
	    {
	      m2 = MONTH_MIN;
	      m3++;
	    }
	}
      if ((m != fiscal_month) && (m != m2) && (m != m3))
	print_line = FALSE;
    }

  return (print_line);
}



static int
get_number (string)
     char **string;
/*!
   Returns the absolute value of a "repeat for N days since..." field or
     "for each N'th day since..." field using the global `s6' text buffer.
*/
{
  register int i = 0;


  if (*string != (char *) NULL)
    {
      (*string)++;
      while (isdigit (**string))
	s6[i++] = *((*string)++);
      s6[i] = '\0';
      /*
         Make sure we return values in range 1...366
         or 0 if an error has occurred.
       */
      if (i > 3)
	return (0);
      i = atoi (s6);
      if (i > DAY_LAST + 1)
	i = DAY_LAST + 1;
    }

  return (i);
}
#endif /* USE_RC */
