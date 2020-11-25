#ifndef __TTY_H
# define __TTY_H
/*!
*  \file tty.h
*  \brief Screen support functions and major output function header file.
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
*  $Id: tty.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*   Definition of the Termcap buffer size.
*/
# if (USE_PAGER || USE_HLS) && defined(GCAL_TCAP)
#  define TC_BUFLEN  4096
# endif	/* (USE_PAGER || USE_HLS) && GCAL_TCAP */



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
# if (USE_PAGER || USE_HLS) && defined(GCAL_TCAP)
#  if !HAVE_TERMCAP_H
extern int tgetent __P_ ((char *buffer, char *termtype));
extern int tgetnum __P_ ((char *name));
#   if USE_HLS
extern char *tgetstr __P_ ((char *name, char **area));
extern char *tputs __P_ ((char *string, int nlines, int (*outfunc) ()));
#   endif /* USE_HLS */
#   if USE_PAGER
extern int tgetflag __P_ ((char *name));
#   endif /* USE_PAGER */
#  endif /* !HAVE_TERMCAP_H */
# endif	/* (USE_PAGER || USE_HLS) && GCAL_TCAP */
/*
************************************************** Defined in `tty.c'.
*/
extern void print_text __P_ ((FILE * fp, char *text_line));
extern void get_tty_hls __P_ ((const char *sequence_str));
# if USE_PAGER
extern void get_tty_scr_size __P_ ((int *rows, int *cols));
# endif
__END_DECLARATIONS
#endif /* __TTY_H */
