
cmake_minimum_required(VERSION 3.13.4)

project (BLDC_simulation)



option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

include_directories(include)

file(GLOB SOURCES src/*.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/javascript")
add_executable(a ${SOURCES})

if(JS_ONLY)
	message(STATUS "Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=0")
else(JS_ONLY)
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=1")
endif(JS_ONLY)
