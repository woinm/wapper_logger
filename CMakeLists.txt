cmake_minimum_required(VERSION 2.8)

project(wrapper_logger)

set(CMAKE_CXX_STANDARD 11)

option(debug "build debug version" ON)

set(CMAKE_C_FLAGS "-fPIC -Wall -pthread")
set(CMAKE_CXX_FLAGS "-fPIC -Wall -pthread")

if (debug)
    set(CMAKE_CXX_FLAGS "-fPIC -Wall -pthread -g3 -ggdb")
endif()

message(STATUS "${PROJECT_NAME} cmake_cxx_flags: ${CMAKE_CXX_FLAGS}")

include_directories(.)
include_directories(example)
include_directories(/usr/include)
include_directories(/usr/local/include)

link_directories(/usr/lib)
link_directories(/usr/lib64)
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} "example/main.cpp")

set(WRAPPER_LOGGER_DEPENDS log4cxx)

target_link_libraries(${PROJECT_NAME} ${WRAPPER_LOGGER_DEPENDS})
