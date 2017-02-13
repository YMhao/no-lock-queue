cmake_minimum_required(VERSION 2.6)
project(cas-no-lock)

set(CMAKE_CXX_FLAG "-g -Wall")

AUX_SOURCE_DIRECTORY(./ SOURCES)

add_executable(cas-no-lock ${SOURCES})

TARGET_LINK_LIBRARIES(cas-no-lock pthread)
