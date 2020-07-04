
/**

 * @file mtel_activities.c
 * @brief Actions to handle mtel_activities configurations
 *
 * This file contains a wrappper implementation for
 * using the maximum freshness stream mechanism in mtel (wrapper interface)
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#include "mtel_freshness_streams.h"
#include <string.h>

struct mtel_freshness_connector *get_connector_by_name(struct mtel_freshness_connectors_list *cl, char *name)
{
    if (!cl)
    {
        return NULL;
    }
    if (!name)
    {
        return NULL;
    }
    for (int ii=0; ii<cl->num_streams; ii++)
    {
        if ( 0 == strcmp(cl->streams[ii]->name, name))
        {
            return cl->streams[ii];
        }
    }
    return NULL;
}
