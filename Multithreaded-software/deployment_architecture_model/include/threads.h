/*
 * @file threads.h
 * @brief header of thread template with time-triggered eventloop
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 20.02.20
 *
 */

#ifndef RT_CONTROL_THREADS_H
#define RT_CONTROL_THREADS_H

#include "lcsm_new.h"
#include "petrinet.h"
#include "activity_scheduler.h"
#include "symbolic_schedule.h"

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#define NO_CYCLE_TIME -1


typedef struct time_triggered_thread_params_s time_triggered_thread_params_t;

typedef
struct timing_s {
    int64_t cycletime; // nanoseconds
    struct timespec timer_ts;
} timing_t;

typedef
struct eventloop_state_s {
    FILE *logfile;
    bool Error;
    petrinet_t *petrinet_thread_coordination;
    activity_scheduler_params_t *activity_scheduler;
} eventloop_state_t;

typedef
struct eventloop_functions_s {
    void (*communicate[NUM_NEW_LCSM_STATES])(void *conf);

    void (*coordinate[NUM_NEW_LCSM_STATES])(void *conf);

    void (*configure[NUM_NEW_LCSM_STATES])(void *conf);

    void (*compute[NUM_NEW_LCSM_STATES])(void *conf);
} eventloop_functions_t;

typedef
struct eventloop_flags_s {
    bool run_eventloop;
    bool cycle_time_enabled;
} eventloop_flags_t;

struct time_triggered_thread_params_s {
    eventloop_functions_t *functions;
    LCSM_t lcsm;

    pthread_t pthread_id;

    eventloop_flags_t flags;

    timing_t timing;

    void *state;
};

typedef
struct eventloop_s {
    eventloop_functions_t *functions;
    LCSM_t lcsm;
} eventloop_t;

void *run_time_triggered_thread_eventloop(void *d);

#endif //RT_CONTROL_THREADS_H
