/*
 *
 * @file command_line.h
 * @brief Header of commandline activity
 *
 * This file is the header of the commandline activity. The machine specific codes for keyboard buttons are defined here.
 *
 * (c) Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
 * Project: Bachelor thesis- Simulaton and control of a robot
 * Tutor : Herman Bruyninckx
 */




#ifndef RT_CONTROL_COMMAND_LINE_H
#define RT_CONTROL_COMMAND_LINE_H
#define PRINT_STATE_COMMANDLINE

#include "activities.h"
/*
#define BUTTON_ESCAPE 27
#define BUTTON_t 116
#define BUTTON_a 97
#define BUTTON_c 99
#define BUTTON_e 101
#define BUTTON_p 112
#define BUTTON_i 105
#define BUTTON_minus_sign 45
#define BUTTON_plus_sign 43

void commandline_create(activity_config_t *activity_config);
void commandline_resource_configure(activity_config_t *activity_config);
void commandline_capability_configure(activity_config_t *activity_config);
void commandline_pausing(activity_config_t *activity_config);
void commandline_running(activity_config_t *activity_config);
void commandline_cleaning(activity_config_t *activity_config);
void commandline_done(activity_config_t *activity_config);
*/
#define BUTTON_PAUSE_p 112
#define BUTTON_RESTART_r 114
#define BUTTON_OPEN_CONTROL_h 104
#define BUTTON_FOC_SPEED_j 106
#define BUTTON_FOC_POSITION_k 107
#define BUTTON_VBUS_MINUS_q 113
#define BUTTON_VBUS_PLUS_w 119
#define BUTTON_TORQUE_MINUS_a 97
#define BUTTON_TORQUE_PLUS_s 115
#define BUTTON_INERTIA_MINUS_z 122
#define BUTTON_INERTIA_PLUS_x 120
#define BUTTON_SPEADREF_MINUS_d 100
#define BUTTON_SPEADREF_PLUS_f 102
#define BUTTON_POSITIONREF_MINUS_c 99
#define BUTTON_POSITIONREF_PLUS_v 118
#define BUTTON_STOP_e 101
#define BUTTON_t 116





void commandline_create(activity_config_t *activity_config);
void commandline_resource_configure(activity_config_t *activity_config);
void commandline_capability_configure(activity_config_t *activity_config);
void commandline_pausing(activity_config_t *activity_config);
void commandline_running(activity_config_t *activity_config);
void commandline_cleaning(activity_config_t *activity_config);
void commandline_done(activity_config_t *activity_config);


#endif //RT_CONTROL_COMMAND_LINE_H
