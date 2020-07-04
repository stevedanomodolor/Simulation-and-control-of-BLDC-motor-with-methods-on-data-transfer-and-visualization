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

//
// @file motor_simulation_control_functions.c
// @brief Implementation of functions to change motors state
//
// This file contains the implementation of C compatible functions for manipulating the motor's current state




 #include <string.h>

#include "motor_simulation_control_functions.h"

bool slider_max_point(slider * paramtres) {

  if (paramtres->current_value >= paramtres->max_boundary) {
    return true;
  }
  return false;
}


bool slider_min_point(slider * paramtres) {

  if (paramtres->current_value <= paramtres->min_boundary) {
    return true;
  }
  return false;
}

void increase_vbus_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = vbus;

  if(!slider_max_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value+= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, vbus);


}
void decrease_vbus_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = vbus;

  if(!slider_min_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value-= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, vbus);


}
void increase_torque_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = loadt;

  if(!slider_max_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value+= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, loadt);


}
void decrease_torque_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = loadt;

  if(!slider_min_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value-= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, loadt);


}
void increase_inertia_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = loadj;

  if(!slider_max_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value+= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, loadj);


}
void decrease_inertia_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = loadj;

  if(!slider_min_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value-= 1;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, loadj);


}
void increase_speedref_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = rpm_ref;

  if(!slider_max_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value+= 10;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, rpm_ref);


}
void decrease_speedref_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = rpm_ref;

  if(!slider_min_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value-= 10;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, rpm_ref);


}
void increase_posref_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = pos_ref;

  if(!slider_max_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value+= 10;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, pos_ref);


}
void decrease_posref_value(conf_motor_simulation_control * configuration)
{
  sliders_input_para type = pos_ref;

  if(!slider_min_point(configuration->input_parametres->slider_input[type])) {
    configuration->input_parametres->slider_input[type]->current_value-= 10;
  }
  printf_current_input_value(configuration->input_parametres->slider_input[type]->current_value, pos_ref);


}

void restart(conf_motor_simulation_control * configuration)
{

  configuration->simulation_parametres->t = 0; // s
  configuration->simulation_parametres->last_loop = 0;
  configuration->simulation_parametres->calculated_stream_rate = 0;
  configuration->simulation_parametres->angle = 0;
  configuration->simulation_parametres->velocity = 0;
  configuration->simulation_parametres->position = 0;
  configuration->simulation_parametres->ia = 0;
  configuration->simulation_parametres->ib = 0;
  configuration->simulation_parametres->vel_t = 0;
  configuration->simulation_parametres->torque = 0;
  configuration->simulation_parametres->new_loop=0;
  configuration->simulation_parametres->calculated_stream_rate=0;
  configuration->simulation_parametres-> angleDeg = 0;
  configuration->simulation_parametres-> vel_cmd = 0;
  configuration->simulation_parametres-> set_pos = 0;






  for (int i = 0; i < 4; i++)
  {
    motor_pid_parametre.pid_result_c[i].p_state = 0;   // current proportional state
    motor_pid_parametre.pid_result_c[i].int_state = 0; // current integral state
    motor_pid_parametre.pid_result_c[i].d_state = 0;   // curent  derivative state
    motor_pid_parametre.pid_result_c[i].previous_error = 0;
  }

  // resetting motor variables

  motor_parametre.ia = 0; // phase current
  motor_parametre.ib = 0;
  motor_parametre.ic = 0;
  motor_parametre.ialpha = 0; // ialpha, ibeta, id, iq //TODO: specific explanation of each term
  motor_parametre.ibeta = 0;
  motor_parametre.id = 0;
  motor_parametre.iq = 0;
  motor_parametre.van = 0; // pahse voltage
  motor_parametre.vbn = 0;
  motor_parametre.vcn = 0;
  motor_parametre.bemfa = 0; //back-emf voltage
  motor_parametre.bemfb = 0;
  motor_parametre.bemfc = 0;
  motor_parametre.torque = 0;   // torque   motor_parametre.velocity= 0; // motor velocity(radians/s)
  motor_parametre.position = 0; //  motor position (radaians)

  // FOC resetting

  foc_parametre.ialpha = 0; // ialpha, ibeta, id, iq //TODO: specific explanation of each term
  foc_parametre.ibeta = 0;
  foc_parametre.iq = 0;
  foc_parametre.id = 0;
  foc_parametre.valpha = 0;
  foc_parametre.vbeta = 0;
  foc_parametre.va = 0;
  foc_parametre.vb = 0;
  foc_parametre.vc = 0;
  foc_parametre.torque = 0; // torque
  foc_parametre.flux = 0;
  foc_parametre.id_pid = 0; // value defined
  foc_parametre.iq_pid = 0; // value defiined
  // Resetting openloop variable

  openloop_motor_parametre.va = 0;
  openloop_motor_parametre.vb = 0;
  openloop_motor_parametre.vc = 0;
}


void update_simulation_varible(conf_motor_simulation_control *configuration)
{

  float Vbus = configuration->input_parametres->slider_input[vbus]->current_value * 1.0;
  float Torque = configuration->input_parametres->slider_input[loadt]->current_value * 1E-3;
  float Inertia = configuration->input_parametres->slider_input[loadj]->current_value * 1E-5;
  float Rpm_ref =  configuration->input_parametres->slider_input[rpm_ref]->current_value * 1.0;
  float Pos_ref = configuration->input_parametres->slider_input[pos_ref]->current_value * 1.0;

  float va, vb, vc;
  int steps;
  struct timespec start;
  //  To make sure that the time was collected correctly
  if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
  {
    perror("clock gettime");
    printf("Timing error\n");
    exit(EXIT_FAILURE);
  }
  configuration->simulation_parametres->new_loop = start.tv_nsec; // getting milli seconds value
  /// TODO: CONFIRM IF  CORRECT
  if (configuration->simulation_parametres->last_loop == 0)
  {
    configuration->simulation_parametres->last_loop = configuration->simulation_parametres->new_loop;

  }
  else
  {
    configuration->simulation_parametres->calculated_stream_rate = (1000000000 / (configuration->simulation_parametres->new_loop-configuration->simulation_parametres->last_loop ));
    configuration->simulation_parametres->last_loop = configuration->simulation_parametres->new_loop;
    // configuration->simulation_parametres->angle = 0;
    // configuration->simulation_parametres->angleDeg = 0;
    steps = 1 / (configuration->simulation_parametres->calculated_stream_rate * configuration->simulation_parametres->dt);
    for (int i = 0; i < steps; i++)
    {
      configuration->simulation_parametres->angle = configuration->simulation_parametres->position * motor_model_parametre.pole / 2;
      configuration->simulation_parametres->angleDeg = configuration->simulation_parametres->angle / M_PI * 180.0;
      if (configuration->input_parametres->mode == open_mode)
      {
        OpenLoop(configuration->simulation_parametres->t, Vbus, Rpm_ref);
        va = openloop_motor_parametre.va;
        vb = openloop_motor_parametre.vb;
        vc = openloop_motor_parametre.vc;
      }

      if (configuration->input_parametres->mode == foc_speed)
      {
        configuration->simulation_parametres->torque  = update_PID(rpm_pid, (Rpm_ref - configuration->simulation_parametres->velocity)) * motor_model_parametre.Tmax;
        update_foc_control_variable(configuration->simulation_parametres->t, configuration->simulation_parametres->dt, Vbus, configuration->simulation_parametres->ia, configuration->simulation_parametres->ib, 0, (-configuration->simulation_parametres->torque / motor_model_parametre.Kt), configuration->simulation_parametres->angle);
        va = foc_parametre.va;
        vb = foc_parametre.vb;
        vc = foc_parametre.vc;
      }

      if (configuration->input_parametres->mode == foc_pos)
      {
        configuration->simulation_parametres->set_pos = Pos_ref;
          if (Pos_ref== 0)
          {
            configuration->simulation_parametres->set_pos = sin(configuration->simulation_parametres->t / 2 * (M_PI * 2.0)) * 1800 + 1800;
          }
          configuration->simulation_parametres->vel_cmd = update_PID(pos_pid, (configuration->simulation_parametres->set_pos - configuration->simulation_parametres->angleDeg)) * 10000;
          configuration->simulation_parametres->vel_cmd = max_num(-Rpm_ref, min_num(Rpm_ref, configuration->simulation_parametres->vel_cmd));
          configuration->simulation_parametres->torque = update_PID(rpm_pid, configuration->simulation_parametres->vel_cmd - configuration->simulation_parametres->velocity) * motor_model_parametre.Tmax;
          update_foc_control_variable(configuration->simulation_parametres->t, configuration->simulation_parametres->dt, Vbus, configuration->simulation_parametres->ia, configuration->simulation_parametres->ib, 0, (-configuration->simulation_parametres->torque / motor_model_parametre.Kt), configuration->simulation_parametres->angle);
          va = foc_parametre.va;
          vb = foc_parametre.vb;
          vc = foc_parametre.vc;

      }

      update_motor_control_variable(configuration->simulation_parametres->t, configuration->simulation_parametres->dt, va, vb, vc, Torque, Inertia);
      configuration->simulation_parametres->t += configuration->simulation_parametres->dt;

      if (configuration->simulation_parametres->t)
      {
        configuration->simulation_parametres->velocity = 0.97 * configuration->simulation_parametres->velocity + 0.03 * ((motor_parametre.position - configuration->simulation_parametres->position) / (configuration->simulation_parametres->t - configuration->simulation_parametres->vel_t) * (30 / M_PI));
      }

      configuration->simulation_parametres->vel_t = configuration->simulation_parametres->t;

      configuration->simulation_parametres->position = motor_parametre.position;

      configuration->simulation_parametres->ia = motor_parametre.ia;
      configuration->simulation_parametres->ib = motor_parametre.ib;
    }
    configuration->motor_state->time = configuration->simulation_parametres->t; // time of simulation in seconds
    configuration->motor_state->va = va;
    configuration->motor_state->vc = vb;
    configuration->motor_state->vb = vc;
    configuration->motor_state->rpm = configuration->simulation_parametres->velocity;
    if (configuration->simulation_parametres->angleDeg <= 360) {
    configuration->motor_state->pos = configuration->simulation_parametres->angleDeg;
}
      else {
            configuration->motor_state->pos = 360;
      }// curent position
    configuration->motor_state->torque = configuration->simulation_parametres->torque;
    configuration->motor_state->iq = motor_parametre.iq;
    configuration->motor_state->id = motor_parametre.id;
    configuration->motor_state->calculated_stream_rate = configuration->simulation_parametres->calculated_stream_rate;

  }

}


float max_num(float a, float b)
{
  if (a < b)
  {
    return b;
  }

  return a;
}

float min_num(float a, float b)
{
  if (a < b)
  {
    return a;
  }
  return b;
}

void change_controller_mode(conf_motor_simulation_control *configuration, controller mode ){

  configuration->input_parametres->mode = mode;


}

void printf_current_input_value(int value, sliders_input_para para)
{
  char *input_para = (char*) malloc(10 * sizeof(char));;
  switch(para)
  {
    case vbus: strcpy(input_para, "vbus"); break;
    case loadt: strcpy(input_para ,"loadt"); break;
    case loadj: strcpy(input_para ,"loadj"); break;
    case rpm_ref: strcpy(input_para, "rpm_ref"); break;
    case pos_ref: strcpy(input_para ,"pos_ref"); break;
  }
  printf("The current input value for %s is: %d\n", input_para, value);
  free(input_para);
}
