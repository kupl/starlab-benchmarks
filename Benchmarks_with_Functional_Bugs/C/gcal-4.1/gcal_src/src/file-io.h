#ifndef __FILE_IO_H
# define __FILE_IO_H
/*!
*  \file file-io.h
*  \brief Managing and accessing resource, include and response files header file.
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
*  $Id: file-io.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  GLOBAL functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `file-io.c'.
*/
extern FILE *file_open __P_ ((char **filename,
			      const int level,
			      const Fmode_enum mode, Bool * bad_sys_include));
extern char *file_read_line __P_ ((FILE * fp,
				   char **line_buffer,
				   int *in_pool,
				   char *pool,
				   char *ptr_pool,
				   const char *filename,
				   long *line_number,
				   int *line_length,
				   const Fmode_enum mode,
				   Bool * is_include,
				   Bool * is_dvar, Bool * is_tvar));
extern char **insert_response_file __P_ ((FILE * fp,
					  char *filename,
					  const char *opt_list,
					  Uint * my_argc_max,
					  int *my_argc, char *my_argv[]));
extern void
  write_log_file __P_ ((const char *filename,
			const Fmode_enum mode,
			const char *mode_txt,
			const char *created_txt,
			const int argc, char *argv[]));
__END_DECLARATIONS
#endif /* __FILE_IO_H */
