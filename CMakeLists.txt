cmake_minimum_required(VERSION 3.16)
project(AIOneWeb VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_WIN32_EXECUTABLE OFF)
# if(MSVC)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
# endif()

set(AIONE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AIOne")

# add_subdirectory(${AIONE_DIR})


# set(ASIO_INCLUDE_DIR OFF CACHE BOOL "" FORCE)
set(ASIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/asio" CACHE STRING "" FORCE)

# add_subdirectory("external/Crow")


file(GLOB APP_SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# target_link_libraries(${PROJECT_NAME} PRIVATE
#     AIOne::Core
#     Crow::Crow
# )