/*
 *
 * @file activities.h
 * @brief Header that defines eventloop and activity configuration structures
 *
 * This file is the header that defines the eventloop and activity configuration structures.
 *
 * (c) Filip Reniers (KU Leuven) 11.12.19
 *
 */

#ifndef RT_CONTROL_ACTIVITIES_H
#define RT_CONTROL_ACTIVITIES_H

#include "lcsm_new.h"
#include "symbolic_schedule.h"


#define MAX_NUMBER_OF_ACTIVITIES MAX_NUMBER_OF_ENTRIES
#define MAX_LENGTH_ACTIVITY_NAME 50

typedef struct activity_config_s activity_config_t;

// struct activity_config_t: the configuration for one activity (LCSM, activity trigger functions)
struct activity_config_s {
    char name[MAX_LENGTH_ACTIVITY_NAME];
    struct LCSM lcsm;
    void *state;
    void (*activityFunc[NUM_NEW_LCSM_STATES])(
            struct activity_config_s *); // array of function pointers: an activity trigger function per LCSM state
};

activity_config_t *lookup_activity_in_array(activity_config_t *scheduler, int number_of_activities, char *activity_name);

void trigger(activity_config_t *act);

#endif //RT_CONTROL_ACTIVITIES_H

