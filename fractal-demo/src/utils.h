#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <time.h>

long double normalize(long double n, long double old_min, long double old_max, long double new_min, long double new_max);
void get_current_time(struct timespec *time);
float get_time_diff(struct timespec *end, struct timespec *start);

#endif  // UTILS_H
