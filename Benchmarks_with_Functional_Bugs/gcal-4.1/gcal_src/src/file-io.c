/*!
*  \file file-io.c
*  \brief Managing and accessing resource, include and response files.
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
#ifdef GCAL_SHELL
# if HAVE_UNISTD_H
#  include <unistd.h>
# endif
#endif
#if HAVE_SYS_STAT_H
# if HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# include <sys/stat.h>
#endif
#include "common.h"
#if USE_RC
# include "rc-defs.h"
#endif /* !USE_RC */
#include "globals.h"
#include "gcal.h"
#include "hd-defs.h"
#include "hd-use.h"
#if USE_RC
# include "rc-utils.h"
#endif
#include "tty.h"
#include "utils.h"
#include "file-io.h"



/*
*  static functions prototypes.
*/
__BEGIN_DECLARATIONS
/*
************************************************** Defined in `file-io.c'.
*/
static void
  make_absolute_filename __P_ ((char **absolute_filename,
				const char *directory, const char *filename));
static FILE *get_file_ptr __P_ ((FILE * fp,
				 const char *filename,
				 const int level,
				 const Fmode_enum mode, Bool * is_first));
__END_DECLARATIONS
/*
*  Function implementations.
*/
  FILE * file_open (filename, level, mode, bad_sys_include)
     char **
       filename;
     const int
       level;
     const Fmode_enum
       mode;
     Bool *
       bad_sys_include;
/*!
   Tries to open a resource/response file (MODE == `REsource', `REsponse')
     in the following order:
       1) $GCALPATH
       2) $HOME
       3) $GCAL_USR_DATADIR (environment variable first; default name if unset)
       4) $GCAL_SYS_DATADIR (environment variable first; default name if unset)
     and returns a file pointer to this file and it's MODIFIED name in some
     cases (due to this, it's necessary to reallocate the memory area of
     `&filename', which must be allocated on the heap), otherwise return a NULL pointer.
   If MODE is set to `USr_include', this function tries to open an
     include file (#include "file" directive found) in:
       1) actual directory
       2) $GCALPATH
       3) $HOME
       4) $GCAL_USR_DATADIR (environment variable first; default name if unset)
       5) $GCAL_SYS_DATADIR (environment variable first; default name if unset)
     and returns a file pointer to this file and it's MODIFIED name in some
     cases (due to this, it's necessary to reallocate the memory area of
     `&filename', which must be allocated on the heap), otherwise return a NULL pointer.
     If a root directory based include file name is given, set `&bad_sys_include'
     to TRUE!
   If MODE is set to `SYs_include', this function tries to open an
     include file (#include <file> directive found) only in:
       1) $GCAL_USR_DATADIR (environment variable first; default name if unset)
       2) $GCAL_SYS_DATADIR (environment variable first; default name if unset)
     and returns a file pointer to this file and it's MODIFIED name in some
     cases (due to this, it's necessary to reallocate the memory area of
     `&filename', which must be allocated on the heap), otherwise return a NULL pointer.
     If a root directory based include file name is given, set `&bad_sys_include'
     to TRUE!
   If MODE is set to `HEre', this function tries to open a
     "here" file (`--here=ARG' option) in
       1) actual directory
     and returns a file pointer to this file and it's name,
     otherwise return a NULL pointer.
   If MODE is set to `COmmon', this function tries to open a
     common file in
       1) $PATH
     and returns a file pointer to this file and it's name,
     otherwise return a NULL pointer.
   Passes the nesting LEVEL only for informational purposes to other functions called here.
   NO informational messages will be emitted in `REsponse' and `COmmon' modes!!
*/
{
  auto FILE *fp = (FILE *) NULL;
  register int len;
  auto char *ptr_env;
  auto char *ptr_char;
  auto Bool is_absolute_filename = FALSE;
#if USE_RC
  auto Bool is_root_based_filename = FALSE;
  auto Bool is_disk_given = FALSE;
  auto Bool is_first = (Bool) ((mode == REsource) ? TRUE : FALSE);
#else /* !USE_RC */
  auto Bool is_first = FALSE;
#endif /* !USE_RC */


  len = (int) strlen (*filename);
  if ((Uint) len >= maxlen_max)
    resize_all_strings (len + 1, FALSE, __FILE__, (long) __LINE__);
  strcpy (s1, *filename);
  ptr_char = *filename;
#ifdef DISK_SEP
  ptr_char = strchr (*filename, *DISK_SEP);
  if (ptr_char != (char *) NULL)
    {
      /*
         If a disk/drive is specified, this is like an absolute file name!
       */
      is_absolute_filename = TRUE;
# if USE_RC
      is_disk_given = TRUE;
# endif
      ptr_char++;
    }
  else
    ptr_char = *filename;
#endif /* DISK_SEP */
#if USE_RC
  /*
     Check if an absolute file name is given.
   */
  if (*ptr_char == *DIR_SEP || *ptr_char == *ACTUAL_DIR)
    {
      if (*ptr_char == *ACTUAL_DIR)
	{
	  while (*ptr_char == *ACTUAL_DIR)
	    ptr_char++;
	  if (*ptr_char == *DIR_SEP)
	    {
	      is_absolute_filename = TRUE;
	      ptr_char--;
	    }
	}
      else
	is_absolute_filename = TRUE;
    }
  else
    /*
       Check if STDIN channel is wanted.
     */
  if ((*ptr_char == '-') && (len == 1))
    is_absolute_filename = TRUE;
  if (*ptr_char == *DIR_SEP)
    is_root_based_filename = TRUE;
  if ((is_disk_given || is_root_based_filename) && (mode == SYs_include))
    {
      /*
         Include files names. which are based by the root directory,
         are not allowed in system include statements, e.g.:
         #include </foo>   or   #include </foo/bar>.
       */
      *bad_sys_include = TRUE;
      return (NULL);
    }
  *bad_sys_include = FALSE;
  if (mode != SYs_include)
#endif
    {
      if (mode != COmmon)
	{
	  /*
	     Try to open the file directly (either absolute file name,
	     USr_include file type or HEre file type given).
	   */
	  if (is_absolute_filename
#if USE_RC
	      || mode == USr_include || mode == HEre
#endif
	    )
	    {
	      fp = get_file_ptr (fp, *filename, level, mode, &is_first);
	      /*
	         If the file is found, return to caller immediately.
	       */
	      if (fp != (FILE *) NULL)
		return (fp);
	      else if (is_absolute_filename
#if USE_RC
		       || mode == HEre
#endif
		)
		{
#if USE_RC
		  if ((fp == (FILE *) NULL)
		      && (warning_level >= WARN_LVL_MAX))
		    /*
		       Terminate if `--debug=abort' option is given
		       and the absolute file name or the `HEre' file
		       name isn't found.
		     */
		    my_error (ERR_FILE_NOT_FOUND, *filename, 0L, *filename,
			      0);
#endif
		  return (NULL);
		}
	    }
	}
#if !defined(AMIGA) || defined(__GNUC__)
      if (fp == (FILE *) NULL)
	{
	  /*
	     Simple file name delivered and the file isn't found:
	     * Either search a COmmon file in the directory/ies,
	     which are stored in the environment variable $PATH
	     * Or search all other file types in the directory/ies,
	     which are stored in the environment variable $GCALPATH.
	   */
	  if (mode == COmmon)
	    ptr_env = getenv (ENV_VAR_PATH);
	  else
	    ptr_env = getenv (ENV_VAR_GCALPATH);
	  if (ptr_env != (char *) NULL)
	    if (*ptr_env)
	      {
		auto Bool ok = FALSE;


		len = (int) strlen (ptr_env);
		if ((Uint) len >= maxlen_max)
		  resize_all_strings (len + 1, FALSE, __FILE__,
				      (long) __LINE__);
		strcpy (s2, ptr_env);
		while (!ok && (fp == (FILE *) NULL))
		  {
		    ok =
		      (Bool) ((ptr_char =
			       strchr (s2, *PATH_SEP)) == (char *) NULL);
		    if (ok)
		      len = (int) strlen (s2);
		    else
		      len = (int) strlen (s2) - strlen (ptr_char);
		    strncpy (s1, s2, len);
		    s1[len] = '\0';
		    if (len)
		      if (s1[len - 1] != *DIR_SEP)
			strcat (s1, DIR_SEP);
		    strcat (s1, *filename);
# ifdef SUFFIX_SEP
		    if ((ptr_char =
			 strchr (s1, *SUFFIX_SEP)) != (char *) NULL)
		      *ptr_char = '\0';
# endif
		    if (*s1)
		      fp = get_file_ptr (fp, s1, level, mode, &is_first);
		    if (ok)
		      break;
		    ptr_char = s2 + len + 1;
		    if (!*ptr_char)
		      break;
		    strcpy (s2, ptr_char);
		  }
		/*
		   If a file isn't found yet but the last character of the
		   $[GCAL]PATH environment variable is a PATH_SEP character
		   (which means the last search for the file must be done
		   in the actual directory), perform this file access!
		 */
		if ((fp == (FILE *) NULL)
		    && (*(ptr_env + strlen (ptr_env) - 1) == *PATH_SEP))
		  fp = get_file_ptr (fp, *filename, level, mode, &is_first);
	      }
	}
      if (mode != COmmon)
	{
	  if (fp == (FILE *) NULL)
	    {
	      /*
	         File not found in the $GCALPATH directory:
	         * Search the file in the directory, which is
	         stored in the environment variable of $HOME.
	       */
	      ptr_env = getenv (ENV_VAR_HOME);
	      if (ptr_env != (char *) NULL)
		if (*ptr_env)
		  {
		    make_absolute_filename (&s1, ptr_env, *filename);
		    fp = get_file_ptr (fp, s1, level, mode, &is_first);
		  }
	    }
	}
    }
  /*
     It's not necessary to perform further searches for COmmon files
     in the $GCAL_???_DATADIR directories!
   */
  if (mode == COmmon)
    return (fp);
# if USE_RC
  if (fp == (FILE *) NULL)
    {
      /*
         File not found in the $HOME directory:
         * Search the file in the user's library directory of
         $HOME/[$]GCAL_USR_DATADIR.
       */
      ptr_env = getenv (ENV_VAR_HOME);
      if (ptr_env != (char *) NULL)
	if (*ptr_env)
	  {
	    ptr_char = getenv (ENV_VAR_USR_DATADIR);
	    /*
	       Search the file in the directory specified in the
	       $GCAL_USR_DATADIR environment variable first.
	     */
	    if (ptr_char != (char *) NULL)
	      {
		if (*ptr_char)
		  {
		    make_absolute_filename (&s1, ptr_env, ptr_char);
		    strcpy (s2, s1);
		    make_absolute_filename (&s1, s2, *filename);
		    fp = get_file_ptr (fp, s1, level, mode, &is_first);
		  }
	      }
	    else
	      {
		/*
		   If the $GCAL_USR_DATADIR environment variable is unset,
		   search the file in burned-in user's library directory of
		   GCAL_USR_DATADIR.
		 */
		make_absolute_filename (&s1, ptr_env, GCAL_USR_DATADIR);
		strcpy (s2, s1);
		make_absolute_filename (&s1, s2, *filename);
		fp = get_file_ptr (fp, s1, level, mode, &is_first);
	      }
	  }
    }
  if (fp == (FILE *) NULL)
    {
      /*
         The file isn't found in user library directory of
         $HOME/[$]GCAL_USR_DATADIR:
         * Search the file in the system's library directory of
         $GCAL_SYS_DATADIR.
       */
      ptr_env = getenv (ENV_VAR_SYS_DATADIR);
      /*
         Search the file in the directory specified in the
         $GCAL_SYS_DATADIR environment variable first.
       */
      if (ptr_env != (char *) NULL)
	{
	  if (*ptr_env)
	    {
	      make_absolute_filename (&s1, ptr_env, *filename);
	      fp = get_file_ptr (fp, s1, level, mode, &is_first);
	    }
	}
      else
	{
	  /*
	     If the $GCAL_SYS_DATADIR environment variable is unset:
	     * Search the file in burned-in system's library directory of
	     GCAL_SYS_DATADIR.
	   */
	  make_absolute_filename (&s1, GCAL_SYS_DATADIR, *filename);
	  fp = get_file_ptr (fp, s1, level, mode, &is_first);
	}
    }
# endif	/* USE_RC */
#else /* AMIGA && !__GNUC__ */
      /*
         It's not necessary to perform further searches for COmmon files,
         because the compiler/system doesn't support the `getenv()' function.
       */
      if (mode == COmmon)
	return (fp);
# if USE_RC
      /*
         This part is for compilers/systems
         which do not support the `getenv()' function.
       */
      if (fp == (FILE *) NULL)
	{
	  /*
	     The file isn't found in the actual directory:
	     * Search the file in the burned-in user's library directory of
	     GCAL_USR_DATADIR.
	   */
	  make_absolute_filename (&s1, GCAL_USR_DATADIR, *filename);
	  fp = get_file_ptr (fp, s1, level, mode, &is_first);
	}
      if (fp == (FILE *) NULL)
	{
	  /*
	     The file isn't found in the user's library directory of
	     GCAL_USR_DATADIR:
	     * Search the file in the burned-in system's library directory of
	     GCAL_SYS_DATADIR.
	   */
	  make_absolute_filename (&s1, GCAL_SYS_DATADIR, *filename);
	  fp = get_file_ptr (fp, s1, level, mode, &is_first);
	}
# endif	/* USE_RC */
#endif /* AMIGA && !__GNUC__ */
      /*
         If the file is found:
         Copy the real (absolute) name of the file to `&filename'.
       */
      if (fp != (FILE *) NULL)
	{
	  len = (int) strlen (s1);
	  if ((int) strlen (*filename) < len)
	    /*
	       We MUST reallocate the memory area of `&filename'!
	     */
	    *filename = (char *) my_realloc ((VOID_PTR) * filename,
					     len + 1,
					     ERR_NO_MEMORY_AVAILABLE,
					     __FILE__, ((long) __LINE__) - 3L,
					     "*filename", 0);
	  strcpy (*filename, s1);
	}
#if USE_RC
      else
	/*
	   File not found!
	 */
      if (warning_level >= WARN_LVL_MAX)
	/*
	   Terminate if `--debug=abort' option is given.
	 */
	my_error (ERR_FILE_NOT_FOUND, *filename, 0L, *filename, 0);
#endif

      return (fp);
    }



  char *file_read_line (fp, line_buffer, in_pool, pool, ptr_pool, filename,
			line_number, line_length, mode, is_include, is_dvar,
			is_tvar) FILE *fp;
  char **line_buffer;
  int *in_pool;
  char *pool;
  char *ptr_pool;
  const char *filename;
  long *line_number;
  int *line_length;
  const Fmode_enum mode;
  Bool *is_include;
  Bool *is_dvar;
  Bool *is_tvar;
/*!
   Reads a line of a delivered resource/response file into `&line_buffer'
     using the delivered char vector `pool', which must be allocated by caller
     with size BUF_LEN+1 (BUF_LEN should be "A POWER OF 2", e.g., 4096).
     Returns the position in buffer of the character managed in next call
     by char pointer `ptr_pool', which must be defined by caller; or NULL
     if EOF is detected.
*/
  {
    static Uint lbuf_max = MAXLEN_MAX;
    register int i = 1;
    auto char *ptr_char;
    auto char ch;
    auto Bool is_error = FALSE;
#if USE_RC
    auto Bool is_rem = FALSE;
#endif


    /*
       Ensure NOW that `&line_buffer' is ALWAYS as large as all "other"
       string vectors (s1...s7) are, because the `&line_buffer' is not
       resized in all cases when the "other" strings vectors are resized!
     */
    if (lbuf_max < maxlen_max)
      {
	*line_buffer = (char *) my_realloc ((VOID_PTR) * line_buffer,
					    maxlen_max,
					    ERR_NO_MEMORY_AVAILABLE,
					    __FILE__, ((long) __LINE__) - 3L,
					    "*line_buffer", maxlen_max);
	lbuf_max = maxlen_max;
      }
    **line_buffer = '\0';
    *line_length = 0;
    ptr_char = *line_buffer;
    (*is_include) = (*is_dvar) = (*is_tvar) = FALSE;
    /*
       Initial fill/refill of `pool'.
     */
    if (!*in_pool)
      {
	*in_pool = read (fileno (fp), (char *) pool, BUF_LEN);
	if (!*in_pool)
	  /*
	     At end of file.
	   */
	  return (NULL);
	else if (*in_pool < 0)
	  /*
	     File read error.
	   */
	  my_error (ERR_READ_FILE, __FILE__, (long) __LINE__, filename, 0);
	ptr_pool = pool;
      }
    if (*ptr_pool == '\n')
      (*line_number)++;
    while ((*ptr_pool != REM_CHAR) && (
#if USE_RC
					((mode == REsource)
					 && (*ptr_pool != *RC_INCL_STMENT)
					 && (*ptr_pool != RC_TVAR_CHAR)
					 && !isalnum (*ptr_pool)) ||
#endif
					((mode == REsponse)
					 && (*ptr_pool != *MONTH3_LIT)
					 && (*ptr_pool != *FYEAR_SEP)
					 && (*ptr_pool != RSP_CHAR)
					 && !isalnum (*ptr_pool)
#if USE_RC
					 && (*ptr_pool != RC_ADATE_CHAR)
#endif
					 && (*ptr_pool != *SWITCH)
					 && (*ptr_pool != *SWITCH2))))
      {
	if ((!**line_buffer && !isspace (*ptr_pool)) || **line_buffer)
	  {
	    if ((Uint) i >= maxlen_max)
	      {
		resize_all_strings (maxlen_max << 1, TRUE, __FILE__,
				    (long) __LINE__);
		ptr_char = *line_buffer + i - 1;
	      }
	    i++;
	    *ptr_char++ = *ptr_pool++;
	  }
	else
	  ptr_pool++;
	(*in_pool)--;
	if (!*in_pool)
	  {
	    /*
	       Refill `pool', because the line we work on isn't complete.
	     */
	    *in_pool = read (fileno (fp), (char *) pool, BUF_LEN);
	    if (!*in_pool)
	      {
		/*
		   At end of file.
		 */
		if (i > 1)
		  /*
		     Error, the line of the resource file contains no valid "date"-part.
		   */
		  break;
		return (NULL);
	      }
	    else if (*in_pool < 0)
	      /*
	         File read error.
	       */
	      my_error (ERR_READ_FILE, __FILE__, (long) __LINE__, filename,
			0);
	    ptr_pool = pool;
	  }
	if (*ptr_pool == '\n')
	  {
	    if (i > 1)
	      {
		/*
		   Error, the line of the resource file contains no valid "date"-part.
		 */
		if ((Uint) i < maxlen_max)
		  i--;
		(*in_pool)++;
		ptr_pool--;
		ptr_char--;
		break;
	      }
	    (*line_number)++;
	  }
      }
    (*line_number)++;
    /*
       Skip whole line.
     */
    if ((mode != COmmon) && (*ptr_pool == REM_CHAR))
      {
#if USE_RC
	is_rem = TRUE;
#endif
	LOOP
	{
	  ch = '\0';
	  /*
	     Read until a NEWLINE character or EOF.
	   */
	  while (*ptr_pool != '\n')
	    {
	      /*
	         Refill `pool', because the line we work on isn't complete.
	       */
	      if (!*in_pool)
		{
		  *in_pool = read (fileno (fp), (char *) pool, BUF_LEN);
		  if (!*in_pool)
		    /*
		       At end of file.
		     */
		    return (NULL);
		  else if (*in_pool < 0)
		    /*
		       File read error.
		     */
		    my_error (ERR_READ_FILE, __FILE__, (long) __LINE__,
			      filename, 0);
		  ptr_pool = pool;
		}
	      else
		{
		  ch = *ptr_pool++;
		  (*in_pool)--;
		}
	    }
	  /*
	     Skip the trailing NEWLINE character of the line.
	   */
	  ptr_pool++;
	  if (*in_pool)
	    (*in_pool)--;
	  /*
	     Check if it is a `\\'`\n' (BACKSLASH-NEWLINE) character sequence:
	     If so, append the next line to the current line.
	   */
	  if (ch == QUOTE_CHAR)
	    {
	      if (!*in_pool)
		{
		  *in_pool = read (fileno (fp), (char *) pool, BUF_LEN);
		  if (!*in_pool)
		    /*
		       At end of file.
		     */
		    return (NULL);
		  else if (*in_pool < 0)
		    /*
		       File read error.
		     */
		    my_error (ERR_READ_FILE, __FILE__, (long) __LINE__,
			      filename, 0);
		  ptr_pool = pool;
		}
	      (*line_number)++;
	    }
	  else
	    break;
	}
      }
    else
      {
	if (i > 1)
	  is_error = TRUE;
#if USE_RC
	else if (mode == REsource)
	  {
	    if (*ptr_pool == *RC_INCL_STMENT)
	      *is_include = TRUE;
	    else if (isalpha (*ptr_pool))
	      *is_dvar = TRUE;
	    else if (*ptr_pool == RC_TVAR_CHAR)
	      *is_tvar = TRUE;
	    else if (!isdigit (*ptr_pool))
	      is_error = TRUE;
	  }
#endif
	ch = (*ptr_char++) = *ptr_pool++;
	(*in_pool)--;
	LOOP
	{
	  if (*in_pool)
	    {
	      if ((Uint) i < maxlen_max)
		i++;
	      else
		{
		  resize_all_strings (maxlen_max << 1, TRUE, __FILE__,
				      (long) __LINE__);
		  ptr_char = *line_buffer + i++;
		}
	      /*
	         Character sequence `\\'`\n' (BACKSLASH-NEWLINE) found:
	         Eliminate the sequence and append the next line to the current line.
	       */
	      if ((*ptr_pool == '\n') && (ch == QUOTE_CHAR))
		{
		  if (is_error)
		    {
		      (*in_pool)--;
		      break;
		    }
		  if ((Uint) i >= maxlen_max)
		    {
		      resize_all_strings (maxlen_max << 1, TRUE, __FILE__,
					  (long) __LINE__);
		      ptr_char = *line_buffer + i - 1;
		    }
		  ptr_char--;
		  i -= 2;
		  (*line_number)++;
		  ch = *ptr_pool;
		  if (*in_pool)
		    ptr_pool++;
		}
	      else
		{
		  ch = *ptr_pool++;
		  /*
		     Single NEWLINE character found:
		     We must finish the line!
		   */
		  if (ch == '\n')
		    {
		      (*in_pool)--;
		      break;
		    }
		  else
		    {
		      if ((Uint) i >= maxlen_max)
			{
			  resize_all_strings (maxlen_max << 1, TRUE, __FILE__,
					      (long) __LINE__);
			  ptr_char = *line_buffer + i - 1;
			}
		      *ptr_char++ = ch;
		    }
		}
	      if (*in_pool)
		(*in_pool)--;
	    }
	  /*
	     Refill `pool', because the line we work on isn't complete.
	   */
	  if (!*in_pool)
	    {
	      *in_pool = read (fileno (fp), (char *) pool, BUF_LEN);
	      if (!*in_pool)
		{
		  /*
		     At end of file.
		   */
		  if (!**line_buffer)
		    return (NULL);
		  else
		    break;
		}
	      else if (*in_pool < 0)
		/*
		   File read error.
		 */
		my_error (ERR_READ_FILE, __FILE__, (long) __LINE__, filename,
			  0);
	      ptr_pool = pool;
	    }
	}
      }
    *ptr_char = '\0';
#if USE_RC
    if (!is_error && !is_rem && (mode == REsource))
      {
	if (*is_tvar)
	  {
	    if (i > 2)
	      {
		/*
		   If the line does not consist of a text variable
		   assignment or operation, expand the text variable.
		 */
		if ((isalpha (*(*line_buffer + 1))
		     && (*(*line_buffer + 2) != *RC_VAR_ASSIGN)
		     && (*(*line_buffer + 2) != *RC_TVAR_ICMD_ASSIGN)
		     && (*(*line_buffer + 2) != *RC_TVAR_UCMD_ASSIGN)
		     && (*(*line_buffer + 2) != *RC_VAR_ADD)
		     && (*(*line_buffer + 2) != *RC_VAR_SUB))
		    || *(*line_buffer + 1) == FLEFT_CHAR
		    || *(*line_buffer + 1) == FCENTER_CHAR
		    || *(*line_buffer + 1) == FRIGHT_CHAR)
		  *is_tvar = FALSE;
	      }
	    else
	      *is_tvar = FALSE;
	  }
	/*
	   Try to expand text variables NOW!
	 */
	if (!*is_tvar)
	  {
	    register int len;
	    register int n;
	    register int k;
	    register int x;
	    register int l;
	    register int flen = 1;
	    auto int fstyle = FSTYLE_NONE;
	    auto int fwidth = SPECIAL_VALUE;
	    auto char *ptr_tvar;
	    static char tvar[2];
	    static char the_tvar;
	    auto Bool is_quoted = FALSE;
	    auto Bool restore_tvar = FALSE;
	    auto Bool ok = FALSE;
	    auto Bool is_cformat = FALSE;
	    auto Bool is_lformat = FALSE;
	    auto Bool is_sign = FALSE;
	    auto Bool is_lzero = FALSE;
	    auto Bool is_fformat = FALSE;
	    auto Bool is_suffix = FALSE;


	    /*
	       Analyse line till ALL referenced text variables are expanded.
	     */
	    do
	      {
		ptr_char = *line_buffer;
		/*
		   Check if the assigned TEXT contains any references
		   to other TVAR variables, if so, insert their TEXTs.
		 */
		ptr_tvar = strchr (ptr_char, RC_TVAR_CHAR);
		if (ptr_tvar != (char *) NULL)
		  {
		    n = flen;
		    /*
		       Buffer the name of the text variable respecting
		       a possible TVAR format $[<|:|>[+][0]N[&]#|*]TVAR.
		     */
		    while (*(ptr_tvar + n) && !isalpha (*(ptr_tvar + n)))
		      n++;
		    the_tvar = *(ptr_tvar + n);
		    *tvar = tvar[1] = '\0';
		    n = k = 0;
		    do
		      {
			/*
			   Check for a TVAR format.
			 */
			flen = decode_format (ptr_tvar, flen, &is_cformat,
					      &is_lformat, &is_sign,
					      &is_lzero, &is_suffix,
					      &is_fformat, &fstyle, &fwidth);
			if (fwidth == SPECIAL_VALUE)
			  flen = 1;
			*tvar = *(ptr_tvar + flen);
			tvar[1] = *(ptr_tvar + flen + 1);
			len = (int) (ptr_tvar - ptr_char);
			if (len)
			  {
			    while ((Uint) len + k >= maxlen_max)
			      {
				resize_all_strings (maxlen_max << 1, TRUE,
						    __FILE__,
						    (long) __LINE__);
				ptr_char = *line_buffer + n;
			      }
			    strncpy (s5 + k, ptr_char, len);
			    k += len;
			  }
			s5[k] = '\0';
			if (k)
			  if (s5[k - 1] == QUOTE_CHAR)
			    is_quoted = TRUE;
			if (!is_quoted && isalpha (*tvar))
			  {
			    register int j = 0;
			    register int j_buf;


			    /*
			       Try to insert the value of this TVAR (this is its TEXT).
			     */
			    if (rc_tvar[IDX (*tvar)].tvar_local.tvar_text !=
				(char *) NULL
				|| rc_tvar[IDX (*tvar)].tvar_global.
				tvar_text != (char *) NULL)
			      {
				auto char *buf;


				if (rc_tvar[IDX (*tvar)].tvar_local.
				    tvar_text != (char *) NULL)
				  {
				    if (!*rc_tvar[IDX (*tvar)].tvar_local.
					tvar_text)
				      {
					if (rc_tvar[IDX (*tvar)].tvar_global.
					    tvar_text != (char *) NULL)
					  buf =
					    rc_tvar[IDX (*tvar)].tvar_global.
					    tvar_text;
					else
					  buf =
					    rc_tvar[IDX (*tvar)].tvar_local.
					    tvar_text;
				      }
				    else
				      buf =
					rc_tvar[IDX (*tvar)].tvar_local.
					tvar_text;
				  }
				else
				  buf =
				    rc_tvar[IDX (*tvar)].tvar_global.
				    tvar_text;
				j_buf = (int) strlen (buf);
				if (fwidth != SPECIAL_VALUE)
				  {
				    if (is_fformat)
				      j = fwidth;
				    else
				      {
					register int num_hls = 0;
					auto char *ptr_buf = buf;
					auto char *tvar_text;
					auto Bool quote = FALSE;


					/*
					   Scan contents of TVAR for special texts used for highlighting.
					 */
					tvar_text =
					  (char *) my_malloc (j_buf + 1,
							      ERR_NO_MEMORY_AVAILABLE,
							      __FILE__,
							      ((long)
							       __LINE__) - 2L,
							      "tvar_text", 0);
					l = x = 0;
					LOOP
					{
					  if (*(ptr_buf + l))
					    {
					      if (*(ptr_buf + l) ==
						  RC_SPECIAL_TEXT_CHAR)
						{
						  l++;
						  switch (*(ptr_buf + l))
						    {
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
						      if (quote)
							{
							  quote = FALSE;
							  x--;
							  tvar_text[x++] =
							    *(ptr_buf + l -
							      1);
							  tvar_text[x++] =
							    *(ptr_buf + l);
							}
						      else
							num_hls++;
						      break;
						    default:
						      tvar_text[x++] =
							*(ptr_buf + l - 1);
						      tvar_text[x++] =
							*(ptr_buf + l);
						    }
						}
					      else
						{
						  if (*(ptr_buf + l) ==
						      QUOTE_CHAR)
						    quote = TRUE;
						  else
						    quote = FALSE;
						  tvar_text[x++] =
						    *(ptr_buf + l);
						}
					      l++;
					    }
					  else
					    {
					      tvar_text[x] = '\0';
					      break;
					    }
					}
					free (tvar_text);
					if (highlight_flag
					    && (ehls1s.len == 1))
					  {
					    fwidth += num_hls;
					    j = MAX (fwidth, j_buf);
					  }
					else
					  {
					    j_buf = x;
					    j = MAX (fwidth, j_buf);
					    j += (num_hls << 1);
					  }
				      }
				  }
				else
				  j = j_buf;
				if (j)
				  {
				    while ((Uint) k + j >= maxlen_max)
				      {
					resize_all_strings (maxlen_max << 1,
							    TRUE, __FILE__,
							    (long) __LINE__);
					ptr_char = *line_buffer + n;
				      }
				    if (fwidth != SPECIAL_VALUE)
				      (void) use_format (&s5, k, buf,
							 atol (buf),
							 is_sign | is_lzero |
							 is_suffix,
							 is_cformat,
							 is_lformat, is_sign,
							 is_lzero, is_suffix,
							 is_fformat, fstyle,
							 fwidth, j_buf);
				    else
				      strcat (s5, buf);
				  }
				/*
				   Skip TVAR name.
				 */
				len += (flen + 1);
				if (j)
				  k += j;
				else
				  /*
				     If TVAR is "empty", remove a possibly obsolete whitespace.
				   */
				if (isspace (s5[k - 1]) && isspace (tvar[1]))
				  s5[--k] = '\0';
			      }
			    else
			      restore_tvar = TRUE;
			  }
			else
			  restore_tvar = TRUE;
			/*
			   If TVAR isn't defined, or quoted, or an invalid
			   TVAR name is found, don't touch it.
			 */
			if (restore_tvar)
			  {
			    if ((Uint) k + 1 >= maxlen_max)
			      {
				resize_all_strings (maxlen_max << 1, TRUE,
						    __FILE__,
						    (long) __LINE__);
				ptr_char = *line_buffer + n;
			      }
			    s5[k++] = RC_TVAR_CHAR;
			    len++;
			    if (*tvar && (flen == 1))
			      {
				if ((Uint) k + 1 >= maxlen_max)
				  {
				    resize_all_strings (maxlen_max << 1, TRUE,
							__FILE__,
							(long) __LINE__);
				    ptr_char = *line_buffer + n;
				  }
				s5[k++] = *tvar;
				len++;
			      }
			    s5[k] = '\0';
			  }
			n += len;
			ptr_char += len;
			ptr_tvar = strchr (ptr_char, RC_TVAR_CHAR);
			flen = 1;
			restore_tvar = is_quoted = FALSE;
		      }
		    while (ptr_tvar != (char *) NULL);
		    /*
		       Add possibly trailing ordinary text.
		     */
		    if (*ptr_char)
		      {
			k += (int) strlen (ptr_char);
			while ((Uint) k >= maxlen_max)
			  {
			    resize_all_strings (maxlen_max << 1, TRUE,
						__FILE__, (long) __LINE__);
			    ptr_char = *line_buffer + n;
			  }
			strcat (s5, ptr_char);
		      }
		    i = k++;
		    strcpy (*line_buffer, s5);
		    /*
		       If the expansion results to a text variable assignment
		       or operation, skip the expansion loop and perform the
		       assignment/operation.
		     */
		    if ((**line_buffer == RC_TVAR_CHAR)
			&& isalpha (*(*line_buffer + 1))
			&& (*(*line_buffer + 2) == *RC_VAR_ASSIGN
			    || *(*line_buffer + 2) == *RC_TVAR_ICMD_ASSIGN
			    || *(*line_buffer + 2) == *RC_TVAR_UCMD_ASSIGN
			    || *(*line_buffer + 2) == *RC_VAR_ADD
			    || *(*line_buffer + 2) == *RC_VAR_SUB))
		      *is_tvar = ok = TRUE;
		    /*
		       Now check whether we have to analyse the line again
		       in case text variables were expanded to texts which
		       contain unquoted references of other text variables.
		     */
		    n = 0;
		    ptr_tvar = strchr (*line_buffer, RC_TVAR_CHAR);
		    if (ptr_tvar != (char *) NULL)
		      {
			do
			  {
			    if (!n && (**line_buffer != RC_TVAR_CHAR))
			      ptr_tvar--;
			    else if (n)
			      ptr_tvar--;
			    *tvar = *ptr_tvar;
			    if (!n && (**line_buffer != RC_TVAR_CHAR))
			      ptr_tvar++;
			    else if (n)
			      ptr_tvar++;
			    ptr_tvar++;
			    /*
			       Check for a TVAR format.
			     */
			    flen = decode_format (ptr_tvar, 0, &is_cformat,
						  &is_lformat, &is_sign,
						  &is_lzero, &is_suffix,
						  &is_fformat, &fstyle,
						  &fwidth);
			    if (fwidth != SPECIAL_VALUE)
			      ptr_tvar += flen;
			    /*
			       Check if a text variable reference is found
			       which needs to be expanded again.
			     */
			    if ((*tvar != QUOTE_CHAR)
				&& isalpha (*ptr_tvar)
				&& (rc_tvar[IDX (*ptr_tvar)].tvar_local.
				    tvar_text != (char *) NULL
				    || rc_tvar[IDX (*ptr_tvar)].tvar_global.
				    tvar_text != (char *) NULL))
			      {
				/*
				   Check if expansion results to invalid recursion.
				 */
				if (*ptr_tvar == the_tvar)
				  ok = TRUE;
				else
				  flen = 1;
				/*
				   Text variable reference found which needs to be expanded again.
				 */
				break;
			      }
			    n = 1;
			    while (*ptr_tvar == RC_TVAR_CHAR)
			      ptr_tvar++;
			    if (*ptr_tvar)
			      ptr_tvar = strchr (ptr_tvar, RC_TVAR_CHAR);
			    else
			      ptr_tvar = (char *) NULL;
			  }
			while (ptr_tvar != (char *) NULL);
			if (ptr_tvar == (char *) NULL)
			  ok = TRUE;
		      }
		    else
		      /*
		         All text variable references already expanded.
		       */
		      ok = TRUE;
		  }
		else
		  /*
		     No text variable references found.
		   */
		  ok = TRUE;
		/*
		   If the expansion results to an include directive,
		   skip the expansion loop and perform the assignment.
		 */
		if (**line_buffer == *RC_INCL_STMENT)
		  *is_include = ok = TRUE;
		else
		  /*
		     If the expansion results to a date variable assignment,
		     skip the expansion loop and perform the assignment.
		   */
		if (isalpha (**line_buffer))
		  *is_dvar = ok = TRUE;
	      }
	    while (!ok);
	  }
	/*
	   Check for a local date variable DVAR definition.
	 */
	if (*is_dvar)
	  set_dvar (*line_buffer, lptrs3, filename, *line_number, LOcal);
	/*
	   Check for a local text variable TVAR definition.
	 */
	if (*is_tvar)
	  set_tvar (*line_buffer, filename, *line_number, LOcal);
      }
#endif
    if (is_error)
      {
#if USE_RC
	if (mode == REsource)
	  /*
	     Error, invalid date-"part" given.
	   */
	  i = ERR_INVALID_DATE_FIELD;
	else
#endif
	  /*
	     Error, illegal NUL character found in file.
	   */
	  i = ERR_ILLEGAL_CHAR_IN_FILE;
	my_error (i, filename, *line_number, *line_buffer, 0);
      }
    if (ch == '\n')
      *line_length = i - 1;
    else
      *line_length = i;

    return (ptr_pool);
  }



  char **insert_response_file (fp, filename, opt_list, my_argc_max, my_argc,
			       my_argv) FILE *fp;
  char *filename;
  const char *opt_list;
  Uint *my_argc_max;
  int *my_argc;
  char *my_argv[];
/*!
   Tries to manage a response file @FILE argument given in the command line.
     Inserts the options and commands found in file @FILE (name delivered
     in `filename' and its file pointer in `fp') into `my_argv[]' and sets
     delivered `&my_argc' and perhaps `&my_argc_max' to according "new"
     values.  Uses the global text buffer `s1' internally.  Returns the
     build `my_argv[]' ptr vector.
*/
  {
    auto long line_number = 0L;
    auto int line_length;
    auto int in_pool = 0;
    auto char *pool = (char *) NULL;
    auto char *ptr_pool = (char *) NULL;
    auto Bool b_dummy;		/* Necessary dummy for `file_read_line()' function. */


    pool = (char *) my_malloc (BUF_LEN + 1, ERR_NO_MEMORY_AVAILABLE,
			       __FILE__, ((long) __LINE__) - 1L, "pool", 0);
    while ((ptr_pool =
	    file_read_line (fp, &s1, &in_pool, pool, ptr_pool, filename,
			    &line_number, &line_length, REsponse, &b_dummy,
			    &b_dummy, &b_dummy)) != (char *) NULL)
      if (*s1)
	{
	  if ((*s1 == *SWITCH
	       || *s1 == *SWITCH2) && s1[1] && (s1[1] != *SWITCH))
	    /*
	       Short-style option found.
	     */
	    if ((strchr (opt_list, s1[1]) != (char *) NULL)
		&& isspace (s1[2]))
	      {
		register int spaces = 1;


		/*
		   Short-style option requires an argument, which could be
		   separated by whitespace characters from the option character;
		   respect this!  This means all separating whitespace
		   characters between the option character and argument
		   will be eliminated.
		 */
		line_length = 2;
		while (s1[line_length + spaces]
		       && isspace (s1[line_length + spaces]))
		  /*
		     Count the separating whitespace characters.
		   */
		  spaces++;
		/*
		   Store needed argument of option adjacent to option character.
		 */
		while (s1[line_length + spaces])
		  {
		    s1[line_length] = s1[line_length + spaces];
		    line_length++;
		  }
		s1[line_length] = '\0';
	      }
	  /*
	     Avoid one or two letter combinations of '-', '/' or '%' characters only!
	   */
	  if (((line_length == 1) && (*s1 == *SWITCH
#if USE_RC
				      || *s1 == RC_ADATE_CHAR
#endif
				      || *s1 == *SWITCH2))
	      || ((line_length == 2) && (*s1 == *SWITCH
#if USE_RC
					 || *s1 == RC_ADATE_CHAR
#endif
					 || *s1 == *SWITCH2)
		  && (s1[1] == *SWITCH
#if USE_RC
		      || s1[1] == RC_ADATE_CHAR
#endif
		      || s1[1] == *SWITCH2)))
	    ;			/* Void, don't allocate memory */
	  else
	    {
	      if ((Uint) * my_argc >= *my_argc_max)
		{
		  /*
		     Resize the `my_argv[]' table.
		   */
		  *my_argc_max <<= 1;
		  if (*my_argc_max * sizeof (char *) > testval)
		    (*my_argc_max)--;
		  my_argv = (char **) my_realloc ((VOID_PTR) my_argv,
						  *my_argc_max *
						  sizeof (char *),
						  ERR_NO_MEMORY_AVAILABLE,
						  __FILE__,
						  ((long) __LINE__) - 3L,
						  "my_argv[my_argc_max]",
						  *my_argc_max);
		}
	      my_argv[*my_argc] = (char *) my_malloc (line_length + 1,
						      ERR_NO_MEMORY_AVAILABLE,
						      __FILE__,
						      ((long) __LINE__) - 2L,
						      "my_argv[my_argc]",
						      *my_argc);
	      strcpy (my_argv[(*my_argc)++], s1);
	    }
	}
    free (pool);

    return (my_argv);
  }



  void
    write_log_file (filename, mode, mode_txt, created_txt, argc, argv)
    const char *filename;
  const Fmode_enum mode;
  const char *mode_txt;
  const char *created_txt;
  const int argc;
  char *argv[];
/*!
   Writes the contents of the environment variable $GCAL, which is already
     stored in `argc' and the arguments of command line either into a
     response file (MODE==REsponse) or into a shell script (MODE==SCript);
     other settings to `mode' will `abort()' the program immediately.
*/
  {
    auto FILE *fp = (FILE *) NULL;
    auto int len = 0;
    register int i = 0;


    fp = fopen (filename, "w");
    if (fp != (FILE *) NULL)
      {
	switch (mode)
	  {
	  case REsponse:
	    len =
	      fprintf (fp,
		       "%c `%s' %s `%s' --- %s %02d-%s-%04d %02d%s%02d%s%02d",
		       REM_CHAR, prgr_name, mode_txt, filename, created_txt,
		       true_day, short_month_name (true_month), true_year,
		       act_hour, time_sep, act_min, time_sep, act_sec);
	    if (tz != (char *) NULL)
	      len = fprintf (fp, " %s", tz);
	    len = fprintf (fp, "\n%c\n", REM_CHAR);
	    break;
#ifdef GCAL_SHELL
	  case SCript:
# if HAVE_SYS_INTERPRETER
	    len =
	      fprintf (fp,
		       "%c%s\n%c\n%c `%s' %s `%s' --- %s %02d-%s-%04d %02d%s%02d%s%02d",
		       *SHL_REM, SHELL, *SHL_REM, *SHL_REM, prgr_name,
		       mode_txt, filename, created_txt, true_day,
		       short_month_name (true_month), true_year, act_hour,
		       time_sep, act_min, time_sep, act_sec);
# else /* !HAVE_SYS_INTERPRETER */
	    len =
	      fprintf (fp,
		       "%c `%s' %s `%s' --- %s %02d-%s-%04 %02d%s%02d%s%02d",
		       *SHL_REM, prgr_name, mode_txt, filename, created_txt,
		       true_day, short_month_name (true_month), true_year,
		       act_hour, time_sep, act_min, time_sep, act_sec);
# endif	/* !HAVE_SYS_INTERPRETER */
	    if (tz != (char *) NULL)
	      len = fprintf (fp, " %s", tz);
	    len =
	      fprintf (fp, "\n%c\n%s %c\n", *SHL_REM, prgr_name, *SHL_ESC);
	    break;
#endif /* GCAL_SHELL */
	  default:
	    /*
	       This case MUST be an internal error!
	     */
	    abort ();
	  }
	if (len != EOF)
	  {
	    for (i = 1; i < argc; i++)
	      {
		/*
		   Don't write the name of a response file or
		   of a shell script into the response file!
		 */
		if (*argv[i] == *SWITCH || *argv[i] == *SWITCH2)
		  {
		    /*
		       If the short-style option -R<NAME> or -S<NAME> is given, skip it!
		     */
		    if (*(argv[i] + 1) == 'R'
#ifdef GCAL_SHELL
			|| *(argv[i] + 1) == 'S'
#endif
		      )
		      continue;
		    if (*(argv[i] + 1) == *SWITCH)
		      {
			/*
			   Detect whether the long-style option `--response-file=ARG'
			   or `--shell-script=ARG' is given and if found,
			   don't write this option itself into the response file!
			 */
			strcpy (s4, argv[i] + 2);
			(void) eval_longopt (s4, &len);
			if (len == SYM_RESPONSE_FILE)
			  continue;
#ifdef GCAL_SHELL
			if (len == SYM_SCRIPT_FILE)
			  continue;
#endif
		      }
		  }
		if (*argv[i] == RSP_CHAR)
		  /*
		     Avoid to write a response file @FILE argument into the resulting file.
		   */
		  continue;
		switch (mode)
		  {
		  case REsponse:
		    len = fprintf (fp, "%s\n", argv[i]);
		    break;
#ifdef GCAL_SHELL
		  case SCript:
		    len = fprintf (fp, "'%s' %c\n", argv[i], *SHL_ESC);
		    break;
#endif
		  default:
		    /*
		       This case can't occur anymore, if so, it MUST be an internal error!
		     */
		    abort ();
		  }
		if (len == EOF)
		  break;
	      }
#ifdef GCAL_SHELL
	    if (mode == SCript)
	      len = fprintf (fp, " $*\n");
#endif
	    i = fclose (fp);
	  }
      }
    else
      len = EOF;
    if (i == EOF)
      len = EOF;
    if (((len != EOF)
	 && (warning_level == 1))
	|| ((len == EOF) && (warning_level == 2)) || warning_level > 2)
      {
	i = (int) strlen (filename)
	  + MAX (ehls1s.len, ehls2s.len)
	  + MAX (ehls1e.len, ehls2e.len) + LEN_SINGLE_LINE;
	if ((Uint) i >= maxlen_max)
	  resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	sprintf (s4, _("Try to write %s `%s'... %s%s%s"), mode_txt, filename,
		 (ehls1s.len !=
		  1) ? ((len == EOF) ? ehls2s.seq : ehls1s.seq) : "",
		 (len == EOF) ? _("failed") : _("success"),
		 (ehls1s.len !=
		  1) ? ((len == EOF) ? ehls2e.seq : ehls1e.seq) : "");
	print_text (stderr, s4);
	/*
	   Terminate the program in case the file can't be written!
	 */
	if ((len == EOF) && (warning_level >= WARN_LVL_MAX))
	  my_error (ERR_WRITE_FILE, __FILE__, (long) __LINE__, filename, 0);
      }
#ifdef GCAL_SHELL
    if ((mode == SCript) && (len != EOF))
      {
	/*
	   Try to make the created shell script executable!
	 */
	i =
	  (int) strlen (filename) + strlen (CHMOD_PRGR) +
	  strlen (CHMOD_OPTS) + 2;
	if ((Uint) i >= maxlen_max)
	  resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	sprintf (s4, "%s %s %s", CHMOD_PRGR, CHMOD_OPTS, filename);
	/*
	   And ignore any errors...
	 */
	(void) my_system (s4);
      }
#endif /* GCAL_SHELL */
  }



  static void
    make_absolute_filename (absolute_filename, directory, filename)
    char **absolute_filename;
  const char *directory;
  const char *filename;
/*!
   Creates an absolute file name (directory+file name) of a delivered
     file name and directory and returns it in delivered `&absolute_filename'.
*/
  {
    register int dir_len = (int) strlen (directory);
    register int fil_len = (int) strlen (filename);


    if (directory[dir_len - 1] != *DIR_SEP)
      dir_len++;
    if ((Uint) dir_len + fil_len >= maxlen_max)
      resize_all_strings (dir_len + fil_len + 1, FALSE, __FILE__,
			  (long) __LINE__);
    strcpy (*absolute_filename, directory);
    if (directory[dir_len - 1] != *DIR_SEP)
      strcat (*absolute_filename, DIR_SEP);
    strcat (*absolute_filename, filename);
  }



  static FILE *get_file_ptr (fp, filename, level, mode, is_first) FILE *fp;
  const char *filename;
  const int level;
  const Fmode_enum mode;
  Bool *is_first;
/*!
   Tries to open the file (with optional diagnostic messages on STDERR channel
     in case the `--debug[=0...WARN_LVL_MAX]' option is set) and returns a
     valid file pointer of that file, or NULL if this fails.
*/
  {
#if HAVE_SYS_STAT_H && defined(S_IFMT) && defined(S_IFREG)
    auto struct stat statbuf;


    fp = (FILE *) NULL;
    /*
       Check whether STDIN channel is wanted.
     */
    if ((*filename == '-') && (strlen (filename) == 1))
      fp = stdin;
    else
      /*
         Test if the file is a regular file, if not, ignore it!
       */
    if (!stat (filename, &statbuf))
      if ((statbuf.st_mode & S_IFMT) == S_IFREG)
	fp = fopen (filename, "r");
#else /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
    if ((*filename == '-') && (strlen (filename) == 1))
      fp = stdin;
    else
      fp = fopen (filename, "r");
#endif /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
    if ((warning_level >= 0) && (mode != REsponse) && (mode != COmmon))
#if USE_RC
      {
	if (((fp != (FILE *) NULL)
	     && (warning_level == 1))
	    || ((fp == (FILE *) NULL)
		&& (warning_level == 2)) || warning_level > 2)
	  {
	    register int i;


	    /*
	       If this function is entered the first time:
	       Print a leading NEWLINE character before any information texts.
	     */
	    if (*is_first)
	      {
		*is_first = FALSE;
		*s4 = '\0';
		print_text (stderr, s4);
	      }
	    i = (int) strlen (filename)
	      + MAX (ehls1s.len, ehls2s.len)
	      + MAX (ehls1e.len, ehls2e.len) + LEN_SINGLE_LINE;
	    if ((Uint) i >= maxlen_max)
	      resize_all_strings (i + 1, FALSE, __FILE__, (long) __LINE__);
	    if (mode == REsource || mode == HEre)
	      sprintf (s4, _("Try to open%sresource file `%s'... %s%s%s"),
		       (mode == REsource) ? " " : _(" `HERE' "), filename,
		       (ehls1s.len !=
			1) ? ((fp ==
			       (FILE *) NULL) ? ehls2s.seq : ehls1s.seq) : "",
		       (fp == (FILE *) NULL) ? _("failed") : _("success"),
		       (ehls1s.len !=
			1) ? ((fp ==
			       (FILE *) NULL) ? ehls2e.seq : ehls1e.
			      seq) : "");
	    else
	      sprintf (s4,
		       _
		       ("Try to open (level: %02d) include file `%s'... %s%s%s"),
		       level, filename,
		       (ehls1s.len !=
			1) ? ((fp ==
			       (FILE *) NULL) ? ehls2s.seq : ehls1s.seq) : "",
		       (fp == (FILE *) NULL) ? _("failed") : _("success"),
		       (ehls1s.len !=
			1) ? ((fp ==
			       (FILE *) NULL) ? ehls2e.seq : ehls1e.
			      seq) : "");
	    print_text (stderr, s4);
	  }
      }
#else /* !USE_RC */
      ;				/* Void, no text to display */
#endif /* !USE_RC */

    return (fp);
  }
