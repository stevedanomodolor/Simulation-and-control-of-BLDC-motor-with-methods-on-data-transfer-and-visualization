/*
 *
 * @file lcsm_new.h
 * @brief LCSM example
 *
 * Life-cycle state machine implementation
 *
 * (c) Filip Reniers (KU Leuven) 2019
 * (c) Nico Huebel (KU Leuven) 2019
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#ifndef HEADER_LCSM_
#define HEADER_LCSM_

# define MAX_EVENT_QUEUE 5
#define NUM_NEW_LCSM_STATES 7
// TODO: Fix the names of different states,
// TODO: onExit(), onEnter(), doo()
typedef enum {creation, resource_configuration, capability_configuration, pausing, running, cleaning, done} LCSM_states;
typedef enum {created, configure_resources, resources_configured, configure_capabilities, capabilities_configured, start, stop, cleanup_resources, cleaned, empty} LCSM_events;
/* transition table:
 * creation -(created)-> configuring
 * creation -(cleanup)-> cleaning
 * configuring -(configured)-> active
 * active -(start)-> running
 * running -(stop)-> active
 * (NOT), active -(disable)-> creation (need to cleanup)
 * cleaning -(cleaned)-> creation
 */

static char *lcsm_state_names[] =
{
    "Creation",
    "Resource configuration",
    "Capability configuration",
    "Pausing",
    "Running",
    "Deletion",
    "Done"
};

struct LCSM {
    LCSM_states state;
    LCSM_events event_queue[MAX_EVENT_QUEUE]; //TODO: replace with event stream/ring buffer
    //sem_ID {unique id of this, unique id of type, id of metamodel}
    int no_events;
};

typedef struct LCSM LCSM_t;

int add_event(LCSM_t *sm, LCSM_events event);
void init_lcsm(LCSM_t* sm);
void update_lcsm(LCSM_t* sm);

#endif // End of HEADER_LCSM_
