/*
 * @file activity_scheduler1.c
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */

#include "activity_schedulers/activity_scheduler1.h"

#include "flags.h"

void coordinate_activity_scheduler(activity_scheduler_params_t *conf){
    activity_scheduler_params_state_t * scheduler_state = (activity_scheduler_params_state_t *) conf->state;
    scheduler_state->communicate_token_flags(scheduler_state->petrinet);
}

void configure_activity_scheduler_thread0(activity_scheduler_params_t *conf){

    if (thread0_schedule1) {
        if (0 != conf->activity_schedule->current_symbolic_schedule_ID) {
            set_activity_schedule(conf,0);
        }
    } else if (thread0_schedule2) {
        if (1 != conf->activity_schedule->current_symbolic_schedule_ID)
            set_activity_schedule(conf,1);
    }
}

void configure_activity_scheduler_thread1(activity_scheduler_params_t *conf){

    if (thread1_schedule1) {
        if (0 != conf->activity_schedule->current_symbolic_schedule_ID) {
            set_activity_schedule(conf,0);
        }
    } else if (thread1_schedule2) {
        if (1 != conf->activity_schedule->current_symbolic_schedule_ID)
            set_activity_schedule(conf,1);
    }
}

void compute_activity_scheduler(activity_scheduler_params_t *conf) {
    for (int i = 0; i < conf->activity_schedule->number_of_activities; i++){
        trigger(conf->activity_schedule->act_conf[i]);
    }
}