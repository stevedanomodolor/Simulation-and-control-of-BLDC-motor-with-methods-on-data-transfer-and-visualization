/*
 *
 * @file activities.c
 * @brief Implementation of timed eventloop
 *
 * This file contains the implementation of timed eventloop which will sequentially trigger all activities according to
 * the prescribed relative frequency. When a activity is triggered it will execute the behaviour of the corresponding
 * life-cycle state machine state in which the activity currently is.
 *
 * (c) Filip Reniers (KU Leuven) 19.12.19
 *
 */


#include "activities.h"

#include <stdio.h>
#include <string.h>

// TODO reimplement with uthash + move to proper source file
activity_config_t *lookup_activity_in_array(activity_config_t* activities_array, int number_of_activities, char *activity_name) {
    for (int i = 0; i < number_of_activities; i++) {
        if (!strcmp(activity_name, activities_array[i].name)) {
            return &activities_array[i];
        }
    }
    printf("LOOKUP FAILED: activity does not exist: %s\n", activity_name);
    return NULL;
}

void trigger(activity_config_t *act) {
    // call the corresponding trigger function for the current LCSM state
    update_lcsm(&act->lcsm); // coordinate_lcsm(&act->lcsm) // todo rename: run, do, execute act->run_lcsm(&act->lcsm)
//    if (act->lcsm.state != done)
    (*act->activityFunc[act->lcsm.state])(act);
}


/*
 *
 * OS, threads, cores, memory -> eventloops onmiddelijke context is threads. Hier zijn de file descriptors
 *
 */