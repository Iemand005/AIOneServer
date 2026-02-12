cmake_minimum_required(VERSION 3.16)
project(AIOneWeb VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)


set(AIONE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AIOne")

add_subdirectory(${AIONE_DIR})



add_subdirectory("external/drogon" drogon)