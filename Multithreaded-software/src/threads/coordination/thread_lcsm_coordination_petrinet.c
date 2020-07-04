/*
 * @file thread_lcsm_coordination_petrinet.c
 * @brief Implementation of petrinet for thread lcsm coordination
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */

#include "threads/coordination/thread_lcsm_coordination_petrinet.h"
#include "flags.h"

/* THREAD COORDINATION */
bool thread0_disabled = true;
bool thread0_configures = false;
bool thread0_configuration_successful = false;
bool thread0_configuration_unsuccessful = false;
bool thread0_running = false;
bool thread0_error_status = false;
bool thread0_disabling = false;
bool thread0_cleaning_completed = false;
bool start_thread0 = true;
bool stop_thread0 = false;

bool thread1_disabled = true;
bool thread1_configures = false;
bool thread1_configuration_successful = false;
bool thread1_configuration_unsuccessful = false;
bool thread1_running = false;
bool thread1_error_status = false;
bool thread1_disabling = false;
bool thread1_cleaning_completed = false;
bool start_thread1 = false;
bool stop_thread1 = false;

struct flag_map_s {
    bool *start_thread;
    bool *stop_thread;
    bool *thread_configuration_successful;
    bool *thread_configuration_unsuccessful;
    bool *thread_cleaning_completed;
    bool *thread_disabled;
    bool *thread_configures;
    bool *thread_running;
    bool *thread_error_status;
    bool *thread_disabling;
};

static struct flag_map_s flag_map_thread0 = {
        .start_thread = &start_thread0,
        .stop_thread = &stop_thread0,
        .thread_configuration_successful = &thread0_configuration_successful,
        .thread_configuration_unsuccessful = &thread0_configuration_unsuccessful,
        .thread_cleaning_completed = &thread0_cleaning_completed,
        .thread_disabled = &thread0_disabled,
        .thread_configures = &thread0_configures,
        .thread_running = &thread0_running,
        .thread_error_status = &thread0_error_status,
        .thread_disabling = &thread0_disabling,
};
static struct flag_map_s flag_map_thread1 = {
        .start_thread = &start_thread1,
        .stop_thread = &stop_thread1,
        .thread_configuration_successful = &thread1_configuration_successful,
        .thread_configuration_unsuccessful = &thread1_configuration_unsuccessful,
        .thread_cleaning_completed = &thread1_cleaning_completed,
        .thread_disabled = &thread1_disabled,
        .thread_configures = &thread1_configures,
        .thread_running = &thread1_running,
        .thread_error_status = &thread1_error_status,
        .thread_disabling = &thread1_disabling,
};

bool cond_thread_configuration_unsuccessful(transition_t *t) {
    bool b = cond_token_in_all_but_one_place(t, THREAD_CONFIGURATION_SUCCESSFUL, BLACK_TOKEN);
    return b;
}

void produce_thread_configuration_unsuccessful(transition_t *t) {
    char *array[2] = {THREAD_DISABLING, THREAD_ERROR_STATUS};
    produce_token_in_places(t, array, 2, BLACK_TOKEN);
//    produce_token_in_one_place(t,THREAD_DISABLING,BLACK_TOKEN);
}

void consume_thread_configuration_unsuccessful(transition_t *t) {
    consume_token_from_all_but_1_place(t, THREAD_CONFIGURATION_SUCCESSFUL, BLACK_TOKEN);
}



bool cond_thread_configuration_successful(transition_t *t) {
    bool b = cond_token_in_all_but_one_place(t, THREAD_CONFIGURATION_UNSUCCESSFUL, BLACK_TOKEN);
    return b;
}

void consume_thread_configuration_successful(transition_t *t) {
    consume_token_from_all_but_1_place(t, THREAD_CONFIGURATION_UNSUCCESSFUL, BLACK_TOKEN);
}

void produce_thread_configuration_successful(transition_t *t) {
    produce_token_in_one_place(t, THREAD_RUNNING, BLACK_TOKEN);
//    produce_token_in_all_but_1_place(t,THREAD_DISABLING,BLACK_TOKEN);
}

bool cond_stop_thread(transition_t *t) {
    char *array[2] = {THREAD_RUNNING, STOP_THREAD};
    return cond_token_in_places(t, array, 2, BLACK_TOKEN);
}

petrinet_t *init_thread_coordination_petrinet(char *petrinet_name) {

    petrinet_t *g = init_petrinet(petrinet_name);

    place_t *p1 = create_place(g, THREAD_CONFIGURES);
    place_t *p2 = create_place(g, THREAD_CONFIGURATION_UNSUCCESSFUL);
    place_t *p3 = create_place(g, THREAD_CONFIGURATION_SUCCESSFUL);
    place_t *p4 = create_place(g, THREAD_RUNNING);
    place_t *p5 = create_place(g, THREAD_DISABLING);
    place_t *p6 = create_place(g, THREAD_DISABLED);
    place_t *p7 = create_place(g, START_THREAD);
    place_t *p8 = create_place(g, STOP_THREAD);
    place_t *p9 = create_place(g, THREAD_CLEANING_COMPLETE);

    place_t *p14 = create_place(g, THREAD_ERROR_STATUS);

    transition_behaviour_t b1 = {
            .condition = cond_Black1,
            .consumption_behaviour=consume_Black1,
            .production_behaviour=produce_Black1
    };

    // successful configuration behaviour
    transition_behaviour_t b2 = {
            .condition = cond_thread_configuration_successful,
            .consumption_behaviour=consume_thread_configuration_successful,
            .production_behaviour=produce_thread_configuration_successful
    };

    // unsuccessful configuration behaviour
    transition_behaviour_t b3 = {
            .condition = cond_thread_configuration_unsuccessful,
            .consumption_behaviour=consume_thread_configuration_unsuccessful,
            .production_behaviour=produce_thread_configuration_unsuccessful
    };

    // stop thread behaviour
    transition_behaviour_t b4 = {
            .condition = cond_stop_thread,
            .consumption_behaviour=consume_Black1,
            .production_behaviour=produce_Black1
    };

    transition_t *t1 = create_transition(g, "t1");
    add_behaviour(t1, &b1);
    transition_t *t2 = create_transition(g, "t2");
    add_behaviour(t2, &b2);
    add_behaviour(t2, &b3);
    transition_t *t3 = create_transition(g, "t3");
    add_behaviour(t3, &b4);
    transition_t *t4 = create_transition(g, "t4");
    add_behaviour(t4, &b1);

    agedge(g, p6, t1, "1", TRUE); // DISABLED
    agedge(g, p7, t1, "2", TRUE); // START
    agedge(g, t1, p1, "1", TRUE); // CONFIGURES

    agedge(g, p1, t2, "1", TRUE); // CONFIGURES
    agedge(g, p2, t2, "2", TRUE); // CONFIGURATION NO SUCCESS
    agedge(g, p3, t2, "3", TRUE); // CONFIGURATION SUCCESS
    agedge(g, t2, p4, "1", TRUE); // RUNNING
    agedge(g, t2, p5, "2", TRUE); // DISABLING
    agedge(g, t2, p14, "3", TRUE); // ERROR STATUS

    agedge(g, p4, t3, "1", TRUE); // RUNNING
    agedge(g, p8, t3, "2", TRUE); // STOP
    agedge(g, t3, p5, "1", TRUE); // DISABLING

    agedge(g, p5, t4, "1", TRUE); // DISABLING
    agedge(g, p9, t4, "2", TRUE); // CLEANING COMPLETE
    agedge(g, t4, p6, "1", TRUE); // DISABLED

    add_tokens(p6, BLACK_TOKEN, 1);
    return g;
}

void communicate_token_flags_thread_coordination(petrinet_t *p, struct flag_map_s *map){
    /*********************
     *  FLAGS -> TOKENS  *
     *********************/

    convert_flag_to_token(p, START_THREAD, map->start_thread, BLACK_TOKEN);
    convert_flag_to_token(p, STOP_THREAD,  map->stop_thread, BLACK_TOKEN);
    convert_flag_to_token(p, THREAD_CONFIGURATION_SUCCESSFUL,  map->thread_configuration_successful, BLACK_TOKEN);
    convert_flag_to_token(p, THREAD_CONFIGURATION_UNSUCCESSFUL,  map->thread_configuration_unsuccessful, BLACK_TOKEN);
    convert_flag_to_token(p, THREAD_CLEANING_COMPLETE,  map->thread_cleaning_completed, BLACK_TOKEN);

    /*
     * EXECUTION POLICY
     */
    trigger_petrinet(p);

    /*********************
     *  TOKENS -> FLAGS  *
     *********************/

    flag_tracks_token(p, THREAD_DISABLED, BLACK_TOKEN, map->thread_disabled);
    flag_tracks_token(p, THREAD_CONFIGURES, BLACK_TOKEN,  map->thread_configures);
    flag_tracks_token(p, THREAD_RUNNING, BLACK_TOKEN,  map->thread_running);
    flag_tracks_token(p, THREAD_ERROR_STATUS, BLACK_TOKEN,  map->thread_error_status);
    flag_tracks_token(p, THREAD_DISABLING, BLACK_TOKEN,  map->thread_disabling);
}

void communicate_token_flags_thread0_coordination(petrinet_t *p) {
    communicate_token_flags_thread_coordination(p, &flag_map_thread0);
}

void communicate_token_flags_thread1_coordination(petrinet_t *p) {
    communicate_token_flags_thread_coordination(p, &flag_map_thread1);
}

