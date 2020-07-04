
/**

 * @file mtel_buffers.h
 * @brief Buffers used in mtel
 *
 * This file describes the structures for
 * using buffers in mtel (wrapper interface)
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#ifndef MTEL_BUFFERS_H_
#define MTEL_BUFFERS_H_

#ifndef MAX_NAME_BUFFER
#  define MAX_NAME_BUFFER 60
#endif // MAX_NAME_BUFFER

#ifndef MAX_BUFFER_PORT_NAME
#  define MAX_BUFFER_PORT_NAME 60
#endif // MAX_BUFFER_PORT_NAME

#ifndef MAX_BUFFER_CONNECTOR_NAME
#  define MAX_BUFFER_CONNECTOR_NAME 60
#endif // MAX_BUFFER_CONNECTOR_NAME

// #include "ctask.h"
#include <ringbuffer_pipe.h>

// struct mtel_buffer
// {
//     char name_id[MAX_NAME_BUFFER];
//     char *doc;
//     int id;
//     char producer_name[MAX_NAME_CTASK];
//     char consumer_name[MAX_NAME_CTASK];
//
//     void *buffer_ptr;
// };
//
//
// struct mtel_buffers_list
// {
//     struct mtel_buffer **buffers;
//     int num_buffers;
// };

struct mtel_ctask_port {
    struct ringbuffer_pipe_port port;
    char name[MAX_BUFFER_PORT_NAME];
};

struct mtel_connector
{
    struct ringbuffer_pipe_connector connector;
    char name[MAX_BUFFER_CONNECTOR_NAME];
};

struct mtel_connectors_array
{
    struct mtel_connector *buffers;
    int num_buffers;
};

struct mtel_connectors_list
{
    struct mtel_connector **buffers;
    int num_buffers;
};

struct mtel_rinbuffer_pipe_port_list
{
    struct mtel_ctask_port **ports;
    int num_ports;
};

#endif // MTEL_BUFFERS_H_
