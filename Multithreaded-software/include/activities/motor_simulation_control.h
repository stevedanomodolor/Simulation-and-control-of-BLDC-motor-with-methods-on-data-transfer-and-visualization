/*
 *
 * @file command_line.h
 * @brief Header of motor_simulation_control activity
 *
 * This file is the header of the motor_simulation_control activity. 
 *
 * (c) Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a robot
 * Tutor : Herman Bruyninckx
 */




#ifndef HEADER_MOTOR_SIMULATION_CONTROL_H
#define HEADER_MOTOR_SIMULATION_CONTROL_H
//#define PRINT_STATE_tile_control

#include <stdio.h>
#include <stdint.h>

#include "activities.h"





void motor_simulation_control_create(activity_config_t *activity_config);
void motor_simulation_control_resource_configure(activity_config_t *activity_config);
void motor_simulation_control_capability_configure(activity_config_t *activity_config);
void motor_simulation_control_pausing(activity_config_t *activity_config);
void motor_simulation_control_running(activity_config_t *activity_config);
void motor_simulation_control_cleaning(activity_config_t *activity_config);
void motor_simulation_control_done(activity_config_t *activity_config);

#endif // HEADER_tile_control_H
