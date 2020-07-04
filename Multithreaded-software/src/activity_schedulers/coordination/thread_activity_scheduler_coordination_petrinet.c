/*
 * @file activity_scheduler_thread0_coordinatrion_petrinet.c
 * @brief
 *
 *
 *
 * (c) Filip Reniers (KU Leuven) 03.03.20
 *
 */
#include "activity_schedulers/coordination/thread_activity_scheduler_coordination_petrinet.h"
#include "flags.h"

#include <stdbool.h>

bool thread0_schedule1 = true;
bool thread0_schedule2 = false;
bool thread1_schedule1 = true;
bool thread1_schedule2 = false;

typedef
struct flag_map_activity_scheduler_s {
    bool *select_schedule1;
//    bool *select_schedule2;
    bool *thread_schedule1;
    bool *thread_schedule2;
} flag_map_activity_scheduler_t;

static flag_map_activity_scheduler_t flag_map_thread0_activity_scheduler = {
        .select_schedule1 = &disabled_command_line_activity,
        .thread_schedule1 = &thread0_schedule1,
        .thread_schedule2 = &thread0_schedule2
};

static flag_map_activity_scheduler_t flag_map_thread1_activity_scheduler = {
        .select_schedule1 = &disabled_motor_simulation_control_activity,
        .thread_schedule1 = &thread1_schedule1,
        .thread_schedule2 = &thread1_schedule2
};

petrinet_t *init_petrinet_activity_scheduler_on_off(char *petrinet_name) {

    petrinet_t *g = init_petrinet(petrinet_name);

    place_t *p1 = create_place(g, THREAD_SCHEDULE1);
    place_t *p2 = create_place(g, THREAD_SELECT_SCHEDULE2);
    place_t *p3 = create_place(g, THREAD_SCHEDULE2);
    place_t *p4 = create_place(g, THREAD_SELECT_SCHEDULE1);

    transition_behaviour_t b1 = {
            .condition = cond_Black1,
            .consumption_behaviour=consume_Black1,
            .production_behaviour=produce_Black1
    };

    transition_t *t5 = create_transition(g, "t5");
    add_behaviour(t5, &b1);
    transition_t *t6 = create_transition(g, "t6");
    add_behaviour(t6, &b1);

    agedge(g, p1, t5, "1", TRUE); // SCHEDULE1
    agedge(g, p2, t5, "2", TRUE); // ACTIVITY ENABLED
    agedge(g, t5, p3, "1", TRUE); // SCHEDULE2

    agedge(g, p3, t6, "1", TRUE); // SCHEDULE2
    agedge(g, p4, t6, "2", TRUE); // ACTIVITY DISABLED
    agedge(g, t6, p1, "1", TRUE); // SCHEDULE1

    add_tokens(p1, BLACK_TOKEN, 1);
    return g;
}

void communicate_token_flags_activity_scheduler_on_off(petrinet_t *p, flag_map_activity_scheduler_t *map){
        /*********************
         *  FLAGS -> TOKENS  *
         *********************/
        bool select_schedule2 = !(*map->select_schedule1);

        token_tracks_flag(p, THREAD_SELECT_SCHEDULE1, map->select_schedule1, BLACK_TOKEN);
        token_tracks_flag(p, THREAD_SELECT_SCHEDULE2, &select_schedule2, BLACK_TOKEN);

        /*
         * EXECUTION POLICY
         */
        trigger_petrinet(p);

        /*********************
         *  TOKENS -> FLAGS  *
         *********************/

        flag_tracks_token(p, THREAD_SCHEDULE1, BLACK_TOKEN,  map->thread_schedule1);
        flag_tracks_token(p, THREAD_SCHEDULE2, BLACK_TOKEN,  map->thread_schedule2);

}

void communicate_token_flags_activity_scheduler_thread0(petrinet_t *p){
    communicate_token_flags_activity_scheduler_on_off(p, &flag_map_thread0_activity_scheduler);
}

void communicate_token_flags_activity_scheduler_thread1(petrinet_t *p){
    communicate_token_flags_activity_scheduler_on_off(p, &flag_map_thread1_activity_scheduler);
}
