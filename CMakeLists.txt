cmake_minimum_required(VERSION 3.11)

set(LIBJOYCON_VERSION_MAJOR 0)
set(LIBJOYCON_VERSION_MINOR 0)
set(LIBJOYCON_VERSION_PATCH 3)
set(LIBJOYCON_VERSION ${LIBJOYCON_VERSION_MAJOR}.${LIBJOYCON_VERSION_MINOR}.${LIBJOYCON_VERSION_PATCH})

# Needed to link external math lib
cmake_policy(SET CMP0079 NEW)

project(joycon LANGUAGES C VERSION ${LIBJOYCON_VERSION})

include_directories(joycon ${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)
target_link_libraries(joycon PRIVATE m)

# TODO: Add public header
# set_target_properties(joycon PROPERTIES PUBLIC_HEADER libjoycon.h)

# Build Python binding if swig is detected
find_package(SWIG)
find_package(PythonLibs)
if(SWIG_FOUND AND PYTHONLIBS_FOUND)
    include(${SWIG_USE_FILE})
    include_directories(${PYTHON_INCLUDE_PATH})
    include_directories(${CMAKE_SOURCE_DIR}/include)

    swig_add_module(libjoycon python ${CMAKE_SOURCE_DIR}/swig/libjoycon.i)
    swig_link_libraries(libjoycon ${PYTHON_LIBRARIES} joycon)
endif()

if(BUILD_EXAMPLE)
    add_subdirectory(examples)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
