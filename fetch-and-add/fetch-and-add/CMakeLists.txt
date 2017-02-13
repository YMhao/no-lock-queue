cmake_minimum_required(VERSION 2.6)
project(fetch-and-add)

set(CMAKE_CXX_FLAG "-g -Wall")

AUX_SOURCE_DIRECTORY(./ SOURCES)

add_executable(fetch-and-add ${SOURCES})

TARGET_LINK_LIBRARIES(fetch-and-add pthread)
