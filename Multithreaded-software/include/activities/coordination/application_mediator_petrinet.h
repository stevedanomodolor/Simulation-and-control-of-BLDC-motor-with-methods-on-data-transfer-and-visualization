/*
 * @file application_mediator_petrinet.h
 * @brief Header of petrinet for the coordination of the whole application
 *
 *
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */
#ifndef RT_CONTROL_APPLICATION_MEDIATOR_PETRINET_H
#define RT_CONTROL_APPLICATION_MEDIATOR_PETRINET_H

#include "petrinet.h"

/* TOKEN NAMES */
#define MOTOR_SIMULATION_CONTROL_TOKEN "motor_simulation_control"
#define COMMANDLINE_TOKEN "commandline"

/* PLACE NAMES */
#define MEDIATOR_CONFIGURES "mediator_configures"
#define MEDIATOR_CONFIGURATION_SUCCESSFUL "mediator_configuration_successful"
#define MEDIATOR_CONFIGURATION_UNSUCCESSFUL "mediator_configuration_unsuccessful"
#define MEDIATOR_MONITORING "mediator_monitoring"
#define MEDIATOR_WAITING_FOR_SHUTDOWN_ACTIVITIES "mediator_waiting_for_shutdown_activities"
#define MEDIATOR_SHUTDOWN "mediator_shutdown"
#define STOP_APPLICATION "stop_application"
#define ACTIVITY_DISABLED "activity_disabled"
#define ACTIVITY_ENABLED "activity_enabled"
#define START_ACTIVITY "start_activity"
#define STOP_ACTIVITY  "stop_activity"
#define ACTIVITY_DISABLING "activity_disabling"
#define ACTIVITY_CLEANING_COMPLETED "activity_cleaning_completed"
#define MEDIATOR_START_THREADS "mediator_start_threads"
#define MEDIATOR_STOP_THREADS "mediator_stop_threads"
#define MEDIATOR_THREADS_RUNNING "mediator_threads_running"
#define MEDIATOR_THREADS_ERROR_STATUS "mediator_threads_error_status"
#define MEDIATOR_WAITING_FOR_START_THREADS "mediator_waiting_for_start_threads"
#define MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS "mediator_waiting_for_shutdown_threads"
#define MEDIATOR_THREADS_DISABLED "mediator_threads_disabled"

petrinet_t *init_application_mediator_coordination_petrinet(char *petrinet_name);
void communicate_token_flags_application_mediator(petrinet_t *p);

#endif //RT_CONTROL_APPLICATION_MEDIATOR_PETRINET_H
