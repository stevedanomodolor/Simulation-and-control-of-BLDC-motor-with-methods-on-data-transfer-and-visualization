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


#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#define NUM_PID_C 4 // Number of pid_controller currenly used,

#include "model.h"

/*******************************************************************************
*               PID CONTROLLER
*
*
*
*
*******************************************************************************/


typedef const struct
{
  float Kp; // proportional  gain
  float Ki; // integration gain
  int Kd;   // derivative gain
  // The following defines the output max and minimum values

} pid_gain_value;

typedef struct
{

  double p_state;        // current proportional state
  double int_state;      // current integral state
  double d_state;        // curent  derivative state
  double previous_error; // storing the previus error to compute the next derivate state

} pid_result_computation; // pid_r_c

typedef struct
{
  pid_gain_value pid_gain[NUM_PID_C];
  pid_result_computation pid_result_c[NUM_PID_C];
} pid_parametre;

/*******************************************************************************
*                   numebering system for the current pid used                 *
*                   Enum list defined in pid.h file                            *
*                                                                              *
*                                                                              *
* 0 - vq_pid;                                                                  *
* 1 - vd_pid;                                                                  *
* 2 - rpm_pid;                                                                 *
* 3 - pos_pid;                                                                 *
*******************************************************************************/
extern pid_parametre motor_pid_parametre;

/*******************************************************************************
*               MOTOR VARIABLE
*
*
*
*
*******************************************************************************/

typedef struct
{
  double ia; // phase current
  double ib;
  double ic;
  double ialpha; // ialpha, ibeta, id, iq //TODO: specific explanation of each term
  double ibeta;
  double id;
  double iq;
  double van; // pahse voltage
  double vbn;
  double vcn;
  double bemfa; //back-emf voltage
  double bemfb;
  double bemfc;
  double torque;   // torque
  double velocity; // motor velocity(radians/s)
  double position; //  motor position (radaians)

} motor_control_variable;

extern motor_control_variable motor_parametre;

/*******************************************************************************
*               FOC CONTROLLER
*
*
*
*
*******************************************************************************/
typedef struct
{

  double ialpha; // ialpha, ibeta, id, iq //TODO: specific explanation of each term
  double ibeta;
  double iq;
  double id;
  double valpha;
  double vbeta;
  double va;
  double vb;
  double vc;
  double torque; // torque
  double flux;
  double id_pid; // value defined
  double iq_pid; // value defiined
} foc_control_variable;

extern foc_control_variable foc_parametre;

/*******************************************************************************
*               OPENLOOP
*
*
*
*
*******************************************************************************/
typedef struct
{
  double va;
  double vb;
  double vc;
} openlopp_control_parametre;

extern openlopp_control_parametre openloop_motor_parametre;

#endif
