cmake_minimum_required(VERSION 3.20)
project(AIOneServer VERSION 1.0.0 LANGUAGES C CXX)

# Generate version file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/Version.hpp"
)


# For metaproject
set(GIT_EXECUTABLE "GIT_EXECUTABLE-NOTFOUND" CACHE FILEPATH "Disable Git tracking in dependencies" FORCE)
if(NOT TARGET AIOneCore)
    add_subdirectory("../AIOne" "${CMAKE_CURRENT_BINARY_DIR}/AIOne")
endif()

# Dependencies
add_subdirectory("external/cpp-httplib" "${CMAKE_CURRENT_BINARY_DIR}/external_cpp-httplib")
add_subdirectory("external/json" "${CMAKE_CURRENT_BINARY_DIR}/external_json")


# Find all source files
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Find all header files
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Compile exe
add_executable(AIOneServer ${CORE_SOURCES})

# Include header files
target_include_directories(AIOneServer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(AIOneServer PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src")
target_link_libraries(AIOneServer PRIVATE AIOneCore) 
target_link_libraries(AIOneServer PRIVATE httplib::httplib)
target_link_libraries(AIOneServer PRIVATE nlohmann_json::nlohmann_json)