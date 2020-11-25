#include "config.h"
#include <stdlib.h>

#ifndef HAVE_STRSEP
char *
strsep (string, delimiters)
     char **string;
     char *delimiters;
{
  char *return_val;
  char *end_point;
  if (*string == NULL)
    return NULL;
  return_val = *string;
  end_point = *string;
  while (*end_point != '\0' && strchr (delimiters, *end_point) == NULL)
    end_point++;
  if (*end_point == '\0')
    *string = NULL;
  else {
    *end_point = '\0';
    end_point++;
    *string = end_point;
    }
  return return_val;
  }
#endif /* No HAVE_STRSEP */

#ifndef HAVE_POW
/* This version works for integral exponents only. */
double
pow (x, y)
     double x;
     double y;
{
  int i;
  double result;
  result = 1;
  for (i = 0; i < y; i++)
    result *= x;
  return result;
  }
#endif /* No HAVE_POW */

#ifndef HAVE_BZERO
void
bzero (void *ptr, size_t length)
{
  char *temp_ptr;

  temp_ptr = ptr;
  while (length > 0) {
    *temp_ptr = 0;
    temp_ptr++;
    length--;
    }
  }
#endif /* No HAVE_BZERO */
