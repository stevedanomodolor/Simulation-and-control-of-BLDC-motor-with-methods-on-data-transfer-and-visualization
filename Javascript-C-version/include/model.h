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
#ifndef MODEL_H
#define  MODEL_H

// Defining motor model varibals
//TODO: CONFIRM WITH THOERY WHETHER THIS ARE CORRECT, IF NOT, IMPROVE MODEL FOR MULTIPLE MOTOR
typedef struct  {
  float Kt;// = 0.2; // Torque constant [Nm/A] == [V/(rad/s)]
  float Kv;// = 0.2; // Motor velocity coonstant or back ENF constant [V/(rad/s)]
  float R; //= 2.5; // Phase Resistance [Ohm]
  float L;// = 2.5e-3; // Phase Inductance [H]
  float J;// = 1e-3; //Rotor inertia [kg*m²]
  float B;// = 1.0e-4; // Friction [Nm/(rad/s)]
  float Tmax;// = 2.0; //Max Torque [Nm]
  float Imax; //= 10;

  /******TODO******/
  /// value might change in the futur, be care full
  double tm;// =  R*J/(Kt*Kv)    // mechanical time contant tm = R*J/(Kt*Kv) [s]
  double te;// =  L/R,   // electrical time constant te = L/R [s]
  int pole; // motor pole

  } model_parametre;

  extern model_parametre motor_model_parametre;



#endif
