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
#include "FOC.h"
#include "PID.h"










void update_foc_control_variable(double t, double dt, double vbus, double ia,
                                    double ib,double id_ref, double iq_ref, double angle)
{
  // clark transform
  // TODO- Try to improve by sharing variable that are constant values
  double sqrt3inv = 0.57735026919; // for more precisiion-> remove 9 896258;
  // park transform
  double s1, c1;
  // PI controllers
  double id_out;
  double iq_out;

  // normalizing variables
  double mag;


  double vn;

  // definition for simplicity
  double sin30 = 0.5, cos30 = 0.8660254037844387;

  // Park transform
  foc_parametre.ialpha = ia;
  foc_parametre.ibeta = sqrt3inv * (ia + 2 * ib);

  // park transform
  c1 = cos(angle);
  s1 = sin(angle);
  foc_parametre.id = c1 * foc_parametre.ialpha + s1*foc_parametre.ibeta;
  foc_parametre.iq = -s1 * foc_parametre.ialpha + c1*foc_parametre.ibeta;

  // Torque
  foc_parametre.torque = -foc_parametre.iq * motor_model_parametre.Kt;
  foc_parametre.flux = -foc_parametre.id * motor_model_parametre.Kt;

  // PI controller
  // TODO -modify for multiple motor simulation
  id_out = update_PID(vd_pid,  (id_ref - foc_parametre.id));
  iq_out = update_PID(vq_pid,  (iq_ref - foc_parametre.iq));

  // normalize
  mag = sqrt(id_out * id_out + iq_out * iq_out);
  if ( mag >1.0) {
    id_out/= mag;
    iq_out/= mag;
  }

  // inverse park transform
  // Why is c1 different from c2?
  foc_parametre.valpha = c1 * id_out - s1 * iq_out;
  foc_parametre.vbeta = s1 * id_out + c1 * iq_out;

  foc_parametre.valpha *=vbus;
  foc_parametre.vbeta *= vbus;


  // inverse Clarke transform
  foc_parametre.va = foc_parametre.valpha;
  foc_parametre.vb = -sin30 * foc_parametre.valpha + cos30 * foc_parametre.vbeta;
  foc_parametre.vc = -sin30 * foc_parametre.valpha - cos30 * foc_parametre.vbeta;

  vn = (foc_parametre.va + foc_parametre.vb + foc_parametre.vc)/3;
  foc_parametre.va -= vn;
  foc_parametre.vb -= vn;
  foc_parametre.vc -= vn;

}
