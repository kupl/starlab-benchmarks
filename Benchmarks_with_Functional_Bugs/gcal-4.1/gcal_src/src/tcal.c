/*!
*  \file tcal.c
*  \brief Start `gcal' with date set ONE day ahead
*
*  (default mode, like the `--shift=1' long-style option is passed to it).
*  Useful if you call this program in `~/.profile' and you want to see all
*  fixed dates related to tomorrow + 1 day (advanced  -ct respectively
*  --period-of-fixed-dates=t  option).  Works *only* for Gregorian years!
*/
/*
*  Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013, 2014 Free Software Foundation, Inc.
*  Copyright (c) 1995, 96, 1997, 2000 Thomas Esken
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
#if HAVE_ASSERT_H
# include <assert.h>
#endif
#if HAVE_CTYPE_H
# include <ctype.h>
#endif
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#if HAVE_LIMITS_H
# include <limits.h>
#endif
#if HAVE_ERRNO_H
# include <errno.h>
#endif
#if (!HAVE_SIGNAL_H || !HAVE_SIGNAL) && HAVE_SYS_TYPES_H
/* Otherwise "gcal.h" includes <sys/types.h>. */
# include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else /* !TIME_WITH_SYS_TIME */
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else /* !HAVE_SYS_TIME_H */
#  include <time.h>
# endif	/* !HAVE_SYS_TIME_H */
#endif /* !TIME_WITH_SYS_TIME */
#ifdef MSDOS
# include <process.h>
#endif
#ifndef USE_RC
# define USE_RC  1
#else
# if !USE_RC
#  undef  USE_RC
#  define USE_RC  1
# endif
#endif
#include "common.h"
#include "rc-defs.h"
#include "tcal.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
  static void
  usage_msg __P_ ((FILE * fp, const char *prgr_name, int exit_status));
static void
  version_msg __P_ ((FILE * fp, const char *prgr_name, int exit_status));
static VOID_PTR
  my_malloc __P_ ((const int amount,
		   const int exit_status,
		   const char *module_name,
		   const long module_line,
		   const char *var_name, const int var_contents));
static VOID_PTR
  my_realloc __P_ ((VOID_PTR ptr_memblock,
		    const int amount,
		    const int exit_status,
		    const char *module_name,
		    const long module_line,
		    const char *var_name, const int var_contents));
static void
  my_error __P_ ((const int exit_status,
		  const char *module_name,
		  const long module_line,
		  const char *var_name, const int var_contents));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
static RETSIGTYPE handle_signal __P_ ((int the_signal));
#endif
#if !HAVE_STRNCASECMP
static int my_strncasecmp __P_ ((const char *s1, const char *s2, int len));
#endif /* !HAVE_STRNCASECMP */
static int days_of_february __P_ ((const int year));
static Bool
  doy2date __P_ ((int doy, const int is_leap_year, int *day, int *month));
static Ulint date2num __P_ ((const int day, const int month, const int year));
static void num2date __P_ ((Ulint mjd, int *day, int *month, int *year));
static void get_actual_date __P_ ((int *day, int *month, int *year));
__END_DECLARATIONS
/*
*  static variables definitions.
*/
/*!
   Number of days in months.
*/
static const int dvec[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
   Number of past days of month.
*/
static const int mvec[] =
  { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/*!
   The Gregorian Reformation dates table is an unterminated vector
     of `Greg_struct'.
*/
static Greg_struct greg_reform_date[] = {
/*
  { int year, int month, int f_day, int l_day },
*/
  {1582, 10, 5, 14},
  {1752, 9, 3, 13}
};

/*! Points to the used Gregorian Reformation date. */
static Greg_struct *greg = greg_reform_date;

#ifdef DJG
/*! Set to SHRT_MAX for checking the maximum table range. */
static Usint testval = (Usint) 0;
#else
/*! Set to INT_MAX for checking the maximum table range. */
static Uint testval = (Uint) 0;
#endif

/*! Actual length of all strings. */
static Uint maxlen_max = MAXLEN_MAX;

/*! The name of this executable. */
static char *prgr_name = (char *) NULL;

/*! Text of `--help' option name. */
static char *help_option_name = "help";

/*! Text of `--version' option name. */
static char *version_option_name = "version";

/*! Text of `--shift' option name. */
static char *shift_option_name = "shift";



/*
*  Function implementations.
*/
static void
usage_msg (fp, prgr_name, exit_status)
     FILE *fp;
     const char *prgr_name;
     int exit_status;
/*!
   Writes the program "usage" text to file `fp' and
     terminates the program with `exit_status'.
*/
{
  fprintf (fp,
	   _
	   ("Usage:  %s  [--%s | --%s] | [--%s=[+|-]NUMBER] [ARGUMENT...]\n"),
	   prgr_name, help_option_name, version_option_name,
	   shift_option_name);
  if (exit_status == EXIT_SUCCESS)
    {
      S_NEWLINE (fp);
      fprintf (fp, _("Email bug reports to <%s>"), BUG_REPORT_ADR1);
      S_NEWLINE (fp);
    }
  exit (exit_status);
}



static void
version_msg (fp, prgr_name, exit_status)
     FILE *fp;
     const char *prgr_name;
     int exit_status;
/*!
   Writes the program "version" text to file `fp' and
     terminates the program with `exit_status'.
*/
{
  fprintf (fp, "%s (GNU cal %s)\n", prgr_name, PACKAGE_VERSION);
  fprintf (fp, "%s\n", COPYRIGHT_TXT);
  fprintf (fp,
	   _
	   ("This is free software; see the source for copying conditions."));
  S_NEWLINE (fp);
  fprintf (fp,
	   _("There is NO warranty, without even the implied warranty of"));
  S_NEWLINE (fp);
  fprintf (fp, _("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."));
  S_NEWLINE (fp);
  exit (exit_status);
}



static VOID_PTR
my_malloc (amount, exit_status, module_name, module_line, var_name,
	   var_contents)
     const int amount;
     const int exit_status;
     const char *module_name;
     const long module_line;
     const char *var_name;
     const int var_contents;
/*!
   Allocate AMOUNT bytes of memory dynamically, with error checking.
     Calls `my_error()' and terminates the program if any errors occur.
     AMOUNT is limited to `int' range instead of `size_t' range;
     this is wanted!
*/
{
  auto VOID_PTR ptr_memblock;


  if ((Uint) amount > testval)
    /*
       Error, table size overflow!
     */
    my_error (ERR_INTERNAL_TABLE_CRASH, module_name, module_line, var_name,
	      (int) testval);
  ptr_memblock = (VOID_PTR) malloc ((int) amount);
  if (ptr_memblock == (VOID_PTR) NULL)
    /*
       Error, `malloc()' function failed.
     */
    my_error (exit_status, module_name, module_line, var_name, var_contents);

  return (ptr_memblock);
}



static VOID_PTR
my_realloc (ptr_memblock, amount, exit_status, module_name, module_line,
	    var_name, var_contents)
     VOID_PTR ptr_memblock;
     const int amount;
     const int exit_status;
     const char *module_name;
     const long module_line;
     const char *var_name;
     const int var_contents;
/*!
   Change the size of an allocated block of memory PTR_MEMBLOCK to AMOUNT
     bytes, with error checking.  Calls `my_error()' and terminates the program
     if any errors occur.  AMOUNT is limited to `int' range instead of `size_t'
     range; this is wanted!  If PTR_MEMBLOCK is NULL, `my_malloc()' is called
     instead.
*/
{
  if ((Uint) amount > testval)
    /*
       Error, table size overflow!
     */
    my_error (ERR_INTERNAL_TABLE_CRASH, module_name, module_line, var_name,
	      (int) testval);
  if (ptr_memblock == (VOID_PTR) NULL)
    return (my_malloc
	    (amount, exit_status, module_name, module_line, var_name,
	     var_contents));
  ptr_memblock = (VOID_PTR) realloc (ptr_memblock, (int) amount);
  if (ptr_memblock == (VOID_PTR) NULL)
    /*
       Error, `realloc()' function failed.
     */
    my_error (exit_status, module_name, module_line, var_name, var_contents);

  return (ptr_memblock);
}



static void
my_error (exit_status, module_name, module_line, var_name, var_contents)
     const int exit_status;
     const char *module_name;
     const long module_line;
     const char *var_name;
     const int var_contents;
/*!
   Displays a specific error message on STDERR channel
     and terminates the program with status `exit_status'.
*/
{
  fprintf (stderr, _("\n%s: abort, "), prgr_name);
  switch (exit_status)
    {
    case ERR_NO_MEMORY_AVAILABLE:
      fprintf (stderr, _("`%s' line %ld: virtual memory exhausted (%s=%d)"),
	       module_name, module_line, var_name, var_contents);
      break;
    case ERR_INTERNAL_TABLE_CRASH:
      fprintf (stderr,
	       _
	       ("`%s' line %ld: (`%s') invalid value for table size `sizeof %s>%d'"),
	       module_name, module_line, _("internal"), var_name,
	       var_contents);
      break;
    case EXIT_FATAL:
      fprintf (stderr, _("shift value `%s' is invalid"), var_name);
      break;
    default:
      fprintf (stderr, _("`%s' line %ld: (`%s') unmanaged error (%d)"),
	       module_name, module_line, _("internal"), exit_status);
    }
  S_NEWLINE (stderr);
  exit (exit_status);
}



#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
static RETSIGTYPE
handle_signal (the_signal)
     int the_signal;
/*!
   Signal handler function which displays the numeric ID of the
     received signal on STDERR channel and terminates the program
     with ERR_TERMINATION_BY_SIGNAL exit status.
*/
{
  fflush (stdout);
  fprintf (stderr, _("\n%s: program aborted by signal %d\n"), prgr_name,
	   the_signal);
  exit (ERR_TERMINATION_BY_SIGNAL);
}
#endif /* HAVE_SIGNAL && (SIGINT || SIGTERM || SIGHUP) */



#if !HAVE_STRNCASECMP
static int
my_strncasecmp (s1, s2, len)
     const char *s1;
     const char *s2;
     int len;
/*!
   Same as the ANSI C `strncmp()' function, but case insensitive.
*/
{
  auto const Uchar *p1 = (const Uchar *) s1;
  auto const Uchar *p2 = (const Uchar *) s2;
  auto Uchar c1;
  auto Uchar c2;


  if (!len || p1 == p2)
    return (0);
  if (len < 0)
    len = -len;
  do
    {
      c1 = tolower (*p1++);
      c2 = tolower (*p2++);
      if (!c1 || c1 != c2)
	return (c1 - c2);
    }
  while (--len);

  return (c1 - c2);
}
#endif /* !HAVE_STRNCASECMP */



static int
days_of_february (year)
     const int year;
/*!
   Computes the number of days in February --- respecting
     the Gregorian Reformation period --- and returns them.
*/
{
  register int day;


  if ((year > greg->year)
      || ((year == greg->year)
	  && (greg->month == 1
	      || ((greg->month == 2) && (greg->last_day >= 28)))))
    day = (year & 3) ? 28 : ((!(year % 100) && (year % 400)) ? 28 : 29);
  else
    day = (year & 3) ? 28 : 29;
  /*
     Exception, the year 4 AD was historically NO leap year!
   */
  if (year == 4)
    day--;

  return (day);
}



static Bool
doy2date (doy, is_leap_year, day, month)
     int doy;
     const int is_leap_year;
     int *day;
     int *month;
/*!
   Converts a given number of days of a year to a standard date
     (returned in &day and &month) and returns:
       TRUE in case the `day_of_year' number is valid;
       FALSE otherwise.
*/
{
  register int i;
  auto Bool decrement_date;


  if (doy > DAY_LAST + is_leap_year || doy < DAY_MIN)
    return (FALSE);
  decrement_date = (Bool) (is_leap_year && (doy > mvec[2]));
  if (decrement_date)
    doy--;
  for (i = MONTH_MIN; i < MONTH_MAX; i++)
    {
      doy -= dvec[i - 1];
      if (doy <= 0)
	{
	  doy += dvec[i - 1];
	  break;
	}
    }
  *month = i;
  *day = doy;
  if (decrement_date && (*month == 2) && (*day == 28))
    (*day)++;

  return (TRUE);
}



static Ulint
date2num (day, month, year)
     const int day;
     const int month;
     const int year;
/*!
   Computes the absolute number of days of the given date since
     00010101(==YYYYMMDD) respecting the missing period of the
     Gregorian Reformation.
*/
{
  auto Ulint mjd =
    (Ulint) ((year - 1) * (Ulint) (DAY_LAST) + ((year - 1) >> 2));


  if (year > greg->year
      || ((year == greg->year)
	  && (month > greg->month
	      || ((month == greg->month) && (day > greg->last_day)))))
    mjd -= (Ulint) (greg->last_day - greg->first_day + 1);
  if (year > greg->year)
    {
      mjd += (((year - 1) / 400) - (greg->year / 400));
      mjd -= (((year - 1) / 100) - (greg->year / 100));
      if (!(greg->year % 100) && (greg->year % 400))
	mjd--;
    }
  mjd += (Ulint) mvec[month - 1];
  mjd += day;
  if ((days_of_february (year) == 29) && (month > 2))
    mjd++;

  return (mjd);
}



static void
num2date (mjd, day, month, year)
     Ulint mjd;
     int *day;
     int *month;
     int *year;
/*!
   Converts a delivered absolute number of days `mjd' to a standard
     date (since 00010101(==YYYYMMDD), returned in &day, &month and &year)
     respecting the missing period of the Gregorian Reformation.
*/
{
  auto double x;
  auto Ulint jdays = date2num (greg->first_day - 1, greg->month, greg->year);
  register int i;


  if (mjd > jdays)
    mjd += (Ulint) (greg->last_day - greg->first_day + 1);
  x = (double) mjd / (DAY_LAST + 0.25);
  i = (int) x;
  if ((double) i != x)
    *year = i + 1;
  else
    {
      *year = i;
      i--;
    }
  if (mjd > jdays)
    {
      /*
         Correction for Gregorian years.
       */
      mjd -= (Ulint) ((*year / 400) - (greg->year / 400));
      mjd += (Ulint) ((*year / 100) - (greg->year / 100));
      x = (double) mjd / (DAY_LAST + 0.25);
      i = (int) x;
      if ((double) i != x)
	*year = i + 1;
      else
	{
	  *year = i;
	  i--;
	}
      if ((*year % 400) && !(*year % 100))
	mjd--;
    }
  i = (int) (mjd - (Ulint) (i * (DAY_LAST + 0.25)));
  /*
     Correction for Gregorian centuries.
   */
  if ((*year > greg->year)
      && (*year % 400)
      && !(*year % 100)
      && (i <
	  ((*year / 100) - (greg->year / 100)) - ((*year / 400) -
						  (greg->year / 400))))
    i++;
  (void) doy2date (i, (days_of_february (*year) == 29), day, month);
}



static void
get_actual_date (day, month, year)
     int *day;
     int *month;
     int *year;
/*!
   Gets the actual date from the system.
*/
{
  auto struct tm *sys_date;
  auto MY_TIME_T sys_time;


  sys_time = time ((MY_TIME_T *) NULL);
  sys_date = localtime (&sys_time);
  *day = sys_date->tm_mday;
  *month = sys_date->tm_mon + 1;
  *year = sys_date->tm_year;
  if (*year < CENTURY)
    *year += CENTURY;
}



int
main (argc, argv)
     int argc;
     char *argv[];
/*!
   Starts Gcal with date set one day ahead (default mode, like the `--shift=1'
     long-style option is passed to it) and all other arguments which are given
     in the command line.  The program first tries to use the `gcal' executable
     which is specified in the environment variable $GCALPROG if it is set,
     otherwise it will be searched using the $PATH environment variable.
*/
{
  auto Ulint the_date;
  auto Slint shift_value = (Slint) CHR2DIG (*SHIFT_VALUE_DEFAULT);
  register Uint my_argc_max = MY_ARGC_MAX;
  register int my_argc = 0;
  register int len_year_max;
  register int i;
  register int arg_len;
  register int status;
  auto int day;
  auto int month;
  auto int year;
  auto char **my_argv = (char **) NULL;
  auto char *s1;
  auto char *gcal_prgr;
  auto char *ptr_char;
  auto char *buf_ptr_char;
  auto Bool shift_value_set = FALSE;
#if defined(GCAL_NLS)
  auto Bool is_en = FALSE;
#endif


  /*
     Let's set `testval' to SHRT_MAX/INT_MAX if SHRT_MAX/INT_MAX itself
     isn't defined.  This solution only works on machines with internal
     arithmethics based on "two complements".
   */
#ifdef DJG
# ifdef SHRT_MAX
  testval = SHRT_MAX;
# else /* !SHRT_MAX */
  testval = ~0;
  testval >>= 1;
# endif	/* !SHRT_MAX */
#else /* !DJG */
# ifdef INT_MAX
  testval = INT_MAX;
# else /* !INT_MAX */
  testval = ~0;
  testval >>= 1;
# endif	/* !INT_MAX */
#endif /* !DJG */
#if HAVE_ASSERT_H
  /*
     To ensure safe program operations,
     MAXLEN_MAX must be 1024 minimum and `testval' maximum!
   */
  assert (MAXLEN_MAX >= 1024);
  assert ((Uint) MAXLEN_MAX <= testval);
#endif
  /*
     Initial memory allocation for the `s1' string.
   */
  s1 = (char *) my_malloc (MAXLEN_MAX, ERR_NO_MEMORY_AVAILABLE,
			   __FILE__, ((long) __LINE__) - 1L, "s1", 0);
  /*
     Compute the string length of the maximum year able to compute.
   */
  sprintf (s1, "%d", YEAR_MAX);
  len_year_max = (int) strlen (s1);
  /*
     Perform some more assertations for safe program operation.
   */
#if HAVE_ASSERT_H
  /*
     Check if value for maximum number of table entries
     fits to the positive range of a signed int (SHRT_MAX/INT_MAX)!
   */
  assert (len_year_max < 11);
  assert (len_year_max > 0);
  assert (YEAR_MAX >= YEAR_MIN);
  assert (MONTH_MAX == 12);
  assert (CENTURY == 1900);
  assert (strlen (PACKAGE_NAME) > 0);
  assert (strlen (PACKAGE_VERSION) > 0);
  assert (MY_ARGC_MAX > 1);
  assert ((Uint) MY_ARGC_MAX <= testval);
#endif /* HAVE_ASSERT_H */
#ifdef GCAL_NLS
  /*
     Now initialize the NLS functions.
   */
#if HAVE_SETLOCALE
  setlocale (LC_ALL, "");
# endif
# ifndef LOCALEDIR
#  define LOCALEDIR  NULL
# endif
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  /*
     Now check whether we have to use the Gregorian Reformation date of 1752
     (table index 1 !!) by default!
   */
# if !defined(AMIGA) || defined(__GNUC__)
  /*
     Detect whether the $LANGUAGE environment variable (GNU specific) is set.
   */
  ptr_char = getenv (ENV_VAR_LANGUAGE);
  if (ptr_char != (char *) NULL)
    if (!*ptr_char)
      ptr_char = (char *) NULL;
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LC_ALL environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LC_ALL);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
#  if HAVE_LC_MESSAGES
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LC_MESSAGES environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LC_MESSAGES);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
#  endif
  if (ptr_char == (char *) NULL)
    {
      /*
         Detect whether the $LANG environment variable is set.
       */
      ptr_char = getenv (ENV_VAR_LANG);
      if (ptr_char != (char *) NULL)
	if (!*ptr_char)
	  ptr_char = (char *) NULL;
    }
  /*
     Now check the kind of territory specifics we have to use!
   */
  if (ptr_char != (char *) NULL)
    {
      if (*ptr_char)
	{
	  strncpy (s1, ptr_char, 6);
	  s1[5] = '\0';
	  if (!strncasecmp (s1, "en", 2))
	    /*
	       We have to use the Gregorian Reformation date of 1752 (table index 1 !!).
	     */
	    is_en = TRUE;
	  else
	    /*
	       Hmm... We have to check whether the special settings "C" or
	       "POSIX" are given, if so, we have to use the Gregorian
	       Reformation date of 1752 (table index 1 !!).
	     */
	  if (!strncasecmp (s1, "posix", 5) || !strcasecmp (s1, "c"))
	    is_en = TRUE;
	}
      else
	/*
	   Environment variable defined but not set, hmm...
	   Let's use the Gregorian Reformation date of 1752 (table index 1 !!).
	 */
	is_en = TRUE;
    }
  else
    /*
       No environment variable defined.
     */
# endif /* !AMIGA || __GNUC__ */
    /*
       Let's use the Gregorian Reformation date of 1752 (table index 1 !!).
     */
    is_en = TRUE;
  if (is_en)
    /*
       Set the date of Gregorian Reformation to 1752 (table index 1 !!)
     */
    greg++;
#else /* !GCAL_NLS */
  /*
     Set the date of Gregorian Reformation to 1752 (table index 1 !!)
   */
  greg++;
#endif	/* !GCAL_NLS */
  /*
     Detect the own program name.
   */
  i = (int) strlen (*argv);
  if ((Uint) i >= maxlen_max)
    s1 = my_realloc ((VOID_PTR) s1,
		     i + 1, ERR_NO_MEMORY_AVAILABLE,
		     __FILE__, ((long) __LINE__) - 2L, "s1", i + 1);
  strcpy (s1, *argv);
#ifdef SUFFIX_SEP
  /*
     Eliminate version suffix under VMS.
   */
  ptr_char = strrchr (s1, *SUFFIX_SEP);
  if (ptr_char != (char *) NULL)
    *ptr_char = '\0';
#endif
  i = (int) strlen (s1);
#ifdef DJG
  ptr_char = strrchr (s1, *DIR2_SEP);
#else /* !DJG */
  ptr_char = strrchr (s1, *DIR_SEP);
#endif /* !DJG */
  if (ptr_char != (char *) NULL)
    {
      ptr_char++;
      i = (int) strlen (ptr_char);
    }
  else
    ptr_char = s1;
  if (tolower ('A') == 'a')
    {
      buf_ptr_char = ptr_char;
      for (; *ptr_char; ptr_char++)
	*ptr_char = (char) tolower (*ptr_char);
      ptr_char = buf_ptr_char;
    }
  /*
     Suppress ".exe" suffix for MSDOS, OS/2 and VMS.
   */
  if ((i > 4) && !strcmp (ptr_char + i - 4, ".exe"))
    {
      i -= 4;
      *(ptr_char + i) = '\0';
    }
  prgr_name = (char *) my_malloc (i + 1, ERR_NO_MEMORY_AVAILABLE,
				  __FILE__, ((long) __LINE__) - 1L,
				  "prgr_name", 0);
  strcpy (prgr_name, ptr_char);
#if HAVE_SIGNAL
  /*
     Now let's modify the signal handling a bit to make sure that
     temporary files are always deleted if such signals are raised.
   */
# ifdef SIGINT
  if (signal (SIGINT, SIG_IGN) != SIG_IGN)
    (void) signal (SIGINT, (Sig_type) handle_signal);
# endif
# ifdef SIGTERM
  if (signal (SIGTERM, SIG_IGN) != SIG_IGN)
    (void) signal (SIGTERM, (Sig_type) handle_signal);
# endif
# ifdef SIGHUP
  if (signal (SIGHUP, SIG_IGN) != SIG_IGN)
    (void) signal (SIGHUP, (Sig_type) handle_signal);
# endif
#endif /* HAVE_SIGNAL */
  buf_ptr_char = SHIFT_VALUE_DEFAULT;
  if (argc > 1)
    {
      /*
         Check if `--help', `--version' or `--shift=[+|-]NUMBER'
         long-style option is given.
       */
      arg_len = (int) strlen (argv[1]) - 2;
      if ((arg_len > 0)
	  && (*argv[1] == *SWITCH) && (*(argv[1] + 1) == *SWITCH))
	{
	  auto Bool is_number = TRUE;


	  if (!strncasecmp (argv[1] + 2, help_option_name, arg_len))
	    usage_msg (stdout, prgr_name, EXIT_SUCCESS);
	  if (!strncasecmp (argv[1] + 2, version_option_name, arg_len))
	    version_msg (stdout, prgr_name, EXIT_SUCCESS);
	  if (!strncasecmp (argv[1] + 2, shift_option_name, 3))
	    {
	      ptr_char = strchr (argv[1] + 2, *LARG_SEP);
	      if (ptr_char != (char *) NULL)
		i = (int) (ptr_char - (argv[1] + 2));
	      else
		i = arg_len;
	      switch (i)
		{
		case 3:
		case 4:
		case 5:
		  if (!strncasecmp (argv[1] + 2, shift_option_name, i))
		    break;
		  /* Fallthrough. */
		default:
		  /*
		     Error, unrecognized option.
		   */
		  fprintf (stderr, _("%s: unrecognized option `%s'"),
			   prgr_name, argv[1]);
		  S_NEWLINE (stderr);
		  usage_msg (stderr, prgr_name, ERR_INVALID_OPTION);
		}
	      if (ptr_char == (char *) NULL
		  || ((ptr_char != (char *) NULL) && !*(ptr_char + 1)))
		{
		  /*
		     Error, option requires an argument.
		   */
		  fprintf (stderr,
			   _("%s: option `--%s' requires an argument"),
			   prgr_name, shift_option_name);
		  S_NEWLINE (stderr);
		  usage_msg (stderr, prgr_name, ERR_INVALID_OPTION);
		}
	      else
		{
		  /*
		     Let's get the argument of the `--shift=[+|-]NUMBER'
		     long-style option.
		   */
		  ptr_char++;
		  shift_value = atol (ptr_char);
		  /*
		     Check if the argument is a NUMBER.
		   */
		  buf_ptr_char = ptr_char;
		  if (*ptr_char == '+' || *ptr_char == '-')
		    ptr_char++;
		  if (!*ptr_char)
		    is_number = FALSE;
		  else
		    {
		      while (isdigit (*ptr_char))
			ptr_char++;
		      if (*ptr_char)
			is_number = FALSE;
		    }
		  if (!is_number)
		    {
		      /*
		         Error, invalid argument.
		       */
		      fprintf (stderr,
			       _("%s: option with invalid argument -- %s"),
			       prgr_name, argv[1]);
		      S_NEWLINE (stderr);
		      usage_msg (stderr, prgr_name, ERR_INVALID_OPTION);
		    }
		  shift_value_set = TRUE;
		}
	    }
	}
    }
  /*
     Detect the name of the Gcal executable.
   */
#if !defined(AMIGA) || defined(__GNUC__)
  gcal_prgr = getenv (ENV_VAR_GCALPROG);
  if (gcal_prgr != (char *) NULL)
    {
      if (!*gcal_prgr)
	gcal_prgr = PACKAGE_NAME;
    }
  else
#endif /* !AMIGA || __GNUC__ */
    gcal_prgr = PACKAGE_NAME;
  /*
     Get the actual date.
   */
  get_actual_date (&day, &month, &year);
  /*
     Compute the Julian Day number of the actual date.
   */
  the_date = date2num (day, month, year);
  /*
     Check if the shifted date is in the right range.
   */
  if ((Slint) the_date + shift_value <= 0L
      || (Slint) the_date + shift_value >
      (Slint) date2num (dvec[MONTH_MAX - 1], MONTH_MAX, YEAR_MAX))
    my_error (EXIT_FATAL, "", 0L, buf_ptr_char, 0);
  /*
     Now calculate the shifted date (default mode is tomorrow [+1]).
   */
  num2date ((Ulint) the_date + shift_value, &day, &month, &year);
  /*
     Initial memory allocation for the `my_argv[]' table.
   */
  my_argv = (char **) my_malloc (MY_ARGC_MAX * sizeof (char *),
				 ERR_NO_MEMORY_AVAILABLE,
				 __FILE__, ((long) __LINE__) - 2L,
				 "my_argv[MY_ARGC_MAX]", MY_ARGC_MAX);
  /*
     Now copy the name of the Gcal executable into `my_argv[]'
     table at position 0.
   */
  my_argv[my_argc] = (char *) my_malloc (strlen (gcal_prgr) + 1,
					 ERR_NO_MEMORY_AVAILABLE,
					 __FILE__, ((long) __LINE__) - 2L,
					 "my_argv[my_argc]", my_argc);
  strcpy (my_argv[my_argc++], gcal_prgr);
  /*
     Now construct and copy the actual date modifier %DATE into `my_argv[]'
     table at position 1.
   */
  sprintf (s1, "%c%0*d%02d%02d", RC_ADATE_CHAR, len_year_max, year, month,
	   day);
  my_argv[my_argc] =
    (char *) my_malloc (strlen (s1) + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
			((long) __LINE__) - 1L, "my_argv[my_argc]", my_argc);
  strcpy (my_argv[my_argc++], s1);
  /*
     Skip the locally respected `--shift=[+|-]NUMBER' long-style option
     if given in the command line.
   */
  if (shift_value_set)
    {
      argc--;
      argv++;
    }
  /*
     Now copy all other command line arguments which are delivered to
     the Gcal executable into `my_argv[]' table at position `my_argc'.
   */
  while (argc > 1)
    {
      argv++;
      if ((Uint) my_argc >= my_argc_max)
	{
	  /*
	     Resize the `my_argv[]' table.
	   */
	  my_argc_max <<= 1;
	  if (my_argc_max * sizeof (char *) > testval)
	    my_argc_max--;
	  my_argv = (char **) my_realloc ((VOID_PTR) my_argv,
					  my_argc_max * sizeof (char *),
					  ERR_NO_MEMORY_AVAILABLE,
					  __FILE__, ((long) __LINE__) - 3L,
					  "my_argv[my_argc_max]",
					  my_argc_max);
	}
      my_argv[my_argc] = (char *) my_malloc (strlen (*argv) + 1,
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 2L,
					     "my_argv[my_argc]", my_argc);
      strcpy (my_argv[my_argc++], *argv);
      argc--;
    }
  /*
     And terminate the `my_argv[]' table by a final NULL element.
   */
  if ((Uint) my_argc >= my_argc_max)
    {
      /*
         Resize the `my_argv[]' table.
       */
      my_argc_max <<= 1;
      if (my_argc_max * sizeof (char *) > testval)
	my_argc_max--;
      my_argv = (char **) my_realloc ((VOID_PTR) my_argv,
				      my_argc_max * sizeof (char *),
				      ERR_NO_MEMORY_AVAILABLE,
				      __FILE__, ((long) __LINE__) - 3L,
				      "my_argv[my_argc_max]", my_argc_max);
    }
  my_argv[my_argc] = (char *) NULL;
  /*
     Run the Gcal executable alltogether with the collected arguments.
   */
  status = execvp (gcal_prgr, my_argv);
  if (status == -1)
    {
      fprintf (stderr, _("%s: error during program execution of `%s'\n"),
	       prgr_name, gcal_prgr);
#if HAVE_ERRNO_H
      perror (gcal_prgr);
#endif
      exit (ERR_EXTERNAL_CMD_FAILURE);
    }

  exit (status);
}
