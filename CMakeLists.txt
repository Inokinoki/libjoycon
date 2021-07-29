cmake_minimum_required(VERSION 3.11)

set(LIBJOYCON_VERSION_MAJOR 0)
set(LIBJOYCON_VERSION_MINOR 0)
set(LIBJOYCON_VERSION_PATCH 2)
set(LIBJOYCON_VERSION ${LIBJOYCON_VERSION_MAJOR}.${LIBJOYCON_VERSION_MINOR}.${LIBJOYCON_VERSION_PATCH})

# Needed to link external math lib
cmake_policy(SET CMP0079 NEW)

project(joycon LANGUAGES C VERSION ${LIBJOYCON_VERSION})

include_directories(joycon ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)
target_link_libraries(joycon PRIVATE m)

# TODO: Add public header
# set_target_properties(joycon PROPERTIES PUBLIC_HEADER libjoycon.h)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
