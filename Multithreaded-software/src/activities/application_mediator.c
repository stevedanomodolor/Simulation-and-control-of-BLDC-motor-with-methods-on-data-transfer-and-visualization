/*
 *
 * @file application_mediator.c
 * @brief Implementation of application mediator activity
 *
 * This file contains the implementation of all life-cycle state machine functions of the application mediator activity. The
 * mediator is responsible of configuring all process resources: opening file descriptors, logfiles and stream buffers. It
 * will also startup new threads and deploy the remaining threads. When everything is configured it will signal to all
 * activities that they can start. When a shutdown event is received, it will properly shutdown all activities before clearing
 * thread resources and process resources
 *
 * (c) Filip Reniers (KU Leuven) 11.12.19
 *
 * based on
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#include "activities/application_mediator.h"

#include "logger_messages.h"

#include "flags.h"
#include "activities/coordination/application_mediator_petrinet.h"
#include "threads/coordination/thread_lcsm_coordination_petrinet.h"

#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h> // FILE CONROL
#include <pthread.h>

void mediator_create(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif

    if (mediator_configures) {

        petrinet_t *activity_coordination_petrinet = init_application_mediator_coordination_petrinet(
                "Application Mediator Petrinet");

        application_mediator_configuration_state.number_of_threads = NUM_OF_OTHER_THREADS;
        application_mediator_configuration_state.other_threads = other_thread_configs;
        application_mediator_configuration_state.own_thread = &thread0_config;
        application_mediator_configuration_state.activity_coordination_petrinet = activity_coordination_petrinet;
        application_mediator_configuration_state.flags.part_of_configuration = 0;
        application_mediator_configuration_state.errors.FD_ERROR = 0x000;
        application_mediator_configuration_state.errors.THREAD_ERROR = 0x00;
        mediator_conf->flags.threads_to_start = 0x00;
        mediator_conf->flags.threads_started = 0x00;
        mediator_conf->flags.threads_started |= MAIN_THREAD_LOCAL_ID;

        add_event(&activity_config->lcsm, created);

    }


}

/* (Re)initialize maximum freshness stream */
void mediator_resource_configure(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif

    communicate_token_flags_application_mediator(mediator_conf->activity_coordination_petrinet);

    if (mediator_shutdown)
        add_event(&activity_config->lcsm, cleanup_resources);
    else if (mediator_configures) {


        mediator_conf->flags.threads_to_start |= MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID;

        add_event(&activity_config->lcsm, resources_configured);
    }

}

void mediator_capability_configure(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif

    communicate_token_flags_application_mediator(mediator_conf->activity_coordination_petrinet);

    if (mediator_shutdown)
        add_event(&activity_config->lcsm, configure_resources);
    else if (mediator_configures)
        add_event(&activity_config->lcsm, capabilities_configured);
}

void mediator_pausing(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif

    communicate_token_flags_application_mediator(mediator_conf->activity_coordination_petrinet);

    // if (mediator_monitoring)
    if (mediator_configures) {
        add_event(&activity_config->lcsm, start);
    } else if (mediator_shutdown && (mediator_conf->flags.threads_started & OTHER_THREADS)) {
        add_event(&activity_config->lcsm, start);
    } else if (mediator_shutdown && !(mediator_conf->flags.threads_started & OTHER_THREADS)) {
        add_event(&activity_config->lcsm, configure_capabilities);
    } else if (mediator_waiting_for_shutdown_threads){
        fprint_marking(mediator_conf->activity_coordination_petrinet,mediator_conf->logfile);
        fflush(mediator_conf->logfile);
    }
}

void mediator_running(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing){
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif
    commandline_conf_t *commandline_conf = lookup_activity_in_array(activities_array, NUMBER_OF_ACTIVITIES, COMMANDLINE_ACTIVITY)->state;
    motor_simulation_control_conf_t *motor_simulation_control = lookup_activity_in_array(activities_array, NUMBER_OF_ACTIVITIES, MOTOR_SIMULATION_CONTROL_ACTIVITY)->state;

    communicate_token_flags_application_mediator(mediator_conf->activity_coordination_petrinet);

    if (mediator_configures) {

        if (0 == mediator_conf->flags.part_of_configuration) {
            /*
             * PART 1 CONFIGURATION:
             * OPENING FILE DESCRIPTORS & BUFFERS
             */

            if (mediator_conf->logfile) {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN,
                         "***RECONFIGURATION application mediator***"); //todo string defines / Lookup tables
                logger(Info, mediator_conf->logfile, temp_msg);
            } else {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "***CONFIGURATION application mediator***");
                fprintf(stdout, "%s\n", temp_msg);
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Opening Loggers");
                fprintf(stdout, "%s\n", temp_msg);
            }

            char log_file_path[MAX_LENGTH_PATH_NAME];

            /* OPEN file descriptors of LOGGERS */
            // Open logfile -> file descriptor
            snprintf(log_file_path, MAX_LENGTH_PATH_NAME, "%s/%s", PATH_LOGFILES, MEDIATOR_LOG_FILE_NAME);
            FILE *logfile_mediator = fopen(log_file_path, "w");
            if (logfile_mediator == NULL) {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Could not open %s", log_file_path);
                fprintf(stderr, "%s\n", temp_msg);
                mediator_conf->errors.FD_ERROR |= ERROR_LOGFILE_MEDIATOR;
            } else {
                mediator_conf->logfile = logfile_mediator;
                logger(Info, logfile_mediator, "Logging started");
            }

            // Open logfile -> file descriptor
            snprintf(log_file_path, MAX_LENGTH_PATH_NAME, "%s/%s", PATH_LOGFILES, MOTOR_SIMULATION_CONTROL_LOG_FILE_NAME);
            FILE *logfile_motor_simulation_control = fopen(log_file_path, "w");
            if (logfile_motor_simulation_control == NULL) {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Could not open %s", log_file_path);
                logger(Error, logfile_mediator, temp_msg);
                mediator_conf->errors.FD_ERROR |= ERROR_LOGFILE_MOTOR_SIMULATION_CONTROL;
            } else {
                motor_simulation_control->logfile = logfile_motor_simulation_control;
                logger(Info, logfile_motor_simulation_control, "Logging started");
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Succesfully opened logfile: %s", log_file_path);
                logger(Info, logfile_mediator, temp_msg);
            }

            // Open logfile -> file descriptor
            snprintf(log_file_path, MAX_LENGTH_PATH_NAME, "%s/%s", PATH_LOGFILES, COMMANDLINE_LOG_FILE_NAME);
            FILE *logfile_commandline = fopen(log_file_path, "w");
            if (logfile_commandline == NULL) {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Could not open %s", log_file_path);
                logger(Error, logfile_mediator, temp_msg);
                mediator_conf->errors.FD_ERROR |= ERROR_LOGFILE_COMMANDLINE;
            } else {
                commandline_conf->logfile = logfile_commandline;
                logger(Info, logfile_commandline, "Logging started");
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Succesfully opened logfile: %s", log_file_path);
                logger(Info, logfile_mediator, temp_msg);
            }

            // Open logfile -> file descriptor
            snprintf(log_file_path, MAX_LENGTH_PATH_NAME, "%s/%s", PATH_LOGFILES, THREAD1_LOG_FILE_NAME);
            FILE *logfile_thread1 = fopen(log_file_path, "w");
            if (logfile_thread1 == NULL) {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Could not open %s", log_file_path);
                logger(Error, logfile_mediator, temp_msg);
                mediator_conf->errors.FD_ERROR |= ERROR_LOGFILE_THREAD1;
            } else {
                ((eventloop_state_t *) other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].state)->logfile = logfile_thread1;
                logger(Info, logfile_thread1, "Logging started");
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Succesfully opened logfile: %s", log_file_path);
                logger(Info, logfile_mediator, temp_msg);
            }


            /* INITIALISING THREAD COORDINATION Petrinets */
            petrinet_t *p;
            p = init_thread_coordination_petrinet("motor simulation control thread coordination");
            ((eventloop_state_t *) other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].state)->petrinet_thread_coordination = p;

            /* CHECK WHETHER CONFIGURATION IS SUCCESSFUL */
            if (mediator_conf->errors.FD_ERROR) {
                mediator_configuration_unsuccessful = true;
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "First part application configuration has errors X%X",
                         mediator_conf->errors.FD_ERROR);
                logger(Error, logfile_mediator, temp_msg);
            } else {
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "First part application configuration successful X%X",
                         mediator_conf->errors.FD_ERROR);
                logger(Info, logfile_mediator, temp_msg);
            }
            mediator_conf->flags.part_of_configuration++;

        } else if (1 == mediator_conf->flags.part_of_configuration) {
            /*
             * PART 2 CONFIGURATION:
             * STARTING THREADS
             */
            if (mediator_conf->flags.threads_to_start & MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID) {
                other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].pthread_id = 0;
                int ret = pthread_create(&other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].pthread_id, NULL,
                                         run_time_triggered_thread_eventloop,
                                         &other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD]);
                if (!ret) {
                    mediator_conf->flags.threads_started |= MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID;
                    snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "motor simulation control thread succesfully created with tid: %ld",
                             other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].pthread_id);
                    logger(Info, motor_simulation_control->logfile, temp_msg);
                    logger(Info, mediator_conf->logfile, temp_msg);
                } else {
                    mediator_conf->errors.THREAD_ERROR |= ERROR_STARTING_MOTOR_SIMULATION_CONTROL_THREAD;
                    mediator_configuration_unsuccessful = true;
                    snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Motor simulation control thread could not be created");
                    logger(Info, motor_simulation_control->logfile, temp_msg);
                    logger(Info, mediator_conf->logfile, temp_msg);
                }
            }

            mediator_conf->flags.part_of_configuration++;

            if (mediator_conf->errors.FD_ERROR || mediator_conf->errors.THREAD_ERROR) {
                mediator_configuration_unsuccessful = true;
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Second part application configuration has errors X%X",
                         mediator_conf->errors.THREAD_ERROR);
                logger(Error, mediator_conf->logfile, temp_msg);
            } else {
                mediator_configuration_successful = true;
                snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Second part application configuration successful X%X",
                         mediator_conf->errors.THREAD_ERROR);
                logger(Info, mediator_conf->logfile, temp_msg);
            }
            fflush(mediator_conf->logfile); //todo



            add_event(&activity_config->lcsm, stop);
        }
    }

    if (mediator_shutdown) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Shutdown all threads");
        logger(Info, mediator_conf->logfile, temp_msg);

        mediator_conf->other_threads[MOTOR_SIMULATION_CONTROL_THREAD].flags.run_eventloop = false;

        if (mediator_conf->flags.threads_started & MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID) {
            pthread_join(mediator_conf->other_threads[MOTOR_SIMULATION_CONTROL_THREAD].pthread_id, NULL);
            mediator_conf->flags.threads_started &= ~(MOTOR_SIMULATION_CONTROL_THREAD_LOCAL_ID);
            motor_simulation_control_conf_t *motor_simulation_control = (motor_simulation_control_conf_t *) lookup_activity_in_array(activities_array, NUMBER_OF_ACTIVITIES, MOTOR_SIMULATION_CONTROL_ACTIVITY)->state;

            log_msg_t temp_msg;
            snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Joined motor simulation control thread with main thread");
            logger(Info,motor_simulation_control->logfile, temp_msg);
            logger(Info, mediator_conf->logfile, temp_msg);
        }

        if (!(mediator_conf->flags.threads_started & OTHER_THREADS))
            add_event(&activity_config->lcsm, stop);
    }

    if (mediator_monitors) {
        add_event(&activity_config->lcsm, stop);
    }
}

void mediator_cleaning(activity_config_t *activity_config) {
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif

//    print_marking(mediator_conf->activity_coordination_petrinet); // todo

    communicate_token_flags_application_mediator(mediator_conf->activity_coordination_petrinet);

    activity_config_t *commandline_act = lookup_activity_in_array(activities_array, NUMBER_OF_ACTIVITIES, COMMANDLINE_ACTIVITY);
    activity_config_t *motor_simulation_control_act = lookup_activity_in_array(activities_array, NUMBER_OF_ACTIVITIES, MOTOR_SIMULATION_CONTROL_ACTIVITY);

    commandline_conf_t *commandline_conf = (commandline_conf_t *) commandline_act->state;
    motor_simulation_control_conf_t *motor_simulation_control = (motor_simulation_control_conf_t *) motor_simulation_control_act->state;

    eventloop_state_t *motor_simulation_control_thread_state = (eventloop_state_t *) other_thread_configs[MOTOR_SIMULATION_CONTROL_THREAD].state;

    FILE *logfile_thread1 = motor_simulation_control_thread_state->logfile;

    char *commandline_name = commandline_act->name;
    char *motor_simulation_control_name = motor_simulation_control_act->name;


    /* DESTROYING THREAD COORDINATION Petrinets */
    destroy_petrinet(motor_simulation_control_thread_state->petrinet_thread_coordination);
    motor_simulation_control_thread_state->petrinet_thread_coordination = NULL;

    /* DESTROYING LOGFILES */
    if (motor_simulation_control->logfile) {
        logger(Info, motor_simulation_control->logfile, "Shutting down logger");
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Shutting down logger of activity %s", motor_simulation_control_name);
        logger(Info, mediator_conf->logfile, temp_msg);
        fflush(motor_simulation_control->logfile);
        fclose(motor_simulation_control->logfile);
        motor_simulation_control->logfile = NULL;
    }
    if (commandline_conf->logfile) {
        logger(Info, commandline_conf->logfile, "Shutting down logger");
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Shutting down logger of activity %s", commandline_name);
        logger(Info, mediator_conf->logfile, temp_msg);
        fflush(commandline_conf->logfile);
        fclose(commandline_conf->logfile);
        commandline_conf->logfile = NULL;
    }
    if (logfile_thread1) {
        logger(Info, logfile_thread1, "Shutting down logger");
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Shutting down logger of thread %d", MOTOR_SIMULATION_CONTROL_THREAD);
        logger(Info, mediator_conf->logfile, temp_msg);
        fflush(logfile_thread1);
        fclose(logfile_thread1);
        logfile_thread1 = NULL;
    }

    if (mediator_conf->logfile) {
        logger(Info, mediator_conf->logfile, "Shutting down logger");
        fflush(mediator_conf->logfile);
        fclose(mediator_conf->logfile);
        mediator_conf->logfile = NULL;
    }


    add_event(&activity_config->lcsm, cleaned);
    stop_thread0 = true;
}

void mediator_done(activity_config_t *activity_config){
    application_mediator_conf_t *mediator_conf = (application_mediator_conf_t *) activity_config->state;

    log_msg_t temp_msg;
#ifdef PRINT_STATE_MEDIATOR
    if (toggle_printing) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,mediator_conf->logfile,temp_msg);
    }
#endif
}
