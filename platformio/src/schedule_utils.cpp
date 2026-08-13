#include "schedule_utils.h"
#include "config.h"

const int SECONDS_IN_MINUTE = 60;
const int SECONDS_IN_HOUR = 60 * SECONDS_IN_MINUTE;
const int SECONDS_IN_DAY = 24 * SECONDS_IN_HOUR;
const int MINUTES_IN_HOUR = 60;
const int MINUTES_IN_DAY = 24 * MINUTES_IN_HOUR;
const int DAYS_IN_WEEK = 7;
const int ERROR_SLEEP_SECONDS = SECONDS_IN_HOUR;

// Helper: Parses a time string in "HH:MM" format and returns minutes since midnight.
int parseTimeStr(const char *timeStr) {
  int hour = 0, minute = 0;
  sscanf(timeStr, "%d:%d", &hour, &minute);
  return hour * 60 + minute;
}

// Computes and returns the next scheduled update time (as a time_t) based on the current time.
time_t getNextUpdateTime(time_t currentTime) {
  struct tm currentTM;
  localtime_r(&currentTime, &currentTM);
  int currentMinutesSinceMidnight = currentTM.tm_hour * MINUTES_IN_HOUR + currentTM.tm_min;

  // Check today (offset 0) and the next 7 days.
  for (int offsetDays = 0; offsetDays <= 7; offsetDays++) {
    time_t candidateDayTime = currentTime + offsetDays * SECONDS_IN_DAY;
    struct tm candidateTM;
    localtime_r(&candidateDayTime, &candidateTM);
    int day = candidateTM.tm_wday;
    int candidateMinutes = INT_MAX; // Will hold the candidate time in minutes since midnight. INT_MAX means no candidate found.

    // Iterate over all schedule segments applicable on this day.
    for (int i = 0; i < scheduleSegmentsCount; i++) {
      if (!(scheduleSegments[i].dayMask & (1 << day))) {
        log_d("Skipping segment no. %d", i);
        continue;
      }
      int segStart = parseTimeStr(scheduleSegments[i].startTime);
      int segEnd = parseTimeStr(scheduleSegments[i].endTime);

      // Today: consider the intervals within the segment.
      if (offsetDays == 0) {
        int intervalSteps = (segEnd - segStart) / scheduleSegments[i].interval; // Number of intervals in the segment. Are only needed for today.
        for (int j = 0; j <= intervalSteps; j++) {
          int possibleWakeupTime = segStart + scheduleSegments[i].interval * j; // in minutes since midnight. Is only needed for today.
          if (possibleWakeupTime <= currentMinutesSinceMidnight) {
            log_d("Skipping time in the past.");
            continue;
          }
          if (possibleWakeupTime < candidateMinutes) { // Update the candidate time if this one is earlier.
            candidateMinutes = possibleWakeupTime;
          }
        }
      } else { // Future day: take the segment's start time.
        if (segStart < candidateMinutes) { // Update the candidate time if this one is earlier.
          candidateMinutes = segStart;
        }
      }
    }

    // If a candidate time was found, build the time_t from the candidate minutes.
    if (candidateMinutes != INT_MAX) {
      candidateTM.tm_hour = candidateMinutes / SECONDS_IN_MINUTE;
      candidateTM.tm_min = candidateMinutes % SECONDS_IN_MINUTE;
      candidateTM.tm_sec = 0;
      return mktime(&candidateTM);
    }
  }

  log_e("No candidate found for the next 7 days. Returning currentTime + one hour.");
  return currentTime + ERROR_SLEEP_SECONDS;
}

// Returns the sleep duration (in seconds) until the next scheduled update.
uint64_t getSleepDurationSeconds(time_t now) {
  time_t nextUpdate = getNextUpdateTime(now);
  return (uint64_t)(nextUpdate - now);
}
