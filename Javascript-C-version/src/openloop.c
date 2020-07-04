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


#include <math.h>

#include "openloop.h"


// setting initial parameter

void OpenLoop(double t, double Vbus, int rpm)
{
  double omega = 2* M_PI * rpm * motor_model_parametre.pole /(2*60); // 4*pi *f/p
  openloop_motor_parametre.va = Vbus * sin(t*omega);
  openloop_motor_parametre.vb = Vbus * sin(t*omega- ((M_PI *2)/3));
  openloop_motor_parametre.vc = Vbus * sin(t*omega - ((M_PI *4)/3));
}
