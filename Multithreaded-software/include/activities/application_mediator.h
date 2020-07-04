/*
 *
 * @file application_mediator.h
 * @brief Header of application mediator activity
 *
 * This file is the header of the application mediator activity. It contains all activity specific configuration structures and
 * also process and thread resource configuration.
 *
 * (c) Filip Reniers (KU Leuven) 11.12.19
 *
 */

#ifndef RT_CONTROL_APPLICATION_MEDIATOR_H
#define RT_CONTROL_APPLICATION_MEDIATOR_H
#define PRINT_STATE_MEDIATOR

#include "threads.h"
#include "activities.h"

#include "activities/command_line.h"
#include "activities/motor_simulation_control.h"

#include "algorithms/motor_simulation_control_functions.h"

#include <stdio.h>

void mediator_resource_configure(activity_config_t *activity_config);

void mediator_capability_configure(activity_config_t *activity_config);

void mediator_running(activity_config_t *activity_config);

void mediator_pausing(activity_config_t *activity_config);

void mediator_cleaning(activity_config_t *activity_config);

void mediator_create(activity_config_t *activity_config);

void mediator_done(activity_config_t *activity_config);

/* ACTIVITIES */

#define NUMBER_OF_ACTIVITIES 3
#define NUMBER_OF_OTHER_ACTIVITIES 2
#define APPLICATION_MEDIATOR_ACTIVITY "application_mediator"
#define MOTOR_SIMULATION_CONTROL_ACTIVITY "motor_simulation_control"
#define COMMANDLINE_ACTIVITY "commandline"

/* STATIC MEMORY DECLARATION */
/* Array of activity configuration structures */
extern activity_config_t activities_array[NUMBER_OF_ACTIVITIES];

/* ACTIVITY STATE STRUCTURES */
typedef struct motor_simulation_control_conf_s{
    FILE *logfile;
    conf_motor_simulation_control *motor;
} motor_simulation_control_conf_t;

typedef struct commandline_conf_s {
    FILE *logfile;
} commandline_conf_t;

typedef
struct local_mediator_flags_s {
    int part_of_configuration;
    char threads_to_start;
    char threads_started;
} local_mediator_flags_t;

typedef
struct local_mediator_errors_s {
    short FD_ERROR; // 2 bytes: 5 bits used -> can be converted to char
    char THREAD_ERROR; // 1 byte: 2 bits used
} local_mediator_errors_t;

typedef struct application_mediator_conf_s {
    FILE *logfile;

    time_triggered_thread_params_t *own_thread;

    int number_of_threads;
    time_triggered_thread_params_t *other_threads;

    petrinet_t *activity_coordination_petrinet;
    local_mediator_flags_t flags;
    local_mediator_errors_t errors;
} application_mediator_conf_t;

/* STATIC MEMORY DECLARATION */
/* Activity state structures */
motor_simulation_control_conf_t motor_simulation_control_conf;
commandline_conf_t commandline_conf;
application_mediator_conf_t application_mediator_configuration_state;

/* ACTIVITY CONFIGURATION */
/* LOGGING */
#define MAX_LENGTH_PATH_NAME 32
#define PATH_LOGFILES "ramsub"

#define MOTOR_SIMULATION_CONTROL_LOG_FILE_NAME "motor_control.log"
#define COMMANDLINE_LOG_FILE_NAME "commandline.log"
#define MEDIATOR_LOG_FILE_NAME "mediator.log"

#define THREAD0_LOG_FILE_NAME "thread0.log"
#define THREAD1_LOG_FILE_NAME "thread1.log"

/* LOCAL ERROR FLAGS */
/* ERROR FILE DESCRIPTORS */
#define ERROR_LOGFILE_MEDIATOR 0x001
#define ERROR_LOGFILE_MOTOR_SIMULATION_CONTROL 0x002
#define ERROR_LOGFILE_COMMANDLINE 0x010

#define ERROR_LOGFILE_THREAD1 0x040
#define ERROR_LOGFILE_THREAD2 0x080

/* ERROR THREADS */
#define ERROR_STARTING_MOTOR_SIMULATION_CONTROL_THREAD 0x01

/* THREADS */

/* THREAD IDENTIFIER */
#define MAIN_THREAD_LOCAL_ID 0x01
#define MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID 0x02

#define OTHER_THREADS 0x1E

#define NUM_OF_OTHER_THREADS 1

#define MOTOR_SIMULATION_CONTROL_THREAD 0

/* STATIC MEMORY DECLARATION */
/* Thread configuration structures */
extern time_triggered_thread_params_t thread0_config;
extern time_triggered_thread_params_t other_thread_configs[NUM_OF_OTHER_THREADS];
/* Thread lcsm functions */
extern eventloop_functions_t ev_func_thread0;
extern eventloop_functions_t ev_func_thread1;

#endif //RT_CONTROL_APPLICATION_MEDIATOR_H
