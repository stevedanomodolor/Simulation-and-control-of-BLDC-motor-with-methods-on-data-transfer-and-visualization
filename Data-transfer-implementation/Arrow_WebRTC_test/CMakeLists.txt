cmake_minimum_required(VERSION 3.13.4)
project(arrow_data_transfer_test C)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -g -O2")

include_directories(include)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include /usr/include/glib-2.0)


#offerer

add_executable(arrow_offerer src/arrow_offerer.c)
target_link_libraries(arrow_offerer  arrow-glib glib-2.0 gobject-2.0 gio-2.0 datachannel)


#answererss

add_executable(arrow_answerer src/arrow_answerer.c)
target_link_libraries(arrow_answerer  arrow-glib glib-2.0 gobject-2.0 gio-2.0 datachannel)
