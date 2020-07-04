/*
 *
 * @file test_program.c
 * @brief Main program running the first time triggered thread
 *
 * This file contains the main program that starts the first thread with the application mediator activity. The application
 * mediator is in charge of starting all other threads and activities.
 *
 * (c) Filip Reniers (KU Leuven) 11.12.19
 * (c) Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a BLDC motor with methods on data transfer and visualitzation
 * Tutor : Herman Bruyninckx
 */
#include "threads.h"
#include "activities.h"
#include "petrinet.h"

#include "threads/thread0.h"
#include "threads/thread1.h"

#include "activity_schedulers/coordination/thread_activity_scheduler_coordination_petrinet.h"
#include "activity_schedulers/activity_scheduler1.h"

#include "activities/application_mediator.h"
#include "activities/motor_simulation_control.h"
#include "activities/command_line.h"

#include <stdio.h>
#include <stdbool.h>




/* ACTIVITIES */
activity_config_t activities_array[NUMBER_OF_ACTIVITIES] = {
        {
                APPLICATION_MEDIATOR_ACTIVITY,
                {.state=creation, .no_events=0},
                (void *) &application_mediator_configuration_state,
                {&mediator_create,
                        &mediator_resource_configure,
                        &mediator_capability_configure,
                        &mediator_pausing,
                        &mediator_running,
                        &mediator_cleaning,
                        &mediator_done
                }
        },
        {
                COMMANDLINE_ACTIVITY,
                {.state=creation, .no_events=0},
                (void *) &commandline_conf,
                {&commandline_create,
                        &commandline_resource_configure,
                        &commandline_capability_configure,
                        &commandline_pausing,
                        &commandline_running,
                        &commandline_cleaning,
                        &commandline_done
                }
        },
        {
                "motor_simulation_control",
                {.state=creation, .no_events=0},
                (void *) &motor_simulation_control_conf,
                {&motor_simulation_control_create,
                        &motor_simulation_control_resource_configure,
                        &motor_simulation_control_capability_configure,
                        &motor_simulation_control_pausing,
                        &motor_simulation_control_running,
                        &motor_simulation_control_cleaning,
                        &motor_simulation_control_done}
        }
};

/* THREADS */
eventloop_functions_t ev_func_thread0 = {
        .communicate = {communicate_thread0_create, communicate_thread0_resource_conf, communicate_thread0_capability_conf,
                        communicate_thread0_pausing, communicate_thread0_running, communicate_thread0_cleaning,
                        communicate_thread0_done},
        .coordinate = {coordinate_thread0_create, coordinate_thread0_resource_conf, coordinate_thread0_capability_conf,
                       coordinate_thread0_pausing, coordinate_thread0_running, coordinate_thread0_cleaning, coordinate_thread0_done},
        .configure = {configure_thread0_create, configure_thread0_resource_conf, configure_thread0_capability_conf,
                      configure_thread0_pausing, configure_thread0_running, configure_thread0_cleaning, configure_thread0_done},
        .compute = {compute_thread0_create, compute_thread0_resource_conf, compute_thread0_capability_conf, compute_thread0_pausing,
                    compute_thread0_running, compute_thread0_cleaning, compute_thread0_done}
};

eventloop_functions_t ev_func_thread1 = {
        .communicate = {communicate_thread1_create, communicate_thread1_resource_conf, communicate_thread1_capability_conf,
                        communicate_thread1_pausing, communicate_thread1_running, communicate_thread1_cleaning,
                        communicate_thread1_done},
        .coordinate = {coordinate_thread1_create, coordinate_thread1_resource_conf, coordinate_thread1_capability_conf,
                       coordinate_thread1_pausing, coordinate_thread1_running, coordinate_thread1_cleaning, coordinate_thread1_done},
        .configure = {configure_thread1_create, configure_thread1_resource_conf, configure_thread1_capability_conf,
                      configure_thread1_pausing, configure_thread1_running, configure_thread1_cleaning, configure_thread1_done},
        .compute = {compute_thread1_create, compute_thread1_resource_conf, compute_thread1_capability_conf, compute_thread1_pausing,
                    compute_thread1_running, compute_thread1_cleaning, compute_thread1_done}
};

symbolic_schedule_t schedule_array0[2] = {
        {.ID = 0, .number_of_entries = 1, .entries = {APPLICATION_MEDIATOR_ACTIVITY}},
        {.ID = 1, .number_of_entries = 5, .entries = {APPLICATION_MEDIATOR_ACTIVITY, COMMANDLINE_ACTIVITY,
                                                      COMMANDLINE_ACTIVITY, COMMANDLINE_ACTIVITY,
                                                      COMMANDLINE_ACTIVITY}}
};

symbolic_schedule_t schedule_array1[2] = {
        {.ID = 0, .number_of_entries = 0, .entries = {}},
        {.ID = 1, .number_of_entries = 1, .entries = {MOTOR_SIMULATION_CONTROL_ACTIVITY}}
};

activity_schedule_t activity_schedule_thread0 = {
.number_of_activities = 0,
.current_symbolic_schedule_ID = -1
};
activity_schedule_t activity_schedule_thread1 = {
.number_of_activities = 0,
.current_symbolic_schedule_ID = -1
};

activity_scheduler_params_state_t activity_scheduler_params_state0 = {
        .petrinet = NULL,
        .init_petrinet = init_petrinet_activity_scheduler_on_off,
        .communicate_token_flags = communicate_token_flags_activity_scheduler_thread0
};

activity_scheduler_params_state_t activity_scheduler_params_state1 = {
        .petrinet = NULL,
        .init_petrinet = init_petrinet_activity_scheduler_on_off,
        .communicate_token_flags = communicate_token_flags_activity_scheduler_thread1
};

activity_scheduler_params_t activity_scheduler0 = {
        .coordinate = coordinate_activity_scheduler,
        .configure = configure_activity_scheduler_thread0,
        .compute = compute_activity_scheduler,
        .activities_array = activities_array,
        .number_of_activities=NUMBER_OF_ACTIVITIES,
        .symbolic_schedules = schedule_array0,
        .number_of_symbolic_schedules = 2,
        .activity_schedule = &activity_schedule_thread0,
        .state = &activity_scheduler_params_state0
};
activity_scheduler_params_t activity_scheduler1 = {
        .coordinate = coordinate_activity_scheduler,
        .configure = configure_activity_scheduler_thread1,
        .compute = compute_activity_scheduler,
        .activities_array = activities_array,
        .number_of_activities=NUMBER_OF_ACTIVITIES,
        .symbolic_schedules = schedule_array1,
        .number_of_symbolic_schedules = 2,
        .activity_schedule = &activity_schedule_thread1,
        .state = &activity_scheduler_params_state1
};

eventloop_state_t state_thread0 = {.activity_scheduler = &activity_scheduler0, .logfile = NULL, .Error = false};
eventloop_state_t state_thread1 = {.activity_scheduler = &activity_scheduler1, .logfile = NULL, .Error = false};

time_triggered_thread_params_t thread0_config = {
        .functions = &ev_func_thread0,
        .lcsm = {.state = creation, .no_events = 0},
        .flags = {.run_eventloop = true, .cycle_time_enabled = true},
        .timing = {
                .cycletime = 20000000, // ns -> 20ms -> 50 Hz
                .timer_ts = {.tv_sec = 0, .tv_nsec = 0}
        },
        .state = &state_thread0
};

time_triggered_thread_params_t other_thread_configs[NUM_OF_OTHER_THREADS] = {
        {
                .functions = &ev_func_thread1,
                .lcsm = {.state = creation, .no_events = 0},
                .flags = {.run_eventloop = true, .cycle_time_enabled = false},
                .timing = {
                        .cycletime = 100000000, // ns -> 100ms -> 10 Hz
                        .timer_ts = {.tv_sec = 0, .tv_nsec = 0}
                },
                .state = &state_thread1
        }
};

int main(int argc, char **argv) {

    run_time_triggered_thread_eventloop(&thread0_config);

    return 0;

}
