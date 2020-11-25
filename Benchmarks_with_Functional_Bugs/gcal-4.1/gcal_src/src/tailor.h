#ifndef __TAILOR_H
# define __TAILOR_H
/*!
*  \file tailor.h
*  \brief Machine/target dependent definitions.
*/
/*
*  Copyright (c) 2010, 2011, 2013 Free Software Foundation, Inc.
*  Copyright (c) 1994, 95, 96, 1997, 2000 Thomas Esken
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
*  $Id: tailor.h 3.01 2000/06/14 03:00:01 tom Exp $
*/



/*
*  SUB-include definitions header files  ;<
*/
# include "defines.h"
# if HAVE_CONFIG_H
#  include <config.h>
# endif

# include <unistd.h>
# include <stdlib.h>

# define GCAL_NLS
# include <locale.h>

# include <gettext.h>

# define _(Str) gettext(Str)
# define N_(Str) (Str)

# ifdef HAVE_LANGINFO_H
#  include <langinfo.h>
# endif

/*
*  Detect the machine / os.
*/
/*
*  Define the symbol MSDOS for MS-DOS machines.
*/
# if defined(__MSDOS__) || defined(__MSDOS) || defined(_MSDOS)
     /* Now let's define the symbol MSDOS generally. */
#  ifndef MSDOS
#   define MSDOS
#  endif
# endif
# if defined(__TURBOC__) || defined(__ZTC__) || defined(M_I86)
#  ifndef MSDOS
       /* Now let's define the symbol MSDOS generally. */
#   define MSDOS
#  endif
# endif
/*
*  Define the symbol OS2 for OS/2 machines.
*/
# if defined(__OS2__) || defined(__OS2) || defined(_OS2)
       /* Now let's define the symbol OS2 generally. */
#  ifndef OS2
#   define OS2
#  endif
# endif
/*
*  Microsoft-C or Borland-C or Waterloo-C under OS/2.
*/
# if defined(OS2)
#  if defined(MSDOS) || defined(__TURBOC__)
#   ifdef MSDOS
#    undef MSDOS
#   endif
#  endif
# endif
/*
*  Define the symbol LINUX for Linux machines.
*/
# if defined(__LINUX__) || defined(__LINUX) || defined(_LINUX) || defined(LINUX)
     /* Now let's define the symbol LINUX generally. */
#  ifndef LINUX
#   define LINUX
#  endif
# endif
# if defined(__linux__) || defined(__linux) || defined(_linux) || defined(linux)
     /* Now let's define the symbol LINUX generally. */
#  ifndef LINUX
#   define LINUX
#  endif
# endif
/*
*  Define the symbol UNIX for UN*X machines.
*/
# if defined(__UNIX__) || defined(__UNIX) || defined(_UNIX)
     /* Now let's define the symbol UNIX generally. */
#  ifndef UNIX
#   define UNIX
#  endif
# endif
# if defined(__unix__) || defined(__unix) || defined(_unix) || defined(unix)
     /* Now let's define the symbol UNIX generally. */
#  ifndef UNIX
#   define UNIX
#  endif
# endif
# ifdef MSDOS
#  ifdef UNIX
#   if (defined(__GNUC__) || defined(__GO32__)) && !defined(M_UNIX)
	 /* Now let's define the symbol DJG (DJ-GCC under MSDOS) for these machines. */
#    ifndef DJG
#     define DJG
#    endif
#   endif
#   undef MSDOS
#  else	/* !UNIX */
#   if defined(__i386) || defined(i386)
	 /* Now let's define the symbol SCO for these machines, too. */
#    ifndef SCO
#     define SCO
#    endif
#    undef MSDOS
#   endif
#  endif /* !UNIX */
# endif	/* !MSDOS */



/*
*  Target specific predefines.
*/
# if defined(MSDOS) || defined(DJG) || defined(WIN32)
#  define USE_PROTOTYPES 1
#  define STDC_HEADERS 1
#  define HAVE_ASSERT_H 1
#  define HAVE_CTYPE_H 1
#  define HAVE_ERRNO_H 1
#  define HAVE_SIGNAL_H 1
#  define HAVE_LIMITS_H 1
#  define HAVE_SYS_STAT_H 1
#  define HAVE_SYS_TYPES_H 1
#  define HAVE_UPPER_LOWER 1
#  define HAVE_ISDIGIT 1
#  define HAVE_ISALPHA 1
#  define HAVE_ISALNUM 1
#  define HAVE_ISSPACE 1
#  define HAVE_SIGNAL 1
#  define HAVE_STRTOL 1
#  define HAVE_STRSTR 1
#  define HAVE_STRCSPN 1
#  define HAVE_SYSTEM 1
#  define HAVE_TIME_T 1
#  define HAVE_VOID 1
#  ifdef DJG
#   define HAVE_UNISTD_H 1
#   define HAVE_MATH_H 1
#   define HAVE_LIBM 1
		   /* Directory/folder separator. */
#   define DIR2_SEP      "/"
#   if 0
#    define HAVE_REGEX_H 1
#    define HAVE_GNU_RE_COMPILE_PATTERN 1
#   endif /* 0 */
#  else	/* !DJG */
#   include <io.h>
#   define HD_TOP20CC 1
#  endif /* !DJG */
#  if USE_PAGER
		   /* Default number of terminal rows. */
#   define SCREEN_ROWS   25
#  endif
		 /* Disk/Drive separator. */
#  define DISK_SEP      ":"
		 /* Directory/folder separator. */
#  define DIR_SEP       "\\"
		 /* Path separator. */
#  define PATH_SEP      ";"
# else /* !MSDOS && !DJG && !WIN32 */
#  if defined(OS2)
#   define USE_PROTOTYPES 1
#   if !defined(__GNUC__) && !defined(__EMX__)
#    define STDC_HEADERS 1
#    define HAVE_ASSERT_H 1
#    define HAVE_CTYPE_H 1
#    define HAVE_ERRNO_H 1
#    define HAVE_LIMITS_H 1
#    define HAVE_SIGNAL_H 1
#    define HAVE_SYS_STAT_H 1
#    define HAVE_SYS_TYPES_H 1
#    define HAVE_UPPER_LOWER 1
#    define HAVE_ISDIGIT 1
#    define HAVE_ISALPHA 1
#    define HAVE_ISALNUM 1
#    define HAVE_ISSPACE 1
#    define HAVE_SIGNAL 1
#    define HAVE_STRTOL 1
#    define HAVE_STRSTR 1
#    define HAVE_STRCSPN 1
#    define HAVE_SYSTEM 1
#    define HAVE_TIME_T 1
#    define HAVE_VOID 1
#   endif /* !__GCC__ && !__EMX__ */
#   if 0
#    define HAVE_REGEX_H 1
#    define HAVE_GNU_RE_COMPILE_PATTERN 1
#    define HAVE_MATH_H 1
#    define HAVE_LIBM 1
#    define HAVE_LIBTERMCAP 1
#    define HAVE_TERMCAP_H 1
#    define HAVE_TERMIOS_H 1
#    define HAVE_TERMIOS_FUNCS 1
#    define HAVE_UNISTD_H 1
#    define HAVE_FCNTL_H 1
#    define HAVE_SYS_IOCTL_H 1
#    define HAVE_SYS_STREAM_H 1
#    define HAVE_SYS_TIME_H 1
#    define HAVE_SYS_WAIT_H 1
#    define HAVE_DUP 1
#    define HAVE_DUP2 1
#    define HAVE_STRCHR 1
#    define HAVE_STRRCHR 1
#   endif /* 0 */
#   include <io.h>
#   if USE_PAGER
		   /* Default number of terminal rows. */
#    define SCREEN_ROWS   25
#   endif
		 /* Disk/Drive separator. */
#   define DISK_SEP      ":"
		 /* Directory/folder separator. */
#   define DIR_SEP       "\\"
		 /* Path separator. */
#   define PATH_SEP      ";"
#   ifdef __GNUC__
		   /* Environment variable holding path to termcap data base file. */
#    define ENV_VAR_TCAP  "TERMCAP"
		   /* Name of termcap database file. */
#    define FNAME_TCAP    "termcap.dat"
		   /* Name of default terminal type. */
#    define DFLT_TERM     "ansi"
#   endif
#  else	/* !OS2 */
#   if defined(ACORN)
#    define USE_PROTOTYPES 1
#    define STDC_HEADERS 1
#    define HAVE_ASSERT_H 1
#    define HAVE_CTYPE_H 1
#    define HAVE_ERRNO_H 1
#    define HAVE_LIMITS_H 1
#    define HAVE_SIGNAL_H 1
#    define HAVE_SYS_STAT_H 1
#    define HAVE_SYS_TYPES_H 1
#    define HAVE_UPPER_LOWER 1
#    define HAVE_ISDIGIT 1
#    define HAVE_ISALPHA 1
#    define HAVE_ISALNUM 1
#    define HAVE_ISSPACE 1
#    define HAVE_SIGNAL 1
#    define HAVE_STRTOL 1
#    define HAVE_STRSTR 1
#    define HAVE_STRCSPN 1
#    define HAVE_TIME_T 1
#    define HAVE_VOID 1
		 /* Disk/Drive separator. */
#    define DISK_SEP      "$"
		 /* Directory/Folder separator. */
#    define DIR_SEP       "."
		 /* Path separator. */
#    define PATH_SEP      ","
		 /* Name of program search path environment variable. */
#    define ENV_VAR_PATH  "Run$Path"
#    ifdef __GNUC__
#     define HAVE_UNISTD_H 1
#     if 0
#      define HAVE_REGEX_H 1
#      define HAVE_GNU_RE_COMPILE_PATTERN 1
#     endif /* 0 */
#    endif /* !__GNUC__ */
#   else /* !ACORN */
#    if defined(AMIGA)
#     define USE_PROTOTYPES 1
#     define STDC_HEADERS 1
#     define HAVE_ASSERT_H 1
#     define HAVE_CTYPE_H 1
#     define HAVE_ERRNO_H 1
#     define HAVE_LIMITS_H 1
#     define HAVE_SIGNAL_H 1
#     define HAVE_SYS_TYPES_H 1
#     define HAVE_UPPER_LOWER 1
#     define HAVE_ISDIGIT 1
#     define HAVE_ISALPHA 1
#     define HAVE_ISALNUM 1
#     define HAVE_ISSPACE 1
#     define HAVE_SIGNAL 1
#     define HAVE_STRTOL 1
#     define HAVE_STRSTR 1
#     define HAVE_STRCSPN 1
#     define HAVE_TIME_T 1
#     define HAVE_VOID 1
#     ifdef __GNUC__
#      define HAVE_UNISTD_H 1
#      if 0
#       define HAVE_REGEX_H 1
#       define HAVE_GNU_RE_COMPILE_PATTERN 1
#      endif /* 0 */
#     endif /* !__GNUC__ */
#    else /* !AMIGA */
#     if defined(ATARI)
#      define USE_PROTOTYPES 1
#      ifdef __GNUC__
#       ifndef STDC_HEADERS
#        define STDC_HEADERS 1
#       endif
#       define HAVE_UNISTD_H 1
#       if 0
#        define HAVE_REGEX_H 1
#        define HAVE_GNU_RE_COMPILE_PATTERN 1
#       endif /* 0 */
#      else /* !__GNUC__ */
#       define STDC_HEADERS 1
#      endif /* !__GNUC__ */
#      define HAVE_ASSERT_H 1
#      define HAVE_CTYPE_H 1
#      define HAVE_ERRNO_H 1
#      define HAVE_LIMITS_H 1
#      define HAVE_SIGNAL_H 1
#      define HAVE_SYS_STAT_H 1
#      define HAVE_SYS_TYPES_H 1
#      define HAVE_UPPER_LOWER 1
#      define HAVE_ISDIGIT 1
#      define HAVE_ISALPHA 1
#      define HAVE_ISALNUM 1
#      define HAVE_ISSPACE 1
#      define HAVE_SIGNAL 1
#      define HAVE_STRTOL 1
#      define HAVE_STRSTR 1
#      define HAVE_STRCSPN 1
#      define HAVE_TIME_T 1
#      define HAVE_VOID 1
		 /* Directory/folder separator. */
#      define DIR_SEP       "\\"
		 /* Path separator. */
#      define PATH_SEP      ","
#     else /* !ATARI */
#      if defined(__50SERIES) || defined(PRIMOS)
		 /* Prime/PRIMOS. */
#       define USE_PROTOTYPES 1
#       define STDC_HEADERS 1
#       define HAVE_ASSERT_H 1
#       define HAVE_CTYPE_H 1
#       define HAVE_ERRNO_H 1
#       define HAVE_LIMITS_H 1
#       define HAVE_SIGNAL_H 1
#       define HAVE_SYS_STAT_H 1
#       define HAVE_SYS_TYPES_H 1
#       define HAVE_UPPER_LOWER 1
#       define HAVE_ISDIGIT 1
#       define HAVE_ISALPHA 1
#       define HAVE_ISALNUM 1
#       define HAVE_ISSPACE 1
#       define HAVE_SIGNAL 1
#       define HAVE_STRTOL 1
#       define HAVE_STRSTR 1
#       define HAVE_STRCSPN 1
#       define HAVE_TIME_T 1
#       define HAVE_VOID 1
		 /* Directory/folder separator. */
#       define DIR_SEP       ">"
#      else /* !__50SERIES */
#       if defined(VAXC) || defined(VMS)
		 /* DEC Vax. */
		 /* Now let's define the symbol UNIX as default. */
#        ifndef UNIX
#         define UNIX
#        endif
#        define USE_PROTOTYPES 1
#        ifdef __GNUC__
#         define HAVE_REGEX_H 1
#         define HAVE_GNU_RE_COMPILE_PATTERN 1
#        else /* !__GNUC__ */
#        endif /* !__GNUC__ */
#        define STDC_HEADERS 1
#        define HAVE_ASSERT_H 1
#        define HAVE_CTYPE_H 1
#        define HAVE_ERRNO_H 1
#        define HAVE_LIMITS_H 1
#        define HAVE_SIGNAL_H 1
#        define HAVE_SYS_STAT_H 1
#        define HAVE_SYS_TYPES_H 1
#        define HAVE_UPPER_LOWER 1
#        define HAVE_ISDIGIT 1
#        define HAVE_ISALPHA 1
#        define HAVE_ISALNUM 1
#        define HAVE_ISSPACE 1
#        define HAVE_SIGNAL 1
#        define HAVE_STRTOL 1
#        define HAVE_STRSTR 1
#        define HAVE_STRCSPN 1
#        define HAVE_SYSTEM 1
#        define HAVE_TIME_T 1
#        define HAVE_VOID 1
#        ifdef VAXC
#         include <unixio.h>
#        endif
		 /* Disk/Drive separator. */
#        define DISK_SEP       ":"
		 /* Directory/folder separator. */
#        define DIR_SEP        "]"
		 /* Suffix (version number) separator. */
#        define SUFFIX_SEP     ";"
		 /* Name of programs default options variable. */
#        define ENV_VAR_GCAL   "GCAL_OPT"
#        ifndef unlink
#         define unlink(file)  delete((file))
#        endif
#       else
		 /*
		    default: UNIX.  Let `configure' detect what's in store .)
		    Now let's define the symbol UNIX (NOT GNU or RMS, hehe) as default.
		  */
#        ifndef UNIX
#         define UNIX
#        endif
#       endif /* default: UNIX */
#      endif /* !__50SERIES */
#     endif /* !ATARI */
#    endif /* !AMIGA */
#   endif /* !ACORN */
#  endif /* !OS2 */
# endif	/* !MSDOS && !DJG && !WIN32 */

/*
*  Compiler specific macro for prototyping.
*/
# ifndef __P_
#  if (defined(__STDC__) && __STDC__) || defined(__cplusplus) || USE_PROTOTYPES
#   define __P_(gc_proto)  gc_proto
#  else	/* !__STDC__ && !__cplusplus && !USE_PROTOTYPES */
#   define __P_(gc_proto)  ()
#  endif /* !__STDC__ && !__cplusplus && !USE_PROTOTYPES */
# endif



/*
*  Function prototype begin and end declarations.
*/
# ifdef __cplusplus
#  ifndef __BEGIN_DECLARATIONS
#   define __BEGIN_DECLARATIONS  extern "C" {
#  endif
#  ifndef __END_DECLARATIONS
#   define __END_DECLARATIONS    }
#  endif
# else /* !__cplusplus */
#  ifndef __BEGIN_DECLARATIONS
#   define __BEGIN_DECLARATIONS
#  endif
#  ifndef __END_DECLARATIONS
#   define __END_DECLARATIONS
#  endif
# endif	/* !__cplusplus */



/*
*  Action specific preprocessor constants.
*/
/*
*  Termcap action is only allowed on these systems.
*/
# ifndef DJG
#  if defined(UNIX) || (defined(OS2) && defined(__GNUC__))
#   if HAVE_TTYLIBS
#    define GCAL_TCAP
#   endif
#  endif
# endif

/*!
*  Writing a shell script is only allowed on these systems.
*/
# if defined(UNIX) && !defined(DJG)
#  define GCAL_SHELL
# endif

/*!
*  Using an external pager is only allowed on these systems.
*/
# if defined(UNIX) && !defined(DJG) && USE_PAGER
#  define GCAL_EPAGER
# endif

/*!
*  Email option is only allowed on these systems.
*/
# if defined(__GNUC__) || defined(UNIX) || defined(OS2) || defined(MSDOS) || defined(DJG) || defined(WIN32)
#  define GCAL_EMAIL
# endif



/*
*  Target specific preprocessor statements.
*/
# ifdef GCAL_TCAP
/* Name of terminal mode/type environment varable. */
#  ifndef ENV_VAR_TERM
#   define ENV_VAR_TERM  "TERM"
#  endif
# endif

# ifdef GCAL_SHELL
/*! The comment character used by shell scripts. */
#  define SHL_REM  "#"

/*! The escape character used by shell scripts. */
#  define SHL_ESC       "\\"

/*! Name of default `chmod' program. */
#  ifndef CHMOD_PRGR
#   define CHMOD_PRGR  "chmod"
#  endif

/*! Make created shell script executable for all users. */
#  define CHMOD_OPTS  "+x"

/*! ID for the first line of a shell script. */
#  if HAVE_SYS_INTERPRETER && !defined(SHELL)
#   define SHELL  "! /bin/sh"
#  endif
# endif

# ifdef GCAL_EMAIL
/*! Input redirection symbol. */
#  ifndef REDIRECT_IN
#   define REDIRECT_IN        "<"
#  endif

/*! Stores the name of default mailer program in environment. */
#  ifndef ENV_VAR_MAILPROG
#   define ENV_VAR_MAILPROG  "MAILPROG"
#  endif

/*! Name of default eMail address environment variable 1. */
#  ifndef ENV_VAR_MAILTO
#   define ENV_VAR_MAILTO    "MAILTO"
#  endif

/*! Name of default eMail address environment variable 2. */
#  ifndef ENV_VAR_USER
#   define ENV_VAR_USER      "USER"
#  endif

/*! Name of default eMail address environment variable 3. */
#  ifndef ENV_VAR_LOGNAME
#   define ENV_VAR_LOGNAME   "LOGNAME"
#  endif

/*! Name of default mailer if $MAILPROG is not set. */
#  ifndef MAIL_PRGR
#   define MAIL_PRGR         "mail"
#  endif
# endif	/* GCAL_EMAIL */



# if USE_RC || defined(GCAL_EMAIL)
/* The proper function for a temporary file name according to used target system. */
#  ifdef MSDOS
#   define TMPFILENAME  tempnam(NULL, NULL)
#  else	/* !MSDOS */
# include <tmpdir.h>
# include <stdio.h>

static char __buftmpfn[2048];
static char *_tmpfn ()
{
  int fd;
  if (path_search (__buftmpfn, sizeof (__buftmpfn), NULL, "gcal", false) < 0)
    return NULL;

  fd = mkostemp (__buftmpfn, 0);
  if (fd < 0)
    return NULL;

  close (fd);
  return __buftmpfn;
}

#   define TMPFILENAME  _tmpfn ()
#  endif /* !MSDOS */
# endif	/* GCAL_EMAIL || USE_RC */



/*! Default number of terminal columns, unconditional. */
# ifndef SCREEN_COLS
#  define SCREEN_COLS   80
# endif



# if USE_PAGER
/*! Default number of terminal rows. */
#  ifndef SCREEN_ROWS
#   define SCREEN_ROWS    24
#  endif

#  ifdef GCAL_EPAGER
/*! Name of external pager program environment variable. */
#   define ENV_VAR_PAGER  "PAGER"

/*! Name of preferred external pager program. */
#   ifndef PAGER1_PROG
#    define PAGER1_PROG    "less"
#   endif

/*! Name of alternative external pager program. */
#   ifndef PAGER2_PROG
#    define PAGER2_PROG    "more"
#   endif

/*! Name of second alternative external pager program. */
#   ifndef PAGER3_PROG
#    define PAGER3_PROG    "pg"
#   endif
#  endif /* GCAL_EPAGER */
# endif	/* USE_PAGER */



# ifdef GCAL_NLS
/*! Name of first used national language environment variable. */
#  define ENV_VAR_LANGUAGE     "LANGUAGE"

/*! Name of second used national language environment variable. */
#  define ENV_VAR_LC_ALL       "LC_ALL"

/*! Name of third used national language environment variable. */
#  if HAVE_LC_MESSAGES
#   define ENV_VAR_LC_MESSAGES  "LC_MESSAGES"
#  endif

/*! Name of fourth used national language environment variable. */
#  define ENV_VAR_LANG         "LANG"
# endif



/*! The character used to specify the actual directory folder. */
# ifndef ACTUAL_DIR
#  define ACTUAL_DIR    "."
# endif

/*! Default directory/folder separator. */
# ifndef DIR_SEP
#  define DIR_SEP       "/"
# endif

/*! Default path separator. */
# ifndef PATH_SEP
#  define PATH_SEP      ":"
# endif

/*! Name of home directory environment variable. */
# ifndef ENV_VAR_HOME
#  define ENV_VAR_HOME  "HOME"
# endif

/*! Name of program search path environment variable. */
# ifndef ENV_VAR_PATH
#  define ENV_VAR_PATH  "PATH"
# endif

/*! Name of data search path environment variable. */
# ifndef ENV_VAR_GCALPATH
#  define ENV_VAR_GCALPATH  "GCALPATH"
# endif

/*! Name of programs default options variable. */
# ifndef ENV_VAR_GCAL
#  define ENV_VAR_GCAL  "GCAL"
# endif

/*! Name of variable for setting ANSI-sequences to default. */
# ifndef ENV_VAR_GCALANSI
#  define ENV_VAR_GCALANSI  "GCALANSI"
# endif

/*! Name of variable for ordering the date. */
# ifndef ENV_VAR_GCAL_DATE_FORMAT
#  define ENV_VAR_GCAL_DATE_FORMAT  "GCAL_DATE_FORMAT"
# endif



/*
*  Define how the German `Umlaute' letters and other accent characters
*    are displayed and how the default marking characters are displayed.
*/
# if USE_EASC
#  if defined(NeXT)
	   /* NeXTstep character set. */
#   define AE          "\303"
#   define OE          "\324"
#   define UE          "\300"
#   define SZ          "\222"
#   define AAE         "\316"
#   define OOE         "\333"
#   define UUE         "\232"
#   define ARA         "\251"
#   define ERA_M       "\301"
#   define ELA         "\312"
#   define ADG         "\302"
#   define EDG         "\310"
#   define ODG         "\323"
#   define CD          "\200"
	   /* Marking character 1 start (current day). */
#   define BUF_HLS1S   "<"
	   /* Marking character 1 end (current day). */
#   define BUF_HLS1E   ">"
	   /* Marking character 2 start (holiday). */
#   define BUF_HLS2S   ":"
	   /* Marking character 2 end (holiday). */
#   define BUF_HLS2E   ":"
#  else
#   if defined(ATARI)
	   /* Atari character set. */
#    define AE          "\204"
#    define OE          "\224"
#    define UE          "\201"
#    define SZ          "\377"
#    define AAE         "\216"
#    define OOE         "\231"
#    define UUE         "\232"
#    define ARA         "\240"
#    define ERA_M       "\202"
#    define ELA         "\212"
#    define ADG         "\203"
#    define EDG         "\210"
#    define ODG         "\223"
#    define CD          "\207"
	   /* Marking character 1 start (current day). */
#    define BUF_HLS1S   "<"
	   /* Marking character 1 end (current day). */
#    define BUF_HLS1E   ">"
	   /* Marking character 2 start (holiday). */
#    define BUF_HLS2S   ":"
	   /* Marking character 2 end (holiday). */
#    define BUF_HLS2E   ":"
#   else
#    if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(SCO) || defined(COHERENT)
	   /* IBM PC character set. */
#     define AE          "\204"
#     define OE          "\224"
#     define UE          "\201"
#     define SZ          "\341"
#     define AAE         "\216"
#     define OOE         "\231"
#     define UUE         "\232"
#     define ARA         "\240"
#     define ERA_M       "\202"
#     define ELA         "\212"
#     define ADG         "\203"
#     define EDG         "\210"
#     define ODG         "\223"
#     define CD          "\207"
	   /*! Marking character 1 start (current day). */
#     define BUF_HLS1S   "\021"
	   /*! Marking character 1 end (current day). */
#     define BUF_HLS1E   "\020"
	   /*! Marking character 2 start (holiday). */
#     define BUF_HLS2S   "\256"
	   /*! Marking character 2 end (holiday). */
#     define BUF_HLS2E   "\257"
#    else /* !IBM PC character set. */
	   /* (defined(UNIX) && !defined(SCO) && !defined(COHERENT) && !defined(NeXT)) || defined(AMIGA) || ... */
	   /* ISO Latin-1 character set. */
#     define AE          "\344"
#     define OE          "\366"
#     define UE          "\374"
#     define SZ          "\337"
#     define AAE         "\304"
#     define OOE         "\326"
#     define UUE         "\334"
#     define ARA         "\301"
#     define ERA_M       "\351"
#     define ELA         "\350"
#     define ADG         "\342"
#     define EDG         "\352"
#     define ODG         "\364"
#     define CD          "\347"
	   /*! Marking character 1 start (current day). */
#     define BUF_HLS1S   "<"
	   /*! Marking character 1 end (current day). */
#     define BUF_HLS1E   ">"
	   /*! Marking character 2 start (holiday). */
#     define BUF_HLS2S   ":"
	   /*! Marking character 2 end (holiday). */
#     define BUF_HLS2E   ":"
#    endif /* !IBM PC character set. */
#   endif /* Atari character set. */
#  endif /* NeXTstep character set. */
# else /* !USE_EASC */
#  define AE          "\"a"
#  define OE          "\"o"
#  define UE          "\"e"
#  define SZ          "ss"
#  define AAE         "\"A"
#  define OOE         "\"O"
#  define UUE         "\"U"
#  define ARA         "'a"
#  define ERA_M       "'e"
#  define ELA         "`e"
#  define ADG         "^a"
#  define EDG         "^e"
#  define ODG         "^o"
#  define CD          ",c"
     /*! Marking character 1 start (current day). */
#  define BUF_HLS1S    "<"
     /*! Marking character 1 end (current day). */
#  define BUF_HLS1E    ">"
     /*! Marking character 2 start (holiday). */
#  define BUF_HLS2S    ":"
     /*! Marking character 2 end (holiday). */
#  define BUF_HLS2E    ":"
# endif	/* !USE_EASC */



/*
*  Define the default highlighting sequences.
*/
# if USE_HLS
#  if defined(MSDOS) || defined(OS2) || defined(LINUX) || defined(DJG)
       /*! Highlighting sequence 1 start (current day). */
#   define HLS1S  "\033[1m"
       /*! Highlighting sequence 2 start (holiday). */
#   define HLS2S  "\033[36m"
#  else	/* !MSDOS && !OS2 && !LINUX && !DJG */
       /*! Highlighting sequence 1 start (current day). */
#   define HLS1S  "\033[7m"
       /*! Highlighting sequence 2 start (holiday). */
#   define HLS2S  "\033[1m"
#  endif /* !MSDOS && !OS2 && !LINUX && !DJG */
     /*! Highlighting sequence 1 end (current day). */
#  define HLS1E       "\033[0m"
     /*! Highlighting sequence 2 end (holiday). */
#  define HLS2E       "\033[0m"
     /*! Termcap mode character HLS 1 start (current day). */
#  define TC_MC_HL1S  "mr"
     /*! Termcap mode character HLS 1 end (current day). */
#  define TC_MC_HL1E  "me"
     /*! Termcap mode character HLS 2 start (holiday). */
#  define TC_MC_HL2S  "md"
     /*! Termcap mode character HLS 2 end (holiday). */
#  define TC_MC_HL2E  "me"
# else /* !USE_HLS */
     /*! Highlighting sequence 1 start (current day). */
#  define HLS1S       BUF_HLS1S
     /*! Highlighting sequence 1 end (current day). */
#  define HLS1E       BUF_HLS1E
     /*! Highlighting sequence 2 start (holiday). */
#  define HLS2S       BUF_HLS2S
     /*! Highlighting sequence 2 end (holiday). */
#  define HLS2E       BUF_HLS2E
# endif	/* !USE_HLS */
/*! Maximum number of termcap mode chars/single HL sequences used. */
# define TC_MC_MAX  4
/*! Print blank character if highlighting is disabled. */
# define NO_HLS  " "

#endif /* __TAILOR_H */
