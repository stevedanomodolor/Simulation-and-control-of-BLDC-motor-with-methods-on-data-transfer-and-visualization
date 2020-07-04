/*
 *
 * @file tile_control.c
 * @brief
 *
 * This file is the implementation of the motor simulation control activity. This activity will change the state of the motor dependeing on the input value it receives
 *
 * (c) Filip Reniers (KU Leuven) 2020
 * (c) Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a BLDC motor with methods on data transfer and visualitzation
 * Tutor : Herman Bruyninckx
 *
 */
#include "activities/motor_simulation_control.h"

#include "logger_messages.h"

#include "activities/application_mediator.h"

#include "algorithms/motor_simulation_control_functions.h"

#include "flags.h"

#include <stdio.h>
#include <stdbool.h>

//#define PRINT_STATE_TILE_CONTROL

bool command_pause_motor_simulation = false;
bool command_restart_motor_simulation = false;
bool command_openloop = false;
bool command_foc_speed = false;
bool command_foc_position = false;

bool command_increase_vbus = false;
bool command_decrease_vbus = false;
int command_vbus = false;

bool command_increase_loadt = false;
bool command_decrease_loadt = false;
int command_loadt = false;

bool command_increase_loadj = false;
bool command_decrease_loadj = false;
int command_loadj = false;

bool command_increase_speedref = false;
bool command_decrease_speedref = false;
int command_speedref = false;

bool command_increase_posref = false;
bool command_decrease_posref = false;
int command_pos_ref = false;

bool toggle_printing = true;


void print_motor_result(conf_motor_simulation_control *motor){
    printf( "t = %.2f\t Va = %.2f\t Vb = %.2f\t Vc = %.2f\t rpm = %.2f\t pos = %.2f\t torque = %.2f\t iq = %.2f id = %.2f data_ps = %d\n", motor->motor_state->time, motor->motor_state->va,motor->motor_state->vb,motor->motor_state->vc,motor->motor_state->rpm,motor->motor_state->pos,motor->motor_state->torque,motor->motor_state->iq, motor->motor_state->id, motor->motor_state->calculated_stream_rate);

}

void motor_simulation_control_create(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *state = (motor_simulation_control_conf_t *) activity_config->state;

    if (enabled_motor_simulation_control_activity) {
#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
        log_msg_t temp_mesg;
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
        logger(Info,state->logfile,temp_mesg);
#endif

// MODIFY FOR MULTIPLE MOTOR->

// CREATING MEMEORY FOR THE DATABASE

        state->motor = (conf_motor_simulation_control *) malloc(sizeof(conf_motor_simulation_control));
        state->motor->motor_state = (motor_simulation_r *) malloc(sizeof(motor_simulation_r));
        state->motor->simulation_parametres = (motor_simulation_parametres *) malloc(sizeof(motor_simulation_parametres));
        state->motor->input_parametres = (motor_input_parametre *) malloc(
                sizeof(motor_input_parametre));


       for(int i = 0; i < NUM_INPUT_SLIDER_PARA; i++) {
          state->motor->input_parametres->slider_input[i] = (slider *) malloc(sizeof(slider));
        }
            //TODO - confirm if correct

        add_event(&activity_config->lcsm,created);
    }
}

void motor_simulation_control_resource_configure(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *state = (motor_simulation_control_conf_t *) activity_config->state;

#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    log_msg_t temp_mesg;
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,state->logfile,temp_mesg);
#endif



    if (disabling_motor_simulation_control_activity)
        add_event(&activity_config->lcsm,cleanup_resources);
    else if (enabled_motor_simulation_control_activity) {



      motor_simulation_r *motor_state = state->motor->motor_state;
      motor_simulation_parametres *simulation_parametres = state->motor->simulation_parametres;
      motor_input_parametre* input_parametres = state->motor->input_parametres;

      // for(int i = 0; i < NUM_INPUT_SLIDER_PARA; i++) {
      //    slider *slider_input[i] = state->motor->input_parametres->slider_input[i];
      //  }

      motor_state->time = 0; // time of simulation in seconds
      motor_state->va = 0;
      motor_state->vc = 0;
      motor_state->vb = 0;
      motor_state->rpm = 0;
      motor_state->pos = 0; // curent position
      motor_state->torque = 0;
      motor_state->iq = 0;
      motor_state->id = 0;
      motor_state->calculated_stream_rate = 0;

      simulation_parametres->last_loop = 0;
      simulation_parametres->t = 0;
      simulation_parametres->dt = 0.0001;
      simulation_parametres->angle = 0;
      simulation_parametres->angleDeg = 0;
      simulation_parametres->vel_t = 0;
      simulation_parametres->velocity = 0;
      simulation_parametres->position = 0;
      simulation_parametres->ia = 0;
      simulation_parametres->torque = 0;
      simulation_parametres->vel_cmd = 0;
      simulation_parametres->set_pos = 0;
      simulation_parametres->calculated_stream_rate = 0;
      simulation_parametres->new_loop = 0;
      simulation_parametres->vel_t = 0;
      simulation_parametres->ib = 0;

      input_parametres->mode = open_mode;
      input_parametres->slider_input[vbus]->current_value = 24 ;
      input_parametres->slider_input[vbus]->min_boundary = 0;
      input_parametres->slider_input[vbus]->max_boundary = 36;

      input_parametres->slider_input[loadt]->current_value = 0;
      input_parametres->slider_input[loadt]->min_boundary = 0;
      input_parametres->slider_input[loadt]->max_boundary = 10;

      input_parametres->slider_input[loadj]->current_value = 0;
      input_parametres->slider_input[loadj]->min_boundary = 0;
      input_parametres->slider_input[loadj]->max_boundary = 10;

      input_parametres->slider_input[rpm_ref]->current_value = 5;
      input_parametres->slider_input[rpm_ref]->min_boundary = 0;
      input_parametres->slider_input[rpm_ref]->max_boundary = 100;

      input_parametres->slider_input[pos_ref]->current_value = 0;
      input_parametres->slider_input[pos_ref]->min_boundary = 0;
      input_parametres->slider_input[pos_ref]->max_boundary = 360;





      add_event(&activity_config->lcsm,resources_configured);
    }
}

void motor_simulation_control_capability_configure(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *motor_simulation_control_config = (motor_simulation_control_conf_t *) activity_config->state;

#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    log_msg_t temp_mesg;
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,motor_simulation_control_config->logfile,temp_mesg);
#endif

    if (disabling_motor_simulation_control_activity)
        add_event(&activity_config->lcsm,configure_resources);
    else if (enabled_motor_simulation_control_activity)
        add_event(&activity_config->lcsm,capabilities_configured);
}

void motor_simulation_control_pausing(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *motor_simulation_control_config = (motor_simulation_control_conf_t *) activity_config->state;

#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    log_msg_t temp_mesg;
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,motor_simulation_control_config->logfile,temp_mesg);
#endif

    if (disabling_motor_simulation_control_activity)
        add_event(&activity_config->lcsm,configure_capabilities);
    else if (enabled_motor_simulation_control_activity)
        add_event(&activity_config->lcsm,start);
}

void motor_simulation_control_running(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *motor_simulation_control_config = (motor_simulation_control_conf_t *) activity_config->state;
    log_msg_t temp_mesg;
#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,motor_simulation_control_config->logfile,temp_mesg);
#endif

    if (enabled_motor_simulation_control_activity) {



        if(command_increase_vbus) {
          command_increase_vbus = false;
          increase_vbus_value(motor_simulation_control_config->motor);
        }
        if(command_decrease_vbus) {
          command_decrease_vbus = false;
          decrease_vbus_value(motor_simulation_control_config->motor);
        }

        if(command_increase_loadt) {
          command_increase_loadt = false;
          increase_torque_value(motor_simulation_control_config->motor);
        }
        if(command_decrease_loadt) {
          command_decrease_loadt = false;
          decrease_torque_value(motor_simulation_control_config->motor);
        }


        if(command_increase_loadj) {
          command_increase_loadj = false;
          increase_inertia_value(motor_simulation_control_config->motor);
        }
        if(command_decrease_loadj) {
          command_decrease_loadj = false;
          decrease_inertia_value(motor_simulation_control_config->motor);
        }

        if(command_increase_speedref) {
          command_increase_speedref = false;
          increase_speedref_value(motor_simulation_control_config->motor);
        }
        if(command_decrease_speedref) {
          command_decrease_speedref = false;
          decrease_speedref_value(motor_simulation_control_config->motor);
        }

        if(command_increase_posref) {
          command_increase_posref = false;
          increase_posref_value(motor_simulation_control_config->motor);
        }
        if(command_decrease_posref) {
          command_decrease_posref = false;
          decrease_posref_value(motor_simulation_control_config->motor);
        }

        if(command_restart_motor_simulation) {
          command_restart_motor_simulation = false;
          restart(motor_simulation_control_config->motor);
        }

        if(command_openloop){
          command_openloop = false;
          // if(motor_simulation_control_config->motor->input_parametres->mode != open_mode){
            change_controller_mode(motor_simulation_control_config->motor, open_mode);
          // }
        }

        if(command_foc_speed){
          command_foc_speed = false;
          // if(motor_simulation_control_config->motor->input_parametres->mode != foc_speed){
            change_controller_mode(motor_simulation_control_config->motor, foc_speed);
          // }
        }

        if(command_foc_position){
          command_foc_position = false;
          // if(motor_simulation_control_config->motor->input_parametres->mode != foc_pos){
            change_controller_mode(motor_simulation_control_config->motor, foc_pos);
          // }
        }


        if(!command_pause_motor_simulation) {
          update_simulation_varible(motor_simulation_control_config->motor );
        }

        if (!toggle_printing){
            print_motor_result(motor_simulation_control_config->motor);
        }

    }

    if (disabling_motor_simulation_control_activity) {
        snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "Shutdown command received");
        logger(Info, motor_simulation_control_config->logfile, temp_mesg);

        add_event(&activity_config->lcsm,stop);
    }
    fflush(motor_simulation_control_config->logfile);
}

void motor_simulation_control_cleaning(activity_config_t *activity_config) {
  motor_simulation_control_conf_t *motor_simulation_control_config = (motor_simulation_control_conf_t *) activity_config->state;

#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    log_msg_t temp_mesg;
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,motor_simulation_control_config->logfile,temp_mesg);
#endif
        //TODO: confirm is correct
        conf_motor_simulation_control *motor_control =motor_simulation_control_config->motor;

        for(int i = 0; i < NUM_INPUT_SLIDER_PARA; i++) {
           // slider *input_para_control = motor_control->input_parametres->slider_input[i];
           free(motor_control->input_parametres->slider_input[i]);
         }
        free(motor_control->motor_state);
        free(motor_control->simulation_parametres);
        free(motor_control->input_parametres);



    free(motor_simulation_control_config->motor);

    cleaning_completed_motor_simulation_control_activity = true;
    add_event(&activity_config->lcsm, cleaned);

}


void motor_simulation_control_done(activity_config_t *activity_config) {
    motor_simulation_control_conf_t *motor_simulation_control_config = (motor_simulation_control_conf_t *) activity_config->state;

#ifdef PRINT_STATE_MOTOR_SIMULATION_CONTROL
    log_msg_t temp_mesg;
    snprintf(temp_mesg, MAX_LOG_MESSAGE_LEN, "%s state: %d, %s", activity_config->name, activity_config->lcsm.state, lcsm_state_names[activity_config->lcsm.state]);
    logger(Info,motor_simulation_control_state->logfile,temp_mesg);
#endif

}
