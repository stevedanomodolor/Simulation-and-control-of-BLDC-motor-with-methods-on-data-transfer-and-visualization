/*
 * @file thread1.h
 * @brief header of thread1
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 20.02.20
 *
 */
#ifndef RT_CONTROL_THREAD1_H
#define RT_CONTROL_THREAD1_H

#include "threads.h"

void communicate_thread1_create(void *eventloop);

void communicate_thread1_resource_conf(void *eventloop);

void communicate_thread1_capability_conf(void *eventloop);

void communicate_thread1_pausing(void *eventloop);

void communicate_thread1_running(void *eventloop);

void communicate_thread1_cleaning(void *eventloop);

void communicate_thread1_done(void *eventloop);

void coordinate_thread1_create(void *eventloop);

void coordinate_thread1_resource_conf(void *eventloop);

void coordinate_thread1_capability_conf(void *eventloop);

void coordinate_thread1_pausing(void *eventloop);

void coordinate_thread1_running(void *eventloop);

void coordinate_thread1_cleaning(void *eventloop);

void coordinate_thread1_done(void *eventloop);

void configure_thread1_create(void *eventloop);

void configure_thread1_resource_conf(void *eventloop);

void configure_thread1_capability_conf(void *eventloop);

void configure_thread1_pausing(void *eventloop);

void configure_thread1_running(void *eventloop);

void configure_thread1_cleaning(void *eventloop);

void configure_thread1_done(void *eventloop);

void compute_thread1_create(void *eventloop);

void compute_thread1_resource_conf(void *eventloop);

void compute_thread1_capability_conf(void *eventloop);

void compute_thread1_pausing(void *eventloop);

void compute_thread1_running(void *eventloop);

void compute_thread1_cleaning(void *eventloop);

void compute_thread1_done(void *eventloop);
#endif //RT_CONTROL_THREAD1_H
