/*
 * @file application_mediator_petrinet.c
 * @brief Implementation of petrinet for the coordination of the whole application
 *
 *
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */

#include "activities/coordination/application_mediator_petrinet.h"
#include "threads/coordination/thread_lcsm_coordination_petrinet.h"
#include "flags.h"

#include "activities/application_mediator.h"

/* APPLICATION MEDIATOR COORDINATION STATUS */
bool stop_application = false;
bool mediator_configuration_successful = false;
bool mediator_configuration_unsuccessful = false;

bool mediator_configures = true;
bool mediator_monitors = false;
bool mediator_waiting_for_start_threads = false;
bool mediator_waiting_for_shutdown_activities = false;
bool mediator_waiting_for_shutdown_threads = false;
bool mediator_shutdown = false;

/* ACTIVITY COORDINATION */
bool disabled_command_line_activity = true;
bool enabled_command_line_activity = false;
bool disabling_command_line_activity = false;
bool cleaning_completed_command_line_activity = false;

bool disabled_motor_simulation_control_activity = true;
bool enabled_motor_simulation_control_activity = false;
bool disabling_motor_simulation_control_activity = false;
bool cleaning_completed_motor_simulation_control_activity = false;

static char *thread_token_type_array[NUM_OF_OTHER_THREADS] = {THREAD1_TOKEN};

static char *activity_token_type_array[NUMBER_OF_OTHER_ACTIVITIES] = {MOTOR_SIMULATION_CONTROL_TOKEN,
                                                                      COMMANDLINE_TOKEN};

static bool *thread_running_flags[NUM_OF_OTHER_THREADS] = {&thread1_running};
static bool *thread_error_status_flags[NUM_OF_OTHER_THREADS] = {&thread1_error_status};
static bool *thread_disabled_flags[NUM_OF_OTHER_THREADS] = {&thread1_disabled};
static bool *start_thread_flags[NUM_OF_OTHER_THREADS] = {&start_thread1};
static bool *stop_thread_flags[NUM_OF_OTHER_THREADS] = {&stop_thread1};
static bool *activity_enabled_flags[NUMBER_OF_OTHER_ACTIVITIES] = {&enabled_motor_simulation_control_activity,
                                                                   &enabled_command_line_activity};
static bool *activity_disabling_flags[NUMBER_OF_OTHER_ACTIVITIES] = {&disabling_motor_simulation_control_activity,
                                                                     &disabling_command_line_activity};
static bool *activity_disabled_flags[NUMBER_OF_OTHER_ACTIVITIES] = {&disabled_motor_simulation_control_activity,
                                                                    &disabled_command_line_activity};
static bool *activity_cleaned_flags[NUMBER_OF_OTHER_ACTIVITIES] = {&cleaning_completed_motor_simulation_control_activity,
                                                                   &cleaning_completed_command_line_activity};

void produce_thread_tokens(transition_t *t, char *place_name_ev_tokens, char *place_name_black_token) {
    // produces thread tokens in "place_name_ev_tokens" and
    // produces black token in "place_name_black_token"

    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        if (!strcmp(agnameof(n), place_name_ev_tokens)) {
            add_tokens(n, THREAD1_TOKEN, 1);
        } else if (!strcmp(agnameof(n), place_name_black_token)) {
            add_tokens(n, BLACK_TOKEN, 1);
        }
    }
}

void produce_activity_tokens_in_place(transition_t *t, char *place_name_act_tokens, char *place_name_black_token) {
    // produces activity tokens in "place_name_act_tokens" and
    // produces black token in "place_name_black_token"
    for (Agedge_t *e = agfstout(agraphof(t), t); e; e = agnxtout(agraphof(t), e)) {
        Agnode_t *n = aghead(e);
        if (!strcmp(agnameof(n), place_name_act_tokens)) {
            add_tokens(n, MOTOR_SIMULATION_CONTROL_TOKEN, 1);
            add_tokens(n, COMMANDLINE_TOKEN, 1);
        } else if (!strcmp(agnameof(n), place_name_black_token)){
            add_tokens(n, BLACK_TOKEN, 1);
        }
    }
}

bool cond_activity_tokens_in_incoming_place(transition_t *t) {
    // check whether all incoming places have a black pixel
    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);
        if (!strcmp(agnameof(e), "1")) {
            if (!check_number_of_tokens(n, BLACK_TOKEN, 1))
                return false;
        } else if (!strcmp(agnameof(e), "2")) {
            if (!check_number_of_tokens(n, MOTOR_SIMULATION_CONTROL_TOKEN, 1))
                return false;
            if (!check_number_of_tokens(n, COMMANDLINE_TOKEN, 1))
                return false;
        }
    }
    return true;
}

bool cond_mediator_configuration_successful(transition_t *t) {

    bool b = cond_token_in_all_but_one_place(t, MEDIATOR_CONFIGURATION_UNSUCCESSFUL, BLACK_TOKEN);
//    if (b)
//        printf ("\n TRIGGERED\n");
    return b;
}

void consume_mediator_configuration_successful(transition_t *t) {
    consume_token_from_all_but_1_place(t, MEDIATOR_CONFIGURATION_UNSUCCESSFUL, BLACK_TOKEN);
}

void produce_resource_configuration_successful(transition_t *t) {
    produce_thread_tokens(t, MEDIATOR_START_THREADS, MEDIATOR_WAITING_FOR_START_THREADS);
}

bool cond_mediator_configuration_unsuccessful(transition_t *t) {

    bool b = cond_token_in_all_but_one_place(t, MEDIATOR_CONFIGURATION_SUCCESSFUL, BLACK_TOKEN);
//    if (b)
//        printf ("\n TRIGGERED\n");
    return b;
}

void consume_mediator_configuration_unsuccessful(transition_t *t) {
    consume_token_from_all_but_1_place(t, MEDIATOR_CONFIGURATION_SUCCESSFUL, BLACK_TOKEN);
}

void produce_mediator_configuration_unsuccessful(transition_t *t) {
    produce_token_in_one_place(t, MEDIATOR_SHUTDOWN, BLACK_TOKEN);
}

void produce_activity_tokens_stop_activity(transition_t *t) {
    produce_activity_tokens_in_place(t, STOP_ACTIVITY,MEDIATOR_WAITING_FOR_SHUTDOWN_ACTIVITIES);
}

void consume_activities_shutdown(transition_t *t){
    consume_token_from_one_place(t,MEDIATOR_WAITING_FOR_SHUTDOWN_ACTIVITIES, BLACK_TOKEN);
}

void produce_activities_shutdown(transition_t *t) {
    produce_thread_tokens(t, MEDIATOR_STOP_THREADS, MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS);
}

bool cond_motor_Simulation_Control1(transition_t *t) {
    return cond_token_type_in_all_incoming_places(t, MOTOR_SIMULATION_CONTROL_TOKEN);
}

void consume_motor_Simulation_Control1(transition_t *t) {
    consume_token_in_all_incoming_places(t, MOTOR_SIMULATION_CONTROL_TOKEN);
}

void produce_motor_Simulation_Control1(transition_t *t) {
    produce_token_in_all_outgoing_places(t, MOTOR_SIMULATION_CONTROL_TOKEN);
}

bool cond_Commandline1(transition_t *t) {
    return cond_token_type_in_all_incoming_places(t, COMMANDLINE_TOKEN);
}

void consume_Commandline1(transition_t *t) {
    consume_token_in_all_incoming_places(t, COMMANDLINE_TOKEN);
}

void produce_Commandline1(transition_t *t) {
    produce_token_in_all_outgoing_places(t, COMMANDLINE_TOKEN);
}

bool cond_starting_threads_successful(transition_t *t) {
    return cond_check_number_of_different_token_types(
            t,
            MEDIATOR_WAITING_FOR_START_THREADS,
            MEDIATOR_THREADS_RUNNING,
            NUM_OF_OTHER_THREADS
    );
}

void consume_starting_threads(transition_t *t) {
    consume_token_from_one_place(t, MEDIATOR_WAITING_FOR_START_THREADS, BLACK_TOKEN);
}

void produce_starting_threads_successful(transition_t *t) {

    for (Agedge_t *e_out = agfstout(agraphof(t), t); e_out; e_out = agnxtout(agraphof(t), e_out)) {
        Agnode_t *n_out = aghead(e_out);
        if (!strcmp(agnameof(n_out), MEDIATOR_MONITORING)){
            add_tokens(n_out,BLACK_TOKEN,1);
        } else if (!strcmp(agnameof(n_out), START_ACTIVITY)){

            // TODO COMMENT / OUTCOMMENT TO TOGGLE ACTIVITIES

            add_tokens(n_out,MOTOR_SIMULATION_CONTROL_TOKEN,1);
            add_tokens(n_out,COMMANDLINE_TOKEN,1);
        }
    }

}

bool cond_starting_threads_unsuccessful(transition_t *t) {
    int number_of_running_threads = 0;
    int number_of_thread_errors = 0;

    for (Agedge_t *e = agfstin(agraphof(t), t); e; e = agnxtin(agraphof(t), e)) {
        Agnode_t *n = agtail(e);

        if (!strcmp(agnameof(n), MEDIATOR_WAITING_FOR_START_THREADS)) {
            if (!check_number_of_tokens(n, BLACK_TOKEN, 1))
                return false;
        } else if (!strcmp(agnameof(n), MEDIATOR_THREADS_RUNNING)) {
            Agrec_t *i = n->base.data->next;
            char *number_of_token_types_str = agget(n, "number_of_token_types");
            number_of_running_threads = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
        } else {
            Agrec_t *i = n->base.data->next;
            char *number_of_token_types_str = agget(n, "number_of_token_types");
            number_of_thread_errors = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
        }
    }
    if (number_of_running_threads + number_of_thread_errors == NUM_OF_OTHER_THREADS && number_of_thread_errors > 0){
        printf("\n ERROR IN STARTING THREADS\n");
        return true;
    }

    else
        return false;
}

void produce_starting_threads_unsuccessful(transition_t *t) {



    for (Agedge_t *e_out = agfstout(agraphof(t), t); e_out; e_out = agnxtout(agraphof(t), e_out)) {
        Agnode_t *n_out = aghead(e_out);
        if (!strcmp(agnameof(n_out), MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS))
            add_tokens(n_out,BLACK_TOKEN,1);

        else if (!strcmp(agnameof(n_out), MEDIATOR_STOP_THREADS)){
            for (Agedge_t *e_in = agfstin(agraphof(t), t); e_in; e_in = agnxtin(agraphof(t), e_in)) {
                Agnode_t *n_in = agtail(e_in);

                if (!strcmp(agnameof(n_in), MEDIATOR_THREADS_RUNNING)) {
                    Agrec_t *i = n_in->base.data->next;
                    char *number_of_token_types_str = agget(n_in, "number_of_token_types");
                    int number_of_token_types = (number_of_token_types_str ? atoi(number_of_token_types_str) : 0);
                    for (int count = 0; count < number_of_token_types; count++) {
                        add_tokens(n_out, ((count_of_token_t *) i)->token_type, 1);
                        i = i->next;
                    }
                }
            }
        }
    }

}

bool cond_shutdown_threads_complete(transition_t *t) {
    bool b = cond_check_number_of_different_token_types(
                t,
                MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS,
                MEDIATOR_THREADS_DISABLED,
                NUM_OF_OTHER_THREADS
        );


    return b;
}

void consume_shutdown_threads_complete(transition_t *t){
    consume_token_from_one_place(t, MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS, BLACK_TOKEN);
}

void consume_motor_Simulation_Control0(transition_t *t) {
    consume_token_from_one_place(t, STOP_ACTIVITY, MOTOR_SIMULATION_CONTROL_TOKEN);
}

void consume_Commandline0(transition_t *t) {
    consume_token_from_one_place(t, STOP_ACTIVITY, COMMANDLINE_TOKEN);
}

petrinet_t *init_application_mediator_coordination_petrinet(char *petrinet_name) {
    petrinet_t *g = init_petrinet(petrinet_name);

    place_t *p1 = create_place(g, MEDIATOR_CONFIGURES);
    place_t *p2 = create_place(g, MEDIATOR_CONFIGURATION_UNSUCCESSFUL);
    place_t *p3 = create_place(g, MEDIATOR_CONFIGURATION_SUCCESSFUL);
    place_t *p4 = create_place(g, MEDIATOR_MONITORING);
    place_t *p5 = create_place(g, MEDIATOR_WAITING_FOR_SHUTDOWN_ACTIVITIES);
    place_t *p6 = create_place(g, MEDIATOR_SHUTDOWN);
    place_t *p7 = create_place(g, STOP_APPLICATION);

    place_t *p8 = create_place(g, ACTIVITY_DISABLED);
    place_t *p9 = create_place(g, ACTIVITY_ENABLED);
    place_t *p10 = create_place(g, START_ACTIVITY);
    place_t *p11 = create_place(g, STOP_ACTIVITY);
    place_t *p12 = create_place(g, ACTIVITY_DISABLING);
    place_t *p13 = create_place(g, ACTIVITY_CLEANING_COMPLETED);

    place_t *p14 = create_place(g, MEDIATOR_START_THREADS);
    place_t *p15 = create_place(g, MEDIATOR_STOP_THREADS);
    place_t *p16 = create_place(g, MEDIATOR_THREADS_RUNNING);
    place_t *p17 = create_place(g, MEDIATOR_THREADS_ERROR_STATUS);
    place_t *p18 = create_place(g, MEDIATOR_WAITING_FOR_START_THREADS);
    place_t *p19 = create_place(g, MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS);
    place_t *p20 = create_place(g, MEDIATOR_THREADS_DISABLED);


    transition_behaviour_t b1 = {
            .condition = cond_mediator_configuration_successful,
            .consumption_behaviour=consume_mediator_configuration_successful,
            .production_behaviour=produce_resource_configuration_successful
    };

    transition_behaviour_t b2 = {
            .condition = cond_mediator_configuration_unsuccessful,
            .consumption_behaviour=consume_mediator_configuration_unsuccessful,
            .production_behaviour=produce_mediator_configuration_unsuccessful
    };

    transition_behaviour_t b3 = {
            .condition = cond_Black1,
            .consumption_behaviour=consume_Black1,
            .production_behaviour=produce_activity_tokens_stop_activity
    };

    transition_behaviour_t b4 = {
            .condition = cond_activity_tokens_in_incoming_place,
            .consumption_behaviour=consume_activities_shutdown,
            .production_behaviour=produce_activities_shutdown
    };

    transition_behaviour_t b5 = {
            .condition = cond_motor_Simulation_Control1,
            .consumption_behaviour=consume_motor_Simulation_Control1,
            .production_behaviour=produce_motor_Simulation_Control1
    };

    transition_behaviour_t b6 = {
            .condition = cond_Commandline1,
            .consumption_behaviour=consume_Commandline1,
            .production_behaviour=produce_Commandline1
    };

    transition_behaviour_t b10 = {
            .condition = cond_starting_threads_successful,
            .consumption_behaviour=consume_starting_threads,
            .production_behaviour=produce_starting_threads_successful
    };

    transition_behaviour_t b11 = {
            .condition = cond_starting_threads_unsuccessful,
            .consumption_behaviour=consume_starting_threads,
            .production_behaviour=produce_starting_threads_unsuccessful
    };

    transition_behaviour_t b12 = {
            .condition = cond_shutdown_threads_complete,
            .consumption_behaviour=consume_shutdown_threads_complete,
            .production_behaviour=produce_Black1
    };

//    transition_behaviour_t b13 = {
//            .condition = cond_motor_Simulation_Control1,
//            .consumption_behaviour=consume_motor_Simulation_Control0,
//            .production_behaviour=produce_nothing
//    };
//
//    transition_behaviour_t b14 = {
//            .condition = cond_Commandline1,
//            .consumption_behaviour=consume_Commandline0,
//            .production_behaviour=produce_nothing
//    };
//


    // the mediator is fully configured or has had an error
    transition_t *t1 = create_transition(g, "t1");
    add_behaviour(t1, &b1); // successful configuration: mediator will start other threads
    add_behaviour(t1, &b2); // unsuccessful configuration: shutdown application
    // the mediator is monitoring
    transition_t *t2 = create_transition(g, "t2");
    add_behaviour(t2, &b3); // application shutdown -> shut down all other activities
    // waiting for activities to be disabled
    transition_t *t3 = create_transition(g, "t3");
    add_behaviour(t3, &b4); // all activities are disabled -> the mediator shuts down the threads
    // for every activity: start activity -> activity traverses lcsm and will run its nominal behaviour
    transition_t *t4 = create_transition(g,"t4");
    add_behaviour(t4, &b5);
    add_behaviour(t4, &b6);
    // for every activity: stop activity -> activity will disable itself
    transition_t *t5 = create_transition(g, "t5");
    add_behaviour(t5, &b5);
    add_behaviour(t5, &b6);
    // for every activity: completely cleaned -> disabled
    transition_t *t6 = create_transition(g, "t6");
    add_behaviour(t6, &b5);
    add_behaviour(t6, &b6);
    // waiting for threads to start
    transition_t *t7 = create_transition(g,"t7");
    add_behaviour(t7, &b10); // successful -> start activities
    add_behaviour(t7, &b11); // unsuccessful -> stop running threads and shutdown
    // waiting for threads to shutdown
    transition_t *t8 = create_transition(g, "t8");
    add_behaviour(t8, &b12); // all threads disabled -> shutdown application

    agedge(g, p1, t1, "1", TRUE);  // CONFIGURE
    agedge(g, p2, t1, "2", TRUE);  // CONFIGURATION NO SUCCESS
    agedge(g, p3, t1, "3", TRUE);  // CONFIGURATION SUCCESS
    agedge(g, t1, p14, "1", TRUE); // START THREADS
    agedge(g, t1, p18, "2", TRUE); // WAITING FOR STARTUP THREADS
    agedge(g, t1, p6, "3", TRUE);  // SHUTDOWN

    agedge(g, p18, t7, "1", TRUE); // WAITING FOR START THREADS
    agedge(g, p16, t7, "2", TRUE); // THREADS RUNNING
    agedge(g, p17, t7, "3", TRUE); // THREADS ERROR
    agedge(g, t7, p15, "1", TRUE); // STOP THREADS
    agedge(g, t7, p19, "2", TRUE); // WAITING FOR SHUTDOWN THREADS
    agedge(g, t7, p4, "3", TRUE);  // MEDIATOR MONITORING
    agedge(g, t7, p10, "4", TRUE); // START ACTIVITY

    agedge(g, p4, t2, "1", TRUE);  // MEDIATOR MONITORING
    agedge(g, p7, t2, "2", TRUE);  // STOP APPLICATION
    agedge(g, t2, p11, "1", TRUE); // STOP ACTIVITY
    agedge(g, t2, p5, "2", TRUE);  // WAITING FOR SHUTDOWN ACTIVITIES

    agedge(g, p5, t3, "1", TRUE);  // WAITING FOR SHUTDOWN ACTIVITIES
    agedge(g, p8, t3, "2", TRUE);  // ACTIVITY DISABLED
    agedge(g, t3, p15, "1", TRUE); // STOP THREADS
    agedge(g, t3, p19, "2", TRUE); // WAITING FOR SHUTDOWN THREADS

    agedge(g, p19, t8, "1", TRUE); // WAITING FOR SHUTDOWN THREADS
    agedge(g, p20, t8, "2", TRUE); // THREADS DISABLED
    agedge(g, t8, p6, "1", TRUE); // SHUTDOWN

    agedge(g, p8, t4, "1", TRUE);  // ACTIVITY DISABLED
    agedge(g, p10, t4, "2", TRUE); // START ACTIVITY
    agedge(g, t4, p9, "1", TRUE);  // ACTIVITY ENABLED

    agedge(g, p9, t5, "1", TRUE);  // ACTIVITY ENABLED
    agedge(g, p11, t5, "2", TRUE); // STOP ACTIVITY
    agedge(g, t5, p12, "1", TRUE); // ACTIVITY DISABLING

    agedge(g, p12, t6, "1", TRUE); // ACTIVITY DISABLING
    agedge(g, p13, t6, "2", TRUE); // ACTIVITY CLEANING COMPLETE
    agedge(g, t6, p8, "1", TRUE);  // ACTIVITY DISABLED

    add_tokens(get_place(g, MEDIATOR_CONFIGURES), BLACK_TOKEN, 1);

    add_tokens(get_place(g, ACTIVITY_DISABLED), MOTOR_SIMULATION_CONTROL_TOKEN, 1);
    add_tokens(get_place(g, ACTIVITY_DISABLED), COMMANDLINE_TOKEN, 1);

    return g;
}

void communicate_token_flags_application_mediator(petrinet_t *p) {

    /*********************
     *  FLAGS -> TOKENS  *
     *********************/

    /* MEDIATOR */
    convert_flag_to_token(p, MEDIATOR_CONFIGURATION_SUCCESSFUL, &mediator_configuration_successful, BLACK_TOKEN);
    convert_flag_to_token(p, MEDIATOR_CONFIGURATION_UNSUCCESSFUL, &mediator_configuration_unsuccessful, BLACK_TOKEN);
    convert_flag_to_token(p, STOP_APPLICATION, &stop_application, BLACK_TOKEN);

    /* THREADS */

    tokens_track_flags(p, MEDIATOR_THREADS_RUNNING, thread_running_flags, thread_token_type_array, NUM_OF_OTHER_THREADS);
    tokens_track_flags(p, MEDIATOR_THREADS_ERROR_STATUS, thread_error_status_flags, thread_token_type_array, NUM_OF_OTHER_THREADS);
    tokens_track_flags(p, MEDIATOR_THREADS_DISABLED, thread_disabled_flags, thread_token_type_array, NUM_OF_OTHER_THREADS);

    /* ACTIVITIES */

    convert_flags_to_tokens(p, ACTIVITY_CLEANING_COMPLETED, activity_cleaned_flags, activity_token_type_array, NUMBER_OF_OTHER_ACTIVITIES);

    /*
     * EXECUTION POLICY
     */
    trigger_petrinet(p);

    /*********************
     *  TOKENS -> FLAGS  *
     *********************/

    /* MEDIATOR */
    flag_tracks_token(p, MEDIATOR_CONFIGURES, BLACK_TOKEN, &mediator_configures);
    flag_tracks_token(p, MEDIATOR_SHUTDOWN, BLACK_TOKEN, &mediator_shutdown);
    flag_tracks_token(p, MEDIATOR_MONITORING, BLACK_TOKEN, &mediator_monitors);

    /* THREADS */
//    flag_tracks_token(p, MEDIATOR_WAITING_FOR_START_THREADS,BLACK_TOKEN,&mediator_waiting_for_start_threads);
    flag_tracks_token(p, MEDIATOR_WAITING_FOR_SHUTDOWN_THREADS,BLACK_TOKEN,&mediator_waiting_for_shutdown_threads);
    convert_tokens_to_flags(p,MEDIATOR_START_THREADS, thread_token_type_array, start_thread_flags, NUM_OF_OTHER_THREADS);
    convert_tokens_to_flags(p,MEDIATOR_STOP_THREADS, thread_token_type_array, stop_thread_flags, NUM_OF_OTHER_THREADS);

    /* ACTIVITIES */
    flags_track_tokens(p, ACTIVITY_ENABLED,activity_token_type_array,activity_enabled_flags,NUMBER_OF_OTHER_ACTIVITIES);
    flags_track_tokens(p, ACTIVITY_DISABLING,activity_token_type_array,activity_disabling_flags,NUMBER_OF_OTHER_ACTIVITIES);
    flags_track_tokens(p, ACTIVITY_DISABLED,activity_token_type_array,activity_disabled_flags,NUMBER_OF_OTHER_ACTIVITIES);
}
