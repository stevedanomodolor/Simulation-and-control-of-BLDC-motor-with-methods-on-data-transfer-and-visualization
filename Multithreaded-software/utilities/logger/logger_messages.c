
/**

 * @file logger_messages.c
 * @brief Logs out error/warning/... messages
 *
 * This file contain the facilities to log messages
 * to standard output and files
 *
 * (c) Sergio Portoles Diez (KU Leuven) 2019
 *
 */

#include "logger_messages.h"

#include <osal.h>

#ifndef MAX_LOG_MESSAGE_LEN
#  define MAX_LOG_MESSAGE_LEN 700
#endif


// // Solution only for Linux
// #include <time.h>
// #include <sys/time.h>
// // For windows check: https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows

int _log_level(int new_log_level)
{
    // Default log level
    static int curr_log_level = Info;
    if (new_log_level < 0)
    {
        return curr_log_level;
    } else {
        curr_log_level = new_log_level;
        return curr_log_level;
    }
}

void set_log_level(int new_log_level)
{
    _log_level(new_log_level);
}

int get_log_level(void)
{
    return _log_level(-1);
}

// Verb log() cannot be used
void logger(int verbosity_level, FILE* out_fd, const char *err_msg)
{
    ec_timet curr_time = osal_current_time();
    char total_msgs[MAX_LOG_MESSAGE_LEN];
    char *verb_description[] = {"Always", "Error", "Warning", "Info", "Debug"};
    // Length secure message generation
    snprintf(total_msgs, MAX_LOG_MESSAGE_LEN, "%d.%d [%s]: %s\n", curr_time.sec, curr_time.usec, verb_description[verbosity_level], err_msg);

    // Only for debbug
    // printf("Log level is set at: %s (%d)\n", verb_description[get_log_level()], get_log_level());
    // printf("Log level received : %s (%d)\n", verb_description[verbosity_level], verbosity_level);

    if (Error == verbosity_level)
    {
        fputs(total_msgs, stderr);
    } else if ( get_log_level() >= verbosity_level ) {
        fputs(total_msgs, stdout);
    }
    if (NULL != out_fd)
    {
        fputs(total_msgs, out_fd);
        fflush(out_fd);
    }
    return;
}

void log_always(FILE *out_fd, const char *msg) {return logger(Always, out_fd, msg);}
void log_error(FILE *out_fd, const char *err_msg) {return logger(Error, out_fd, err_msg);}
void log_warning(FILE *out_fd, const char *war_msg) {return logger(Warning, out_fd, war_msg);}
void log_info(FILE *out_fd, const char *info_msg) {return logger(Info, out_fd, info_msg);}
void log_dbg(FILE *out_fd, const char *dbg_msg) {return logger(Dbg, out_fd, dbg_msg);}
