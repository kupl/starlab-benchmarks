#ifndef __RC_USE_H
# define __RC_USE_H
/*!
*  \file rc-use.h
*  \brief Central function, manages the resource file handling and
*  			displays the results file header.
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
*  $Id: rc-use.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  The REGEX stuff.
*/
# if HAVE_POSIX_REGCOMP
#  ifdef REG_EXTENDED
#   define REGCOMP_FLAG  REG_EXTENDED
#  else
#   define REGCOMP_FLAG  0
#  endif
# endif



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
*  The REGEX stuff.
*/
# if HAVE_RE_COMP
extern char *re_comp ();
# endif
/*
************************************************** Defined in `rc-use.c'.
*/
extern void rc_use __P_ ((void));
__END_DECLARATIONS
#endif /* __RC_USE_H */
