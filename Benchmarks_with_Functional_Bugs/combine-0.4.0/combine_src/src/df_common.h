/* combine - flexible file merging    
   Copyright (C) 2002, 2003, 2004, 2013 Daniel P. Valentine

   This file was written by Daniel P. Valentine <dpv@world.std.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef DF_COMMON_H_GUARD
#define DF_COMMON_H_GUARD

/* Make sure we don't try assembly. */
#define _INTL_REDIRECT_MACROS

#include "../config.h"
#include <stdlib.h>
#include <math.h>
#include "dstring.h"
#include "extras.h"
#ifdef HAVE_LIBGUILE_H
#  include <libguile.h>
#else
#  ifdef HAVE_LIBGUILE
#  include <guile/gh.h>
#    endif /* HAVE_LIBGUILE */
#  endif /* HAVE_LIBGUILE_H */
#ifdef HAVE_LIBINTL_H
#  include <libintl.h>
#  endif

#define DEBUG 0

#define DEFAULT_HASHTABLE_SIZE 32767
#define MAX_VALUE_SIZE 32767
#define INPUT_BUFFER_SIZE 40000
#define DF_BUFFER_INCREMENT 1000000
#define DF_HRCHY_INCREMENT 20

#ifdef HAVE_LONG_LONG
#define   HASHTYPE unsigned long long
#define   COUNTTYPE unsigned long long
#define   COUNTFORMAT "%*lld"
#define   COUNTFRACTFORMAT "%0*lld"
#else /* no long longs */
#define   HASHTYPE unsigned long int
#define   COUNTTYPE unsigned long int
#define   COUNTFORMAT "%*ld"
#define   COUNTFRACTFORMAT "%0*ld"
#endif /* no long longs */

#ifdef HAVE_STRTOLL
#define   CONVERT_SUM(x) strtoll (x, NULL, 10)
#define   CONVERT_SUM_PRECISION(x, y, z) do { \
                                           char* remainder; \
                                           int amt_sign; \
                                           COUNTTYPE temp_sum_value; \
                                           long int itmp; \
                                           amt_sign = 1; \
                                           temp_sum_value = strtoll ((x), &remainder, 10) * pow (10, (y)); \
                                           if (temp_sum_value < 0) amt_sign = -1; \
                                           else if (temp_sum_value == 0) { \
                                             if (remainder == (x)) remainder = strchr ((x), '.'); \
                                             if (remainder != NULL && *remainder == '-') { \
                                               amt_sign = -1; \
                                               remainder++; \
                                               } \
                                             else if (remainder != NULL && strchr ((x), '-') != NULL) amt_sign = -1; \
                                             } \
                                           if (remainder != NULL && *remainder == '.') { \
                                             remainder++; \
                                             for (itmp=0; itmp<(y); itmp++) { \
                                                 if (remainder[itmp] == '\0') break; \
                                               } \
                                             if (remainder[itmp] != '\0') remainder[itmp] = '\0'; \
                                             temp_sum_value += amt_sign * strtoll (remainder, NULL, 10) * pow (10, (y) - itmp); \
                                             } \
                                           (z) += temp_sum_value; \
                                           } while (0)
#else /* no strtoll */
#define   CONVERT_SUM(x) atol (x)
#define   CONVERT_SUM_PRECISION(x, y, z) do { \
                                           char* remainder; \
                                           int amt_sign; \
                                           COUNTTYPE temp_sum_value; \
                                           long int itmp; \
                                           amt_sign = 1; \
                                           temp_sum_value = strtol ((x), &remainder, 10) * pow (10, (y)); \
                                           if (temp_sum_value < 0) amt_sign = -1; \
                                           else if (temp_sum_value == 0) { \
                                             if (remainder == (x)) remainder = strchr ((x), '.'); \
                                             if (remainder != NULL && *remainder == '-') { \
                                               amt_sign = -1; \
                                               remainder++; \
                                               } \
                                             else if (remainder != NULL && strchr ((x), '-') != NULL) amt_sign = -1; \
                                             } \
                                           if (remainder != NULL && *remainder == '.') { \
                                             remainder++; \
                                             for (itmp=0; itmp<(y); itmp++) { \
                                                 if (remainder[itmp] == '\0') break; \
                                               } \
                                             if (remainder[itmp] != '\0') remainder[itmp] = '\0'; \
                                             temp_sum_value += amt_sign * strtol (remainder, NULL, 10) * pow (10, (y) - itmp); \
                                             } \
                                           (z) += temp_sum_value; \
                                           } while (0)
#endif /* no strtoll */

#define STRINGTYPE DStr_string_descriptor

#define PROGRAM_NAME "combine"
#define AUTHORS "Daniel P. Valentine"

#ifdef HAVE_GETTEXT
#  define _(String) gettext (String)
#else
#  define _(String) (String)
#  endif

#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef ISDIGIT
# define ISDIGIT(a) ('0'<=(a)<='9')
#endif

#ifndef ISBLANK
# define ISBLANK(a) ((a)==' ')
#endif

#define FATAL_ERROR(Message)                                            \
    do {                                                                \
      fprintf (stderr, program_name);                                   \
      fprintf (stderr, ": ");                                           \
      fprintf (stderr, (Message));                                      \
      fprintf (stderr, "\n");                                           \
      exit(EXIT_FAILURE);                                               \
      } while(0)                                                        \

#define FATAL_ERROR1(Message,Arg1)                                      \
    do {                                                                \
      fprintf (stderr, program_name);                                   \
      fprintf (stderr, ": ");                                           \
      fprintf (stderr, (Message), (Arg1));                     \
      fprintf (stderr, "\n");                                           \
      exit(EXIT_FAILURE);                                               \
      } while(0)                                                        \

#define FATAL_ERROR2(Message,Arg1,Arg2)                                 \
    do {                                                                \
      fprintf (stderr, program_name);                                   \
      fprintf (stderr, ": ");                                           \
      fprintf (stderr, (Message), (Arg1), (Arg2));             \
      fprintf (stderr, "\n");                                           \
      exit(EXIT_FAILURE);                                               \
      } while(0)                                                        \

#define FATAL_ERROR3(Message,Arg1,Arg2,Arg3)                            \
    do {                                                                \
      fprintf (stderr, program_name);                                   \
      fprintf (stderr, ": ");                                           \
      fprintf (stderr, (Message), (Arg1), (Arg2), (Arg3));     \
      fprintf (stderr, "\n");                                           \
      exit(EXIT_FAILURE);                                               \
      } while(0)                                                        \

#define FATAL_PERROR(Message)                                           \
    do {                                                                \
      char* tmp_str;                                                    \
      tmp_str = malloc(strlen((Message)) + strlen(program_name) + 3);    \
      strcpy(tmp_str,program_name);                                     \
      strcat(tmp_str,": ");                                             \
      strcat(tmp_str,(Message));                               \
      perror (tmp_str);                                                 \
      free (tmp_str)  ;                                                 \
      exit(EXIT_FAILURE);                                               \
      } while(0)                                                        \

struct s_val_entry
{
  HASHTYPE hash_key;
  int another_for_key;
  STRINGTYPE *value;
  STRINGTYPE *chain_key;
  STRINGTYPE **output_fields;
#ifdef HAVE_LIBGUILE
  SCM **extension_data;
#endif
  COUNTTYPE **counters;
  void *info;
  };

typedef struct s_val_entry val_entry;



/* The name this program was run with. */
extern char *program_name;



#endif
