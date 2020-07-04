cmake_minimum_required(VERSION 3.5.1)
project(ArrowCGlibTest C)

include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include /usr/include/glib-2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -g -O2")

add_executable(ArrowCGlibTest arrow_test.c)
target_link_libraries(ArrowCGlibTest  arrow-glib glib-2.0 gobject-2.0 gio-2.0 )
