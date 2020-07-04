/*
 *
 * @file flags.h
 * @brief Header containing global variables
 *
 * (c) Filip Reniers (KU Leuven) 2019
 *
 */
#ifndef HEADER_FLAGS_H
#define HEADER_FLAGS_H

#include <stdbool.h>


extern bool command_pause_motor_simulation;
extern bool command_restart_motor_simulation;
extern bool command_openloop;
extern bool command_foc_speed;
extern bool command_foc_position;

extern bool command_increase_vbus;
extern bool command_decrease_vbus;
extern int command_vbus;

extern bool command_increase_loadt;
extern bool command_decrease_loadt;
extern int command_loadt;

extern bool command_increase_loadj;
extern bool command_decrease_loadj;
extern int command_loadj;

extern bool command_increase_speedref;
extern bool command_decrease_speedref;
extern int command_speedref;

extern bool command_increase_posref;
extern bool command_decrease_posref;
extern int command_pos_ref;

extern bool toggle_printing;



/* APPLICATION MEDIATOR COORDINATION STATUS */
extern bool stop_application;
extern bool mediator_configuration_successful;
extern bool mediator_configuration_unsuccessful;

extern bool mediator_configures;
//extern bool mediator_end_of_configuration;
extern bool mediator_monitors;
extern bool mediator_waiting_for_start_threads;
extern bool mediator_waiting_for_shutdown_activities;
extern bool mediator_waiting_for_shutdown_threads;
extern bool mediator_shutdown;

/* THREAD COORDINATION */
extern bool thread0_disabled;
extern bool thread0_configures;
extern bool thread0_configuration_successful;
extern bool thread0_configuration_unsuccessful;
extern bool thread0_running;
extern bool thread0_error_status;
extern bool thread0_schedule1;
extern bool thread0_schedule2;
extern bool thread0_disabling;
extern bool thread0_cleaning_completed;
extern bool start_thread0;
extern bool stop_thread0;

extern bool thread1_disabled;
extern bool thread1_configures;
extern bool thread1_configuration_successful;
extern bool thread1_configuration_unsuccessful;
extern bool thread1_running;
extern bool thread1_error_status;
extern bool thread1_schedule1;
extern bool thread1_schedule2;
extern bool thread1_disabling;
extern bool thread1_cleaning_completed;
extern bool start_thread1;
extern bool stop_thread1;

/* ACTIVITY COORDINATION */
extern bool disabled_command_line_activity;
extern bool enabled_command_line_activity;
extern bool disabling_command_line_activity;
extern bool cleaning_completed_command_line_activity;

extern bool disabled_motor_simulation_control_activity;
extern bool enabled_motor_simulation_control_activity;
extern bool disabling_motor_simulation_control_activity;
extern bool cleaning_completed_motor_simulation_control_activity;

#endif //HEADER_FLAGS_H
