#include "config.h"

#ifndef HAVE_STRSEP
char *strsep (char **, char *);
#endif
#ifndef HAVE_POW
double pow (double, double);
#endif
#ifndef HAVE_BZERO
void bzero (void *, size_t);
#endif
