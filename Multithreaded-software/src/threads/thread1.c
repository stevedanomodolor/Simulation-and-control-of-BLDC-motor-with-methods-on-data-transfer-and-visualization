/*
 * @file thread1.c
 * @brief Implementation of lcsm functions of thread1
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 20.02.20
 *
 */

#include "threads/thread1.h"

#include "logger_messages.h"

#include "flags.h"
#include "threads/coordination/thread_lcsm_coordination_petrinet.h"

void communicate_thread1_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void communicate_thread1_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

/* COORDINATE */

void coordinate_thread1_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread1_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread1_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread1_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread1_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);

    if (thread1_running) {

        ev_state->activity_scheduler->coordinate(ev_state->activity_scheduler);

    }
}

void coordinate_thread1_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread1_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread1_coordination(ev_state->petrinet_thread_coordination);
}

/* CONFIGURE */

void configure_thread1_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void configure_thread1_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void configure_thread1_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void configure_thread1_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (thread1_running) {
        ev->flags.cycle_time_enabled = true;
    }
}

void configure_thread1_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (thread1_running) {
        ev_state->activity_scheduler->configure(ev_state->activity_scheduler);
    } else if (thread1_disabling) {
        ev->flags.cycle_time_enabled = false;
    }
}

void configure_thread1_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

void configure_thread1_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

/* COMPUTE */

void compute_thread1_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (thread1_configures) {
//        if (ev_state->logfile) {
//            log_msg_t temp_msg;
//            snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//            logger(Info, ev_state->logfile, temp_msg);
//            fflush(ev_state->logfile);
//        }

        ev_state->Error = false;

        activity_scheduler_params_state_t *activity_scheduler_state = (activity_scheduler_params_state_t *)ev_state->activity_scheduler->state;
        activity_scheduler_state->petrinet = activity_scheduler_state->init_petrinet("thread1 activity scheduler coordination");

        add_event(&ev->lcsm, created);
    }
}

void compute_thread1_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    if (ev_state->logfile) {
//        log_msg_t temp_msg;
//        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//        logger(Info, ev_state->logfile, temp_msg);
//    }

    if (thread1_disabling)
        add_event(&ev->lcsm, cleanup_resources);
    else if (thread1_configures)
        add_event(&ev->lcsm, resources_configured);
}

void compute_thread1_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    if (ev_state->logfile) {
//        log_msg_t temp_msg;
//        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//        logger(Info, ev_state->logfile, temp_msg);
//    }

    if (thread1_disabling)
        add_event(&ev->lcsm, configure_resources);
    else if (thread1_configures) {
        add_event(&ev->lcsm, capabilities_configured);
        if (!ev_state->Error){
            thread1_configuration_successful = true;
        } else {
            thread1_configuration_unsuccessful = true;
        }

    }
}

void compute_thread1_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    if (ev_state->logfile) {
//        log_msg_t temp_msg;
//        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//        logger(Info, ev_state->logfile, temp_msg);
//        fprint_marking(ev_state->petrinet_thread_coordination, ev_state->logfile);
//        fflush(ev_state->logfile);
//    }

    if (thread1_disabling)
        add_event(&ev->lcsm, configure_capabilities);
    else if (thread1_running) {
        add_event(&ev->lcsm, start);
    }
}

void compute_thread1_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    if (ev_state->logfile) {
//        log_msg_t temp_msg;
//        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//        logger(Info, ev_state->logfile, temp_msg);
//        fprint_marking(ev_state->petrinet_thread_coordination, ev_state->logfile);
//        fflush(ev_state->logfile);
//    }

    if (thread1_running) {

        ev_state->activity_scheduler->compute(ev_state->activity_scheduler);

    } else if (thread1_disabling) {
        add_event(&ev->lcsm, stop);
    }
//    if (mediator_waiting_for_shutdown_threads) {
//        fprint_marking(ev_state->petrinet_thread_coordination, ev_state->logfile);
//        fflush(ev_state->logfile);
//    }
}

void compute_thread1_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (thread1_disabling) {
//        if (ev_state->logfile) {
//            log_msg_t temp_msg;
//            snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 1: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//            logger(Info, ev_state->logfile, temp_msg);
//        }
        activity_scheduler_params_state_t *activity_scheduler_state = (activity_scheduler_params_state_t *)ev_state->activity_scheduler->state;
        destroy_petrinet(activity_scheduler_state->petrinet);

        add_event(&ev->lcsm, cleaned);
        thread1_cleaning_completed = true;
    }
}

void compute_thread1_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (thread1_disabled)
        ev->flags.run_eventloop = false;
}
