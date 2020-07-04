/*
 * @file schedule.h
 * @brief 
 *
 * 
 *
 * (c) Filip Reniers (KU Leuven) 02.03.20
 *
 */
#ifndef RT_CONTROL_SCHEDULE_H
#define RT_CONTROL_SCHEDULE_H

#define MAX_NUMBER_OF_ENTRIES 10

typedef
struct symbolic_schedule_s {
    int ID;
    int number_of_entries;
    char *entries[MAX_NUMBER_OF_ENTRIES];
} symbolic_schedule_t;

typedef
struct symbolic_schedules_s {
    int number_of_schedules;
    symbolic_schedule_t *schedules;
} symbolic_schedules_t;



#endif //RT_CONTROL_SCHEDULE_H
