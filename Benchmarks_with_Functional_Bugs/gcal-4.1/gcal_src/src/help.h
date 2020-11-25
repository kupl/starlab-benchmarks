#ifndef __HELP_H
# define __HELP_H
/*!
*  \file help.h
*  \brief The help, version and license screens header file.
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
*  $Id: help.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  Type definitions.
*/
/*!
*  The supported date format information record.
*/
typedef struct date_format_information_type
{
  /*! Information text of the format. */
  char *di_info;

  /*! First character of the format. */
  char di_1format;

  /*! Second character of the format. */
  char di_2format;
}
Di_struct;

# if USE_RC
/*!
*  The exclusion date information record.
*/
typedef struct exclusion_date_type
{
  /* The character used in the %? inclusive/exclusive day special text. */
  char ed_id;

  /* Description text of the %? inclusive/exclusive day special text. */
  char *ed_info;
}
Ed_struct;
# endif	/* USE_RC */



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `help.c'.
*/
extern void
  my_help_on_help __P_ ((FILE * fp,
			 const char *longopt,
			 const Bool lopt_ambig, const int cols));
extern void my_extended_help __P_ ((FILE * fp, const int longopt_symbolic));
extern void my_basic_help __P_ ((FILE * fp));
extern void my_license __P_ ((FILE * fp));
extern void my_version __P_ ((FILE * fp));
extern char *usage_msg __P_ ((void));
extern char *lopt_msg __P_ ((void));
__END_DECLARATIONS
#endif /* __HELP_H */
