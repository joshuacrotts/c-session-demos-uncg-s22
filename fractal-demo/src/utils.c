#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define BILLION 1000000000.0f

/**
 * Normalizes a value n from one domain to another.
 * 
 * @param long double - number to normalize.
 * @param long double - old min to normalize from.
 * @param long double - old max to normalize from.
 * @param long double - new min to normalize to.
 * @param long double - new max to normalize to.
 * 
 * @return normalized value.
 */
long double normalize(long double n, long double old_min, long double old_max, long double new_min, long double new_max) {
    return (((n - old_min) * (new_max - new_min)) / (old_max - old_min)) + new_min;
}

/**
 * Stores the current wallclock time in the timespec struct.
 * 
 * @param struct timespec * pointer to store wallclock time in.
 * 
 * @return void. 
 */
void get_current_time(struct timespec *time) {
    if (clock_gettime(CLOCK_MONOTONIC_RAW, time) < 0) {
        fprintf(stderr, "Could not compute clock_gettime with CLOCK_MONOTONIC_RAW: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * Computes the time difference between two timespec objects.
 * 
 * @param struct timespec * pointer to ending time.
 * @param struct timespec * pointer to start time.
 * 
 * @return float time difference in seconds.
 */
float get_time_diff(struct timespec *end, struct timespec *start) {
    return (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / BILLION;
}
