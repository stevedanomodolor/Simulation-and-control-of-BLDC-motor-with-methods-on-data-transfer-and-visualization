/*
 * @file thread_activity_scheduler_coordination_petrinet.h
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */
#ifndef RT_CONTROL_THREAD_ACTIVITY_SCHEDULER_COORDINATION_PETRINET_H
#define RT_CONTROL_THREAD_ACTIVITY_SCHEDULER_COORDINATION_PETRINET_H

#include "petrinet.h"

/* PLACE NAMES */
#define THREAD_SELECT_SCHEDULE1 "thread_select_schedule1"
#define THREAD_SELECT_SCHEDULE2 "thread_select_schedule2"
#define THREAD_SCHEDULE1 "thread_schedule1"
#define THREAD_SCHEDULE2 "thread_schedule2"

petrinet_t *init_petrinet_activity_scheduler_on_off(char *petrinet_name);

void communicate_token_flags_activity_scheduler_thread0(petrinet_t *p);
void communicate_token_flags_activity_scheduler_thread1(petrinet_t *p);


#endif //RT_CONTROL_THREAD_ACTIVITY_SCHEDULER_COORDINATION_PETRINET_H
