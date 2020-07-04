
cmake_minimum_required(VERSION 2.8.3)
project(rt_control)

# If you read this, you are more curious than most of your fellow sutdents :-)
# For more information on CMAKE read this tutorial (or any other tutorial): http://derekmolloy.ie/hello-world-introductions-to-cmake

set(MOTOR_SIMULATION_CONTROL_NAME main_program)
set(RT_CONTROL_NAME rt_control)
set(CMAKE_C_FLAGS "-g")


if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(EXTERNAL_LOCATION ${PROJECT_SOURCE_DIR}/external)
set(INCLUDE_GENERATED_LOCATION ${PROJECT_SOURCE_DIR}/include)

set(GIT_CLONE_COMMAND git clone)

function(gitclone addr destination)
    if (EXISTS ${EXTERNAL_LOCATION}/${destination})
        message(STATUS "External repo exists already: ${addr}")
    else ()
        message(STATUS "Launching Git to retrieve: ${addr}")
        execute_process(COMMAND ${GIT_CLONE_COMMAND} ${addr} ${destination}
                WORKING_DIRECTORY ${EXTERNAL_LOCATION}
                )
        # message("Git Launched")
    endif ()
endfunction(gitclone)

# Get parson library
#gitclone(https://github.com/kgabis/parson.git parson)
#
#include_directories(external/parson)

# Variables with paths to include directies
set(INCLUDE_DIRS_SOEM
        external/soem
        external/soem/soem
        external/soem/osal
        external/soem/osal/linux
        external/soem/oshw/linux
        )

set(INCLUDE_DIRS_DISRUPTOR
        external/disruptor
        external/disruptor/liblfds7.1.1/liblfds711/inc
        )

add_subdirectory(external/soem)
target_compile_options(soem
        PUBLIC -fPIC
        )

# UTILITY LIBRARIES
add_library(logger_messages
        utilities/logger/logger_messages.c
        )

target_include_directories(logger_messages PUBLIC
        utilities/logger
        ${INCLUDE_DIRS_SOEM})

target_link_libraries(logger_messages soem)

add_library(time_helper
        utilities/time/time_helper.c
        )

target_include_directories(time_helper PUBLIC
        ${PROJECT_SOURCE_DIR}/utilities/time)

# GRAPHVIZ + PETRINETS
find_path(GRAPHVIZ_INCLUDE_DIR NAMES graphviz/cgraph.h)
find_library(GRAPHVIZ_CDT_LIBRARY NAMES cdt)
find_library(GRAPHVIZ_CGRAPH_LIBRARY NAMES cgraph)
if (GRAPHVIZ_INCLUDE_DIR AND GRAPHVIZ_CDT_LIBRARY AND GRAPHVIZ_CGRAPH_LIBRARY)
    set(GRAPHVIZ_FOUND TRUE)
    message("Graphviz found")
else ()
    set(GRAPHVIZ_FOUND FALSE)
    message("Graphviz not found")
endif ()
set(GRAPHVIZ_INCLUDE_DIRS ${GRAPHVIZ_INCLUDE_DIR} ${GRAPHVIZ_INCLUDE_DIR}/graphviz)


message("")
message("Include directories and libraries")

message("")

add_library(petrinet
        coordination_libraries/petrinet/petrinet.c
        )

target_include_directories(petrinet PUBLIC
        ${PROJECT_SOURCE_DIR}/coordination_libraries/petrinet
        ${GRAPHVIZ_INCLUDE_DIRS})

# LCSM
add_library(lcsm
        coordination_libraries/lcsm/lcsm_new.c
        )

target_include_directories(lcsm PUBLIC
        ${PROJECT_SOURCE_DIR}/coordination_libraries/lcsm)


# DEPLOYMENT ARCHITECTURE MODEL LIBRARY

add_library(deployment_architecture_model STATIC
        deployment_architecture_model/src/activities.c
        deployment_architecture_model/src/activity_scheduler.c
        deployment_architecture_model/src/threads.c
        deployment_architecture_model/src/algorithm.c)

target_include_directories(deployment_architecture_model PUBLIC
        ${PROJECT_SOURCE_DIR}/deployment_architecture_model/include

        ${PROJECT_SOURCE_DIR}/coordination_libraries/lcsm
        ${PROJECT_SOURCE_DIR}/coordination_libraries/petrinet
        ${PROJECT_SOURCE_DIR}/utilities/logger
        ${PROJECT_SOURCE_DIR}/utilities/time
        )

target_link_libraries(deployment_architecture_model
        # Utilities
        logger_messages
        time_helper

        # Coordination libs
        lcsm
        petrinet
        )





set(EXAMPLE_NAME motor_simulation)
add_executable(${EXAMPLE_NAME}
        src/main_program.c
        src/threads/thread0.c
        src/threads/thread1.c
        src/threads/coordination/thread_lcsm_coordination_petrinet.c
        src/activity_schedulers/activity_scheduler1.c
        src/activity_schedulers/coordination/thread_activity_scheduler_coordination_petrinet.c
        src/activities/application_mediator.c
        src/activities/motor_simulation_control.c
        src/activities/command_line.c
        src/activities/coordination/application_mediator_petrinet.c
        src/algorithms/motor_simulation_control_functions.c
        src/algorithms/FOC.c
        src/algorithms/global_variables.c
        src/algorithms/model.c
        src/algorithms/motor.c
        src/algorithms/openloop.c
        src/algorithms/PID.c

        )
target_include_directories(${EXAMPLE_NAME} PUBLIC
        include/algorithms
        include
        ${PROJECT_SOURCE_DIR}/deployment_architecture_model/include

        ${PROJECT_SOURCE_DIR}/coordination_libraries/lcsm
        ${PROJECT_SOURCE_DIR}/coordination_libraries/petrinet
        ${PROJECT_SOURCE_DIR}/utilities/logger
        ${PROJECT_SOURCE_DIR}/utilities/time
        )

target_link_libraries(${EXAMPLE_NAME}
        deployment_architecture_model
        logger_messages
        time_helper
        petrinet
        ${GRAPHVIZ_CDT_LIBRARY}
        ${GRAPHVIZ_CGRAPH_LIBRARY}
        pthread
        m
        )

set (directory ${CMAKE_CURRENT_BINARY_DIR}/ramsub)
file(MAKE_DIRECTORY ${directory})
