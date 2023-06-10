/* Custom strftime ISO C library routine for esp32-weather-epd.
 * Copyright (C) 2023  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* An implementation of ISO C library routine, strftime.
 *
 * This implementation uses locale info from _locale.h rather than depending on
 * system locale files.
 *
 * This is modified version of a public domain strftime implementation that
 * was retrieved from https://github.com/arnoldrobbins/strftime/.
 *
 * The C99 standard now specifies just about all of the formats that were
 * additional in the earlier versions of this file.
 *
 * For extensions from SunOS (Olson's timezone package), add TZ_EXT.
 * For VMS dates, add VMS_EXT.
 * For extensions from GNU, add GNU_EXT.
 *
 * The code for %X follows the C99 specification for the "C" locale.
 * The code for %c, and %x follows the C11 specification for the "C" locale.
 *
 * Note: No implementations for %z and %Z.
 */

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "_locale.h"
#include "_strftime.h"

#define TZ_EXT     // Olson's timezone package
#define VMS_EXT    // includes %v for VMS date format
#define POSIX_2008 // flag and fw for C, F, G, Y formats
#define GNU_EXT    // includes %P

#define range(low, item, hi) std::max(low, std::min(item, hi))

/* Returns an int indication whether or not it is a leap year.
 */
static int isleap(long year)
{
  return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
} // end isleap

/* Compute week number.
 */
static int weeknumber(const struct tm *timeptr, int firstweekday)
{
  int wday = timeptr->tm_wday;
  int ret;

  if (firstweekday == 1)
  {
    if (wday == 0) // sunday
      wday = 6;
    else
      wday--;
  }
  ret = ((timeptr->tm_yday + 7 - wday) / 7);
  if (ret < 0)
    ret = 0;
  return ret;
} // end weeknumber

/* Compute week number according to ISO 8601.
 */
static int iso8601wknum(const struct tm *timeptr)
{
  // From 1003.2:
  //  If the week (Monday to Sunday) containing January 1 has four or more days
  //    in the new year, then it is week 1;
  //  otherwise it is the highest numbered week of the previous year (52 or 53),
  //    and the next week is week 1.
  //
  // ADR: This means if Jan 1 was Monday through Thursday,
  //      it was week 1, otherwise week 52 or 53.
  //
  // XPG4 erroneously included POSIX.2 rationale text in the main body of the
  // standard. Thus it requires week 53.

  int weeknum, jan1day;

  // get week number, Monday as first day of the week
  weeknum = weeknumber(timeptr, 1);

  // What day of the week does January 1 fall on?
  // We know that
  //   (timeptr->tm_yday - jan1.tm_yday) MOD 7 ==
  //     (timeptr->tm_wday - jan1.tm_wday) MOD 7
  // and that
  //   jan1.tm_yday == 0
  // and that
  //   timeptr->tm_wday MOD 7 == timeptr->tm_wday
  // from which it follows that...

  jan1day = timeptr->tm_wday - (timeptr->tm_yday % 7);
  if (jan1day < 0)
    jan1day += 7;

  // If Jan 1 was a Monday through Thursday, it was in week 1. Otherwise it was
  // last year's highest week, which is this year's week 0.
  //
  // What does that mean?
  // - If Jan 1 was Monday, the week number is exactly right, it can never be 0.
  // - If it was Tuesday through Thursday, the weeknumber is one less than it
  //   should be, so we add one.
  // - Otherwise, Friday, Saturday or Sunday, the week number is OK, but if it
  //   is 0, it needs to be 52 or 53.

  switch (jan1day)
  {
  case 1: // Monday
    break;
  case 2: // Tuesday
  case 3: // Wednesday
  case 4: // Thursday
    weeknum++;
    break;
  case 5: // Friday
  case 6: // Saturday
  case 0: // Sunday
    if (weeknum == 0)
    {
#ifdef USE_BROKEN_XPG4
      // XPG4 (as of March 1994) says 53 unconditionally
      weeknum = 53;
#else
      // get week number of last week of last year
      struct tm dec31ly; // 12/31 last year
      dec31ly = *timeptr;
      dec31ly.tm_year--;
      dec31ly.tm_mon = 11;
      dec31ly.tm_mday = 31;
      dec31ly.tm_wday = (jan1day == 0) ? 6 : jan1day - 1;
      dec31ly.tm_yday = 364 + isleap(dec31ly.tm_year + 1900L);
      weeknum = iso8601wknum(&dec31ly);
#endif
    }
    break;
  }

  if (timeptr->tm_mon == 11)
  {
    // The last week of the year can be in week 1 of next year.
    //
    // This can only happen if
    //   M  T  W
    //   29 30 31
    //   30 31
    //   31

    int wday, mday;

    wday = timeptr->tm_wday;
    mday = timeptr->tm_mday;
    if ((wday == 1 && (mday >= 29 && mday <= 31))
     || (wday == 2 && (mday == 30 || mday == 31))
     || (wday == 3 && mday == 31) )
      weeknum = 1;
  }

  return weeknum;
} // end iso8601wknum

#ifdef POSIX_2008
/* Format a year per ISO 8601:2000 as in 1003.1
 */
static void iso_8601_2000_year(char *buf, int year, size_t fw)
{
  int extra;
  char sign = '\0';

  if (year >= -9999 && year <= 9999)
  {
    sprintf(buf, "%0*d", (int)fw, year);
    return;
  }

  // now things get weird
  if (year > 9999)
  {
    sign = '+';
  }
  else
  {
    sign = '-';
    year = -year;
  }

  extra = year / 10000;
  year %= 10000;
  sprintf(buf, "%c_%04d_%d", sign, extra, year);
}
#endif // POSIX_2008

/* The strftime() function formats the broken-down time tm according to the
 * format specification format and places the result in the character array s of
 * size max.
 */
size_t _strftime(char *s, size_t maxsize, const char *format,
                 const struct tm *timeptr)
{
  char *endp = s + maxsize;
  char *start = s;
  char tbuf[100];
  int i, w;
  long y;

#ifdef POSIX_2008
  int pad;
  size_t fw;
  char flag;
#endif // POSIX_2008

  if (s == NULL || format == NULL || timeptr == NULL || maxsize == 0)
    return 0;

  // quick check if we even need to bother
  if (strchr(format, '%') == NULL && strlen(format) + 1 >= maxsize)
    return 0;

  for (; *format && s < endp - 1; format++)
  {
    tbuf[0] = '\0';
    if (*format != '%')
    {
      *s++ = *format;
      continue;
    }
#ifdef POSIX_2008
    pad = '\0';
    fw = 0;
    flag = '\0';
    switch (*++format)
    {
    case '+':
      flag = '+';
      // fall through
    case '0':
      pad = '0';
      format++;
      break;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      break;

    default:
      format--;
      goto again;
    }
    for (; isdigit(*format); format++)
    {
      fw = fw * 10 + (*format - '0');
    }
    format--;
#endif // POSIX_2008

  again:
    switch (*++format)
    {
    case '\0':
      *s++ = '%';
      goto out;

    case '%':
      *s++ = '%';
      continue;

    case 'a': // abbreviated weekday name
      if (timeptr->tm_wday < 0 || timeptr->tm_wday > 6)
        strcpy(tbuf, "?");
      else
        strcpy(tbuf, LC_ABDAY[timeptr->tm_wday]);
      break;

    case 'A': // full weekday name
      if (timeptr->tm_wday < 0 || timeptr->tm_wday > 6)
        strcpy(tbuf, "?");
      else
        strcpy(tbuf, LC_DAY[timeptr->tm_wday]);
      break;

    case 'b': // abbreviated month name
      if (timeptr->tm_mon < 0 || timeptr->tm_mon > 11)
        strcpy(tbuf, "?");
      else
        strcpy(tbuf, LC_ABMON[timeptr->tm_mon]);
      break;

    case 'B': // full month name
      if (timeptr->tm_mon < 0 || timeptr->tm_mon > 11)
        strcpy(tbuf, "?");
      else
        strcpy(tbuf, LC_MON[timeptr->tm_mon]);
      break;

    case 'c':
      _strftime(tbuf, sizeof(tbuf), LC_D_T_FMT, timeptr);
      break;

    case 'C':
#ifdef POSIX_2008
      if (pad != '\0' && fw > 0)
      {
        size_t min_fw = (flag ? 3 : 2);

        fw = max(fw, min_fw);
        sprintf(tbuf, flag ? "%+0*ld" : "%0*ld", (int)fw,
                (timeptr->tm_year + 1900L) / 100);
      }
      else
#endif // POSIX_2008
        sprintf(tbuf, "%02ld", (timeptr->tm_year + 1900L) / 100);
      break;

    case 'd': // day of the month, 01 - 31
      i = range(1, timeptr->tm_mday, 31);
      sprintf(tbuf, "%02d", i);
      break;

    case 'D': // date as %m/%d/%y
      _strftime(tbuf, sizeof(tbuf), "%m/%d/%y", timeptr);
      break;

    case 'e': // day of month, blank padded
      sprintf(tbuf, "%2d", range(1, timeptr->tm_mday, 31));
      break;

    case 'E':
      // POSIX (now C99) locale extensions, ignored for now
      goto again;

    case 'F': // ISO 8601 date representation
    {
#ifdef POSIX_2008
      // Field width for %F is for the whole thing.
      // It must be at least 10.

      char m_d[10];
      _strftime(m_d, sizeof(m_d), "-%m-%d", timeptr);
      size_t min_fw = 10;

      if (pad != '\0' && fw > 0)
      {
        fw = max(fw, min_fw);
      }
      else
      {
        fw = min_fw;
      }

      fw -= 6; // -XX-XX at end are invariant

      iso_8601_2000_year(tbuf, timeptr->tm_year + 1900, fw);
      strcat(tbuf, m_d);
#else
      _strftime(tbuf, sizeof(tbuf), "%Y-%m-%d", timeptr);
#endif // POSIX_2008
    }
    break;

    case 'g':
    case 'G':
      // Year of ISO week.
      //
      // If it's December but the ISO week number is one,
      // that week is in next year.
      // If it's January but the ISO week number is 52 or
      // 53, that week is in last year.
      // Otherwise, it's this year.

      w = iso8601wknum(timeptr);
      if (timeptr->tm_mon == 11 && w == 1)
        y = 1900L + timeptr->tm_year + 1;
      else if (timeptr->tm_mon == 0 && w >= 52)
        y = 1900L + timeptr->tm_year - 1;
      else
        y = 1900L + timeptr->tm_year;

      if (*format == 'G')
      {
#ifdef POSIX_2008
        if (pad != '\0' && fw > 0)
        {
          size_t min_fw = 4;

          fw = max(fw, min_fw);
          sprintf(tbuf, flag ? "%+0*ld" : "%0*ld", (int)fw,
                  y);
        }
        else
#endif // POSIX_2008
          sprintf(tbuf, "%ld", y);
      }
      else
        sprintf(tbuf, "%02ld", y % 100);
      break;

    case 'h': // abbreviated month name
      if (timeptr->tm_mon < 0 || timeptr->tm_mon > 11)
        strcpy(tbuf, "?");
      else
        strcpy(tbuf, LC_ABMON[timeptr->tm_mon]);
      break;

    case 'H': // hour, 24-hour clock, 00 - 23
      i = range(0, timeptr->tm_hour, 23);
      sprintf(tbuf, "%02d", i);
      break;

    case 'I': // hour, 12-hour clock, 01 - 12
      i = range(0, timeptr->tm_hour, 23);
      if (i == 0)
        i = 12;
      else if (i > 12)
        i -= 12;
      sprintf(tbuf, "%02d", i);
      break;

    case 'j': // day of the year, 001 - 366
      sprintf(tbuf, "%03d", timeptr->tm_yday + 1);
      break;

    case 'm': // month, 01 - 12
      i = range(0, timeptr->tm_mon, 11);
      sprintf(tbuf, "%02d", i + 1);
      break;

    case 'M': // minute, 00 - 59
      i = range(0, timeptr->tm_min, 59);
      sprintf(tbuf, "%02d", i);
      break;

    case 'n': // same as \n
      tbuf[0] = '\n';
      tbuf[1] = '\0';
      break;

    case 'O':
      // POSIX (now C99) locale extensions, ignored for now
      goto again;

    case 'p': // am or pm based on 12-hour clock
      i = range(0, timeptr->tm_hour, 23);
      if (i < 12)
        strcpy(tbuf, LC_AM_STR);
      else
        strcpy(tbuf, LC_PM_STR);
      break;

#ifdef GNU_EXT
    case 'P': // Like %p but in lowercase: "am" or "pm"
      i = range(0, timeptr->tm_hour, 23);
      if (i < 12)
        strcpy(tbuf, LC_AM_STR);
      else
        strcpy(tbuf, LC_PM_STR);
      i = 0;
      while(tbuf[i] != '\0' && i != sizeof(tbuf))
      {
        tbuf[i] = tolower(tbuf[i]);
        ++i;
      }
      break;
#endif

    case 'r': // time in a.m. or p.m. notation
      _strftime(tbuf, sizeof(tbuf), LC_T_FMT_AMPM, timeptr);
      break;

    case 'R': // time as %H:%M
      _strftime(tbuf, sizeof(tbuf), "%H:%M", timeptr);
      break;

    case 's': // time as seconds since the Epoch
    {
      struct tm non_const_timeptr;

      non_const_timeptr = *timeptr;
      sprintf(tbuf, "%ld", mktime(&non_const_timeptr));
      break;
    }

    case 'S': // second, 00 - 60
      i = range(0, timeptr->tm_sec, 60);
      sprintf(tbuf, "%02d", i);
      break;

    case 't': // same as \t
      tbuf[0] = '\t';
      tbuf[1] = '\0';
      break;

    case 'T': // time as %H:%M:%S
      _strftime(tbuf, sizeof(tbuf), "%H:%M:%S", timeptr);
      break;

    case 'u':
      // ISO 8601: Weekday as a decimal number [1 (Monday) - 7]
      sprintf(tbuf, "%d", timeptr->tm_wday == 0 ? 7 : timeptr->tm_wday);
      break;

    case 'U': // week of year, Sunday is first day of week
      sprintf(tbuf, "%02d", weeknumber(timeptr, 0));
      break;

    case 'V': // week of year according ISO 8601
      sprintf(tbuf, "%02d", iso8601wknum(timeptr));
      break;

    case 'w': // weekday, Sunday == 0, 0 - 6
      i = range(0, timeptr->tm_wday, 6);
      sprintf(tbuf, "%d", i);
      break;

    case 'W': // week of year, Monday is first day of week
      sprintf(tbuf, "%02d", weeknumber(timeptr, 1));
      break;

    case 'x': // appropriate date representation
      _strftime(tbuf, sizeof(tbuf), LC_D_FMT, timeptr);
      break;

    case 'X': // appropriate time representation
      _strftime(tbuf, sizeof(tbuf), LC_T_FMT, timeptr);
      break;

    case 'y': // year without a century, 00 - 99
      i = timeptr->tm_year % 100;
      sprintf(tbuf, "%02d", i);
      break;

    case 'Y': // year with century
#ifdef POSIX_2008
      if (pad != '\0' && fw > 0)
      {
        size_t min_fw = 4;

        fw = max(fw, min_fw);
        sprintf(tbuf, flag ? "%+0*ld" : "%0*ld", (int)fw,
                1900L + timeptr->tm_year);
      }
      else
#endif // POSIX_2008
        sprintf(tbuf, "%ld", 1900L + timeptr->tm_year);
      break;

#ifdef TZ_EXT
    case 'k': // hour, 24-hour clock, blank pad
      sprintf(tbuf, "%2d", range(0, timeptr->tm_hour, 23));
      break;

    case 'l': // hour, 12-hour clock, 1 - 12, blank pad
      i = range(0, timeptr->tm_hour, 23);
      if (i == 0)
        i = 12;
      else if (i > 12)
        i -= 12;
      sprintf(tbuf, "%2d", i);
      break;
#endif

#ifdef VMS_EXT
    case 'v': // date as dd-bbb-YYYY
      sprintf(tbuf, "%2d-%3.3s-%4ld",
              range(1, timeptr->tm_mday, 31),
              LC_ABMON[range(0, timeptr->tm_mon, 11)],
              timeptr->tm_year + 1900L);
      for (i = 3; i < 6; i++)
        if (islower(tbuf[i]))
          tbuf[i] = toupper(tbuf[i]);
      break;
#endif

    default:
      tbuf[0] = '%';
      tbuf[1] = *format;
      tbuf[2] = '\0';
      break;
    }
    i = strlen(tbuf);
    if (i)
    {
      if (s + i < endp - 1)
      {
        strcpy(s, tbuf);
        s += i;
      }
      else
        return 0;
    }
  }
out:
  if (s < endp && *format == '\0')
  {
    *s = '\0';
    return (s - start);
  }
  else
    return 0;
} // end _strftime

