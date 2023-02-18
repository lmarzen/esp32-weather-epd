#ifndef __C_STRFTIME_H__
#define __C_STRFTIME_H__

#include <time.h>

size_t c_strftime(char *s, size_t maxsize, const char *format,
                  const struct tm *timeptr);

#endif