#pragma once

#include <time.h>

int parseTimeStr(const char* timeStr);
time_t getNextUpdateTime(time_t currentTime);
uint64_t getSleepDurationSeconds(time_t now);
