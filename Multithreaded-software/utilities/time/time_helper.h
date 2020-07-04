/*
 *
 * @file time_helper.c
 * @brief Header of time helper functions
 *
 * This file is the header of timespec helper functions
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#ifndef TIME_HELPER_
#define TIME_HELPER_

#include <time.h>
#include <stdint.h>

#define NSEC_PER_SEC 1000000000

/* add ns to timespec */

void add_timespec(struct timespec *ts, int64_t addtime);

#endif // End of TIME_HELPER_
