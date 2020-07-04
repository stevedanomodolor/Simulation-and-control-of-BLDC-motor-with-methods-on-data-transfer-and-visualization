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



#include <stdio.h>


#include "PID.h"


// Confirm assumtion

// assumption that this variables are constant
int integral_max = 1;
int integral_min = -1;
int output_max = 1;
int output_min = -1;


double update_PID(pid_type pid,  double error)
{
  double output;

  // portpotional part
  motor_pid_parametre.pid_result_c[pid].p_state = error * motor_pid_parametre.pid_gain[pid].Kp;

  // Integration part
  motor_pid_parametre.pid_result_c[pid].int_state += (error * motor_pid_parametre.pid_gain[pid].Ki);

   // derivative part
  motor_pid_parametre.pid_result_c[pid].d_state = (error - motor_pid_parametre.pid_result_c[pid].previous_error) * motor_pid_parametre.pid_gain[pid].Kd;
  motor_pid_parametre.pid_result_c[pid].previous_error = error;


  if (motor_pid_parametre.pid_result_c[pid].int_state > integral_max)
    motor_pid_parametre.pid_result_c[pid].int_state = integral_max;

  else if (motor_pid_parametre.pid_result_c[pid].int_state < integral_min)
    motor_pid_parametre.pid_result_c[pid].int_state = integral_min;



  // final output

  output = motor_pid_parametre.pid_result_c[pid].int_state + motor_pid_parametre.pid_result_c[pid].p_state + motor_pid_parametre.pid_result_c[pid].d_state;
  if (output > output_max)
    output = output_max;
  else if (output < output_max)
    output= output_min;


  return output;





}
