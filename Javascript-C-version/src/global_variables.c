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

#include "global_variables.h"




// TODO: Modify variballe type is necessary

// Setting the PDI values



pid_parametre motor_pid_parametre=
    { .pid_gain = {
        {0.5, 0.01, 0},{0.5, 0.01, 0},
        {5, 1E-5, 1000}, {50, 0, 10000}

        }
    };





motor_control_variable motor_parametre;


foc_control_variable foc_parametre;

openlopp_control_parametre openloop_motor_parametre;
