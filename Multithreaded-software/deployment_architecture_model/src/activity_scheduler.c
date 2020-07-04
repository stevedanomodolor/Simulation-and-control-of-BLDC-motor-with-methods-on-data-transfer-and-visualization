/*
 * @file activity_scheduler.c
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */

#include "activity_scheduler.h"

activity_config_t *lookup_activity(activity_scheduler_params_t *scheduler, char *activity_name) {
    return lookup_activity_in_array(scheduler->activities_array, scheduler->number_of_activities, activity_name);
}

void set_activity_schedule(activity_scheduler_params_t *conf, int ID) {
    conf->activity_schedule->current_symbolic_schedule_ID = ID;
    conf->activity_schedule->number_of_activities = conf->symbolic_schedules[ID].number_of_entries;
    for (int i =0; i < conf->symbolic_schedules[ID].number_of_entries; i++){
        conf->activity_schedule->act_conf[i] = lookup_activity(conf,conf->symbolic_schedules[ID].entries[i]);
    }
}
