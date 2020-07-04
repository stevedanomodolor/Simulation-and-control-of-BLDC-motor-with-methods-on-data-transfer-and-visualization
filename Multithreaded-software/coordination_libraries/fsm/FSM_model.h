//
// Created by filip on 04.12.19.
// DEPRECATED

#ifndef RT_CONTROL_LCSM_MODEL_H
#define RT_CONTROL_LCSM_MODEL_H

/* STATES */
typedef int state_t;

typedef struct state_list_s {
    state_t *states;
    int number_of_states;
} state_list_t;

/* TRANSITIONS */
typedef int transition_t;

typedef struct transition_list_s {
    transition_t *transitions;
    int number_of_transitions;
} transition_list_t;

/* TRANSITION TABLE */
typedef struct transition_table_entry_s {
    state_t state1;
    transition_t transition;
    state_t state2;
} transition_table_entry_t;

typedef struct transition_table_s {
    transition_table_entry_t *transition;
    int number_of_entries;
} transition_table_t;

/* EVENTS */
typedef int event_t;

typedef struct event_list_s{
    event_t *events;
    int number_of_events;
} event_list_t;

/* EVENT TRANSITION TABLE */
typedef struct event_transition_table_entry_s {
    event_t event;
    transition_t transition;
} event_transition_table_entry_t;

typedef struct event_transition_table_s {
    event_transition_table_entry_t *entry;
    int number_of_entries;
} event_transition_table_t;

typedef struct FSM_model_s {
    state_list_t states;
    transition_list_t transitions;
    event_list_t events;

    transition_table_t transition_table;
    event_transition_table_t event_transition_table;
} FSM_model_t;

typedef struct FSM_s {
    state_t state;
    event_list_t event_list;
} FSM_t;



void init_FSM(FSM_t *fsm);



void update_FSM(FSM_t *fsm, FSM_model_t *fsm_model)
{
    transition_t temp;

    for (int i =0; i < fsm->event_list.number_of_events; i++){
        for (int j = 0; j < fsm_model->event_transition_table.number_of_entries; j++) {
            if (fsm->event_list.events[i] == fsm_model->event_transition_table.entry[j].event){
                temp = fsm_model->event_transition_table.entry[j].transition;
                for
            }
        }


        for (int j = 0; j < fsm_model->states.number_of_states; j++) {
            if (fsm_model->



                    states.states[j] == fsm->state) {



                break;
            } else {
                continue;
            }

        }






    }



    for (int i=0;i<sm->no_events; i++)
    {
        switch(sm->state) {
            case inactive:
                if (sm->event_queue[i] == enable)
                {
                    // printf("LCSM: I was creation. Received: enable. Moving to: configuring.\n");
                    sm->state = configuring;
                }
                break;
                if (sm->event_queue[i] == cleanup)
                {
                    // printf("LCSM: I was creation. Received: cleanup. Moving to: cleaning.\n");
                    sm->state = cleaning;
                }
                break;
            case configuring:
                if (sm->event_queue[i] == configured)
                {
                    // printf("LCSM: I was configuring. Received: configured. Moving to: active.\n");
                    sm->state = active;
                }
                break;
            case active:
                if (sm->event_queue[i] == start)
                {
                    // printf("LCSM: I was active. Received: start. Moving to: running.\n");
                    sm->state = running;
                }
                    // else if (sm->event_queue[i] == disable)
                    // {
                    //     printf("LCSM: I was active. Received: disable. Moving to: creation.\n");
                    //     sm->state = creation;
                    // }
                else if (sm->event_queue[i] == cleanup)
                {
                    // printf("LCSM: I was active. Received: cleanup. Moving to: cleaning.\n");
                    sm->state = cleaning;
                }
                break;
            case running:
                if (sm->event_queue[i] == stop)
                {
                    // printf("LCSM: I was running. Received: stop. Moving to: active.\n");
                    sm->state = active;
                }
                break;
            case cleaning:
                if (sm->event_queue[i] == cleaned)
                {
                    // printf("LCSM: I was cleaning. Received: cleaned. Moving to: creation.\n");
                    sm->state = inactive;
                }
                break;
        }
        // Assumption: local copy of event queue -> so can delete all events after consumption; TODO: replace with ring buffer
        sm->event_queue[i] = empty;
    }
    sm->no_events = 0;
}










#endif //RT_CONTROL_LCSM_MODEL_H
