/*
 * @file threads.c
 * @brief  Implementation of thread template with time-triggered eventloop
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 20.02.20
 *
 */
#include "threads.h"

#include "time_helper.h"


void communicate(void *conf) {
    eventloop_t *eventloop = (eventloop_t *) conf;
    (*eventloop->functions->communicate[eventloop->lcsm.state])(eventloop);
}

void coordinate(void *conf) {
    eventloop_t *eventloop = (eventloop_t *) conf;
    (*eventloop->functions->coordinate[eventloop->lcsm.state])(eventloop);
}

void configure(void *conf) {
    eventloop_t *eventloop = (eventloop_t *) conf;
    (*eventloop->functions->configure[eventloop->lcsm.state])(eventloop);
}

void compute(void *conf) {
    eventloop_t *eventloop = (eventloop_t *) conf;
    (*eventloop->functions->compute[eventloop->lcsm.state])(eventloop);
}

void *run_time_triggered_thread_eventloop(void *d) {
    time_triggered_thread_params_t *eventloop = (time_triggered_thread_params_t *) d;

    if (clock_gettime(CLOCK_MONOTONIC, &eventloop->timing.timer_ts) == -1)
        return NULL;

    while (eventloop->flags.run_eventloop) {

        if (eventloop->flags.cycle_time_enabled) {
            add_timespec(&eventloop->timing.timer_ts, eventloop->timing.cycletime);
            /* wait to cycle start */
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &eventloop->timing.timer_ts,
                            NULL); // &event_loop->tleft
        }
        update_lcsm(&eventloop->lcsm); // todo move -> when forgotten to call in coordinate: stuck in loop

        communicate(eventloop);
        coordinate(eventloop); // beslist welke activiteiten er mogen lopen, of dat er cycle time is
        configure(eventloop); // maakt de lijst van activiteiten (echte pointers)
        compute(eventloop); // do_eventloop(activity_resource_mediator)
        // do resource mediator && do commandline  OR  do resource mediator
    }


}




