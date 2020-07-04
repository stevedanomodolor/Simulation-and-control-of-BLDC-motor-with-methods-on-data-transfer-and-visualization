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
 // @file arrow_answerer.h
 // @brief Header of arrow_offerer
 //
 // This file is the header of the arrow_offerer. All the variable and structures are defined here



#include <rtc/rtc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep
#include <ctype.h>
#include <arrow-glib/arrow-glib.h>
#define NUMBER_OF_MOTOR 3
#define RECORD_BACTH_LEN 50
#define NUMBER_OF_DATA_SENT 3
#define NUMBER_OF_ITERATION 100

typedef struct
{
        rtcState state;
        rtcGatheringState gatheringState;
        int pc;
        int dc;
        bool connected;
} Peer;

enum valid_check
{
        position_valid,
        velocity_valid,
        torque_valid
};
 Peer* peer = NULL;

typedef struct
{
        gfloat position[RECORD_BACTH_LEN];
        gfloat velocity[RECORD_BACTH_LEN];
        gfloat torque[RECORD_BACTH_LEN];
        gboolean position_is_valid[RECORD_BACTH_LEN];
        gboolean velocity_is_valid[RECORD_BACTH_LEN];
        gboolean torque_is_valid[RECORD_BACTH_LEN];
        gint64 is_valid_length[NUMBER_OF_DATA_SENT]; // amount of non-null values per
        // data to send

} robot_data_input_t;

typedef struct
{
        GArrowFloatArrayBuilder* position_builder;
        GArrowFloatArrayBuilder* velocity_builder;
        GArrowFloatArrayBuilder* torque_builder;

} arrow_array_builder_t;

typedef struct
{
        arrow_array_builder_t* robot_build_data[NUMBER_OF_MOTOR];

        // buffer
        gconstpointer buffer; // gconstpointer is typdef for void *
        gsize buffer_size; // gsize is typedef for  signed long
        // amount of data to send per record_batches
        GError* error; // TODO: dont forget to intialize
} data_communication_config_t;



int create_arrow_buffer(data_communication_config_t* data_output, robot_data_input_t *data_input);
static void descriptionCallback(const char* sdp, const char* type, void* ptr);
static void candidateCallback(const char* cand, const char* mid, void* ptr);
static void stateChangeCallback(rtcState state, void* ptr);
static void gatheringStateCallback(rtcGatheringState state, void* ptr);
static void openCallback(void* ptr);
static void closedCallback(void* ptr);
static void messageCallback(const char* message, int size, void* ptr);
static void deletePeer(Peer* peer);
int all_space(const char* str);
char* state_print(rtcState state);
char* rtcGatheringState_print(rtcState state);
