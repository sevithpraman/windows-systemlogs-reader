#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_COUNT 26
void logger(const char* tag, const char* message) {
   time_t now;
   struct tm _calendar_time;
   char _buf[MAX_COUNT];

   time(&now);
   localtime_s(&_calendar_time, &now);

   strftime(_buf, MAX_COUNT, "%c", &_calendar_time);
   printf("%s [%s]: %s\n", _buf, tag, message);
}

int main() {
   logger("main", "Hello, World!");
   return EXIT_SUCCESS;
}