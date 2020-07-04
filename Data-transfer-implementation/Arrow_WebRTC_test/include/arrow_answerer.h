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
// @brief Header of arrow_answerer
//
// This file is the header of the arrow_answerer. All the variable and strcures are defined here


#include <rtc/rtc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep
#include <ctype.h>
#include <arrow-glib/arrow-glib.h>


typedef struct {
        rtcState state;
        rtcGatheringState gatheringState;
        int pc;
        int dc;
        bool connected;
} Peer;


extern Peer *peer;
static void print_record_batch(GArrowRecordBatch *record_batch);
void display_arrow_buffer(gconstpointer data_input, gsize data_size);
static void dataChannelCallback(int dc, void *ptr);
static void descriptionCallback(const char *sdp, const char *type, void *ptr);
static void candidateCallback(const char *cand, const char *mid, void *ptr);
static void stateChangeCallback(rtcState state, void *ptr);
static void gatheringStateCallback(rtcGatheringState state, void *ptr);
static void closedCallback(void *ptr);
static void messageCallback(const char *message, int size, void *ptr);
static void deletePeer(Peer *peer);
int all_space(const char *str);
char* state_print(rtcState state);
char* rtcGatheringState_print(rtcState state);
