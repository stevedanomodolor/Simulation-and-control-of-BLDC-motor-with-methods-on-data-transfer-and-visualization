/*
 *
 * @file command_line.c
 * @brief Implementation of commandline activity
 *
 * This file contains the implementation of all life-cycle state machine functions of the commandline activity
 *
 * (c) Filip Reniers (KU Leuven) 13.12.19
 * (c) Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a BLDC motor with methods on data transfer and visualitzation
 * Tutor : Herman Bruyninckx
 */

#include "activities/command_line.h"

#include "logger_messages.h"

#include "flags.h"
#include "activities/application_mediator.h"

#include <stdbool.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>

bool kbhit() {
    struct termios term;
    tcgetattr(0, &term);

    struct termios term2 = term;
    term2.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}


void commandline_create(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

    if (enabled_command_line_activity)
        add_event(&activity_config->lcsm,created);
}

void commandline_resource_configure(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

    if (disabling_command_line_activity)
        add_event(&activity_config->lcsm,cleanup_resources);
    else if (enabled_command_line_activity)
        add_event(&activity_config->lcsm,resources_configured);
}

void commandline_capability_configure(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

    if (disabling_command_line_activity)
        add_event(&activity_config->lcsm,configure_resources);
    else if (enabled_command_line_activity)
        add_event(&activity_config->lcsm,capabilities_configured);
}

void commandline_pausing(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif


    if (disabling_command_line_activity)
        add_event(&activity_config->lcsm,configure_capabilities);
    else if (enabled_command_line_activity)
        add_event(&activity_config->lcsm,start);
}

void commandline_running(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

    log_msg_t temp_msg;

    if (enabled_command_line_activity) {
        int n;
        char key;
        /*
        BUTTON_VBUS_PLUS
        BUTTON_VBUS_MINUS
        BUTTON_TORQUE_PLUS
        BUTTON_TORQUE_MINUS
        BUTTON_INERTIA_PLUS
        BUTTON_INERTIA_MINUS
        BUTTON_SPEADREF_PLUS
        BUTTON_SPEADREF_MINUS
        BUTTON_OPEN_CONTROL
        BUTTON_FOC_SPEED
        BUTTON_FOC_POSITION
        BUTTON_PAUSE
        BUTTON_PLAY
        BUTTON_RESTART
        BUTTON_ESCAPE
        */
        if (kbhit()) // if a key was hit ( returns non-zero - true), execute 'if' statment. else continue looping
        {
            n = getchar();
            snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Key pressed: %d", n);
            logger(Info, commandline_conf->logfile, temp_msg);
            printf("%s\n", temp_msg);
            if (n != EOF) {
                key = (unsigned char) n;
                if( key ==BUTTON_PAUSE_p) {
		  if(command_pause_motor_simulation==false) {

                  	command_pause_motor_simulation = true;
			}
		  else {command_pause_motor_simulation= false;}
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "p Key pressed: pause");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_RESTART_r) {
                  command_restart_motor_simulation = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "r pressed: restart");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_OPEN_CONTROL_h) {
                  command_openloop = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "h pressed: openloop mode");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                else if( key ==BUTTON_FOC_SPEED_j) {
                  command_foc_speed = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "j pressed: foc speed mode");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                else if( key ==BUTTON_FOC_POSITION_k) {
                  command_foc_position = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "k pressed: foc position mode");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_VBUS_MINUS_q) {
                  command_decrease_vbus = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "q pressed: vbus decrease");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_VBUS_PLUS_w) {
                  command_increase_vbus = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "w pressed: vbus increase");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_TORQUE_MINUS_a) {
                  command_decrease_loadt = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "a pressed: torque decrease");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_TORQUE_PLUS_s) {
                  command_increase_loadt = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "s pressed: torque increase");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_INERTIA_MINUS_z) {
                  command_decrease_loadj = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "z pressed: inertia decrease");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_INERTIA_PLUS_x) {
                  command_increase_loadj = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "x pressed: inertia increase");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_SPEADREF_MINUS_d) {
                  command_decrease_speedref = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "e pressed: speed decrease");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_SPEADREF_PLUS_f) {
                  command_increase_speedref = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "r pressed: speed increase");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_POSITIONREF_MINUS_c) {
                  command_decrease_posref = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "d pressed: position decrease");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }
                if( key ==BUTTON_POSITIONREF_PLUS_v ) {
                  command_increase_posref = true;
                  snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "f pressed: position increase");
                  logger(Info, commandline_conf->logfile, temp_msg);

                }

                if (key == BUTTON_t) {
                    toggle_printing = !toggle_printing;
                }
                if (key == BUTTON_STOP_e) {
                    stop_application = true;
                    snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "e button pressed: shutdown application");
                    logger(Info, commandline_conf->logfile, temp_msg);
                }



            }
        }
    }
    else if (disabling_command_line_activity) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Shutdown command received");
        logger(Info, commandline_conf->logfile, temp_msg);

        add_event(&activity_config->lcsm,stop);
    }
    fflush(commandline_conf->logfile);

}

void commandline_cleaning(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

    cleaning_completed_command_line_activity = true; //add_tokens(get_place(activity_config->activity_coordination_petrinet,ACTIVITY_CLEANING_COMPLETED),COMMANDLINE_TOKEN,1);
    add_event(&activity_config->lcsm,cleaned);
}

void commandline_done(activity_config_t *activity_config){
    commandline_conf_t *commandline_conf = (commandline_conf_t *) activity_config->state;

#ifdef PRINT_STATE_COMMANDLINE
    if (toggle_printing) {
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,commandline_conf->logfile,temp_mesg);
    }
#endif

}
