cmake_minimum_required(VERSION 3.16)
project(AIOneWeb VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(AIONE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AIOne")

add_subdirectory(${AIONE_DIR})


set(ASIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/asio" CACHE STRING "" FORCE)

include_directories("external/asio/include")
add_subdirectory("external/Crow")


file(GLOB APP_SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    AIOne::Core
    Crow::Crow
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")