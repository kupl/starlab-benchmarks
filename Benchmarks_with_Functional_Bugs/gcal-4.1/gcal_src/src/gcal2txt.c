/*!
*  \file gcal2txt.c
*  \brief Creates a verbatim text file from a Gcal resource file.
*/
/*
*  Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013, 2014 Free Software Foundation, Inc.
*  Copyright (c) 1996, 1997, 2000 Thomas Esken
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


#include "tailor.h"
#if HAVE_ASSERT_H
# include <assert.h>
#endif
#if HAVE_CTYPE_H
# include <ctype.h>
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
#include "gcal2txt.h"



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
static char *decode_format __P_ ((FILE * fp,
				  Bool * is_eof,
				  int *flen, int *fwidth, int *ch));
__END_DECLARATIONS
/*
*  static variables definitions.
*/
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
  fprintf (fp, _("Usage:  %s  [--%s | --%s] | [RESOURCE-FILE | -]\n"),
	   prgr_name, help_option_name, version_option_name);
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



static char *
decode_format (fp, is_eof, flen, fwidth, ch)
     FILE *fp;
     Bool *is_eof;
     int *flen;
     int *fwidth;
     int *ch;
/*!
   Simplified check for an optional format instruction taken from `fp' stream
     which can either be used in case a TVAR or a `%...' special text is
     referenced. The template of such a format is:
     [ ALIGNMENT [SIGN] [LZERO] WIDTH [STYLE] [SUFFIX] FORMAT ],
     e.g., $[<|:|>[+][0]N[u|U|l|L|w|W][&]*|#]TVAR, resp.,
     %[<|:|>[+][0]N[u|U|l|L|w|W][&]*|#]?, like `$:+010u&#a' or `%>20l&*Y'.
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
   This function returns the scanned format instruction inclusive the TVAR
     resp., %... special text character, its length in `&flen' , and on
     success, a value not equal SPECIAL_VALUE in `&fwidth'.  `&fwidth' is set
     to SPECIAL_VALUE in case an error occurs during the scan of the format
     instruction.  `&is_eof' is set to TRUE if END_OF_FILE is detected during
     the scan, otherwise it is set to FALSE.  `&ch' contains the
     last character scanned.
*/
{
  static int the_len = 0;
  static char *format_txt;
  auto char *ptr_char;
  static Bool is_allocated = FALSE;


  *is_eof = FALSE;
  *flen = 0;
  *fwidth = SPECIAL_VALUE;
  /*
     Allocate the memory area of the returned `format_txt' string.
   */
  if (!is_allocated)
    {
      is_allocated = TRUE;
      /*
         Detect the number of digits of FWIDTH_MAX.
       */
      *ch = FWIDTH_MAX;
      while (*ch)
	{
	  the_len++;
	  *ch /= 10;
	}
      /*
         Initial memory allocation for the `format_txt' string, which is:
         ALIGNMENT + [SIGN] + [LZERO] + WIDTH   + [STYLE] + [SUFFIX] + FORMAT + CHAR + TERMINATOR
         1 + [1+]     [1+]    + the_len + [1+]    + [1+]       1      + 1    + 1
         ==> 7 + 1('\0' terminator) + the_len(FWIDTH_MAX==up to 3 digits)
         ==> 11 characters maximum.
       */
      format_txt = (char *) my_malloc (the_len + 8, ERR_NO_MEMORY_AVAILABLE,
				       __FILE__, ((long) __LINE__) - 1L,
				       "format_txt", 0);
    }
  ptr_char = format_txt;
  /*
     Start scanning the format instruction.
   */
  *ch = fgetc (fp);
  *ptr_char++ = (char) *ch;
  (*flen)++;
  switch (*ch)
    {
    case FLEFT_CHAR:
    case FCENTER_CHAR:
    case FRIGHT_CHAR:
      if ((*ch = fgetc (fp)) == EOF)
	{
	  *is_eof = TRUE;
	  *ptr_char = '\0';
	  return (format_txt);
	}
      break;
    default:
      *ptr_char = '\0';
      return (format_txt);
    }
  if (*ch == FSIGN_CHAR)
    {
      *ptr_char++ = (char) *ch;
      (*flen)++;
      if ((*ch = fgetc (fp)) == EOF)
	{
	  *is_eof = TRUE;
	  *ptr_char = '\0';
	  return (format_txt);
	}
    }
  if (*ch == FLZERO_CHAR)
    {
      *ptr_char++ = (char) *ch;
      (*flen)++;
      if ((*ch = fgetc (fp)) == EOF)
	{
	  *is_eof = TRUE;
	  *ptr_char = '\0';
	  return (format_txt);
	}
    }
  if (isdigit (*ch))
    {
      if (*ch != FLZERO_CHAR)
	{
	  register int len = the_len;


	  LOOP
	  {
	    if (isdigit (*ch))
	      {
		*ptr_char++ = (char) *ch;
		(*flen)++;
		if (!--len)
		  {
		    if ((*ch = fgetc (fp)) == EOF)
		      {
			*is_eof = TRUE;
			*ptr_char = '\0';
			return (format_txt);
		      }
		    break;
		  }
	      }
	    else
	      break;
	    if ((*ch = fgetc (fp)) == EOF)
	      {
		*is_eof = TRUE;
		*ptr_char = '\0';
		return (format_txt);
	      }
	  }
	  *fwidth = 0;
	  if (toupper (*ch) == toupper (FUPPER_CHAR)
	      || toupper (*ch) == toupper (FLOWER_CHAR)
	      || toupper (*ch) == toupper (FWORD_CHAR))
	    {
	      *ptr_char++ = (char) *ch;
	      (*flen)++;
	      if ((*ch = fgetc (fp)) == EOF)
		{
		  *is_eof = TRUE;
		  *fwidth = SPECIAL_VALUE;
		  *ptr_char = '\0';
		  return (format_txt);
		}
	    }
	  if (*ch == FSUFFIX_CHAR)
	    {
	      *ptr_char++ = (char) *ch;
	      (*flen)++;
	      if ((*ch = fgetc (fp)) == EOF)
		{
		  *is_eof = TRUE;
		  *fwidth = SPECIAL_VALUE;
		  *ptr_char = '\0';
		  return (format_txt);
		}
	    }
	  switch (*ch)
	    {
	    case FFIX_CHAR:
	    case FVAR_CHAR:
	      *ptr_char++ = (char) *ch;
	      (*flen)++;
	      if ((*ch = fgetc (fp)) == EOF)
		{
		  *is_eof = TRUE;
		  *fwidth = SPECIAL_VALUE;
		}
	      *ptr_char++ = (char) *ch;
	      (*flen)++;
	      break;
	    default:
	      *ptr_char++ = (char) *ch;
	      (*flen)++;
	      *fwidth = SPECIAL_VALUE;
	    }
	}
      else
	{
	  *ptr_char++ = (char) *ch;
	  (*flen)++;
	}
    }
  *ptr_char = '\0';

  return (format_txt);
}



int
main (argc, argv)
     int argc;
     char *argv[];
/*!
   Creates a verbatim text file from a Gcal resource file (optional argument 1)
     and displays the results on the STDOUT channel.  If no text file name or
     only a dash '-' is given, read input from STDIN channel.
*/
{
  auto FILE *fp = (FILE *) NULL;
  register int i;
  register int ch;
  register int ch2;
  register int ch3;
  auto int flen;
  auto int fwidth;
  auto int last_char = '\0';
  auto char *s1;
  auto char *ptr_char;
  auto Bool got_quote = FALSE;
  auto Bool had_quote = FALSE;
  auto Bool is_eof = FALSE;
  auto Bool is_regular_file = TRUE;


#ifdef GCAL_NLS
  /*
     Now initialize the NLS functions.
   */
# if HAVE_SETLOCALE
  setlocale (LC_ALL, "");
# endif
# ifndef LOCALEDIR
#  define LOCALEDIR  NULL
# endif
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
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
  assert (strlen (PACKAGE_VERSION) > 0);
#endif /* HAVE_ASSERT_H */
  /*
     Initial memory allocation for the `s1' string.
   */
  s1 = (char *) my_malloc (MAXLEN_MAX, ERR_NO_MEMORY_AVAILABLE,
			   __FILE__, ((long) __LINE__) - 1L, "s1", 0);
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
      auto char *buf_ptr_char = ptr_char;


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
  if (argc > 1)
    {
      if ((strlen (argv[1]) == 1) && (*argv[1] == *SWITCH))
	/*
	   STDIN channel explicitly wanted!
	 */
	fp = stdin;
      else
	{
	  /*
	     Check if `--help' or `--version' long-style option is given.
	   */
	  if ((strlen (argv[1]) > 2)
	      && (*argv[1] == *SWITCH) && (*(argv[1] + 1) == *SWITCH))
	    {
	      if (!strncasecmp
		  (argv[1] + 2, help_option_name, strlen (argv[1] + 2)))
		usage_msg (stdout, prgr_name, EXIT_SUCCESS);
	      if (!strncasecmp
		  (argv[1] + 2, version_option_name, strlen (argv[1] + 2)))
		version_msg (stdout, prgr_name, EXIT_SUCCESS);
	      /*
	         Error, unknown long-style option given.
	       */
	      fprintf (stderr, _("%s: unrecognized option `%s'"), prgr_name,
		       argv[1]);
	      S_NEWLINE (stderr);
	      usage_msg (stderr, prgr_name, ERR_INVALID_OPTION);
	    }
	  else
	    {
#if HAVE_SYS_STAT_H && defined(S_IFMT) && defined(S_IFREG)
	      auto struct stat statbuf;


	      /*
	         Test if the file is a regular file, if not, this is an error!
	       */
	      if (!stat (argv[1], &statbuf))
		{
		  if ((statbuf.st_mode & S_IFMT) == S_IFREG)
		    fp = fopen (argv[1], "r");
		  else
		    is_regular_file = FALSE;
		}
#else /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
	      fp = fopen (argv[1], "r");
#endif /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
	    }
	}
    }
  else
    fp = stdin;
  if (fp == (FILE *) NULL)
    {
      i = (int) strlen (prgr_name) + strlen (argv[1]) + 5;
      if ((Uint) i >= maxlen_max)
	s1 = (char *) my_realloc ((VOID_PTR) s1,
				  i + 1, ERR_NO_MEMORY_AVAILABLE,
				  __FILE__, ((long) __LINE__) - 2L, "s1", 0);
      sprintf (s1, "%s: `%s' ", prgr_name, argv[1]);
#if HAVE_ERRNO_H
      if (!is_regular_file)
	fprintf (stderr, _("%s: no regular file\n"), s1);
      else
	perror (s1);
      exit (EXIT_FAILURE);
#else /* !HAVE_ERRNO_H */
      fprintf (stderr, _("%s: file not found\n"), s1);
      exit (ERR_FILE_NOT_FOUND);
#endif /* !HAVE_ERRNO_H */
    }
  /*
     Skip the "date"-part of the line.
   */
  while ((ch = fgetc (fp)) != EOF)
    if (isspace (ch))
      {
	ch = fgetc (fp);
	if (ch != EOF)
	  {
	    if (ch != QUOTE_CHAR)
	      ungetc (ch, fp);
	    else
	      got_quote = TRUE;
	  }
	break;
      }
  /*
     Manage the "text"-part of the line.
   */
  if (ch != EOF)
    while ((ch = fgetc (fp)) != EOF)
      {
	switch (ch)
	  {
	  case RC_NL_CHAR:
	  case RC_NL2_CHAR:
	    ch2 = fgetc (fp);
	    if (ch2 != EOF)
	      switch (ch2)
		{
		case QUOTE_CHAR:
		  ch3 = fgetc (fp);
		  if (ch3 != EOF)
		    switch (ch3)
		      {
		      case '\n':
			fputc (ch3, stdout);
			break;
		      default:
			fprintf (stdout, "%c%c%c", ch, ch2, ch3);
		      }
		  else
		    fprintf (stdout, "%c%c", ch, ch2);
		  break;
		default:
		  fprintf (stdout, "%c%c", ch, ch2);
		}
	    else
	      fputc (ch, stdout);
	    had_quote = FALSE;
	    break;
	  case QUOTE_CHAR:
	    if (got_quote)
	      fputc (ch, stdout);
	    ch2 = fgetc (fp);
	    if (ch2 != EOF)
	      switch (ch2)
		{
		case RC_TVAR_CHAR:
		  ptr_char =
		    decode_format (fp, &is_eof, &flen, &fwidth, &last_char);
		  if (!is_eof)
		    {
		      ch3 = *(ptr_char + flen - 1);
		      if (isalpha (ch3)
			  && (fwidth != SPECIAL_VALUE || *ptr_char == ch3))
			{
			  if (had_quote)
			    fprintf (stdout, "%c%c%s", ch, ch2, ptr_char);
			  else
			    fprintf (stdout, "%c%s", ch2, ptr_char);
			}
		      had_quote = FALSE;
		      break;
		    }
		  else
		    fputc (ch, stdout);
		  /* Fallthrough. */
		case '\n':
		case RC_NL_CHAR:
		case RC_NL2_CHAR:
		  fputc (ch2, stdout);
		  had_quote = FALSE;
		  break;
		case RC_SPECIAL_TEXT_CHAR:
		  ptr_char =
		    decode_format (fp, &is_eof, &flen, &fwidth, &last_char);
		  if (!is_eof)
		    {
		      ch3 = *(ptr_char + flen - 1);
		      switch (ch3)
			{
			case RC_SHELL_ESC_CHAR:
			case RC_ENV_VAR_CHAR:
			case RC_EX_LHDY_CHAR:
			case RC_EX_NLHDY_CHAR:
			case RC_EX_AHDY_CHAR:
			case RC_EX_NAHDY_CHAR:
			case RC_EX_MON_CHAR:
			case RC_EX_NMON_CHAR:
			case RC_EX_TUE_CHAR:
			case RC_EX_NTUE_CHAR:
			case RC_EX_WED_CHAR:
			case RC_EX_NWED_CHAR:
			case RC_EX_THU_CHAR:
			case RC_EX_NTHU_CHAR:
			case RC_EX_FRI_CHAR:
			case RC_EX_NFRI_CHAR:
			case RC_EX_SAT_CHAR:
			case RC_EX_NSAT_CHAR:
			case RC_EX_SUN_CHAR:
			case RC_EX_NSUN_CHAR:
			case RC_EX_MON_2_THU_CHAR:
			case RC_EX_NMON_2_THU_CHAR:
			case RC_EX_MON_2_FRI_CHAR:
			case RC_EX_NMON_2_FRI_CHAR:
			case RC_IDATE_CHAR:
			case RC_EDATE_CHAR:
			case RC_WDNAME_CHAR:
			case RC_WDNR_M1_2_S7_CHAR:
			case RC_WDNR_M0_2_S6_CHAR:
			case RC_WDNR_S1_2_S7_CHAR:
			case RC_WDNR_S0_2_S6_CHAR:
			case RC_WDNR_X1_2_X7_CHAR:
			case RC_WDNR_X0_2_X6_CHAR:
			case RC_DOYNR_CHAR:
			case RC_DAYNR_CHAR:
			case RC_MONTHNAME_CHAR:
			case RC_MONTHNR_CHAR:
			case RC_YEARNR_CHAR:
			case RC_WEEKNR_CHAR:
			case RC_BYEAR_CHAR:
			case RC_MOON_CHAR:
			case RC_MOON_IMAGE_CHAR:
			case RC_BIO_CHAR:
			case RC_BIO_BAR_CHAR:
			case RC_TDATE_CHAR:
			case RC_JDAYS_CHAR:
			case RC_TIME_CHAR:
			case RC_TIME_TMI_CHAR:
			case RC_TIME_HR_CHAR:
			case RC_TIME_MI_CHAR:
			case RC_TIME_AMPM_CHAR:
			case RC_GMTIME_CHAR:
			case RC_GMTIME_TMI_CHAR:
			case RC_GMTIME_HR_CHAR:
			case RC_GMTIME_MI_CHAR:
			case RC_GMTIME_AMPM_CHAR:
			case RC_LT_ZONE_OFFS_CHAR:
			case RC_UT_ZONE_OFFS_CHAR:
			case RC_DISTANCE_CHAR:
			case RC_SUN_RISE_CHAR:
			case RC_SUN_SET_CHAR:
			case RC_SUN_DAY_CHAR:
			case RC_SUN_NIGHT_CHAR:
			case RC_MOON_RISE_CHAR:
			case RC_MOON_SET_CHAR:
			case RC_MOON_DAY_CHAR:
			case RC_MOON_NIGHT_CHAR:
			case RC_DAY_DIFF_CHAR:
			case RC_WEEK_DIFF_CHAR:
			case RC_MONTH_DIFF_CHAR:
			case RC_YEAR_DIFF_CHAR:
			case RC_HLS1S_CHAR:
			case RC_HLS1E_CHAR:
			case RC_HLS2S_CHAR:
			case RC_HLS2E_CHAR:
			case RC_HLS3S_CHAR:
			case RC_HLS3E_CHAR:
			case RC_HLS4S_CHAR:
			case RC_HLS4E_CHAR:
			case RC_HLS5S_CHAR:
			case RC_HLS5E_CHAR:
			  if (had_quote)
			    fprintf (stdout, "%c%c%s", ch, ch2, ptr_char);
			  else
			    fprintf (stdout, "%c%s", ch2, ptr_char);
			  break;
			default:
			  if (ch3 == RC_TVAR_CHAR
			      || ch3 == RC_SPECIAL_TEXT_CHAR
			      || ch3 == QUOTE_CHAR
			      || ch3 == RC_NL_CHAR || ch3 == RC_NL2_CHAR)
			    {
			      *(ptr_char + flen - 1) = '\0';
			      ungetc (ch3, fp);
			    }
			  fprintf (stdout, "%c%c%s", ch, ch2, ptr_char);
			}
		    }
		  else
		    fprintf (stdout, "%c%c", ch, ch2);
		  had_quote = FALSE;
		  break;
		default:
		  fputc (ch, stdout);
		  if (ch2 == QUOTE_CHAR)
		    {
		      if (had_quote)
			fputc (ch2, stdout);
		      ch3 = fgetc (fp);
		      if (ch3 != EOF)
			switch (ch3)
			  {
			  case QUOTE_CHAR:
			    fputc (ch3, stdout);
			    had_quote = TRUE;
			    break;
			  default:
			    had_quote = FALSE;
			    ungetc (ch3, fp);
			  }
		      else
			{
			  fputc (ch2, stdout);
			  had_quote = FALSE;
			}
		    }
		  else
		    {
		      ungetc (ch2, fp);
		      had_quote = FALSE;
		    }
		}
	    break;
	  default:
	    fputc (ch, stdout);
	    had_quote = FALSE;
	  }
	if (is_eof)
	  break;
	got_quote = FALSE;
      }
  (void) fclose (fp);

  exit (EXIT_SUCCESS);
}
