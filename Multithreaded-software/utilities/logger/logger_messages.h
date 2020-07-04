
/*
 *
 * @file logger_messages.h
 * @brief Logs out error/warning/... messages
 *
 * This file contain the facilities to log messages
 * to standard output and files
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#ifndef HEADER_LOGGER_MESSAGES_H
#define HEADER_LOGGER_MESSAGES_H

#include <stdio.h>

#ifndef MAX_LOG_MESSAGE_LEN
#  define MAX_LOG_MESSAGE_LEN 700
#endif // MAX_LOG_MESSAGE_LEN

enum Verbosity_Levels {
    Always,
    Error,
    Warning,
    Info,
    Dbg
};

// Verb log() cannot be used
void logger(int verbosity_level, FILE* out_fd, const char *err_msg);

// Set/Get pair for log level
void set_log_level(int log_level);
int get_log_level(void);


void log_always(FILE *out_fd, const char *msg);
void log_error(FILE *out_fd, const char *err_msg);
void log_warning(FILE *out_fd, const char *war_msg);
void log_info(FILE *out_fd, const char *info_msg);
void log_dbg(FILE *out_fd, const char *dbg_msg);

typedef char log_msg_t[MAX_LOG_MESSAGE_LEN];

#endif // End of HEADER_LOGGER_MESSAGES_H
