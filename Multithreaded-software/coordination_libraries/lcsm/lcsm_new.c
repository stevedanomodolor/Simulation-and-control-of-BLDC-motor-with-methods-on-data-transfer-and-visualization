
/**

 * @file lcsm_new.c
 * @brief LCSM example
 *
 * Life-cycle state machine implementation
 *
 * (c) Nico Huebel (KU Leuven) 2019
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 * (c) Filip Reniers (KU Leuven) 2019
 *
 */


#include "lcsm_new.h"
// #include <stdio.h>

//could be replaced with constructure or default initialization above in C++
void init_lcsm(struct LCSM* sm)
{
    sm->state = creation;
    for (int i = 0; i<MAX_EVENT_QUEUE; i++)
    {
        sm->event_queue[i] = empty;
    }
    sm->no_events = 0;
}


void update_lcsm(struct LCSM *sm)
{
    for (int i=0;i<sm->no_events; i++)
    {
        switch(sm->state) {
            case creation:
                if (sm->event_queue[i] == created)
                {
                    // printf("LCSM: I was creation. Received: created. Moving to: configuring.\n");
                    sm->state = resource_configuration;
                }
                break;
            case resource_configuration:
                if (sm->event_queue[i] == resources_configured)
                {
                    sm->state = capability_configuration;
                }
                else if (sm->event_queue[i] == cleanup_resources)
                {
                    sm->state = cleaning;
                }
                break;
            case capability_configuration:
                if (sm->event_queue[i] == capabilities_configured)
                {
                    sm->state = pausing;
                }
                else if (sm->event_queue[i] == configure_resources)
                {
                    sm->state = resource_configuration;
                }
                break;
            case pausing:
                if (sm->event_queue[i] == start)
                {
                    sm->state = running;
                }
                else if (sm->event_queue[i] == configure_capabilities)
                {
                    sm->state = capability_configuration;
                }
                break;
            case running:
                if (sm->event_queue[i] == stop)
                {
                    sm->state = pausing;
                }
                break;
            case cleaning:
                if (sm->event_queue[i] == cleaned)
                {
                    sm->state = done;
                }
                break;
            case done:
                break;
        }
        // Assumption: local copy of event queue -> so can delete all events after consumption; TODO: replace with ring buffer
        sm->event_queue[i] = empty;
    }
    sm->no_events = 0;
}

int add_event(LCSM_t *sm, LCSM_events event) {
    int ret = -1;
    if (sm->no_events < MAX_EVENT_QUEUE)
    {
        sm->event_queue[sm->no_events] = event; // since setting an int is atomic, this should be thread safe
        sm->no_events++;
        ret = 0;
    }
    return ret;
}
