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
#include <math.h>
#include "motor.h"






// initial values for the motor_control_variable for motor 1// set model type and ind

// value initiated in the restart funtion();








void update_motor_control_variable(double t, double dt,
                                    double va, double vb, double vc,
                                    double load_T, double load_J)
{
  // phase voltage
  double vn;
  // vL of a single phase variables - declaration
  double vLa, vLb, vLc;
  // varibale intensity average - definition
  double iavg;
  // variables for angle computation
  double sina, sinb, sinc, angle; // TODO: verify the use of this variable
  // definition for simplicity
  //double sin30 = 0.5, cos30 = 0.8660254037844387;
  // clark transform
  // 1/sqrt(3)
  double sqrt3inv = 0.57735026919; // for more precisiion-> remove 9 896258;

  // park transform
  double s1, c1;

  //Wyne connection -phase volatges
  vn = ( va + vb +vc) / 3.0;
  motor_parametre.van = va - vn;
  motor_parametre.vbn = vb - vn;
  motor_parametre.vcn = vc - vn;

  // R- >resistance of a single Phase, particular to a motor
  // computation vl of each phase- definitions
  vLa = motor_parametre.van - motor_parametre.bemfa - (motor_parametre.ia * motor_model_parametre.R);
  vLb = motor_parametre.vbn - motor_parametre.bemfb - (motor_parametre.ib * motor_model_parametre.R);
  vLc = motor_parametre.vcn - motor_parametre.bemfc - (motor_parametre.ic * motor_model_parametre.R);

  // inductance of a single Phase, L is defined in the model
  // note that vL = L* di/dt
  motor_parametre.ia += (vLa / motor_model_parametre.L )*dt;
  motor_parametre.ib += (vLb / motor_model_parametre.L )*dt;
  motor_parametre.ic += (vLc / motor_model_parametre.L )*dt;

  // wye connection - sum of all currents must be 0
  // updating final current
  iavg = ( motor_parametre.ia + motor_parametre.ib + motor_parametre.ic) / 3.0;
  motor_parametre.ia -= iavg;
  motor_parametre.ib -= iavg;
  motor_parametre.ic -= iavg;


  // motor angles
  angle = (motor_parametre.position * motor_model_parametre.pole)/2;
  sina = sin(angle);
  sinb = sin(angle - ((M_PI*2)/3));
  sinc = sin(angle - ((M_PI*4)/3));

  // torqye value
  motor_parametre.torque = ((motor_parametre.ia * sina ) + (motor_parametre.ib*sinb) + (motor_parametre.ic * sinc)) * motor_model_parametre.Kt;

  // alpa, beta, d, q, model
  // clark transform
  motor_parametre.ialpha = motor_parametre.ia;
  motor_parametre.ibeta = sqrt3inv * ( motor_parametre.ia + (2*motor_parametre.ib));

  // park transform
  c1 = cos(angle);
  s1 = sin(angle);
  motor_parametre.id = c1 * motor_parametre.ialpha + s1*motor_parametre.ibeta;
  motor_parametre.iq = -s1 * motor_parametre.ialpha + c1*motor_parametre.ibeta;

  // update motor velocity and position
  // Te = J *dw/dt + B*w + Tload => (T-w*B) /J *dt
  motor_parametre.velocity += ((motor_parametre.torque - (motor_parametre.velocity* motor_model_parametre.B) - load_T)/(motor_model_parametre.J + load_J)) * dt;

  motor_parametre.position = motor_parametre.position + motor_parametre.velocity * dt;


  // update back-emf volatges
  // TODO: verify that kv is the same as kt, if not modify this next statements
  motor_parametre.bemfa = sina * motor_parametre.velocity * motor_model_parametre.Kv;
  motor_parametre.bemfb = sinb * motor_parametre.velocity * motor_model_parametre.Kv;
  motor_parametre.bemfc = sinc * motor_parametre.velocity * motor_model_parametre.Kv;


}
