// Software based on http://www.sosw.poznan.pl/tfitzer/pmsm/
// Copyright Stevedan Ogochukwu Omodolor 17/05/2020 Ku leuven UPC EEBE
// Project: Bachelor thesis-Simulaton and control of a BLDC motor with methods on data transfer and visualitzation
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

#ifdef __EMSCRIPTEN__ // This is the emscripten library's header file
#include <emscripten.h>
#endif

#define NSEC_SEC_CONVERSTION 1000000000





// Different control mode
typedef enum
{
        open,
        foc_speed,
        foc_pos

} controller;

// Data to be sent for visualitzation
typedef struct
{
        float time; // time of simulation in seconds
        float va;
        float vc;
        float vb;
        float rpm; // current velocity
        float pos; // curent position
        float iq;
        float id;
        float avg_calculated_stream_rate;
        float calculated_stream_rate;
} output_data_javascript;

// intiliatizing function

output_data_javascript result_comp = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

long last_loop;
float t;
const float dt = 0.0001;
int avg_calculated_stream_rate;
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
controller mode_previous = 0;

// Helper functions

float max_num(float a, float b)
{
        return ((a > b) ? a : b);
}

float min_num(float a, float b)
{
        return ((b > a) ? a : b);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif

void restart(void)
{

        t = 0; // s
        last_loop = 0;
        avg_calculated_stream_rate = 0;
        angle = 0;
        velocity = 0;
        position = 0;
        ia = 0;
        ib = 0;
        vel_t = 0;
        angleDeg = 0;
        calculated_stream_rate = 0;

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
        motor_parametre.ialpha = 0; // ialpha, ibeta, id, iq
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

        foc_parametre.ialpha = 0; // ialpha, ibeta, id, iq
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

// this way the compiler does not rename the functions

/*******************************************************************************************
 *  This funtion is given the paramtres choosen in jv, compute and return a struct that    *
 *  processed in javascript and stream through perpective                                  *
 *
 *
 * ****************************************************************************************/

// THIS IS TO KEEP THIS FUNCTION ALIVE IN JAVASCRIPT
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif

output_data_javascript *computational_data(float vbus, float loadt,
                                           float loadj, float pos_ref, float rpm, controller mode)
{

        float va, vb, vc;
        // If there is a change in controller mode, restart the simulation
        if (mode_previous != mode)
        {
                restart();
                mode_previous = mode;
        }

        struct timespec start;

        //  To make sure that the time was collected correctly
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        {
                perror("clock gettime");
                exit(EXIT_FAILURE);
        }
        //Keep Track of time
        new_loop = start.tv_nsec; // getting milli seconds value

        if (last_loop == 0)
        {
                last_loop = new_loop;
                return &result_comp;
        }

        else
        {

                calculated_stream_rate = (NSEC_SEC_CONVERSTION / (new_loop - last_loop));
                last_loop = new_loop;
                // This is to calculate the avergae stream
                avg_calculated_stream_rate = (avg_calculated_stream_rate * 0.9) + (calculated_stream_rate * 0.1);
                // angle = 0;
                // angleDeg = 0;

                int step = 1 / (calculated_stream_rate * dt);
                for (int i = 0; i < step; i++)
                {
                        angle = position * motor_model_parametre.pole / 2;
                        angleDeg = angle / M_PI * 180.0;

                        if (mode == open)
                        {
                                OpenLoop(t, vbus, rpm);
                                va = openloop_motor_parametre.va;
                                vb = openloop_motor_parametre.vb;
                                vc = openloop_motor_parametre.vc;
                        }

                        if (mode == foc_speed)
                        {
                                torque = update_PID(rpm_pid, (rpm - velocity)) * motor_model_parametre.Tmax;
                                update_foc_control_variable(t, dt, vbus, ia, ib, 0, (-torque / motor_model_parametre.Kt), angle);
                                va = foc_parametre.va;
                                vb = foc_parametre.vb;
                                vc = foc_parametre.vc;
                        }

                        if (mode == foc_pos)
                        {
                                set_pos = pos_ref;
                                if (pos_ref == 0)
                                {
                                        set_pos = sin(t / 2 * (M_PI * 2.0)) * 1800 + 1800;
                                }
                                vel_cmd = update_PID(pos_pid, (set_pos - angleDeg)) * 10000;
                                vel_cmd = max_num(-rpm, min_num(rpm, vel_cmd));
                                torque = update_PID(rpm_pid, vel_cmd - velocity) * motor_model_parametre.Tmax;
                                update_foc_control_variable(t, dt, vbus, ia, ib, 0, (-torque / motor_model_parametre.Kt), angle);
                                va = foc_parametre.va;
                                vb = foc_parametre.vb;
                                vc = foc_parametre.vc;
                        }

                        update_motor_control_variable(t, dt, va, vb, vc, loadt, loadj);
                        t += dt;

                        if (t)
                        {
                                velocity = 0.97 * velocity + 0.03 * ((motor_parametre.position - position) / (t - vel_t) * (30 / M_PI));
                        }

                        vel_t = t;

                        position = motor_parametre.position;


                        ia = motor_parametre.ia;
                        ib = motor_parametre.ib;
                }
                // floorf -> two decimal points

                result_comp.va = floorf(va * 100) / 100;
                result_comp.vb = floorf(vb * 100) / 100;
                result_comp.vc = floorf(vc * 100) / 100;
                result_comp.pos = floorf(angleDeg * 100) / 100;
                result_comp.avg_calculated_stream_rate = avg_calculated_stream_rate;
                result_comp.calculated_stream_rate = calculated_stream_rate;
                result_comp.time = floorf(t * 100) / 100;
                result_comp.iq = floorf(motor_parametre.iq * 100) / 100;
                result_comp.id = floorf(motor_parametre.id * 100) / 100;
                result_comp.rpm = floorf(velocity * 100) / 100;
        }

        return &result_comp;
}
