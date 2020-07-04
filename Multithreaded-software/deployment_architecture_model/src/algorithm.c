/*
 * @file algorithm.c
 * @brief Implementation of algorithm scheduler eventloop
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 02.03.20
 *
 */
#include "algorithm.h"
#include <string.h>
#include <stdio.h>

algorithm_conf_t *lookup_algorithm(algorithm_scheduler_params_t *scheduler, char *algorithm_name) {
    for (int i = 0; i < scheduler->number_of_algorithms; i++) {
        if (!strcmp(algorithm_name, scheduler->algorithms_array[i].name)) {
            return &scheduler->algorithms_array[i];
        }
    }
    printf("LOOKUP FAILED: algorithm does not exist: %s\n", algorithm_name);
    return NULL;
}

void set_algorithm_schedule(algorithm_scheduler_params_t *conf, int ID) {
    conf->algorithm_schedule->current_symbolic_schedule_ID = ID;
    conf->algorithm_schedule->number_of_algorithms = conf->symbolic_schedules[ID].number_of_entries;
    for (int i = 0; i < conf->symbolic_schedules[ID].number_of_entries; i++) {
        conf->algorithm_schedule->alg_conf[i] = lookup_algorithm(conf,conf->symbolic_schedules[ID].entries[i]);
    }
}

void coordinate_algorithm_with_petrinet(algorithm_scheduler_params_t *conf) {
    algorithm_scheduler_params_state_t *state = (algorithm_scheduler_params_state_t *)conf->state;
    state->communicate_token_flags(state->petrinet);
}

void compute_execute_algorithm_schedule(algorithm_scheduler_params_t *conf) {
    for (int i = 0; i < conf->algorithm_schedule->number_of_algorithms; i++){
        conf->algorithm_schedule->alg_conf[i]->algorithm(conf->algorithm_schedule->alg_conf[i]->state);
    }
}

void *do_algorithm_eventloop(algorithm_scheduler_params_t *alg) {
    alg->coordinate(alg);
    alg->configure(alg);
    alg->compute(alg);
}