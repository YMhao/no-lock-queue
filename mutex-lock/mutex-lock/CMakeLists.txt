cmake_minimum_required(VERSION 2.6)
project(mutex-lock)

set(CMAKE_CXX_FLAG "-g -Wall")

AUX_SOURCE_DIRECTORY(./ SOURCES)

add_executable(mutex-lock ${SOURCES})

TARGET_LINK_LIBRARIES(mutex-lock pthread)
