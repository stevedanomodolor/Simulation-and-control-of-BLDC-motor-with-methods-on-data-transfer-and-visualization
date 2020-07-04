/*
 *
 * @file petrinet.h
 * @brief Header of petrinet implementation
 *
 * This file is the header of the coloured petrinet implementation. It contains all internal petrinet structures and
 * coordination specific place and token names.
 *
 * (c) Filip Reniers (KU Leuven) 23.12.19
 *
 */

#ifndef RT_CONTROL_PETRINET_H
#define RT_CONTROL_PETRINET_H

#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <cgraph.h>
#include <stdio.h>

#define MAX_LENGTH_TOKEN_TYPE_NAME 32

/* GENERIC TOKEN TYPE */

#define BLACK_TOKEN "black"

/* PETRINET STRUCTURE */

typedef Agnode_t place_t;
typedef Agnode_t transition_t;
typedef Agraph_t petrinet_t;

typedef
struct count_of_tokens_s {
    Agrec_t rec;
    int count;
    char token_type[MAX_LENGTH_TOKEN_TYPE_NAME];
} count_of_token_t;

typedef
struct transition_behaviour_s {
    bool (*condition)(transition_t *);
    void (*consumption_behaviour)(transition_t *);
    void (*production_behaviour)(transition_t *);
} transition_behaviour_t;

typedef struct transition_attribute_s {
    Agrec_t rec;
    int number_of_behaviours;
    transition_behaviour_t *behaviours;
} transition_attribute_t;

/* PETRINET CREATION FUNCTIONS */
petrinet_t *init_petrinet(char *petrinet_name);
void destroy_petrinet(petrinet_t *p);

place_t *create_place(petrinet_t *p, char *name);
transition_t *create_transition(petrinet_t *p, char *name);
void add_behaviour(transition_t *t1, transition_behaviour_t *b1);
void delete_behaviour(transition_t *t1);

int create_token_type(place_t *p, char *typename, int n);
void delete_token_type(place_t *p, char *typename);
void add_tokens(place_t *p, char *typename, int n);
int delete_tokens(place_t *p, char *typename, int n);

/* PETRINET TRIGGER FUNCTION */
void trigger_petrinet(petrinet_t *p);

/* PETRINET MARKING: print function */
void fprint_marking(petrinet_t *p, FILE *pipe);
void print_marking(petrinet_t *p);
int write_petrinet(petrinet_t *p);

/* PETRINET QUERY FUNCTIONS */
place_t *get_place(petrinet_t *p, char *id);
transition_t *get_transition(petrinet_t *p, char *id);
int get_number_of_tokens(place_t *p, char *typename);
bool check_number_of_tokens(place_t *p, char *typename, int n);
bool has_any_tokens(place_t *p);


/* BEHAVIOUR CONDITION, CONSUMPTION, PRODUCTION TEMPLATE FUNCTIONS */

void consume_nothing(transition_t *t);
void produce_nothing(transition_t *t);

bool cond_token_type_in_all_incoming_places(transition_t *t, char *token_type);
void consume_token_in_all_incoming_places(transition_t *t, char *token_name);
void produce_token_in_all_outgoing_places(transition_t *t, char *token_name);

bool cond_token_in_all_but_one_place(transition_t *t, char *place_name, char *token_type);
void consume_token_from_all_but_1_place(transition_t *t, char *place_name, char *token_name);
void produce_token_in_all_but_1_place(transition_t *t, char *place_name, char *token_name);

bool cond_token_in_one_place(transition_t *t, char *place_name, char *token_name);
void consume_token_from_one_place(transition_t *t, char *place_name, char *token_name);
void produce_token_in_one_place(transition_t *t, char *place_name, char *token_name);

bool cond_token_in_places(transition_t *t, char *place_name_list[], int number_of_places, char *token_name);
void consume_token_from_places(transition_t *t, char **place_name_list, int number_of_places, char *token_name);
void produce_token_in_places(transition_t *t, char **place_name_list, int number_of_places, char *token_name);

bool cond_Black1(transition_t *t);
void consume_Black1(transition_t *t);
void produce_Black1(transition_t *t);

bool cond_check_number_of_different_token_types(transition_t *t, char *black_token_place, char *place_collecting_different_types, int number_of_different_types);

/* TOKEN FLAG CONSISTENCY */

void cvt_flag2token(petrinet_t *p, place_t *n, bool *flag, char *token_type);
void convert_flag_to_token(petrinet_t *p, char * place_name, bool *flag, char* token_type);
void convert_flags_to_tokens(petrinet_t *p, char * place_name, bool *flag[], char *token_type[], int number_of_flags);

void cvt_token2flag(petrinet_t *p, place_t *n, char *token_type, bool *flag);
void convert_token_to_flag(petrinet_t *p, char *place_name, char *token_type, bool *flag);
void convert_tokens_to_flags(petrinet_t *p, char * place_name, char *token_type[], bool *flag[], int number_of_tokens);

void flag_tracks_token(petrinet_t *p, char *place_name, char *token_type, bool *flag);
void flags_track_tokens(petrinet_t *p, char * place_name, char *token_type[], bool *flag[], int number_of_tokens);

void token_tracks_flag(petrinet_t *p, char *place_name, bool *flag, char *token_type);
void tokens_track_flags(petrinet_t *p, char * place_name, bool *flag[], char *token_type[], int number_of_flags);


#endif //RT_CONTROL_PETRINET_H
