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
 //
 // @file arrow_answerer.c
 // @brief Implementation of arrow_offerer
 //
 // This file is the implementation of the arrow_answerer. It creats data and converts it to the arrow format after the connection between the two peer has being made.It sends a recordbatch of a particular length that can be modified every 1 seconds.
 // that data to console. This example requires the sdp protocol and canditate data be copied manually to the console to make the initial
 // connection and open the datachannel.




#include "arrow_offerer.h"






int main(void)
{
        int i;
        int j;
        int p;
        int number_of_iteration = 0;
        //initializing data
        robot_data_input_t robot_data[NUMBER_OF_MOTOR];
        data_communication_config_t data_to_send = {.buffer=NULL, .buffer_size=0, .error=NULL};
        for(i = 0; i < NUMBER_OF_MOTOR; i++) {
                data_to_send.robot_build_data[i] = (arrow_array_builder_t *)malloc(NUMBER_OF_MOTOR*sizeof(arrow_array_builder_t));;
        }
        for (i = 0; i < NUMBER_OF_MOTOR; i++) {
          for(int j = 0; j < RECORD_BACTH_LEN; j++){

                robot_data[i].position[j] = 0 ;
                robot_data[i].velocity[j] = 0 ;
                robot_data[i].torque[j] = 0 ;
                robot_data[i].position_is_valid[j] =  true ;
                robot_data[i].velocity_is_valid[j] =  true ;
                robot_data[i].torque_is_valid[j] =  true ;
              }
                for (p = position_valid; p <= torque_valid; p++) {
                        robot_data[i].is_valid_length[p] = RECORD_BACTH_LEN;
                }
        }
        // data_to_send->buffer = ;
        // data_to_send->buffer_size = 0;
        // data_to_send->error=NULL;



        // do the configuration for the peer to peer communication
        rtcInitLogger(RTC_LOG_DEBUG);

        // Create peer
        rtcConfiguration config;
        memset(&config, 0, sizeof(config));

        Peer *peer = (Peer *)malloc(sizeof(Peer));
        if (!peer) {

                printf("Error allocating memory for peer\n");
                deletePeer(peer);

        }
        memset(peer, 0, sizeof(Peer));

        printf("Peer created\n");

        // Create peer connection
        peer->pc = rtcCreatePeerConnection(&config);
        rtcSetUserPointer(peer->pc, peer);
        rtcSetLocalDescriptionCallback(peer->pc, descriptionCallback);
        rtcSetLocalCandidateCallback(peer->pc, candidateCallback);
        rtcSetStateChangeCallback(peer->pc, stateChangeCallback);
        rtcSetGatheringStateChangeCallback(peer->pc, gatheringStateCallback);

        // Since this is the offere, we will create a datachannel
        peer->dc = rtcCreateDataChannel(peer->pc, "test");

        rtcSetOpenCallback(peer->dc, openCallback);


        rtcSetClosedCallback(peer->dc, closedCallback);

        rtcSetMessageCallback(peer->dc, messageCallback);


        sleep(1);

        bool exit = false;

        while (!exit) {

                printf("\n");
                printf("***************************************************************************************\n");

                // << endl
                printf("* 0: Exit /"
                       " 1: Enter remote description /"
                       " 2: Enter remote candidate *\n"
                       "[Command]: ");

                int command = -1;
                int c;
                if (scanf("%d", &command)) {

                }else {
                        break;
                }
                while ((c = getchar()) != '\n' && c != EOF) { }
                fflush(stdin);

                switch (command) {
                case 0: {
                        exit = true;
                        break;
                }
                case 1: {
                        // Parse Description
                        printf("[Description]: ");


                        char *line = NULL;
                        size_t len = 0;
                        size_t read = 0;
                        char *sdp = (char*) malloc(sizeof(char));
                        while ((read = getline(&line, &len, stdin)) != -1 && !all_space(line)) {
                                sdp = (char*) realloc (sdp,(strlen(sdp)+1) +strlen(line)+1);
                                strcat(sdp, line);

                        }
                        printf("%s\n",sdp);
                        rtcSetRemoteDescription(peer->pc, sdp, "answer");
                        free(sdp);
                        free(line);
                        break;

                }
                case 2: {
                        // Parse Candidate
                        printf("[Candidate]: ");
                        char* candidate = NULL;
                        size_t candidate_size = 0;
                        if(getline(&candidate, &candidate_size, stdin)) {
                                rtcAddRemoteCandidate(peer->pc, candidate, "0");
                                free(candidate);

                        }else {
                                printf("Error reading line\n");

                        }


                        break;
                }
                default: {
                        printf("** Invalid Command **");
                        break;
                }
                }
                if(peer->connected) {
                 exit = true;
                }
        }




        // when communication is done, begin sending the messgae


        bool shutdown = false;

        while (!shutdown) {


                //start sending the messgae
                usleep(200000);
                //create data to send
                for (i = 0; i < NUMBER_OF_MOTOR; i++) {
                        // generate the data and store them in and arrray
                        for (j = 0; j < RECORD_BACTH_LEN; j++) {
                                // fill array
                                robot_data[i].position[j] = 1;
                                robot_data[i].velocity[j] = 1;
                                robot_data[i].torque[j] = 1;
                                // checking if any value is null for arrow building, in case the data was not created

                        }
                }

                // Convert data to arrow columna structure and create buffer
                if (create_arrow_buffer(&data_to_send, robot_data) == 1) {
                        printf("Buffer was created without any issue\n");
                        // derefereing builder
                        // send the buffer to other peer connected
                        if(peer->state==RTC_CONNECTED && peer->connected) {
                        rtcSendMessage(peer->dc, (char*)data_to_send.buffer, (int)data_to_send.buffer_size);
                       }else {
                        printf("There was an error connecting the peer and opening the channel\n");
                        return 0;
                       }

                }
                else {
                        printf("Error creating arrow buffer");
                        shutdown = true;
                }


                // data_to_send.buffer=NULL;

                number_of_iteration++;
                if (number_of_iteration == NUMBER_OF_ITERATION) {
                        shutdown = true;
                }
        }

        // delecting peer to peer connection
        deletePeer(peer);
        for (int p = 0; p < NUMBER_OF_MOTOR; p++) {
                g_object_unref(data_to_send.robot_build_data[p]->position_builder);
                g_object_unref(data_to_send.robot_build_data[p]->velocity_builder);
                g_object_unref(data_to_send.robot_build_data[p]->torque_builder);
        }

        for(i = 0; i < NUMBER_OF_MOTOR; i++) {
                free(data_to_send.robot_build_data[i]);
        }

        printf("Number of iteratiions completed: %d\n", number_of_iteration );


        return 0;
}


int create_arrow_buffer(data_communication_config_t* data_output, robot_data_input_t *data_input)
{

        // First we create the array for each of the members of the position structure object
        GArrowArray* arrow_array_position[NUMBER_OF_MOTOR];
        GArrowArray* arrow_array_velocity[NUMBER_OF_MOTOR];
        GArrowArray* arrow_array_torque[NUMBER_OF_MOTOR];
        // initializingand building array
        gboolean success = TRUE;

        for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {

                data_output->robot_build_data[i]->position_builder = garrow_float_array_builder_new();
                data_output->robot_build_data[i]->velocity_builder = garrow_float_array_builder_new();
                data_output->robot_build_data[i]->torque_builder = garrow_float_array_builder_new();
                if (success) {
                        success = garrow_float_array_builder_append_values(data_output->robot_build_data[i]->position_builder, data_input[i].position, RECORD_BACTH_LEN, data_input[i].position_is_valid, data_input[i].is_valid_length[position_valid], &(data_output->error));
                }
                if (success) {
                        success = garrow_float_array_builder_append_values(data_output->robot_build_data[i]->velocity_builder, data_input[i].velocity, RECORD_BACTH_LEN, data_input[i].velocity_is_valid, data_input[i].is_valid_length[velocity_valid], &(data_output->error));
                }
                if (success) {
                        success = garrow_float_array_builder_append_values(data_output->robot_build_data[i]->torque_builder, data_input[i].torque, RECORD_BACTH_LEN, data_input[i].torque_is_valid, data_input[i].is_valid_length[torque_valid], &(data_output->error));
                }

                if (!success) {
                        g_print("failed to append: %s\n", data_output->error->message);
                        g_error_free(data_output->error);
                        for (int p = 0; p == i; p++) {
                                g_object_unref(data_output->robot_build_data[i]->position_builder);
                                g_object_unref(data_output->robot_build_data[i]->velocity_builder);
                                g_object_unref(data_output->robot_build_data[i]->torque_builder);
                        }
                        return 0;
                }
        }

        // creating array

        for (int i = 0; i < NUMBER_OF_MOTOR; i++) {
                arrow_array_position[i] = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(data_output->robot_build_data[i]->position_builder), &(data_output->error));
                arrow_array_velocity[i] = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(data_output->robot_build_data[i]->velocity_builder), &(data_output->error));
                arrow_array_torque[i] = garrow_array_builder_finish(GARROW_ARRAY_BUILDER(data_output->robot_build_data[i]->torque_builder), &(data_output->error));
                if (!arrow_array_position[i] || !arrow_array_velocity[i] || !arrow_array_torque[i]) {
                        g_print("Failed to finish: %s\n", data_output->error->message);
                        g_error_free(data_output->error);
                        for (int p = 0; p == NUMBER_OF_MOTOR; p++) {
                                g_object_unref(data_output->robot_build_data[i]->position_builder);
                                g_object_unref(data_output->robot_build_data[i]->velocity_builder);
                                g_object_unref(data_output->robot_build_data[i]->torque_builder);
                        }
                        return 0;
                }
        }

        GArrowField* fields[NUMBER_OF_MOTOR * NUMBER_OF_DATA_SENT];
        GList* fields_list = NULL; // declaring the fields

        for (int i = 0; i < (NUMBER_OF_MOTOR * NUMBER_OF_DATA_SENT); i++) {
                char number_str[11];
                char column[20] = "column_";
                sprintf(number_str, "%d", i);
                strcat(column, number_str);

                fields[i] = garrow_field_new(column, GARROW_DATA_TYPE(garrow_float_data_type_new()));
                fields_list = g_list_append(fields_list, fields[i]);
        }

        GArrowSchema* schema;
        schema = garrow_schema_new(fields_list);
        // derefereing field list
        g_list_free(fields_list);
        GList* columns = NULL;

        // creating the record_batch
        // creating list of the column data
        for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                columns = g_list_append(columns, arrow_array_position[i]);
                columns = g_list_append(columns, arrow_array_velocity[i]);
                columns = g_list_append(columns, arrow_array_torque[i]);
        }

        GArrowRecordBatch* record_batch = garrow_record_batch_new(schema, RECORD_BACTH_LEN, columns, &(data_output->error));
        // derefereing columns list
        g_list_free(columns);

        // checking for error while building
        if (!record_batch) {
                g_print("error: %s\n", data_output->error->message);
                for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                        g_object_unref(arrow_array_position[i]);
                        g_object_unref(arrow_array_velocity[i]);
                        g_object_unref(arrow_array_torque[i]);
                }
                for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                        g_object_unref(fields[i]);
                }
                g_object_unref(schema);
                return 0;
                // In real program, use this as an exception handling to go out of the thread	// TODO: confirm
        }

        // sending the data to a buffer

        // buffer initial size
        printf("Arrived\n");

        gint64 initial_size = 2000;
        GArrowResizableBuffer* buffer = garrow_resizable_buffer_new(initial_size, &(data_output->error));
        if (!buffer) {
                g_print("error: %s\n", data_output->error->message);
                for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                        g_object_unref(arrow_array_position[i]);
                        g_object_unref(arrow_array_velocity[i]);
                        g_object_unref(arrow_array_torque[i]);
                }
                for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                        g_object_unref(fields[i]);
                }
                g_object_unref(schema);
                g_object_unref(record_batch);
                return 0;
        }

        // creating the stream
        GArrowBufferOutputStream* output = garrow_buffer_output_stream_new(buffer);

        //putting the record inside the steam
        GArrowRecordBatchStreamWriter* writer = garrow_record_batch_stream_writer_new(GARROW_OUTPUT_STREAM(output),
                                                                                      schema, &(data_output->error));
        if (!writer) {
                printf("Error building stream writter\n");
                g_print("error: %s\n", data_output->error->message);
                for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                        g_object_unref(arrow_array_position[i]);
                        g_object_unref(arrow_array_velocity[i]);
                        g_object_unref(arrow_array_torque[i]);
                }
                for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                        g_object_unref(fields[i]);
                }
                g_object_unref(schema);
                g_object_unref(record_batch);
                g_object_unref(output);
                g_object_unref(buffer);
                return 0;
        }

        if (!garrow_record_batch_writer_write_record_batch(GARROW_RECORD_BATCH_WRITER(writer),
                                                           record_batch, &(data_output->error))) {
                printf("Error building stream writter\n");
                g_print("error: %s\n", data_output->error->message);
                for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                        g_object_unref(arrow_array_position[i]);
                        g_object_unref(arrow_array_velocity[i]);
                        g_object_unref(arrow_array_torque[i]);
                }
                for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                        g_object_unref(fields[i]);
                }
                g_object_unref(schema);
                g_object_unref(record_batch);
                g_object_unref(writer);
                g_object_unref(output);
                g_object_unref(buffer);
                return 0;
        }

        if (!garrow_record_batch_writer_close(GARROW_RECORD_BATCH_WRITER(writer), &(data_output->error))) {
                printf("Error closing stream writter\n");
                g_print("error: %s\n", data_output->error->message);
                for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                        g_object_unref(arrow_array_position[i]);
                        g_object_unref(arrow_array_velocity[i]);
                        g_object_unref(arrow_array_torque[i]);
                }
                for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                        g_object_unref(fields[i]);
                }
                g_object_unref(schema);
                g_object_unref(record_batch);
                g_object_unref(writer);
                g_object_unref(output);
                g_object_unref(buffer);
                return 0;
        }

        GBytes* data = garrow_buffer_get_data(GARROW_BUFFER(buffer));

        data_output->buffer = g_bytes_get_data(data, &(data_output->buffer_size));

        for (int i = 0; i < (NUMBER_OF_MOTOR); i++) {
                g_object_unref(arrow_array_position[i]);
                g_object_unref(arrow_array_velocity[i]);
                g_object_unref(arrow_array_torque[i]);
        }
        for (int i = 0; i < (NUMBER_OF_MOTOR + NUMBER_OF_DATA_SENT); i++) {
                g_object_unref(fields[i]);
        }
        g_object_unref(schema);
        g_object_unref(record_batch);
        g_object_unref(writer);
        g_object_unref(output);
        g_object_unref(buffer);
        return 1;
}


static void descriptionCallback(const char *sdp, const char *type, void *ptr) {
        // Peer *peer = (Peer *)ptr;
        printf("Description %s:\n%s\n", "offerer", sdp);
}

static void candidateCallback(const char *cand, const char *mid, void *ptr) {
        // Peer *peer = (Peer *)ptr;
        printf("Candidate %s: %s\n", "offerer", cand);

}

static void stateChangeCallback(rtcState state, void *ptr) {
        Peer *peer = (Peer *)ptr;
        peer->state = state;
        printf("State %s: %s\n", "offerer", state_print(state));
}

static void gatheringStateCallback(rtcGatheringState state, void *ptr) {
        Peer *peer = (Peer *)ptr;
        peer->gatheringState = state;
        printf("Gathering state %s: %s\n", "offerer", rtcGatheringState_print(state));
}


static void openCallback(void *ptr) {
        Peer *peer = (Peer *)ptr;
        peer->connected = true;
        char buffer[256];
        if (rtcGetDataChannelLabel(peer->dc, buffer, 256) >= 0)
                printf("DataChannel %s: Received with label \"%s\"\n","offerer", buffer);


}

static void closedCallback(void *ptr) {
        Peer *peer = (Peer *)ptr;
        peer->connected = false;


}

static void messageCallback(const char *message, int size, void *ptr) {
        // Peer *peer = (Peer *)ptr;
        if (size < 0) { // negative size indicates a null-terminated string
                printf("Message %s: %s\n", "offerer", message);
        } else {
                printf("Message %s: [binary of size %d]\n", "offerer", size);
        }
}
static void deletePeer(Peer *peer) {
        if (peer) {
                if (peer->dc)
                        rtcDeleteDataChannel(peer->dc);
                if (peer->pc)
                        rtcDeletePeerConnection(peer->pc);
                free(peer);
        }
}


int all_space(const char *str) {
        while (*str) {
                if (!isspace(*str++)) {
                        return 0;
                }
        }
        return 1;
}

char* state_print(rtcState state) {
        char *str = NULL;
        switch (state) {
        case RTC_NEW:
                str = "RTC_NEW";
                break;
        case RTC_CONNECTING:
                str = "RTC_CONNECTING";
                break;
        case RTC_CONNECTED:
                str = "RTC_CONNECTED";
                break;
        case RTC_DISCONNECTED:
                str = "RTC_DISCONNECTED";
                break;
        case RTC_FAILED:
                str = "RTC_FAILED";
                break;
        case RTC_CLOSED:
                str = "RTC_CLOSED";
                break;
        default:
                break;
        }

        return str;

}

char* rtcGatheringState_print(rtcState state) {
        char* str = NULL;
        switch (state) {
        case RTC_GATHERING_NEW:
                str = "RTC_GATHERING_NEW";
                break;
        case RTC_GATHERING_INPROGRESS:
                str = "RTC_GATHERING_INPROGRESS";
                break;
        case RTC_GATHERING_COMPLETE:
                str = "RTC_GATHERING_COMPLETE";
                break;
        default:
                break;
        }

        return str;

}
