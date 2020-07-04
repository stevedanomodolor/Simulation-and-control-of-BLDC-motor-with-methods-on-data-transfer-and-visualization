/*
 * @file thread0.h
 * @brief header of main thread (thread0)
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */
#ifndef RT_CONTROL_THREAD0_H
#define RT_CONTROL_THREAD0_H

#include "threads.h"

void communicate_thread0_create(void *eventloop);

void communicate_thread0_resource_conf(void *eventloop);

void communicate_thread0_capability_conf(void *eventloop);

void communicate_thread0_pausing(void *eventloop);

void communicate_thread0_running(void *eventloop);

void communicate_thread0_cleaning(void *eventloop);

void communicate_thread0_done(void *eventloop);

void coordinate_thread0_create(void *eventloop);

void coordinate_thread0_resource_conf(void *eventloop);

void coordinate_thread0_capability_conf(void *eventloop);

void coordinate_thread0_pausing(void *eventloop);

void coordinate_thread0_running(void *eventloop);

void coordinate_thread0_cleaning(void *eventloop);

void coordinate_thread0_done(void *eventloop);

void configure_thread0_create(void *eventloop);

void configure_thread0_resource_conf(void *eventloop);

void configure_thread0_capability_conf(void *eventloop);

void configure_thread0_pausing(void *eventloop);

void configure_thread0_running(void *eventloop);

void configure_thread0_cleaning(void *eventloop);

void configure_thread0_done(void *eventloop);

void compute_thread0_create(void *eventloop);

void compute_thread0_resource_conf(void *eventloop);

void compute_thread0_capability_conf(void *eventloop);

void compute_thread0_pausing(void *eventloop);

void compute_thread0_running(void *eventloop);

void compute_thread0_cleaning(void *eventloop);

void compute_thread0_done(void *eventloop);

#endif //RT_CONTROL_THREAD0_H
