/*!
*  \file tty.c
*  \brief Screen support functions and major output function.
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
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#if USE_PAGER || USE_HLS
# if defined(UNIX) && !defined(DJG)
#  if HAVE_TERMIOS_H && HAVE_TERMIOS_FUNCS
#   include <termios.h>
#   if HAVE_SYS_IOCTL_H && !defined(TIOCGWINSZ)
#    include <sys/ioctl.h>
#   endif
#  else	/* !HAVE_TERMIOS_H || !HAVE_TERMIOS_FUNCS */
#   if HAVE_TERMIO_H
#    include <termio.h>
#   else /* !HAVE_TERMIO_H */
#    include <sgtty.h>
#    if HAVE_SYS_IOCTL_H && (defined(WIOCGETD) || defined(TIOCGWINSZ) || defined(TCGETA) || defined(TIOCGETP))
#     include <sys/ioctl.h>
#    endif
#   endif /* !HAVE_TERMIO_H */
#  endif /* !HAVE_TERMIOS_H || !HAVE_TERMIOS_FUNCS */
/*
*  For the Unix PC (ATT 7300 & 3B1):  (taken from less-278 by Mark Nudelman)
*  Since WIOCGETD is defined in sys/window.h, we can't use that to decide
*  whether to include sys/window.h.  Use SIGPHONE from sys/signal.h instead.
*/
#  ifndef TIOCGWINSZ
#   include <sys/signal.h>
#   ifdef SIGPHONE
#    include <sys/window.h>
#   endif
#  endif
#  if HAVE_SYS_STREAM_H
#   include <sys/stream.h>
#  endif
#  if HAVE_SYS_PTEM_H
#   include <sys/ptem.h>
#  endif
# endif	/* UNIX && !DJG */
# ifdef DJG
#  include <pc.h>
# else /* !DJG */
#  if defined(UNIX) || (defined(OS2) && defined(__GNUC__))
#   if HAVE_TERMCAP_H && HAVE_TTYLIBS
#    if HAVE_TERMIOS_H && HAVE_TERMIOS_FUNCS && defined(OS2) && defined(__GNUC__)
#     include <termios.h>
#    endif
#    include <termcap.h>
#   endif /* HAVE_TERMCAP_H && HAVE_TTYLIBS */
#  endif /* UNIX || (OS2 && __GNUC__) */
# endif	/* !DJG */
#endif /* USE_PAGER || USE_HLS */
#include "common.h"
#include "globals.h"
#include "utils.h"
#include "tty.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `tty.c'.
*/
#if defined(GCAL_TCAP) && USE_HLS
static char *skip_leading_padding_info __P_ ((char *sequence_str));
#endif
#if USE_HLS || USE_PAGER
# ifdef GCAL_TCAP
static Bool open_termcap __P_ ((void));
#  if USE_HLS
static void get_ospeed __P_ ((void));
static int outchar __P_ ((int ch));
static Bool get_termcap_hls __P_ ((Bool * hls1_set, Bool * hls2_set));
#  endif /* USE_HLS */
#  if USE_PAGER
static Bool get_termcap_scr_attrib __P_ ((int *rows, int *cols));
#  endif /* USE_PAGER */
# else /* !GCAL_TCAP */
#  if defined(MSDOS) && USE_PAGER
static Uchar peek_byte __P_ ((Uint segment, Uint offset));
#  endif
# endif	/* GCAL_TCAP */
#endif /* USE_HLS || USE_PAGER */
static void
  get_hl_seq __P_ ((const char *sequence_str,
		    Bool * hls1_set, Bool * hls2_set));
#if !HAVE_STRTOL
static int sbyte2int __P_ ((const char *string, const int base));
#endif
__END_DECLARATIONS
/*
*  static variables definitions.
*/
#if USE_PAGER || USE_HLS
# ifdef GCAL_TCAP
/*! Module global Termcap buffer. */
static char tc_buf[TC_BUFLEN];

/*! Termcap access error occurred. */
static Bool tc_no_error = TRUE;

#  if USE_HLS
/*! Module global file which is used by the `tputs()' function. */
static FILE *fp_outchar = (FILE *) NULL;

/*! Stores whether padding is used or not. */
static Bool is_padding = FALSE;
#  endif
# endif	/* GCAL_TCAP */

# if USE_PAGER
/*! Terminal has automatic margins. */
static Bool tty_am = TRUE;

/*! Terminal ignores newline after wrap. */
static Bool tty_xn = FALSE;
# endif	/* USE_PAGER */
#endif /* USE_PAGER || USE_HLS */



/*
*  Function implementations.
*/
void
print_text (fp, text_line)
     FILE *fp;
     char *text_line;
/*!
   This is the central tty output function, which works according to
     actual display mode.  It prints a line of text given in `text_line'
     with newline to file `fp' with paging option (very poor and simple
     paging, only used if preprocessor symbol USE_PAGER is defined) and
     ALWAYS "deletes" the delivered `text_line' automatically after printing
     (*text_line = '\0').
*/
{
#if USE_PAGER || (defined(GCAL_TCAP) && USE_HLS)
  if (is_tty
# if USE_PAGER && (!defined(GCAL_TCAP) || !USE_HLS)
      && pager_flag
# endif
      && is_tty1 && is_tty2)
    {
      register int hls_pos = SPECIAL_VALUE;
      register int hls1_pos = SPECIAL_VALUE;
      register int hls2_pos = SPECIAL_VALUE;
      register int hls_chars = 0;
      register int i = 0;
      register int nl;
      register int j;
# if USE_PAGER
      register int k;
      register int final_nl = 0;
      static int lines_printed = 0;
# endif
      auto char *ptr_1hls = (char *) NULL;
      auto char *ptr_2hls = (char *) NULL;
      auto Bool hls_start;
# if USE_PAGER
      auto Bool print_hls = (Bool) (is_tty && highlight_flag && !emu_hls);
# endif
      auto Bool nl_found = FALSE;
      auto Bool buf_highlight_flag = highlight_flag;


# if defined(GCAL_TCAP) && USE_HLS
      fp_outchar = fp;
# endif
      LOOP
      {
# if USE_PAGER
	if (final_nl != SPECIAL_VALUE)
	  final_nl = 0;
# endif
	hls_chars = nl = j = 0;
	if (highlight_flag && !emu_hls)
	  {
	    /*
	       Try to detect a highlighting sequence,
	       store its position and point to it...
	     */
	    hls_pos = hls1_pos = hls2_pos = SPECIAL_VALUE;
	    if (*(text_line + i))
	      {
		if (tty_cols != SPECIAL_VALUE)
		  {
		    ptr_1hls = strstr (text_line + i, ehls1s.seq);
		    if (ptr_1hls != (char *) NULL)
		      hls1_pos = i + (int) (ptr_1hls - (text_line + i));
		    ptr_2hls = strstr (text_line + i, ehls2s.seq);
		    if (ptr_2hls != (char *) NULL)
		      hls2_pos = i + (int) (ptr_2hls - (text_line + i));
		  }
		if ((hls1_pos != SPECIAL_VALUE)
		    && (hls2_pos != SPECIAL_VALUE))
		  {
		    if (hls1_pos > hls2_pos)
		      hls_pos = hls2_pos;
		    else
		      hls_pos = hls1_pos;
		  }
		else if (hls1_pos != SPECIAL_VALUE)
		  hls_pos = hls1_pos;
		else if (hls2_pos != SPECIAL_VALUE)
		  hls_pos = hls2_pos;
	      }
	    if (hls_pos == SPECIAL_VALUE)
	      highlight_flag = FALSE;
	  }
	/*
	   No real highlighting sequence found AND
	   either: no '\n'-NEWLINE character found
	   or: no "pager_flag" given
	   or: "pager_flag" given but we use an external paging program
	   --> Let's print the complete line as a whole!
	 */
	if ((strchr (text_line + i, '\n') == (char *) NULL
# if USE_PAGER
	     || !pager_flag
#  ifdef GCAL_EPAGER
	     || (pager_flag && (ext_pager != (char *) NULL))
#  endif
# endif
	    ) && (hls_pos == SPECIAL_VALUE))
	  {
	    fprintf (fp, "%s\n", text_line + i);
	    i = (int) strlen (text_line);
	  }
	else
	  {
	    /*
	       Otherwise we have to inspect each character of the line
	       whether it's part of a real highlighting sequence or not
	       (because of managing a potential padding) and to count
	       the NEWLINES so we are able to paginate the output internally.
	     */
	    hls_start = TRUE;
	    while (*(text_line + i))
	      {
		/*
		   `\n'-NEWLINE character found (actual line must be a RC
		   fixed date => TILDE-character expansion was performed):
		   Print this NEWLINE character.
		 */
		if (*(text_line + i) == '\n')
		  {
		    i++;
		    if (j)
		      nl++;
		    nl_found = TRUE;
# if USE_PAGER
		    if (final_nl != SPECIAL_VALUE)
		      final_nl = 1;
# endif
		    break;
		  }
		/*
		   Look for a real highlighting sequence;
		   if found, store its length in `hls_chars'
		   and print it explicitly by means of tputs().
		 */
		if (highlight_flag && !emu_hls && (i == hls_pos))
		  {
		    if (hls_pos == hls1_pos)
		      {
			if (hls_start)
			  {
# if defined(GCAL_TCAP) && USE_HLS
			    tputs ((char *) ehls1s.seq, 1, outchar);
# else /* !GCAL_TCAP || !USE_HLS */
			    fputs (ehls1s.seq, fp);
# endif	/* !GCAL_TCAP || !USE_HLS */
			    hls_chars += ehls1s.len;
			    i += ehls1s.len;
			    if (tty_cols != SPECIAL_VALUE)
			      ptr_1hls = strstr (text_line + i, ehls1e.seq);
			    if (ptr_1hls != (char *) NULL)
			      hls_pos = hls1_pos =
				i + (int) (ptr_1hls - (text_line + i));
			    else
			      hls_pos = hls2_pos;
			    hls_start = FALSE;
			  }
			else
			  {
# if defined(GCAL_TCAP) && USE_HLS
			    tputs ((char *) ehls1e.seq, 1, outchar);
# else /* !GCAL_TCAP || !USE_HLS */
			    fputs (ehls1e.seq, fp);
# endif	/* !GCAL_TCAP || !USE_HLS */
			    hls_chars += ehls1e.len;
			    i += ehls1e.len;
			    if (tty_cols != SPECIAL_VALUE)
			      ptr_1hls = strstr (text_line + i, ehls1s.seq);
			    if (ptr_1hls != (char *) NULL)
			      hls_pos = hls1_pos =
				i + (int) (ptr_1hls - (text_line + i));
			    else
			      hls_pos = hls2_pos;
			    hls_start = TRUE;
			  }
			if (*(text_line + i))
			  fputc (*(text_line + i), fp);
			else
			  break;
		      }
		    else if (hls_pos == hls2_pos)
		      {
			if (hls_start)
			  {
# if defined(GCAL_TCAP) && USE_HLS
			    tputs ((char *) ehls2s.seq, 1, outchar);
# else /* !GCAL_TCAP || !USE_HLS */
			    fputs (ehls2s.seq, fp);
# endif	/* !GCAL_TCAP || !USE_HLS */
			    hls_chars += ehls2s.len;
			    i += ehls2s.len;
			    if (tty_cols != SPECIAL_VALUE)
			      ptr_2hls = strstr (text_line + i, ehls2e.seq);
			    if (ptr_2hls != (char *) NULL)
			      hls_pos = hls2_pos =
				i + (int) (ptr_2hls - (text_line + i));
			    else
			      hls_pos = hls1_pos;
			    hls_start = FALSE;
			  }
			else
			  {
# if defined(GCAL_TCAP) && USE_HLS
			    tputs ((char *) ehls2e.seq, 1, outchar);
# else /* !GCAL_TCAP || !USE_HLS */
			    fputs (ehls2e.seq, fp);
# endif	/* !GCAL_TCAP || !USE_HLS */
			    hls_chars += ehls2e.len;
			    i += ehls2e.len;
			    if (tty_cols != SPECIAL_VALUE)
			      ptr_2hls = strstr (text_line + i, ehls2s.seq);
			    if (ptr_2hls != (char *) NULL)
			      hls_pos = hls2_pos =
				i + (int) (ptr_2hls - (text_line + i));
			    else
			      hls_pos = hls1_pos;
			    hls_start = TRUE;
			  }
			if (*(text_line + i))
			  fputc (*(text_line + i), fp);
			else
			  break;
		      }
		  }
		else
		  /*
		     Otherwise, print the actual character in the string.
		   */
		  fputc (*(text_line + i), fp);
		j++;
		i++;
	      }
	    S_NEWLINE (fp);
	  }
# if USE_PAGER
	if (pager_flag
#  ifdef GCAL_EPAGER
	    && (ext_pager == (char *) NULL)
#  endif
	  )
	  {
	    /*
	       Must we prompt the user?
	     */
	    if (!tty_am)
	      j = 0;
	    else
	      {
		k = ((nl_found) ? j : i) - nl - hls_chars;
		j = k / tty_cols;
		if (j && tty_xn && !(k % tty_cols))
		  j--;
	      }
	    if (lines_printed + j >= tty_rows - 1)
	      {
		if (print_hls)
#  if defined(GCAL_TCAP) && USE_HLS
		  tputs ((char *) ehls1s.seq, 1, outchar);
#  else	/* !GCAL_TCAP || !USE_HLS */
		  fputs (ehls1s.seq, fp);
#  endif /* !GCAL_TCAP || !USE_HLS */
		fprintf (fp, _("%s: <Return> for more, <%s> to quit..."),
			 prgr_name, PAGER_QUIT);
		if (print_hls)
#  if defined(GCAL_TCAP) && USE_HLS
		  tputs ((char *) ehls1e.seq, 1, outchar);
#  else	/* !GCAL_TCAP || !USE_HLS */
		  fputs (ehls1e.seq, fp);
#  endif /* !GCAL_TCAP || !USE_HLS */
		k = fgetc (stdin);
		/*
		   Quit the pager by the quit "key" command.
		 */
		if (tolower (k) == (int) *PAGER_QUIT)
		  {
		    /*
		       In case a leading "quit" character and other text is in the STDIN buffer:
		       Clean the whole buffer.
		     */
		    while (((k = fgetc (stdin)) != '\n') && (k != EOF))
		      ;		/* Void */
		    k = EOF;
		  }
		else
		  /*
		     In case a "quit" character is not leading (anywhere) in the STDIN buffer:
		     Clean the buffer until the "quit" character isn't found.
		   */
		if (k != '\n')
		  while (((k = fgetc (stdin)) != '\n')
			 && (tolower (k) != (int) *PAGER_QUIT) && (k != EOF))
		    ;		/* Void */
		/*
		   In case the "quit" character is found in the STDIN buffer now:
		   Clean up the rest of buffer (until its end).
		 */
		if (tolower (k) == (int) *PAGER_QUIT)
		  {
		    while (((k = fgetc (stdin)) != '\n') && (k != EOF))
		      ;		/* Void */
		    k = EOF;
		  }
		/*
		   Exit the program with success if the PAGER_QUIT character
		   was found in the STDIN buffer.
		 */
		if (k == EOF)
		  my_exit (EXIT_SUCCESS);
		/*
		   Begin scrolling of the next page.
		 */
		lines_printed = 0;
	      }
	    else
	      lines_printed += (j + 1);
	    if (!*(text_line + i))
	      {
		if (final_nl && (final_nl != SPECIAL_VALUE))
		  {
		    /*
		       Respect an implicit NEWLINE character in case
		       a TILDE-expansion was performed at the end of a line!
		     */
		    final_nl = SPECIAL_VALUE;
		    i--;
		  }
		else
		  break;
	      }
	  }
	else
# endif	/* USE_PAGER */
	if (!*(text_line + i))
	  break;
      }
      highlight_flag = buf_highlight_flag;
    }
  else
#endif /* USE_PAGER || (GCAL_TCAP && USE_HLS) */
    {
      /*
         If the mailing option is selected,
         print the output to the temporary file.
       */
      if ((fp != (FILE *) stderr)
#ifdef GCAL_EMAIL
	  && (tfp != (FILE *) NULL)
#endif
	)
	fprintf (tfp, "%s\n", text_line);
      else
	fprintf (fp, "%s\n", text_line);
    }
  *text_line = '\0';
}



void
get_tty_hls (sequence_str)
     const char *sequence_str;
/*!
   Reads the colors/highlighting sequences from Termcap and assigns them
     to the according variables.  If Termcap isn't present, defaults are used.
*/
{
#if USE_HLS
# if !defined(AMIGA) || defined(__GNUC__)
  auto char *ptr_env = getenv (ENV_VAR_GCALANSI);
# else /* AMIGA && !__GNUC__ */
  auto char *ptr_env = (char *) NULL;
# endif	/* AMIGA && !__GNUC__ */
# ifdef GCAL_TCAP
  auto Bool check_again = FALSE;
# endif
#endif
  auto Bool hls1_set = FALSE;
  auto Bool hls2_set = FALSE;


  /*
     Check whether highlighting must be disabled
     (`-H<no>' given in the command line).
   */
  if (!highlight_flag)
    {
      emu_hls = TRUE;
      ehls1s.seq = NO_HLS;
      ehls1e.seq = NO_HLS;
      ehls2s.seq = NO_HLS;
      ehls2e.seq = NO_HLS;
    }
  else
    {
      /*
         If output is not directed to a tty,
         emulate the highlighting sequences by using marking characters.
       */
      if (!is_tty && !emu_hls && highlight_flag)
	emu_hls = TRUE;
      /*
         Check whether user defined highlighting sequences are given
         in the command line (`-H<LIST_OF_HL_SEQUENCES>').
       */
      if (sequence_str != (char *) NULL)
	{
	  get_hl_seq (sequence_str, &hls1_set, &hls2_set);
#if USE_HLS && defined(GCAL_TCAP)
	  if ((hls1_set
	       && (ehls1s.len > 1
		   || ehls1e.len > 1))
	      || (hls2_set && (ehls2s.len > 1 || ehls2e.len > 1)))
	    check_again = TRUE;
#endif
	}
      /*
         No or partitial highlighting sequences are given in command line
         -> Complete them (or if a padding was given, init the necessary
         functions for managing it).
       */
      if (!hls1_set
#if USE_HLS && defined(GCAL_TCAP)
	  || check_again
#endif
	  || !hls2_set)
	{
#if USE_HLS
# ifdef GCAL_TCAP
	  if (!emu_hls)
	    {
	      /*
	         Try to open the `[/etc/]termcap' file.
	       */
	      tc_no_error = open_termcap ();
	      if (tc_no_error)
		get_termcap_hls (&hls1_set, &hls2_set);
	      if (check_again)
		{
		  get_hl_seq (sequence_str, &hls1_set, &hls2_set);
		  if (tc_no_error)
		    get_termcap_hls (&hls1_set, &hls2_set);
		}
	      if (ptr_env != (char *) NULL)
		{
		  /*
		     Some or all Termcap highlighting sequences are missing:
		     Use according "default" ANSI highlighting sequences
		     if the environment variable $GCALANSI is set.
		   */
		  if (!hls1_set)
		    {
		      ehls1s.seq = HLS1S;
		      ehls1e.seq = HLS1E;
		    }
		  if (!hls2_set)
		    {
		      ehls2s.seq = HLS2S;
		      ehls2e.seq = HLS2E;
		    }
		}
	      else if (!hls1_set || !hls2_set)
		{
		  /*
		     Some or all Termcap highlighting sequences are missing:
		     Emulate ALL highlighting sequences by marking character
		     pairs if the environment variable $GCALANSI isn't set.
		   */
		  emu_hls = TRUE;
		  ehls1s.seq = BUF_HLS1S;
		  ehls1e.seq = BUF_HLS1E;
		  ehls2s.seq = BUF_HLS2S;
		  ehls2e.seq = BUF_HLS2E;
		}
	    }
	  else
# endif	/* GCAL_TCAP */
	    {
	      if (emu_hls)
		{
		  /*
		     Use emulation of highlighting sequences
		     in case output is not directed to a tty and the
		     highlighting sequences are not explicit disabled
		     in the command line by `-H<no>'.
		   */
		  if (!hls1_set)
		    {
		      ehls1s.seq = BUF_HLS1S;
		      ehls1e.seq = BUF_HLS1E;
		    }
		  if (!hls2_set)
		    {
		      ehls2s.seq = BUF_HLS2S;
		      ehls2e.seq = BUF_HLS2E;
		    }
		}
	      else
		{
		  /*
		     Use highlighting sequences directly in all other cases.
		   */
		  if (ptr_env != (char *) NULL)
		    {
		      /*
		         If environment variable $GCALANSI is set:
		         Use the according "default" ANSI highlighting sequences.
		       */
		      if (!hls1_set)
			{
			  ehls1s.seq = HLS1S;
			  ehls1e.seq = HLS1E;
			}
		      if (!hls2_set)
			{
			  ehls2s.seq = HLS2S;
			  ehls2e.seq = HLS2E;
			}
		    }
		  else if (!hls1_set || !hls2_set)
		    {
		      /*
		         If environment variable $GCALANSI is not set:
		         Emulate ALL highlighting sequences by marking character pairs.
		       */
		      emu_hls = TRUE;
		      ehls1s.seq = BUF_HLS1S;
		      ehls1e.seq = BUF_HLS1E;
		      ehls2s.seq = BUF_HLS2S;
		      ehls2e.seq = BUF_HLS2E;
		    }
		}
	    }
#else /* !USE_HLS */
	  /*
	     Use the default highlighting sequences.
	   */
	  if (!hls1_set)
	    {
	      ehls1s.seq = HLS1S;
	      ehls1e.seq = HLS1E;
	    }
	  if (!hls2_set)
	    {
	      ehls2s.seq = HLS2S;
	      ehls2e.seq = HLS2E;
	    }
#endif /* !USE_HLS */
	}
    }
  /*
     Detect and store length of the highlighting sequences.
   */
#if defined(GCAL_TCAP) && USE_HLS
  /*
     If padding is specified in Termcap and output is not connected to a tty,
     avoid to emit the padding information itself in the output, because
     it's senseless to redirect/pipe such an information to the consumser,
     who doesn't interpret it!
   */
  if (is_padding && (!is_tty || !is_tty1 || !is_tty2))
    {
      ehls1s.seq =
	(const char *) skip_leading_padding_info ((char *) ehls1s.seq);
      ehls1e.seq =
	(const char *) skip_leading_padding_info ((char *) ehls1e.seq);
      ehls2s.seq =
	(const char *) skip_leading_padding_info ((char *) ehls2s.seq);
      ehls2e.seq =
	(const char *) skip_leading_padding_info ((char *) ehls2e.seq);
    }
#endif /* !GCAL_TCAP || !USE_HLS */
  ehls1s.len = (int) strlen (ehls1s.seq);
  ehls1e.len = (int) strlen (ehls1e.seq);
  ehls2s.len = (int) strlen (ehls2s.seq);
  ehls2e.len = (int) strlen (ehls2e.seq);
}



#if USE_PAGER
void
get_tty_scr_size (rows, cols)
     int *rows;
     int *cols;
/*!
   Detects the number of rows and columns of a tty
     and stores the values found in `&rows' and `&cols'.
*/
{
# if !defined(AMIGA) || defined(__GNUC__)
  register int li = 0;
  register int co = 0;
  auto char *ptr_env;


  /*
     First look into the environment variable pair $GCAL_LINES and
     $GCAL_COLUMNS resp., $LINES and $COLUMNS in case these are defined
     and have valid settings: Use these settings.
   */
  ptr_env = getenv (ENV_VAR_LI);
  if (ptr_env != (char *) NULL)
    if (*ptr_env)
      {
	li = my_atoi (ptr_env);
	ptr_env = getenv (ENV_VAR_CO);
	if (ptr_env != (char *) NULL)
	  if (*ptr_env)
	    co = my_atoi (ptr_env);
      }
  if ((li > 0) && (co > 0))
    {
      *rows = li;
      *cols = co;
    }
  else
    {
      ptr_env = getenv (ENV_VAR_LI2);
      if (ptr_env != (char *) NULL)
	if (*ptr_env)
	  {
	    li = my_atoi (ptr_env);
	    ptr_env = getenv (ENV_VAR_CO2);
	    if (ptr_env != (char *) NULL)
	      if (*ptr_env)
		co = my_atoi (ptr_env);
	  }
      if ((li > 0) && (co > 0))
	{
	  *rows = li;
	  *cols = co;
	}
      else
	{
#  if defined(OS2) && defined(__GNUC__)
	  auto int info[2];
#  endif /* OS2 && __GNUC__ */
#  if defined(UNIX) && !defined(DJG)
#   ifdef TIOCGWINSZ
	  auto struct winsize wsz;
#   else /* !TIOCGWINSZ */
#    ifdef WIOCGETD
	  auto struct uwdata wsz;
#    endif
#   endif /* !TIOCGWINSZ */
#  endif /* UNIX && !DJG */


#  if !defined(DJG) && !defined(MSDOS) && !defined(OS2) && !defined(UNIX)
	  /*
	     For these machines: Defaults only!
	   */
	  *rows = SCREEN_ROWS;
	  *cols = SCREEN_COLS;
#  else	/* DJG || MSDOS || OS2 || UNIX */
#   ifdef DJG
	  /*
	     Get the actual number of lines and columns of the video by
	     using the DJGPP-GCC `ScreenRows()' and `ScreenCols()' functions.
	   */
	  *rows = ScreenRows ();
	  *cols = ScreenCols ();
#   else /* !DJG */
#    if defined(MSDOS)
	  /*
	     Look directly into the PC-BIOS and get the actual number
	     of lines and columns of the video.
	   */
	  *rows = peek_byte (0x40, 0x84) + 1;
	  /*
	     Get lower part of 2-byte word.
	   */
	  *cols = peek_byte (0x40, 0x4b);
	  *cols <<= 0x08;
	  /*
	     Add higher part of 2-byte word.
	   */
	  *cols += peek_byte (0x40, 0x4a);
#    else /* !MSDOS */
#     if defined(OS2) && defined(__GNUC__)
	  /*
	     Get the actual number of lines and columns of the
	     video by using the EMX-GCC `_scrsize()' function.
	   */
	  _scrsize (info);
	  *cols = s1[0];
	  *rows = s1[1];
#     else /* !OS2 || !__GNUC__ */
#      if defined(UNIX)
	  /*
	     Get the actual number of lines and columns of the
	     video by using the `ioctl()' function.
	   */
#       ifdef TIOCGWINSZ
	  if (!ioctl (1, TIOCGWINSZ, &wsz) && (wsz.ws_row > 0))
	    {
	      *rows = wsz.ws_row;
	      if (!ioctl (1, TIOCGWINSZ, &wsz) && (wsz.ws_col > 0))
		*cols = wsz.ws_col;
	      else
		*rows = SPECIAL_VALUE;
	    }
#       else /* !TIOCGWINSZ */
#        ifdef WIOCGETD
	  if (!ioctl (1, WIOCGETD, &wsz) && (wsz.uw_height > 0))
	    {
	      *rows = wsz.uw_height / wsz.uw_vs;
	      if (!ioctl (1, WIOCGETD, &wsz) && (wsz.uw_width > 0))
		*cols = wsz.uw_width / wsz.uw_hs;
	      else
		*rows = SPECIAL_VALUE;
	    }
#        endif
#       endif /* !TIOCGWINSZ */
#      endif /* UNIX */
#     endif /* !OS2 || !__GNUC__ */
#    endif /* !MSDOS */
#   endif /* !DJG */
	  if ((*rows == SPECIAL_VALUE) && (*cols == SPECIAL_VALUE))
	    {
#   if HAVE_TTYLIBS
	      /*
	         If the previous actions have failed,
	         try to open the `[/etc/]termcap' file.
	       */
	      tc_no_error = open_termcap ();
	      if (tc_no_error)
		{
		  if (!get_termcap_scr_attrib (rows, cols))
		    {
		      /*
		         No valid Termcap entries: Defaults only!
		       */
		      *rows = SCREEN_ROWS;
		      *cols = SCREEN_COLS;
		    }
		}
	      else
		{
		  /*
		     Access to `[/etc/]termcap' file has failed: Defaults only!
		   */
		  *rows = SCREEN_ROWS;
		  *cols = SCREEN_COLS;
		}
#   else /* !HAVE_TTYLIBS */
	      /*
	         No `[/etc/]termcap' file available: Defaults only!
	       */
	      *rows = SCREEN_ROWS;
	      *cols = SCREEN_COLS;
#   endif /* !HAVE_TTYLIBS */
	    }
#  endif /* DJG || MSDOS || OS2 || UNIX */
	}
    }
# else /* AMIGA && !__GNUC__ */
#  ifdef AMIGA
  /*
     Amiga gets the window size by asking the `console.device'.
   */
  {
    auto long len;
    auto char buf[30];


    Write (Output (), "\2330 q", 4);
    len = Read (Input (), buf, 29);
    buf[len] = '\0';
    sscanf (&buf[5], "%d;%d", rows, cols);
  }
#  else	/* !AMIGA */
  /*
     All other systems: Defaults only.
   */
  *rows = SCREEN_ROWS;
  *cols = SCREEN_COLS;
#  endif /* !AMIGA */
# endif	/* AMIGA && !__GNUC__ */
  if (*rows > 1)
    (*rows)--;
}
#endif /* USE_PAGER */



#if defined(GCAL_TCAP) && USE_HLS
static char *
skip_leading_padding_info (sequence_str)
     char *sequence_str;
/*!
   Skips the leading padding information part of a highlighting sequence.
*/
{
  if (isdigit (*sequence_str))
    {
      is_padding = TRUE;
      while (isdigit (*sequence_str))
	sequence_str++;
      if (*sequence_str == '.')
	{
	  sequence_str++;
	  if (isdigit (*sequence_str))
	    sequence_str++;
	}
      if (*sequence_str == '*')
	sequence_str++;
    }

  return (sequence_str);
}
#endif /* GCAL_TCAP && USE_HLS */



#if USE_PAGER || USE_HLS
# ifdef GCAL_TCAP
static Bool
open_termcap ()
/*!
   Tries to open the Termcap library and returns the terminal entry found
     in the module global vector `tc_buf[]'.  I cannot rely that we use the
     access functions of the GNU Termcap library --- which allows to pass
     a NULL pointer to `tgetent()' --- so this function can check itself
     how large `tc_buf[]' must be and allocates it automatically.  So I set
     `tc_buf' to a size of 4096 (TC_BUFLEN) bytes hoping that this will be
     enough for save program operation...
     May be called only once.
     Returns FALSE if an error occurs, otherwise TRUE.
*/
{
#  if defined(OS2) && defined(__GNUC__)
  auto char *ptr_env = getenv (ENV_VAR_TCAP);
  auto char *ptr_tc;
#  endif /* OS2 && __GNUC__ */
  auto char *term = getenv (ENV_VAR_TERM);
  static Bool func_accessed = FALSE;
  auto Bool is_error = FALSE;


  if (!func_accessed)
    {
      func_accessed = TRUE;
#  if defined(OS2) && defined(__GNUC__)
      /*
         Under OS/2 with GNU-C, we use the default terminal type (ANSI)
         and access the Termcap library instead of printing an informational
         message and using burned-in defaults if the $TERM environment
         variable isn't set.
       */
      if (term == (char *) NULL || !*term)
	term = DFLT_TERM;
#  else	/* !OS2 || !__GNUC__ */
      if (term == (char *) NULL)
	{
	  if (warning_level >= 0)
	    sprintf (s1, _("environment variable `%s' not found"),
		     ENV_VAR_TERM);
	  is_error = TRUE;
	}
      else if (!*term)
	{
	  if (warning_level >= 0)
	    sprintf (s1, _("environment variable `%s' not set"),
		     ENV_VAR_TERM);
	  is_error = TRUE;
	}
      else
#  endif /* !OS2 || !__GNUC__ */
	{
#  if defined(OS2) && defined(__GNUC__)
	  /*
	     Ensure the Termcap database is available,
	     i.e. store its access path in the environment explicitly
	     so we are able to refer it.
	   */
	  if (ptr_env == NULL || !*ptr_env)
	    {
	      ptr_env = (char *) my_malloc (256, ERR_NO_MEMORY_AVAILABLE,
					    __FILE__, ((long) __LINE__) - 1L,
					    "ptr_env", 0);
	      _searchenv (FNAME_TCAP, "INIT", ptr_env);
	      if (!*ptr_env)
		_searchenv (FNAME_TCAP, ENV_VAR_PATH, ptr_env);
	      if (!*ptr_env)
		_searchenv (FNAME_TCAP, ENV_VAR_GCALPATH, ptr_env);
	      if (*ptr_env)
		{
		  ptr_tc = (char *) my_malloc (strlen (ptr_env) + 9,
					       ERR_NO_MEMORY_AVAILABLE,
					       __FILE__,
					       ((long) __LINE__) - 2L,
					       "ptr_tc", 0);
		  sprintf (ptr_tc, "%s=%s", ENV_VAR_TCAP, ptr_env);
		  putenv (ptr_tc);
		}
	      free (ptr_env);
	    }
#  endif /* OS2 && __GNUC__ */
	  switch (tgetent (tc_buf, term))
	    {
	    case -1:
	      if (warning_level >= 0)
		strcpy (s1, _("`termcap' file not found"));
	      is_error = TRUE;
	      break;
	    case 0:
	      if (warning_level >= 0)
		sprintf (s1, _("unknown terminal type defined in `%s'"),
			 ENV_VAR_TERM);
	      is_error = TRUE;
	      break;
	    default:
	      ;			/* Void, Termcap access ok */
	    }
	}
      if (is_error && (warning_level >= 0))
	fprintf (stderr, _("\n%s: warning, %s.\n\n"), prgr_name, s1);
      return ((Bool) ! is_error);
    }

  return (tc_no_error);
}



#  if USE_HLS
static void
get_ospeed ()
/*!
   Tries to detect the terminal speed and store its value to
     Termcap's global `ospeed' variable.
*/
{
#   if HAVE_TERMIOS_H && HAVE_TERMIOS_FUNCS
  auto struct termios buf;


  /*
     Get the terminal mode.
   */
  tcgetattr (1, &buf);
  /*
     Get ospeed!
   */
#    if HAVE_OSPEED
  switch (cfgetospeed (&buf))
    {
#     ifdef B0
    case B0:
      ospeed = 0;
      break;
#     endif
#     ifdef B50
    case B50:
      ospeed = 1;
      break;
#     endif
#     ifdef B75
    case B75:
      ospeed = 2;
      break;
#     endif
#     ifdef B110
    case B110:
      ospeed = 3;
      break;
#     endif
#     ifdef B134
    case B134:
      ospeed = 4;
      break;
#     endif
#     ifdef B150
    case B150:
      ospeed = 5;
      break;
#     endif
#     ifdef B200
    case B200:
      ospeed = 6;
      break;
#     endif
#     ifdef B300
    case B300:
      ospeed = 7;
      break;
#     endif
#     ifdef B600
    case B600:
      ospeed = 8;
      break;
#     endif
#     ifdef B1200
    case B1200:
      ospeed = 9;
      break;
#     endif
#     ifdef B1800
    case B1800:
      ospeed = 10;
      break;
#     endif
#     ifdef B2400
    case B2400:
      ospeed = 11;
      break;
#     endif
#     ifdef B4800
    case B4800:
      ospeed = 12;
      break;
#     endif
#     ifdef B9600
    case B9600:
      ospeed = 13;
      break;
#     endif
#     ifdef EXTA
    case EXTA:
      ospeed = 14;
      break;
#     endif
#     ifdef EXTB
    case EXTB:
      ospeed = 15;
      break;
#     endif
#     ifdef B57600
    case B57600:
      ospeed = 16;
      break;
#     endif
#     ifdef B115200
    case B115200:
      ospeed = 17;
      break;
#     endif
    default:
      ;				/* Void */
    }
#    endif /* HAVE_OSPEED */
#   else /* !HAVE_TERMIOS_H || !HAVE_TERMIOS_FUNC */
#    if TCGETA
  auto struct termio buf;


  /*
     Get the terminal mode.
   */
  ioctl (1, TCGETA, &buf);
  /*
     Get ospeed!
   */
#     if HAVE_OSPEED
  ospeed = buf.c_cflag & CBAUD;
#     endif
#    else /* !TCGETA */
  auto struct sgttyb buf;


  /*
     Get the terminal mode.
   */
  ioctl (1, TIOCGETP, &buf);
  /*
     Get ospeed!
   */
#     if HAVE_OSPEED
  ospeed = buf.sg_ospeed;
#     endif
#    endif /* !TCGETA */
#   endif /* !HAVE_TERMIOS_H || !HAVE_TERMIOS_FUNC */
}



static int
outchar (ch)
     int ch;
/*!
   Termcap's `tputs()' function prints a character to the module local defined
     file `fp_outchar', which must be assigned before using `tputs()' itself.
*/
{
  return (fputc (ch, fp_outchar));
}



static Bool
get_termcap_hls (hls1_set, hls2_set)
     Bool *hls1_set;
     Bool *hls2_set;
/*!
   Inspects the Termcap buffer `tc_buf' to detect the tty color/highlighting
     sequences.  The module global vector `tc_buf[]' must be filled previously.
     May be called only once.
     Returns FALSE if an error occurs, otherwise TRUE.
*/
{
  register int i = (*hls1_set) ? 2 : 0;
  register int j = (*hls2_set) ? TC_MC_MAX - 2 : TC_MC_MAX;
  static char *tc[TC_MC_MAX] = {
    TC_MC_HL1S,
    TC_MC_HL1E,
    TC_MC_HL2S,
    TC_MC_HL2E
  };
#   if HAVE_OSPEED
  static char *padding;
#   endif
  static char *area;
  auto char *ptr_char;
  auto char *ptr_area;
  static Bool func_accessed = FALSE;
  auto Bool is_error = FALSE;


  if (!func_accessed)
    {
      func_accessed = TRUE;
      area = (char *) my_malloc (TC_BUFLEN, ERR_NO_MEMORY_AVAILABLE,
				 __FILE__, ((long) __LINE__) - 1L, "area", 0);
      ptr_area = area;
#   if HAVE_OSPEED
      /*
         Get the padding sequence.
       */
      padding = tgetstr ("pc", &ptr_area);
      PC = (padding) ? *padding : '\0';
      /*
         Get the terminal speed.
       */
      get_ospeed ();
#   endif
      for (; (i < j) && !is_error; i++)
	{
	  ptr_char = tgetstr (tc[i], &ptr_area);
	  if (ptr_char != (char *) NULL)
	    {
	      switch (i)
		{
		case 0:
		  ehls1s.seq = ptr_char;
		  break;
		case 1:
		  ehls1e.seq = ptr_char;
		  *hls1_set = TRUE;
		  break;
		case 2:
		  ehls2s.seq = ptr_char;
		  break;
		case 3:
		  ehls2e.seq = ptr_char;
		  *hls2_set = TRUE;
		  break;
		default:
		  /*
		     Error, more then 2 highlight sequence pairs given.
		   */
		  is_error = TRUE;
		}
	    }
	  else
	    /*
	       Error, no terminal capability string found for mode `tc[i]'.
	     */
	    is_error = TRUE;
	}
    }

  return ((Bool) ! is_error);
}
#  endif /* USE_HLS */



#  if USE_PAGER
static Bool
get_termcap_scr_attrib (rows, cols)
     int *rows;
     int *cols;
/*!
   Inspects the Termcap buffer `tc_buf' to detect first the amount of rows and
     columns of the tty, next whether the terminal wraps the line automatically
     at right margin.  If Termcap isn't present, internal default values are
     used.  The module global vector `tc_buf[]' must be filled previously.
     May be called only once.
     Returns FALSE if an error occurs, otherwise TRUE.
*/
{
  static Bool func_accessed = FALSE;


  if (!func_accessed)
    {
      func_accessed = TRUE;
      /*
         Get the amount of tty rows and columns.
       */
      if (tgetnum ("li") == SPECIAL_VALUE || tgetnum ("co") == SPECIAL_VALUE)
	return (FALSE);
      else
	{
	  *rows = tgetnum ("li");
	  *cols = tgetnum ("co");
	  /*
	     Check whether tty wraps the line automatically
	     at right margin and ignores newline after wrapping.
	   */
	  tty_am = tgetflag ("am");
	  tty_xn = tgetflag ("xn");
	}
    }

  return (TRUE);
}
#  endif /* USE_PAGER */
# else /* !GCAL_TCAP */
#  if defined(MSDOS) && USE_PAGER
static Uchar
peek_byte (segment, offset)
     Uint segment;
     Uint offset;
/*!
   Gets a byte of IBM/PC-memory from address (segment:offset).
*/
{
  auto Ulint long_tmp;
  auto Uchar far *ptr_char;


  long_tmp = (Ulint) segment;
  long_tmp <<= 0x10;
  long_tmp += (Ulint) offset;
  ptr_char = (Uchar far *) long_tmp;

  return (*ptr_char);
}
#  endif /* MSDOS && USE_PAGER */
# endif	/* !GCAL_TCAP */
#endif /* USE_PAGER || USE_HLS */



static void
get_hl_seq (sequence_str, hls1_set, hls2_set)
     const char *sequence_str;
     Bool *hls1_set;
     Bool *hls2_set;
/*!
   Highlighting sequences/marking characters are given in command line
   (`-H<seq1_start:seq1_end:seq2_start:seq2_end>' option set),
   i.e.:  the colon separated string `sequence_str', which should contain
          highlighting sequence/marking character pairs (2 pairs maximum,
          first for actual day, second for holiday; seq?_start enables,
          seq?_end disables), is delivered,
          e.g.:   \x20:\x20:\x1:#
                    marks holiday date like:  \x1`date'#   using given marking characters.
          e.g.:   \x1b[34;42m:\x1b[0;40m   or
                  \033[34;42m:\033[0;40m   or
                  \E[34;42m:\E[0;40m
                    thus all defines starting (ANSI)escape highlighting sequence
                    \x1b[34;42m used for actual day and ending (ANSI)escape
                    highlighting sequence \x1b[0;40m with no given highlighting
                    sequence for holiday, so default highlighting sequences
                    for holiday are used (unnotated entries are always skipped).
          Control code definitions may contain any printable characters.
          Non-printable characters may be encoded in octal or hexadecimal
          notation.  The abbreviation \E resp., \e directly encodes the escape
          character (\x1B resp., \033).
          A character can be encoded octal by typing \NNN (BACKSLASH-octal
          digit(s)), where N must be a valid octal digit (0...7).  Normally,
          3 octal digits must be given.  If the octal character code consists
          of 1 or 2 octal digits, leading zeroes must be added; except the case,
          the encoded octal character is given last in single sequence.
          A character can be encoded hexadecimal by typing \xNN (BACKSLASH-x-
          hexadecimal digit(s)), where N must be a valid hexadecimal digit
          (0...9A...Fa...f).  Normally, 2 hexadecimal digits must be given.
          If the hexadecimal character code consists of 1 hexadecimal digit,
          a leading zero must be added; except the case, the encoded
          hexadecimal character is given last in a single sequence.
          If the sequence separator character, the colon `:' character
          itself is used for marking character, it must be specified
          either octal by \072 or hexadecimal by \x3A.
   Gets, converts and stores them in the global hls structs.
*/
{
  register int i = 0;
  register int n;
  register int j;
  register int k;
  register int diff;
  register int seq_no = 0;
  static char *buf_hls[TC_MC_MAX];
  auto const char *ptr_char = sequence_str;
  auto char *ptr_err = (char *) NULL;
  static char str4[5];
  auto Bool is_error = FALSE;
  auto Bool is_hex;
  auto Bool is_oct;
  auto Bool is_esc;


  (*hls1_set) = (*hls2_set) = FALSE;
  while (*ptr_char && !is_error && (seq_no < TC_MC_MAX))
    {
      /*
         Copy highlighting sequence/marking character to temporary
         sequence string (until colon character found).
       */
      while (*ptr_char && (*ptr_char != *SEP))
	{
	  if ((Uint) i >= maxlen_max)
	    resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
				(long) __LINE__);
	  s1[i++] = *ptr_char++;
	}
      if ((Uint) i == maxlen_max)
	resize_all_strings (maxlen_max << 1, FALSE, __FILE__,
			    (long) __LINE__);
      if (*ptr_char)
	ptr_char++;
      if (i)
	{
	  s1[i] = '\0';
	  i = n = 0;
	  /*
	     Convert all textual:
	     HEX character sequences \xNN
	     OCT character sequences \NNN
	     ESC character sequences \E or \e
	     found in sequence string to real characters
	   */
	  while (s1[i + n])
	    {
	      is_hex = is_oct = is_esc = FALSE;
	      diff = 0;
	      if (s1[i + n] == '\\')
		{
		  is_esc = (Bool) (tolower (s1[i + n + 1]) == 'e');
		  is_hex = (Bool) (tolower (s1[i + n + 1]) == 'x');
		  is_oct = (Bool) isdigit (s1[i + n + 1]);
		}
	      if (is_esc)
		{
		  s1[i] = '\033';	/* \x1b */
		  n++;
		}
	      else if (is_hex || is_oct)
		{
		  k = 0;
		  if (is_hex)
		    {
		      str4[k++] = '0';
		      str4[k++] = 'x';
		    }
		  else
		    {
		      if (s1[i + n + 1] != '0')
			str4[k++] = '0';
		      else
			diff = 1;
		      str4[k++] = s1[i + n + 1];
		    }
		  /*
		     Copy hex/oct digits to prefix.
		   */
		  j = i + n + 2;
		  while (s1[j] && (s1[j] != *SEP) && (k < 4 - diff))
		    str4[k++] = s1[j++];
		  str4[k] = '\0';
		  /*
		     Convert textual hex/oct character to a decimal value.
		   */
#if HAVE_STRTOL
		  j = (int) strtol (str4, &ptr_err, 0);
#else /* !HAVE_STRTOL */
		  ptr_err = str4 + 1;
		  if (is_hex)
		    ptr_err++;
		  j = sbyte2int (ptr_err, (is_hex) ? 16 : 8);
#endif /* !HAVE_STRTOL */
		  /*
		     If conversion error occurs (invalid chars in hex/oct
		     character sequence), don't store ALL sequences,
		     which are previously decoded!
		   */
		  if (j == -1
#if HAVE_STRTOL
		      || *ptr_err
#endif
		    )
		    {
		      is_error = TRUE;
		      break;
		    }
		  /*
		     Put converted character code back to the sequence string.
		   */
		  s1[i] = (char) j;
		  n += (k - 2 + diff) + 1;
		}
	      i++;
	      s1[i] = s1[i + n];
	    }
	  if (!is_error)
	    {
	      /*
	         Store highlighting sequence/marker character
	         in according global highlighting sequence
	         struct_variable using static buffer `buf_hls[]'.
	       */
	      n = (int) strlen (s1);
	      buf_hls[seq_no] =
		(char *) my_malloc (n + 1, ERR_NO_MEMORY_AVAILABLE, __FILE__,
				    ((long) __LINE__) - 1L, "buf_hls[seq_no]",
				    seq_no);
	      strcpy (buf_hls[seq_no], s1);
	      switch (seq_no)
		{
		case 0:
		  ehls1s.seq = buf_hls[seq_no];
		  ehls1s.len = n;
		  break;
		case 1:
		  ehls1e.seq = buf_hls[seq_no];
		  ehls1e.len = n;
		  *hls1_set = TRUE;
		  break;
		case 2:
		  ehls2s.seq = buf_hls[seq_no];
		  ehls2s.len = n;
		  break;
		case 3:
		  ehls2e.seq = buf_hls[seq_no];
		  ehls2e.len = n;
		  *hls2_set = TRUE;
		  break;
		default:
		  ;		/* Void */
		}
	    }
	}
      i = 0;
      seq_no++;
    }
  /*
     Either real highlighting sequences (ESC-char..., length>1) only
     or marking characters (length==1) only can be managed:
     Avoid mixture of both!
   */
#if USE_HLS
  if (*hls1_set)
    if (((ehls1s.len == 1)
	 && (ehls1e.len != 1)) || ((ehls1s.len != 1) && (ehls1e.len == 1)))
      *hls1_set = FALSE;
  if (*hls2_set)
    if (((ehls2s.len == 1)
	 && (ehls2e.len != 1)) || ((ehls2s.len != 1) && (ehls2e.len == 1)))
      *hls2_set = FALSE;
  if (*hls1_set)
    {
      if ((ehls1s.len == 1) && (ehls1e.len == 1))
	{
	  if (*hls2_set)
	    {
	      if ((ehls2s.len == 1) && (ehls2e.len == 1))
		emu_hls = TRUE;
	      else
		{
		  if (emu_hls)
		    *hls2_set = FALSE;
		  else
		    *hls1_set = FALSE;
		}
	    }
	  else if (!emu_hls)
	    *hls1_set = FALSE;
	}
      else
	{
	  if (*hls2_set)
	    {
	      if ((ehls2s.len != 1) && (ehls2e.len != 1))
		;		/* Void, ok */
	      else
		{
		  if (emu_hls)
		    *hls1_set = FALSE;
		  else
		    *hls2_set = FALSE;
		}
	    }
	  else if (emu_hls)
	    *hls1_set = FALSE;
	}
    }
  if (!*hls1_set && *hls2_set)
    {
      if ((ehls2s.len == 1) && (ehls2e.len == 1))
	{
	  if (!emu_hls)
	    *hls2_set = FALSE;
	}
      else if (emu_hls)
	*hls2_set = FALSE;
    }
  if (*hls1_set && *hls2_set)
    if (emu_hls && (ehls1s.len > 1) && (ehls2s.len > 1))
      (*hls1_set) = (*hls2_set) = FALSE;
#else /* !USE_HLS */
  if (*hls1_set)
    if (ehls1s.len != 1 || ehls1e.len != 1)
      *hls1_set = FALSE;
  if (*hls2_set)
    if (ehls2s.len != 1 || ehls2e.len != 1)
      *hls2_set = FALSE;
#endif /* !USE_HLS */
}



#if !HAVE_STRTOL
static int
sbyte2int (string, base)
     const char *string;
     const int base;
/*!
   Converts a textual b-adic string `string', which contains the absolute
     textual representation of a byte given in number base `base' (2-16)
     to decimal base, and returns its decimal value.
     If an error occurs, SPECIAL_VALUE is returned.
*/
{
  register int val;
  register int res = 0;
  register int mul = 1;
  register int len = (int) strlen (string);
  register int len_base_str;
  static const char base_str[] = "0123456789abcdef";
  auto const char *ptr_char = string + (len - 1);


  len_base_str = (int) strlen (base_str);
  if (!len || base < 2 || base > len_base_str)
    return (SPECIAL_VALUE);
  for (; len; len--)
    {
      if (strchr (base_str, tolower (*ptr_char)) == (char *) NULL)
	return (SPECIAL_VALUE);
      val = len_base_str - (int) strlen (strchr (base_str, *ptr_char--));
      if (val > base)
	return (SPECIAL_VALUE);
      res += (val * mul);
      mul *= base;
    }

  return (res);
}
#endif /* !HAVE_STRTOL */
