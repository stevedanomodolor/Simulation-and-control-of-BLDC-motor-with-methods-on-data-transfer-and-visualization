/*
 * @file thread_lcsm_coordination_petrinet.h
 * @brief header of petrinet for thread lcsm coordination
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */
#ifndef RT_CONTROL_THREAD_LCSM_COORDINATION_PETRINET_H
#define RT_CONTROL_THREAD_LCSM_COORDINATION_PETRINET_H

#include "petrinet.h"

/* TOKEN NAMES */
#define THREAD1_TOKEN "thread1"

/* PLACE NAMES */
#define START_THREAD "start_thread"
#define STOP_THREAD "stop_thread"
#define THREAD_CONFIGURES "thread_configures"
#define THREAD_CONFIGURATION_SUCCESSFUL "thread_conf_successful"
#define THREAD_CONFIGURATION_UNSUCCESSFUL "thread_conf_unsuccessful"
#define THREAD_RUNNING "thread_running"
#define THREAD_ERROR_STATUS "thread_error_status"
#define THREAD_DISABLING "thread_disabling"
#define THREAD_CLEANING_COMPLETE "thread_cleaning_complete"
#define THREAD_DISABLED "thread_disabled"

petrinet_t *init_thread_coordination_petrinet(char *petrinet_name);

void communicate_token_flags_thread0_coordination(petrinet_t *p);
void communicate_token_flags_thread1_coordination(petrinet_t *p);



#endif //RT_CONTROL_THREAD_LCSM_COORDINATION_H
