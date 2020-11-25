#ifndef __TCAL_H
# define __TCAL_H
/*!
*  \file tcal.h
*  \brief Start `gcal' with date set ONE day ahead header file.
*/
/*
*  Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013 Free Software Foundation, Inc.
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
*  $Id: tcal.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*   Program specific preprocessor statements.
*/
/*! Name of environment variable for accessing the Gcal program. */
# define ENV_VAR_GCALPROG     "GCALPROG"

/*! Default shift value; may be set to "0"..."9". */
# define SHIFT_VALUE_DEFAULT  "1"



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS extern int main __P_ ((int argc, char *argv[]));
__END_DECLARATIONS
#endif /* __TCAL_H */
