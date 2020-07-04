/*
 *
 * @file time_helper.c
 * @brief Implementation of time helper functions
 *
 * This file contains the implementation of timespec helper functions
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */
#include "time_helper.h"
#include <stdio.h>
#include <inttypes.h>

// Needs the definition of int64
// The definition comes from ethercat osal.h

void add_timespec(struct timespec *ts, int64_t addtime)
{
   int64_t sec, nsec;

   nsec = addtime % NSEC_PER_SEC;
   sec = (addtime - nsec) / NSEC_PER_SEC;
   ts->tv_sec += sec;
   ts->tv_nsec += nsec;
   if ( ts->tv_nsec > NSEC_PER_SEC )
   {
      nsec = ts->tv_nsec % NSEC_PER_SEC;
      ts->tv_sec += (ts->tv_nsec - nsec) / NSEC_PER_SEC;
      ts->tv_nsec = nsec;
   }
}

void print_timespec(struct timespec *ts){
   printf("time: %" PRIu64 " s, %" PRIu64 " ns\n",ts->tv_sec, ts->tv_nsec);

}