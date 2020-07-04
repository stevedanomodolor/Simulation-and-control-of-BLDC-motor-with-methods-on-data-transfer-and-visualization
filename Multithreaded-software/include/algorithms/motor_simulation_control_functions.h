// Software based on http://www.sosw.poznan.pl/tfitzer/pmsm/
// Copyright Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
// Project: Bachelor thesis- Simulaton and control of a BLDC motor with methods on data transfer and visualitzation
// Tutor : Herman Bruyninckx
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//@ @file motor_simulation_control_functions.h
// @brief Header of functions to move and manipulate tiles in a rectangular area
//
// This file is the header containing the definition of the functions that defines the motors behaviour
//
// (c) Filip Reniers (KU Leuven) 04.11.19



#ifndef MOTOR_SIMULATION_CONTROL_H
#define MOTOR_SIMULATION_CONTROL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "model.h"
#include "motor.h"
#include "FOC.h"
#include "openloop.h"
#include "PID.h"
#define NUM_INPUT_SLIDER_PARA 5


typedef enum
{
  open_mode,
  foc_speed,
  foc_pos
} controller;

typedef enum {
  vbus,
  loadt,
  loadj,
  rpm_ref,
  pos_ref
}sliders_input_para;


typedef struct
{
  float time; // time of simulation in seconds
  float va;
  float vc;
  float vb;
  float rpm;
  float pos; // curent position
  float torque;
  float iq;
  float id;
  int calculated_stream_rate;


} motor_simulation_r;


// keep track of the sliders current value and make sure it doesnt surpase the boundaries
typedef struct {
  int current_value;
  int max_boundary;
  int min_boundary;

}slider;

typedef struct {
  slider* slider_input[NUM_INPUT_SLIDER_PARA];
  controller mode;
}motor_input_parametre;

typedef struct {

  long last_loop;
  float t;
  float dt;
  float angle;
  float angleDeg;
  float vel_t;
  float velocity;
  float position;
  float ia, ib;
  float torque;
  float vel_cmd;
  float set_pos;
  int calculated_stream_rate;
  long new_loop;


}motor_simulation_parametres;


typedef struct  {
    motor_simulation_r *motor_state;
    motor_simulation_parametres *simulation_parametres;
    motor_input_parametre* input_parametres;

} conf_motor_simulation_control;

void increase_vbus_value(conf_motor_simulation_control * configuration);
void decrease_vbus_value(conf_motor_simulation_control * configuration);
void increase_torque_value(conf_motor_simulation_control * configuration);
void decrease_torque_value(conf_motor_simulation_control * configuration);
void increase_inertia_value(conf_motor_simulation_control * configuration);
void decrease_inertia_value(conf_motor_simulation_control * configuration);
void increase_speedref_value(conf_motor_simulation_control * configuration);
void decrease_speedref_value(conf_motor_simulation_control * configuration);
void increase_posref_value(conf_motor_simulation_control * configuration);
void decrease_posref_value(conf_motor_simulation_control * configuration);
void restart(conf_motor_simulation_control * configuration);
bool slider_max_point(slider *paramtres);
bool slider_min_point(slider *paramtres);
void update_simulation_varible(conf_motor_simulation_control * configuration);
void change_controller_mode(conf_motor_simulation_control *configuration, controller mode );
float max_num(float a, float b);
float min_num(float a, float b);
void printf_current_input_value(int value, sliders_input_para para);

#endif
