/*
 * @file algorithm.h
 * @brief Header of algorithm scheduler eventloop
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 02.03.20
 *
 */
#ifndef RT_CONTROL_ALGORITHM_H
#define RT_CONTROL_ALGORITHM_H

#include "symbolic_schedule.h"
#include "petrinet.h"

#define MAX_NUMBER_OF_ALGORITHMS MAX_NUMBER_OF_ENTRIES
#define MAX_LENGTH_ALGORITHM_NAME 16

typedef void (*algorithm_t)(void *state);

typedef
struct algorithm_conf_s {
    char name[MAX_LENGTH_ALGORITHM_NAME];
    void *state;
    algorithm_t algorithm;
} algorithm_conf_t;

typedef
struct algorithm_schedule_s {
    algorithm_conf_t *alg_conf[MAX_NUMBER_OF_ALGORITHMS];
    int number_of_algorithms;
    int current_symbolic_schedule_ID;
} algorithm_schedule_t;

typedef struct algorithm_scheduler_params_s algorithm_scheduler_params_t;

struct algorithm_scheduler_params_s {
    void (*coordinate)(algorithm_scheduler_params_t *conf);
    void (*configure)(algorithm_scheduler_params_t *conf);
    void (*compute)(algorithm_scheduler_params_t *conf);

    algorithm_conf_t *algorithms_array;
    int number_of_algorithms;

    symbolic_schedule_t *symbolic_schedules;
    int number_of_symbolic_schedules;

    algorithm_schedule_t *algorithm_schedule;
    void *state;
};

typedef
struct algorithm_scheduler_params_state_s {
    petrinet_t *petrinet;
    petrinet_t *(*init_petrinet)(char *name);
    void (*communicate_token_flags)(petrinet_t *p);
} algorithm_scheduler_params_state_t;

algorithm_conf_t *lookup_algorithm(algorithm_scheduler_params_t *scheduler, char *algorithm_name);
void set_algorithm_schedule(algorithm_scheduler_params_t *conf, int ID);

void coordinate_algorithm_with_petrinet(algorithm_scheduler_params_t *conf);
void compute_execute_algorithm_schedule(algorithm_scheduler_params_t *conf);

void *do_algorithm_eventloop(algorithm_scheduler_params_t *alg);


#endif //RT_CONTROL_ALGORITHM_H
