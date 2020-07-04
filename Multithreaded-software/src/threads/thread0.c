/*
 * @file thread0.c
 * @brief Implementation of main thread (thread0)
 *
 *
 *
 * (c) Filip Reniers (KU Leuven) 21.02.20
 *
 */

#include "threads/thread0.h"

#include "logger_messages.h"

#include "activities/application_mediator.h"

#include "flags.h"
#include "threads/coordination/thread_lcsm_coordination_petrinet.h"

void communicate_thread0_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void communicate_thread0_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void communicate_thread0_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void communicate_thread0_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void communicate_thread0_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void communicate_thread0_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}
void communicate_thread0_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

/* COORDINATE */
// seeing thish state in petrinet we are
void coordinate_thread0_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void coordinate_thread0_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread0_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread0_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread0_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread0_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread0_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread0_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread0_coordination(ev_state->petrinet_thread_coordination);

    if (thread0_running) {

        ev_state->activity_scheduler->coordinate(ev_state->activity_scheduler);

    }
}

void coordinate_thread0_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    communicate_token_flags_thread0_coordination(ev_state->petrinet_thread_coordination);
}

void coordinate_thread0_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;
}

/* CONFIGURE */

void configure_thread0_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void configure_thread0_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void configure_thread0_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void configure_thread0_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;

    if (thread0_running) {
        ev->flags.cycle_time_enabled = true;
    }
}

void configure_thread0_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;

    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    print_marking(ev_state->petrinet_thread_coordination);

    if (thread0_running) {

        ev_state->activity_scheduler->configure(ev_state->activity_scheduler);

    } else if (thread0_disabling) {
        ev->flags.cycle_time_enabled = false;
    }

}

void configure_thread0_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

void configure_thread0_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
}

/* COMPUTE */

void compute_thread0_create(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;

    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    ev_state->Error = false;

    char log_file_path[MAX_LENGTH_PATH_NAME];
    log_msg_t temp_msg;
    // Open logfile -> file descriptor
    snprintf(log_file_path, MAX_LENGTH_PATH_NAME, "%s/%s", PATH_LOGFILES, THREAD0_LOG_FILE_NAME);
    FILE *logfile_thread0 = fopen(log_file_path, "w");
    if (logfile_thread0 == NULL) {
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Could not open %s", log_file_path);
        fprintf(stdout, "%s\n", temp_msg);
        ev_state->Error = true;
    } else {
        ev_state->logfile = logfile_thread0;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Succesfully opened logfile: %s", log_file_path);
        logger(Info, logfile_thread0, temp_msg);
        logger(Info, logfile_thread0, "Logging started");
    }
    if (ev_state->logfile) {
        log_msg_t temp_msg;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
        logger(Info, ev_state->logfile, temp_msg);
    }

    ev_state->petrinet_thread_coordination = init_thread_coordination_petrinet("main thread coordination");

    activity_scheduler_params_state_t *activity_scheduler_state = (activity_scheduler_params_state_t *)ev_state->activity_scheduler->state;
    activity_scheduler_state->petrinet = activity_scheduler_state->init_petrinet("thread0 activity scheduler coordination");

    add_event(&ev->lcsm, created);

}

void compute_thread0_resource_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (ev_state->logfile) {
        log_msg_t temp_msg;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
        logger(Info, ev_state->logfile, temp_msg);
    }

    if (thread0_disabling)
        add_event(&ev->lcsm, cleanup_resources);
    else if (thread0_configures)
        add_event(&ev->lcsm, resources_configured);
}

void compute_thread0_capability_conf(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (ev_state->logfile) {
        log_msg_t temp_msg;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
        logger(Info, ev_state->logfile, temp_msg);
    }

    if (thread0_disabling)
        add_event(&ev->lcsm, configure_resources);
    else if (thread0_configures) {
        add_event(&ev->lcsm, capabilities_configured);
        if (!ev_state->Error)
            thread0_configuration_successful = true;
        else
            thread0_configuration_unsuccessful = true;

    }
}

void compute_thread0_pausing(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (ev_state->logfile) {
        log_msg_t temp_msg;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
        logger(Info, ev_state->logfile, temp_msg);
    }

    if (thread0_disabling)
        add_event(&ev->lcsm, configure_capabilities);
    else if (thread0_running) {
        add_event(&ev->lcsm, start);
    }
//    if (mediator_waiting_for_shutdown_threads) {
//        fprint_marking(ev_state->petrinet_thread_coordination, ev_state->logfile);
//        fflush(ev_state->logfile);
//    }

}

void compute_thread0_running(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

//    if (ev_state->logfile){
//        log_msg_t temp_msg;
//        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
//        logger(Info, ev_state->logfile, temp_msg);
////        fprint_marking(ev_state->petrinet_thread_coordination,ev_state->logfile);
//        fflush(ev_state->logfile);
//    }

    if (thread0_running) {

        ev_state->activity_scheduler->compute(ev_state->activity_scheduler);

    } else if (thread0_disabling) {
        add_event(&ev->lcsm, stop);
    }
//    if (mediator_waiting_for_shutdown_threads) {
//        fprint_marking(ev_state->petrinet_thread_coordination, ev_state->logfile);
//        fflush(ev_state->logfile);
//    }
}

void compute_thread0_cleaning(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    if (ev_state->logfile) {
        log_msg_t temp_msg;
        snprintf(temp_msg, MAX_LOG_MESSAGE_LEN, "Thread 0: lcsm state: %s", lcsm_state_names[ev->lcsm.state]);
        logger(Info, ev_state->logfile, temp_msg);
    }

    if (ev_state->logfile) {
        logger(Info, ev_state->logfile, "Shutting down logger");
        fflush(ev_state->logfile);
        fclose(ev_state->logfile);
        ev_state->logfile = NULL;
    }

    activity_scheduler_params_state_t *activity_scheduler_state = (activity_scheduler_params_state_t *)ev_state->activity_scheduler->state;
    destroy_petrinet(activity_scheduler_state->petrinet);

    add_event(&ev->lcsm, cleaned);
    thread0_cleaning_completed = true;
}

void compute_thread0_done(void *eventloop) {
    time_triggered_thread_params_t *ev = (time_triggered_thread_params_t *) eventloop;
    eventloop_state_t *ev_state = (eventloop_state_t *) ev->state;

    destroy_petrinet(ev_state->petrinet_thread_coordination);
    ev->flags.run_eventloop = false;
}

//void compute_thread0_resource_conf(time_triggered_thread_params_t *eventloop) {
//    if (thread0_disabling)
//        add_event(&eventloop->lcsm,cleanup_resources);
//    else if (thread0_configures)
//        add_event(&eventloop->lcsm,resources_configured);
//}
//
//void compute_thread0_capability_conf(time_triggered_thread_params_t *eventloop) {
//    if (thread0_disabling)
//        add_event(&eventloop->lcsm,configure_resources);
//    else if (thread0_configures){
//        add_event(&eventloop->lcsm,capabilities_configured);
//        thread0_configuration_successful = true;
//        eventloop_application_mediator_end_of_configuration = true;
//    }
//}
//
//void compute_thread0_pausing(time_triggered_thread_params_t *eventloop) {
//    if (thread0_disabling)
//        add_event(&eventloop->lcsm,configure_capabilities);
//    else if (thread0_running)
//        add_event(&eventloop->lcsm,start);
//
//}
//
//void compute_thread0_running(time_triggered_thread_params_t *eventloop) {
//    if (thread0_running) {
//
//    } else if (thread0_disabling) {
//        add_event(&eventloop->lcsm,stop);
//    }
//}
//
//void compute_thread0_cleaning(time_triggered_thread_params_t *eventloop) {
//
//    add_event(&eventloop->lcsm,cleaned);
//    thread0_cleaning_completed = true;
//}
