/*
 * @file activity_scheduler1.h
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */
#ifndef RT_CONTROL_ACTIVITY_SCHEDULER1_H
#define RT_CONTROL_ACTIVITY_SCHEDULER1_H

#include "activity_scheduler.h"

void coordinate_activity_scheduler(activity_scheduler_params_t *conf);
void configure_activity_scheduler_thread0(activity_scheduler_params_t *conf);
void configure_activity_scheduler_thread1(activity_scheduler_params_t *conf);
void compute_activity_scheduler(activity_scheduler_params_t *conf);



#endif //RT_CONTROL_ACTIVITY_SCHEDULER1_H
