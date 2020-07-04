/*
 * @file activity_scheduler.h
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */
#ifndef RT_CONTROL_ACTIVITY_SCHEDULER_H
#define RT_CONTROL_ACTIVITY_SCHEDULER_H

#include "petrinet.h"
#include "activities.h"

typedef
struct activity_schedule_s {
    activity_config_t *act_conf[MAX_NUMBER_OF_ACTIVITIES];
    int number_of_activities;
    int current_symbolic_schedule_ID;
} activity_schedule_t;

typedef struct activity_scheduler_params_s activity_scheduler_params_t;

struct activity_scheduler_params_s {
    void (*coordinate)(activity_scheduler_params_t *conf);
    void (*configure)(activity_scheduler_params_t *conf);
    void (*compute)(activity_scheduler_params_t *conf);

    /* Pointer to array of ALL the activities in the application */
    activity_config_t *activities_array;
    int number_of_activities;

    /* Symbolic schedules from which can be chosen */
    symbolic_schedule_t *symbolic_schedules;
    int number_of_symbolic_schedules;

    /* Actual schedule: array of pointers to activity configuration structures */
    activity_schedule_t *activity_schedule;

    /* State which the scheduler uses*/
    void *state;
};

typedef
struct activity_scheduler_params_state_s {
    petrinet_t *petrinet;
    petrinet_t *(*init_petrinet)(char *name);
    void (*communicate_token_flags)(petrinet_t *p);
} activity_scheduler_params_state_t;

activity_config_t *lookup_activity(activity_scheduler_params_t *scheduler, char *activity_name);
void set_activity_schedule(activity_scheduler_params_t *conf, int ID);

#endif //RT_CONTROL_ACTIVITY_SCHEDULER_H
