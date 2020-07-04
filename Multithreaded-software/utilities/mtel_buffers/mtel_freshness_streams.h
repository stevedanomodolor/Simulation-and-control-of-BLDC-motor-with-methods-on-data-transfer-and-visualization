
/**

 * @file mtel_freshness_streams.h
 * @brief Freshness Stream mechanism for mtel
 *
 * This file describes the structures for
 * using the maximum freshness stream mechanism in mtel (wrapper interface)
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#ifndef MTEL_FRESHNESS_STREAMS_H_
#define MTEL_FRESHNESS_STREAMS_H_

#ifndef MAX_STREAM_CONNECTOR_NAME
#  define MAX_STREAM_CONNECTOR_NAME 50
#endif

#include <freshness_stream.h>

struct mtel_freshness_connector
{
    struct freshness_stream_connector connector;
    char name[MAX_STREAM_CONNECTOR_NAME];
};

struct mtel_freshness_connectors_list
{
    struct mtel_freshness_connector **streams;
    int num_streams;
};

struct mtel_freshness_connector *get_connector_by_name(struct mtel_freshness_connectors_list *cl, char *name);

#endif // MTEL_FRESHNESS_STREAMS_H_
